# tk/CMakeLists.txt

# Configure the build of tk.

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

# These build configuration details for Tk adapted from information in
# <http://www.linuxfromscratch.org/blfs/view/svn/general/tk.html>.

set(PACKAGE tk)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(dependencies_LIST tcl)

# Do boilerplate tasks that must be done for each different project
# that is configured as part of epa_build.
epa_boilerplate(
  ignored_dependencies_LIST
  PACKAGE
  dependencies_LIST
  dependencies_targets
  EPA_visited_subdirectories_LIST
  EPA_processed_subdirectories_LIST
  EPA_needed_subdirectories_LIST
  EPA_build_targets_LIST
  EPA_PATH
  source_PATH
  IF_RETURN
  ) 

# Propagate list changes to parent scope.
set(EPA_visited_subdirectories_LIST ${EPA_visited_subdirectories_LIST} PARENT_SCOPE)
set(EPA_processed_subdirectories_LIST ${EPA_processed_subdirectories_LIST} PARENT_SCOPE)
set(EPA_needed_subdirectories_LIST ${EPA_needed_subdirectories_LIST} PARENT_SCOPE)
set(EPA_build_targets_LIST ${EPA_build_targets_LIST} PARENT_SCOPE)

if(IF_RETURN)
  return()
endif(IF_RETURN)

# Data that is related to the PATH that must be used.
# N.B. note below that we always use the unix subdirectory of the
# source tree to find the configure script.  Note, there is a macosx
# and win subdirectory there as well that contain configure scripts
# with additional options for those platforms, but for now the unix
# version of configure may be good enough for our needs for all
# platforms.
if(MSYS_PLATFORM)
  set(source_PATH ${source_PATH}\unix)
else(MSYS_PLATFORM)
  set(source_PATH ${source_PATH}/unix)
endif(MSYS_PLATFORM)

set(CFLAGS "$ENV{CFLAGS}")
# Drop -fvisibility=hidden since that option may not work for this package.
string(REGEX REPLACE "-fvisibility=hidden" "" CFLAGS "${CFLAGS}")

# Data that is related to downloads.
set(TCL_TK_VERSION 8.6.1)
string(REGEX REPLACE "\\.[0-9]$" "" TCL_TK_LIBVERSION ${TCL_TK_VERSION})
set(URL http://downloads.sourceforge.net/project/tcl/Tcl/${TCL_TK_VERSION}/tk${TCL_TK_VERSION}-src.tar.gz)
set(DOWNLOAD_HASH_TYPE MD5)
set(DOWNLOAD_HASH 63f21c3a0e0cefbd854b4eb29b129ac6)

if(EPA_HAVE_64_BIT_OS)
  set(ENABLE_64_BIT --enable-64bit)
endif(EPA_HAVE_64_BIT_OS)

ExternalProject_Add(
  build_${PACKAGE}
  DEPENDS ${dependencies_targets}
  URL ${URL}
  URL_HASH ${DOWNLOAD_HASH_TYPE}=${DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} "CFLAGS=${CFLAGS}" CPPFLAGS=-I${EPA_CMAKE_INSTALL_PREFIX}/include ${source_PATH}/${EPA_CONFIGURE_COMMAND} --mandir=${EPA_CMAKE_INSTALL_PREFIX}/share/man ${ENABLE_64_BIT}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-build
  COMMAND echo "Replace build-tree locations by install-tree locations"
  COMMAND ${SED_EXECUTABLE}
  -e "s@^\\(TK_SRC_DIR='\\).*@\\1${EPA_CMAKE_INSTALL_PREFIX}/include'@"
  -e "/TK_B/s@='\\(-L\\)\\?.*build_tk@='\\1${EPA_CMAKE_INSTALL_PREFIX}/lib@"
  -i tkConfig.sh
  APPEND
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-install
  COMMAND echo "Install-tree fixups"
  COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install-private-headers
  COMMAND ${LN_EXECUTABLE} -v -sf wish${TCL_TK_LIBVERSION} ${EPA_CMAKE_INSTALL_PREFIX}/bin/wish
  COMMAND ${CHMOD_EXECUTABLE} -v ${SO_NUMERICAL_PERMISSIONS} ${EPA_CMAKE_INSTALL_PREFIX}/lib/libtk${TCL_TK_LIBVERSION}.so
  APPEND
  )
