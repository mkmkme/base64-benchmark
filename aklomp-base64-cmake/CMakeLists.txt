set(LIBRARY_DIR ${CMAKE_SOURCE_DIR}/aklomp-base64)
message(STATUS "aklomp/base64 directory: ${LIBRARY_DIR}")

include(CMakeDependentOption)

add_library(aklomp_base64
    "${LIBRARY_DIR}/lib/lib.c"
    "${LIBRARY_DIR}/lib/codec_choose.c"
    "${LIBRARY_DIR}/include/libbase64.h"

    "${LIBRARY_DIR}/lib/tables/tables.c"
    "${LIBRARY_DIR}/lib/tables/table_dec_32bit.h"
    "${LIBRARY_DIR}/lib/tables/table_enc_12bit.h"

    "${LIBRARY_DIR}/lib/arch/generic/codec.c"

    "${LIBRARY_DIR}/lib/arch/ssse3/codec.c"
    "${LIBRARY_DIR}/lib/arch/sse41/codec.c"
    "${LIBRARY_DIR}/lib/arch/sse42/codec.c"
    "${LIBRARY_DIR}/lib/arch/avx/codec.c"
    "${LIBRARY_DIR}/lib/arch/avx2/codec.c"

    "${LIBRARY_DIR}/lib/arch/neon32/codec.c"
    "${LIBRARY_DIR}/lib/arch/neon64/codec.c"
)

target_include_directories(aklomp_base64
    PUBLIC
        "${LIBRARY_DIR}/include"
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"
)

target_compile_definitions(aklomp_base64 PUBLIC BASE64_STATIC_DEFINE)

set(COMPILE_FLAGS_SSSE3 "-mssse3")
set(COMPILE_FLAGS_SSE41 "-msse4.1")
set(COMPILE_FLAGS_SSE42 "-msse4.2")
set(COMPILE_FLAGS_AVX "-mavx")
set(COMPILE_FLAGS_AVX2 "-mavx2")
set(COMPILE_FLAGS_NEON32 "-mfpu=neon")

cmake_dependent_option(BASE64_WITH_SSSE3 "add SSSE 3 codepath" ON "${ARCH_AMD64}" OFF)
cmake_dependent_option(BASE64_WITH_SSE41 "add SSE 4.1 codepath" ON "${ARCH_AMD64}" OFF)
cmake_dependent_option(BASE64_WITH_SSE42 "add SSE 4.2 codepath" ON "${ARCH_AMD64}" OFF)
cmake_dependent_option(BASE64_WITH_AVX "add AVX codepath" ON "${ARCH_AMD64}" OFF)
cmake_dependent_option(BASE64_WITH_AVX2 "add AVX 2 codepath" ON "${ARCH_AMD64}" OFF)

cmake_dependent_option(BASE64_WITH_NEON32 "add NEON32 codepath" OFF "${ARCH_AARCH64}" OFF)

cmake_dependent_option(BASE64_WITH_NEON64 "add NEON64 codepath" ON "${ARCH_AARCH64}" OFF)

if (ARCH_AMD64)
    macro(configure_codec _TYPE)
        if (BASE64_WITH_${_TYPE})
            string(TOLOWER "${_TYPE}" _DIR)
            message(STATUS "Will add ${_TYPE} codepath")
            set_source_files_properties("${LIBRARY_DIR}/lib/arch/${_DIR}/codec.c" PROPERTIES
                COMPILE_FLAGS "${COMPILE_FLAGS_${_TYPE}}"
            )

            if (${ARGC} GREATER 1 AND MSVC)
                set_source_files_properties("${LIBRARY_DIR}/lib/arch/${_DIR}/codec.c" PROPERTIES
                    COMPILE_DEFINITIONS ${ARGV1}
                )
            endif()
        endif()
    endmacro()

    configure_codec(SSSE3 __SSSE3__)
    configure_codec(SSE41 __SSSE4_1__)
    configure_codec(SSE42 __SSSE4_2__)
    configure_codec(AVX)
    configure_codec(AVX2)

elseif (_TARGET_ARCH STREQUAL "arm")
    set(BASE64_NEON32_CFLAGS "${COMPILE_FLAGS_NEON32}" CACHE STRING "the NEON32 compile flags (for 'lib/arch/neon32/codec.c')")
    mark_as_advanced(BASE64_NEON32_CFLAGS)

    if (BASE64_WITH_NEON32)
        set_source_files_properties("lib/arch/neon32/codec.c" PROPERTIES
            COMPILE_FLAGS "${BASE64_NEON32_CFLAGS} "
        )
    endif()

endif()

configure_file("${CMAKE_CURRENT_LIST_DIR}/config.h.in" "${CMAKE_CURRENT_BINARY_DIR}/config.h" @ONLY)
