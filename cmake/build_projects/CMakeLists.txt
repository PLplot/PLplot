# Top-level CMakeLists.txt file for a project that simply builds
# and installs external projects using epa.cmake include fragments
# representing the build and test instructions for several different projects.

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

project(build_projects NONE)

message(STATUS "CMake version = ${CMAKE_VERSION}")
message(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

cmake_minimum_required(VERSION 2.8.11.2 FATAL_ERROR)

set(EP_BASE ${CMAKE_BINARY_DIR}/build_projects)
message(STATUS "EP_BASE = ${EP_BASE}")
set_directory_properties(PROPERTIES EP_BASE ${EP_BASE})

# Set build_projects variables to control builds in general

# Test whether there is access to the MSYS platform on the PATH.  Note
# this is a different question than whether you are using the "MSYS
# Makefiles" generator since some other generators (e.g., "MinGW
# Makefiles") can have MSYS on the path so long as sh.exe is renamed
# or removed from that PATH.  Note, we search for the automatic
# installer app, mingw-get.exe and the bash.exe shell to check
# (approximately) whether we have access to the MSYS platform, and
# then we test that platform (very approximately) for completeness by
# making sure unzip (necessary for some build configurations to unpack
# their packages) is installed (using mingw-get).

find_program(MINGW_GET_EXECUTABLE mingw-get)
find_program(BASH_EXECUTABLE bash)
if(MINGW_GET_EXECUTABLE AND BASH_EXECUTABLE)
  set(MSYS_PLATFORM ON)
else(MINGW_GET_EXECUTABLE AND BASH_EXECUTABLE)
  set(MSYS_PLATFORM OFF)
endif(MINGW_GET_EXECUTABLE AND BASH_EXECUTABLE)
message(STATUS "WIN32 = ${WIN32}")
message(STATUS "MINGW = ${MINGW}")
message(STATUS "MSYS = ${MSYS}")
message(STATUS "MSYS_PLATFORM = ${MSYS_PLATFORM}")
message(STATUS "CYGWIN = ${CYGWIN}")

if(CYGWIN)
  message(FATAL_ERROR "build_projects does not work correctly on the Cygwin platform.  Use the MinGW compiler and MSYS platform instead.")
endif(CYGWIN)

if(WIN32 AND NOT MSYS_PLATFORM)
  message(FATAL_ERROR "build_projects does not work correctly on Windows without the MSYS platform.  Install the MSYS platform (perhaps without sh.exe depending on CMake generator) on Windows.")
endif(WIN32 AND NOT MSYS_PLATFORM)

# List of executables required by build_projects CMake logic.
set(executables_LIST
  # ExternalProject workarounds
  touch
  # Unpacking
  unzip
  # Updating
  mkdir
  cp
  patch
  # General
  env
  # Not specifically required by build_projects (yet), but some
  # individual project configurations will not work unless this is
  # available so might as well check it here.
  bash
  make
  python
  uname
  sed
  ln
  chmod
  )

foreach(executable ${executables_LIST})
  string(TOUPPER ${executable} EXECUTABLE)
  find_program(${EXECUTABLE}_EXECUTABLE ${executable})
  if(NOT ${EXECUTABLE}_EXECUTABLE)
    if(MSYS_PLATFORM)
      message(STATUS "${executable} missing from your MSYS platform.  Use mingw-get to install it.")
    endif(MSYS_PLATFORM)
    message(FATAL_ERROR "${executable} must be on your PATH in order for build_projects to work correctly")  
  endif(NOT ${EXECUTABLE}_EXECUTABLE)
endforeach(executable ${executables_LIST})

# Get the Python version.
execute_process(
  COMMAND
  ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print(sysconfig.get_python_version())"
  OUTPUT_VARIABLE PYTHON_VERSION
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
message(STATUS "PYTHON_VERSION = ${PYTHON_VERSION}")

# Determine whether OS is 64-bits from uname -m "machine name" field.

execute_process(
  COMMAND
  ${UNAME_EXECUTABLE} -m
  OUTPUT_VARIABLE UNAME_MACHINE
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )

# Determine desired install permissions on shared objects.
if(CMAKE_INSTALL_SO_NO_EXE)
  set(SO_NUMERICAL_PERMISSIONS 644)
else(CMAKE_INSTALL_SO_NO_EXE)
  set(SO_NUMERICAL_PERMISSIONS 755)
endif(CMAKE_INSTALL_SO_NO_EXE)

# This will need refinement as more platforms tested, but it works on
# Intel/AMD Linux hardware and also MinGW/MSYS on 32-bit Windows for
# now.
if(UNAME_MACHINE STREQUAL x86_64)
  set(BP_HAVE_64_BIT_OS ON)
else(UNAME_MACHINE STREQUAL x86_64)
  set(BP_HAVE_64_BIT_OS OFF)
endif(UNAME_MACHINE STREQUAL x86_64)

# Use modified version of the CMake 2.8.12 ExternalProject module
# where the tar.xz processing has been fixed.
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})
include(ExternalProject)

if(MSYS_PLATFORM)
  # Useful function to convert Windows list of semicolon-delimited
  # PATHs to the equivalent list of MSYS PATHs (exactly like the
  # colon-delimited Unix list of PATHs except the driver letters are
  # specified as the initial one-character component of each of the
  # PATHs).  For example, this function will transform the Windows
  # list of PATHs, "z:\path1;c:\path2" to "/z/path1:/c/path2".
  function(determine_msys_path MSYS_PATH NATIVE_PATH)
    #message(STATUS "NATIVE_PATH = ${NATIVE_PATH}")
    string(REGEX REPLACE "^\([a-zA-z]\):" "/\\1" PATH  "${NATIVE_PATH}")
    string(REGEX REPLACE ";\([a-zA-z]\):" ";/\\1" PATH  "${PATH}")
    string(REGEX REPLACE ";" ":" PATH  "${PATH}")
    file(TO_CMAKE_PATH "${PATH}" PATH)
    #message(STATUS "MSYS_PATH = ${PATH}")
    set(${MSYS_PATH} ${PATH} PARENT_SCOPE)
  endfunction(determine_msys_path)
endif(MSYS_PLATFORM)

# This gives the full pathname of the associated build tool for at
# least the "Unix Makefiles", "Ninja", "MSYS Makefiles", "MinGW
# Makefiles", and "NMake Makefiles JOM" CMake generators.
set(BP_BUILD_COMMAND "${CMAKE_MAKE_PROGRAM}")

# Actual make programme used for autotools builds.
set(BP_MAKE_COMMAND ${MAKE_EXECUTABLE})

set(BP_CTEST_COMMAND "${CMAKE_CTEST_COMMAND}")

if(MSYS_PLATFORM)
  # On the MSYS platform, the env command (used to set relevant
  # environment variables for the commands below in the */bp.cmake
  # scripts) requires all full paths be in the MSYS platform form.
  determine_msys_path(BP_BUILD_COMMAND "${BP_BUILD_COMMAND}")
  determine_msys_path(BP_MAKE_COMMAND "${BP_MAKE_COMMAND}")
  determine_msys_path(BP_CTEST_COMMAND "${BP_CTEST_COMMAND}")
  determine_msys_path(BP_CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
  determine_msys_path(MSYS_CMAKE_COMMAND "${CMAKE_COMMAND}")
  # Propagate the overall CMake generator and install prefix to all CMake-based
  # software project builds.
  set(BP_CMAKE_COMMAND ${MSYS_CMAKE_COMMAND} "-G${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX:PATH=${BP_CMAKE_INSTALL_PREFIX})
  # Propagate CMAKE_C_COMPILER, CMAKE_CXX_COMPILER, and
  # CMAKE_RC_COMPILER to all CMake-based software project builds.
  # (jom currently requires this as a workaround).
  if(CMAKE_C_COMPILER)
    determine_msys_path(MSYS_CMAKE_C_COMPILER "${CMAKE_C_COMPILER}")
    list(APPEND BP_CMAKE_COMMAND -DCMAKE_C_COMPILER:FILEPATH=${MSYS_CMAKE_C_COMPILER})
  endif(CMAKE_C_COMPILER)
  if(CMAKE_CXX_COMPILER)
    determine_msys_path(MSYS_CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}")
    list(APPEND BP_CMAKE_COMMAND -DCMAKE_CXX_COMPILER:FILEPATH=${MSYS_CMAKE_CXX_COMPILER})
  endif(CMAKE_CXX_COMPILER)
  if(CMAKE_RC_COMPILER)
    determine_msys_path(MSYS_CMAKE_RC_COMPILER "${CMAKE_RC_COMPILER}")
    list(APPEND BP_CMAKE_COMMAND -DCMAKE_RC_COMPILER:FILEPATH=${MSYS_CMAKE_RC_COMPILER})
  endif(CMAKE_RC_COMPILER)
  # No extra dropped dependencies for the MSYS_PLATFORM case.
  set(extra_ignored_dependencies_list)
else(MSYS_PLATFORM)
  # Propagate the overall CMake generator and install prefix to all CMake-based
  # software project builds.
  set(BP_CMAKE_COMMAND ${CMAKE_COMMAND} "-G${CMAKE_GENERATOR}" -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX})
  set(BP_CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})

  # Drop xmlcatalog-wrapper dependency since that is only suitable for
  # the MSYS_PLATFORM case.
  set(extra_ignored_dependencies_list xmlcatalog-wrapper)
endif(MSYS_PLATFORM)

# Propagate the install prefix to autotools-based builds
set(BP_CONFIGURE_COMMAND configure --prefix=${BP_CMAKE_INSTALL_PREFIX})

if(CMAKE_VERBOSE_MAKEFILE)
  # Not sure whether all contemplated back-ends support this or not, but
  # most should.
  list(APPEND BP_CMAKE_COMMAND -DCMAKE_VERBOSE_MAKEFILE=ON)
  # Some autotools build projects give verbose make results when V=1
  # and I don't think it will interfere with the rest.
  list(APPEND BP_MAKE_COMMAND V=1)
endif(CMAKE_VERBOSE_MAKEFILE)


# The parallel versions are for software packages that
# do not have race conditions for parallel builds or tests.
set(BP_PARALLEL_BUILD_COMMAND "${BP_BUILD_COMMAND}" -j4)
set(BP_PARALLEL_CTEST_COMMAND "${BP_CTEST_COMMAND}" -j4)
# For autotools based builds.
option(AUTOTOOLS_PARALLEL_BUILD "Build autotools projects using parallel make unless a specific project is expressely configured otherwise" ON)
if(AUTOTOOLS_PARALLEL_BUILD)
  set(BP_PARALLEL_MAKE_COMMAND "${BP_MAKE_COMMAND}" -j4)
else(AUTOTOOLS_PARALLEL_BUILD)
  set(BP_PARALLEL_MAKE_COMMAND "${BP_MAKE_COMMAND}")
endif(AUTOTOOLS_PARALLEL_BUILD)

message(STATUS "BP_CMAKE_COMMAND = ${BP_CMAKE_COMMAND}")
message(STATUS "BP_CONFIGURE_COMMAND = ${BP_CONFIGURE_COMMAND}")
message(STATUS "BP_BUILD_COMMAND = ${BP_BUILD_COMMAND}")
message(STATUS "BP_PARALLEL_BUILD_COMMAND = ${BP_PARALLEL_BUILD_COMMAND}")
message(STATUS "BP_MAKE_COMMAND = ${BP_MAKE_COMMAND}")
message(STATUS "BP_PARALLEL_MAKE_COMMAND = ${BP_PARALLEL_MAKE_COMMAND}")
message(STATUS "BP_CTEST_COMMAND = ${BP_CTEST_COMMAND}")
message(STATUS "BP_PARALLEL_CTEST_COMMAND = ${BP_PARALLEL_CTEST_COMMAND}")

# Put ${CMAKE_INSTALL_PREFIX}/bin on the PATH as well for those cases
# where some executable built and installed by build_projects needs
# to be found by another project being configured by build_projects.
if(MSYS_PLATFORM)
  set(BP_ORIGINAL_NATIVE_PATH "$ENV{PATH};${CMAKE_INSTALL_PREFIX}/bin")
else(MSYS_PLATFORM)
  set(BP_ORIGINAL_NATIVE_PATH "$ENV{PATH}:${CMAKE_INSTALL_PREFIX}/bin")
endif(MSYS_PLATFORM)
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")

# This option not used currently, but instead of removing it, comment
# it out because it might be useful later if we ever configure a project
# where we want a choice between an ordinary download and repository access.
# option(PREFER_DOWNLOAD "Prefer to obtain source code for projects using a simple download of tarball rather than via svn, git, hg, or bzr repository access" ON)

# Now include the build configurations for certain targets and their
# dependencies.

# Each (directly or indirectly) included build configuration
# that is judged to be working should append its target to build_target_LIST
# so that in the logic below we can creat a "build_all" target that depends
# on those individual build targets
set(build_target_LIST)

option(BUILD_THE_BUILDTOOLS "Build the buildtools (such as cmake) used for the rest of the builds" OFF)

if(BUILD_THE_BUILDTOOLS)
  set(executables_LIST
    sh
    )
  foreach(executable ${executables_LIST})
    string(TOUPPER ${executable} EXECUTABLE)
    find_program(${EXECUTABLE}_EXECUTABLE ${executable})
    if(NOT ${EXECUTABLE}_EXECUTABLE)
      if(MSYS_PLATFORM)
	message(STATUS "${executable} missing from your MSYS platform.  If sh is missing, use a generator other than 'MinGW Makefiles'.  If something else is missing use mingw-get to install it.")
      endif(MSYS_PLATFORM)
      message(FATAL_ERROR "${executable} must be on your PATH in order for build_projects to work correctly")  
    endif(NOT ${EXECUTABLE}_EXECUTABLE)
  endforeach(executable ${executables_LIST})

  set(build_configuration_LIST
    #cmake
    pkg-config
    #subversion
    swig
    tcl
    tk
    # itcl version 4 is integrated into tcl
    # itk version 4 is not integrated into tk (or tcl).
    itk
    # iwidgets version 4.1 is not integrated into itcl
    iwidgets
    # itcl version 3 is an independent project
    itcl3
    )
else(BUILD_THE_BUILDTOOLS)
  # List of all configurations.  Order doesn't matter because multiple
  # attempts to include the same configuration (via dependencies) are ignored.
  set(build_configuration_LIST
    ndiff
    plplot
    wxwidgets
    glib
    pango
    )
endif(BUILD_THE_BUILDTOOLS)

foreach(build_configuration ${build_configuration_LIST})
  include(${build_configuration}/bp.cmake)
endforeach(build_configuration ${build_configuration_LIST})

add_custom_target(build_all)
add_dependencies(build_all ${build_target_LIST})
