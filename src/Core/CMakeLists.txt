add_library(Core STATIC
        PPC/Gekko.c
        PPC/Gekko.h
        PPC/instructions.h
        PPC/instructions.c
        PPC/Gekko_init.c
        PPC/MMU.h
        PPC/MMU.c
        system.h
        system.c
        Breakpoints/breakpoints.h
        Breakpoints/breakpoints.c
        PPC/gekko_instruction.h
        include/float_utils.h
        include/float_utils.c
        include/core_utils.h)
# for logging / stuff I want to use in all files
include_directories(../../include ./include)

set_target_properties(Core PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(PPC/Registers)

target_link_libraries(Core Loader)
target_link_libraries(Core Registers)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})