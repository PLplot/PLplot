# libqhull/ep.cmake
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build of libqhull.

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

set(BP_PACKAGE libqhull)

# Data that is related to downloads.
set(${BP_PACKAGE}_URL http://www.qhull.org/download/qhull-2010.1-src.tgz)
set(${BP_PACKAGE}_URL_MD5 e64138470acdeb18f752a0bc2a11ceb4)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(BP_PATH_NODLL "${BP_PATH}")
  #set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")

ExternalProject_Add(
  build_${BP_PACKAGE}
  URL ${${BP_PACKAGE}_URL}
  URL_MD5 ${${BP_PACKAGE}_URL_MD5}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_CMAKE_COMMAND} ${EP_BASE}/Source/build_${BP_PACKAGE}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND} install
  )

# Add custom commands to the current no-command update step.
add_custom_command(
  OUTPUT
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-update
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/CMakeLists.txt
  ${EP_BASE}/Source/build_${BP_PACKAGE}/CMakeLists.txt
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/src/CMakeLists.txt
  ${EP_BASE}/Source/build_${BP_PACKAGE}/src/CMakeLists.txt
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/src/${BP_PACKAGE}.h
  ${EP_BASE}/Source/build_${BP_PACKAGE}/src/${BP_PACKAGE}.h
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/src/mem.h
  ${EP_BASE}/Source/build_${BP_PACKAGE}/src/mem.h
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/src/unix.c
  ${EP_BASE}/Source/build_${BP_PACKAGE}/src/unix.c
  COMMENT "Custom updating of ${BP_PACKAGE}"
  DEPENDS
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/src/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/src/${BP_PACKAGE}.h
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/src/mem.h
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/src/unix.c
  APPEND
  )

list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
