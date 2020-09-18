file(GLOB GLSL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/shaders/raw/*.glsl)

message(STATUS "Found GLSL shaders: ${GLSL_FILES}")

if (WIN32)
    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shaders.h
            COMMENT "Generating shaders/shaders.h"
            COMMAND py ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shaders.py
            DEPENDS ${GLSL_FILES}
    )
else()
    # linux/mac use python in command line instead of py
    add_custom_command(
            OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shaders.h
            COMMENT "Generating shaders/shaders.h"
            COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/shaders/shaders.py
            DEPENDS ${GLSL_FILES}
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
        shaders/GX_constants.h
)

set_target_properties(Flipper PROPERTIES LINKER_LANGUAGE C)

target_include_directories(Flipper PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include
        ../include
)