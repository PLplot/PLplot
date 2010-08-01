# CMakeLists.txt
#
# Copyright (C) 2008  Werner Smekal
# Copyright (C) 2010  Alan W. Irwin
#
# Process this file with cmake to produce Makefiles or project files
# for your specific compiler tool set
#
# TODO:
# - shared and static library
# - packaging
# - devpackage
# =======================================================================
# libharu project
# =======================================================================
project(libharu C)

# we want cmake version 2.4.8 at least
cmake_minimum_required(VERSION 2.4.8 FATAL_ERROR)

# Location where the haru cmake build system first looks for cmake modules
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

# determine compiler name
set(COMPILER_LABEL "unknown")
if(CMAKE_COMPILER_IS_GNUCC)
  set(COMPILER_LABEL "gcc")
endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC)
  set(COMPILER_LABEL "vc")
endif(MSVC)

### Version information for libharu software package.  
### N.B. Needs review before each release!
set(LIBHARU_MAJOR 2)
set(LIBHARU_MINOR 2)
set(LIBHARU_PATCH 0)
set(LIBHARU_VERSION ${LIBHARU_MAJOR}.${LIBHARU_MINOR}.${LIBHARU_PATCH})
# libhpdf library version information.
# N.B. Not to be confused with package version set above!

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
### End of version information to be reviewed.

# Use RPATH?
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # No rpath on Darwin. Setting it will only cause trouble.
else(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  option(USE_RPATH "Use -rpath when linking libraries, executables" ON)
endif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

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

# Description of libharu for cpack.
set(LIBHARU_DESCRIPTION "libHaru is a free, cross platform, open source library for generating PDF files.")
set(LIBHARU_PACKAGE_NAME "libHaru-${LIBHARU_VERSION}-${COMPILER_LABEL}")

# Comment out all this name stuff which I feel is probably unnecessary (since
# PLplot doesn't bother with this.)
# set library name, msvc does not prepend 'lib' automatically
#if(MSVC)
#  set(LIBHARU_NAME lib)
#endif(MSVC)
#set(LIBHARU_NAME ${LIBHARU_NAME}hpdf)
#set(LIBHARU_NAME_STATIC ${LIBHARU_NAME}s)

# =======================================================================
# command line options
# =======================================================================
option(LIBHARU_SHARED "Build shared lib" YES)
option(LIBHARU_STATIC "Build static lib" YES)
option(LIBHARU_EXAMPLES "Build libharu examples" NO)
option(DEVPAK "Create DevPackage" NO)

if(DEVPAK AND NOT WIN32)
  message( STATUS "DevPackage only available for Win32. Set DEVPAK to OFF." )
  set(DEVPAK OFF)
endif(DEVPAK AND NOT WIN32)
if(DEVPAK AND LIBHARU_EXAMPLES)
  message( STATUS "Examples are not build for DevPackage. Set LIBHARU_EXAMPLES to OFF." )
  set(LIBHARU_EXAMPLES OFF)
endif(DEVPAK AND LIBHARU_EXAMPLES)



if(BUILD_SHARED_LIBS)
  set(LIBHARU_SHARED ON)
endif(BUILD_SHARED_LIBS)

# =======================================================================
# look for headers and libraries
# =======================================================================
include(haru)
include(summary)

# check zlib availibility
find_package(ZLIB)
if(ZLIB_FOUND)
  set(HAVE_LIBZ ON)
  include_directories(${ZLIB_INCLUDE_DIR})
  set(ADDITIONAL_LIBRARIES ${ZLIB_LIBRARIES})
else(ZLIB_FOUND)
  set(HPDF_NOZLIB ON)  
endif(ZLIB_FOUND)

# check png availibility
find_package(PNG)
if(PNG_FOUND)
  set(HAVE_LIBPNG ON)
  include_directories(${PNG_INCLUDE_DIR})
  add_definitions(${PNG_DEFINITIONS})
  set(ADDITIONAL_LIBRARIES ${ADDITIONAL_LIBRARIES} ${PNG_LIBRARIES})
else(PNG_FOUND)
  set(HPDF_NOPNGLIB ON)  
endif(PNG_FOUND)

# =======================================================================
# Add compiler flags
# =======================================================================
# add definitions and directories to include
#if(CMAKE_COMPILER_IS_GNUCC)
#  add_definitions("-Wall")
#endif(CMAKE_COMPILER_IS_GNUCC)
if(MSVC_VERSION GREATER 1399)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE)
endif(MSVC_VERSION GREATER 1399)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)

# create DevPackage file
if(DEVPAK)
  configure_file(
    ${CMAKE_SOURCE_DIR}/libharu.DevPackage.cmake
    ${CMAKE_BINARY_DIR}/libharu.DevPackage
    )
endif(DEVPAK)

# =======================================================================
# Configure and install header files.
# =======================================================================
add_subdirectory(include)

# =======================================================================
# create library and demos
# =======================================================================
add_subdirectory(src)
add_subdirectory(demo)

# install various files
# These commented out to keep install tree as lean as possible.  May want
# to add some documentation files later to the install tree.
#install(FILES README CHANGES INSTALL DESTINATION .)
#install(DIRECTORY doc DESTINATION .)
#if(NOT DEVPAK)
#  install(DIRECTORY if DESTINATION .)
#endif(NOT DEVPAK)
#if(DEVPAK)
#  install(FILES ${CMAKE_BINARY_DIR}/libharu.DevPackage DESTINATION .)
#endif(DEVPAK)
# =======================================================================
# print out some information
# =======================================================================
summary()

# =======================================================================
# packing stuff
# =======================================================================
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${LIBHARU_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "Werner Smekal")
set(CPACK_PACKAGE_DESCRIPTION_FILE ${CMAKE_CURRENT_SOURCE_DIR}/README)
set(CPACK_PACKAGE_VERSION_MAJOR "${LIBHARU_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${LIBHARU_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${LIBHARU_PATCH}")
set(CPACK_PACKAGE_FILE_NAME "libHaru-${LIBHARU_VERSION}-${COMPILER_LABEL}")

set(CPACK_STRIP_FILES ON)

if(WIN32)
  set(CPACK_GENERATOR ZIP)
else(WIN32)
  set(CPACK_GENERATOR TGZ)
endif(WIN32)

INCLUDE( CPack )
