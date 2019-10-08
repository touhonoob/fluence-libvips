cmake_minimum_required (VERSION 3.15.3)
project(fluence-libvips)

set(WASI_SDK_PREFIX /opt/wasi-sdk)
set(CMAKE_C_COMPILER ${WASI_SDK_PREFIX}/bin/clang)
set(CMAKE_CXX_COMPILER ${WASI_SDK_PREFIX}/bin/clang++)
set(CMAKE_SYSROOT "${WASI_SDK_PREFIX}/share/wasi-sysroot")
set(CMAKE_SYSTEM_NAME Wasm)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR wasm32)
set(triple wasm32-wasi)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -nostartfiles -fvisibility=hidden")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-entry,--demangle,--allow-undefined -Wl,--export=allocate,--export=deallocate,--export=invoke")
set(CMAKE_CXX_FLAGS_DEBUG  "-g")

set(SRC_FILES sdk/allocator.c sdk/logger.c sdk/syscalls_stubs.c cJSON/cJSON.c)
add_executable(fluence_libvips.wasm src/main.c ${SRC_FILES})
