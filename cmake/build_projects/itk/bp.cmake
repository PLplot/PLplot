# itk/bp.cmake
# CMakeLists.txt file to configure the build of itk.

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

# These build configuration details for itk adapted from
# the fink build
# <http://www.mail-archive.com/fink-commits@lists.sourceforge.net/msg113511.html>
# Protect against configuring a build twice in one CMake call
if(itk_configured)
  return()
endif(itk_configured)
set(itk_configured ON)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(BP_ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(itk_dependencies_LIST tk tcl)
# Remove dependencies that should be ignored.
if(itk_dependencies_LIST)
  list(REMOVE_ITEM itk_dependencies_LIST ${BP_ignored_dependencies_LIST})
endif(itk_dependencies_LIST)

set(itk_dependencies_targets)
foreach(build_configuration ${itk_dependencies_LIST})
  if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
    include(${build_configuration}/bp.cmake)
    list(APPEND itk_dependencies_targets build_${build_configuration})
  else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
    message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of itk has been installed another way.")
  endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
endforeach(build_configuration ${itk_dependences_LIST})

# This can be safely done only after above includes.
set(BP_PACKAGE itk)

# Data that is related to downloads.
set(ITCL_ITK_VERSION 4.0.0)
set(ITCL_ITK_LIBVERSION ${ITCL_ITK_VERSION})
set(${BP_PACKAGE}_URL http://downloads.sourceforge.net/project/incrtcl/%5bincr%20Tcl_Tk%5d-4-source/Itcl%20${ITCL_ITK_VERSION}/itk${ITCL_ITK_VERSION}.tar.gz)
set(${BP_PACKAGE}_DOWNLOAD_HASH_TYPE MD5)
set(${BP_PACKAGE}_DOWNLOAD_HASH e3600a9ad0fcdcbbc4138af5a4893b7e)

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
  set(ITK_ENABLE_64_BIT --enable-64bit)
endif(BP_HAVE_64_BIT_OS)

ExternalProject_Add(
  build_${BP_PACKAGE}
  DEPENDS ${${BP_PACKAGE}_dependencies_targets}
  URL ${${BP_PACKAGE}_URL}
  URL_HASH ${${BP_PACKAGE}_DOWNLOAD_HASH_TYPE}=${${BP_PACKAGE}_DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${${BP_PACKAGE}_SET_CFLAGS} CPPFLAGS=-I${BP_CMAKE_INSTALL_PREFIX}/include ${source_PATH}/${BP_CONFIGURE_COMMAND} --mandir=${BP_CMAKE_INSTALL_PREFIX}/share/man ${ITK_ENABLE_64_BIT} --with-itcl=${BP_CMAKE_INSTALL_PREFIX}/lib/itcl${ITCL_ITK_LIBVERSION}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND} install
  )

add_custom_command(
  OUTPUT
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-build
  COMMAND echo "Replace build-tree locations by install-tree locations"
  COMMAND ${SED_EXECUTABLE}
  -e "s@^\\(ITK_SRC_DIR='\\).*@\\1${BP_CMAKE_INSTALL_PREFIX}/include'@"
  # Comment out the next since itk build system does not configure any
  # of these variables so they are left in symbolic "@" form.
  #-e "/ITK_B/s@='\\(-L\\)\\?.*build_itk@='\\1${BP_CMAKE_INSTALL_PREFIX}/lib@"
  -i itkConfig.sh
  APPEND
  )

add_custom_command(
  OUTPUT
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-install
  COMMAND echo "Install-tree fixups"
  COMMAND ${CHMOD_EXECUTABLE} -v ${SO_NUMERICAL_PERMISSIONS} ${BP_CMAKE_INSTALL_PREFIX}/lib/itk${ITCL_ITK_LIBVERSION}/libitk${ITCL_ITK_LIBVERSION}.so
  APPEND
  )

list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
