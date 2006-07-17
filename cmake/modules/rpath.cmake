# cmake/modules/rpath.cmake
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

# Module for determining treatment of rpath for PLplot.  Must be
# included after the instdirs module which defines LIB_DIR.

option(USE_RPATH "use -rpath when linking libraries" ON)
if(USE_RPATH)
  set(CMAKE_INSTALL_RPATH ${LIB_DIR})
  set(RPATHCMD "-Wl,-rpath -Wl,${LIB_DIR}")
endif(USE_RPATH)
