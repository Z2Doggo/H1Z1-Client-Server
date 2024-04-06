#ifndef MISC_HPP
#define MISC_HPP

#define _CRT_SECURE_NO_WARNINGS

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <iostream>
#include <iterator>
#include <string>
#include <any>
#include <type_traits>
#include <vector>
#include <memory>
#include <iomanip>
#include <array>
#include <limits>
#include <tuple>
#include <map>
#include <unordered_map>

#include <cassert>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#define TRANSMUTE(t) *(t*)&
#define ALIGN_POW2(x, align) (((x) + ((align)-1)) & ~(((x) - (x)) + (align)-1))
#define DEFER_SCOPE(begin, end) for (uintptr_t _ = ((begin), 0); !_; ++_, (end))
#define UNUSED(x) (void)x

#define STRING_CAST(x) const_cast<char*>(x)

#define STREAM_REMAINING_DATA(stream)  (stream.buffer.data + stream.cursor)
#define STREAM_REMAINING_SIZE(stream)  ((stream).buffer.size - (stream).cursor)
#define STREAM_AT(stream) ((stream).buffer.data + (stream).cursor)

#define MIN(a,b)  (a < b ? a : b)
#define MAX(a,b)  (a > b ? a : b)
#define KB(n) ((n) * 1024)
#define MB(n) (KB(n) * 1024)

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef intptr_t iptr;
typedef ptrdiff_t isize;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t uptr;
typedef size_t usize;

typedef u8 b8;
typedef u16 b16;
typedef u32 b32;
typedef u64 b64;

typedef unsigned char uchar;
typedef float f32;
typedef double f64;

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

typedef struct
{
	f32 x, y, z;
} vec3;

typedef struct
{
	f32 x, y, z, w;
} vec4;

Arena* arena_alloc(size_t size);
void arena_push_alignment(Arena* arena, uintptr_t alignment);

void* arena_push_size(Arena* arena, size_t size);
void* arena_push_copy(Arena* arena, void* memory_to_copy, size_t size);
void* arena_push_copy_zt(Arena* arena, void* memory, size_t size);

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

Buffer buffer_from_array(size_t size, uint8_t* data);
uint32_t mem_match(void* data_1, void* data_2, size_t size);

#define BUNDLE_STRING(s) buffer_from_array(sizeof(s) - 1, s)
#define STRING(s) { .size = sizeof(s) - 1, .data = (uint8_t *)s }

#endif // !MISC_HPP