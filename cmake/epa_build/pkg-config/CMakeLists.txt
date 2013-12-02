# pkg-config/bp.cmake
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
# have run "include(ExternalProject)" and set EP_BASE variable (and
# corresponding directory property) as well as various BP variables
# used below that configure how the External_Project functions
# operate.

set(BP_PACKAGE pkg-config)

# Protect against configuring a build twice in one CMake call
if(${BP_PACKAGE}_configured)
  return()
endif(${BP_PACKAGE}_configured)
set(${BP_PACKAGE}_configured ON)

# Data that is related to downloads.
set(${BP_PACKAGE}_URL http://pkgconfig.freedesktop.org/releases/pkg-config-0.28.tar.gz)
set(${BP_PACKAGE}_DOWNLOAD_HASH_TYPE MD5)
set(${BP_PACKAGE}_DOWNLOAD_HASH aa3c86e67551adc3ac865160e34a2a0d)


# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(BP_PATH_NODLL "${BP_PATH}")
  #set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
  # configure (for glib subpackage) dies without this.
  set(${BP_PACKAGE}_SET_CFLAGS "CFLAGS=-march=native $ENV{CFLAGS}")
else(MSYS_PLATFORM)
  set(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
  set(${BP_PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")

ExternalProject_Add(
  build_${BP_PACKAGE}
  URL ${${BP_PACKAGE}_URL}
  URL_HASH ${${BP_PACKAGE}_DOWNLOAD_HASH_TYPE}=${${BP_PACKAGE}_DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${${BP_PACKAGE}_SET_CFLAGS} ${source_PATH}/${BP_CONFIGURE_COMMAND} --with-internal-glib
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND}
# 4 of 25 tests fail for the MinGW/MSYS/Wine platform so comment out
# the next two lines for now.
#  TEST_BEFORE_INSTALL ON
#  TEST_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND} check
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND} install
  )

ExternalProject_Add_Step(
  build_${BP_PACKAGE} preinstall_fix
  COMMENT "remove pkg-config binaries from the install tree"
  DEPENDEES build
  DEPENDERS install 
  COMMAND ${CMAKE_COMMAND} -DFILE_WILDCARD:STRING=${BP_CMAKE_INSTALL_PREFIX}/bin/*-pkg-config* -P ${CMAKE_SOURCE_DIR}/wildcard_remove.cmake
  )

list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
