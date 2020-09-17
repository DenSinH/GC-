#ifndef GC__INTERFACE_H
#define GC__INTERFACE_H

#include <stdbool.h>
#include <stdint.h>

#define CONSOLE_COMMAND(name) void name(char** args, int argc, char* output)
#define MAX_OUTPUT_LENGTH 0x100

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct s_framebuffer {
        unsigned int id;
        size_t src_width, src_height;
        size_t dest_width, dest_height;
    } s_framebuffer;

    int ui_run();

    void frontend_init(
            bool* shutdown,
            uint32_t* PC,
            uint8_t* memory,
            uint64_t mem_size,
            uint32_t (*valid_address_mask)(uint32_t),
            uint64_t* timer,
            uint8_t (*mem_read)(const uint8_t* data, uint64_t off)
    );

    void add_command(const char* command, const char* description, CONSOLE_COMMAND((*callback)));
    int add_register_tab(const char* name);
    void add_register_data(char* name, const void* value, size_t size, int tab);

    void bind_video_init(void (*initializer)());
    void bind_video_render(s_framebuffer (*render)());
#ifdef __cplusplus
}
#endif

#endif //GC__INTERFACE_H
