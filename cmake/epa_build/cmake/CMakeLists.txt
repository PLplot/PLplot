# cmake/CMakeLists.txt
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build of cmake.

# Copyright (C) 2013 Alan W. Irwin

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this file; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

# The top-level CMakeLists.txt file that includes this file should
# have run "include(ExternalProject)" and set EPA_BASE variable (and
# corresponding directory property) as well as various EPA variables
# used below that configure how the External_Project functions
# operate.

set(PACKAGE cmake)

# Protect against configuring a build twice in one CMake call
if(configured)
  return()
endif(configured)
set(configured ON)

# Data that is related to downloads.
set(EPA_CMAKE_DIR v2.8)
set(EPA_CMAKE_VERSION 2.8.11.1)
set(URL http://www.cmake.org/files/${EPA_CMAKE_DIR}/cmake-${EPA_CMAKE_VERSION}.tar.gz)
set(URL_MD5 df5324a3b203373a9e0a04b924281a43)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  set(EPA_PATH_NODLL "${EPA_PATH}")
  set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
  determine_msys_path(${PACKAGE}_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
else(MSYS_PLATFORM)
  set(${PACKAGE}_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

ExternalProject_Add(
  build_${PACKAGE}
  URL ${URL}
  URL_MD5 ${URL_MD5}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} CFLAGS=-O3 CXXFLAGS=-O3 ${SH_EXECUTABLE} ${EPA_BASE}/Source/build_${PACKAGE}/bootstrap --prefix=${${PACKAGE}_INSTALL_PREFIX} --parallel=8
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND} install
  )

list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
