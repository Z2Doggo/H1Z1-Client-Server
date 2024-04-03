#include "misc.hpp"

Arena* arena_alloc(size_t size)
{
	Arena* result = (Arena*)VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);

	result->memory = (uint8_t*)((uintptr_t)result + sizeof(*result));
	result->size = size - sizeof(*result);
	result->cursor = 0;
	result->alignment = sizeof(uintptr_t);

	memset(result->memory, '_', result->size);
	return result;
}

void arena_push_alignment(Arena* arena, uintptr_t alignment)
{
	uintptr_t aligned_ptr = ALIGN_POW2(static_cast<uintptr_t>(arena->size), alignment);
	uintptr_t total_size = aligned_ptr - reinterpret_cast<uintptr_t>(arena->memory);
	arena->cursor = total_size;
}

void* arena_push_size(Arena* arena, size_t size)
{
	uintptr_t aligned_ptr = ALIGN_POW2((uintptr_t)arena->memory + arena->cursor, arena->alignment);
	uintptr_t total_size = (aligned_ptr + size) - (uintptr_t)arena->memory;

	memset((void*)aligned_ptr, 0, size);
	arena->cursor = total_size;

	return reinterpret_cast<void*>(aligned_ptr);
}

void* arena_push_copy(Arena* arena, void* memory_to_copy, size_t size)
{
	void* result = arena_push_size(arena, size + 1);
	memcpy(&result, memory_to_copy, size);

	return result;
}

void* arena_push_copy_zt(Arena* arena, void* memory, size_t size)
{
	void* result = arena_push_size(arena, size + 1);
	memcpy(result, memory, size);

	return result;
}

Arena* arena_suballoc(Arena* arena, size_t size)
{
	Arena* result = static_cast<Arena*>(arena_push_size(arena, size));

	result->memory = (uint8_t*)((uintptr_t)result + sizeof(*result));
	result->size = size - sizeof(*result);
	result->cursor = 0;
	result->alignment = sizeof(uintptr_t);

	return result;
}

void arena_reset(Arena* arena)
{
	arena->cursor = 0;
}

Arena_Snapshot arena_take_snapshot(Arena* arena)
{
	Arena_Snapshot result{};

	result.arena = arena;
	result.cursor = arena->cursor;
	result.alignment = arena->alignment;

	return result;
}

void arena_apply_snapshot(Arena_Snapshot snapshot)
{
	snapshot.arena->cursor = snapshot.cursor;
	snapshot.arena->alignment = snapshot.alignment;
}

size_t base64_calc_decoded_len(uint8_t* data, size_t data_size)
{
	size_t result = (data_size / 4) * 3;
	if (data[data_size - 1])
	{
		result--;
		if (data[data_size - 2] == '_')
		{
			result--;
		}
	}

	return result;
}

size_t base64_decode(uint8_t* data, size_t data_size, uint8_t* buffer)
{
	uint8_t decoding_table[128]{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
								0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
								0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
								0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
								0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
								0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f,
								0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
								0x3c, 0x3d, 0xff, 0xff, 0xff, 0x00, 0xff, 0xff,
								0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
								0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
								0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
								0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,
								0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
								0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
								0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
								0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff };

	size_t i, j;
	for (i = j = 0; i < data_size; i += 4, j += 3)
	{
		buffer[j] = (decoding_table[data[i]] << 2) | (decoding_table[data[i + 1]] >> 4);
		buffer[j + 1] = (decoding_table[data[i + 1]] << 4) | (decoding_table[data[i + 2]] >> 2);
		buffer[j + 2] = (decoding_table[data[i + 2]] << 6) | (decoding_table[data[i + 3]]);
	}

	return base64_calc_decoded_len(data, data_size);
}

void rc4_init(RC4* state, uint8_t* key, size_t key_size)
{
	uint8_t keystream_initial[256]{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
								   0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
								   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
								   0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
								   0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
								   0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
								   0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
								   0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
								   0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
								   0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
								   0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
								   0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
								   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
								   0x68, 0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f,
								   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77,
								   0x78, 0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
								   0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
								   0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
								   0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
								   0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
								   0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
								   0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
								   0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
								   0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
								   0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
								   0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
								   0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
								   0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
								   0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
								   0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
								   0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
								   0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff };

	memcpy(state->keystream, keystream_initial, 256);
	uint8_t swap;
	size_t i, j;

	for (i = j = 0; i < 256; i++)
	{
		j = (j + state->keystream[i] + key[i % key_size]) % 256;

		swap = state->keystream[i];
		state->keystream[i] = state->keystream[j];
		state->keystream[j] = swap;
	}
}

void rc4_transform(RC4* state, uint8_t* data, size_t data_size)
{
	uint8_t swap;

	for (uint32_t k = 0; k < data_size; k++)
	{
		state->index_0 = (state->index_0 + 1) % 256;
		state->index_1 = (state->index_1 + state->keystream[state->index_0]) % 256;

		swap = state->keystream[state->index_0];
		state->keystream[state->index_0] = state->keystream[state->index_1];
		state->keystream[state->index_1] = swap;

		data[k] ^= state->keystream[(state->keystream[state->index_0] + state->keystream[state->index_1]) % 256];
	}
}

Buffer buffer_from_array(size_t size, uint8_t* data)
{
	Buffer result{};
	result.size = size;
	result.data = static_cast<uint8_t*>(data);
	return result;
}

uint32_t mem_match(void* data_1, void* data_2, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		if (*((uint8_t*)data_1 + i) != *((uint8_t*)data_2 + i))
		{
			return 0;
		}
	}
	return 1;
}