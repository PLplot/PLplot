# ndiff/ep.cmake
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build and test of ndiff.

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

set(ndiff_URL ftp://ftp.math.utah.edu/pub/misc/ndiff-2.00.tar.gz)
# TEMPORARY local version for debugging
set(ndiff_URL /home/software/ndiff/ndiff-2.00.tar.gz)

# The MD5 value below is correct for the gpg --verified version
# Use URL_MD5 because ExternalProject_Add doesn't deal with gpg --version.
# In any case, gpg is not available for the MSYS case.
set(ndiff_URL_MD5 885548b4dc26e72c5455bebb5ba6c16d)

ExternalProject_Add(
  build_ndiff
  URL ${ndiff_URL}
  URL_MD5 ${ndiff_URL_MD5} 
  CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${EP_BASE}/Install/build_ndiff
  BUILD_COMMAND ${BP_PARALLEL_BUILD_COMMAND}
  TEST_BEFORE_INSTALL ON
  TEST_COMMAND ${BP_PARALLEL_CTEST_COMMAND}
  INSTALL_COMMAND ${BP_PARALLEL_BUILD_COMMAND} install
  STEP_TARGETS download update_build_system configure build install test
  )

# Use custom command approach to generate real file dependencies
# rather than time stamps alone.
add_custom_command(
  OUTPUT
  ${EP_BASE}/Source/build_ndiff/CMakeLists.txt
  ${EP_BASE}/Source/build_ndiff/config.h.cmake
  COMMAND cp -f ${CMAKE_SOURCE_DIR}/ndiff/CMakeLists.txt ${CMAKE_SOURCE_DIR}/ndiff/config.h.cmake ${EP_BASE}/Source/build_ndiff
  COMMENT "Updating of ndiff build system"
  DEPENDS
  ${CMAKE_SOURCE_DIR}/ndiff/CMakeLists.txt
  ${CMAKE_SOURCE_DIR}/ndiff/config.h.cmake
  )
ExternalProject_Add_Step(build_ndiff update_build_system
  COMMENT "Updated ndiff build system"
  DEPENDEES download
  DEPENDERS configure
  DEPENDS
  ${EP_BASE}/Source/build_ndiff/CMakeLists.txt
  ${EP_BASE}/Source/build_ndiff/config.h.cmake
  ALWAYS OFF
  )
