# shapelib/ep.cmake
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build and test of shapelib.

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

# Data that is related to downloads.
set(shapelib_URL http://download.osgeo.org/shapelib/shapelib-1.3.0.tar.gz)
set(shapelib_URL_MD5 2ff7d0b21d4b7506b452524492795f77)

set(shape_eg_data_URL http://dl.maptools.org/dl/shapelib/shape_eg_data.zip)
set(shape_eg_data_URL_MD5 36208abd5d34c5c80101d8b214109f0d)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  set(BP_PATH_NODLL "${BP_PATH}")
  set(BP_PATH "${EP_BASE}/Build/build_shapelib/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for shapelib = ${BP_PATH}")

ExternalProject_Add(
  build_shapelib
  URL ${shapelib_URL}
  URL_MD5 ${shapelib_URL_MD5}
  CONFIGURE_COMMAND env PATH=${BP_PATH} ${BP_CMAKE_COMMAND} -DEG_DATA:PATH=${EP_BASE}/Source/build_shapelib/eg_data ${EP_BASE}/Source/build_shapelib
  BUILD_COMMAND env PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND}
  TEST_BEFORE_INSTALL ON
  TEST_COMMAND env PATH=${BP_PATH} ${BP_PARALLEL_CTEST_COMMAND}
  INSTALL_COMMAND env PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND} install
  STEP_TARGETS download update_build_system get_eg_data configure build install test
  )

# Use custom command approach to allow more than one COMMAND and/or
# generate real file dependencies rather than time stamps alone.
add_custom_command(
  OUTPUT
  ${EP_BASE}/Source/build_shapelib/CMakeLists.txt
  COMMAND cp -f ${CMAKE_SOURCE_DIR}/shapelib/CMakeLists.txt ${EP_BASE}/Source/build_shapelib
  COMMENT "Updating of shapelib build system"
  DEPENDS
  ${CMAKE_SOURCE_DIR}/shapelib/CMakeLists.txt
  )
ExternalProject_Add_Step(build_shapelib update_build_system
  COMMENT "Updated shapelib build system"
  DEPENDEES download
  DEPENDERS configure
  DEPENDS
  ${EP_BASE}/Source/build_shapelib/CMakeLists.txt
  ALWAYS OFF
  )

add_custom_command(
  OUTPUT
  ${EP_BASE}/Download/build_shapelib/shape_eg_data.zip
  COMMAND ${CMAKE_COMMAND} -DURL:STRING=${shape_eg_data_URL} -DFILE:FILEPATH=${EP_BASE}/Download/build_shapelib/shape_eg_data.zip -DMD5:STRING=${shape_eg_data_URL_MD5} -P ${CMAKE_SOURCE_DIR}/download_check.cmake
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${EP_BASE}/Source/build_shapelib/eg_data
  COMMAND unzip -q ${EP_BASE}/Download/build_shapelib/shape_eg_data.zip -d ${EP_BASE}/Source/build_shapelib/eg_data
  COMMENT "getting eg_data for shapelib test"
  )
ExternalProject_Add_Step(build_shapelib get_eg_data
  COMMENT "got eg_data for shapelib test"
  DEPENDEES download
  DEPENDERS configure
  DEPENDS
  ${EP_BASE}/Download/build_shapelib/shape_eg_data.zip
  ALWAYS OFF
  )

# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "shapelib restored original BP_PATH = ${BP_PATH}")
