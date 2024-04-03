#ifndef BINARY_HPP
#define BINARY_HPP

#include "misc.hpp"

#define read_u8_little_at read_uint8_t_at
#define read_u8_big_at read_uint8_t_at
#define write_u8_little_at write_uint8_t_at
#define write_u8_big_at write_uint8_t_at
#define read_b8_little_at read_uint8_t_at
#define read_b8_big_at read_uint8_t_at
#define write_b8_little_at write_uint8_t_at
#define write_b8_big_at read_uint8_t_at
#define read_u8_little read_uint8_t
#define read_u8_big read_uint8_t
#define write_u8_little write_uint8_t
#define write_u8_big write_uint8_t
#define peek_u8_little peek_uint8_t
#define peek_u8_big peek_uint8_t
#define read_b8_little read_uint8_t
#define read_b8_big read_uint8_t
#define write_b8_little write_uint8_t
#define write_b8_big write_uint8_t
#define peek_b8_little peek_uint8_t
#define peek_b8_big peek_uint8_t
#define push_u8_little push_uint8_t
#define push_u8_big push_uint8_t
#define push_b8_little push_uint8_t
#define push_b8_big push_uint8_t

uint8_t read_uint8_t_at(void* memory);
uint16_t read_uint16_t_little_at(void* memory);
uint32_t read_uint32_t_little_at(void* memory);
uint64_t read_uint64_t_little_at(void* memory);
float read_float_little_at(void* memory);
double read_double_little_at(void* memory);
vec3 read_vec3_little_at(void* memory);
vec4 read_vec4_little_at(void* memory);
uint16_t read_uint16_t_big_at(void* memory);
uint32_t read_uint32_t_big_at(void* memory);
uint64_t read_uint64_t_big_at(void* memory);
float read_float_big_at(void* memory);
double read_double_big_at(void* memory);
vec3 read_vec3_big_at(void* memory);
vec4 read_vec4_big_at(void* memory);
void write_uint8_t_at(void* memory, uint8_t value);
void write_uint16_t_little_at(void* memory, uint16_t value);
void write_uint32_t_little_at(void* memory, uint32_t value);
void write_uint64_t_little_at(void* memory, uint64_t value);
void write_float_little_at(void* memory, float value);
void write_double_little_at(void* memory, double value);
void write_vec3_little_at(void* memory, vec3 value);
void write_vec4_little_at(void* memory, vec4 value);
void write_uint16_t_big_at(void* memory, uint16_t value);
void write_uint32_t_big_at(void* memory, uint32_t value);
void write_uint64_t_big_at(void* memory, uint64_t value);
void write_float_big_at(void* memory, float value);
void write_double_big_at(void* memory, double value);
void write_vec3_big_at(void* memory, vec3 value);
void write_vec4_big_at(void* memory, vec4 value);
uint8_t read_uint8_t(Stream* stream);
uint16_t read_uint16_t_little(Stream* stream);
uint32_t read_uint32_t_little(Stream* stream);
uint64_t read_uint64_t_little(Stream* stream);
float read_float_little(Stream* stream);
double read_double_little(Stream* stream);
vec3 read_vec3_little(Stream* stream);
vec4 read_vec4_little(Stream* stream);
uint16_t read_uint16_t_big(Stream* stream);
uint32_t read_uint32_t_big(Stream* stream);
uint64_t read_uint64_t_big(Stream* stream);
float read_float_big(Stream* stream);
double read_double_big(Stream* stream);
vec3 read_vec3_big(Stream* stream);
vec4 read_vec4_big(Stream* stream);
void write_uint8_t(Stream* stream, uint8_t value);
void write_uint16_t_little(Stream* stream, uint16_t value);
void write_uint32_t_little(Stream* stream, uint32_t value);
void write_uint64_t_little(Stream* stream, uint64_t value);
void write_float_little(Stream* stream, float value);
void write_double_little(Stream* stream, double value);
void write_vec3_little(Stream* stream, vec3 value);
void write_vec4_little(Stream* stream, vec4 value);
void write_uint16_t_big(Stream* stream, uint16_t value);
void write_uint32_t_big(Stream* stream, uint32_t value);
void write_uint64_t_big(Stream* stream, uint64_t value);
void write_float_big(Stream* stream, float value);
void write_double_big(Stream* stream, double value);
void write_vec3_big(Stream* stream, vec3 value);
void write_vec4_big(Stream* stream, vec4 value);
uint8_t peek_uint8_t(Stream stream, uintptr_t offset);
uint16_t peek_uint16_t_little(Stream stream, uintptr_t offset);
uint32_t peek_uint32_t_little(Stream stream, uintptr_t offset);
uint64_t peek_uint64_t_little(Stream stream, uintptr_t offset);
float peek_float_little(Stream stream, uintptr_t offset);
double peek_double_little(Stream stream, uintptr_t offset);
vec3 peek_vec3_little(Stream stream, uintptr_t offset);
vec4 peek_vec4_little(Stream stream, uintptr_t offset);
uint16_t peek_uint16_t_big(Stream stream, uintptr_t offset);
uint32_t peek_uint32_t_big(Stream stream, uintptr_t offset);
uint64_t peek_uint64_t_big(Stream stream, uintptr_t offset);
float peek_float_big(Stream stream, uintptr_t offset);
double peek_double_big(Stream stream, uintptr_t offset);
vec3 peek_vec3_big(Stream stream, uintptr_t offset);
vec4 peek_vec4_big(Stream stream, uintptr_t offset);
float read_f16_little_at(void* memory);
float read_f16_big_at(void* memory);
void write_f16_little_at(void* memory, float value);
void write_f16_big_at(void* memory, float value);
float read_f16_little(Stream* stream);
float read_f16_big(Stream* stream);
void write_f16_little(Stream* stream, float value);
void write_f16_big(Stream* stream, float value);
float peek_f16_little(Stream stream, uintptr_t offset);
float peek_f16_big(Stream stream, uintptr_t offset);
void push_uint8_t(Arena* arena, uint8_t value);
void push_uint16_t_little(Arena* arena, uint16_t value);
void push_uint32_t_little(Arena* arena, uint32_t value);
void push_uint64_t_little(Arena* arena, uint64_t value);
void push_float_little(Arena* arena, float value);
void push_double_little(Arena* arena, double value);
void push_vec3_little(Arena* arena, vec3 value);
void push_vec4_little(Arena* arena, vec4 value);
void push_uint16_t_big(Arena* arena, uint16_t value);
void push_uint32_t_big(Arena* arena, uint32_t value);
void push_uint64_t_big(Arena* arena, uint64_t value);
void push_float_big(Arena* arena, float value);
void push_double_big(Arena* arena, double value);
void push_vec3_big(Arena* arena, vec3 value);
void push_vec4_big(Arena* arena, vec4 value);
void push_f16_little(Arena* arena, float value);
void push_f16_big(Arena* arena, float value);

#endif // !BINARY_HPP