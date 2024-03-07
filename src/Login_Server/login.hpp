#ifndef LOGIN_HPP
#define LOGIN_HPP

#include "../Packets/LoginUdp_11/login_packets.hpp"

void login_packet_send(void* packet_ptr, enum Login_Packet_Kind kind, Arena* arena, Session_Handle handle, App_State* app);
void login_packet_raw_data_send(const char* file_path, size_t reserved_size, Arena* arena, Session_Handle handle, App_State* app);
void login_packet_route(Buffer buffer, Session_Handle handle, App_State* app);

#endif // !LOGIN_HPP 