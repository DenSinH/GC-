add_library(IPL STATIC loader.c loader.h)

# for logging / stuff I want to use in all files
include_directories(../../include)

target_include_directories(IPL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})