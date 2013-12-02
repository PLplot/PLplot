# libharu/CMakeLists.txt
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build of libharu.

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

set(PACKAGE libharu)

# Protect against configuring a build twice in one CMake call
if(configured)
  return()
endif(configured)
set(configured ON)

# Data that is related to downloads.
set(URL http://${PACKAGE}.org/files/${PACKAGE}-2.2.1.tar.gz)
set(URL_MD5 63da8e087744f1d6cf16f9501b1cb766)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(EPA_PATH_NODLL "${EPA_PATH}")
  #set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

ExternalProject_Add(
  build_${PACKAGE}
  URL ${URL}
  URL_MD5 ${URL_MD5}
  PATCH_COMMAND ""
  # Do not use -DLIBHARU_EXAMPLES=ON (which builds the demos, but does
  # not test them) because latest release does not include demos.
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_CMAKE_COMMAND} ${EPA_BASE}/Source/build_${PACKAGE}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND} install
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-patch
  COMMAND ${CMAKE_COMMAND} -E echo "Apply all patches"
  COMMAND ${PATCH_EXECUTABLE} -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE}/cmake.patch
  COMMAND ${PATCH_EXECUTABLE} -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE}/large_font.patch
  COMMAND ${PATCH_EXECUTABLE} -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE}/remove_configured.patch
  COMMAND ${PATCH_EXECUTABLE} -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE}/visibility.patch
  DEPENDS
  ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE}/cmake.patch
  ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE}/large_font.patch
  ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE}/remove_configured.patch
  ${CMAKE_CURRENT_SOURCE_DIR}/${PACKAGE}/visibility.patch
  APPEND
  )

list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
