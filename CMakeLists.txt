# Top-level CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Alan W. Irwin
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

# Version 2.4.3 or above of cmake is required!
CMAKE_MINIMUM_REQUIRED(VERSION 2.4.3 FATAL_ERROR)

# Recommended way to do user overrides of C-related variables, is given at
# http://public.kitware.com/pipermail/cmake/2006-July/010334.html.
# See also, CMakeCInformation.cmake.  There are also C++, and Fortran-specific
# overrides that can be done.  However, I prefer putting overrides in just
# one file so I use the overall variable CMAKE_USER_MAKE_RULES_OVERRIDE
# instead.
set(
CMAKE_USER_MAKE_RULES_OVERRIDE
${CMAKE_SOURCE_DIR}/cmake/UserOverride.cmake
)

project(plplot)

# 2.4.2 bug workarounds here.

message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
 SET(CMAKE_SHARED_LIBRARY_SONAME_Fortran_FLAG "-Wl,-soname,")
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

# End of 2.4.2 bug workarounds.

# Start of version area that needs modification for each release.
set(VERSION 5.6.1)

# PLplot library version information.

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

set(csirocsa_SOVERSION 0)
set(csirocsa_VERSION ${csirocsa_SOVERSION}.0.1)

set(csironn_SOVERSION 0)
set(csironn_VERSION ${csironn_SOVERSION}.0.1)

set(plplot_SOVERSION 9)
set(plplot_VERSION ${plplot_SOVERSION}.2.0)

set(plplotcxx_SOVERSION 9)
set(plplotcxx_VERSION ${plplotcxx_SOVERSION}.2.0)

set(plplotf77_SOVERSION 9)
set(plplotf77_VERSION ${plplotf77_SOVERSION}.1.0)

set(plplotf95_SOVERSION 9)
set(plplotf95_VERSION ${plplotf95_SOVERSION}.1.0)

set(plplotgnome2_SOVERSION 0)
set(plplotgnome2_VERSION ${plplotgnome2_SOVERSION}.0.0)

# End of version area that needs modification for each release.

# parameters to control overall cmake behaviour.

# libraries are all shared for now.
set(BUILD_SHARED_LIBS ON)

set(PACKAGE plplot)

# Location where PLplot cmake build system first looks for cmake modules.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# Configure PLplot component variables....

include(plplot)

# Use configured variables to process configurable top-level files.

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake 
${CMAKE_CURRENT_BINARY_DIR}/config.h
)
# Allow access to the generated config.h for this build.
ADD_DEFINITIONS("-DHAVE_CONFIG_H")
# Install top-level files

# Enable testing framework for examples
if (BUILD_TEST)
  enable_testing()
endif (BUILD_TEST)



set(top_level_DOCFILES
AUTHORS
COPYING.LIB
ChangeLog
Copyright
FAQ
NEWS
PROBLEMS
README
README.release
)
install(FILES ${top_level_DOCFILES} DESTINATION ${DOC_INSTALL_DIR})

# Process other directories including using configured variables to 
# process configurable files in those directories.

# needs no cmake processing
#add_subdirectory(fonts)
add_subdirectory(lib)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(bindings)
add_subdirectory(drivers)
add_subdirectory(examples)
####not yet implemented
#add_subdirectory(utils)
add_subdirectory(test)
####not yet implemented
#add_subdirectory(scripts)
####not yet implemented
add_subdirectory(doc)
# needs no cmake processing
#add_subdirectory(pkgcfg)
# needs no cmake processing
#add_subdirectory(cf)

#
# Packing stuff
#
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Scientific Plotting Library PLplot")
set(CPACK_PACKAGE_VENDOR "PLplot development team")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README)
set(CPACK_PACKAGE_VERSION_MAJOR 5)
set(CPACK_PACKAGE_VERSION_MINOR 6)
set(CPACK_PACKAGE_VERSION_PATCH 1)
set(CPACK_GENERATOR TGZ)
set(CPACK_SOURCE_GENERATOR TGZ)
# The following components are regex's to match anywhere (unless anchored)
# in absolute patch + filename to find files or directories to be excluded
# from source tarball.
set(CPACK_SOURCE_IGNORE_FILES
"Makefile\\\\.in$"
"~$"
"^${PROJECT_SOURCE_DIR}.*/CVS/"
"^${PROJECT_SOURCE_DIR}/bindings/"
"^${PROJECT_SOURCE_DIR}/cf/"
"^${PROJECT_SOURCE_DIR}/debian/"
"^${PROJECT_SOURCE_DIR}/doc/"
"^${PROJECT_SOURCE_DIR}/drivers/"
"^${PROJECT_SOURCE_DIR}/examples/"
"^${PROJECT_SOURCE_DIR}/fonts/"
"^${PROJECT_SOURCE_DIR}/libltdl/"
"^${PROJECT_SOURCE_DIR}/old/"
"^${PROJECT_SOURCE_DIR}/pkgcfg/"
"^${PROJECT_SOURCE_DIR}/rpm/"
"^${PROJECT_SOURCE_DIR}/scripts/"
"^${PROJECT_SOURCE_DIR}/sys/"
"^${PROJECT_SOURCE_DIR}/test/"
"^${PROJECT_SOURCE_DIR}/utils/"
)
message("CPACK_SOURCE_IGNORE_FILES = ${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)
