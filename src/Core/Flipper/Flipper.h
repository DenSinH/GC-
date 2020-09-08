#ifndef GC__FLIPPER_H
#define GC__FLIPPER_H

#include "default.h"

#include <stdbool.h>

#define FLIPPER_TEMP_BUFFER_SIZE 0x10000

typedef struct s_Flipper {

    struct s_GameCube* system;
    u8* memory;
    struct s_CP* CP;

    struct {
        float r, g, b;
    } backdrop;

    u16 n_vertices;
    u8 command;
    u8 temp_buffer[FLIPPER_TEMP_BUFFER_SIZE];

    unsigned int VBOs[21];
    unsigned int EBO;
    unsigned int VAO;
    int VCD_location, VAT_A_location, VAT_B_location, VAT_C_location;
    unsigned int shaderProgram;

    volatile bool draw_ready;

} s_Flipper;

void init_Flipper(s_Flipper* flipper);

void video_init_Flipper(s_Flipper* gpu);
void queue_draw_Flipper(s_Flipper* flipper, u16 n_vertices, u8 command);
void render_Flipper(s_Flipper* flipper);

#endif //GC__FLIPPER_H
