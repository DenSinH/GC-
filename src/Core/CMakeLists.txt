add_library(Core STATIC
        system.h
        system.c
        Breakpoints/breakpoints.h
        Breakpoints/breakpoints.c
        Scheduler/scheduler.cpp
        Scheduler/scheduler.h
        Scheduler/scheduler_impl.hpp
)

add_subdirectory(HW)
add_subdirectory(PPC)
add_subdirectory(Flipper)

# for logging / stuff I want to use in all files
include_directories(${PROJECT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        $<TARGET_PROPERTY:HW,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:PPC,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Flipper,INCLUDE_DIRECTORIES>
)

set_target_properties(Core PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(Core HW)
target_link_libraries(Core PPC)
target_link_libraries(Core Flipper)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})