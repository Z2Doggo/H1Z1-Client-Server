#include "app_memory.hpp"

#define MODULE_FILE "H1_Login_Module.dll"
#define MODULE_FILE_TEMP "H1_Login_Module_TEMP.dll"

typedef struct
{
	HMODULE module;
	FILETIME last_write_time;
	app_tick_t* tick_func;
	unsigned int is_valid;
} App_Code;

APP_TICK(app_tick_stub)
{
	(void)memory;
}

FILETIME get_last_write_time(const char* file_name)
{
	FILETIME result{};
	WIN32_FILE_ATTRIBUTE_DATA file_data;
	if (GetFileAttributesEx(file_name, GetFileExInfoStandard, &file_data))
	{
		result = file_data.ftLastWriteTime;
	}

	return result;
}

App_Code app_code_load()
{
	App_Code result{};

	result.last_write_time = get_last_write_time(MODULE_FILE);
	CopyFile(MODULE_FILE, MODULE_FILE_TEMP, false);

	result.module = LoadLibrary(MODULE_FILE_TEMP);
	if (result.module)
	{
		result.tick_func = (app_tick_t*)GetProcAddress(result.module, "server_tick");
		result.is_valid = !!result.tick_func;
	}

	if (!result.is_valid)
	{
		result.tick_func = app_tick_stub;
	}

	return result;
}

void app_code_unload(App_Code* code)
{
	if (code->module)
	{
		FreeLibrary(code->module);
		code->module = 0;
	}

	code->is_valid = false;
	code->tick_func = app_tick_stub;
}

int main(void)
{
	App_Memory memory{
		.api{
			.folder_create = socket_win_folder_create,
			.buffer_write_to_file = socket_win_buffer_write_to_file,
			.buffer_load_from_file = socket_win_buffer_load_from_file,
			.wall_clock = socket_win_wall_clock,
			.elapsed_seconds = socket_win_elapsed_seconds,
			.udp_create_and_bind = socket_win_udp_create_and_bind,
			.receive_from = socket_win_receive_from,
			.send_to = socket_win_send_to,
		},
		.arena_total = arena_alloc(MB(100)),
	};

	LARGE_INTEGER performance_freq;
	QueryPerformanceFrequency(&performance_freq);

	unsigned int is_sleep_granular = timeBeginPeriod(1) == TIMERR_NOERROR;
	float tick_rate = 64.f;
	float seconds_per_tick = 1.f / tick_rate;

	App_Code code = app_code_load();

	unsigned long long previous_counter = socket_win_wall_clock();
	unsigned int is_running = true;

	while (is_running)
	{
		code.tick_func(&memory);
		unsigned long long work_counter = socket_win_wall_clock();

		memory.ms_work = 1000.f * socket_win_elapsed_seconds(previous_counter, work_counter);
		float tick_elapsed_seconds = socket_win_elapsed_seconds(previous_counter, socket_win_wall_clock());
		if (tick_elapsed_seconds < seconds_per_tick)
		{
			if (is_sleep_granular)
			{
				int ms_sleep = (int)(seconds_per_tick * 1000.f) - (int)(tick_elapsed_seconds * 1000.f) - 1;
				if (ms_sleep > 0 )
				{
					Sleep(ms_sleep);
				}
			}

			while (tick_elapsed_seconds < seconds_per_tick)
			{
				tick_elapsed_seconds = socket_win_elapsed_seconds(previous_counter, socket_win_wall_clock());
			}
		}

		unsigned long long end_counter = socket_win_wall_clock();
		memory.ms_tick = 1000.f * socket_win_elapsed_seconds(previous_counter, end_counter);

		previous_counter = end_counter;
		memory.tick_count++;
	}

	return 0;
}