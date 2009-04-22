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

message(STATUS "CMake version = ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}")
if(MSYS)
  # Version 2.6.3 or above of cmake is required for MSYS
  CMAKE_MINIMUM_REQUIRED(VERSION 2.6.3 FATAL_ERROR)
else(MSYS)
  # Version 2.6.0 or above of cmake is required for all other platforms
  CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0 FATAL_ERROR)
endif(MSYS)

#foreach(policy RANGE 0 9)
#  cmake_policy(GET CMP000${policy} policy_result)
#  message(STATUS "Policy CMP000${policy} is ${policy_result}")
#endforeach(policy RANGE 0 9)

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

set(PACKAGE plplot)

# Location where PLplot cmake build system first looks for cmake modules.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# We need the path to the MinGW/Borland compiler in order to find
# the import libraries for system libraries.
IF(MINGW)
	get_filename_component(MINGWBINPATH ${CMAKE_C_COMPILER} PATH)
	set(MINGWLIBPATH ${MINGWBINPATH}/../lib
			CACHE FILEPATH
			DOCSTRING "Path to MinGW import libraries")
ENDIF(MINGW)
IF(BORLAND)
	get_filename_component(BORLANDBINPATH ${CMAKE_C_COMPILER} PATH)
	set(BORLANDLIBPATH ${BORLANDBINPATH}/../Lib/PSDK
			CACHE FILEPATH
			DOCSTRING "Path to Borland import libraries")
ENDIF(BORLAND)

# Version data that need modification for each release.
include(plplot_version)
# parameters to control overall cmake behaviour.

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
  include(CTest)
endif (BUILD_TEST)

# Disable warnings about deprecated functions (Visual C++ 2005)
if(MSVC_VERSION GREATER 1399)
  ADD_DEFINITIONS("/D_CRT_SECURE_NO_DEPRECATE")
endif(MSVC_VERSION GREATER 1399)

# in windows all created dlls are gathered in the dll directory
# if you add this directory to your PATH all shared libraries are available
if(BUILD_SHARED_LIBS AND WIN32 AND NOT CYGWIN)
  SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/dll)
endif(BUILD_SHARED_LIBS AND WIN32 AND NOT CYGWIN)

# Borland Compiler must compile in ANSII mode
if(BORLAND)
  ADD_DEFINITIONS(-A)
endif(BORLAND)

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
README.testing
)
install(FILES ${top_level_DOCFILES} DESTINATION ${DOC_DIR})

# Process other directories including using configured variables to
# process configurable files in those directories.
# N.B. Order is important here at the cmake stage because targets must
# be processed by cmake in a specific order e.g., due to get_target_property
# invocations.  Note, this order has nothing to do with the order which make
# processes these subdirectories at build time.  That build-time order
# is determined by the dependencies between targets and also by file
# dependencies that are established by the cmake configuration files.

add_subdirectory(fonts)
add_subdirectory(lib)
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(data)
add_subdirectory(bindings)
add_subdirectory(drivers)
# Must be processed after drivers since depends on qt target created by
# drivers.
add_subdirectory(bindings/qt_gui)
add_subdirectory(examples)
add_subdirectory(utils)
add_subdirectory(plplot_test)
add_subdirectory(scripts)
add_subdirectory(doc)
add_subdirectory(www)
add_subdirectory(pkgcfg)
summary()

if(PREBUILD_DIST)
  # Pre-build everything required for a distribution tarball and copy it to
  # the source tree (if build tree is different from source tree).

  # List of targets that must be (pre-)built.
  # N.B. the plhershey-unicode.h_built target has to be pre-built only because
  # of the needs of the old windows build at sys/win32/msdev.
  set(
  DIST_TARGETS
  plhershey-unicode.h_built
  )

  if(ENABLE_octave)
    set(DIST_TARGETS ${DIST_TARGETS} make_documentation)
  endif(ENABLE_octave)

  if(BUILD_PRINT)
    set(DIST_TARGETS ${DIST_TARGETS} print)
  endif(BUILD_PRINT)

  if(BUILD_INFO)
    set(DIST_TARGETS ${DIST_TARGETS} info)
  endif(BUILD_INFO)

  if(BUILD_MAN)
    set(DIST_TARGETS ${DIST_TARGETS} man)
  endif(BUILD_MAN)

  if(BUILD_HTML)
    set(DIST_TARGETS ${DIST_TARGETS} html)
  endif(BUILD_HTML)

  if(CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
    add_custom_target(prebuild_dist)
  else(CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
    # copy prebuilds back to source tree.
    add_custom_target(
    prebuild_dist
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_BINARY_DIR}/include/plhershey-unicode.h
    ${CMAKE_SOURCE_DIR}/include
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_BINARY_DIR}/bindings/octave/plplot_octave_txt
    ${CMAKE_SOURCE_DIR}/bindings/octave/plplot_octave_txt
    COMMAND cp
    `cat static_built_files ${INFO_MANIFEST} ${MAN_MANIFEST} ${HTML_MANIFEST}`
    ${CMAKE_SOURCE_DIR}/doc/docbook/src
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/doc/docbook/src
    )
  endif(CMAKE_BINARY_DIR STREQUAL "${CMAKE_SOURCE_DIR}")
  add_dependencies(prebuild_dist ${DIST_TARGETS})
endif(PREBUILD_DIST)

#
# Packing stuff
#
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Scientific Plotting Library PLplot")
set(CPACK_PACKAGE_VENDOR "PLplot development team")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README)
if(WIN32)
  set(CPACK_GENERATOR ZIP)
else(WIN32)
  set(CPACK_GENERATOR TGZ)
endif(WIN32)
set(
CPACK_SOURCE_PACKAGE_FILE_NAME
"plplot-${VERSION}"
CACHE INTERNAL "tarball basename"
)
set(CPACK_SOURCE_GENERATOR TGZ)
# The following components are regex's to match anywhere (unless anchored)
# in absolute path + filename to find files or directories to be excluded
# from source tarball.
set(CPACK_SOURCE_IGNORE_FILES
"~$"
"\\\\.cvsignore$"
"^${PROJECT_SOURCE_DIR}.*/\\\\.svn/"
"^${PROJECT_SOURCE_DIR}/old/"
"^${PROJECT_SOURCE_DIR}/sys/mac"
"^${PROJECT_SOURCE_DIR}/sys/os2"
"^${PROJECT_SOURCE_DIR}/sys/unix"
"^${PROJECT_SOURCE_DIR}/sys/dos/msc"
"^${PROJECT_SOURCE_DIR}/sys/dos/bcc"
"^${PROJECT_SOURCE_DIR}/sys/dos/emx"
"^${PROJECT_SOURCE_DIR}/sys/win-tk"
"^${PROJECT_SOURCE_DIR}/sys/win32"
)
#message("CPACK_SOURCE_IGNORE_FILES = ${CPACK_SOURCE_IGNORE_FILES}")
include(CPack)

# Path to native build for executables required in the build process.
# This is only required for cross compiling
if (CMAKE_CROSSCOMPILING)
  set(CMAKE_NATIVE_BINARY_DIR NATIVEDIR-NOTFOUND CACHE FILEPATH "Point to the native build directory")
endif (CMAKE_CROSSCOMPILING)

