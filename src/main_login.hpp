#ifndef MAIN_LOGIN_HPP
#define MAIN_LOGIN_HPP

#include "win32_sockets.hpp"
#include "misc.hpp"
#include "binary.hpp"

#define LOCAL_PORT 1115
#define MAX_SESSIONS_COUNT 16

#define SESSION_MAP_OVERHEAD(value) (value * 4)
#define SESSION_MAP_MAX_COLLISION 3

#define SESSION_HANDLE_GENERATION_READ(handle) (handle >> 19)
#define SESSION_HANDLE_GENERATION_WRITE(handle, value) handle |= (value << 19)
#define SESSION_HANDLE_INDEX_READ(handle) (handle & 0x3FFF)
#define SESSION_HANDLE_INDEX_WRITE(handle, value) handle |= (value & 0x3FFF)

enum class Session_Enums
{
	Unhandled = 0x00,
	PingResponder = 0x01,
};

// SESSION_ADDRESS REGION BEGIN
struct Adress_Part
{
	uint32_t ip;
	uint16_t port;
};

union Address
{
	uint64_t full;
	Adress_Part part;
};
// SESSION_ADDRESS REGION END

struct Protocol_Params
{
	uint32_t crc_seed;
	uint8_t crc_size;
	uint16_t compression;
	uint32_t udp_size;
	uint32_t use_encryption;
};

class Session
{
public:
	// Session related
	Address address;
	Protocol_Params args;

	// Fragment related
	int32_t acked_in;
	int32_t sequence_in;
	int32_t sequence_out;

	// Encryption related
	RC4 rc4_in;
	RC4 rc4_out;

	// Client related
	uint32_t handle_id;
	Session_Enums session_enums;
	uint64_t guid;
};

struct Address_Bucket
{
	Address key;
	uint32_t handle_id;
};

enum class Slot_Enums
{
	Invalid,
	Free,
	Active,
};

union Content
{
	uint32_t next_index;
	Session session;
};

struct Slot
{
	uint32_t generation;
	Slot_Enums slot_enums;
	Content content;
};

class Pool
{
public:
	uint32_t active_count;
	uint32_t free_head_index;
	uint32_t slots_capacity;
	Slot* slots;
	uint32_t map_bucket_count;
	Address_Bucket* session_address_map;

	void free_all(Pool* pool);
	Pool create_session(Arena* arena, uint32_t capacity);
	void map_insert(Pool* pool, Address address, uint32_t handle_id);
	uint32_t get_id_from_address(Pool* pool, Address address);
	uint32_t acquire_session(Pool* pool, Address address);
	Session* get_ptr_from_id(Pool* pool, uint32_t handle_id);
};

class Application : public Pool
{
public:
	Socket_Api* api;
	Pool pool;
	Socket_Sock socket;
	Arena* arena_per_tick;
	size_t rc4_key_decoded_len;
	uint8_t rc4_key_decoded[256];
	Stream fragment_accumulator;
};

#include "SOE/soe_protocol.hpp"

#endif // !MAIN_LOGIN_HPP