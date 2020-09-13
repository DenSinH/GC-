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

    unsigned int framebuffer[2];
    bool current_framebuffer;

    u32 draw_command_index;
    // todo: UBO for XF mem (faster)
    unsigned int VAO, EBO, command_SSBO, XF_SSBO;  // VBO will just hold the index, the actual vector is calculated in the shader
    void* fence;

    unsigned int VCD_lo_location, VCD_hi_location, VAT_A_location, VAT_B_location, VAT_C_location;
    unsigned int MATIDX_REG_A_location, MATIDX_REG_B_location;
    unsigned int shaderProgram;
} s_Flipper;

void init_Flipper(s_Flipper* flipper);

void video_init_Flipper(s_Flipper* gpu);
struct s_framebuffer render_Flipper(s_Flipper* flipper);

#endif //GC__FLIPPER_H
