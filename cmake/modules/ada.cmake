# cmake/modules/ada.cmake
#
# Copyright (C) 2007-2015 Alan W. Irwin
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

# Module for determining Ada bindings configuration options

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_ada "Enable Ada bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_ada "Enable Ada bindings" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_ada AND NOT PLPLOT_Ada_COMPILER_WORKS)
  workaround_9220(Ada PLPLOT_Ada_COMPILER_WORKS)
  if(NOT PLPLOT_Ada_COMPILER_WORKS)
    message(STATUS "WARNING: no working Ada compiler so disabling Ada bindings and examples.")
    set(ENABLE_ada OFF CACHE BOOL "Enable Ada bindings" FORCE)
  endif(NOT PLPLOT_Ada_COMPILER_WORKS)
endif(ENABLE_ada AND NOT PLPLOT_Ada_COMPILER_WORKS)

if(ENABLE_ada)
  # Find and check Ada compiler
  enable_language(Ada OPTIONAL)
  if(NOT CMAKE_Ada_COMPILER_WORKS)
    message(STATUS "WARNING: no working Ada compiler so disabling Ada bindings and examples.")
    set(ENABLE_ada OFF CACHE BOOL "Enable Ada bindings" FORCE)
  endif(NOT CMAKE_Ada_COMPILER_WORKS)
endif(ENABLE_ada)
