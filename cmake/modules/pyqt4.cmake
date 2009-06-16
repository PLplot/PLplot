# bindings/python/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2009 Hazen Babcock
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
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Options to enable PyQt4 bindings
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_pyqt4 "Enable PyQt4 bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_pyqt4 "Enable PyQt4 bindings" OFF)
endif(DEFAULT_NO_BINDINGS)

if(NOT ENABLE_python AND NOT PLD_extqt)
  set(ENABLE_pyqt4 OFF CACHE BOOL "Enable PyQt4 bindings" FORCE)
endif(NOT ENABLE_python AND NOT PLD_extqt)

if(ENABLE_pyqt4)
  message(STATUS "Checking for PyQt4")
endif(ENABLE_pyqt4)
