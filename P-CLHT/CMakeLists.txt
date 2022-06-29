cmake_minimum_required(VERSION 2.8)
project(P-CLHT)

if( NOT CMAKE_BUILD_TYPE )
    message(STATUS "No build type selected, default to Release")
    set( CMAKE_BUILD_TYPE Release)
else()
    message(STATUS "Build type is set to ${CMAKE_BUILD_TYPE}")
endif()

set(LLVM_DIR $ENV{LLVM_HOME})
set(CMAKE_C_COMPILER "${LLVM_DIR}/bin/clang")
set(CMAKE_CXX_COMPILER "${LLVM_DIR}/bin/clang++")

set(COMMON_FLAGS "-march=native -mavx -Wno-deprecated-declarations -Wall -Wextra -fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free -faligned-new=64")

execute_process(COMMAND cat /proc/cpuinfo COMMAND grep clflush OUTPUT_VARIABLE ENABLE_CLFLUSH)
execute_process(COMMAND cat /proc/cpuinfo COMMAND grep clflushopt OUTPUT_VARIABLE ENABLE_CLFLUSHOPT)
execute_process(COMMAND cat /proc/cpuinfo COMMAND grep clwb OUTPUT_VARIABLE ENABLE_CLWB)

if(ENABLE_CLWB)
    add_definitions(-DCLWB)
    set(COMMON_FLAGS "${COMMON_FLAGS} -mclwb")
    message(STATUS "Looking for clwb instruction - found")
elseif(ENABLE_CLFLUSHOPT)
    add_definitions(-DCLFLUSH_OPT)
    set(COMMON_FLAGS "${COMMON_FLAGS} -mclflushopt")
    message(STATUS "Looking for clwb instruction - not found")
    message(STATUS "Looking for clflushopt instruction - found")
elseif(ENABLE_CLFLUSH)
    add_definitions(-DCLFLUSH)
    message(STATUS "Looking for clwb instruction - not found")
    message(STATUS "Looking for clflushopt instruction - not found")
    message(STATUS "Looking for clflush instruction - found")
else()
    message(FATAL_ERROR "Cannot find any flush instructions (clflush, clflushopt, clwb)")
endif()

set(HOOK_FLAGS "-fheinous-gnu-extensions -g -O0 -v")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS} ${HOOK_FLAGS} -std=c++17 -I/usr/include/c++/9 -I/usr/include/x86_64-linux-gnu/c++/9")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS} ${HOOK_FLAGS}")

add_compile_options("-fsanitize=thread")
link_libraries("-fsanitize=thread")

add_compile_options("-stdlib=libc++")
link_libraries("-stdlib=libc++")

add_definitions(-DADD_PADDING)

# include_directories(include /usr/include/c++/9)
# include_directories(include /usr/include/x86_64-linux-gnu/c++/9)
include_directories(include external/include)
include_directories(include ${LLVM_DIR}/include/c++/v1)

find_library(JemallocLib jemalloc)
find_library(TbbLib tbb)
find_package (Threads)
link_directories(${LLVM_DIR}/lib)
# link_directories(${PMRACE_DIR}/instrument)
# link_directories(${PMRACE_DIR}/deps/pmdk/install/lib/pmdk_debug)

link_libraries("-Wl,--copy-dt-needed-entries")

set(P_CLHT_TEST example.cpp src/clht_lb_res.c src/clht_gc.c
    external/sspfd/sspfd.c external/ssmem/src/ssmem.c)
add_executable(example ${P_CLHT_TEST})

target_link_libraries(example ${TbbLib} ${JemallocLib} boost_system
    boost_thread pthread)
