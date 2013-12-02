# shapelib/CMakeLists.txt
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build of shapelib.

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

set(PACKAGE shapelib)

# Protect against configuring a build twice in one CMake call
if(configured)
  return()
endif(configured)
set(configured ON)

# Data that is related to downloads.
set(URL http://download.osgeo.org/${PACKAGE}/${PACKAGE}-1.3.0.tar.gz)
set(URL_MD5 2ff7d0b21d4b7506b452524492795f77)

set(shape_eg_data_URL http://dl.maptools.org/dl/${PACKAGE}/shape_eg_data.zip)
set(shape_eg_data_URL_MD5 36208abd5d34c5c80101d8b214109f0d)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  set(EPA_PATH_NODLL "${EPA_PATH}")
  set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

set(CFLAGS $ENV{CFLAGS})
# Eliminate this option which is not allowed for shapelib
string(REGEX REPLACE "-fvisibility=hidden" "" CFLAGS "${CFLAGS}") 

ExternalProject_Add(
  build_${PACKAGE}
  URL ${URL}
  URL_MD5 ${URL_MD5}
    CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} CFLAGS=${CFLAGS} ${EPA_CMAKE_COMMAND} -DEG_DATA:PATH=${EPA_BASE}/Source/build_${PACKAGE}/eg_data ${EPA_BASE}/Source/build_${PACKAGE}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND}
  TEST_BEFORE_INSTALL ON
  TEST_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_CTEST_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND} install
  )

# Add custom commands to the current no-command update step.
add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-update
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/${PACKAGE}/${PACKAGE}_CMakeLists.txt
  ${EPA_BASE}/Source/build_${PACKAGE}/CMakeLists.txt
  COMMAND ${CMAKE_COMMAND} -DURL:STRING=${shape_eg_data_URL} -DFILE:FILEPATH=${EPA_BASE}/Download/build_${PACKAGE}/shape_eg_data.zip -DMD5:STRING=${shape_eg_data_URL_MD5} -P ${CMAKE_SOURCE_DIR}/download_check.cmake
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${EPA_BASE}/Source/build_${PACKAGE}/eg_data
  COMMAND ${UNZIP_EXECUTABLE} -q ${EPA_BASE}/Download/build_${PACKAGE}/shape_eg_data.zip -d ${EPA_BASE}/Source/build_${PACKAGE}/eg_data
  COMMENT "Custom updating of ${PACKAGE}"
  DEPENDS
  ${CMAKE_SOURCE_DIR}/${PACKAGE}/${PACKAGE}_CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/download_check.cmake
  # N.B. no file dependencies are worthwhile for the eg_data
  # chain above since it all starts with a download.
  APPEND
  )

list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
