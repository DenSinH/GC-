find_package(imgui CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

if (WIN32)
    find_library(Capstone capstone_dll REQUIRED)
else()
    find_package(Capstone REQUIRED)
endif()

find_path(Capstone_INCLUDE_DIR capstone.h PATH_SUFFIXES capstone)
set(Capstone_INCLUDE_DIR ${Capstone_INCLUDE_DIR}/../)

set(FRONTEND_SRCS
               imgui/imgui_impl_opengl3.cpp
               imgui/imgui_impl_sdl.cpp
        frontend.cpp
        widgets/menubar.cpp
        widgets/disassemble.c
        debugger.cpp)

set(FRONTEND_HDRS
               imgui/imgui_impl_opengl3.h
               imgui/imgui_impl_sdl.h
        widgets/memory_viewer.h
        widgets/console.h
        widgets/menubar.h
        widgets/register_viewer.h
        widgets/disassembly_viewer.h
        widgets/overlay.h
        widgets/disassemble.h
        debugger.h
        frontend.h
        interface.h
)

add_library(Frontend ${FRONTEND_SRCS} ${FRONTEND_HDRS})

set_target_properties(Frontend PROPERTIES
                      LINKER_LANGUAGE CXX
                      CXX_STANDARD 17
                      CXX_STANDARD_REQUIRED YES
                      CXX_EXTENSIONS ON)

target_link_libraries(Frontend PRIVATE imgui::imgui SDL2::SDL2 SDL2::SDL2main glad::glad)
if (WIN32)
    target_link_libraries(Frontend PRIVATE ${Capstone})
else()
    target_link_libraries(Frontend PRIVATE Capstone::Capstone)
endif()
target_include_directories(Frontend PRIVATE imgui ${PROJECT_SOURCE_DIR}/include)