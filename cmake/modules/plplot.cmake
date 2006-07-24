# cmake/modules/plplot.cmake
#
# Start driver initializations
#
# Copyright (C) 2006  Alan W. Irwin
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
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Module for determining all configuration variables for PLplot.

# Need this module to check for C header files in included modules
# below using the check_include_files macro.
include(CheckIncludeFiles)

# =======================================================================
# Compilation and build options (PLFLT, install locations, and rpath)
# Note, must come before java since that depends on, e.g., LIB_DIR.
# =======================================================================
include(double)
include(instdirs)
include(rpath)
option(BUILD_TEST "Compile examples in the build tree and enable ctest" OFF)

# =======================================================================
# System checks for headers etc
# =======================================================================

# Reasonable approximation to AC_HEADER_DIRENT without the SCO stuff.
include(CheckDIRSymbolExists)
check_dirsymbol_exists("sys/types.h;dirent.h" HAVE_DIRENT_H)
if(NOT HAVE_DIRENT_H)
  check_dirsymbol_exists("sys/types.h;sys/ndir.h" HAVE_SYS_NDIR_H)
  if(NOT HAVE_SYS_NDIR_H)
    check_dirsymbol_exists("sys/types.h;sys/dir.h" HAVE_SYS_DIR_H)
    if(NOT HAVE_SYS_DIR_H)
      check_dirsymbol_exists("sys/types.h;ndir.h" HAVE_NDIR_H)
      if(NOT HAVE_NDIR_H)
        message(FATAL_ERROR 
	"FATAL_ERROR for plplot.cmake: "
	"DIR symbol must be defined by system headers."
	)
      endif(NOT HAVE_NDIR_H)
    endif(NOT HAVE_SYS_DIR_H)
  endif(NOT HAVE_SYS_NDIR_H)
endif(NOT HAVE_DIRENT_H)
# Note the above tests #include <sys/types.h> to follow how
# AC_HEADER_DIRENT does its testing.  Therefore, always do our
# own #defines that way for the cmake build system.  Note, this
# sys/types.h requirement occurs for Mac OS X and possibly other systems.
# It is possible it will go away in the future, but we will follow whatever
# is done by AC_HEADER_DIRENT here until that changes.
set(NEED_SYS_TYPE_H ON)

# Test signal handler return type (mimics AC_TYPE_SIGNAL)
include(TestSignalType)

include(CheckFunctionExists)
check_function_exists(popen HAVE_POPEN)
check_function_exists(usleep HAVE_USLEEP)
check_function_exists(isinf HAVE_ISINF)
check_function_exists(finite HAVE_FINITE)
check_function_exists(isnan HAVE_ISNAN)

# =======================================================================
# Language bindings
# =======================================================================

# Find swig - no version checking at present
FIND_PACKAGE(SWIG)
if(SWIG_FOUND)
  include(${SWIG_USE_FILE})
endif(SWIG_FOUND)

# Load language specific files
include(c++)
include(fortran)
include(java)
#include(python)

# =======================================================================
# additional library support
# =======================================================================
include(freetype)

# =======================================================================
# pkg-config support
# =======================================================================

include(pkg-config)

# =======================================================================
# Device drivers
# =======================================================================
include(drivers)
