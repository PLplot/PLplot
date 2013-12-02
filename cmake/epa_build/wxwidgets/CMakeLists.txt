# wxwidgets/CMakeLists.txt
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build of wxwidgets.

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

set(PACKAGE wxwidgets)

# Protect against configuring a build twice in one CMake call
if(configured)
  return()
endif(configured)
set(configured ON)

# Data that is related to downloads.
set(URL http://prdownloads.sourceforge.net/wxwindows/wxWidgets-2.8.12.tar.gz)
set(URL_MD5 2fa39da14bc06ea86fe902579fedc5b1)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(EPA_PATH_NODLL "${EPA_PATH}")
  #set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  # This flag is still necessary to keep ld step from exhausting
  # memory with MinGW.  See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=43601
  # for discussion of this.  Apparently the issue was fixed for
  # MinGW-4.6, but it appears to be back for 4.7.2.
  set(${PACKAGE}_SET_CXXFLAGS "CXXFLAGS=-fno-keep-inline-dllexport $ENV{CXXFLAGS}")
else(MSYS_PLATFORM)
  set(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  set(${PACKAGE}_SET_CXXFLAGS "CXXFLAGS=$ENV{CXXFLAGS}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

set(${PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")

# Drop -fvisibility=hidden since that option does not work for this package.
string(REGEX REPLACE "-fvisibility=hidden" "" ${PACKAGE}_SET_CFLAGS "${${PACKAGE}_SET_CFLAGS}")
string(REGEX REPLACE "-fvisibility=hidden" "" ${PACKAGE}_SET_CXXFLAGS "${${PACKAGE}_SET_CXXFLAGS}")

ExternalProject_Add(
  build_${PACKAGE}
  URL ${URL}
  URL_MD5 ${URL_MD5}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${${PACKAGE}_SET_CFLAGS} ${${PACKAGE}_SET_CXXFLAGS} ${source_PATH}/${EPA_CONFIGURE_COMMAND} --enable-shared --enable-unicode --enable-debug --enable-debug_gdb
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install
  )

list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
