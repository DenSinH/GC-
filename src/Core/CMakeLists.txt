add_library(Core STATIC
        PPC/Gekko.c
        PPC/Gekko.h
        PPC/instructions.h
        PPC/instructions.c
        PPC/Gekko_init.c
        PPC/MMU.h
        PPC/MMU.c
        PPC/Registers/CR.h
        PPC/Registers/MSR.h
        PPC/Registers/FPSCR.h
        PPC/Registers/XER.h
        PPC/Registers/GQR.h
        PPC/Registers/HID.h
        PPC/Registers/FPR.h
        PPC/Registers/SPR.h
        PPC/Registers/WPAR.h
        PPC/Registers/SPR.c
        PPC/Registers/TBR.h
        system.h
        system.c
        Breakpoints/breakpoints.h
        Breakpoints/breakpoints.c
        PPC/gekko_instruction.h
        include/float_utils.h
        include/float_utils.c
        include/core_utils.h)

add_subdirectory(HWregs)
add_subdirectory(Flipper)

# for logging / stuff I want to use in all files
include_directories(../../include ./include)

set_target_properties(Core PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(Core Loader)
target_link_libraries(Core HWregs)
target_link_libraries(Core Flipper)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})