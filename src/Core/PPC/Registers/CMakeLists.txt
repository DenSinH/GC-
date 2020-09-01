add_library(Registers STATIC
        CR.h
        MSR.h
        FPSCR.h
        XER.h
        GQR.h
        HID.h
        FPR.h
        SPR.h
        WPAR.h
        SPR.c
        TBR.h
        hardware_registers/hardware_registers.h
        hardware_registers/hardware_registers.c
        hardware_registers/CommandProcessor.h
        hardware_registers/PixelEngine.h
        hardware_registers/CommandProcessor.c
        hardware_registers/PixelEngine.c
        hardware_registers/VideoInterface.c
        hardware_registers/VideoInterface.h
        hardware_registers/ProcessorInterface.c
        hardware_registers/ProcessorInterface.h
        hardware_registers/DigitalSoundProcessorInterface.c
        hardware_registers/DigitalSoundProcessorInterface.h
        hardware_registers/DVDInterface.c
        hardware_registers/DVDInterface.h
        hardware_registers/SerialInterface.c
        hardware_registers/SerialInterface.h
        hardware_registers/ExternalInterface.c
        hardware_registers/ExternalInterface.h
        hardware_registers/AudioStreamingInterface.c
        hardware_registers/AudioStreamingInterface.h
        hardware_registers/GXFIFO.c
        hardware_registers/GXFIFO.h)


# for logging / stuff I want to use in all files
include_directories(../../include ./include)

set_target_properties(Registers PROPERTIES LINKER_LANGUAGE C)

target_include_directories(Registers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})