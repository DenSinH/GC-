add_library(Core STATIC PPC/Gekko.c PPC/Gekko.h PPC/pipeline.h PPC/MMU.h PPC/MMU.c system.h system.c PPC/instructions/gekko_instruction.h PPC/instructions/instructions.h PPC/registers/condition_register.h PPC/registers/MSR.h PPC/registers/floating_point_status_condition_register.h PPC/registers/XER.h PPC/instructions/unimplemented.c PPC/instructions/integer_arithmetic.c PPC/instructions/integer_compare.c PPC/instructions/integer_logical.c PPC/instructions/integer_rotate_and_shift.c PPC/instructions/float_arithmetic.c PPC/instructions/float_mul_add.c PPC/instructions/float_rounding_and_conversion.c PPC/instructions/float_compare_and_status.c PPC/instructions/integer_load_store.c PPC/instructions/mem_sync.c PPC/instructions/float_load_store.c PPC/instructions/float_move.c PPC/instructions/branch_and_linkage.c PPC/instructions/condition_register.c PPC/instructions/trap_and_control.c PPC/instructions/misc_control.c PPC/instructions/paired_single_load_store.c PPC/instructions/paired_single_arithmetic.c PPC/instructions/paired_single_misc.c)

# for logging / stuff I want to use in all files
include_directories(../../include)

target_link_libraries(Core IPL)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})