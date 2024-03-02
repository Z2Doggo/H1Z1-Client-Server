#include "core_protocol.hpp"

static int global_core_packet_begin = Core_Packet_Kind_Invalid + 1;
static int global_core_packet_count = Core_Packet_Kind__End;

CORE_DATA_INPUT_CALLBACK(core_on_data_input)
{
	(void)data_buffer;
	(void)session_handle;
	(void)app_state;
}

SESSION_ESTABLISHED_CALLBACK(on_session_established)
{
	Session_State* session = session_get_pointer_from_handle(&app_state->session_pool, session_handle);
	session->args.use_encryption = true;
}

uint32_t core_read_chunk_size(Stream* data_stream)
{
	uint32_t chunk_length = read_u8_big(data_stream);
	if (chunk_length == 0xff)
	{
		if (peek_u8_big(*data_stream, 0) == 0xff && peek_u8_big(*data_stream, 1))
		{
			data_stream->cursor += 2;
			chunk_length = read_uint32_t_big(data_stream);
		}
		else if (peek_u8_big(*data_stream, 0) > 0)
		{
			data_stream->cursor++;
			chunk_length = read_uint16_t_big(data_stream);
		}
	}

	return chunk_length;
}

void core_input_data_process_chunks(Buffer data_buffer, Protocol_Args args, Session_Handle handle, App_State* app_state)
{
	Stream data_stream =
	{
		.buffer = data_buffer,
	};

	Session_State* session = session_get_pointer_from_handle(&app_state->session_pool, handle);

	if (peek_u8_big(data_stream, 0) == 0x0 && peek_u8_big(data_stream, 1) == 0x19)
	{
		data_stream.cursor += 2;

		while (data_stream.cursor < data_stream.buffer.size)
		{
			uint32_t chunk_length = core_read_chunk_size(&data_stream);

			Buffer chunk_buffer{
				.size = chunk_length,
				.data = data_stream.buffer.data + data_stream.cursor,
			};

			if (args.use_encryption)
			{
				rc4_transform(&session->rc4_in, chunk_buffer.data, chunk_buffer.size);
			}

			core_on_data_input(chunk_buffer, handle, app_state);
			data_stream.cursor += chunk_length;
		}
	}
	else
	{
		if (args.use_encryption)
		{
			rc4_transform(&session->rc4_in, data_buffer.data, data_buffer.size);
		}

		core_on_data_input(data_buffer, handle, app_state);
	}
}

Buffer core_input_data_accumulate(Core_Packet_Data data_packet, App_State* app_state)
{
	Buffer result{};

	Stream data_packet_stream =
	{
		.buffer =
		{
			.size = data_packet.data_size,
			.data = data_packet.data,
		},
	};

	static uint32_t target_size;
	static uint32_t is_occupied;
	if (!is_occupied)
	{
		is_occupied = true;
		target_size = read_uint32_t_big(&data_packet_stream);
	}
	memcpy(STREAM_REMAINING_DATA(app_state->fragment_accumulator), STREAM_REMAINING_DATA(data_packet_stream), STREAM_REMAINING_SIZE(data_packet_stream));
	app_state->fragment_accumulator.cursor += STREAM_REMAINING_SIZE(data_packet_stream);

	if (app_state->fragment_accumulator.cursor == target_size)
	{
		result.data = app_state->fragment_accumulator.buffer.data;
		result.size = app_state->fragment_accumulator.cursor;
		app_state->fragment_accumulator.cursor = 0;
		is_occupied = false;
		target_size = 0;
	}

	return result;
}

void core_input_data_intake(Core_Packet_Data data_packet, uint32_t is_fragment, Session_Handle session_handle, App_State* app_state)
{
	Session_State* session = session_get_pointer_from_handle(&app_state->session_pool, session_handle);
	if (data_packet.sequence != session->sequence_in + 1)
	{
		printf("Sequence out of order. expected %d, got %u\n", session->sequence_in + 1, data_packet.sequence);
		session->sequence_in++;
	}

	if (is_fragment)
	{
		Buffer completed_data_buffer = core_input_data_accumulate(data_packet, app_state);
		if (completed_data_buffer.size)
		{
			core_input_data_process_chunks(completed_data_buffer, session->args, session_handle, app_state);
		}
	}
	else
	{
		Buffer data_buffer =
		{
			.size = data_packet.data_size,
			.data = data_packet.data,
		};
		core_input_data_process_chunks(data_buffer, session->args, session_handle, app_state);
	}
}

void core_packet_unpack(Stream* packet_stream, void* result_ptr, Core_Packet_Kind packet_kind, uint32_t is_sub_packet, Protocol_Args args)
{
	switch (packet_kind)
	{
	case Core_Packet_Kind_SessionRequest:
	{
		Core_Packet_SessionRequest* session_request = (Core_Packet_SessionRequest*)result_ptr;
		session_request->crc_length = read_uint32_t_big(packet_stream);
		session_request->session_id = read_uint32_t_big(packet_stream);
		session_request->udp_length = read_uint32_t_big(packet_stream);

		for (uintptr_t i = 0; i < 32; i++)
		{
			if (!peek_uint8_t(*packet_stream, 0))
			{
				packet_stream->cursor++;
				break;
			}

			session_request->protocol_name[i] = read_uint8_t(packet_stream);
		}
	} break;

	case Core_Packet_Kind_Ack:
	{
		Core_Packet_Ack* ack_packet = (Core_Packet_Ack*)result_ptr;
		ack_packet->sequence = read_uint16_t_big(packet_stream);

	} break;

	case Core_Packet_Kind_Data:
	case Core_Packet_Kind_DataFragment:
	{
		Core_Packet_Data* data_packet = (Core_Packet_Data*)result_ptr;
		if (args.compression && !is_sub_packet)
		{
			packet_stream->cursor++;
		}

		data_packet->sequence = read_u8_big(packet_stream);

		int64_t data_end;
		if (is_sub_packet)
		{
			data_end = packet_stream->buffer.size;
		}
		else
		{
			data_end = packet_stream->buffer.size - args.crc_size;
		}

		if (!is_sub_packet && args.crc_size)
		{
			printf("ignoring crc for now");
			return;
		}

		data_packet->data = (uint8_t*)packet_stream->buffer.data + packet_stream->cursor;
		data_packet->data_size = (uint32_t)(data_end - packet_stream->cursor);
		packet_stream->cursor += data_packet->data_size;
	} break;
	default:
	{
		printf("unpacking not handled");
		return;
	}
	}
}

void core_packet_send(void* packet_ptr, Core_Packet_Kind packet_kind, Session_Handle session_handle, App_State* app_state)
{
	uint8_t buffer[MAX_PACKET_SIZE]{};
	Stream output_stream =
	{
		.buffer =
		{
			.size = sizeof(buffer),
			.data = buffer,
		},
	};

	Session_State* session = session_get_pointer_from_handle(&app_state->session_pool, session_handle);
	switch (packet_kind)
	{
	case Core_Packet_Kind_SessionReply:
	{
		Core_Packet_SessionReply* session_reply = (Core_Packet_SessionReply*)packet_ptr;

		write_uint16_t_big(&output_stream, global_core_packet_ids[packet_kind]);
		write_uint32_t_big(&output_stream, session_reply->session_id);
		write_uint32_t_big(&output_stream, session_reply->crc_seed);
		write_u8_big(&output_stream, session_reply->crc_size);
		write_uint16_t_big(&output_stream, session_reply->compression);
		write_uint32_t_big(&output_stream, session_reply->udp_size);
	} break;

	case Core_Packet_Kind_Data:
	case Core_Packet_Kind_DataFragment:
	{
		Core_Packet_Data* data_packet = (Core_Packet_Data*)packet_ptr;

		write_uint16_t_big(&output_stream, global_core_packet_ids[packet_kind]);
		if (session->args.compression)
		{
			output_stream.cursor++;
		}
		write_uint16_t_big(&output_stream, data_packet->sequence);
		memcpy((uint8_t*)output_stream.buffer.data + output_stream.cursor, data_packet->data, data_packet->data_size);
		output_stream.cursor += data_packet->data_size;
	} break;

	case Core_Packet_Kind_Ack:
	{
		Core_Packet_Ack* ack_packet = (Core_Packet_Ack*)packet_ptr;

		write_uint16_t_big(&output_stream, global_core_packet_ids[packet_kind]);
		write_uint16_t_big(&output_stream, ack_packet->sequence);

		printf("Sending ack %d\n", ack_packet->sequence);
	} break;

	default:
	{
		printf("sending not handled");
		return;
	}
	}

	printf("Sending %s...\n", global_core_packet_names[packet_kind]);
	int64_t sent_size = app_state->api->send_to(app_state->socket,
		output_stream.buffer.data,
		(uint32_t)output_stream.cursor,
		session->addr.part.ip,
		session->addr.part.port);

	printf("Sent %zd bytes to %#x@%u.%u.%u.%u:%u\n",
		sent_size,
		session_handle.id,
		(session->addr.part.ip & 0xff000000) >> 24,
		(session->addr.part.ip & 0x00ff0000) >> 16,
		(session->addr.part.ip & 0x0000ff00) >> 8,
		(session->addr.part.ip & 0x000000ff),
		session->addr.part.port);
}

void protocol_core_data_send(Buffer data_buffer, uint32_t ignore_encryption, Session_Handle session_handle, App_State* app_state)
{
	Session_State* session = session_get_pointer_from_handle(&app_state->session_pool, session_handle);


	Stream output_stream =
	{
		.buffer =
		{
			.size = data_buffer.size - CORE_DATA_FRAGMENT_EXTRA_SIZE,
			.data = data_buffer.data + CORE_DATA_FRAGMENT_EXTRA_SIZE,
		}
	};

	if (!ignore_encryption && session->args.use_encryption)
	{
		rc4_transform(&session->rc4_out, output_stream.buffer.data, output_stream.buffer.size);
	}

	Core_Packet_Kind packet_kind = Core_Packet_Kind_Invalid;
	uint32_t packets_required = 0;
	if (output_stream.buffer.size <= MAX_CORE_DATA_FRAGMENT_SIZE)
	{
		packets_required = 1;
		packet_kind = Core_Packet_Kind_Data;
	}
	else
	{
		output_stream.buffer.size += CORE_DATA_FRAGMENT_EXTRA_SIZE;
		output_stream.buffer.data = (uint8_t*)((uint64_t)output_stream.buffer.data - CORE_DATA_FRAGMENT_EXTRA_SIZE);

		write_uint32_t_big((Stream*)((uintptr_t)output_stream.buffer.data + output_stream.cursor), (uint32_t)output_stream.buffer.size - CORE_DATA_FRAGMENT_EXTRA_SIZE);

		packets_required = ((uint32_t)output_stream.buffer.size + MAX_CORE_DATA_FRAGMENT_SIZE - 1) / MAX_CORE_DATA_FRAGMENT_SIZE;
		packet_kind = Core_Packet_Kind_DataFragment;
	}

	for (int64_t packet_iter = 0; packet_iter < packets_required; packet_iter++)
	{
		uint32_t size_to_write = MIN(MAX_CORE_DATA_FRAGMENT_SIZE, (uint32_t)output_stream.buffer.size - (uint32_t)output_stream.cursor);

		session->sequence_out++;
		Core_Packet_Data packet =
		{
			.sequence = (uint8_t)session->sequence_out,
			.data = output_stream.buffer.data + output_stream.cursor,
			.data_size = size_to_write,
		};
		core_packet_send(&packet, packet_kind, session_handle, app_state);
		output_stream.cursor += size_to_write;
	}
}

void protocol_core_packet_route(Buffer packet_buffer, uint32_t is_sub_packet, Session_Handle session_handle, App_State* app_state)
{
	Stream packet_stream =
	{
		.buffer = packet_buffer,
	};

	uint16_t packet_id = read_uint16_t_big(&packet_stream);
	Core_Packet_Kind packet_kind = Core_Packet_Kind_Invalid;

	for (int kind_iter = global_core_packet_begin; kind_iter < global_core_packet_count; kind_iter++)
	{
		if (packet_id == global_core_packet_ids[kind_iter])
		{
			packet_kind = (Core_Packet_Kind)kind_iter;
			break;
		}
	}
	if (!packet_kind)
	{
		printf("Invalid core packet");
	}

	printf("Routing %s...\n", global_core_packet_names[packet_kind]);
	Session_State* session = session_get_pointer_from_handle(&app_state->session_pool, session_handle);

	if (!session->handle.id)
	{
		printf("Got invalid session");
	}

	switch (packet_kind)
	{
	case Core_Packet_Kind_SessionRequest:
	{
		Core_Packet_SessionRequest packet{};
		core_packet_unpack(&packet_stream, &packet, packet_kind, is_sub_packet, session->args);

		session->id = packet.session_id;

		if (!packet.protocol_name[0])
		{
			session->kind = Session_Kind_Ping_Responder;
		}

		Core_Packet_SessionReply reply =
		{
			.session_id = session->id,
			.crc_seed = session->args.crc_seed,
			.crc_size = session->args.crc_size,
			.compression = (uint8_t)session->args.compression,
			.udp_size = session->args.udp_size,
		};
		core_packet_send(&reply, Core_Packet_Kind_SessionReply, session_handle, app_state);

		on_session_established(session_handle, app_state);
	} break;
	case Core_Packet_Kind_Ack:
	{
		Core_Packet_Ack ack_packet{};
		core_packet_unpack(&packet_stream, &ack_packet, packet_kind, is_sub_packet, session->args);

		printf("Received ack %d\n", ack_packet.sequence);

	} break;
	case Core_Packet_Kind_Data:
	case Core_Packet_Kind_DataFragment:
	{
		Core_Packet_Data packet{};
		core_packet_unpack(&packet_stream, &packet, packet_kind, is_sub_packet, session->args);

		if (packet.sequence != session->sequence_in + 1)
		{
			printf("Sequence out of order. expected %d, got %u\n", session->sequence_in + 1, packet.sequence);
			session->sequence_in++;
		}

		uint32_t is_fragment = packet_kind == Core_Packet_Kind_DataFragment;
		if (is_fragment)
		{
			Buffer completed_data_buffer{};

			Stream data_stream =
			{
				.buffer =
				{
					.size = packet.data_size,
					.data = packet.data,
				},
			};

			static uint32_t target_size;
			static uint32_t is_occupied;
			if (!is_occupied)
			{
				is_occupied = TRUE;
				target_size = read_uint32_t_big(&data_stream);
			}
			memcpy(STREAM_REMAINING_DATA(app_state->fragment_accumulator), STREAM_REMAINING_DATA(data_stream), STREAM_REMAINING_SIZE(data_stream));
			app_state->fragment_accumulator.cursor += STREAM_REMAINING_SIZE(data_stream);

			if (app_state->fragment_accumulator.cursor == target_size)
			{
				completed_data_buffer.data = app_state->fragment_accumulator.buffer.data;
				completed_data_buffer.size = app_state->fragment_accumulator.cursor;
				app_state->fragment_accumulator.cursor = 0;
				is_occupied = FALSE;
				target_size = 0;
			}

			if (completed_data_buffer.size)
			{
				core_input_data_process_chunks(completed_data_buffer, session->args, session_handle, app_state);
			}
		}
		else
		{
			Buffer data_buffer{
				.size = packet.data_size,
				.data = packet.data,
			};
			core_input_data_process_chunks(data_buffer, session->args, session_handle, app_state);
		}

	} break;

	case Core_Packet_Kind_MultiPacket:
	{
		if (session->args.compression)
		{
			packet_stream.cursor++;
		}

		while (packet_stream.cursor < packet_stream.buffer.size - session->args.crc_size)
		{
			uint32_t chunk_size = core_read_chunk_size(&packet_stream);

			Buffer chunk_buffer =
			{
				.size = chunk_size,
				.data = STREAM_REMAINING_DATA(packet_stream),
			};

			protocol_core_packet_route(chunk_buffer, TRUE, session_handle, app_state);
			packet_stream.cursor += chunk_size;
		}
	} break;

	default:
	{
		printf("Unhandled packet\n");
		return;
	}
	}
}