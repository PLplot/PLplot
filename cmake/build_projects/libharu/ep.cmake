# libharu/ep.cmake
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build and test of libharu.

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
set(libharu_URL http://libharu.org/files/libharu-2.1.0.tar.gz)
set(libharu_URL_MD5 0623b8fb08ae1b28af08b2cdbd66b662)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(BP_PATH_NODLL "${BP_PATH}")
  #set(BP_PATH "${EP_BASE}/Build/build_libharu/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for libharu = ${BP_PATH}")

ExternalProject_Add(
  build_libharu
  URL ${libharu_URL}
  URL_MD5 ${libharu_URL_MD5}
  # Note -DPOST_2.1.0=OFF is essential for the 2.1.0 version, but you
  # should drop this option for anything after 2.1.0.  Also note that
  # -DLIBHARU_EXAMPLES=ON builds the demos, but does not test them.
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_CMAKE_COMMAND} -DPOST_2.1.0=OFF -DLIBHARU_EXAMPLES=ON ${EP_BASE}/Source/build_libharu
  BUILD_COMMAND ${BP_PARALLEL_BUILD_COMMAND}
  INSTALL_COMMAND ${BP_PARALLEL_BUILD_COMMAND} install
  STEP_TARGETS download update_build_system configure build install test
  )

# Use custom command approach to generate real file dependencies
# rather than time stamps alone.
add_custom_command(
  OUTPUT
  ${EP_BASE}/Source/build_libharu/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/demo/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/src/hpdf_page_operator.c
  ${EP_BASE}/Source/build_libharu/src/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/cmake/modules/haru.cmake
  ${EP_BASE}/Source/build_libharu/cmake/modules/summary.cmake
  ${EP_BASE}/Source/build_libharu/include/hpdf_consts.h
  ${EP_BASE}/Source/build_libharu/include/hpdf_config.h.cmake
  ${EP_BASE}/Source/build_libharu/include/hpdf.h
  ${EP_BASE}/Source/build_libharu/include/CMakeLists.txt
  # File that is patched.
  ${EP_BASE}/Source/build_libharu/src/hpdf_streams.c
  COMMAND ${CMAKE_COMMAND} -E make_directory ${EP_BASE}/Source/build_libharu/cmake/modules
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/CMakeLists.txt
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/demo/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/demo/CMakeLists.txt
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/src/hpdf_page_operator.c
  ${EP_BASE}/Source/build_libharu/src/hpdf_page_operator.c
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/src/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/src/CMakeLists.txt
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/cmake/modules/haru.cmake
  ${EP_BASE}/Source/build_libharu/cmake/modules/haru.cmake
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/cmake/modules/summary.cmake
  ${EP_BASE}/Source/build_libharu/cmake/modules/summary.cmake
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/include/hpdf_consts.h
  ${EP_BASE}/Source/build_libharu/include/hpdf_consts.h
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/include/hpdf_config.h.cmake
  ${EP_BASE}/Source/build_libharu/include/hpdf_config.h.cmake
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/include/hpdf.h
  ${EP_BASE}/Source/build_libharu/include/hpdf.h
  COMMAND ${CMAKE_COMMAND} -E copy 
  ${CMAKE_SOURCE_DIR}/libharu/include/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/include/CMakeLists.txt
  COMMAND ${PATCH_EXECUTABLE} -d ${EP_BASE}/Source/build_libharu -p1 < ${CMAKE_SOURCE_DIR}/libharu/include_hpdf_config.h.patch
  COMMENT "Updating of libharu build system"
  DEPENDS
  ${CMAKE_SOURCE_DIR}/libharu/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/libharu/demo/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/libharu/src/hpdf_page_operator.c
  ${CMAKE_SOURCE_DIR}/libharu/src/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/libharu/cmake/modules/haru.cmake
  ${CMAKE_SOURCE_DIR}/libharu/cmake/modules/summary.cmake
  ${CMAKE_SOURCE_DIR}/libharu/include/hpdf_consts.h
  ${CMAKE_SOURCE_DIR}/libharu/include/hpdf_config.h.cmake
  ${CMAKE_SOURCE_DIR}/libharu/include/hpdf.h
  ${CMAKE_SOURCE_DIR}/libharu/include/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/libharu/include_hpdf_config.h.patch
  )
ExternalProject_Add_Step(build_libharu update_build_system
  COMMENT "Updated libharu build system"
  DEPENDEES download
  DEPENDERS configure
  DEPENDS
  ${EP_BASE}/Source/build_libharu/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/demo/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/src/hpdf_page_operator.c
  ${EP_BASE}/Source/build_libharu/src/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/cmake/modules/haru.cmake
  ${EP_BASE}/Source/build_libharu/cmake/modules/summary.cmake
  ${EP_BASE}/Source/build_libharu/include/hpdf_consts.h
  ${EP_BASE}/Source/build_libharu/include/hpdf_config.h.cmake
  ${EP_BASE}/Source/build_libharu/include/hpdf.h
  ${EP_BASE}/Source/build_libharu/include/CMakeLists.txt
  ${EP_BASE}/Source/build_libharu/src/hpdf_streams.c
  ALWAYS OFF
  )

# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "shapelib restored original BP_PATH = ${BP_PATH}")
