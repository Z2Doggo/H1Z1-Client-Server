#ifndef CORE_PROTOCOL_HPP
#define CORE_PROTOCOL_HPP

#include "../app_memory.hpp"

#define MAX_PACKET_SIZE 512
#define CORE_PACKET_ID_SIZE 2
#define CORE_DATA_SEQUENCE_SIZE 2
#define CORE_DATA_FRAGMENT_EXTRA_SIZE 4
#define MAX_CORE_DATA_FRAGMENT_SIZE (MAX_PACKET_SIZE - CORE_PACKET_ID_SIZE - CORE_DATA_SEQUENCE_SIZE)

#define CORE_PACKET_KINDS \
	CORE_PACKET_KIND(Core_Packet_Kind_Invalid,           0xffff,  "INVALID"), \
	CORE_PACKET_KIND(Core_Packet_Kind_SessionRequest,    0x0001,  "SessionRequest"), \
	CORE_PACKET_KIND(Core_Packet_Kind_SessionReply,      0x0002,  "SessionReply"), \
	CORE_PACKET_KIND(Core_Packet_Kind_MultiPacket,       0x0003,  "MultiPacket"), \
	CORE_PACKET_KIND(Core_Packet_Kind_Disconnect,        0x0005,  "Disconnect"), \
	CORE_PACKET_KIND(Core_Packet_Kind_Ping,              0x0006,  "Ping"), \
	CORE_PACKET_KIND(Core_Packet_Kind_NetStatusRequest,  0x0007,  "NetStatusRequest"), \
	CORE_PACKET_KIND(Core_Packet_Kind_NetStatusReply,    0x0008,  "NetStatusReply"), \
	CORE_PACKET_KIND(Core_Packet_Kind_Data,              0x0009,  "Data"), \
	CORE_PACKET_KIND(Core_Packet_Kind_DataFragment,      0x000d,  "DataFragment"), \
	CORE_PACKET_KIND(Core_Packet_Kind_OutOfOrder,        0x0011,  "OutOfOrder"), \
	CORE_PACKET_KIND(Core_Packet_Kind_Ack,               0x0015,  "Ack"), \
	CORE_PACKET_KIND(Core_Packet_Kind_Unknown_1c,        0x001c,  "UNKNOWN_1C"), \
	CORE_PACKET_KIND(Core_Packet_Kind_FatalError,        0x001d,  "FatalError"), \
	CORE_PACKET_KIND(Core_Packet_Kind_FatalErrorReply,   0x001e,  "FatalErrorReply"), \
	CORE_PACKET_KIND(Core_Packet_Kind_Unknown_1f,        0x001f,  "UNKNOWN_1F"), \
	CORE_PACKET_KIND(Core_Packet_Kind__End,              0xffff,  "")

enum Core_Packet_Kind
{
#define CORE_PACKET_KIND(kind, id, str) kind
	CORE_PACKET_KINDS
#undef CORE_PACKET_KIND
};

static Core_Packet_Kind global_core_packet_kinds[Core_Packet_Kind__End + 1]
{
#define CORE_PACKET_KIND(kind, id, str) kind
		CORE_PACKET_KINDS
#undef CORE_PACKET_KIND
};

static uint16_t global_core_packet_ids[Core_Packet_Kind__End + 1]
{
#define CORE_PACKET_KIND(kind, id, str) id
		CORE_PACKET_KINDS
#undef CORE_PACKET_KIND
};

static const char* global_core_packet_names[Core_Packet_Kind__End + 1]
{
#define CORE_PACKET_KIND(kind, id, str) str
		CORE_PACKET_KINDS
#undef CORE_PACKET_KIND
};

struct Core_Packet_SessionRequest
{
	uint32_t crc_length;
	uint32_t session_id;
	uint32_t udp_length;
	char protocol_name[32];
};

struct Core_Packet_SessionReply
{
	uint32_t session_id;
	uint32_t crc_seed;
	uint8_t crc_size;
	uint8_t compression;
	uint32_t udp_size;
};

struct Core_Packet_Data
{
	uint8_t sequence;
	uint8_t* data;
	uint32_t data_size;
	uint16_t crc;
};

struct Core_Packet_Ack
{
	uint16_t sequence;
};

#define CORE_DATA_INPUT_CALLBACK(name)  void name(Buffer data_buffer, Session_Handle session_handle, App_State* app_state)
CORE_DATA_INPUT_CALLBACK(core_on_data_input); 

#define SESSION_ESTABLISHED_CALLBACK(name)  void name(Session_Handle session_handle, App_State * app_state)
SESSION_ESTABLISHED_CALLBACK(on_session_established);

uint32_t core_read_chunk_size(Stream* data_stream);
void core_input_data_process_chunks(Buffer data_buffer, Protocol_Args args, Session_Handle handle, App_State* app_state);
void core_packet_unpack(Stream* packet_stream, void* result_ptr, Core_Packet_Kind packet_kind, uint32_t is_sub_packet, Protocol_Args args);
void core_packet_send(void* packet_ptr, Core_Packet_Kind packet_kind, Session_Handle session_handle, App_State* app_state);
void core_data_send(Buffer data_buffer, uint32_t ignore_encryption, Session_Handle session_handle, App_State* app_state);
void core_packet_route(Buffer packet_buffer, uint32_t is_sub_packet, Session_Handle session_handle, App_State* app_state);

#endif // !CORE_PROTOCOL_HPP