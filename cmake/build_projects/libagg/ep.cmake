# libagg/ep.cmake
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build and test of libagg.

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
set(libagg_URL http://www.antigrain.com/agg-2.5.tar.gz)
set(libagg_URL_MD5 0229a488bc47be10a2fee6cf0b2febd6)

# Data that is related to the PATH that must be used.
if(MSYS)
  #set(BP_PATH_NODLL "${BP_PATH}")
  #set(BP_PATH "${EP_BASE}/Build/build_libagg/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
endif(MSYS)
#message(STATUS "modified BP_PATH for libagg = ${BP_PATH}")

ExternalProject_Add(
  build_libagg
  URL ${libagg_URL}
  URL_MD5 ${libagg_URL_MD5} 
  CONFIGURE_COMMAND env PATH=${BP_PATH} ${BP_CMAKE_COMMAND} ${EP_BASE}/Source/build_libagg
  BUILD_COMMAND ${BP_PARALLEL_BUILD_COMMAND}
  INSTALL_COMMAND ${BP_PARALLEL_BUILD_COMMAND} install
  STEP_TARGETS download update_build_system configure build install
  )

# Use custom command approach to generate real file dependencies
# rather than time stamps alone.
add_custom_command(
  OUTPUT
  ${EP_BASE}/Source/build_libagg/CMakeLists.txt
  COMMAND cp -f ${CMAKE_SOURCE_DIR}/libagg/CMakeLists.txt
  ${EP_BASE}/Source/build_libagg
  COMMENT "Updating of libagg build system"
  DEPENDS
  ${CMAKE_SOURCE_DIR}/libagg/CMakeLists.txt
  )
ExternalProject_Add_Step(build_libagg update_build_system
  COMMENT "Updated libagg build system"
  DEPENDEES download
  DEPENDERS configure
  DEPENDS
  ${EP_BASE}/Source/build_libagg/CMakeLists.txt
  ALWAYS OFF
  )

# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "libagg restored original BP_PATH = ${BP_PATH}")
