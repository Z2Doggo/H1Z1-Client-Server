#include "../main_login.hpp"

void Pool::free_all(Pool* pool)
{
	pool->free_head_index = 1;
	for (uint32_t i = free_head_index; i < pool->slots_capacity - 1; i++)
	{
		pool->slots[i].content.next_index = i + 1;
		pool->slots[i].slot_enums = Slot_Enums::Free;
	}

	pool->slots[pool->slots_capacity - 1].content.next_index = 0;
	pool->slots[pool->slots_capacity - 1].slot_enums = Slot_Enums::Free;
}

Pool Pool::create_session(Arena* arena, uint32_t capacity)
{
	Pool pool{
		.slots_capacity = capacity,
		.slots = static_cast<Slot*>(arena_push_size(arena, sizeof(Slot) * capacity)),
		.map_bucket_count = SESSION_MAP_OVERHEAD(capacity),
		.session_address_map = static_cast<Address_Bucket*>(arena_push_size(arena, sizeof(Address_Bucket) * pool.map_bucket_count)),
	};

	free_all(&pool);
	return pool;
}

void Pool::map_insert(Pool* pool, Address address, uint32_t handle_id)
{
	uint32_t index = address.full % pool->map_bucket_count;

	for (uint32_t collision_iter = 0; pool->session_address_map[index].handle_id; index++, collision_iter++)
	{
		if (collision_iter == SESSION_MAP_MAX_COLLISION)
		{
			std::cout << "Max collision count reached\n";
			return;
		}

		if (pool->session_address_map[index].handle_id == handle_id)
		{
			std::cout << "Inserting duplicate value in address map\n";
			return;
		}
	}
	pool->session_address_map[index].key = address;
	pool->session_address_map[index].handle_id = handle_id;
}

uint32_t Pool::get_id_from_address(Pool* pool, Address address)
{
	uint32_t index = address.full % pool->map_bucket_count;
	for (uint32_t collision_iter = 0; pool->session_address_map[index].key.full != address.full; index++, collision_iter++)
	{
		if (collision_iter == SESSION_MAP_MAX_COLLISION)
		{
			return 0;
		}
	}

	return pool->session_address_map[index].handle_id;
}

uint32_t Pool::acquire_session(Pool* pool, Address address)
{
	uint32_t result = 0;

	if (pool->active_count >= pool->slots_capacity)
	{
		std::cout << "Session pool is full\n";
		return 0;
	}

	uint32_t index = pool->free_head_index;
	Slot* slot = &pool->slots[index];

	pool->free_head_index = slot->content.next_index;
	pool->active_count++;
	slot->generation++;
	slot->slot_enums = Slot_Enums::Active;

	SESSION_HANDLE_INDEX_WRITE(result, index);
	SESSION_HANDLE_GENERATION_WRITE(result, slot->generation);
	map_insert(pool, address, result);

	memset(&slot->content, 0, sizeof(slot->content));
	Session* state = &slot->content.session;
	state->handle_id = result;
	state->address = address;

	return result;
}
Session* Pool::get_ptr_from_id(Pool* pool, uint32_t handle_id)
{
	std::cout << "[Session] - Getting session ptr for 0x" << std::hex << handle_id << "\n";

	uint32_t generation = SESSION_HANDLE_GENERATION_READ(handle_id);
	uint32_t index = SESSION_HANDLE_INDEX_READ(handle_id);
	Session* session = &pool->slots[index].content.session;

	return session;
}