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

#if SCREEN_TYPE == SCREEN_PAL

/*
 * PAL defines the horizontal active area as 52us of the 64us line.
 * */
const static size_t LINES_PER_FRAME = 625;
const static size_t DOTS_PER_LINE = 64 * GAMECUBE_SCREEN_WIDTH / 52;

#elif SCREEN_TYPE == SCREEN_NTSC
/*
 * NTSC defines the horizontal active area very vaguely
 * The total length is 63.555555.. us
 * a lot of sources give active areas of around 52.655555 us
 * */

const static size_t LINES_PER_FRAME = 525;
const static size_t DOTS_PER_LINE = 63.5555555 * GAMECUBE_SCREEN_WIDTH / 52.6555555;

#else
#error Incorrect screen type selected in flags.h
#endif

/*
 * The math:
 * CPU runs at 486 MHz, so that's 486M / (60fps * GAMECUBE_SCREEN_WIDTH * GAMECUBE_SCREEN_WIDTH) = 23.4 cycles per pixel
 *
 * */
#define CYCLES_PER_PIXEL 23

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
