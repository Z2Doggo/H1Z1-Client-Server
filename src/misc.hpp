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
#include <cmath>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cstring>

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

typedef bool b8;
typedef float f32;
typedef double f64;

#define TRANSMUTE(t) *(t*)&
#define ALIGN_POW2(x, align) (((x) + ((align)-1)) & ~(((x) - (x)) + (align)-1))
#define DEFER_SCOPE(begin, end) for (uptr _ = ((begin), 0); !_; ++_, (end))
#define UNUSED(x) (void)x

#define STRING_CAST(x) const_cast<char*>(x)

#define STREAM_REMAINING_DATA(stream)  (stream.buffer.data + stream.cursor)
#define STREAM_REMAINING_SIZE(stream)  ((stream).buffer.size - (stream).cursor)
#define STREAM_AT(stream) ((stream).buffer.data + (stream).cursor)

#define MIN(a,b)  (a < b ? a : b)
#define MAX(a,b)  (a > b ? a : b)
#define KB(n) ((n) * 1024)
#define MB(n) (KB(n) * 1024)

struct Buffer
{
	usize size;
	char* data;
};

struct Stream
{
	Buffer buffer;
	uptr cursor;
};

struct Arena
{
	u8* memory;
	usize size;
	uptr cursor;
	uptr alignment;
};

struct vec3
{
	f32 x, y, z;
};

struct vec4
{
	f32 x, y, z, w;
};

Arena* arena_alloc(usize size);
void arena_push_alignment(Arena* arena, uptr alignment);

void* arena_push_size(Arena* arena, usize size);
void* arena_push_copy(Arena* arena, void* memory_to_copy, usize size);
void* arena_push_copy_zt(Arena* arena, void* memory, usize size);

#define arena_push_array(arena, type, count) arena_push_size(arena, sizeof(type) * (count))
#define arena_push_struct(arena, type) arena_push_size(arena, sizeof(type))

Arena* arena_suballoc(Arena* arena, usize size);
void arena_reset(Arena* arena);

struct Arena_Snapshot
{
	Arena* arena;
	uptr cursor;
	uptr alignment;
};

Arena_Snapshot arena_take_snapshot(Arena* arena);
void arena_apply_snapshot(Arena_Snapshot snapshot);

usize base64_calc_decoded_len(u8* data, usize data_size);
usize base64_decode(u8* data, usize data_size, u8* buffer);

struct RC4
{
	u8 keystream[256];
	u32 index_0;
	u32 index_1;
};

void rc4_init(RC4* state, u8* key, usize key_size);
void rc4_transform(RC4* state, char* data, usize data_size);

Buffer buffer_from_array(usize size, const char* data);
u32 mem_match(void* data_1, void* data_2, usize size);

#define BUNDLE_STRING(s) buffer_from_array(sizeof(s) - 1, s)
#define BUFFER_STRING(s) {.size = sizeof(s) - 1, .data = const_cast<char*>(s)}

#endif // !MISC_HPP