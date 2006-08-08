# bindings/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Alan W. Irwin
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

ADD_SUBDIRECTORY( c++ ) 
ADD_SUBDIRECTORY( f77 ) 
ADD_SUBDIRECTORY( f95 ) 
ADD_SUBDIRECTORY( python ) 
ADD_SUBDIRECTORY( tcl ) 
ADD_SUBDIRECTORY( tk ) 
#ADD_SUBDIRECTORY( octave ) 
ADD_SUBDIRECTORY( java ) 
ADD_SUBDIRECTORY( gnome2 ) 

set(index)
if(ENABLE_tcl)
  set(index ${index} ${CMAKE_CURRENT_BINARY_DIR}/tcl/pkgIndex.tcl)
endif(ENABLE_tcl)
if(ENABLE_tk)
  set(index ${index} ${CMAKE_CURRENT_BINARY_DIR}/tk/pkgIndex.tcl)
endif(ENABLE_tk)
if(ENABLE_tkwin)
  set(index ${index} ${CMAKE_CURRENT_BINARY_DIR}/tk-x-plat/pkgIndex.tcl)
endif(ENABLE_tkwin)

if(index)
  add_custom_target(pkgIndex.tcl ALL
  COMMAND cat ${index} > pkgIndex.tcl
  DEPENDS ${index}
  )
  install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl
  DESTINATION ${DATA_DIR}
  )
endif(index)
