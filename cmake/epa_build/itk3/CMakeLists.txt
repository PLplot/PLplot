# itk3/CMakeLists.txt

# Configure build of itk3.

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

# These build configuration details for itk3 adapted from
# the fink build
# <http://www.mail-archive.com/fink-commits@lists.sourceforge.net/msg113511.html>

set(PACKAGE itk3)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(dependencies_LIST itcl3 tk tcl)

# Do boilerplate tasks that must be done for each different project
# that is configured as part of epa_build.
epa_boilerplate(
  ignored_dependencies_LIST
  PACKAGE
  dependencies_LIST
  dependencies_targets
  EPA_PATH
  source_PATH
  ) 

set(CFLAGS "$ENV{CFLAGS}")
# Drop -fvisibility=hidden since that option does not work for itk3.
string(REGEX REPLACE "-fvisibility=hidden" "" CFLAGS "${CFLAGS}")

# This variable provides the opportunity to set an alternative install
# prefix to distinguish what files are installed by version 3 of itcl
# and friends from what is installed by version 4 of itcl and friends.
# But currently just use the ordinary install location.
set(INCRTCL3_PREFIX ${EPA_CMAKE_INSTALL_PREFIX})

# Data that is related to downloads.
set(VERSION 3.3)
set(LIBVERSION 3.3)
set(ITCL3_LIBVERSION 3.4)
set(URL http://downloads.sourceforge.net/project/incrtcl/%5bIncr%20Tcl_Tk%5d-source/${VERSION}-RC/itk${VERSION}.tar.gz)
set(DOWNLOAD_HASH_TYPE MD5)
set(DOWNLOAD_HASH a97c17f3cfa5e377f43073c653c501b5)

if(EPA_HAVE_64_BIT_OS)
  set(ENABLE_64_BIT --enable-64bit)
endif(EPA_HAVE_64_BIT_OS)

ExternalProject_Add(
  build_${PACKAGE}
  DEPENDS ${dependencies_targets}
  URL ${URL}
  URL_HASH ${DOWNLOAD_HASH_TYPE}=${DOWNLOAD_HASH}
  # Order of CPPFLAGS is important since want to access the version 3
  # itcl header.
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} "CFLAGS=${CFLAGS}" "CPPFLAGS=-I${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION} -I${EPA_CMAKE_INSTALL_PREFIX}/include" ${source_PATH}/configure --prefix=${INCRTCL3_PREFIX} --exec-prefix=${INCRTCL3_PREFIX} --includedir=${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION} --mandir=${INCRTCL3_PREFIX}/share/man ${ENABLE_64_BIT} --with-tcl=${EPA_CMAKE_INSTALL_PREFIX}/lib --with-tk=${EPA_CMAKE_INSTALL_PREFIX}/lib --with-itcl=${INCRTCL3_PREFIX}/lib/itcl${ITCL3_LIBVERSION}
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
  COMMAND ${CHMOD_EXECUTABLE} -v ${SO_NUMERICAL_PERMISSIONS} ${INCRTCL3_PREFIX}/lib/itk${LIBVERSION}/libitk${LIBVERSION}.so
  APPEND
  )
