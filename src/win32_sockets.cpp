#include "win32_sockets.hpp"

static uint64_t global_freq;

uint32_t socket_win_folder_create(const char* folder_path)
{
	if (!CreateDirectoryA(folder_path, nullptr))
	{
		DWORD last_error = GetLastError();
		if (last_error != 183L)
		{
			printf("[!] CreateDirectory error on \"%s\" - Error: %lu\n", folder_path, last_error);
			return 0;
		}
	}

	return 1;
}

uint32_t socket_win_buffer_write_to_file(const char* file_path, void* buffer, uint32_t size)
{
	HANDLE file_handle = CreateFileA(file_path,
		FILE_APPEND_DATA,
		0,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (file_handle == INVALID_HANDLE_VALUE)
	{
		printf("[!] Unable to open file to write \"%s\" - Error: %lu\n", file_path, GetLastError());
		return 0;
	}

	if (!buffer)
	{
		printf("[!] Buffer to write from is null\n");
		CloseHandle(file_handle);
		return 0;
	}

	DWORD bytes_written;
	if (!WriteFile(file_handle,
		buffer,
		size,
		&bytes_written,
		0))
	{
		printf("[!] Unable to write to file \"%s\" - Error: %lu\n", file_path, GetLastError());
		CloseHandle(file_handle);
		return 0;
	}

	CloseHandle(file_handle);
	return bytes_written;
}

uint32_t socket_win_buffer_load_from_file(const char* file_path, void* buffer, uint32_t size)
{
	HANDLE file_handle = CreateFileA(file_path,
		GENERIC_READ,
		0,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (file_handle == INVALID_HANDLE_VALUE)
	{
		printf("[!] Unable to open file to read \"%s\" - Error: %lu\n", file_path, GetLastError());
		return 0;
	}

	if (!buffer)
	{
		printf("[!] Buffer to load to is null\n");
		CloseHandle(file_handle);
		return 0;
	}

	LARGE_INTEGER file_size;
	if (!GetFileSizeEx(file_handle, &file_size))
	{
		printf("[!] Unable to get size of \"%s\" - Error: %lu\n", file_path, GetLastError());
		CloseHandle(file_handle);
		return 0;
	}

	if (file_size.QuadPart > 0xffffffff)
	{
		printf("[!] File \"%s\" is too large to load\n", file_path);
		CloseHandle(file_handle);
		return 0;
	}

	if (file_size.LowPart > size)
	{
		printf("[!] Buffer is too small to load from file \"%s\"\n", file_path);
		CloseHandle(file_handle);
		return 0;
	}

	DWORD bytes_read;
	if (!ReadFile(file_handle,
		buffer,
		file_size.LowPart,
		&bytes_read,
		NULL))
	{
		printf("[!] Unable to load from file \"%s\" - Error: %lu\n", file_path, GetLastError());
		CloseHandle(file_handle);
		return 0;
	}

	CloseHandle(file_handle);
	return bytes_read;
}

uint64_t socket_win_wall_clock(void)
{
	LARGE_INTEGER result;
	QueryPerformanceCounter(&result);
	return result.QuadPart;
}

float socket_win_elapsed_seconds(uint64_t begin, uint64_t end)
{
	float result = (float)(end - begin) / (float)global_freq;

	return result;
}

int32_t socket_win_wsa_initialize(void)
{
	WSADATA wsa_data;
	int32_t error = WSAStartup(MAKEWORD(2, 2), &wsa_data);
	if (error)
	{
		printf("[!] WSAStartup failed - Error: %d\n", error);
		return 0;
	}

	return 1;
}

Socket_Sock socket_close(void)
{
	Socket_Sock result{};
	if (closesocket(result.socket) == SOCKET_ERROR)
	{
		printf("[!] closesocket() failed: %d\n", WSAGetLastError());
	}

	return result;
}

Socket_Sock socket_win_udp_create_and_bind(uint16_t port)
{
	int is_wsa_initialized = 0;
	if (!is_wsa_initialized)
	{
		is_wsa_initialized = socket_win_wsa_initialize();
	}

	Socket_Sock result{};
	result.socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (result.socket == INVALID_SOCKET)
	{
		printf("[!] socket() failed: %d\n", WSAGetLastError());
		return result;
	}

	u_long cmd_arg = 1;
	if (ioctlsocket(result.socket, FIONBIO, &cmd_arg) == SOCKET_ERROR)
	{
		printf("[!] ioctlsocket() failed: %d\n", WSAGetLastError());
		socket_close();
	}

	SOCKADDR_IN addr{};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int bind_result = bind(result.socket, (SOCKADDR*)&addr, sizeof(addr));
	if (bind_result == SOCKET_ERROR)
	{
		printf("[!] bind() failed: %d\n", WSAGetLastError());
		socket_close();
	}

	result.is_valid = 1;
	return result;
}

uint32_t socket_win_receive_from(Socket_Sock sock, void* buffer, uint32_t size, uint32_t* from_ip, uint16_t* from_port)
{
	SOCKADDR_IN from_address = { 0 };
	int from_address_size = sizeof(from_address);

	int result = recvfrom(sock.socket,
		static_cast<char*>(buffer),
		size,
		0,
		(struct sockaddr*)&from_address,
		&from_address_size);

	if (result == SOCKET_ERROR)
	{
		int last_error = WSAGetLastError();
		if (last_error != WSAEWOULDBLOCK)
		{
			printf("[!] recvfrom() failed: %d\n", last_error);
		}
		return 0;
	}
	else if (!result)
	{
		printf("[!] recvfrom() returned 0\n");
		return 0;
	}

	*from_ip = ntohl(from_address.sin_addr.s_addr);
	*from_port = ntohs(from_address.sin_port);
	return result;
}

uint32_t socket_win_send_to(Socket_Sock sock, void* buffer, uint32_t size, uint32_t to_ip, uint16_t to_port)
{
	SOCKADDR_IN to_address{};
	to_address.sin_family = AF_INET;
	to_address.sin_addr.s_addr = htonl(to_ip);
	to_address.sin_port = htons(to_port);

	int result = sendto((SOCKET)sock.socket,
		(const char*)buffer,
		size,
		0,
		(SOCKADDR*)&to_address,
		sizeof(to_address));

	if (result == SOCKET_ERROR)
	{
		printf("[!] sendto() failed: %d\n", WSAGetLastError());
		return 0;
	}

	return result;
}