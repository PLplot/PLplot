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

if(ENABLE_pyqt4)
  message(" ENABLE_pyqt4")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/config.py.in "")
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/
    COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/config.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif(ENABLE_pyqt4)
