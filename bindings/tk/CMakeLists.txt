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

# Ugly hack to copy all .tcl files to the build tree so that tclIndex 
# can be generated there. This is a limitation is tcl's auto_mkindex which 
# always generates the index in the directory with the files.
if(NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  set(tclIndex_DEPENDS)
  foreach(file ${tcldata} ${itkdata})
    set(
    tclIndex_DEPENDS
    ${tclIndex_DEPENDS}
    ${CMAKE_CURRENT_BINARY_DIR}/${file}
    )
    add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${file}
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
    )
  endforeach(file ${tcldata} ${itkdata})
  add_custom_target(tcl_files ALL DEPENDS ${tclIndex_DEPENDS})
else(NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  set(tclIndex_DEPENDS ${tcldata} ${itkdata})
endif(NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")

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

add_custom_command(
OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
COMMAND ${TCL_TCLSH} ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
DEPENDS ${tclIndex_DEPENDS}
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

add_custom_target(tclIndex_tk ALL
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
)

# The tcl_files dependency is required to
# work around CMake 2-level make recursion issue where independent
# file dependency chains that refer to the same files must have
# target dependencies between them in order to work for parallel
# builds (and possibly some visual studio builds).
if(NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  add_dependencies(tclIndex_tk tcl_files)
endif(NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")

install(
FILES ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
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
