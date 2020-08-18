add_library(IPL STATIC loader.c)
target_include_directories(IPL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})