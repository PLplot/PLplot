# cmake/modules/fortran.cmake
#
# F77/F95 binding configuration
#
# Copyright (C) 2006  Andrew Ross
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

# Module for determining F77/F95 bindings configuration options

# Options to enable Fortran bindings
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_f77 "Enable f77 bindings" OFF)
  option(ENABLE_f95 "Enable f95 bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_f77 "Enable f77 bindings" ON)
  option(ENABLE_f95 "Enable f95 bindings" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_f77 OR ENABLE_f95)
  # Check for fortran compiler
  include(CMakeDetermineFortranCompiler)
  if(NOT CMAKE_Fortran_COMPILER)
    message(STATUS "WARNING: "
    "fortran compiler not found. Disabling f77/f95 bindings"
    )
    set(ENABLE_f77 OFF CACHE BOOL "Enable f77 bindings" FORCE)
    set(ENABLE_f95 OFF CACHE BOOL "Enable f95 bindings" FORCE)
  endif(NOT CMAKE_Fortran_COMPILER)
endif (ENABLE_f77 OR ENABLE_f95)

if(ENABLE_f77 OR ENABLE_f95)
  # Enable fortran language
  enable_language(Fortran)

  # Don't compile Fortran 95 binding if compiler doesn't support it
  if(ENABLE_f95 AND NOT CMAKE_Fortran_COMPILER_SUPPORTS_F90)
    message(STATUS "WARNING: "
    "fortran compiler does not support f90/95. Disabling f95 bindings"
    )
    set(ENABLE_f95 OFF CACHE BOOL "Enable f95 bindings" FORCE)
  endif(ENABLE_f95 AND NOT CMAKE_Fortran_COMPILER_SUPPORTS_F90)

  # Set installation location for f95 modules.
  set(F95_MOD_DIR ${LIB_DIR}/fortran/modules/${PACKAGE})

  # Check if f77 command line parsing is possible
  if(ENABLE_f77)
    include(TestF77CmdLine)
  endif(ENABLE_f77)

  # Determine which Fortran compiler we have.  We do not need to
  # this for all compilers, just the ones that have a test in
  # bindings/f77/plstubs.h and bindings/f95/plstubs.h

  message(STATUS "NOTICE: " "Found: ${CMAKE_Fortran_COMPILER}")
  if(CMAKE_Fortran_COMPILER MATCHES ".*/ifort.*")
    # Intel Visual Fortran
    message(STATUS "NOTICE: " "Found Intel Visual Fortran")
    set(TARGET_FORTRAN "IVF" CACHE STRING "Target Fortran Compiler")
  endif(CMAKE_Fortran_COMPILER MATCHES ".*/ifort.*")

  if(CMAKE_Fortran_COMPILER MATCHES ".*/DF.*")
    # Compaq Visual Fortran
    message(STATUS "NOTICE: " "Found Compaq Visual Fortran")
    set(TARGET_FORTRAN "CVF" CACHE STRING "Target Fortran Compiler")
  endif(CMAKE_Fortran_COMPILER MATCHES ".*/DF.*")

  if(CMAKE_Fortran_COMPILER MATCHES ".*/F90.*" AND WIN32)
    # Compaq Visual Fortran - alternative name
    message(STATUS "NOTICE: " "Found Compaq Visual Fortran")
    set(TARGET_FORTRAN "CVF" CACHE STRING "Target Fortran Compiler")
  endif(CMAKE_Fortran_COMPILER MATCHES ".*/F90.*" AND WIN32)

endif(ENABLE_f77 OR ENABLE_f95)
