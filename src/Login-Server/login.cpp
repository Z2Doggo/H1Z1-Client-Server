#include "login.hpp"

void login_packet_send(void* packet_ptr, Login_Packet_Kind kind, Arena* arena, Session_Handle handle, App_State* app)
{
	uint8_t* reserved_buffer = (uint8_t*)arena_push_size(arena, LOGIN_PACKET_RESERVED_SIZE);
	Buffer login_part = pack_login_packet(arena, packet_ptr, kind);

	Buffer total_buffer{
	  .size = login_part.size + LOGIN_PACKET_RESERVED_SIZE,
	  .data = reserved_buffer,
	};
	core_data_send(total_buffer, false, handle, app);
}

void login_packet_raw_data_send(const char* file_path, size_t reserved_size, Arena* arena, Session_Handle handle, App_State* app)
{
	size_t total_reserved_size = reserved_size + LOGIN_PACKET_RESERVED_SIZE;
	uint8_t* total_buffer = (uint8_t*)arena_push_size(arena, total_reserved_size);

	uint8_t* login_part_buffer = (uint8_t*)total_reserved_size + LOGIN_PACKET_RESERVED_SIZE;
	size_t login_part_size = app->api->buffer_load_from_file(file_path, (uint8_t*)login_part_buffer, (uint32_t)(reserved_size - LOGIN_PACKET_RESERVED_SIZE));

	size_t final_size = login_part_size + LOGIN_PACKET_RESERVED_SIZE;
	Buffer buffer_to_send{
		.size = final_size,
		.data = total_buffer,
	};

	core_data_send(buffer_to_send, false, handle, app);
}

void login_packet_route(Buffer buffer, Session_Handle handle, App_State* app)
{
	Stream packet_stream{
		.buffer = buffer,
	};
	uint8_t packet_id = read_u8_little(&packet_stream);

	// login_packet_raw_data_send("../src/Login-Server/raw_login_reply_data.bin", KB(20), app->arena_per_tick, handle, app);
}