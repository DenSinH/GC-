add_library(Core STATIC
        system.h
        system.c
        Breakpoints/breakpoints.h
        Breakpoints/breakpoints.c
        include/float_utils.h
        include/float_utils.c
        include/core_utils.h
        Scheduler/scheduler.c
        Scheduler/scheduler.h
)

add_subdirectory(HWregs)
add_subdirectory(PPC)
add_subdirectory(Flipper)

# for logging / stuff I want to use in all files
include_directories(${PROJECT_SOURCE_DIR}/include ./include
        $<TARGET_PROPERTY:HWregs,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:PPC,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Flipper,INCLUDE_DIRECTORIES>
        )

set_target_properties(Core PROPERTIES LINKER_LANGUAGE C)

target_link_libraries(Core Loader)
target_link_libraries(Core HWregs)
target_link_libraries(Core PPC)
target_link_libraries(Core Flipper)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})