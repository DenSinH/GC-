add_library(HWregs STATIC
        hardware_registers.h
        hardware_registers.c
        CommandProcessor.h
        PixelEngine.h
        CommandProcessor.c
        PixelEngine.c
        VideoInterface.c
        VideoInterface.h
        ProcessorInterface.c
        ProcessorInterface.h
        DigitalSoundProcessorInterface.c
        DigitalSoundProcessorInterface.h
        DVDInterface.c
        DVDInterface.h
        SerialInterface.c
        SerialInterface.h
        ExternalInterface.c
        ExternalInterface.h
        AudioStreamingInterface.c
        AudioStreamingInterface.h
        GX_FIFO.c
        GX_FIFO.h
        MemoryInterface.c
        MemoryInterface.h
        ../include/hwreg_utils.h)


# for logging / stuff I want to use in all files
include_directories(../../../include ../include)

set_target_properties(HWregs PROPERTIES LINKER_LANGUAGE C)

target_include_directories(HWregs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})