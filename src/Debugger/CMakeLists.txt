find_package(SDL2 CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)

set(IMGUI_SRCS imgui/imgui.cpp
               imgui/imgui_demo.cpp
               imgui/imgui_draw.cpp
               imgui/imgui_impl_opengl3.cpp
               imgui/imgui_impl_sdl.cpp
               imgui/imgui_widgets.cpp
               debugger.cpp
    )

set(IMGUI_HDRS imgui/imconfig.h
               imgui/imgui.h
               imgui/imgui_impl_opengl3.h
               imgui/imgui_impl_sdl.h
               imgui/imgui_internal.h
               imgui/imstb_rectpack.h
               imgui/imstb_textedit.h
               imgui/imstb_truetype.h
               debugger.h
        widgets/console.h
        widgets/menubar.h widgets/menubar.cpp widgets/register_viewer.h)

add_library(Debugger ${IMGUI_SRCS} ${IMGUI_HDRS})

set_target_properties(Debugger PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS ON)

target_link_libraries(Debugger PRIVATE SDL2::SDL2 SDL2::SDL2main unofficial::gl3w::gl3w)
target_include_directories(Debugger PRIVATE imgui)