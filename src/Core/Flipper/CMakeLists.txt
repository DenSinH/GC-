find_package(SDL2 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

file(GLOB VERT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/raw/*.vert)

if (WIN32)
    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shaders.h
            COMMENT "Generating shaders/shaders.h"
            COMMAND py ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shaders.py
            DEPENDS ${VERT_FILES}
    )
else()
    # linux/mac use python in command line instead of py
    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shaders.h
            COMMENT "Generating shaders/shaders.h"
            COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shaders.py
            DEPENDS ${VERT_FILES}
    )
endif()


set_source_files_properties(
        shaders/shaders.h PROPERTIES
        GENERATED TRUE
)

add_library(
        Flipper STATIC
        shaders/shaders.h
        Flipper.c
        Flipper.h
)

set_target_properties(Flipper PROPERTIES LINKER_LANGUAGE C)

target_include_directories(Flipper PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${SDL2_INCLUDE_DIR} ${GLAD_INCLUDE_DIR})

target_link_libraries(Flipper PRIVATE ${SDL2_LIBRARY} glad::glad)