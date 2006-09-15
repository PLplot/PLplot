# cmake/modules/agg.cmake
#
# Copyright (C) 2006  Werner Smekal
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
#
# Configuration for the freetype support in plplot.

option(
WITH_AGG
"Enable driver options for using AGG library for antializing"
OFF
)

# Look for agg libraries
if (WITH_AGG)
  find_package(AGG)
  if (NOT AGG_FOUND)
    set(WITH_AGG OFF
    CACHE BOOL "Enable driver options for using AGG library for antializing"
    FORCE
    )
  else (NOT AGG_FOUND)
	  set(HAVE_AGG ON)
  endif (NOT AGG_FOUND)
endif (WITH_AGG)
