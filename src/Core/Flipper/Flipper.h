#ifndef GC__FLIPPER_H
#define GC__FLIPPER_H

#include "default.h"
#include "flags.h"

#include <stdbool.h>

#define GAMECUBE_SCREEN_WIDTH 640
#define GAMECUBE_SCREEN_HEIGHT 480

// full HD framebuffer, to be rescaled when blitted to the frontend screen
#define FLIPPER_FRAMEBUFFER_WIDTH 1920
#define FLIPPER_FRAMEBUFFER_HEIGHT 1080
#define FLIPPER_QUAD_INDEX_ARRAY_SIZE 0x8000

typedef struct s_Flipper {

    struct s_GameCube* system;
    u8* memory;
    struct s_CP* CP;
    struct s_VI* VI;

    // todo: make this variable through VI
    u32 screen_width, screen_height;

    unsigned int framebuffer[2];
    bool current_framebuffer;

    u32 draw_command_index;

    // todo: UBO for XF mem (faster)
    unsigned int XF_SSBO, BP_SSBO;
    unsigned int draw_VAO, quad_EBO, command_SSBO, texture_SSBO;
    unsigned int efb_VAO, efb_VBO, efb_EBO;
    unsigned int efb_texture;
    u32 efb_width, efb_height;
    void* fence;

    unsigned int VCD_lo_location, VCD_hi_location, VAT_A_location, VAT_B_location, VAT_C_location;
    unsigned int MATIDX_REG_A_location, MATIDX_REG_B_location;
    unsigned int efb_width_location, efb_location;
    unsigned int draw_program, efb_program;
} s_Flipper;

void init_Flipper(s_Flipper* flipper);

void video_init_Flipper(s_Flipper* gpu);
struct s_framebuffer render_Flipper(s_Flipper* flipper);

#endif //GC__FLIPPER_H
