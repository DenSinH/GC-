#include "Flipper.h"
#include "../system.h"

#include "shaders/shaders.h"
#include "shaders/GX_constants.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <glad.h>


void init_Flipper(s_Flipper* flipper) {
    flipper->memory = flipper->system->memory;
    flipper->CP = &flipper->system->HW_regs.CP;

    flipper->backdrop.r = 0.45;
    flipper->backdrop.g = 0.55f;
    flipper->backdrop.b = 0.60f;
}

static void debug_shader_init(s_Flipper* flipper, unsigned int shader) {
#ifndef NDEBUG
    int  success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf("Vertex shader compilation failed: %s\n", infoLog);
    }
#endif
}

static void debug_program_init(s_Flipper* flipper, unsigned int program) {
#ifndef NDEBUG
    int  success;
    glGetProgramiv(flipper->shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(flipper->shaderProgram, 512, NULL, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
    }
#endif
}

static void init_program(s_Flipper* flipper) {
    // create shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // compile it
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    debug_shader_init(flipper, vertexShader);

    // create and compile fragmentshader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    debug_shader_init(flipper, fragmentShader);

    // create program object
    flipper->shaderProgram = glCreateProgram();
    glAttachShader(flipper->shaderProgram, vertexShader);
    glAttachShader(flipper->shaderProgram, fragmentShader);
    glLinkProgram(flipper->shaderProgram);
    debug_program_init(flipper, flipper->shaderProgram);

    // dump shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

static void init_buffers(s_Flipper* flipper) {
    // create buffers
    glGenBuffers(21, flipper->VBOs);
    glGenBuffers(1, &flipper->EBO);

    // Setup VAO to bind the buffers to
    glGenVertexArrays(1, &flipper->VAO);
    glBindVertexArray(flipper->VAO);

    /* set up POS buffer */
    glBindBuffer(GL_ARRAY_BUFFER, flipper->VBOs[draw_arg_POS]);
    glVertexAttribPointer(POS_2D_U8, 2, GL_UNSIGNED_BYTE, GL_FALSE, 2 * sizeof(u8), (void*)0);
    glVertexAttribPointer(POS_2D_S8, 2, GL_BYTE, GL_FALSE, 2 * sizeof(i8), (void*)0);
    glVertexAttribPointer(POS_2D_U16, 2, GL_UNSIGNED_SHORT, GL_FALSE, 2 * sizeof(u16), (void*)0);
    glVertexAttribPointer(POS_2D_S16, 2, GL_SHORT, GL_FALSE, 2 * sizeof(i16), (void*)0);
    glVertexAttribPointer(POS_2D_F32, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    glVertexAttribPointer(POS_3D_U8, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(u8), (void*)0);
    glVertexAttribPointer(POS_3D_S8, 3, GL_BYTE, GL_FALSE, 3 * sizeof(i8), (void*)0);
    glVertexAttribPointer(POS_3D_U16, 3, GL_UNSIGNED_SHORT, GL_FALSE, 3 * sizeof(u16), (void*)0);
    glVertexAttribPointer(POS_3D_S16, 3, GL_SHORT, GL_FALSE, 3 * sizeof(i16), (void*)0);
    glVertexAttribPointer(POS_3D_F32, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    for (e_VA_locations i = 0; i <= POS_3D_F32; i++) {
        glEnableVertexAttribArray(i);
    }

    /* set up CLR0 buffer */
    glBindBuffer(GL_ARRAY_BUFFER, flipper->VBOs[draw_arg_CLR0]);
    glVertexAttribPointer(CLR0_rgb565, 1, GL_UNSIGNED_SHORT, GL_FALSE, sizeof(u16), (void*)0);
    glVertexAttribPointer(CLR0_rgb888, 3, GL_UNSIGNED_BYTE, GL_FALSE, 3 * sizeof(u8), (void*)0);
    glVertexAttribPointer(CLR0_rgb888x, 4, GL_UNSIGNED_BYTE, GL_FALSE, 4 * sizeof(u8), (void*)0);
    glVertexAttribPointer(CLR0_rgba4444, 2, GL_UNSIGNED_BYTE, GL_FALSE, 2 * sizeof(u8), (void*)0);
    glVertexAttribPointer(CLR0_rgba6666, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(u32), (void*)0);
    glVertexAttribPointer(CLR0_rgba8888, 4, GL_UNSIGNED_BYTE, GL_FALSE, 4 * sizeof(u8), (void*)0);

    for (e_VA_locations i = CLR0_rgb565; i <= CLR0_rgba8888; i++) {
        glEnableVertexAttribArray(i);
    }

    // unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void video_init_Flipper(s_Flipper* flipper) {
    // set up openGL stuff

    init_program(flipper);

    flipper->VCD_location = glGetUniformLocation(flipper->shaderProgram, "VCD");
    flipper->VAT_A_location = glGetUniformLocation(flipper->shaderProgram, "VAT_A");
    flipper->VAT_B_location = glGetUniformLocation(flipper->shaderProgram, "VAT_B");
    flipper->VAT_C_location = glGetUniformLocation(flipper->shaderProgram, "VAT_C");

    float vertices[] =
    {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f,
    };

    u32 colors[] = {
            0xffffffff,
            0xffffffff,
            0xffffffff,
    };

    init_buffers(flipper);
    glBindBuffer(GL_ARRAY_BUFFER, flipper->VBOs[draw_arg_POS]);   // bind to array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  // buffer data

    glBindBuffer(GL_ARRAY_BUFFER, flipper->VBOs[draw_arg_CLR0]);   // bind to array buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);  // buffer data
}

void draw_Flipper(s_Flipper* flipper, u16 n_vertices, u8 command) {
    u8 format = command & 7;

    s_VCD_lo VCD_lo = { .raw = flipper->CP->internalCPregs[0x50 + format - INTERNAL_CP_REGISTER_BASE] };
    s_VCD_hi VCD_hi = { .raw = flipper->CP->internalCPregs[0x60 + format - INTERNAL_CP_REGISTER_BASE] };
    s_VAT_A VAT_A = { .raw = flipper->CP->internalCPregs[0x70 + format - INTERNAL_CP_REGISTER_BASE] };
    s_VAT_B VAT_B = { .raw = flipper->CP->internalCPregs[0x80 + format - INTERNAL_CP_REGISTER_BASE] };
    s_VAT_C VAT_C = { .raw = flipper->CP->internalCPregs[0x90 + format - INTERNAL_CP_REGISTER_BASE] };

    for (int i = 0; flipper->CP->draw_arg_index[format][i] != draw_arg_UNUSED; i++) {
        switch (flipper->CP->draw_arg_index[format][i]) {
            case draw_arg_POS:

        }
    }
}

bool first = true;
void render_Flipper(s_Flipper* flipper) {

    glClearColor(flipper->backdrop.r, flipper->backdrop.g, flipper->backdrop.b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(flipper->shaderProgram);
    glBindVertexArray(flipper->VAO);
    glUniform1ui(flipper->VCD_location, 1);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    unsigned error = glGetError();
    if (error && first) {
        first = false;
        printf("Error: %08x\n", error);
    }
}