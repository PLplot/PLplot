# cmake/modules/plplot.cmake
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

# Need these modules to do subsequent checks.
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckPrototypeExists)

# =======================================================================
# Compilation and build options (PLFLT, install locations, and rpath)
# Note, must come before java since that depends on, e.g., LIB_DIR.
# =======================================================================

# WIN32 covers CYGWIN as well (and possibly MINGW, but we will make sure).
if(WIN32 OR MINGW)
  set(EXEEXT .exe)
endif(WIN32 OR MINGW)

include(double)
include(instdirs)
include(rpath)
option(BUILD_TEST "Compile examples in the build tree and enable ctest" OFF)

# =======================================================================
# System checks for headers etc
# =======================================================================

# AC_HEADER_STDC is gross overkill since the current PLplot code only uses 
# this for whether or not atexit can be used.  But implement the full suite
# of AC_HEADER_STDC checks to keep the cmake version in synch with autotools
# and just in case some PLplot developer assumes the complete check for
# standard headers is done for a future programming change.
#
# From info autoconf....
# Define STDC_HEADERS if the system has ANSI C header files.
# Specifically, this macro checks for stdlib.h', stdarg.h',
# string.h', and float.h'; if the system has those, it probably
# has the rest of the ANSI C header files.  This macro also checks
# whether string.h' declares memchr' (and thus presumably the
# other mem' functions), whether stdlib.h' declare free' (and
# thus presumably malloc' and other related functions), and whether
# the ctype.h' macros work on characters with the high bit set, as
# ANSI C requires.

message(STATUS "Checking whether system has ANSI C header files")
check_include_files("stdlib.h;stdarg.h;string.h;float.h" StandardHeadersExist)
if(StandardHeadersExist)
  check_prototype_exists(memchr string.h memchrExists)
  if(memchrExists)
    check_prototype_exists(free stdlib.h freeExists)
    if(freeExists)
      include(TestForHighBitCharacters)
      if(CMAKE_HIGH_BIT_CHARACTERS)
        message(STATUS "ANSI C header files - found")
        set(STDC_HEADERS 1 CACHE INTERNAL "System has ANSI C header files")
      endif(CMAKE_HIGH_BIT_CHARACTERS)
    endif(freeExists)
  endif(memchrExists)
endif(StandardHeadersExist)
if(NOT STDC_HEADERS)
  message(STATUS "ANSI C header files - not found")
  set(STDC_HEADERS 0 CACHE INTERNAL "System has ANSI C header files")
endif(NOT STDC_HEADERS)

# AC_CHECK_HEADERS(unistd.h termios.h)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(termios.h HAVE_TERMIOS_H)

# AC_HEADER_SYS_WAIT
include(TestForStandardHeaderwait)

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
include(python)

# =======================================================================
# additional library support
# =======================================================================
include(freetype)
include(csiro)

# =======================================================================
# pkg-config support
# =======================================================================

include(pkg-config)

# =======================================================================
# Device drivers
# =======================================================================
include(drivers)

# =======================================================================
# Miscellaneous other features - including docbook documentation
# =======================================================================
include(docbook)
