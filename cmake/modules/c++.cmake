# cmake/modules/cxx.cmake
#
# C++ binding configuration
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

# Module for determining C++ bindings configuration options

# Option to enable C++ bindings
if(DEFAULT_NO_BINDINGS)
  OPTION(ENABLE_cxx "Enable C++ bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  OPTION(ENABLE_cxx "Enable C++ bindings" ON)
endif(DEFAULT_NO_BINDINGS)

# OPTIONAL below currently does not work properly with CMake so attempt
# to deal with a possibly missing compiler first as a workaround.
if(ENABLE_cxx)
  include(CMakeDetermineCXXCompiler)
  if(NOT CMAKE_CXX_COMPILER)
    message(STATUS "WARNING: C++ compiler not found so disabling C++ bindings and examples.")
    set(ENABLE_cxx OFF CACHE BOOL "Enable C++ bindings" FORCE)
  endif(NOT CMAKE_CXX_COMPILER)
endif(ENABLE_cxx)

if(ENABLE_cxx)
  # Find and check C++ compiler.
  enable_language(CXX OPTIONAL)
  
  if(NOT CMAKE_CXX_COMPILER_WORKS)
    message(STATUS "No working C++ compiler found so disabling C++ bindings and examples.")
    set(ENABLE_cxx OFF CACHE BOOL "Enable C++ bindings" FORCE)
  endif(NOT CMAKE_CXX_COMPILER_WORKS)
endif(ENABLE_cxx)

if(ENABLE_cxx)
  # Check if "using namespace std;" works
  INCLUDE(TestForNamespace)

  # Check for <cmath>
  INCLUDE(CheckIncludeFileCXX)
  CHECK_INCLUDE_FILE_CXX(cmath HAVE_CMATH)

  if(NOT MSVC)
    # Need to add check for broken cmath with isnan missing (BROKEN_ISNAN_CXX)
    # but not for Visual C++ compilers
    include(TestBrokenIsnanCXX)
  endif(NOT MSVC)

  # Need to add check if stdint.h can be used from c++ (HAVE_CXX_STDINT_H)
  INCLUDE(TestForStdintCXX)
endif(ENABLE_cxx)
