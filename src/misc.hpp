#ifndef MISC_HPP
#define MISC_HPP

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <limits>
#include <map>
#include <unordered_map>

#include <cstdint>
#include <cstddef>
#include <cstdlib>

#define TRANSMUTE(t) *(t*)&
#define ALIGN_POW2(x, align) (((x) + ((align)-1)) & ~(((x) - (x)) + (align)-1))
#define DEFER_SCOPE(begin, end) for (uintptr_t _ = ((begin), 0); !_; ++_, (end))

#define STREAM_REMAINING_DATA(stream)  (stream.buffer.data + stream.cursor)
#define STREAM_REMAINING_SIZE(stream)  ((stream).buffer.size - (stream).cursor)
#define STREAM_AT(stream) ((stream).buffer.data + (stream).cursor)

#define MIN(a,b)  (a < b ? a : b)
#define MAX(a,b)  (a > b ? a : b)
#define KB(n) ((n) * 1024)
#define MB(n) (KB(n) * 1024)

struct Buffer
{
	size_t size;
	uint8_t* data;
};

struct Stream
{
	Buffer buffer;
	uintptr_t cursor;
};

struct Arena
{
	uint8_t* memory;
	size_t size;
	uintptr_t cursor;
	uintptr_t alignment;
};

Arena* arena_alloc(size_t size);
void arena_push_alignment(Arena* arena, uintptr_t alignment);
void* arena_push_size(Arena* arena, size_t size);
void* arena_push_copy(Arena* arena, void* memory_to_copy, size_t size);

#define arena_push_array(arena, type, count) arena_push_size(arena, sizeof(type) * (count))
#define arena_push_struct(arena, type) arena_push_size(arena, sizeof(type))

Arena* arena_suballoc(Arena* arena, size_t size);
void arena_reset(Arena* arena);

struct Arena_Snapshot
{
	Arena* arena;
	uintptr_t cursor;
	uintptr_t alignment;
};

Arena_Snapshot arena_take_snapshot(Arena* arena);
void arena_apply_snapshot(Arena_Snapshot snapshot);

size_t base64_calc_decoded_len(uint8_t* data, size_t data_size);
size_t base64_decode(uint8_t* data, size_t data_size, uint8_t* buffer);

struct RC4
{
	uint8_t keystream[256];
	uint32_t index_0;
	uint32_t index_1;
};

void rc4_init(RC4* state, uint8_t* key, size_t key_size);
void rc4_transform(RC4* state, uint8_t* data, size_t data_size);

Buffer buffer_from_array(size_t size, void* data);
#define BUNDLE_STRING(s) { .size = sizeof(s) - 1, .data = (uint8_t *)s }
#define BUNDLE_STRING_CONST(s) {sizeof(s) - 1, (uint8_t*)(s)}

#endif // !MISC_HPP