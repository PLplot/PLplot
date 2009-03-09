# cmake/modules/d.cmake
#
# D binding configuration
#
# Copyright (C) 2008  Werner Smekal
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

# Module for determining D bindings configuration options

# Options to enable D bindings
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_d "Enable D bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_d "Enable D bindings" OFF)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_d)
  # Check for gdc or dmd compiler
  include(CMakeDetermineDCompiler)
  if(NOT CMAKE_D_COMPILER)
    message(STATUS "WARNING: "
    "D compiler not found. Disabling D bindings"
    )
    set(ENABLE_d OFF CACHE BOOL "Enable D bindings" FORCE)
  endif(NOT CMAKE_D_COMPILER)
endif(ENABLE_d)

if(ENABLE_d)
  # Enable D language
  enable_language(D)
endif(ENABLE_d)
