cmake_minimum_required (VERSION 3.15.3)
project(fluence-libvips-test)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG  "-g")

set(SRC_FILES ../cJSON/cJSON.c)
add_executable(fluence_libvips_test test.c ${SRC_FILES})
