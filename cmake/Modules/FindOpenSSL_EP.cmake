#
# FindOpenSSL_EP.cmake
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
# Finds the OpenSSL library, installing with an ExternalProject as necessary.
# This module defines:
#   - OPENSSL_INCLUDE_DIR, directory containing headers
#   - OPENSSL_LIBRARIES, the OpenSSL library path
#   - OPENSSL_FOUND, whether OpenSSL has been found
#   - The OpenSSL::SSL and OpenSSL::Crypto imported targets

# Search the path set during the superbuild for the EP.
# Add /usr/local/opt, as Homebrew sometimes installs it there.
set(OPENSSL_PATHS ${OPENSSL_DIR} /usr/local/opt/openssl)

# First try the CMake-provided find script.
set(OPENSSL_ROOT_DIR ${OPENSSL_PATHS})
find_package(OpenSSL QUIET)

# Next try finding the superbuild external project
if (NOT OPENSSL_FOUND)
  find_path(OPENSSL_INCLUDE_DIR
    NAMES openssl/ssl.h
    PATHS ${OPENSSL_PATHS}
    PATH_SUFFIXES include
  )

  # Link statically if installed with the EP.
  find_library(OPENSSL_SSL_LIBRARY
    NAMES
      libssl${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS ${OPENSSL_PATHS}
    PATH_SUFFIXES lib
  )

  find_library(OPENSSL_CRYPTO_LIBRARY
    NAMES
      libcrypto${CMAKE_STATIC_LIBRARY_SUFFIX}
    PATHS ${OPENSSL_PATHS}
    PATH_SUFFIXES lib
  )

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenSSL
    REQUIRED_VARS OPENSSL_SSL_LIBRARY OPENSSL_CRYPTO_LIBRARY OPENSSL_INCLUDE_DIR
  )
endif()

if (NOT OPENSSL_FOUND AND TILEDB_SUPERBUILD)
  message(STATUS "Could NOT find OpenSSL")
  message(STATUS "Adding OpenSSL as an external project")

  if (WIN32)
    message(FATAL_ERROR "OpenSSL external project unimplemented on Windows.")
  endif()

  ExternalProject_Add(ep_openssl
    PREFIX "externals"
    URL "https://github.com/openssl/openssl/archive/OpenSSL_1_1_0h.zip"
    URL_HASH SHA1=7558a4444480047759041df1424071041b91d065
    CONFIGURE_COMMAND
      ${TILEDB_EP_BASE}/src/ep_openssl/config
        --prefix=${TILEDB_EP_INSTALL_PREFIX}
        no-shared
        -fPIC
    BUILD_IN_SOURCE TRUE
    BUILD_COMMAND $(MAKE)
    INSTALL_COMMAND $(MAKE) install
    UPDATE_COMMAND ""
    LOG_DOWNLOAD TRUE
    LOG_CONFIGURE TRUE
    LOG_BUILD TRUE
    LOG_INSTALL TRUE
  )

  set(OPENSSL_DIR ${TILEDB_EP_INSTALL_PREFIX})
  list(APPEND TILEDB_EXTERNAL_PROJECTS ep_openssl)
  list(APPEND FORWARD_EP_CMAKE_ARGS
    -DOPENSSL_DIR=${OPENSSL_DIR}
  )
endif()

if (OPENSSL_FOUND)
  if (NOT TARGET OpenSSL::SSL)
    add_library(OpenSSL::SSL UNKNOWN IMPORTED)
    set_target_properties(OpenSSL::SSL PROPERTIES
      IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
    )
  endif()

  if (NOT TARGET OpenSSL::Crypto)
    add_library(OpenSSL::Crypto UNKNOWN IMPORTED)
    set_target_properties(OpenSSL::Crypto PROPERTIES
      IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
      )
  endif()
endif()
