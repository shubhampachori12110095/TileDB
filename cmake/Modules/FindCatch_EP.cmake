#
# FindCatch_EP.cmake
#
#
# The MIT License
#
# Copyright (c) 2018 TileDB, Inc.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# Finds the Catch library, installing with an ExternalProject as necessary.
# This module defines:
#   - CATCH_INCLUDE_DIR, directory containing headers
#   - CATCH_FOUND, whether Catch has been found
#   - The Catch::Catch imported target

# Search the path set during the superbuild for the EP.
set(CATCH_PATHS ${CATCH_INCLUDE_DIR})

find_path(CATCH_INCLUDE_DIR
  NAMES catch.hpp
  PATHS ${CATCH_PATHS}
  ${TILEDB_DEPS_NO_DEFAULT_PATH}
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Catch
  REQUIRED_VARS CATCH_INCLUDE_DIR
)

if (NOT CATCH_FOUND AND TILEDB_SUPERBUILD)
  message(STATUS "Adding Catch as an external project")
  ExternalProject_Add(ep_catch
    PREFIX "externals"
    URL "https://github.com/catchorg/Catch2/archive/v2.2.1.zip"
    URL_HASH SHA1=578908c96d67e681a13ea903188a107076a6d1ee
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
    LOG_DOWNLOAD TRUE
  )
  ExternalProject_Get_Property(ep_catch source_dir)
  set(CATCH_INCLUDE_DIR ${source_dir}/single_include)
  list(APPEND TILEDB_EXTERNAL_PROJECTS ep_catch)
  list(APPEND FORWARD_EP_CMAKE_ARGS
    -DCATCH_INCLUDE_DIR=${CATCH_INCLUDE_DIR}
  )
endif()

if (CATCH_FOUND AND NOT TARGET Catch::Catch)
  add_library(Catch::Catch INTERFACE IMPORTED)
  set_target_properties(Catch::Catch PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${CATCH_INCLUDE_DIR}"
  )
endif()