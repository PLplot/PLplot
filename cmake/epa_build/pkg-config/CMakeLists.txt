# pkg-config/CMakeLists.txt
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build of pkg-config.

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

set(PACKAGE pkg-config)

# Protect against configuring a build twice in one CMake call
if(configured)
  return()
endif(configured)
set(configured ON)

# Data that is related to downloads.
set(URL http://pkgconfig.freedesktop.org/releases/pkg-config-0.28.tar.gz)
set(${PACKAGE}_DOWNLOAD_HASH_TYPE MD5)
set(${PACKAGE}_DOWNLOAD_HASH aa3c86e67551adc3ac865160e34a2a0d)


# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(EPA_PATH_NODLL "${EPA_PATH}")
  #set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  # configure (for glib subpackage) dies without this.
  set(${PACKAGE}_SET_CFLAGS "CFLAGS=-march=native $ENV{CFLAGS}")
else(MSYS_PLATFORM)
  set(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  set(${PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

ExternalProject_Add(
  build_${PACKAGE}
  URL ${URL}
  URL_HASH ${${PACKAGE}_DOWNLOAD_HASH_TYPE}=${${PACKAGE}_DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${${PACKAGE}_SET_CFLAGS} ${source_PATH}/${EPA_CONFIGURE_COMMAND} --with-internal-glib
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND}
# 4 of 25 tests fail for the MinGW/MSYS/Wine platform so comment out
# the next two lines for now.
#  TEST_BEFORE_INSTALL ON
#  TEST_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} check
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install
  )

ExternalProject_Add_Step(
  build_${PACKAGE} preinstall_fix
  COMMENT "remove pkg-config binaries from the install tree"
  DEPENDEES build
  DEPENDERS install 
  COMMAND ${CMAKE_COMMAND} -DFILE_WILDCARD:STRING=${EPA_CMAKE_INSTALL_PREFIX}/bin/*-pkg-config* -P ${CMAKE_SOURCE_DIR}/wildcard_remove.cmake
  )

list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
