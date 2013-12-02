# tcl/CMakeLists.txt
# CMakeLists.txt file to configure the build of tcl.

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

# These build configuration details for Tcl adapted from information in
# <http://www.linuxfromscratch.org/blfs/view/svn/general/tcl.html>.

# Protect against configuring a build twice in one CMake call
if(tcl_configured)
  return()
endif(tcl_configured)
set(tcl_configured ON)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(EPA_ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(tcl_dependencies_LIST)
# Remove dependencies that should be ignored.
if(tcl_dependencies_LIST)
  list(REMOVE_ITEM tcl_dependencies_LIST ${EPA_ignored_dependencies_LIST})
endif(tcl_dependencies_LIST)

set(tcl_dependencies_targets)
foreach(build_configuration ${tcl_dependencies_LIST})
  if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    include(${build_configuration}/CMakeLists.txt)
    list(APPEND tcl_dependencies_targets build_${build_configuration})
  else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of tcl has been installed another way.")
  endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
endforeach(build_configuration ${tcl_dependences_LIST})

# This can be safely done only after above includes.
set(PACKAGE tcl)

# These variables are also used by tk (and accessible to that
# build configuration because it depends on this one).
set(TCL_TK_VERSION 8.6.1)
string(REGEX REPLACE "\\.[0-9]$" "" TCL_TK_LIBVERSION ${TCL_TK_VERSION})

# Data that is related to downloads.
set(URL http://downloads.sourceforge.net/project/tcl/Tcl/${TCL_TK_VERSION}/tcl${TCL_TK_VERSION}-src.tar.gz)
set(${PACKAGE}_DOWNLOAD_HASH_TYPE MD5)
set(${PACKAGE}_DOWNLOAD_HASH aae4b701ee527c6e4e1a6f9c7399882e)

# Data that is related to the PATH that must be used.
# N.B. note below that we always use the unix subdirectory of the
# source tree to find the configure script.  Note, there is a macosx
# and win subdirectory there as well that contain configure scripts
# with additional options for those platforms, but for now the unix
# version of configure may be good enough for our needs for all
# platforms.
if(MSYS_PLATFORM)
  #set(EPA_PATH_NODLL "${EPA_PATH}")
  #set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}/unix")
  set(${PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
else(MSYS_PLATFORM)
  set(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}/unix")
  set(${PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

if(EPA_HAVE_64_BIT_OS)
  set(TCL_ENABLE_64_BIT --enable-64bit)
endif(EPA_HAVE_64_BIT_OS)

ExternalProject_Add(
  build_${PACKAGE}
  DEPENDS ${${PACKAGE}_dependencies_targets}
  URL ${URL}
  URL_HASH ${${PACKAGE}_DOWNLOAD_HASH_TYPE}=${${PACKAGE}_DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${${PACKAGE}_SET_CFLAGS} ${source_PATH}/${EPA_CONFIGURE_COMMAND} --mandir=${EPA_CMAKE_INSTALL_PREFIX}/share/man ${TCL_ENABLE_64_BIT}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-build
  COMMAND echo "Replace build-tree locations by install-tree locations"
  COMMAND ${SED_EXECUTABLE}
  -e "s@^\\(TCL_SRC_DIR='\\).*@\\1${EPA_CMAKE_INSTALL_PREFIX}/include'@"
  -e "/TCL_B/s@='\\(-L\\)\\?.*build_tcl@='\\1${EPA_CMAKE_INSTALL_PREFIX}/lib@"
  -i tclConfig.sh
  APPEND
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-install
  COMMAND echo "Install-tree fixups"
  COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install-private-headers
  COMMAND ${LN_EXECUTABLE} -v -sf tclsh${TCL_TK_LIBVERSION} ${EPA_CMAKE_INSTALL_PREFIX}/bin/tclsh
  COMMAND ${CHMOD_EXECUTABLE} -v ${SO_NUMERICAL_PERMISSIONS} ${EPA_CMAKE_INSTALL_PREFIX}/lib/libtcl${TCL_TK_LIBVERSION}.so
  APPEND
  )

list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
