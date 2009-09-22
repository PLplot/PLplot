# cmake/modules/plplot.cmake
#
# Copyright (C) 2006, 2007, 2008, 2009 Alan W. Irwin
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

# Module file to decide whether to build doxygen documentation and
# and, if so, find the necessary doxygen command.

option(BUILD_DOX_DOC "Build doxygen documentation" OFF)

if(BUILD_DOX_DOC)
  find_package(Doxygen)
  if(NOT DOXYGEN_FOUND)
    message(STATUS "WARNING: "
      "Cannot find doxygen so disabling doxygen documentation build.")
    set(BUILD_DOX_DOC OFF CACHE BOOL "Build doxygen documentation" FORCE)
  endif(NOT DOXYGEN_FOUND)
endif(BUILD_DOX_DOC)
