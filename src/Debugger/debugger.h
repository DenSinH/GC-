#pragma once

#include <stdbool.h>

#undef SHOW_EXAMPLE_MENU
#define CONSOLE_COMMAND(name) const char* name(char** args, int argc)

#ifdef __cplusplus
extern "C" {
#endif
int debugger_run();
void debugger_init(bool* shutdown);
void add_command(const char* name, const char* description, CONSOLE_COMMAND((*callback)));
#ifdef __cplusplus
}
#endif