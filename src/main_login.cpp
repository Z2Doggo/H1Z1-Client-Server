#include "Login_Server/login.hpp"

extern "C" __declspec(dllexport) APP_TICK(server_tick)
{
	Application* app = memory->app;
	if (!app)
	{
		app = memory->app = static_cast<Application*>(arena_push_size(memory->arena_total, sizeof(Application)));
		app->api = &memory->api;
		app->arena_per_tick = arena_suballoc(memory->arena_total, MB(10));

		uint8_t rc4_key_encoded[] = "F70IaxuU8C/w7FPXY1ibXw==";
		app->rc4_key_decoded_len = base64_decode(rc4_key_encoded, sizeof(rc4_key_encoded) - 1, app->rc4_key_decoded);
		app->pool = memory->create_session(memory->arena_total, MAX_SESSIONS_COUNT);

		app->fragment_accumulator =
		{ 
			.buffer{
				.size = MB(10),
				.data = static_cast<uint8_t*>(arena_push_size(memory->arena_total, MB(10))),
			},
		};
		app->socket = app->api->udp_create_and_bind(LOCAL_PORT);
		std::cout << "[Login Server] - Bound to socket " << LOCAL_PORT << "\n";
	}

    DEFER_SCOPE(0, arena_reset(app->arena_per_tick)) 
    {
        uint8_t incoming_buffer[MAX_PACKET_SIZE]{};
        uint32_t from_ip;
        uint16_t from_port;
        int32_t receive_result = app->api->receive_from(app->socket, incoming_buffer, MAX_PACKET_SIZE, &from_ip,  &from_port);

        if (receive_result) 
        {
            std::cout << "\n\n________________________________________ Packet Tick Begin _________________________________________\n";

            Address incoming_session_address{
                .part{
                    .ip = from_ip,
                    .port = from_port,
                },
            };

            Session* session = nullptr;
            uint32_t session_handle = memory->get_id_from_address(&app->pool, incoming_session_address);
            if (!session_handle) 
            {
                session_handle = memory->acquire_session(&app->pool, incoming_session_address);
                session = memory->get_ptr_from_id(&app->pool, session_handle);

                session->args = {
                    .crc_seed = 0,
                    .crc_size = 0,
                    .compression = 0,
                    .udp_size = MAX_PACKET_SIZE,
                    .use_encryption = FALSE,
                };
                session->acked_in = -1;
                session->sequence_in = -1;
                session->sequence_out = -1;
                rc4_init(&session->rc4_in, app->rc4_key_decoded,  app->rc4_key_decoded_len);
                rc4_init(&session->rc4_out,  app->rc4_key_decoded, app->rc4_key_decoded_len);
            }

            if (!session) 
            {
                session = memory->get_ptr_from_id(&app->pool, session_handle);
            }

            Buffer packet_buffer = { 
                .size = static_cast<uintptr_t>(receive_result),               
                .data = incoming_buffer 
            };
            memory->soe_packet_route(packet_buffer, FALSE, session_handle, app, memory->soe_protocol);
            
            if (&session->sequence_in > &session->acked_in) 
            {
                for (int32_t ack_iter = 0; ack_iter < (&session->sequence_in - &session->acked_in);  ack_iter++) 
                {
                    session->acked_in++;
                    SOE_Ack ack 
                    {
                        .sequence = static_cast<uint16_t>(session->acked_in),
                    };
                    memory->soe_packet_send(&ack, SOE_Protocol::Ack, session_handle, app);
                }
            }

            std::cout << "----------------------------------------- Packet Tick End ------------------------------------------\n";
        }
    }
}