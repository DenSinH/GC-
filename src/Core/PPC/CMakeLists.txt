add_library(
        PPC STATIC
        Gekko.c
        Gekko.h
        instructions.h
        instructions.c
        Gekko_init.c
        MMU.h
        MMU.c
        Registers/CR.h
        Registers/MSR.h
        Registers/FPSCR.h
        Registers/XER.h
        Registers/GQR.h
        Registers/HID.h
        Registers/FPR.h
        Registers/SPR.h
        Registers/WPAR.h
        Registers/SPR.c
        Registers/TBR.h
        gekko_instruction.h
        interrupts.c
        interrupts.h
        float_utils.c
        include/float_utils.h
)

set_target_properties(PPC PROPERTIES LINKER_LANGUAGE C)

add_subdirectory(Loader)
target_link_libraries(PPC Loader)

target_include_directories(PPC PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include
        ../include
        ../HW/include
)