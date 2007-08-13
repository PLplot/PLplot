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

# libraries are all shared by default
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Need these modules to do subsequent checks.
include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckPrototypeExists)

macro(TRANSFORM_VERSION _numerical_result _version)
# _version must be a period-delimited triplet string of the form
# "major.minor.patch".
# This macro transforms that into a numerical result that can be compared.
if(${_version} MATCHES "^[0-9]*\\.[0-9]*\\.[0-9]*$")
  string(REGEX REPLACE "^([0-9]*)\\.[0-9]*\\.[0-9]*$" "\\1" _major ${_version})
  string(REGEX REPLACE "^[0-9]*\\.([0-9]*)\\.[0-9]*$" "\\1" _minor ${_version})
  string(REGEX REPLACE "^[0-9]*\\.[0-9]*\\.([0-9]*)$" "\\1" _patch ${_version})
  math(EXPR ${_numerical_result}
  "${_major}*1000000 + ${_minor}*1000 + ${_patch}
  ")
else(${_version} MATCHES "^[0-9]*\\.[0-9]*\\.[0-9]*$")
  set(${_numerical_result} 0)
endif(${_version} MATCHES "^[0-9]*\\.[0-9]*\\.[0-9]*$")
endmacro(TRANSFORM_VERSION)

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
# Use bash when available for ctest and install tree test support
find_program(SH_EXECUTABLE bash)
find_program(SH_EXECUTABLE win-bash)
if(SH_EXECUTABLE)
  set(HAVE_BASH ON)
else(SH_EXECUTABLE)
  find_program(SH_EXECUTABLE sh)
endif(SH_EXECUTABLE)

if(NOT SH_EXECUTABLE)
   message(STATUS
   "WARNING: bash shell not found, ctest will not work properly"
   )
endif(NOT SH_EXECUTABLE)

option(PREBUILD_DIST "Pre-build all components required for distribution" OFF)
if(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(
  PREBUILD_DIST OFF CACHE INTERNAL
  "Pre-build all components required for distribution"
  )
endif(NOT CMAKE_SYSTEM_NAME STREQUAL "Linux")

# =======================================================================
# Headers
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

# AC_CHECK_HEADERS(unistd.h termios.h stdint.h)
check_include_files(unistd.h HAVE_UNISTD_H)
check_include_files(termios.h HAVE_TERMIOS_H)
check_include_files(stdint.h HAVE_STDINT_H)

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
      if(NOT HAVE_NDIR_H AND UNIX)
        message(FATAL_ERROR
        "FATAL_ERROR for plplot.cmake: "
        "DIR symbol must be defined by Unix system headers."
        )
      endif(NOT HAVE_NDIR_H AND UNIX)
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

#=======================================================================
# Typedefs
#=======================================================================

# In the past, some X11 headers required "caddr_t" even on systems that
# claimed POSIX.1 compliance, which was illegal.  This made it impossible
# to compile programs that included X11 headers if _POSIX_SOURCE was
# defined.  We used to work around this potential problem by just defining
# caddr_t to 'char *' on all systems (unless it is set already), whether
# it was needed or not. Now we ignore the issue because we don't expect
# such broken X behaviour any more and because this kind of argument list
# for AC_CHECK_TYPE is now deprecated in the autoconf documentation.

# Do not implement the equivalent of this since commented out in the ABS
# system.
# AC_CHECK_TYPE(caddr_t, char *)

# Test signal handler return type (mimics AC_TYPE_SIGNAL)
include(TestSignalType)

include(CheckFunctionExists)
check_function_exists(popen HAVE_POPEN)
check_function_exists(usleep HAVE_USLEEP)
check_function_exists(isinf HAVE_ISINF)
check_function_exists(finite HAVE_FINITE)
if(NOT HAVE_FINITE)
  check_function_exists(_finite _HAVE_FINITE)
  set(HAVE_FINITE ${_HAVE_FINITE} CACHE INTERNAL "Have function _finite")
endif(NOT HAVE_FINITE)
check_function_exists(isnan HAVE_ISNAN)
if(NOT HAVE_ISNAN)
  check_function_exists(_isnan _HAVE_ISNAN)
  set(HAVE_ISNAN ${_HAVE_ISNAN} CACHE INTERNAL "Have function _isnan")
endif(NOT HAVE_ISNAN)

# =======================================================================
# Language bindings
# =======================================================================

# Find swig.  Required for python and java bindings.
# N.B. all version tests done below need experimental FindSWIG.cmake which
# is currently carried in this directory by PLplot, but which eventually
# should get into CMake.
find_package(SWIG)
if(SWIG_FOUND)
  message(STATUS "SWIG_VERSION = ${SWIG_VERSION}")
  # Logic that depends on swig version
  transform_version(NUMERICAL_SWIG_MINIMUM_VERSION "1.3.22")
  transform_version(NUMERICAL_SWIG_VERSION "${SWIG_VERSION}")
  if(NUMERICAL_SWIG_VERSION LESS "${NUMERICAL_SWIG_MINIMUM_VERSION}")
    message(STATUS "WARNING: swig version too old.  SWIG_FOUND set to OFF")
    set(SWIG_FOUND OFF)
  endif(NUMERICAL_SWIG_VERSION LESS "${NUMERICAL_SWIG_MINIMUM_VERSION}")
endif(SWIG_FOUND)

if(SWIG_FOUND)
  transform_version(NUMERICAL_NOPGCPP_MINIMUM_VERSION "1.3.30")
  if(NUMERICAL_SWIG_VERSION LESS "${NUMERICAL_NOPGCPP_MINIMUM_VERSION}")
    set(SWIG_JAVA_NOPGCPP OFF)
  else(NUMERICAL_SWIG_VERSION LESS "${NUMERICAL_NOPGCPP_MINIMUM_VERSION}")
    set(SWIG_JAVA_NOPGCPP ON)
  endif(NUMERICAL_SWIG_VERSION LESS "${NUMERICAL_NOPGCPP_MINIMUM_VERSION}")
endif(SWIG_FOUND)

if(SWIG_FOUND)
  include(${SWIG_USE_FILE})
endif(SWIG_FOUND)

# Find Perl.  Required in several places in the build system (e.g.,
# tcl and docbook).
find_package(Perl)
if(PERL_FOUND)
    include(CheckPerlModules)
endif(PERL_FOUND)

# Find X headers, libraries, and library directory (required by xwin device
# driver and also everything that is Tk related).
find_package(X11)
if(X11_INCLUDE_DIR)
  # remove duplicates in the X11_INCLUDE_DIR list since those screw up
  # certain of our modules and also slow down compilation if any of
  # those duplicates leak through to the compile -I options.
  list(SORT X11_INCLUDE_DIR)
  set(copy_X11_INCLUDE_DIR ${X11_INCLUDE_DIR})
  set(listindex 0)
  set(listindexplus 1)
  foreach(copy_listelement ${copy_X11_INCLUDE_DIR})
    # need to get list elements corresponding to list indices.
    list(LENGTH X11_INCLUDE_DIR listlength)
    if(listindexplus LESS ${listlength})
      list(GET X11_INCLUDE_DIR ${listindex} listelement)
      list(GET X11_INCLUDE_DIR ${listindexplus} listelementplus)
    else(listindexplus LESS ${listlength})
      set(listelement)
      set(listelementplus)
    endif(listindexplus LESS ${listlength})
    if(copy_listelement STREQUAL "${listelement}" AND
    copy_listelement STREQUAL "${listelementplus}"
    )
      list(REMOVE_AT X11_INCLUDE_DIR ${listindex})
    else(copy_listelement STREQUAL "${listelement}" AND
    copy_listelement STREQUAL "${listelementplus}"
    )
      # update list indices
      math(EXPR listindex "${listindex} + 1")
      math(EXPR listindexplus "${listindex} + 1")
    endif(copy_listelement STREQUAL "${listelement}" AND
    copy_listelement STREQUAL "${listelementplus}"
    )
  endforeach(copy_listelement ${copy_X11_INCLUDE_DIR})
  string(REGEX REPLACE ";" " -I" X11_COMPILE_FLAGS "-I${X11_INCLUDE_DIR}")
endif(X11_INCLUDE_DIR)
message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "X11_INCLUDE_DIR = ${X11_INCLUDE_DIR}")
message(STATUS "X11_COMPILE_FLAGS = ${X11_COMPILE_FLAGS}")
message(STATUS "X11_LIBRARIES = ${X11_LIBRARIES}")
message(STATUS "X11_LIBRARY_DIR = ${X11_LIBRARY_DIR}")

option(DEFAULT_NO_BINDINGS
"All language bindings are disabled by default"
OFF
)
# Load language specific files
include(c++)
include(fortran)
include(java)
include(python)
include(octave)
include(tcl-related)
include(pdl)
include(ada)

# =======================================================================
# additional library support
# =======================================================================
include(freetype)
include(csiro)
# On windows systems the math library is not separated so do not specify
# it unless you are on a non-windows system.
if(NOT WIN32)
  set(MATH_LIB m)
endif(NOT WIN32)

# =======================================================================
# pkg-config support
# =======================================================================
include(pkg-config)

# =======================================================================
# libpango support
# =======================================================================
include(pango)

# =======================================================================
# Device drivers
# =======================================================================
include(drivers)

# =======================================================================
# Miscellaneous other features - including docbook documentation
# =======================================================================
include(docbook)
include(summary)
