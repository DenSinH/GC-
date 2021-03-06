cmake_minimum_required(VERSION 3.16)
project(GC_ LANGUAGES C CXX)

# set(CMAKE_VERBOSE_MAKEFILE ON)

# check of LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(CMAKE_C_FLAGS_RELEASE "-Ofast")

# add_compile_options($<$<CONFIG:DEBUG>:-Wall>)
add_compile_definitions(
        $<$<CONFIG:RELEASE>:NDEBUG>
        $<$<CONFIG:RELEASE>:BOOST_DISABLE_ASSERTS>
)

add_executable(GC_
        main.c
        include/default.h
        include/helpers.h
        include/flags.h
        include/log.h
        include/sleeping.h
        modules/glad/glad.h
        modules/glad/glad.c
        modules/KHR/khrplatform.h
        UIthread.h
        UIthread.c
        debugging.h
        init.c
        init.h
        include/custom_threading.h
        custom_threading.c
        include/const.h
        include/byteswap.h
)

# add link time optimizations for extra speed
if( ipo_supported )
    message(STATUS "IPO / LTO enabled when compiled in release mode")
    if (CMAKE_BUILD_TYPE EQUAL "RELEASE")
        set_property(TARGET GC_ PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif (CMAKE_BUILD_TYPE EQUAL "RELEASE")
else()
    message(STATUS "IPO / LTO not supported: <${ipo_error}>")
endif()

add_subdirectory(modules/imgui)

add_subdirectory(src/Core)
add_subdirectory(src/Frontend)


include_directories(include
        $<TARGET_PROPERTY:Core,INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:Frontend,INCLUDE_DIRECTORIES>
)

target_link_libraries(GC_ PRIVATE Core)
target_link_libraries(GC_ PRIVATE Frontend)
target_link_libraries(GC_ PRIVATE imgui)

# copy over files with settings
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/imgui.ini
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/input.map
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})