#ifndef BINARY_HPP
#define BINARY_HPP

#include "misc.hpp"

#define read_u8_little_at read_u8_at
#define read_u8_big_at read_u8_at
#define write_u8_little_at write_u8_at
#define write_u8_big_at write_u8_at
#define read_b8_little_at read_u8_at
#define read_b8_big_at read_u8_at
#define write_b8_little_at write_u8_at
#define write_b8_big_at read_u8_at
#define read_u8_little read_u8
#define read_u8_big read_u8
#define write_u8_little write_u8
#define write_u8_big write_u8
#define peek_u8_little peek_u8
#define peek_u8_big peek_u8
#define read_b8_little read_u8
#define read_b8_big read_u8
#define write_b8_little write_u8
#define write_b8_big write_u8
#define peek_b8_little peek_u8
#define peek_b8_big peek_u8
#define push_u8_little push_u8
#define push_u8_big push_u8
#define push_b8_little push_u8
#define push_b8_big push_u8

u8 read_u8_at(void* memory);
u16 read_u16_little_at(void* memory);
u32 read_u32_little_at(void* memory);
u64 read_u64_little_at(void* memory);
f32 read_f32_little_at(void* memory);
f64 read_f64_little_at(void* memory);
vec3 read_vec3_little_at(void* memory);
vec4 read_vec4_little_at(void* memory);
u16 read_u16_big_at(void* memory);
u32 read_u32_big_at(void* memory);
u64 read_u64_big_at(void* memory);
f32 read_f32_big_at(void* memory);
f64 read_f64_big_at(void* memory);
vec3 read_vec3_big_at(void* memory);
vec4 read_vec4_big_at(void* memory);
void write_u8_at(void* memory, u8 value);
void write_u16_little_at(void* memory, u16 value);
void write_u32_little_at(void* memory, u32 value);
void write_u64_little_at(void* memory, u64 value);
void write_f32_little_at(void* memory, f32 value);
void write_f64_little_at(void* memory, f64 value);
void write_vec3_little_at(void* memory, vec3 value);
void write_vec4_little_at(void* memory, vec4 value);
void write_u16_big_at(void* memory, u16 value);
void write_u32_big_at(void* memory, u32 value);
void write_u64_big_at(void* memory, u64 value);
void write_f32_big_at(void* memory, f32 value);
void write_f64_big_at(void* memory, f64 value);
void write_vec3_big_at(void* memory, vec3 value);
void write_vec4_big_at(void* memory, vec4 value);
u8 read_u8(Stream* stream);
u16 read_u16_little(Stream* stream);
u32 read_u32_little(Stream* stream);
u64 read_u64_little(Stream* stream);
f32 read_f32_little(Stream* stream);
f64 read_f64_little(Stream* stream);
vec3 read_vec3_little(Stream* stream);
vec4 read_vec4_little(Stream* stream);
u16 read_u16_big(Stream* stream);
u32 read_u32_big(Stream* stream);
u64 read_u64_big(Stream* stream);
f32 read_f32_big(Stream* stream);
f64 read_f64_big(Stream* stream);
vec3 read_vec3_big(Stream* stream);
vec4 read_vec4_big(Stream* stream);
void write_u8(Stream* stream, u8 value);
void write_u16_little(Stream* stream, u16 value);
void write_u32_little(Stream* stream, u32 value);
void write_u64_little(Stream* stream, u64 value);
void write_f32_little(Stream* stream, f32 value);
void write_f64_little(Stream* stream, f64 value);
void write_vec3_little(Stream* stream, vec3 value);
void write_vec4_little(Stream* stream, vec4 value);
void write_u16_big(Stream* stream, u16 value);
void write_u32_big(Stream* stream, u32 value);
void write_u64_big(Stream* stream, u64 value);
void write_f32_big(Stream* stream, f32 value);
void write_f64_big(Stream* stream, f64 value);
void write_vec3_big(Stream* stream, vec3 value);
void write_vec4_big(Stream* stream, vec4 value);
u8 peek_u8(Stream stream, uptr offset);
u16 peek_u16_little(Stream stream, uptr offset);
u32 peek_u32_little(Stream stream, uptr offset);
u64 peek_u64_little(Stream stream, uptr offset);
f32 peek_f32_little(Stream stream, uptr offset);
f64 peek_f64_little(Stream stream, uptr offset);
vec3 peek_vec3_little(Stream stream, uptr offset);
vec4 peek_vec4_little(Stream stream, uptr offset);
u16 peek_u16_big(Stream stream, uptr offset);
u32 peek_u32_big(Stream stream, uptr offset);
u64 peek_u64_big(Stream stream, uptr offset);
f32 peek_f32_big(Stream stream, uptr offset);
f64 peek_f64_big(Stream stream, uptr offset);
vec3 peek_vec3_big(Stream stream, uptr offset);
vec4 peek_vec4_big(Stream stream, uptr offset);
f32 read_f16_little_at(void* memory);
f32 read_f16_big_at(void* memory);
void write_f16_little_at(void* memory, f32 value);
void write_f16_big_at(void* memory, f32 value);
f32 read_f16_little(Stream* stream);
f32 read_f16_big(Stream* stream);
void write_f16_little(Stream* stream, f32 value);
void write_f16_big(Stream* stream, f32 value);
f32 peek_f16_little(Stream stream, uptr offset);
f32 peek_f16_big(Stream stream, uptr offset);
void push_u8(Arena* arena, u8 value);
void push_u16_little(Arena* arena, u16 value);
void push_u32_little(Arena* arena, u32 value);
void push_u64_little(Arena* arena, u64 value);
void push_f32_little(Arena* arena, f32 value);
void push_f64_little(Arena* arena, f64 value);
void push_vec3_little(Arena* arena, vec3 value);
void push_vec4_little(Arena* arena, vec4 value);
void push_u16_big(Arena* arena, u16 value);
void push_u32_big(Arena* arena, u32 value);
void push_u64_big(Arena* arena, u64 value);
void push_f32_big(Arena* arena, f32 value);
void push_f64_big(Arena* arena, f64 value);
void push_vec3_big(Arena* arena, vec3 value);
void push_vec4_big(Arena* arena, vec4 value);
void push_f16_little(Arena* arena, f32 value);
void push_f16_big(Arena* arena, f32 value);

#endif // !BINARY_HPP