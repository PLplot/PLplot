# shapelib/ep.cmake
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
# have run "include(ExternalProject)" and set EP_BASE variable (and
# corresponding directory property) as well as various BP variables
# used below that configure how the External_Project functions
# operate.

set(BP_PACKAGE shapelib)

# Data that is related to downloads.
set(${BP_PACKAGE}_URL http://download.osgeo.org/${BP_PACKAGE}/${BP_PACKAGE}-1.3.0.tar.gz)
set(${BP_PACKAGE}_URL_MD5 2ff7d0b21d4b7506b452524492795f77)

set(shape_eg_data_URL http://dl.maptools.org/dl/${BP_PACKAGE}/shape_eg_data.zip)
set(shape_eg_data_URL_MD5 36208abd5d34c5c80101d8b214109f0d)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  set(BP_PATH_NODLL "${BP_PATH}")
  set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")

ExternalProject_Add(
  build_${BP_PACKAGE}
  URL ${${BP_PACKAGE}_URL}
  URL_MD5 ${${BP_PACKAGE}_URL_MD5}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_CMAKE_COMMAND} -DEG_DATA:PATH=${EP_BASE}/Source/build_${BP_PACKAGE}/eg_data ${EP_BASE}/Source/build_${BP_PACKAGE}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND}
  TEST_BEFORE_INSTALL ON
  TEST_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_CTEST_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND} install
  )

# Add custom commands to the current no-command update step.
add_custom_command(
  OUTPUT
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-update
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/CMakeLists.txt
  ${EP_BASE}/Source/build_${BP_PACKAGE}/CMakeLists.txt
  COMMAND ${CMAKE_COMMAND} -DURL:STRING=${shape_eg_data_URL} -DFILE:FILEPATH=${EP_BASE}/Download/build_${BP_PACKAGE}/shape_eg_data.zip -DMD5:STRING=${shape_eg_data_URL_MD5} -P ${CMAKE_SOURCE_DIR}/download_check.cmake
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${EP_BASE}/Source/build_${BP_PACKAGE}/eg_data
  COMMAND ${UNZIP_EXECUTABLE} -q ${EP_BASE}/Download/build_${BP_PACKAGE}/shape_eg_data.zip -d ${EP_BASE}/Source/build_${BP_PACKAGE}/eg_data
  COMMENT "Custom updating of ${BP_PACKAGE}"
  DEPENDS
  ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/download_check.cmake
  # N.B. no file dependencies are worthwhile for the eg_data
  # chain above since it all starts with a download.
  APPEND
  )

list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
