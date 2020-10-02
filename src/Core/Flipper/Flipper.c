#include "Flipper.h"

#include "custom_threading.h"

#include "../HWregs/CommandProcessor.h"
#include "../HWregs/VideoInterface.h"
#include "../system.h"

#include "../../Frontend/interface.h"

#include "shaders/shaders.h"
#include "shaders/GX_constants.h"

#include <stdio.h>
#include <glad/glad.h>


/*
 * All of this code should run in the rendering thread
 * */

#define FENCE_SYNC_TIMEOUT_NS 0xffffffff  // must fit into a u64
static u16 quad_EBO[FLIPPER_QUAD_INDEX_ARRAY_SIZE];


void GLAPIENTRY debug_opengl(
                 GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* user_param
) {
    if (severity >= GL_DEBUG_SEVERITY_LOW) {
        log_gl("[%x] %s",severity, message);
    }
}


void init_Flipper(s_Flipper* flipper) {
    flipper->memory = flipper->system->memory;
    flipper->CP = &flipper->system->HW_regs.CP;
    flipper->VI = &flipper->system->HW_regs.VI;

    flipper->screen_width = GAMECUBE_SCREEN_WIDTH;
    flipper->screen_height = GAMECUBE_SCREEN_HEIGHT;

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
        log_fatal("Shader compilation failed: %s\n", infoLog);
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
    // I don't know why, but doing this fixed a flickering issue
    for (int i = 1; i >= 0; i--) {
        glGenFramebuffers(1, &flipper->framebuffer[i]);
        glBindFramebuffer(GL_FRAMEBUFFER, flipper->framebuffer[i]);

        GLuint rendered_texture, depth_buffer;
        // create a texture to render to and fill it with 0 (also set filtering to low)
        glGenTextures(1, &rendered_texture);
        glBindTexture(GL_TEXTURE_2D, rendered_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, FLIPPER_FRAMEBUFFER_WIDTH, FLIPPER_FRAMEBUFFER_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // add depth buffer
        glGenRenderbuffers(1, &depth_buffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, FLIPPER_FRAMEBUFFER_WIDTH, FLIPPER_FRAMEBUFFER_HEIGHT);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, rendered_texture, 0);
        GLenum draw_buffers[1] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, draw_buffers);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
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
    glGenBuffers(1, &flipper->texture_SSBO);
    glGenBuffers(1, &flipper->BP_SSBO);
    glGenBuffers(1, &flipper->XF_SSBO);

    // Setup VAO to bind the buffers to
    glGenVertexArrays(1, &flipper->VAO);
    glBindVertexArray(flipper->VAO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->command_SSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, flipper->command_SSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->texture_SSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, flipper->texture_SSBO);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->BP_SSBO);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, flipper->BP_SSBO);

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

    // flipper clipping happens between 0 and 1
    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);  // todo: base this on BP register
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if COMPONENT_FLAGS & COMPONENT_OPENGL
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(debug_opengl, NULL);
#endif
}

static inline void wait_for_fence(s_Flipper* flipper) {
    // wait until either a fail (GL_WAIT_FAILED) or a successful draw command (GL_ALREADY_SIGNALED, GL_TIMEOUT_EXPIRED)
    // first do a non-blocking check to see if we are already done
    if (glClientWaitSync(flipper->fence, GL_SYNC_FLUSH_COMMANDS_BIT, 0) != GL_ALREADY_SIGNALED) {
        while (glClientWaitSync(flipper->fence, GL_SYNC_FLUSH_COMMANDS_BIT, FENCE_SYNC_TIMEOUT_NS) == GL_TIMEOUT_EXPIRED) {}
    }
    flipper->fence = NULL;
}

void Flipper_frameswap(s_Flipper* flipper, u16 pe_copy_command) {
    log_flipper("Frameswapping flipper");
    // frameswap
    flipper->fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

    flipper->current_framebuffer ^= true;
    glBindFramebuffer(GL_FRAMEBUFFER, flipper->framebuffer[flipper->current_framebuffer]);
    glViewport(0, 0, FLIPPER_FRAMEBUFFER_WIDTH, FLIPPER_FRAMEBUFFER_HEIGHT);

    if (pe_copy_command & PE_copy_clear) {
        // clear (emulator) framebuffer to the backdrop color
        u16 AR = (u16)flipper->CP->internal_BP_regs[BP_reg_int_PE_copy_clear_AR];
        u16 GB = (u16)flipper->CP->internal_BP_regs[BP_reg_int_PE_copy_clear_GB];

        glClearColor((float)(AR & 0xff) / 255.0, (float)(GB >> 8) / 255.0, (float)(GB & 0xff) / 255.0, (float)(AR >> 8) / 255.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

// GL_QUADS is depricated, we have to separate case this by using an EBO for the vertices
static const GLenum draw_commands[8] = {
        0, 0, GL_TRIANGLES, GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN, GL_LINES, GL_LINE_STRIP, GL_POINTS
};

void draw_Flipper(s_Flipper* flipper, s_draw_command* command, s_texture_data* texture_data) {
    acquire_mutex(&flipper->CP->draw_lock);
    glBindVertexArray(flipper->VAO);

    // buffer command data
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->command_SSBO);
    glBufferData(
            GL_SHADER_STORAGE_BUFFER,
            sizeof(s_draw_command) - DRAW_COMMAND_DATA_BUFFER_SIZE + command->data_size,
            command,
            GL_STATIC_COPY
    );

    // todo: cache this somehow?
    log_flipper("buffer %lx bytes of command data",
                sizeof(s_draw_command) - DRAW_COMMAND_DATA_BUFFER_SIZE + command->data_size);

    // buffer texture data (if necessary)
    if (texture_data->data_size) {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->texture_SSBO);
        glBufferData(
                GL_SHADER_STORAGE_BUFFER,
                sizeof(s_texture_data) - DRAW_COMMAND_TEXTURE_BUFFER_SIZE + texture_data->data_size,
                texture_data,
                GL_STATIC_COPY
        );

        log_flipper("buffer %lx bytes of texture data",
                    sizeof(s_texture_data) - DRAW_COMMAND_TEXTURE_BUFFER_SIZE + texture_data->data_size);
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->BP_SSBO);
    glBufferData(
            GL_SHADER_STORAGE_BUFFER,
            sizeof(flipper->CP->internal_BP_regs),
            &flipper->CP->internal_BP_regs,
            GL_STATIC_COPY
    );

    log_flipper("loaded %lx bytes of BP regs", sizeof(flipper->CP->internal_BP_regs));

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, flipper->XF_SSBO);
    glBufferData(
            GL_SHADER_STORAGE_BUFFER,
            sizeof(flipper->CP->internal_XF_mem) + sizeof(flipper->CP->internal_XF_regs),
            &flipper->CP->internal_XF_mem,
            GL_STATIC_COPY
    );

    log_flipper("loaded %lx bytes of XF data", sizeof(flipper->CP->internal_XF_mem) + sizeof(flipper->CP->internal_XF_regs));

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

    log_flipper("Drawing command %02x, vertices %d", command->command, command->vertices);
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
    release_mutex(&flipper->CP->draw_lock);
}

struct s_framebuffer render_Flipper(s_Flipper* flipper){

    acquire_mutex(&flipper->CP->availability_lock);
    if (!flipper->CP->draw_command_available[flipper->draw_command_index]) {
        // bind our framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, flipper->framebuffer[flipper->current_framebuffer]);
        glViewport(0, 0, FLIPPER_FRAMEBUFFER_WIDTH, FLIPPER_FRAMEBUFFER_HEIGHT);

        u32 start_draw_index = flipper->draw_command_index;

        // from the first if statement, we know at least one draw command is available
        do {
            log_flipper("Processing draw command %02x @%d",
                        flipper->CP->draw_command_queue[flipper->draw_command_index].command,
                        flipper->draw_command_index
            );

            // draw and set draw command slot to available in CP
            draw_Flipper(
                    flipper,
                    &flipper->CP->draw_command_queue[flipper->draw_command_index],
                    &flipper->CP->texture_data[flipper->draw_command_index]
            );

            /*
             * Funnily enough, the way I handle my draw commands is actually pretty similar to the way the GameCube
             * handles it: I send draw commands, put them in a buffer to wait to be drawn by the GPU,
             * then I prevent it from passing the CP (similar to watermarks).
             *
             * I then explicitly wait for the GPU to finish the current commands before proceeding (this is actually
             * different from the GameCube probably, where it is handled in interrupts, I don't know how yet).
             * */

            // check if we need to frameswap
            if (flipper->CP->frameswap[flipper->draw_command_index] & PE_copy_execute) {
                Flipper_frameswap(flipper, flipper->CP->frameswap[flipper->draw_command_index]);
            }

            if (++flipper->draw_command_index == MAX_DRAW_COMMANDS) {
                flipper->draw_command_index = 0;
            }
        } while (!flipper->CP->draw_command_available[flipper->draw_command_index]
                 && flipper->draw_command_index != flipper->CP->draw_command_index);

        // mark all draw commands as available again
        u32 i = start_draw_index;
        // use do/while loop so that we can let the flipper draw_command_index catch up with the CP draw_command_index
        do {
            flipper->CP->draw_command_available[i] = true;

            if (++i == MAX_DRAW_COMMANDS) {
                i = 0;
            }
        } while (i != flipper->draw_command_index);

        log_flipper("Done drawing commands");

//        static bool first;
//        unsigned error = glGetError();
//        if (error && first) {
//            first = false;
//            printf("Error: %08x\n", error);
//        }

        // unbind framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    release_mutex(&flipper->CP->availability_lock);

    if (flipper->fence) {
        // frameswap happened, wait for draw commands to complete
        wait_for_fence(flipper);
    }

    // return the framebuffer that is "ready"
    return (s_framebuffer) {
        .id = flipper->framebuffer[!flipper->current_framebuffer],
        .src_width = FLIPPER_FRAMEBUFFER_WIDTH,
        .src_height = FLIPPER_FRAMEBUFFER_HEIGHT,
        .dest_width = flipper->screen_width,
        .dest_height = flipper->screen_height
    };
}