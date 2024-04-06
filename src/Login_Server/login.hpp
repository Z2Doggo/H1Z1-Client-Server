#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "../main_login.hpp"
#include "../Packets/LoginUdp_11/LoginUdp_11.hpp"

#define LOGIN_PACKET_RESERVED_SIZE (CORE_DATA_FRAGMENT_EXTRA_SIZE)

struct LOGIN_LoginRequest
{
	std::string session_id;
	std::string system_fingerprint;
	uint32_t locale;
	uint32_t third_party_auth_ticket;
	uint32_t third_party_user_id;
	uint32_t third_party_id;
};

class LoginUdp_11
{
public:
	LOGIN_LoginRequest* login_request;

	void login_packet_send(void* packet_ptr, enum Login_Packet_Kind kind, uint32_t handle_id, Application* app, SOE* soe_protcol);
	void login_packet_raw_data_send(const char* file_path, size_t reserved_size, Arena* arena, uint32_t handle_id, Application* app, SOE* soe_protocol);
	void login_packet_route(Buffer buffer, uint32_t handle_id, Application* app, SOE* soe_protocol);
};

#endif // !LOGIN_HPP