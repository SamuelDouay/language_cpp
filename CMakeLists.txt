cmake_minimum_required(VERSION 3.25)

# Project declaration
project(
  language_cpp
  VERSION 1.0.0
  DESCRIPTION "language_cpp"
  HOMEPAGE_URL "https://github.com/SamuelDouay/language_cpp"
  LANGUAGES CXX
)

# Set CMake modules path
list(APPEND
  CMAKE_MODULE_PATH
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules"
)

# Include modules
include(flags)
include(format)
include(tidy)

# setup custom build types
set(build_types)
get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
if(is_multi_config)
    if(CMAKE_CONFIGURATION_TYPES)
        set(build_types ${CMAKE_CONFIGURATION_TYPES})
        list(TRANSFORM build_types TOUPPER)
    endif()
else()
    if(CMAKE_BUILD_TYPE)
        string(TOUPPER ${CMAKE_BUILD_TYPE} build_types)
    endif()
endif()
if("COVERAGE" IN_LIST build_types)
    setup_coverage_flags()
endif()
if("ASAN" IN_LIST build_types)
    setup_asan_flags()
endif()
if("UBSAN" IN_LIST build_types)
    setup_ubsan_flags()
endif()
if("TSAN" IN_LIST build_types)
    setup_tsan_flags()
endif()
if("SANITIZE" IN_LIST build_types)
    setup_sanitize_flags()
endif()

# Disable in source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# Use strict C standard
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Use strict C++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Statically link runtime library with MSVC
# https://cmake.org/cmake/help/latest/variable/CMAKE_MSVC_RUNTIME_LIBRARY.html
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<OR:$<CONFIG:Debug>,$<CONFIG:Asan>,$<CONFIG:UBSan>,$<CONFIG:Tsan>,$<CONFIG:Sanitize>>:Debug>")

# Statically link libgcc and libstdc++ with MINGW gcc
# https://gcc.gnu.org/onlinedocs/gcc/Link-Options.html
if(MINGW AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    custom_check_cxx_compiler_flag("-static-libgcc" res1)
    custom_check_cxx_compiler_flag("-static-libstdc++" res2)
    if(res1 AND res2)
        set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
    else()
        message(WARNING "-static-libgcc/-static-libstdc++ reported as unsupported by compiler")
    endif()
endif()

# Export compile commands for external tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# IDE folders config
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "_CMake")

# Catch2
get_filename_component(CATCH2_CMAKELISTS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/externals/Catch2/CMakeLists.txt" ABSOLUTE)
if(NOT EXISTS "${CATCH2_CMAKELISTS_PATH}")
    message(FATAL_ERROR "Catch2 dependency is missing, maybe you didn't pull the git submodules")
endif()
add_subdirectory(externals/Catch2 SYSTEM)
include(externals/Catch2/extras/Catch.cmake)

# Binaries
add_subdirectory(excercice)
add_subdirectory(sudoku)
add_subdirectory(minesweeper)
add_subdirectory(dynamic)

# Tests
enable_testing()
add_subdirectory(tests)
