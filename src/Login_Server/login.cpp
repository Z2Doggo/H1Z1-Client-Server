#include "login.hpp"

void login_packet_send(void* packet_ptr, enum Login_Packet_Kind kind, Arena* arena, Session_Handle handle, App_State* app)
{
	Arena_Snapshot arena_snapshot = arena_take_snapshot(arena);
	void* reserved_buffer = arena_push_size(arena, LOGIN_PACKET_RESERVED_SIZE);
	Buffer login_part = pack_login_packet(arena, packet_ptr, kind);

	Buffer total_buffer{
		.size = login_part.size + LOGIN_PACKET_RESERVED_SIZE,
		.data = (uint8_t*)reserved_buffer,
	};
	core_data_send(total_buffer, 0, handle, app);
	arena_apply_snapshot(arena_snapshot);
}

void login_packet_raw_data_send(const char* file_path, size_t reserved_size, Arena* arena, Session_Handle handle, App_State* app)
{
	size_t total_reserved_size = reserved_size + LOGIN_PACKET_RESERVED_SIZE;
	void* total_buffer = arena_push_size(arena, total_reserved_size);

	uint8_t* login_part_buffer = (uint8_t*)total_reserved_size + LOGIN_PACKET_RESERVED_SIZE;
	size_t login_part_size = app->api->buffer_load_from_file(file_path, (uint8_t*)login_part_buffer, (uint32_t)(reserved_size - LOGIN_PACKET_RESERVED_SIZE));

	size_t final_size = login_part_size + LOGIN_PACKET_RESERVED_SIZE;
	Buffer buffer_to_send{
		.size = final_size,
		.data = (uint8_t*)total_buffer,
	};

	core_data_send(buffer_to_send, 0, handle, app);
}

void login_packet_route(Buffer buffer, Session_Handle handle, App_State* app)
{
	Stream packet_stream =
	{
		.buffer = buffer,
	};
	uint8_t packet_id = read_u8_little(&packet_stream);
	Login_Packet_Kind kind = Login_Packet_Kind_LoginRequest;

	for (int32_t kind_iter = Login_Packet_Kind_Unhandled + 1; kind_iter < Login_Packet_Kind__End; kind_iter++)
	{
		if (packet_id == login_packet_ids[kind_iter])
		{
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
			.namespace_name = BUNDLE_STRING("soe"),
		};
		login_packet_send(&reply, Login_Packet_Kind_LoginReply, app->arena_per_tick, handle, app);
	} break;
	default: 
	{
		printf("Unhandled login packet, packet id: %d\n", packet_id);
	}
	}
}