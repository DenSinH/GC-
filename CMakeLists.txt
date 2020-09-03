cmake_minimum_required(VERSION 3.17)
project(GC_ LANGUAGES CXX C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_C_FLAGS_RELEASE "-Ofast")
# add_compile_options(-Wall $<$<CONFIG:_DEBUG>:-fsanitize=address>)
add_compile_options(
        -Wno-gnu-binary-literal
        -Wno-visibility
        -Wno-c++98-compat
        -Wno-double-promotion
)
add_compile_definitions(
        $<$<CONFIG:RELEASE>:NDEBUG>
        $<$<CONFIG:RELEASE>:BOOST_DISABLE_ASSERTS>
)
# add_link_options($<$<CONFIG:_DEBUG>:-fsanitize=address>)

add_executable(GC_
        main.c
        include/default.h
        include/helpers.h
        include/flags.h
        include/log.h
        debugthread.h
        debugthread.c
        debugging.h
        include/sleeping.h
)

add_subdirectory(src/Core)
add_subdirectory(src/Debugger)
add_subdirectory(src/Loader)

# for logging / stuff I want to use in all files
include_directories(include src/Core/include)

target_link_libraries(GC_ PRIVATE Core)
target_link_libraries(GC_ PRIVATE Debugger)