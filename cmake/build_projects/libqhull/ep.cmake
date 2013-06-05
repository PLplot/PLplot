# libqhull/ep.cmake
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build and test of libqhull.

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
set(libqhull_URL http://www.qhull.org/download/qhull-2010.1-src.tgz)
set(libqhull_URL_MD5 e64138470acdeb18f752a0bc2a11ceb4)

# Data that is related to the PATH that must be used.
if(MSYS)
  #set(BP_PATH_NODLL "${BP_PATH}")
  #set(BP_PATH "${EP_BASE}/Build/build_libqhull/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
endif(MSYS)
#message(STATUS "modified BP_PATH for libqhull = ${BP_PATH}")

ExternalProject_Add(
  build_libqhull
  URL ${libqhull_URL}
  URL_MD5 ${libqhull_URL_MD5} 
  CONFIGURE_COMMAND env PATH=${BP_PATH} ${BP_CMAKE_COMMAND} ${EP_BASE}/Source/build_libqhull
  BUILD_COMMAND ${BP_PARALLEL_BUILD_COMMAND}
  INSTALL_COMMAND ${BP_PARALLEL_BUILD_COMMAND} install
  STEP_TARGETS download update_build_system configure build install test
  )

# Use custom command approach to generate real file dependencies
# rather than time stamps alone.
add_custom_command(
  OUTPUT
  ${EP_BASE}/Source/build_libqhull/CMakeLists.txt
  ${EP_BASE}/Source/build_libqhull/src/CMakeLists.txt
  ${EP_BASE}/Source/build_libqhull/src/libqhull.h
  ${EP_BASE}/Source/build_libqhull/src/mem.h
  ${EP_BASE}/Source/build_libqhull/src/unix.c
  COMMAND cp -f ${CMAKE_SOURCE_DIR}/libqhull/CMakeLists.txt ${EP_BASE}/Source/build_libqhull
  COMMAND cp -f ${CMAKE_SOURCE_DIR}/libqhull/src/CMakeLists.txt ${CMAKE_SOURCE_DIR}/libqhull/src/libqhull.h ${CMAKE_SOURCE_DIR}/libqhull/src/mem.h ${CMAKE_SOURCE_DIR}/libqhull/src/unix.c ${EP_BASE}/Source/build_libqhull/src
  COMMENT "Updating of libqhull build system"
  DEPENDS
  ${CMAKE_SOURCE_DIR}/libqhull/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/libqhull/src/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/libqhull/src/libqhull.h
  ${CMAKE_SOURCE_DIR}/libqhull/src/mem.h
  ${CMAKE_SOURCE_DIR}/libqhull/src/unix.c
  )
ExternalProject_Add_Step(build_libqhull update_build_system
  COMMENT "Updated libqhull build system"
  DEPENDEES download
  DEPENDERS configure
  DEPENDS
  ${EP_BASE}/Source/build_libqhull/CMakeLists.txt
  ${EP_BASE}/Source/build_libqhull/src/CMakeLists.txt
  ${EP_BASE}/Source/build_libqhull/src/libqhull.h
  ${EP_BASE}/Source/build_libqhull/src/mem.h
  ${EP_BASE}/Source/build_libqhull/src/unix.c
  ALWAYS OFF
  )

# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "shapelib restored original BP_PATH = ${BP_PATH}")
