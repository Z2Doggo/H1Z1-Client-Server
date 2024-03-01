#ifndef SESSION_HPP
#define SESSION_HPP

#include "../app_memory.hpp"

#define SESSION_MAP_OVERHEAD(value) (value * 4)
#define SESSION_MAP_MAX_COLLISION 3

#define SESSION_HANDLE_GENERATION_READ(handle) (handle >> 19)
#define SESSION_HANDLE_GENERATION_WRITE(handle, value) handle |= (value << 19)
#define SESSION_HANDLE_INDEX_READ(handle) (handle & 0x3FFF)
#define SESSION_HANDLE_INDEX_WRITE(handle, value) handle |= (value & 0x3FFF)

struct Session_Handle
{
	unsigned int id;
};

enum Session_Kind
{
	Session_Kind_Unhandled = 0,
	Session_Kind_Ping_Responder,
};

union Session_Address
{
	uint64_t full;
	struct Part
	{
		uint32_t ip;
		uint16_t port;
	} part;
};

struct Protocol_Args
{
	uint32_t crc_seed;
	uint8_t crc_size;
	uint16_t compression;
	uint32_t udp_size;
	uint32_t use_encryption;
};

struct Session_State
{
	// Session related
	Session_Handle handle;
	Session_Address addr;
	Protocol_Args args;

	// Fragment related
	int32_t acked_in;
	int32_t sequence_in;
	int32_t sequence_out;

	// Encryption related
	RC4 rc4_in;
	RC4 rc4_out;

	// Client related
	uint32_t id;
	Session_Kind kind;
	uint64_t guid;
};

enum Session_Slot_Kind
{
	Session_Slot_Kind_Invalid = 0,
	Session_Slot_Kind_Free,
	Session_Slot_Kind_Active,
};

struct Session_Slot
{
	uint32_t generation;
	Session_Slot_Kind kind;
	union Session_Content
	{
		Session_State state;
		uint32_t next_index;
	} session_content;
};

struct Session_Address_Bucket
{
	Session_Address key;
	Session_Handle value;
};

struct Session_Pool
{
	uint32_t active_count;
	uint32_t free_head_index;
	uint32_t slots_capacity;
	Session_Slot* slots;
	uint32_t map_bucket_count;
	Session_Address_Bucket* session_address_map;
};

void session_free_all_from_pool(Session_Pool* pool);
Session_Pool session_pool_create(Arena* arena, uint32_t capacity);
void session_address_map_insert(Session_Pool* pool, Session_Address address, Session_Handle handle);
Session_Handle session_get_handle_from_address(Session_Pool* pool, Session_Address address);
uint32_t session_address_map_key_exists(Session_Pool* pool, Session_Address address);
Session_Handle session_acquire(Session_Pool* pool, Session_Address address);
Session_State* session_get_pointer_from_handle(Session_Pool* pool, Session_Handle handle);
void session_release(Session_Pool* pool, Session_Handle handle);

#endif // !SESSION_HPP