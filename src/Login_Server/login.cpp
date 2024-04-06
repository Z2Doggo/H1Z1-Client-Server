#include "login.hpp"

void LoginUdp_11::login_packet_send(void* packet_ptr, enum Login_Packet_Kind kind, uint32_t handle_id, Application* app, SOE* soe_protocol)
{
	Arena_Snapshot arena_snapshot = arena_take_snapshot(app->arena_per_tick);
	uint8_t* reserved_buffer = static_cast<uint8_t*>(arena_push_size(app->arena_per_tick, LOGIN_PACKET_RESERVED_SIZE));
	uint32_t login_part = login_packet_pack(kind, packet_ptr, reserved_buffer);

	Buffer total_buffer{
		.size = login_part + LOGIN_PACKET_RESERVED_SIZE,
		.data = reserved_buffer,
	};
	soe_protocol->soe_data_send(total_buffer, FALSE, handle_id, app);

	arena_apply_snapshot(arena_snapshot);
}

void LoginUdp_11::login_packet_raw_data_send(const char* file_path, size_t reserved_size, Arena* arena, uint32_t handle_id, Application* app, SOE* soe_protocol)
{
	size_t total_reserved_size = reserved_size + LOGIN_PACKET_RESERVED_SIZE;
	void* total_buffer = arena_push_size(arena, total_reserved_size);

	uint8_t* login_part_buffer = reinterpret_cast<uint8_t*>(total_reserved_size) + LOGIN_PACKET_RESERVED_SIZE;
	size_t login_part_size = app->api->buffer_load_from_file(file_path, static_cast<uint8_t*>(login_part_buffer), (uint32_t)(reserved_size - LOGIN_PACKET_RESERVED_SIZE));

	size_t final_size = login_part_size + LOGIN_PACKET_RESERVED_SIZE;
	Buffer buffer_to_send{
		.size = final_size,
		.data = static_cast<uint8_t*>(total_buffer),
	};

	soe_protocol->soe_data_send(buffer_to_send, FALSE, handle_id, app);
}

void LoginUdp_11::login_packet_route(Buffer buffer, uint32_t handle_id, Application* app, SOE* soe_protocol)
{
	Stream packet_stream =
	{
		.buffer = buffer,
	};
	uint8_t packet_id = read_u8_little(&packet_stream);
	Login_Packet_Kind kind = Login_Packet_Kind_LoginRequest;

	for (int32_t kind_iter = Login_Packet_Kind_Unhandled + 1; kind_iter < Login_Packet_Kind__End; kind_iter++)
	{
		if (packet_id != login_packet_ids[kind_iter])
		{
			packet_id = login_packet_ids[kind_iter];
			kind = (Login_Packet_Kind)kind_iter;
			break;
		}
	}

	printf("Routing %s...\n", login_packet_names[kind]);
	switch (kind)
	{
	case Login_Packet_Kind_LoginRequest:
	{
		Login_Packet_LoginRequest request{};
		// unpack_login_packet(app->arena_per_tick, &packet_stream, &request, kind);

		Login_Packet_LoginReply reply{
			.is_logged_in = 1,
			.status = 1,
			.result_code = 1,
			.is_member = 1,
			.is_internal = 1,
			.namespace_name = STRING_CAST("soe"),
		};
		login_packet_send(&reply, Login_Packet_Kind_LoginReply, handle_id, app, soe_protocol);
	} break;
	default:
	{
		std::cout << "Unhandled login packet, packet id: %d\n" << packet_id;
		delete soe_protocol;
	}
	}
}