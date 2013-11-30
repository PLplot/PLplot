# cmake/bp.cmake
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
# have run "include(ExternalProject)" and set EP_BASE variable (and
# corresponding directory property) as well as various BP variables
# used below that configure how the External_Project functions
# operate.

set(BP_PACKAGE cmake)

# Protect against configuring a build twice in one CMake call
if(${BP_PACKAGE}_configured)
  return()
endif(${BP_PACKAGE}_configured)
set(${BP_PACKAGE}_configured ON)

# Data that is related to downloads.
set(BP_CMAKE_DIR v2.8)
set(BP_CMAKE_VERSION 2.8.11.1)
set(${BP_PACKAGE}_URL http://www.cmake.org/files/${BP_CMAKE_DIR}/cmake-${BP_CMAKE_VERSION}.tar.gz)
set(${BP_PACKAGE}_URL_MD5 df5324a3b203373a9e0a04b924281a43)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  set(BP_PATH_NODLL "${BP_PATH}")
  set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
  determine_msys_path(${BP_PACKAGE}_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
else(MSYS_PLATFORM)
  set(${BP_PACKAGE}_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")

ExternalProject_Add(
  build_${BP_PACKAGE}
  URL ${${BP_PACKAGE}_URL}
  URL_MD5 ${${BP_PACKAGE}_URL_MD5}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} CFLAGS=-O3 CXXFLAGS=-O3 ${SH_EXECUTABLE} ${EP_BASE}/Source/build_${BP_PACKAGE}/bootstrap --prefix=${${BP_PACKAGE}_INSTALL_PREFIX} --parallel=8
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND} install
  )

list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
