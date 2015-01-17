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
#     list(APPEND CMAKE_MODULE_PATH "path/to/pile-integration.cmake")
#
# then include the module:
#
#     include("pile-integration.cmake")
#
#
#
#
# ============================================================================

# ----------------------------------------------------------------------------
# The list of elements in this directory

set( PILE_INTEGRATION_SOURCES
    integration.cc
    inmanager.cc
    inprocess.cc)
	
set( PILE_INTEGRATION_HEADERS
    integration.h
    inmanager.h
    inprocess.h)

set( PILE_INTEGRATION_QT_MODS
    Core)

# ============================================================================


configure_file (
    "integration_config.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/integration_config.h"
    @ONLY)

	
# ============================================================================
