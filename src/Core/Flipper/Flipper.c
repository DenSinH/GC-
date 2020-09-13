#include "Flipper.h"
#include "../HWregs/CommandProcessor.h"
#include "../system.h"
#include "../../Frontend/interface.h"

#include "shaders/shaders.h"
#include "shaders/GX_constants.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <glad.h>


/*
 * All of this code should run in the rendering thread
 * */

#define FENCE_SYNC_TIMEOUT_NS 0xffffffff  // must fit into a u64
static u16 quad_EBO[FLIPPER_QUAD_INDEX_ARRAY_SIZE];


void init_Flipper(s_Flipper* flipper) {
    flipper->memory = flipper->system->memory;
    flipper->CP = &flipper->system->HW_regs.CP;

    int i = 0;
    for (u16 v = 0; i + 6 < FLIPPER_QUAD_INDEX_ARRAY_SIZE; v += 4) {
        // triangle fan structure
        quad_EBO[i++] = v;
        quad_EBO[i++] = v + 1;
        quad_EBO[i++] = v + 2;
        quad_EBO[i++] = v;
        quad_EBO[i++] = v + 2;
        quad_EBO[i++] = v + 3;
    }
}

#define QUAD_EBO_COUNT(n_vertices) (((n_vertices) >> 2) * 6)

static void debug_shader_init(s_Flipper* flipper, unsigned int shader) {
    int  success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        log_fatal("Vertex shader compilation failed: %s\n", infoLog);
    }
}

static void debug_program_init(s_Flipper* flipper, unsigned int program) {
    int  success;
    glGetProgramiv(flipper->shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        char infoLog[512];
        glGetProgramInfoLog(flipper->shaderProgram, 512, NULL, infoLog);
        log_fatal("Shader program linking failed: %s\n", infoLog);
    }
}

static void init_framebuffer(s_Flipper* flipper) {
    // create framebuffer for flipper
    for (int i = 0; i < 1; i++) {
        glGenFramebuffers(1, &flipper->framebuffer[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, flipper->framebuffer[i]);

        GLuint rendered_texture;
        // create a texture to render to and fill it with 0 (also set filtering to low)
        glGenTextures(1, &rendered_texture);
        glBindTexture(GL_TEXTURE_2D, rendered_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FLIPPER_SCREEN_WIDTH, FLIPPER_SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rendered_texture, 0);
        GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, draw_buffers);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            log_fatal("Error initializing framebuffer");
        }
    }
}

static void init_program(s_Flipper* flipper) {
    // create shader
    unsigned int vertexShader, transformationShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    transformationShader = glCreateShader(GL_VERTEX_SHADER);

    // compile it
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    debug_shader_init(flipper, vertexShader);

    glShaderSource(transformationShader, 1, &transformationShaderSource, NULL);
    glCompileShader(transformationShader);
    debug_shader_init(flipper, transformationShader);

    // create and compile fragmentshader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    debug_shader_init(flipper, fragmentShader);

    // create program object
    flipper->shaderProgram = glCreateProgram();
    glAttachShader(flipper->shaderProgram, vertexShader);
    glAttachShader(flipper->shaderProgram, transformationShader);
    glAttachShader(flipper->shaderProgram, fragmentShader);
    glLinkProgram(flipper->shaderProgram);
    debug_program_init(flipper, flipper->shaderProgram);

    // dump shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

static void init_buffers(s_Flipper* flipper) {
    // create buffers
    glGenBuffers(1, &flipper->command_SSBO);
    glGenBuffers(1, &flipper->XF_SSBO);

    // Setup VAO to bind the buffers to
    glGenVertexArrays(1, &flipper->VAO);
    glBindVertexArray(flipper->VAO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->command_SSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, flipper->command_SSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->XF_SSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, flipper->XF_SSBO);

    // bind the quad EBO to the VAO
    glGenBuffers(1, &flipper->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, flipper->EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void video_init_Flipper(s_Flipper* flipper) {

    init_framebuffer(flipper);
    init_program(flipper);

    flipper->VCD_lo_location = glGetUniformLocation(flipper->shaderProgram, "VCD_lo");
    flipper->VCD_hi_location = glGetUniformLocation(flipper->shaderProgram, "VCD_hi");
    flipper->VAT_A_location = glGetUniformLocation(flipper->shaderProgram, "VAT_A");
    flipper->VAT_B_location = glGetUniformLocation(flipper->shaderProgram, "VAT_B");
    flipper->VAT_C_location = glGetUniformLocation(flipper->shaderProgram, "VAT_C");

    flipper->MATIDX_REG_A_location = glGetUniformLocation(flipper->shaderProgram, "MATIDX_REG_A");
    flipper->MATIDX_REG_B_location = glGetUniformLocation(flipper->shaderProgram, "MATIDX_REG_B");

    init_buffers(flipper);
}

void clear_Flipper_buffer(s_Flipper* flipper) {
    // clear (emulator) framebuffer to the backdrop color
    u16 AR = (u16)flipper->CP->internal_BP_regs[BP_reg_int_PE_copy_clear_AR];
    u16 GB = (u16)flipper->CP->internal_BP_regs[BP_reg_int_PE_copy_clear_GB];

    glClearColor((float)(AR & 0xff) / 255.0, (float)(GB >> 8) / 255.0, (float)(GB & 0xff) / 255.0, (float)(AR >> 8) / 255.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

// GL_QUADS is depricated, we have to separate case this by using an EBO for the vertices
static const GLenum draw_commands[8] = {
        0, 0, GL_TRIANGLES, GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN, GL_LINES, GL_LINE_STRIP, GL_POINTS
};

void draw_Flipper(s_Flipper* flipper, s_draw_command_small* command) {
    glBindVertexArray(flipper->VAO);

    // buffer command data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->command_SSBO);
    glBufferData(
            GL_SHADER_STORAGE_BUFFER,
            sizeof(s_draw_command_small) - DRAW_COMMAND_DATA_BUFFER_SIZE + command->data_size,
            command,
            GL_STATIC_COPY
    );

    // todo: cache this somehow?
    log_flipper("buffer %x bytes of command data",
                sizeof(s_draw_command_small) - DRAW_COMMAND_DATA_BUFFER_SIZE + command->data_size)

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->XF_SSBO);
    glBufferData(
            GL_SHADER_STORAGE_BUFFER,
            sizeof(flipper->CP->internal_XF_mem) + sizeof(flipper->CP->internal_XF_regs),
            &flipper->CP->internal_XF_mem,
            GL_STATIC_COPY
    );

    log_flipper("loaded %x bytes of XF data", sizeof(flipper->CP->internal_XF_mem) + sizeof(flipper->CP->internal_XF_regs));

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    glUseProgram(flipper->shaderProgram);
    glBindVertexArray(flipper->VAO);

    // pass relevant registers as uniforms
    u8 format = command->command & 7;
    glUniform1ui(flipper->VCD_lo_location, get_internal_CP_reg(flipper->CP, CP_reg_int_VCD_lo_base + format));
    glUniform1ui(flipper->VCD_hi_location, get_internal_CP_reg(flipper->CP, CP_reg_int_VCD_hi_base + format));
    glUniform1ui(flipper->VAT_A_location, get_internal_CP_reg(flipper->CP, CP_reg_int_VAT_A_base + format));
    glUniform1ui(flipper->VAT_B_location, get_internal_CP_reg(flipper->CP, CP_reg_int_VAT_B_base + format));
    glUniform1ui(flipper->VAT_C_location, get_internal_CP_reg(flipper->CP, CP_reg_int_VAT_C_base + format));

    glUniform1ui(flipper->MATIDX_REG_A_location, get_internal_CP_reg(flipper->CP, CP_reg_int_MATIDX_REG_A));
    glUniform1ui(flipper->MATIDX_REG_B_location, get_internal_CP_reg(flipper->CP, CP_reg_int_MATIDX_REG_B));

    log_flipper("Drawing command %02x, vertices %d", command->command, command->vertices)
    if ((command->command & 0xf8) == 0x80) {
        // quads need separate case for the indices since GL_QUADS is deprecated
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, flipper->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * QUAD_EBO_COUNT(command->vertices),
                     quad_EBO, GL_STATIC_DRAW);
        glDrawElements(GL_TRIANGLES, QUAD_EBO_COUNT(command->vertices), GL_UNSIGNED_SHORT, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    else {
        glDrawArrays(draw_commands[(command->command >> 3) & 7], 0, command->vertices);
    }

}

bool first = true;
struct s_framebuffer render_Flipper(s_Flipper* flipper){

    // bind our framebuffer
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, flipper->framebuffer[flipper->current_framebuffer]);
    glViewport(0, 0, FLIPPER_SCREEN_WIDTH, FLIPPER_SCREEN_HEIGHT);

    if (!flipper->CP->draw_command_available[flipper->draw_command_index]) {
        clear_Flipper_buffer(flipper);

        while (!flipper->CP->draw_command_available[flipper->draw_command_index]) {
            log_flipper("Processing draw command %d", flipper->draw_command_index);
            // wait until either a fail (GL_WAIT_FAILED) or a successful draw command (GL_ALREADY_SIGNALED, GL_TIMEOUT_EXPIRED)
            // first do a non-blocking check to see if we are already done
            if (glClientWaitSync(flipper->fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0) != GL_ALREADY_SIGNALED) {
                while (glClientWaitSync(flipper->fence, GL_SYNC_FLUSH_COMMANDS_BIT, FENCE_SYNC_TIMEOUT_NS) == GL_TIMEOUT_EXPIRED) {}
            }

            // draw and set draw command slot to available in CP
            draw_Flipper(flipper, &flipper->CP->draw_command_queue[flipper->draw_command_index]);
            flipper->CP->draw_command_available[flipper->draw_command_index++] = true;
            if (flipper->draw_command_index == MAX_DRAW_COMMANDS) {
                flipper->draw_command_index = 0;
            }
            flipper->fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        }

        flipper->current_framebuffer ^= true;  // frameswap for the emulator
    }

    unsigned error = glGetError();
    if (error && first) {
        first = false;
        printf("Error: %08x\n", error);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // return the framebuffer that is "ready"
    return (s_framebuffer) {
        .id = flipper->framebuffer[!flipper->current_framebuffer],
        .width = FLIPPER_SCREEN_WIDTH,
        .height = FLIPPER_SCREEN_HEIGHT
    };
}