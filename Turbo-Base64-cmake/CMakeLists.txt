set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/Turbo-Base64)
message(STATUS "Turbo-Base64 directory: ${LIBRARY_DIR}")

# Based on https://github.com/ClickHouse/ClickHouse/blob/master/contrib/base64-cmake/CMakeLists.txt

add_library(_base64_scalar OBJECT "${LIBRARY_DIR}/turbob64c.c" "${LIBRARY_DIR}/turbob64d.c")
add_library(_base64_ssse3 OBJECT "${LIBRARY_DIR}/turbob64sse.c") # This file also contains code for ARM NEON

if (ARCH_AMD64)
    add_library(_base64_avx OBJECT "${LIBRARY_DIR}/turbob64sse.c") # This is not a mistake. One file is compiled twice.
    add_library(_base64_avx2 OBJECT "${LIBRARY_DIR}/turbob64avx2.c")
endif ()

if (ARCH_AMD64)
    target_compile_options(_base64_ssse3 PRIVATE -mno-avx -mno-avx2 -mssse3 -falign-loops)
    target_compile_options(_base64_avx PRIVATE -falign-loops -mavx)
    target_compile_options(_base64_avx2 PRIVATE -falign-loops -mavx2)
else ()
    target_compile_options(_base64_ssse3 PRIVATE -falign-loops)
endif ()

if (ARCH_AMD64)
    add_library(TurboBase64
        $<TARGET_OBJECTS:_base64_scalar>
        $<TARGET_OBJECTS:_base64_ssse3>
        $<TARGET_OBJECTS:_base64_avx>
        $<TARGET_OBJECTS:_base64_avx2>)
else ()
    add_library(TurboBase64
        $<TARGET_OBJECTS:_base64_scalar>
        $<TARGET_OBJECTS:_base64_ssse3>)
endif ()

target_include_directories(TurboBase64 SYSTEM PUBLIC ${LIBRARY_DIR})
