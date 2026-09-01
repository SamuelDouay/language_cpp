#
# Copyright (c) 2024 Maxime Pinard
#
# Distributed under the MIT license
# See accompanying file LICENSE or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.18.4...3.25.1)
include_guard()

function(generate_format_target target)
    # Checks
    if(NOT TARGET ${target})
        message(FATAL_ERROR "Invalid argument: ${target} is not a target")
    endif()
    set(format-target "format-${target}")
    if(TARGET ${format-target})
        message(FATAL_ERROR "${format-target} already exists")
    endif()

    # Find clang-format
    find_program(CLANG_FORMAT clang-format)
    if(${CLANG_FORMAT} STREQUAL CLANG_FORMAT-NOTFOUND)
        message(WARNING "clang-format not found, ${format-target} target not generated")
        return()
    endif()

    # Get sources
    get_target_property(target_sources ${target} CUSTOM_SOURCES)
    if(NOT target_sources)
        get_target_property(target_type ${target} TYPE)
        if(target_type STREQUAL "INTERFACE_LIBRARY")
            message(WARNING "${target} has no FORMAT_SOURCES set, ${format-target} target not generated")
            return()
        endif()
        get_property(target_sources TARGET ${target} PROPERTY SOURCES)
    endif()

    # Generate target
    add_custom_target(
      ${format-target}
      COMMAND "${CLANG_FORMAT}" -style=file -i ${target_sources}
      WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
      VERBATIM
    )
    set_target_properties(${format-target} PROPERTIES FOLDER "format")
    message(STATUS "Generated format target ${format-target}")

    # Add dependency to format-all
    if(NOT TARGET format-all)
        add_custom_target(format-all)
        set_target_properties(format-all PROPERTIES FOLDER "format")
    endif()
    add_dependencies(format-all ${format-target})
endfunction()
