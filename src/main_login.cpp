#define LOCAL_PORT 1115
#define MAX_SESSIONS_COUNT 16

#include "SOE/session.hpp"
#include "SOE/core_protocol.hpp"

struct App_State
{
	Socket_Api *api;
	Socket_Sock socket;
	Arena* arena_per_tick;
	size_t rc4_key_decoded_len;
	unsigned char rc4_key_decoded[256];
	Session_Pool session_pool;
	Stream fragment_accumulator;
}; 

extern "C" __declspec(dllexport) APP_TICK(server_tick)
{
	App_State* app = memory->app;
	if (!app)
	{
		app = memory->app = (App_State*)arena_push_struct(memory->arena_total, App_State);
		app->api = &memory->api;
		app->arena_per_tick = arena_suballoc(memory->arena_total, MB(10));

		uint8_t rc4_key_encoded[] = "F70IaxuU8C/w7FPXY1ibXw==";
		app->rc4_key_decoded_len = base64_decode(rc4_key_encoded, sizeof(rc4_key_encoded) - 1, app->rc4_key_decoded);
		app->session_pool = session_pool_create(memory->arena_total, MAX_SESSIONS_COUNT);

		app->fragment_accumulator =
		{ 
			.buffer{
				.size = MB(1),
				.data = (uint8_t*)arena_push_size(memory->arena_total, MB(1)),
			},
		};
		app->socket = app->api->udp_create_and_bind(LOCAL_PORT);
		printf("[Server] - Login server bound to socket 1115\n\n");
	}

	DEFER_SCOPE(0, arena_reset(app->arena_per_tick))
	{
		uint8_t inc_buffer[MAX_PACKET_SIZE]{};
		uint32_t from_ip;
		uint16_t from_port;

		uint32_t receive_result = app->api->receive_from(app->socket, inc_buffer, MAX_PACKET_SIZE, &from_ip, &from_port);
		if (receive_result)
		{
			printf("\n\n________________________________________ Packet Tick Begin _________________________________________\n");

			Session_Address inc_session_addr
			{
				.part
				{
					.ip = from_ip,
					.port = from_port,
				},
			};

			Session_Handle session_handle = session_get_handle_from_address(&app->session_pool, inc_session_addr);
			if (!session_handle.id)
			{
				session_handle = session_acquire(&app->session_pool, inc_session_addr);
				Session_State* session = session_get_pointer_from_handle(&app->session_pool, session_handle);

				session->args = {
					.crc_seed = 0,
					.crc_size = 0,
					.compression = 0 ,
					.udp_size = MAX_PACKET_SIZE,
					.use_encryption = false,
				};
				session->acked_in = -1;
				session->sequence_in = -1;
				session->sequence_out = -1;

				rc4_init(&session->rc4_in, app->rc4_key_decoded, app->rc4_key_decoded_len);
				rc4_init(&session->rc4_out, app->rc4_key_decoded, app->rc4_key_decoded_len);
			}

			Buffer packet_buffer{
				.size = (size_t)receive_result,
				.data = inc_buffer,
			};

			/* TODO: FINISH THE CORE_PROTOCOL THEN FINISH THE DLL ENTRY */
		}
	}
}