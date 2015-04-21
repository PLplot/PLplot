# bindings/tk-x-plat/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2015 Alan W. Irwin
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

if(PLD_tkwin)

  set(DRV_HARDDIR ${DRV_DIR})
  set(LIB_HARDDIR ${LIB_DIR})

  set(DLNAME_LIBPLPLOT $<TARGET_FILE_NAME:plplot>)
  # Configure "@" items
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkgIndex.tcl.in
    ${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl_atonly
    @ONLY
    )
  # Configure generator expression items.
  file(GENERATE
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl
    INPUT ${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl_atonly
    )

endif(PLD_tkwin)
