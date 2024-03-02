#ifndef APP_MEMORY_HPP
#define APP_MEMORY_HPP

#include "main_login.hpp"

struct App_Memory
{
	Socket_Api api;
	Arena* arena_total;
	App_State* app;
	float ms_tick;
	float ms_work;
	uint64_t tick_count;
};

#define APP_TICK(name) void name(struct App_Memory *memory)
typedef APP_TICK(app_tick_t);

#endif // !APP_MEMORY_HPP