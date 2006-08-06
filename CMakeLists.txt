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

# 2.4.3 bug workarounds here.

message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
 SET(CMAKE_SHARED_LIBRARY_SONAME_Fortran_FLAG "-Wl,-soname,")
endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  if(NOT XCODE)
    SET(CMAKE_SHARED_LIBRARY_SONAME_Fortran_FLAG "-install_name")
  endif(NOT XCODE)
  set(CMAKE_FORTRAN_CREATE_SHARED_LIBRARY "<CMAKE_FORTRAN_COMPILER> <LANGUAGE_COMPILE_FLAGS> <CMAKE_SHARED_LIBRARY_CREATE_FORTRAN_FLAGS> <LINK_FLAGS> -o <TARGET> -install_name <TARGET_INSTALLNAME_DIR><TARGET_SONAME> <OBJECTS> <LINK_LIBRARIES>")
endif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

# End of 2.4.3 bug workarounds.

# Location where PLplot cmake build system first looks for cmake modules.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# Version data that need modification for each release.
include(plplot_version)
# parameters to control overall cmake behaviour.

# libraries are all shared for now.
set(BUILD_SHARED_LIBS ON)

set(PACKAGE plplot)

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
add_subdirectory(utils)
add_subdirectory(test)
add_subdirectory(scripts)
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
