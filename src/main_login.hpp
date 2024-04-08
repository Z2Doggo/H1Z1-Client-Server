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
	Unhandled,
	PingResponder,
};

// SESSION_ADDRESS REGION BEGIN
struct Adress_Part
{
	u32 ip;
	u16 port;
};

union Address
{
	u64 full;
	Adress_Part part;
};
// SESSION_ADDRESS REGION END

struct Protocol_Params
{
	u32 crc_seed;
	u8 crc_size;
	u16 compression;
	u32 udp_size;
	u32 use_encryption;
};

class Session
{
public:
	// Session related
	Address address;
	Protocol_Params args;

	// Fragment related
	i32 acked_in;
	i32 sequence_in;
	i32 sequence_out;

	// Encryption related
	RC4 rc4_in;
	RC4 rc4_out;

	// Client related
	u32 handle_id;
	Session_Enums session_enums;
	u64 guid;
};

struct Address_Bucket
{
	Address key;
	u32 handle_id;
};

enum class Slot_Enums
{
	Invalid,
	Free,
	Active,
};

union Content
{
	u32 next_index;
	Session session;
};

struct Slot
{
	u32 generation;
	Slot_Enums slot_enums;
	Content content;
};

class Pool
{
public:
	u32 active_count;
	u32 free_head_index;
	u32 slots_capacity;
	Slot* slots;
	u32 map_bucket_count;
	Address_Bucket* session_address_map;

	void free_all(Pool* pool);
	Pool create_session(Arena* arena, u32 capacity);
	void map_insert(Pool* pool, Address address, u32 handle_id);
	u32 get_id_from_address(Pool* pool, Address address);
	u32 acquire_session(Pool* pool, Address address);
	Session* get_ptr_from_id(Pool* pool, u32 handle_id);
};

class Application : public Pool
{
public:
	Socket_Api* api;
	Pool pool;
	Socket_Sock socket;
	Arena* arena_per_tick;
	usize rc4_key_decoded_len;
	u8 rc4_key_decoded[256];
	Stream fragment_accumulator;
};

#include "SOE/soe_protocol.hpp"

#endif // !MAIN_LOGIN_HPP