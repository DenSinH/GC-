#include "frontend.h"

#include "debugger.h"
#include "interface.h"
#include "controller.h"

#include <stdio.h>
#include <SDL.h>
#include <thread>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static struct s_frontend {
    ImGuiIO io;
    s_controller controller;
    void (*parse_input)(s_controller* controller);

    bool* shutdown;
    void (*video_init)();
    s_framebuffer (*render)();
} Frontend;

ImGuiIO *frontend_set_io() {
    Frontend.io = ImGui::GetIO();
    return &Frontend.io;
}

#ifdef __cplusplus
extern "C" {
#endif

void frontend_init(
        bool *shutdown,
        uint32_t *PC,
        uint8_t *memory,
        uint64_t mem_size,
        uint32_t (*valid_address_mask)(uint32_t),
        uint64_t *timer,
        uint8_t (*mem_read)(const uint8_t *data, uint64_t off),
        void (*parse_input)(s_controller* controller)
) {
    Frontend.shutdown = shutdown;
    Frontend.parse_input = parse_input;
    debugger_init(
            PC, memory, mem_size, valid_address_mask, timer, mem_read
    );
}

void bind_video_init(void (*initializer)()) {
    Frontend.video_init = initializer;
}

void bind_video_render(s_framebuffer (*render)()) {
    Frontend.render = render;
}

SDL_GameController* gamecontroller = NULL;

void init_gamecontroller() {
    if (SDL_NumJoysticks() < 0) {
        printf("No gamepads detected\n");
    }
    else {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            if (SDL_IsGameController(i)) {
                gamecontroller = SDL_GameControllerOpen(i);

                if (gamecontroller == NULL) {
                    printf("Failed to connect to gamecontroller at index %d\n", i);
                    continue;
                }

                printf("Connected game controller at index %d\n", i);
                return;
            }
        }
    }
    printf("No gamepads detected (only joysticks)\n");
}

int ui_run() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }
    init_controller_mapping(CONTROLLER_MAP_FILE);
    init_gamecontroller();

    // Decide GL+GLSL versions
#if __APPLE__
    const char* glsl_version = "#version 400";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    const char *glsl_version = "#version 400";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_WindowFlags window_flags = (SDL_WindowFlags) (SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | // SDL_WINDOW_RESIZABLE |
                                                      SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("GC-", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
                                          window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    debugger_video_init(glsl_version, window, &gl_context);
    Frontend.video_init();

    printf("Done initializing frontend\n");

    // Main loop
    while (!*Frontend.shutdown) {
        // Get events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type) {
                case SDL_QUIT:
                    *Frontend.shutdown = true;
                    break;
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_CLOSE &&
                        event.window.windowID == SDL_GetWindowID(window)) {
                        *Frontend.shutdown = true;
                    }
                    break;
                case SDL_CONTROLLERDEVICEADDED:
                    gamecontroller =  SDL_GameControllerOpen(event.cdevice.which);
                    if (gamecontroller == nullptr) {
                        printf("Error with connected gamecontroller\n");
                    }
                    break;
                case SDL_KEYDOWN:
                    // printf("pressed keyboard key %d\n", event.key.keysym.sym);
                    parse_keyboard_input(&Frontend.controller, event.key.keysym.sym, 1);
                    break;
                case SDL_KEYUP:
                    parse_keyboard_input(&Frontend.controller, event.key.keysym.sym, 0);
                    break;
                case SDL_CONTROLLERBUTTONDOWN:
                    // printf("pressed button %d\n", event.cbutton.button);
                    parse_controller_button(&Frontend.controller, event.cbutton.button, 1);
                    break;
                case SDL_CONTROLLERBUTTONUP:
                    parse_controller_button(&Frontend.controller, event.cbutton.button, 0);
                    break;
                case SDL_CONTROLLERAXISMOTION:
                    // printf("moved axis %d, %d\n", event.caxis.axis, event.caxis.value);
                    parse_controller_axis(&Frontend.controller, event.caxis.axis, event.caxis.value);
                    break;
                default:
                    break;
            }
        }

        if (Frontend.parse_input) {
            Frontend.parse_input(&Frontend.controller);
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        debugger_render();

#ifdef SHOW_EXAMPLE_MENU
        ImGui::ShowDemoWindow(NULL);
#endif
        // render actual emulation
        s_framebuffer emu_framebuffer = Frontend.render();
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

        float scale = (float) WINDOW_WIDTH / emu_framebuffer.dest_width;
        if ((float) WINDOW_HEIGHT / emu_framebuffer.dest_height < scale) {
            scale = (float) WINDOW_HEIGHT / emu_framebuffer.dest_height;
        }

        unsigned offsx = (WINDOW_WIDTH - scale * emu_framebuffer.dest_width) / 2;
        unsigned offsy = (WINDOW_HEIGHT - scale * emu_framebuffer.dest_height) / 2;

        float x_start = -1.0 + 2 * offsx / (float)WINDOW_WIDTH;
        float y_start = -1.0 + 2 * offsy / (float)WINDOW_HEIGHT;
        float dest_width = -2 * x_start;
        float dest_height = -2 * y_start;

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, emu_framebuffer.id);
        glBlitFramebuffer(0, 0,
                          emu_framebuffer.src_width, emu_framebuffer.src_height,
                          offsx, offsy,
                          offsx + scale * emu_framebuffer.dest_width, offsy + scale * emu_framebuffer.dest_height,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST
        );

        // blit the overlay
        if (emu_framebuffer.draw_overlay) {
            emu_framebuffer.draw_overlay(
                emu_framebuffer.caller,
                x_start, y_start,
                dest_width, dest_height
            );
        }

        // then draw the imGui stuff over it
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // frameswap
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#ifdef __cplusplus
};
#endif