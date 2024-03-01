#include "session.hpp"

void session_free_all_from_pool(Session_Pool* pool)
{
	pool->free_head_index = 1;
	for (unsigned int iter = 1; iter < pool->slots_capacity - 1; iter++)
	{
		pool->slots[iter].session_content.next_index = iter + 1;
		pool->slots[iter].kind = Session_Slot_Kind_Free;
	}

	pool->slots[pool->slots_capacity - 1].session_content.next_index = 0;
	pool->slots[pool->slots_capacity - 1].kind = Session_Slot_Kind_Free;
}

Session_Pool session_pool_create(Arena* arena, uint32_t capacity)
{
	Session_Pool result{
		.slots_capacity = capacity,
		.slots = (Session_Slot*)arena_push_array(arena, Session_Slot, capacity),
		.map_bucket_count = SESSION_MAP_OVERHEAD(capacity),
		.session_address_map = (Session_Address_Bucket*)arena_push_array(arena, Session_Address_Bucket, result.map_bucket_count),
	};

	session_free_all_from_pool(&result);
	return result;
}

void session_address_map_insert(Session_Pool* pool, Session_Address address, Session_Handle handle)
{
	uint32_t index = address.full % pool->map_bucket_count;

	for (uint32_t collision_iter = 0; pool->session_address_map[index].value.id; index++, collision_iter++)
	{
		if (collision_iter == SESSION_MAP_MAX_COLLISION)
		{
			printf("Max collision count reached\n");
			return;
		}

		if (pool->session_address_map[index].value.id == handle.id)
		{
			printf("Inserting duplicate value in address map\n");
			return;
		}
	}
	pool->session_address_map[index].key = address;
	pool->session_address_map[index].value = handle;
}

Session_Handle session_get_handle_from_address(Session_Pool* pool, Session_Address address)
{
	uint32_t index = address.full % pool->map_bucket_count;

	for (uint32_t collision_iter = 0; pool->session_address_map[index].key.full != address.full; index++, collision_iter++)
	{
		if (collision_iter == SESSION_MAP_MAX_COLLISION)
		{
			return {};
		}
	}

	return pool->session_address_map[index].value;
}

uint32_t session_address_map_key_exists(Session_Pool* pool, Session_Address address)
{
	(void)pool;
	(void)address;

	return 0;
}

Session_Handle session_acquire(Session_Pool* pool, Session_Address address)
{
	Session_Handle result{};

	if (pool->active_count >= pool->slots_capacity)
	{
		printf("Session pool full\n");
		return {};
	}

	uint32_t index = pool->free_head_index;
	Session_Slot* slot = &pool->slots[index];

	pool->free_head_index = slot->session_content.next_index;
	pool->active_count++;
	slot->generation++;
	slot->kind = Session_Slot_Kind_Active;

	SESSION_HANDLE_INDEX_WRITE(result.id, index);
	SESSION_HANDLE_GENERATION_WRITE(result.id, slot->generation);
	session_address_map_insert(pool, address, result);

	memset(&slot->session_content, 0, sizeof(slot->session_content));
	Session_State* state = &slot->session_content.state;
	state->handle = result;
	state->addr = address;

	return result;
}

Session_State* session_get_pointer_from_handle(Session_Pool* pool, Session_Handle handle)
{
	printf("[Server] - Getting session pointer for %#x\n", handle.id);

	uint32_t generation = SESSION_HANDLE_GENERATION_READ(handle.id);
	uint32_t index = SESSION_HANDLE_INDEX_READ(handle.id);
	Session_State* result = &pool->slots[index].session_content.state;

	return result;
}

void session_release(Session_Pool* pool, Session_Handle handle)
{
	(void)pool;
	(void)handle;
}