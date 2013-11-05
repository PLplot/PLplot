# iwidgets4.0/bp.cmake
# CMakeLists.txt file to configure the build of iwidgets4.0.

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

# Protect against configuring a build twice in one CMake call
if(iwidgets4.0_configured)
  return()
endif(iwidgets4.0_configured)
set(iwidgets4.0_configured ON)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(BP_ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(iwidgets4.0_dependencies_LIST itk3 itcl3 tk tcl)
# Remove dependencies that should be ignored.
if(iwidgets4.0_dependencies_LIST)
  list(REMOVE_ITEM iwidgets4.0_dependencies_LIST ${BP_ignored_dependencies_LIST})
endif(iwidgets4.0_dependencies_LIST)

set(iwidgets4.0_dependencies_targets)
foreach(build_configuration ${iwidgets4.0_dependencies_LIST})
  if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
    include(${build_configuration}/bp.cmake)
    list(APPEND iwidgets4.0_dependencies_targets build_${build_configuration})
  else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
    message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of iwidgets4.0 has been installed another way.")
  endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
endforeach(build_configuration ${iwidgets4.0_dependences_LIST})

# This can be safely done only after above includes.
set(BP_PACKAGE iwidgets4.0)

# Data that is related to downloads.
set(IWIDGETS4.0_VERSION 4.0.1)
set(IWIDGETS4.0_LIBVERSION 4.0)
set(${BP_PACKAGE}_URL http://downloads.sourceforge.net/project/incrtcl/%5bIncr%20Widgets%5d/${IWIDGETS4.0_VERSION}/iwidgets${IWIDGETS4.0_VERSION}.tar.gz)
set(${BP_PACKAGE}_DOWNLOAD_HASH_TYPE MD5)
set(${BP_PACKAGE}_DOWNLOAD_HASH 0e9c140e81ea6015b56130127c7deb03)

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

# Eliminate this option which gives bad results for this package.
string(REGEX REPLACE "-fvisibility=hidden" "" ${BP_PACKAGE}_SET_CFLAGS "${${BP_PACKAGE}_SET_CFLAGS}")

ExternalProject_Add(
  build_${BP_PACKAGE}
  DEPENDS ${${BP_PACKAGE}_dependencies_targets}
  URL ${${BP_PACKAGE}_URL}
  URL_HASH ${${BP_PACKAGE}_DOWNLOAD_HASH_TYPE}=${${BP_PACKAGE}_DOWNLOAD_HASH}
  # Because of severe Itcl/Itk bit rot since 2002 the iwidgets4.0 build system
  # is substantially broken so must patch it.  This patch taken from
  # the Debian build of iwidgets4.0.
  PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 < ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/${BP_PACKAGE}_build_system.patch
  # Order of CPPFLAGS is important since want to access the version 3
  # itcl header.
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${${BP_PACKAGE}_SET_CFLAGS} "CPPFLAGS=-I${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION} -I${BP_CMAKE_INSTALL_PREFIX}/include" ${source_PATH}/configure --prefix=${INCRTCL3_PREFIX} --exec-prefix=${INCRTCL3_PREFIX} --includedir=${INCRTCL3_PREFIX}/include/itcl${ITCL3_LIBVERSION} --mandir=${INCRTCL3_PREFIX}/share/man --with-tcl=${BP_CMAKE_INSTALL_PREFIX}/lib --with-tk=${BP_CMAKE_INSTALL_PREFIX}/lib
  # Possible make race conditions so use non-parallel make
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_MAKE_COMMAND} install
  )

add_custom_command(
  OUTPUT
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-patch
  COMMAND echo "Permissions fixup after patch"
  COMMAND ${CHMOD_EXECUTABLE} -v ugo+x ${EP_BASE}/Source/build_${BP_PACKAGE}/mkinstalldirs
  APPEND
  )

if(0)
add_custom_command(
  OUTPUT
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-build
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
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-install
  COMMAND echo "Install-tree fixups"
  COMMAND ${CHMOD_EXECUTABLE} -v ${SO_NUMERICAL_PERMISSIONS} ${INCRTCL3_PREFIX}/lib/itk${ITK3_LIBVERSION}/libitk${ITK3_LIBVERSION}.so
  APPEND
  )
endif(0)


list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
