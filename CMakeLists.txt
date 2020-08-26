cmake_minimum_required(VERSION 3.17)
project(GC_ LANGUAGES CXX C)

find_package(Threads)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_executable(GC_ main.c include/default.h include/helpers.h include/flags.h include/log.h debugthread.h debugthread.c)

add_subdirectory(src/Loader)
add_subdirectory(src/Core)
add_subdirectory(src/Debugger)

# for logging / stuff I want to use in all files
include_directories(./include)

target_link_libraries(GC_ PRIVATE Core)
target_link_libraries(GC_ PRIVATE Debugger)

# add to turn on aggressive optimizations you can also add -O1 -O2 or -O3 to ADD_COMPILE_OPTIONS
# or -Ofast (super fast) or -Oz (small code and optimized)
# -O3 being the most aggressive
ADD_COMPILE_OPTIONS(-Wall -Werror -fsanitize=address -O3)
ADD_LINK_OPTIONS(-fsanitize=address)