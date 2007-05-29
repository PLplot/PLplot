# bindings/tk/CMakeLists.txt
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

if(ENABLE_tk)

set(include_INSTALLED_HEADERS
pltk.h
)

install(FILES ${include_INSTALLED_HEADERS} DESTINATION ${INCLUDE_DIR})

set(tcldata
FileSelector.tcl
Pltkwin.tcl
about.tcl
help_gui.tcl
help_keys.tcl
help_tcltk.tcl
plclient.tcl
plcolor.tcl
plconfig.tcl
pldefaults.tcl
plplot.tcl
plserver.tcl
pltools.tcl
plwidget.tcl
)

set(itkdata
PLWin.itk
PLXWin.itk
)

set(paldata
cmap0a.pal
cmap1a.pal
cmap1a1.pal
cmap1b.pal
cmap1c.pal
cmap1d.pal
)

install(
FILES ${tcldata} ${itkdata} ${paldata}
DESTINATION ${TCL_DIR}
)

include_directories(
${TCL_INCLUDE_PATH}
${TK_INCLUDE_PATH}
${CMAKE_SOURCE_DIR}/include
${CMAKE_SOURCE_DIR}/bindings/tcl
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)

add_executable(plserver plserver.c)
target_link_libraries(plserver plplot${LIB_TAG} plplottcltk${LIB_TAG})

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
  set_target_properties(plserver
  PROPERTIES
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  )
endif(USE_RPATH)

install(TARGETS plserver DESTINATION ${BIN_DIR})

add_custom_target(tclIndex_tk ALL
COMMAND ${TCL_TCLSH} ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
DEPENDS ${tcldata} ${itkdata}
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

install(
FILES ${CMAKE_CURRENT_SOURCE_DIR}/tclIndex
DESTINATION ${TCL_DIR}
)

set(DRV_HARDDIR ${DRV_DIR})
set(LIB_HARDDIR ${LIB_DIR})

get_target_property(
DLNAME_LIBPLPLOT
plplot${LIB_TAG}
LOCATION
)
get_filename_component(
DLNAME_LIBPLPLOT
${DLNAME_LIBPLPLOT}
NAME
)
string(REPLACE "plplotd" "plplot"
DLNAME_LIBPLPLOT
${DLNAME_LIBPLPLOT}
)
string(REPLACE "plplot" "plplotd"
DLNAME_LIBPLPLOTD
${DLNAME_LIBPLPLOT}
)

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/pkgIndex.tcl.in
${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl
@ONLY
)

endif(ENABLE_tk)
