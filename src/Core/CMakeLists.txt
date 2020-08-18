add_library(Core STATIC PPC/Gekko.c PPC/Gekko.h PPC/pipeline.h PPC/MMU.h PPC/MMU.c system.h system.c PPC/gekko_instruction.h PPC/instructions.c PPC/instructions.h PPC/registers/condition_register.h PPC/registers/MSR.h PPC/registers/floating_point_status_condition_register.h PPC/registers/XER.h)

# for logging / stuff I want to use in all files
include_directories(../../include)

target_link_libraries(Core IPL)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})