# itcl3/bp.cmake
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
# have run "include(ExternalProject)" and set EP_BASE variable (and
# corresponding directory property) as well as various BP variables
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
set(BP_ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(itcl3_dependencies_LIST tk tcl)
# Remove dependencies that should be ignored.
if(itcl3_dependencies_LIST)
  list(REMOVE_ITEM itcl3_dependencies_LIST ${BP_ignored_dependencies_LIST})
endif(itcl3_dependencies_LIST)

set(itcl3_dependencies_targets)
foreach(build_configuration ${itcl3_dependencies_LIST})
  if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
    include(${build_configuration}/bp.cmake)
    list(APPEND itcl3_dependencies_targets build_${build_configuration})
  else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
    message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of itcl3 has been installed another way.")
  endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
endforeach(build_configuration ${itcl3_dependences_LIST})

# This can be safely done only after above includes.
set(BP_PACKAGE itcl3)

# Data that is related to downloads.
set(ITCL3_VERSION 3.4.1)
set(ITCL3_LIBVERSION 3.4)
set(${BP_PACKAGE}_URL http://downloads.sourceforge.net/project/incrtcl/%5bIncr%20Tcl_Tk%5d-source/${ITCL3_VERSION}/itcl${ITCL3_VERSION}.tar.gz)
set(${BP_PACKAGE}_DOWNLOAD_HASH_TYPE MD5)
set(${BP_PACKAGE}_DOWNLOAD_HASH e7c98e0f69df1a675073ddd3344f0b7f)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(BP_PATH_NODLL "${BP_PATH}")
  #set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
  set(${BP_PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
else(MSYS_PLATFORM)
  set(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
  set(${BP_PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")

if(BP_HAVE_64_BIT_OS)
  set(ITCL3_ENABLE_64_BIT --enable-64bit)
endif(BP_HAVE_64_BIT_OS)

ExternalProject_Add(
  build_${BP_PACKAGE}
  DEPENDS ${${BP_PACKAGE}_dependencies_targets}
  URL ${${BP_PACKAGE}_URL}
  URL_HASH ${${BP_PACKAGE}_DOWNLOAD_HASH_TYPE}=${${BP_PACKAGE}_DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${${BP_PACKAGE}_SET_CFLAGS} ${source_PATH}/${BP_CONFIGURE_COMMAND} --mandir=${BP_CMAKE_INSTALL_PREFIX}/share/man ${ITCL3_ENABLE_64_BIT} --with-tcl=${BP_CMAKE_INSTALL_PREFIX}/lib --with-tclinclude=${BP_CMAKE_INSTALL_PREFIX}/include 
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND} install
  )

add_custom_command(
  OUTPUT
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-build
  COMMAND echo "Replace build-tree locations by install-tree locations"
  COMMAND ${SED_EXECUTABLE}
  # Cover two different patterns of uppercasing.
  -e "s@^\\(itcl_SRC_DIR='\\).*@\\1${BP_CMAKE_INSTALL_PREFIX}/include'@"
  -e "s@^\\(ITCL_SRC_DIR='\\).*@\\1${BP_CMAKE_INSTALL_PREFIX}/include'@"
  # Cover two different patterns of uppercasing.
  -e "/itcl_B/s@='\\(-L\\)\\?.*build_itcl3@='\\1${BP_CMAKE_INSTALL_PREFIX}/lib/itcl${ITCL3_LIBVERSION}@"
  -e "/ITCL_B/s@='\\(-L\\)\\?.*build_itcl3@='\\1${BP_CMAKE_INSTALL_PREFIX}/lib/itcl${ITCL3_LIBVERSION}@"
  # Overwrite original file.
  -i itclConfig.sh
  APPEND
  )

add_custom_command(
  OUTPUT
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-install
  COMMAND echo "Install-tree fixups"
  COMMAND ${CHMOD_EXECUTABLE} -v ${SO_NUMERICAL_PERMISSIONS} ${BP_CMAKE_INSTALL_PREFIX}/lib/itcl${ITCL3_LIBVERSION}/libitcl${ITCL3_LIBVERSION}.so
  APPEND
  )


list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
