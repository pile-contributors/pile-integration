# ============================================================================
#
# Integration helpers
# ===================
#
# This file contains support CMake script for integration tests.
#
# Usage
# -----
#
# Include the directory where this file resides by using:
#
#     list(APPEND CMAKE_MODULE_PATH "path/to/pile-integration")
#
# then include the module:
#
#     include("pile-integration")
#
#
#
#
# ============================================================================

# ----------------------------------------------------------------------------
# The list of elements in this directory

set( PILE_INTEGRATION_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/inapplication.cc
    ${CMAKE_CURRENT_LIST_DIR}/integration.cc
    ${CMAKE_CURRENT_LIST_DIR}/inmanager.cc
    ${CMAKE_CURRENT_LIST_DIR}/inprocess.cc)
	
set( PILE_INTEGRATION_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/inapplication.h
    ${CMAKE_CURRENT_LIST_DIR}/integration.h
    ${CMAKE_CURRENT_LIST_DIR}/inmanager.h
    ${CMAKE_CURRENT_LIST_DIR}/inprocess.h)

set( PILE_INTEGRATION_QT_MODS
    Core)

include_directories("${CMAKE_CURRENT_LIST_DIR}")

if (WIN32)
    set (PILE_INTEGRATION_WIN32 ON)
else ()
    set (PILE_INTEGRATION_WIN32 OFF)
endif()

# ============================================================================


if(NOT PILE_INTEGRATION_ORGANIZATION)
    set (PILE_INTEGRATION_ORGANIZATION "pileintegration")
endif()

if(NOT PILE_INTEGRATION_DOMAIN)
    set (PILE_INTEGRATION_DOMAIN "pileintegration.org")
endif()

if(NOT PILE_INTEGRATION_APPNAME)
    set (PILE_INTEGRATION_APPNAME "pileintegration")
endif()

set (EXES_DEFINED_INSIDE_COUNT 0)
set (PILE_INTEGRATION_TEST_PATH "${CMAKE_CURRENT_LIST_DIR}")

configure_file (
    "${PILE_INTEGRATION_TEST_PATH}/integration_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/integration_config.h"
    @ONLY)

	
# ============================================================================

# Creates an executable target for tests
#
# Arguments
#     - name: the name of the target to generate
#     - sources: any number of sources can follow standard arguments

macro(pileIntegrationTest pit_name)

    string (TOUPPER "${pit_name}" pit__name_u)
    string (TOLOWER "${pit_name}" pit__name_l)

    pileTarget( "${pit_name}")

        list(APPEND ${pit__name_u}_SOURCES
            ${ARGN}
            ${PILE_INTEGRATION_SOURCES}
        )
        list(APPEND ${pit__name_u}_HEADERS
            ${PILE_INTEGRATION_HEADERS}
        )

        # list(APPEND ${pit__name_u}_LIBS )

        list(APPEND ${pit__name_u}_QT_MODS
            ${PILE_INTEGRATION_QT_MODS}
        )

    pileEndTarget( "${pit_name}" "exe")

endmacro()

# ============================================================================

# Adds one or more executables to the list defined inside configuration file
#
# Arguments
#     - a list of executables

macro(pileIntegrationAddExes )

    message (STATUS "IIIII  ----------------------------------}")
    foreach(exe_instance ${ARGN})
        if (EXES_DEFINED_INSIDE_COUNT GREATER 20)
            message(FATAL_ERROR "pile-integration supports only 20 executables in config file.")
        endif()

        # we use a number to identify the executable inside
        set (exe_crt_suffix "${EXES_DEFINED_INSIDE_COUNT}")
        string (LENGTH "${exe_crt_suffix}" exe_crt_suffix_len)
        if (exe_crt_suffix_len LESS 2)
            set (exe_crt_suffix "0${exe_crt_suffix}")
        endif()
        message (STATUS "IIIII  exe_instance = ${exe_instance}")
        message (STATUS "IIIII  exe_crt_suffix = ${exe_crt_suffix}")

        # for the identifier we will need a string without
        # funky characters
        string(REGEX REPLACE
            "[ \\/\\\\_\\.\\!\\@\\#\\$\\%\\^\\&\\*\\9\\)-\\+\\=\\|\\<\\>]+"
            "_"
            EXE_${exe_crt_suffix}
            "${exe_instance}")

        string(TOUPPER "${EXE_${exe_crt_suffix}}" EXE_${exe_crt_suffix})

        set(EXE_NAME_${exe_crt_suffix} "${exe_instance}")

        # increment the counter
        math(EXPR EXES_DEFINED_INSIDE_COUNT "${EXES_DEFINED_INSIDE_COUNT}+1")

    endforeach()

    # we need to recreate the config file
    configure_file (
        "${PILE_INTEGRATION_TEST_PATH}/integration_config.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/integration_config.h"
        @ONLY)
endmacro()

# ============================================================================

