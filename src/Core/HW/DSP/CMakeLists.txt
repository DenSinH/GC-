add_library(DSP STATIC
        DSP.c
        DSP.h
        DSP_memory.h
        DSP_memory.c
        DSP_instructions.c
        DSP_instructions.h
        DSP_extended_instructions.c
        DSP_extended_instructions.h
        DSP_util.h)

add_executable(
        DSP_standalone
        main.c
)

target_link_libraries(DSP_standalone DSP)


# for logging / stuff I want to use in all files
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ../include
        ../../include
        ../PPC/include
)

set_target_properties(DSP PROPERTIES LINKER_LANGUAGE C)

target_include_directories(DSP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )