# cmake/modules/double.cmake
#
# Start driver initializations
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

# Module for determining configuration variables for PLplot related to
# double/single precision.

option(PL_DOUBLE 
"Compile the library with double precision floating point" ON)

# Force LIB_TAG to be empty in all cases.  This is completely
# different from the previous case where "d" was appended to the user
# setting of this variable if the library was double precision.  As
# discussed on list we are forcing LIB_TAG to be empty because it is
# only applied to a subset of our libraries (because the other ones do
# not have a single-precision or double-precision variant).
# Furthermore, the recommended and absolutely most reliable way to
# distinguish between various kinds of PLplot builds is not by
# appending various confusing library suffixes.  (For example, "d" is
# often incorrectly assumed to refer to a debugging version of the
# PLplot library.)  Instead, simply use a different install prefix for
# each different kind of build.
set(LIB_TAG "" CACHE STRING "LIB_TAG should _always_ be the empty string" FORCE)

