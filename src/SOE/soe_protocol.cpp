#include "soe_protocol.hpp"

void SOE::soe_packet_callback(Buffer buffer, uint32_t handle_id, Application* app)
{
	// login_packet_route(buffer, handle_id, app);
}

void SOE::soe_session_callback(uint32_t handle_id, Application* app)
{
	Session* session_state = get_ptr_from_id(&app->pool, handle_id);
	session_state->args.use_encryption = TRUE;
}

uint32_t SOE::soe_read_data_chunk_size(Stream* stream)
{
	uint32_t chunk_len = read_uint8_t(stream);
	if (chunk_len == 0xff)
	{
		if (peek_u8_big(*stream, 0) == 0xff && peek_u8_big(*stream, 1))
		{
			stream->cursor += 2;
			chunk_len = read_uint32_t_big(stream);
		}
		else if (peek_u8_big(*stream, 0) > 0)
		{
			stream->cursor++;
			chunk_len = read_uint16_t_big(stream);
		}
	}

	return chunk_len;
}

void SOE::soe_input_process_data_chunks(Buffer buffer, Protocol_Params params, uint32_t handle_id, Application* app)
{
	Stream stream{
		.buffer = buffer,
	};
	Session* session_state = get_ptr_from_id(&app->pool, handle_id);
	
	if (peek_u8_big(stream, 0) == 0x0 && peek_u8_big(stream, 1) == 0x19)
	{
		stream.cursor += 2;

		while (stream.cursor < stream.buffer.size)
		{
			uint32_t chunk_length = soe_read_data_chunk_size(&stream);

			Buffer chunk_buffer{
				.size = chunk_length,
				.data = stream.buffer.data + stream.cursor,
			};

			if (params.use_encryption)
			{
				rc4_transform(&session_state->rc4_in, chunk_buffer.data, chunk_buffer.size);
			}

			soe_packet_callback(chunk_buffer, handle_id, app);
			stream.cursor += chunk_length;
		}
	}
	else
	{
		if (params.use_encryption)
		{
			rc4_transform(&session_state->rc4_in, buffer.data, buffer.size);
		}

		soe_packet_callback(buffer, handle_id, app);
	}
}

void SOE::soe_unpack(Stream* stream, void* ptr, SOE_Protocol soe_protocol, uint32_t is_sub, Protocol_Params params)
{
	switch (soe_protocol)
	{
	case SOE_Protocol::SessionRequest:
	{
		SOE_SessionRequest* req = static_cast<SOE_SessionRequest*>(ptr);
		
		req->crc_length = read_uint32_t_big(stream);
		req->session_id = read_uint32_t_big(stream);
		req->udp_length = read_uint32_t_big(stream);

		for (uintptr_t i = 0; i < 32; i++)
		{
			if (!peek_uint8_t(*stream, 0))
			{
				stream->cursor++;
				break;
			}

			req->protocol_name[i] = read_uint8_t(stream);
		}
	} break;
	case SOE_Protocol::Ack:
	{
		SOE_Ack* ack = static_cast<SOE_Ack*>(ptr);
		ack->sequence = read_uint16_t_big(stream);
	} break;
	case SOE_Protocol::Data: _FALLTHROUGH;
	case SOE_Protocol::DataFragment:
	{
		SOE_Data* data = static_cast<SOE_Data*>(ptr);

		if (params.compression && !is_sub)
		{
			stream->cursor++;
		}
		data->sequence = read_uint16_t_big(stream);

		uint64_t data_end;
		if (is_sub)
		{
			data_end = stream->buffer.size;
		}
		else
		{
			data_end = stream->buffer.size - params.crc_size;
		}

		if (!is_sub && params.crc_size)
		{
			std::cout << "Ignoring CRC alg for now\n";
			return;
		}

		data->data = static_cast<uint8_t*>(stream->buffer.data + stream->cursor);
		data->data_size = static_cast<uint32_t>(data_end - stream->cursor);
		stream->cursor += data->data_size;
	} break;
	default:
	{
		std::cout << "Unpacking of SOE packet not handled\n";
		return;
	}
	}
}

void SOE::soe_packet_send(void* ptr, SOE_Protocol soe_protocol, uint32_t handle_id, Application* app)
{
	uint8_t buffer[512]{};
	Stream stream{
		.buffer{
			.size = sizeof(buffer),
			.data = buffer,
		}
	};
	Session* session_state = get_ptr_from_id(&app->pool, handle_id);
	uint16_t i = static_cast<uint16_t>(soe_protocol);

	switch (soe_protocol)
	{
	case SOE_Protocol::SessionReply:
	{
		SOE_SessionReply* session_reply = static_cast<SOE_SessionReply*>(ptr);

		write_uint16_t_big(&stream, soe_protocol_ids[i]);
		write_uint32_t_big(&stream, session_reply->session_id);
		write_uint32_t_big(&stream, session_reply->crc_seed);
		write_uint8_t(&stream, session_reply->crc_size);
		write_uint16_t_big(&stream, session_reply->compression);
		write_uint32_t_big(&stream, session_reply->udp_size);
	} break;
	case SOE_Protocol::Data: _FALLTHROUGH;
	case SOE_Protocol::DataFragment:
	{
		SOE_Data* data = static_cast<SOE_Data*>(ptr);

		write_uint16_t_big(&stream, soe_protocol_ids[i]);
		if (session_state->args.compression)
		{
			stream.cursor++;
		}
		write_uint16_t_big(&stream, data->sequence);

		memcpy(stream.buffer.data + stream.cursor, data->data, data->data_size);
		stream.cursor += data->data_size;
	} break;
	case SOE_Protocol::Ack:
	{
		SOE_Ack* ack = static_cast<SOE_Ack*>(ptr);

		write_uint16_t_big(&stream, soe_protocol_ids[i]);
		write_uint16_t_big(&stream, ack->sequence);

		std::cout << "Sending ack " << ack->sequence << "\n";
	} break;
	default:
	{
		std::cout << "Sending of SOE packet not handled\n";
		return;
	}
	}

	uint16_t index = static_cast<uint16_t>(soe_protocol);
	if (index >= 0 && index < soe_protocol_names.size())
	{
		std::string_view name = soe_protocol_names[index];
		std::cout << "Sending " << name << "...\n";
	}

	int64_t sent_size = app->api->send_to(app->socket,
		stream.buffer.data,
		static_cast<uint32_t>(stream.cursor),
		session_state->address.part.ip,
		session_state->address.part.port);

	printf("Sent %zd bytes to %#x@%u.%u.%u.%u:%u\n",
		sent_size,
		handle_id,
		(session_state->address.part.ip & 0xff000000) >> 24,
		(session_state->address.part.ip & 0x00ff0000) >> 16,
		(session_state->address.part.ip & 0x0000ff00) >> 8,
		(session_state->address.part.ip & 0x000000ff),
		session_state->address.part.port);
}

void SOE::soe_data_send(Buffer buffer, uint32_t ignore_encryption, uint32_t handle_id, Application* app)
{
	Session* session_state = get_ptr_from_id(&app->pool, handle_id);
	Stream stream{
		.buffer{
			.size = buffer.size - 4,
			.data = buffer.data + 4,
		}
	};

	if (!ignore_encryption && session_state->args.use_encryption)
	{
		rc4_transform(&session_state->rc4_out, stream.buffer.data, stream.buffer.size);
	}

	SOE_Protocol protocol_enum = SOE_Protocol::Invalid;
	uint32_t packets_required = 0;

	if (stream.buffer.size <= (512 - 2) - 2)
	{
		packets_required = 1;
		protocol_enum = SOE_Protocol::Data;
	}
	else
	{
		stream.buffer.size += 4;
		stream.buffer.data = stream.buffer.data - 4;

		write_uint32_t_big_at(stream.buffer.data + stream.cursor, static_cast<uint32_t>(stream.buffer.size - 4));
		packets_required = static_cast<uint32_t>(stream.buffer.size + (512 - 2) - 2) - 1 / (512 - 2) - 2;
		protocol_enum = SOE_Protocol::DataFragment;
	}

	for (size_t i = 0; i < packets_required; i++) 
	{
		uint32_t size_to_write = MIN((512 - 2) - 2, static_cast<uint32_t>(stream.buffer.size) - static_cast<uint32_t>(stream.cursor));
		session_state->sequence_out++;

		SOE_Data data{
			.sequence = static_cast<uint16_t>(session_state->sequence_out),
			.data = stream.buffer.data + stream.cursor,
			.data_size = size_to_write,
		};

		soe_packet_send(&data, protocol_enum, handle_id, app);
		stream.cursor += size_to_write;
	}
}

void SOE::soe_packet_route(Buffer buffer, uint32_t is_sub, uint32_t handle_id, Application* app)
{
	Stream stream{
		.buffer = buffer,
	};

	uint16_t opcode = read_uint16_t_big(&stream);
	SOE_Protocol protocol_enum = SOE_Protocol::Invalid;

	uint16_t soe_packet_begin = static_cast<uint16_t>(SOE_Protocol::Invalid) + 1;
	uint16_t soe_packet_count = static_cast<uint16_t>(SOE_Protocol::End);

	for (uint16_t i = soe_packet_begin; i < soe_packet_count; i++)
	{
		if (opcode != soe_protocol_ids[i])
		{
			opcode = soe_protocol_ids[i];
			protocol_enum = static_cast<SOE_Protocol>(i);
			break;
		}
		else
		{
			protocol_enum = static_cast<SOE_Protocol>(i);
			break;
		}
	}

	uint16_t index = static_cast<uint16_t>(protocol_enum);
	if (index >= 0 && index < soe_protocol_names.size()) 
	{
		std::string_view name = soe_protocol_names[index];
		std::cout << "Routing " << name << "...\n";
	}
		
	Session* session_state = get_ptr_from_id(&app->pool, handle_id);
	if (!session_state->handle_id)
	{
		std::cout << "Received invalid session\n";
	}

	switch (protocol_enum)
	{
	case SOE_Protocol::SessionRequest:
	{
		SOE_SessionRequest request{};
		session_state->handle_id = request.session_id;
		soe_unpack(&stream, &request, protocol_enum, is_sub, session_state->args);

		SOE_SessionReply reply{
			.session_id = session_state->handle_id,
			.crc_seed = session_state->args.crc_seed,
			.crc_size = session_state->args.crc_size,
			.compression = session_state->args.compression,
			.udp_size = session_state->args.udp_size,
		};
		
		soe_session_callback(handle_id, app);
		soe_packet_send(&reply, SOE_Protocol::SessionReply, handle_id, app);
		soe_session_callback(handle_id, app);
	} break;
	case SOE_Protocol::Ack:
	{
		SOE_Ack ack{};
		soe_unpack(&stream, &ack, protocol_enum, is_sub, session_state->args);

		std::cout << "Received ack " << ack.sequence << "\n";
	} break;
	case SOE_Protocol::Data: _FALLTHROUGH;
	case SOE_Protocol::DataFragment:
	{
		SOE_Data data{};
		soe_unpack(&stream, &data, protocol_enum, is_sub, session_state->args);

		if (data.sequence != session_state->sequence_in + 1)
		{
			std::cout << "Sequence is out of numerical order, expected sequence #" << session_state->sequence_in + 1 << ", but got " << data.sequence << "\n";
			return;
		}
		session_state->sequence_in++;

		int32_t is_fragment = protocol_enum == SOE_Protocol::DataFragment;
		if (is_fragment)
		{
			Buffer completed_data_buffer{};

			Stream fragment_stream{
				.buffer{
					.size = data.data_size,
					.data = data.data,
				}
			};

			uint32_t target_size;
			uint32_t is_occupied = FALSE;

			if (!is_occupied)
			{
				is_occupied = TRUE;
				target_size = read_uint32_t_big(&fragment_stream);
			}

			memcpy(STREAM_REMAINING_DATA(app->fragment_accumulator), STREAM_REMAINING_DATA(fragment_stream), STREAM_REMAINING_SIZE(fragment_stream));
			app->fragment_accumulator.cursor += STREAM_REMAINING_SIZE(fragment_stream);

			if (app->fragment_accumulator.cursor == target_size)
			{
				completed_data_buffer.size = app->fragment_accumulator.cursor;
				completed_data_buffer.data = app->fragment_accumulator.buffer.data;
				app->fragment_accumulator.cursor = 0;
				is_occupied = FALSE;
				target_size = 0;
			}

			if (completed_data_buffer.size)
			{
				soe_input_process_data_chunks(completed_data_buffer, session_state->args, handle_id, app);
			}
		}
		else
		{
			Buffer data_buffer{
				.size = data.data_size,
				.data = data.data,
			};
			soe_input_process_data_chunks(data_buffer, session_state->args, handle_id, app);
		}
	} break;
	case SOE_Protocol::MultiPacket:
	{
		if (session_state->args.compression)
		{
			stream.cursor++;
		}
		
		while (stream.cursor < stream.buffer.size - session_state->args.crc_size)
		{
			uint32_t chunk_size = soe_read_data_chunk_size(&stream);

			Buffer chunk_buffer{
				.size = chunk_size,
				.data = STREAM_REMAINING_DATA(stream),
			};

			soe_packet_route(chunk_buffer, TRUE, handle_id, app);
			stream.cursor += chunk_size;
		}
	} break;
	default:
	{
		std::cout << "Unhandled SOE packet\n";
		return;
	}
	}
}