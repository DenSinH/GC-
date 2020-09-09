#ifndef GC__FLIPPER_H
#define GC__FLIPPER_H

#include "default.h"

#include <stdbool.h>

#define FLIPPER_SCREEN_WIDTH 1280
#define FLIPPER_SCREEN_HEIGHT 720

#define FLIPPER_TEMP_BUFFER_SIZE 0x10000

typedef struct s_Flipper {

    struct s_GameCube* system;
    u8* memory;
    struct s_CP* CP;

    struct {
        float r, g, b;
    } backdrop;

    unsigned int framebuffer[2];
    bool current_framebuffer;

    u16 n_vertices;
    u8 command;
    u8 temp_buffer[FLIPPER_TEMP_BUFFER_SIZE];

    unsigned int VBOs[21];
    unsigned int EBO;
    unsigned int VAO;
    u32 VCD_lo, VCD_hi, VAT_A, VAT_B, VAT_C;
    int VCD_lo_location, VCD_hi_location, VAT_A_location, VAT_B_location, VAT_C_location;
    unsigned int shaderProgram;
} s_Flipper;

void init_Flipper(s_Flipper* flipper);

void video_init_Flipper(s_Flipper* gpu);
void queue_draw_Flipper(s_Flipper* flipper, u16 n_vertices, u8 command);
struct s_framebuffer render_Flipper(s_Flipper* flipper);

#endif //GC__FLIPPER_H
