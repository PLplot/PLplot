# wxwidgets/bp.cmake
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
# have run "include(ExternalProject)" and set EP_BASE variable (and
# corresponding directory property) as well as various BP variables
# used below that configure how the External_Project functions
# operate.

set(BP_PACKAGE wxwidgets)

# Protect against configuring a build twice in one CMake call
if(${BP_PACKAGE}_configured)
  return()
endif(${BP_PACKAGE}_configured)
set(${BP_PACKAGE}_configured ON)

# Data that is related to downloads.
set(${BP_PACKAGE}_URL http://prdownloads.sourceforge.net/wxwindows/wxWidgets-2.8.12.tar.gz)
set(${BP_PACKAGE}_URL_MD5 2fa39da14bc06ea86fe902579fedc5b1)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(BP_PATH_NODLL "${BP_PATH}")
  #set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
  # This flag is still necessary to keep ld step from exhausting
  # memory with MinGW.  See http://gcc.gnu.org/bugzilla/show_bug.cgi?id=43601
  # for discussion of this.  Apparently the issue was fixed for
  # MinGW-4.6, but it appears to be back for 4.7.2.
  set(${BP_PACKAGE}_SET_CXXFLAGS "CXXFLAGS=-fno-keep-inline-dllexport")
else(MSYS_PLATFORM)
  set(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
  set(${BP_PACKAGE}_SET_CXXFLAGS)
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")

# Got intermittent failures using ${BP_PARALLEL_MAKE} both
# historically and recently which are likely due to a parallel make
# race condition for wxwidgets.  So I have decided to use
# slow but sure ${BP_MAKE_COMMAND} instead.

ExternalProject_Add(
  build_${BP_PACKAGE}
  URL ${${BP_PACKAGE}_URL}
  URL_MD5 ${${BP_PACKAGE}_URL_MD5}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${${BP_PACKAGE}_SET_CXXFLAGS} ${source_PATH}/${BP_CONFIGURE_COMMAND} --enable-shared --enable-unicode --enable-debug --enable-debug_gdb
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_MAKE_COMMAND} install
  )

list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
