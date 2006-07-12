# cmake/modules/plplot.cmake
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

# Module for determining all configuration variables for PLplot.

# =======================================================================
# Compilation and build options (PLFLT, install locations, and rpath)
# Note, must come before java since that depends on, e.g., LIB_DIR.
# =======================================================================
include(double)
include(instdirs)
include(rpath)

# =======================================================================
# System checks for headers etc
# =======================================================================
include(CheckFunctionExists)
CHECK_FUNCTION_EXISTS(popen HAVE_POPEN)
CHECK_FUNCTION_EXISTS(usleep HAVE_USLEEP)
CHECK_FUNCTION_EXISTS(isinf HAVE_ISINF)
CHECK_FUNCTION_EXISTS(finite HAVE_FINITE)
CHECK_FUNCTION_EXISTS(isnan HAVE_ISNAN)

# =======================================================================
# Language bindings
# =======================================================================

include(c++)
#include(f77)
#include(f95)

include(drivers)
