add_library(Core STATIC
        PPC/Gekko.c
        PPC/Gekko.h
        PPC/instructions.c
        PPC/Gekko_init.c
        PPC/MMU.h
        PPC/MMU.c
        system.h
        system.c
        Breakpoints/breakpoints.h
        Breakpoints/breakpoints.c
        PPC/gekko_instruction.h
        PPC/instructions.h
        PPC/registers/CR.h
        PPC/registers/MSR.h
        PPC/registers/FPSCR.h
        PPC/registers/XER.h
        PPC/registers/GQR.h
        PPC/registers/HID.h
        PPC/registers/FPR.h
        PPC/registers/SPR.h
        PPC/registers/hardware_registers/hardware_registers.h
        PPC/registers/hardware_registers/hardware_registers.c
        PPC/registers/TBR.h include/float_utils.h include/float_utils.c PPC/registers/WPAR.h PPC/registers/SPR.c)

# for logging / stuff I want to use in all files
include_directories(../../include ./include)

set_target_properties(Core PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(Core Loader)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})