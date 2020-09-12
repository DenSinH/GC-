#ifndef GC__FLIPPER_H
#define GC__FLIPPER_H

#include "default.h"

#include <stdbool.h>

#define FLIPPER_SCREEN_WIDTH 1280
#define FLIPPER_SCREEN_HEIGHT 720

#define FLIPPER_QUAD_INDEX_ARRAY_SIZE 0x8000
#define FLIPPER_QUAD_RESET_INDEX 0xffff

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
    // todo: UBO for XF mem
    unsigned int VAO, EBO, command_SSBO, XF_SSBO;  // VBO will just hold the index, the actual vector is calculated in the shader

    unsigned int VCD_lo_location, VCD_hi_location, VAT_A_location, VAT_B_location, VAT_C_location;
    unsigned int shaderProgram;
} s_Flipper;

void init_Flipper(s_Flipper* flipper);

void video_init_Flipper(s_Flipper* gpu);
void queue_draw_Flipper(s_Flipper* flipper, u8 command);
struct s_framebuffer render_Flipper(s_Flipper* flipper);

#endif //GC__FLIPPER_H
