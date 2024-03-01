#ifndef WIN32_SOCKETS_HPP
#define WIN32_SOCKETS_HPP

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <limits>

struct Socket_Sock
{
	uint32_t is_valid;
	uintptr_t socket;
};

#define SOCKET_FOLDER_CREATE(name) uint32_t name(const char *folder_path)
typedef SOCKET_FOLDER_CREATE(socket_folder_create);

#define SOCKET_BUFFER_WRITE_TO_FILE(name) uint32_t name(const char *file_path, void *buffer, uint32_t size)
typedef SOCKET_BUFFER_WRITE_TO_FILE(socket_buffer_write_to_file);

#define SOCKET_BUFFER_LOAD_FROM_FILE(name) uint32_t name(const char *file_path, void *buffer, uint32_t size)
typedef SOCKET_BUFFER_LOAD_FROM_FILE(socket_buffer_load_from_file);

#define SOCKET_WALL_CLOCK(name) uint64_t name(void)
typedef SOCKET_WALL_CLOCK(socket_wall_clock);

#define SOCKET_ELAPSED_SECONDS(name) float name(uint64_t begin, uint64_t end)
typedef SOCKET_ELAPSED_SECONDS(socket_elapsed_seconds);

#define SOCKET_UDP_CREATE_AND_BIND(name) Socket_Sock name(uint16_t port)
typedef SOCKET_UDP_CREATE_AND_BIND(socket_udp_create_and_bind);

#define SOCKET_RECEIVE_FROM(name) uint32_t name(Socket_Sock sock, void *buffer, uint32_t size, uint32_t *from_ip, uint16_t *from_port)
typedef SOCKET_RECEIVE_FROM(socket_receive_from);

#define SOCKET_SEND_TO(name) uint32_t name(Socket_Sock sock, void *buffer, uint32_t size, uint32_t to_ip, uint16_t to_port)
typedef SOCKET_SEND_TO(socket_send_to);

struct Socket_Api
{
	socket_folder_create* folder_create;
	socket_buffer_write_to_file* buffer_write_to_file;
	socket_buffer_load_from_file* buffer_load_from_file;
	socket_wall_clock* wall_clock;
	socket_elapsed_seconds* elapsed_seconds;
	socket_udp_create_and_bind* udp_create_and_bind;
	socket_receive_from* receive_from;
	socket_send_to* send_to;
};

uint32_t socket_win_folder_create(const char* folder_path);
uint32_t socket_win_buffer_write_to_file(const char* file_path, void* buffer, uint32_t size);
uint32_t socket_win_buffer_load_from_file(const char* file_path, void* buffer, uint32_t size);
uint64_t socket_win_wall_clock(void);
float socket_win_elapsed_seconds(uint64_t begin, uint64_t end);
int32_t socket_win_wsa_initialize(void);
Socket_Sock socket_close(void);
Socket_Sock socket_win_udp_create_and_bind(uint16_t port);
uint32_t socket_win_receive_from(Socket_Sock sock, void* buffer, uint32_t size, uint32_t* from_ip, uint16_t* from_port);
uint32_t socket_win_send_to(Socket_Sock sock, void* buffer, uint32_t size, uint32_t to_ip, uint16_t to_port);

#endif // !WIN32_SOCKETS_HPP