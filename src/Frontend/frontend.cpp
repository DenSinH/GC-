#include "frontend.h"
#include "debugger.h"
#include "interface.h"

#include <stdio.h>
#include <SDL.h>
#include <thread>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static struct s_frontend {
    ImGuiIO io;
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
        uint8_t (*mem_read)(const uint8_t *data, uint64_t off)
) {
    Frontend.shutdown = shutdown;
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

int ui_run() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

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
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
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

    // Main loop
    while (!*Frontend.shutdown) {
        // Get events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                *Frontend.shutdown = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(window))
                *Frontend.shutdown = true;
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
        glViewport(0, 0, (int) Frontend.io.DisplaySize.x, (int) Frontend.io.DisplaySize.y);

        float scale = (float) WINDOW_WIDTH / emu_framebuffer.width;
        if ((float) WINDOW_HEIGHT / emu_framebuffer.height  < scale) {
            scale = (float) WINDOW_HEIGHT / emu_framebuffer.height;
        }
        unsigned offsx = (WINDOW_WIDTH - scale * emu_framebuffer.width) / 2;
        unsigned offsy = (WINDOW_HEIGHT - scale * emu_framebuffer.height) / 2;

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, emu_framebuffer.id);
        glBlitFramebuffer(0, 0,
                          emu_framebuffer.width, emu_framebuffer.height,
                          offsx, offsy,
                          offsx + scale * emu_framebuffer.width, offsy + scale * emu_framebuffer.height,
                          GL_COLOR_BUFFER_BIT, GL_NEAREST
        );

        // then draw the imGui stuff over it
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // frameswap
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

#ifdef __cplusplus
};
#endif