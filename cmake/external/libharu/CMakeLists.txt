# Top-level CMakeLists.txt for libharu.
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2010 Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

project(libharu C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.2 FATAL_ERROR)

# Version information for libharu software package.  
# Needs review before each release. 

set(VERSION 2.1.0)

# CPack version numbers for release tarball name.
set(CPACK_PACKAGE_VERSION_MAJOR 2)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_VERSION_PATCH 0)

# libhpdf library version information.

# Rules: 
# (1) If a backwards incompatible API change has been made in the library
#     API (e.g., if old compiled and linked applications will no longer work)
#     then increment SOVERSION and zero the corresponding minor and patch
#     numbers just before release.
# (2) If the library changes are limited to additions to the API, then
#     then leave SOVERSION alone, increment the minor number and zero the
#     patch number just before release.
# (3) If the library changes are limited to implementation changes with 
#     no API changes at all, then leave SOVERSION and minor number alone, and
#     increment the patch number just before the release.
# (4) If there are no library source code changes at all, then leave all
#     library version numbers the same for the release.
# If library has not been properly versioned before start the numbering
# at 0.0.0.
set(hpdf_SOVERSION 0)
set(hpdf_VERSION ${hpdf_SOVERSION}.0.0)

# Set macros to define visibility for various Windows platforms.
# (gcc visibility support happens by default if none of these
# are defined).
# N.B. something slightly different must be done for the demo
# visibility, but at this stage this CMake-based build system ignores
# building the demos so don't worry about this issue for now.

if(WIN32)
  if(CYGWIN)
    # Special Cygwin visibility support.
    add_definitions(-DHPDF_DLL_MAKE_CDECL)
  else(CYGWIN)
    # proprietary and Mingw compilers for Windows without Cygwin.
    add_definitions(-DHPDF_DLL_MAKE)
  endif(CYGWIN)
endif(WIN32)

# Find zlib and png libraries which libhpdf depends upon.
find_package(ZLIB)
if(ZLIB_FOUND)
  set(HPDF_NOZLIB OFF)
else(ZLIB_FOUND)
  set(HPDF_NOZLIB ON)
endif(ZLIB_FOUND)

find_package(PNG)
if(PNG_FOUND)
  set(HPDF_NOPNGLIB OFF)
else(PNG_FOUND)
  set(HPDF_NOPNGLIB ON)
endif(PNG_FOUND)

# libraries built shared?
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Use RPATH?
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # No rpath on Darwin. Setting it will only cause trouble.
else(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  option(USE_RPATH "Use -rpath when linking libraries, executables" ON)
endif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

# Set installation directories.
set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set(INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/include/${PACKAGE})

# Configure and install headers.
add_subdirectory(include)

# Build and install library.
add_subdirectory(src)
