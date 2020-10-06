cmake_minimum_required(VERSION "3.17.20032601-MSVC_2")
project("lto-test" LANGUAGES C)

cmake_policy(SET CMP0069 NEW)

add_library(foo foo.c)
add_executable(boo main.c)
target_link_libraries(boo PUBLIC foo)
