#ifndef OLD_ENDIAN_HPP
#define OLD_ENDIAN_HPP

#include <cmath>

#include "misc.hpp"

//----------------------------------------------------------------
// F16 Conversion Tables
//----------------------------------------------------------------

#define endian_read_u8_little(p) endian_read_u8(p)
#define endian_read_u8_big(p) endian_read_u8(p)

// --TODO--(rhett): Will casting unsigned to signed cause weirdness?
// NOTE(rhett): This behaves as I need it to on my system. Unsure if this is the case everywhere
#define endian_read_i8_little(p) (i8) endian_read_u8(p)
#define endian_read_i8_big(p) (i8) endian_read_u8(p)
#define endian_read_i16_little(p) (i16) endian_read_u16_little(p)
#define endian_read_i16_big(p) (i16) endian_read_u16_big(p)
#define endian_read_i32_little(p) (i32) endian_read_u32_little(p)
#define endian_read_i32_big(p) (i32) endian_read_u32_big(p)
#define endian_read_i64_little(p) (i64) endian_read_u64_little(p)
#define endian_read_i64_big(p) (i64) endian_read_u64_big(p)

#define endian_read_b8_little(p) (b8) endian_read_u8(p)
#define endian_read_b8_big(p) (b8) endian_read_u8(p)
#define endian_read_b16_little(p) (b16) endian_read_u16_little(p)
#define endian_read_b16_big(p) (b16) endian_read_u16_big(p)
#define endian_read_b32_little(p) (b32) endian_read_u32_little(p)
#define endian_read_b32_big(p) (b32) endian_read_u32_big(p)
#define endian_read_b64_little(p) (b64) endian_read_u64_little(p)
#define endian_read_b64_big(p) (b64) endian_read_u64_big(p)

// TODO(rhett): not sure if these writers will work how I expect them to. verify written data
#define endian_write_u8_little(p, v) endian_write_u8(p, v)
#define endian_write_u8_big(p, v) endian_write_u8(p, v)

#define endian_write_i8_little(p, v) endian_write_u8(p, (i8)v)
#define endian_write_i8_big(p, v) endian_write_u8(p, (i8)v)
#define endian_write_i16_little(p, v) endian_write_u16_little(p, (i16)v)
#define endian_write_i16_big(p, v) endian_write_u16_big(p, (i16)v)
#define endian_write_i32_little(p, v) endian_write_u32_little(p, (i32)v)
#define endian_write_i32_big(p, v) endian_write_u32_big(p, (i32)v)
#define endian_write_i64_little(p, v) endian_write_u64_little(p, (i64)v)
#define endian_write_i64_big(p, v) endian_write_u64_big(p, (i64)v)

#define endian_write_b8_little(p, v) endian_write_u8(p, (b8)v)
#define endian_write_b8_big(p, v) endian_write_u8(p, (b8)v)
#define endian_write_b16_little(p, v) endian_write_u16_little(p, (b16)v)
#define endian_write_b16_big(p, v) endian_write_u16_big(p, (b16)v)
#define endian_write_b32_little(p, v) endian_write_u32_little(p, (b32)v)
#define endian_write_b32_big(p, v) endian_write_u32_big(p, (b32)v)
#define endian_write_b64_little(p, v) endian_write_u64_little(p, (b64)v)
#define endian_write_b64_big(p, v) endian_write_u64_big(p, (b64)v)

u8 endian_read_u8(u8* data);

u16 endian_read_u16_little(u8* data);

u32 endian_read_u32_little(u8* data);

u64 endian_read_u64_little(u8* data);

f64 endian_read_f64_little(u8* data);

f32 endian_read_f32_little(u8* data);

f32 endian_read_f16_little(u8* data);
u16 endian_read_u16_big(u8* data);

u32 endian_read_u32_big(u8* data);

u64 endian_read_u64_big(u8* data);

f64 endian_read_f64_big(u8* data);

f32 endian_read_f32_big(u8* data);

f32 endian_read_f16_big(u8* data);

void endian_write_f64_little(u8* buffer, f64 value);

void endian_write_f32_little(u8* buffer, f32 value);

void endian_write_u8(u8* buffer, u8 value);
void endian_write_u16_little(u8* buffer, u16 value);

void endian_write_u32_little(u8* buffer, u32 value);
void endian_write_u64_little(u8* buffer, u64 value);

void endian_write_u16_big(u8* buffer, u16 value);

void endian_write_u32_big(u8* buffer, u32 value);

void endian_write_u64_big(u8* buffer, u32 value);

void endian_write_f32_big(u8* buffer, f32 value);

void endian_write_f64_big(u8* buffer, f64 value);

struct vecf64
{
    f64 x;
    f64 y;
    f64 z;
    f64 w;
};

typedef struct euler_angle
{
    f32 qx;
    f32 qy;
    f32 qz;
    f32 qw;
} euler_angle;

vec4 eul2quat(euler_angle* angles);

vec3 endian_read_vec3_little(u8* data);

vec4 endian_read_vec4_little(u8* data);

void endian_write_vec3_little(u8* buffer, vec3 value);

void endian_write_vec4_little(u8* buffer, vec4 value);

typedef struct
{
    u32 value;
    u32 length;
} uint2b;

uint2b endian_read_uint2b_little(u8* data);

u32 endian_write_uint2b_little(u8* buffer, u32 value);

typedef struct
{
    i32 value;
    i32 length;
} int2b;

int2b endian_read_int2b_little(u8* data, i32 offset);
i32 endian_write_int2b_little(u8* buffer, i32 value);

#endif // !OLD_ENDIAN_HPP