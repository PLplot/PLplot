# cmake/modules/fortran.cmake
#
# F95 binding configuration
#
# Copyright (C) 2006  Andrew Ross
# Copyright (C) 2006-2016 Alan W. Irwin
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

# Module for determining F95 binding configuration options

option(PL_DEPRECATED_f95 "Use deprecated version of Fortran 95 binding and examples" OFF)

# Options to enable Fortran binding
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_f95 "Enable f95 binding" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_f95 "Enable f95 binding" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_f95 AND NOT PLPLOT_Fortran_COMPILER_WORKS)
  workaround_9220(Fortran PLPLOT_Fortran_COMPILER_WORKS)
  if(NOT PLPLOT_Fortran_COMPILER_WORKS)
    message(STATUS "WARNING: no working Fortran compiler so disabling f95 binding and examples.")
    set(ENABLE_f95 OFF CACHE BOOL "Enable f95 binding" FORCE)
  endif(NOT PLPLOT_Fortran_COMPILER_WORKS)
endif(ENABLE_f95 AND NOT PLPLOT_Fortran_COMPILER_WORKS)

if(ENABLE_f95)
  # Find and check Fortran compiler.
  enable_language(Fortran OPTIONAL)
  if(NOT CMAKE_Fortran_COMPILER_WORKS)
    message(STATUS "WARNING: no working Fortran compiler so disabling f95 binding and examples.")
    set(ENABLE_f95 OFF CACHE BOOL "Enable f95 binding" FORCE)
  endif(NOT CMAKE_Fortran_COMPILER_WORKS)
endif(ENABLE_f95)

if(ENABLE_f95)
  # Don't compile Fortran 95 binding if compiler doesn't support it
  if(ENABLE_f95 AND NOT CMAKE_Fortran_COMPILER_SUPPORTS_F90)
    message(STATUS "WARNING: "
    "fortran compiler does not support f90/95. Disabling f95 binding"
    )
    set(ENABLE_f95 OFF CACHE BOOL "Enable f95 binding" FORCE)
  endif(ENABLE_f95 AND NOT CMAKE_Fortran_COMPILER_SUPPORTS_F90)

  # Set installation location for f95 modules.
  set(F95_MOD_DIR ${LIB_DIR}/fortran/modules/${PACKAGE}
    CACHE PATH "installation location for f95 modules"
    )

  # Check if isnan is available as a fortran function
  include(TestFortranIsnan)

  # Determine which Fortran compiler we have.  We do not need to
  # this for all compilers, just the ones that have a test in
  # bindings/f95/plstubs.h

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

  if(CMAKE_Fortran_COMPILER MATCHES ".*/F90.*" AND WIN32_OR_CYGWIN)
    # Compaq Visual Fortran - alternative name
    message(STATUS "NOTICE: " "Found Compaq Visual Fortran")
    set(TARGET_FORTRAN "CVF" CACHE STRING "Target Fortran Compiler")
  endif(CMAKE_Fortran_COMPILER MATCHES ".*/F90.*" AND WIN32_OR_CYGWIN)

endif(ENABLE_f95)
