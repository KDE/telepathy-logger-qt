# - Common macros for TelepathyLoggerQt

# Copyright (c) 2014, Daniel Vrátil <dvratil@redhat.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.



# Based on ecm_generate_headers from extra-cmake-modules
# Unlike the ECM version, this one can find "lower-case.h" header files for given
# CamelCase headers
function(tplqt_generate_headers camelcase_headers_var)
    set(options)
    set(oneValueArgs REQUIRED_HEADERS)
    set(multiValueArgs HEADER_NAMES)
    cmake_parse_arguments(GH "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    foreach (_CLASSNAME ${GH_HEADER_NAMES})
        set(FANCY_HEADER_FILE "${CMAKE_CURRENT_BINARY_DIR}/${_CLASSNAME}")
        if (${_CLASSNAME} MATCHES "([a-z]+)([A-Z][a-z]+)")
            string(REGEX REPLACE "([a-z]+)([A-Z][a-z]+)" "\\1-\\2" dashcamel ${_CLASSNAME})
            string(TOLOWER ${dashcamel} lowercase)
        else()
            string(TOLOWER ${_CLASSNAME} lowercase)
        endif()
        set(_actualheader "${CMAKE_CURRENT_SOURCE_DIR}/${lowercase}.h")
        if (NOT EXISTS ${_actualheader})
            message(FATAL_ERROR "Could not find \"${_actualheader}\"")
        endif()
        if (NOT EXISTS ${FANCY_HEADER_FILE})
            file(WRITE ${FANCY_HEADER_FILE} "#include \"${lowercase}.h\"\n")
        endif()
        list(APPEND ${camelcase_headers_var} "${FANCY_HEADER_FILE}")
        list(APPEND ${GH_REQUIRED_HEADERS} "${_actualheader}")
    endforeach()
    set(${camelcase_headers_var} ${${camelcase_headers_var}} PARENT_SCOPE)
    if (NOT GH_REQUIRED_HEADERS STREQUAL "")
        set(${GH_REQUIRED_HEADERS} ${${GH_REQUIRED_HEADERS}} PARENT_SCOPE)
    endif ()
endfunction()