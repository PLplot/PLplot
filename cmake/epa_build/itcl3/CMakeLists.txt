# itcl3/CMakeLists.txt
# CMakeLists.txt file to configure the build of itcl3.

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

# These build configuration details for itcl3 adapted from
# the fink build
# <http://www.mail-archive.com/fink-commits@lists.sourceforge.net/msg113511.html>
# Protect against configuring a build twice in one CMake call
if(itcl3_configured)
  return()
endif(itcl3_configured)
set(itcl3_configured ON)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(EPA_ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(itcl3_dependencies_LIST tk tcl)
# Remove dependencies that should be ignored.
if(itcl3_dependencies_LIST)
  list(REMOVE_ITEM itcl3_dependencies_LIST ${EPA_ignored_dependencies_LIST})
endif(itcl3_dependencies_LIST)

set(itcl3_dependencies_targets)
foreach(build_configuration ${itcl3_dependencies_LIST})
  if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    include(${build_configuration}/CMakeLists.txt)
    list(APPEND itcl3_dependencies_targets build_${build_configuration})
  else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of itcl3 has been installed another way.")
  endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
endforeach(build_configuration ${itcl3_dependences_LIST})

# This can be safely done only after above includes.
set(PACKAGE itcl3)

# Data that is related to downloads.
set(ITCL3_VERSION 3.4.1)
set(ITCL3_LIBVERSION 3.4)
set(URL http://downloads.sourceforge.net/project/incrtcl/%5bIncr%20Tcl_Tk%5d-source/${ITCL3_VERSION}/itcl${ITCL3_VERSION}.tar.gz)
set(${PACKAGE}_DOWNLOAD_HASH_TYPE MD5)
set(${PACKAGE}_DOWNLOAD_HASH e7c98e0f69df1a675073ddd3344f0b7f)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(EPA_PATH_NODLL "${EPA_PATH}")
  #set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  set(${PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
else(MSYS_PLATFORM)
  set(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  set(${PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

# Eliminate this option which gives bad results for this package.
string(REGEX REPLACE "-fvisibility=hidden" "" ${PACKAGE}_SET_CFLAGS "${${PACKAGE}_SET_CFLAGS}")

if(EPA_HAVE_64_BIT_OS)
  set(ITCL3_ENABLE_64_BIT --enable-64bit)
endif(EPA_HAVE_64_BIT_OS)

# This variable provides the opportunity to set an alternative install
# prefix to distinguish what files are installed by version 3 of itcl
# and friends from what is installed by version 4 of itcl and friends.
# But currently just use the ordinary install location.
set(INCRTCL3_PREFIX ${EPA_CMAKE_INSTALL_PREFIX})

ExternalProject_Add(
  build_${PACKAGE}
  DEPENDS ${${PACKAGE}_dependencies_targets}
  URL ${URL}
  URL_HASH ${${PACKAGE}_DOWNLOAD_HASH_TYPE}=${${PACKAGE}_DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${${PACKAGE}_SET_CFLAGS} ${source_PATH}/configure --prefix=${INCRTCL3_PREFIX} --exec-prefix=${INCRTCL3_PREFIX} --includedir=${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION} --mandir=${INCRTCL3_PREFIX}/share/man ${ITCL3_ENABLE_64_BIT} --with-tcl=${EPA_CMAKE_INSTALL_PREFIX}/lib --with-tclinclude=${EPA_CMAKE_INSTALL_PREFIX}/include 
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-build
  COMMAND echo "Replace build-tree locations by install-tree locations"
  COMMAND ${SED_EXECUTABLE}
  # Cover two different patterns of uppercasing.
  -e "s@^\\(itcl_SRC_DIR='\\).*@\\1${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION}'@"
  -e "s@^\\(ITCL_SRC_DIR='\\).*@\\1${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION}'@"
  # Cover two different patterns of uppercasing.
  -e "/itcl_B/s@='\\(-L\\)\\?.*build_itcl3@='\\1${INCRTCL3_PREFIX}/lib/itcl${ITCL3_LIBVERSION}@"
  -e "/ITCL_B/s@='\\(-L\\)\\?.*build_itcl3@='\\1${INCRTCL3_PREFIX}/lib/itcl${ITCL3_LIBVERSION}@"
  # Overwrite original file.
  -i itclConfig.sh
  APPEND
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-install
  COMMAND ${CMAKE_COMMAND} -E echo "Install-tree fixups"
  COMMAND ${CHMOD_EXECUTABLE} -v ${SO_NUMERICAL_PERMISSIONS} ${INCRTCL3_PREFIX}/lib/itcl${ITCL3_LIBVERSION}/libitcl${ITCL3_LIBVERSION}.so
  COMMAND ${CMAKE_COMMAND} -E rename ${INCRTCL3_PREFIX}/lib/itclConfig.sh ${INCRTCL3_PREFIX}/lib/itcl${ITCL3_LIBVERSION}/itclConfig.sh
  APPEND
  )

list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
