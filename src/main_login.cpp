#define LOCAL_PORT 1115
#define MAX_SESSIONS_COUNT 16

#include "main_login.hpp"

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
		printf("[Server] - Login server bound to socket %d\n\n", LOCAL_PORT);
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

			Session_Address inc_session_addr{
				.part{
					.ip = from_ip,
					.port = from_port,
				},
			};

			Session_Handle session_handle = session_get_handle_from_address(&app->session_pool, inc_session_addr);
			Session_State* session = (Session_State*)arena_suballoc(app->arena_per_tick, KB(10)); // idk but it works
			if (!session_handle.id)
			{
				session_handle = session_acquire(&app->session_pool, inc_session_addr);
				session = session_get_pointer_from_handle(&app->session_pool, session_handle);

				session->args = {
					.crc_seed = 0,
					.crc_size = 0,
					.compression = 0,
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
				.size = receive_result,
				.data = inc_buffer,
			};

			core_packet_route(packet_buffer, false, session_handle, app);
			if (session->sequence_in > session->acked_in)
			{
				for (int32_t ack_iter = 0; ack_iter < (session->sequence_in - session->acked_in); ack_iter++)
				{
					session->acked_in++;
					Core_Packet_Ack ack{
						.sequence = (uint16_t)session->acked_in,
					};
					core_packet_send(&ack, Core_Packet_Kind_Ack, session_handle, app);
				}
			}

			printf("----------------------------------------- Packet Tick End ------------------------------------------\n");
		}
	}
}