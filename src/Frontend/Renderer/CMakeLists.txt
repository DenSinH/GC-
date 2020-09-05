find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_library(
        Renderer STATIC
        renderer_frontend.h
        renderer_frontend.c
        renderer_backend.h
        renderer_backend.c
        renderer_common.c renderer_common.h)

set_target_properties(Renderer PROPERTIES LINKER_LANGUAGE C)

target_include_directories(Renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDE_DIR})

target_link_libraries(Renderer PRIVATE ${SDL2_LIBRARY} glad::glad)