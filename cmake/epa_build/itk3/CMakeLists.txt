# itk3/CMakeLists.txt
# CMakeLists.txt file to configure the build of itk3.

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

# These build configuration details for itk3 adapted from
# the fink build
# <http://www.mail-archive.com/fink-commits@lists.sourceforge.net/msg113511.html>
# Protect against configuring a build twice in one CMake call
if(itk3_configured)
  return()
endif(itk3_configured)
set(itk3_configured ON)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(EPA_ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(itk3_dependencies_LIST itcl3 tk tcl)
# Remove dependencies that should be ignored.
if(itk3_dependencies_LIST)
  list(REMOVE_ITEM itk3_dependencies_LIST ${EPA_ignored_dependencies_LIST})
endif(itk3_dependencies_LIST)

set(itk3_dependencies_targets)
foreach(build_configuration ${itk3_dependencies_LIST})
  if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    include(${build_configuration}/CMakeLists.txt)
    list(APPEND itk3_dependencies_targets build_${build_configuration})
  else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of itk3 has been installed another way.")
  endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
endforeach(build_configuration ${itk3_dependences_LIST})

# This can be safely done only after above includes.
set(PACKAGE itk3)

# Data that is related to downloads.
set(ITK3_VERSION 3.3)
set(ITK3_LIBVERSION 3.3)
set(URL http://downloads.sourceforge.net/project/incrtcl/%5bIncr%20Tcl_Tk%5d-source/${ITK3_VERSION}-RC/itk${ITK3_VERSION}.tar.gz)
set(${PACKAGE}_DOWNLOAD_HASH_TYPE MD5)
set(${PACKAGE}_DOWNLOAD_HASH a97c17f3cfa5e377f43073c653c501b5)

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
  set(ITK3_ENABLE_64_BIT --enable-64bit)
endif(EPA_HAVE_64_BIT_OS)

ExternalProject_Add(
  build_${PACKAGE}
  DEPENDS ${${PACKAGE}_dependencies_targets}
  URL ${URL}
  URL_HASH ${${PACKAGE}_DOWNLOAD_HASH_TYPE}=${${PACKAGE}_DOWNLOAD_HASH}
  # Order of CPPFLAGS is important since want to access the version 3
  # itcl header.
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${${PACKAGE}_SET_CFLAGS} "CPPFLAGS=-I${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION} -I${EPA_CMAKE_INSTALL_PREFIX}/include" ${source_PATH}/configure --prefix=${INCRTCL3_PREFIX} --exec-prefix=${INCRTCL3_PREFIX} --includedir=${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION} --mandir=${INCRTCL3_PREFIX}/share/man ${ITK3_ENABLE_64_BIT} --with-tcl=${EPA_CMAKE_INSTALL_PREFIX}/lib --with-tk=${EPA_CMAKE_INSTALL_PREFIX}/lib --with-itcl=${INCRTCL3_PREFIX}/lib/itcl${ITCL3_LIBVERSION}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-build
  COMMAND echo "Replace build-tree locations by install-tree locations"
  COMMAND ${SED_EXECUTABLE}
  # Cover two different patterns of uppercasing.
  -e "s@^\\(itk_SRC_DIR='\\).*@\\1${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION}'@"
  -e "s@^\\(ITK_SRC_DIR='\\).*@\\1${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION}'@"
  # Cover two different patterns of uppercasing.
  -e "/itk_B/s@='\\(-L\\)\\?.*build_itk3@='\\1${INCRTCL3_PREFIX}/lib/itk${ITK3_LIBVERSION}@"
  -e "/ITK_B/s@='\\(-L\\)\\?.*build_itk3@='\\1${INCRTCL3_PREFIX}/lib/itk${ITK3_LIBVERSION}@"
  # Overwrite original file.
  -i itkConfig.sh
  APPEND
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-install
  COMMAND echo "Install-tree fixups"
  COMMAND ${CHMOD_EXECUTABLE} -v ${SO_NUMERICAL_PERMISSIONS} ${INCRTCL3_PREFIX}/lib/itk${ITK3_LIBVERSION}/libitk${ITK3_LIBVERSION}.so
  APPEND
  )


list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
