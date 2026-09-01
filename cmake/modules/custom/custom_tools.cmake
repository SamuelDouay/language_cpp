#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.25)

include_guard()

# Dependencies
enable_language(C)
enable_language(CXX)
include(CheckCSourceCompiles)
include(CheckCXXSourceCompiles)
include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

function(custom_build_type_set_base_flags build_type)
    cmake_parse_arguments(
      arg_custom_build_type_set_base_flags
      ""
      "INIT_FROM"
      "COMPILER_FLAGS;C_FLAGS;CXX_FLAGS;LINKER_FLAGS;EXE_LINKER_FLAGS;SHARED_LINKER_FLAGS"
      ${ARGN}
    )
    if(arg_custom_build_type_set_base_flags_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected arguments: ${arg_custom_build_type_set_base_flags_UNPARSED_ARGUMENTS}")
    endif()
    # if(arg_custom_build_type_set_base_flags_KEYWORDS_MISSING_VALUES)
    #     message(FATAL_ERROR "Keyword without values: ${arg_custom_build_type_set_base_flags_KEYWORDS_MISSING_VALUES}")
    # endif()
    string(TOUPPER ${build_type} build_type)

    if(arg_custom_build_type_set_base_flags_INIT_FROM)
        set(NEW_CMAKE_C_FLAGS_${build_type} "${CMAKE_C_FLAGS_${arg_custom_build_type_set_base_flags_INIT_FROM}}")
        set(NEW_CMAKE_CXX_FLAGS_${build_type} "${CMAKE_CXX_FLAGS_${arg_custom_build_type_set_base_flags_INIT_FROM}}")
        set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type} "${CMAKE_EXE_LINKER_FLAGS_${arg_custom_build_type_set_base_flags_INIT_FROM}}")
        set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type} "${CMAKE_SHARED_LINKER_FLAGS_${arg_custom_build_type_set_base_flags_INIT_FROM}}")
    else()
        set(NEW_CMAKE_C_FLAGS_${build_type})
        set(NEW_CMAKE_CXX_FLAGS_${build_type})
        set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type})
        set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type})
    endif()

    foreach(flag ${arg_custom_build_type_set_base_flags_COMPILER_FLAGS} ${arg_custom_build_type_set_base_flags_C_FLAGS})
        set(NEW_CMAKE_C_FLAGS_${build_type} "${NEW_CMAKE_C_FLAGS_${build_type}} ${flag}")
    endforeach()
    foreach(flag ${arg_custom_build_type_set_base_flags_COMPILER_FLAGS} ${arg_custom_build_type_set_base_flags_CXX_FLAGS})
        set(NEW_CMAKE_CXX_FLAGS_${build_type} "${NEW_CMAKE_CXX_FLAGS_${build_type}} ${flag}")
    endforeach()
    foreach(flag ${arg_custom_build_type_set_base_flags_LINKER_FLAGS} ${arg_custom_build_type_set_base_flags_EXE_LINKER_FLAGS})
        set(NEW_CMAKE_EXE_LINKER_FLAGS_${build_type} "${NEW_CMAKE_EXE_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()
    foreach(flag ${arg_custom_build_type_set_base_flags_LINKER_FLAGS} ${arg_custom_build_type_set_base_flags_SHARED_LINKER_FLAGS})
        set(NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type} "${NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()

    set(CMAKE_C_FLAGS_${build_type}
      "${NEW_CMAKE_C_FLAGS_${build_type}}"
      CACHE STRING "Flags used by the C compiler for ${build_type} build type"
      FORCE
    )
    set(CMAKE_CXX_FLAGS_${build_type}
      "${NEW_CMAKE_CXX_FLAGS_${build_type}}"
      CACHE STRING "Flags used by the CXX compiler for ${build_type} build type"
      FORCE
    )
    set(CMAKE_EXE_LINKER_FLAGS_${build_type}
      "${NEW_CMAKE_EXE_LINKER_FLAGS_${build_type}}"
      CACHE STRING "Linker flags to be used to create executables for ${build_type} build type."
      FORCE
    )
    set(CMAKE_SHARED_LINKER_FLAGS_${build_type}
      "${NEW_CMAKE_SHARED_LINKER_FLAGS_${build_type}}"
      CACHE STRING "Linker lags to be used to create shared libraries for ${build_type} build type."
      FORCE
    )
    mark_as_advanced(
      CMAKE_C_FLAGS_${build_type}
      CMAKE_CXX_FLAGS_${build_type}
      CMAKE_EXE_LINKER_FLAGS_${build_type}
      CMAKE_SHARED_LINKER_FLAGS_${build_type}
    )

    # foreach(flag_type C_FLAGS CXX_FLAGS EXE_LINKER_FLAGS SHARED_LINKER_FLAGS)
    #     string(REPLACE ";" " " flags_print "${CMAKE_${flag_type}_${build_type}}")
    #     if(flags_print)
    #         message(STATUS "${build_type} ${flag_type}: set to \"${flags_print}\"")
    #     endif()
    # endforeach()
endfunction()

function(custom_build_type_add_base_flags build_type)
    cmake_parse_arguments(arg_custom_build_type_add_base_flags "" "" "COMPILER_FLAGS;C_FLAGS;CXX_FLAGS;LINKER_FLAGS;EXE_LINKER_FLAGS;SHARED_LINKER_FLAGS" ${ARGN})
    if(arg_custom_build_type_add_base_flags_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unexpected arguments: ${arg_custom_build_type_add_base_flags_UNPARSED_ARGUMENTS}")
    endif()
    # if(arg_custom_build_type_add_base_flags_KEYWORDS_MISSING_VALUES)
    #     message(FATAL_ERROR "Keyword without values: ${arg_custom_build_type_add_base_flags_KEYWORDS_MISSING_VALUES}")
    # endif()
    string(TOUPPER ${build_type} build_type)

    set(ADDED_CMAKE_C_FLAGS_${build_type})
    set(ADDED_CMAKE_CXX_FLAGS_${build_type})
    set(ADDED_CMAKE_EXE_LINKER_FLAGS_${build_type})
    set(ADDED_CMAKE_SHARED_LINKER_FLAGS_${build_type})

    foreach(flag ${arg_custom_build_type_add_base_flags_COMPILER_FLAGS} ${arg_custom_build_type_add_base_flags_C_FLAGS})
        set(ADDED_CMAKE_C_FLAGS_${build_type} "${ADDED_CMAKE_C_FLAGS_${build_type}} ${flag}")
    endforeach()
    foreach(flag ${arg_custom_build_type_add_base_flags_COMPILER_FLAGS} ${arg_custom_build_type_add_base_flags_CXX_FLAGS})
        set(ADDED_CMAKE_CXX_FLAGS_${build_type} "${ADDED_CMAKE_CXX_FLAGS_${build_type}} ${flag}")
    endforeach()
    foreach(flag ${arg_custom_build_type_add_base_flags_LINKER_FLAGS} ${arg_custom_build_type_add_base_flags_EXE_LINKER_FLAGS})
        set(ADDED_CMAKE_EXE_LINKER_FLAGS_${build_type} "${ADDED_CMAKE_EXE_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()
    foreach(flag ${arg_custom_build_type_add_base_flags_LINKER_FLAGS} ${arg_custom_build_type_add_base_flags_SHARED_LINKER_FLAGS})
        set(ADDED_CMAKE_SHARED_LINKER_FLAGS_${build_type} "${ADDED_CMAKE_SHARED_LINKER_FLAGS_${build_type}} ${flag}")
    endforeach()

    set(CMAKE_C_FLAGS_${build_type}
      "${CMAKE_C_FLAGS_${build_type}} ${ADDED_CMAKE_C_FLAGS_${build_type}}"
      CACHE STRING "Flags used by the C compiler for ${build_type} build type"
      FORCE
    )
    set(CMAKE_CXX_FLAGS_${build_type}
      "${CMAKE_CXX_FLAGS_${build_type}} ${ADDED_CMAKE_CXX_FLAGS_${build_type}}"
      CACHE STRING "Flags used by the CXX compiler for ${build_type} build type"
      FORCE
    )
    set(CMAKE_EXE_LINKER_FLAGS_${build_type}
      "${CMAKE_EXE_LINKER_FLAGS_${build_type}} ${ADDED_CMAKE_EXE_LINKER_FLAGS_${build_type}}"
      CACHE STRING "Linker flags to be used to create executables for ${build_type} build type."
      FORCE
    )
    set(CMAKE_SHARED_LINKER_FLAGS_${build_type}
      "${CMAKE_SHARED_LINKER_FLAGS_${build_type}} ${ADDED_CMAKE_SHARED_LINKER_FLAGS_${build_type}}"
      CACHE STRING "Linker lags to be used to create shared libraries for ${build_type} build type."
      FORCE
    )
    mark_as_advanced(
      CMAKE_C_FLAGS_${build_type}
      CMAKE_CXX_FLAGS_${build_type}
      CMAKE_EXE_LINKER_FLAGS_${build_type}
      CMAKE_SHARED_LINKER_FLAGS_${build_type}
    )

    # foreach(flag_type C_FLAGS CXX_FLAGS EXE_LINKER_FLAGS SHARED_LINKER_FLAGS)
    #     string(REPLACE ";" " " flags_print "${ADDED_CMAKE_${flag_type}_${build_type}}")
    #     if(flags_print)
    #         message(STATUS "${build_type} ${flag_type}: added \"${flags_print}\"")
    #     endif()
    # endforeach()
endfunction()

function(custom_check_cxx_compiler_flag flag var)
    # get check variable name from flag
    set(check ${flag})
    string(REPLACE "-" "_" check ${check})
    string(REPLACE "=" "_" check ${check})
    string(REPLACE " " "_" check ${check})
    string(REPLACE "," "_" check ${check})
    string(REPLACE "/" "_" check ${check})
    string(REPLACE ":" "_" check ${check})
    string(REPLACE ";" "_" check ${check})
    string(REPLACE "!" "_" check ${check})

    # clang only rejects unknown warning flags if -Werror=unknown-warning-option is also specified.
    check_cxx_compiler_flag(-Werror=unknown-warning-option _Werror_unknown_warning_option)
    if(_Werror_unknown_warning_option)
        set(CMAKE_REQUIRED_FLAGS "-Werror=unknown-warning-option")
    endif()

    # check
    check_cxx_compiler_flag(${flag} ${check})

    # set result
    set(${var} ${${check}} PARENT_SCOPE)
endfunction()

function(custom_check_compiler_linker_flag flag var)
    # get check variable name from flag
    set(check ${flag})
    string(REPLACE "-" "_" check ${check})
    string(REPLACE "=" "_" check ${check})
    string(REPLACE " " "_" check ${check})
    string(REPLACE "," "_" check ${check})
    string(REPLACE "/" "_" check ${check})
    string(REPLACE ":" "_" check ${check})
    string(REPLACE ";" "_" check ${check})
    string(REPLACE "!" "_" check ${check})

    # set flag in REQUIRED vars for linker check
    # clang only rejects unknown warning flags if -Werror=unknown-warning-option is also specified.
    check_cxx_compiler_flag(-Werror=unknown-warning-option _Werror_unknown_warning_option)
    if(_Werror_unknown_warning_option)
        set(CMAKE_REQUIRED_FLAGS "-Werror=unknown-warning-option ${flag}")
        set(CMAKE_REQUIRED_LINK_OPTIONS "-Werror=unknown-warning-option ${flag}")
    else()
        set(CMAKE_REQUIRED_FLAGS "${flag}")
        set(CMAKE_REQUIRED_LINK_OPTIONS "${flag}")
    endif()

    # check
    check_cxx_compiler_flag(${flag} ${check})

    # set result
    set(${var} ${${check}} PARENT_SCOPE)
endfunction()
