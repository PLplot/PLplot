# cmake/modules/ada.cmake
#
# Copyright (C) 2007 Alan W. Irwin
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
  option(ENABLE_ada "Enable Ada bindings" OFF)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_ada)
  find_library(GNAT_LIB NAMES gnat gnat-4.1 gnat-4.2 gnat-4.3)
  if(NOT GNAT_LIB)
    message(STATUS "WARNING: "
      "gnat library not found. Disabling ada bindings")
    set(ENABLE_ada OFF CACHE BOOL "Enable Ada bindings" FORCE)
  else(NOT GNAT_LIB)
    message(STATUS "FOUND gnat library ${GNAT_LIB}")
  endif(NOT GNAT_LIB)
endif(ENABLE_ada)

if(ENABLE_ada)
  enable_language(Ada)
endif(ENABLE_ada)



# New stuff by Jerry for source modifications for Ada 2007 or not Ada 2007.
option(HAVE_ADA_2007 "Ada 2007?" OFF)

if(HAVE_ADA_2007)
  set(Ada_Is_2007_With_and_Use_Numerics "    with Ada.Numerics.Long_Real_Arrays; use Ada.Numerics.Long_Real_Arrays;")
else(HAVE_ADA_2007)
  # Is there some way to put a line ending after the first semicolon in the quoted string? Not essential.
  set(Ada_Is_Not_2007_Vector_Matrix_Declarations "    type Real_Vector is array (Integer range <>) of Long_Float;    type Real_Matrix is array (Integer range <>, Integer range <>) of Long_Float;")
endif(HAVE_ADA_2007)
