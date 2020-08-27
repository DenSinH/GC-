#pragma once

#include <stdbool.h>
#include <stdint.h>

#undef SHOW_EXAMPLE_MENU
#define CONSOLE_COMMAND(name) void name(char** args, int argc, char* output)
#define MAX_OUTPUT_LENGTH 0x100

#ifdef __cplusplus
extern "C" {
#endif
int debugger_run();
void debugger_init(
        bool* shutdown,
        uint32_t* PC,
        uint8_t * memory,
        uint32_t (*valid_address_mask)(uint32_t),
        uint64_t* timer
);
void add_command(const char* command, const char* description, CONSOLE_COMMAND((*callback)));
void add_register_data(char* name, const void* value, bool islong);
#ifdef __cplusplus
}
#endif