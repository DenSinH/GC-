add_library(Loader STATIC loader.c loader.h helpers.c helpers.h)

# for logging / stuff I want to use in all files
include_directories(../../include)

target_include_directories(Loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})