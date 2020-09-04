#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "debugger.h"
#include "widgets/menubar.h"
#include "widgets/overlay.h"
#include "widgets/console.h"
#include "widgets/register_viewer.h"
#include "widgets/disassembly_viewer.h"
#include "widgets/memory_viewer.h"

#include "Renderer/renderer_frontend.h"
#include <stdio.h>
#include <SDL.h>
#include <thread>
#include <glad/glad.h>


static struct s_debugger {
    ConsoleWidget console;
    RegisterViewer register_viewer;
    DisassemblyViewer disassembly_viewer;
    Overlay overlay;
    MemoryViewer memory_viewer;
} Debugger;

static struct s_frontend {
    bool* shutdown;
} Frontend;

void add_command(const char* command, const char* description, CONSOLE_COMMAND((*callback))) {
    Debugger.console.AddCommand(s_console_command {
            .command = command,
            .description = description,
            .callback = callback
    });
}

void add_register_data(char* name, const void* value, bool islong) {
    Debugger.register_viewer.AddRegister(name, value, islong);
}

void debugger_init(
        bool* shutdown,
        uint32_t* PC,
        uint8_t* memory,
        uint64_t mem_size,
        uint32_t (*valid_address_mask)(uint32_t),
        uint64_t* timer,
        uint8_t (*mem_read)(const uint8_t* data, uint64_t off)
        ) {
    Frontend.shutdown = shutdown;
    Debugger.disassembly_viewer.PC = PC;
    Debugger.disassembly_viewer.memory = memory;
    Debugger.disassembly_viewer.valid_address_mask = valid_address_mask;
    Debugger.overlay.timer = timer;
    Debugger.memory_viewer.mem_data = memory;
    Debugger.memory_viewer.mem_size = mem_size;
    Debugger.memory_viewer.ReadFn = mem_read;
}

// Main code
int ui_run()
{
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("GC-", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
    bool err = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress) == 0;
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    Debugger.overlay.io = &io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_console = true;
    bool show_register_viewer = true;
    bool show_disassembly_viewer = true;
    bool show_overlay = false;
    bool show_memory_viewer = true;

    // Main loop
    while (!(*Frontend.shutdown))
    {
        // Get events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                *Frontend.shutdown = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                *Frontend.shutdown = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        // render the widgets
        ShowMenuBar(
                &show_console,
                &show_register_viewer,
                &show_disassembly_viewer,
                &show_memory_viewer,
                &show_overlay
                );
        if (show_console)
            Debugger.console.Draw(&show_console);
        if (show_register_viewer)
            Debugger.register_viewer.Draw(&show_register_viewer);
        if (show_disassembly_viewer)
            Debugger.disassembly_viewer.Draw(&show_disassembly_viewer);
        if (show_overlay)
            Debugger.overlay.Draw(&show_overlay);
        if (show_memory_viewer)
            Debugger.memory_viewer.Draw(&show_memory_viewer);

#ifdef SHOW_EXAMPLE_MENU
        ImGui::ShowDemoWindow(NULL);
#endif

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        s_color backdrop = get_backdrop();
        glClearColor(backdrop.r, backdrop.g, backdrop.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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