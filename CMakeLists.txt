cmake_minimum_required(VERSION 3.16)

project(base64-benchmark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ARCH_AMD64 0)
set(ARCH_AARCH64 0)

if (CMAKE_SYSTEM_PROCESSOR MATCHES "amd64|x86_64")
    if (CMAKE_LIBRARY_ARCHITECTURE MATCHES "i386")
        message (FATAL_ERROR "32bit platforms are not supported")
    endif ()
    set (ARCH_AMD64 1)
elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(aarch64.*|AARCH64.*|arm64.*|ARM64.*)")
    set (ARCH_AARCH64 1)
else ()
    message (FATAL_ERROR "Platform ${CMAKE_SYSTEM_PROCESSOR} is not supported")
endif ()

message(STATUS "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMAKE_LIBRARY_ARCHITECTURE: ${CMAKE_LIBRARY_ARCHITECTURE}")
message(STATUS "ARCH_AMD64: ${ARCH_AMD64}")
message(STATUS "ARCH_AARCH64: ${ARCH_AARCH64}")

add_subdirectory(aklomp-base64-cmake)
add_subdirectory(Turbo-Base64-cmake)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)
add_subdirectory(benchmark)

add_executable(base64-benchmark main.cpp)
target_link_libraries(base64-benchmark PRIVATE
    aklomp_base64 TurboBase64 benchmark::benchmark)
