# examples/tcl/CMakeLists.txt
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

set(tcl_FILES
README.tcldemos
plgrid.tcl
plot.dat
plot.tcl
r.dat
stats.log
tcldemos.tcl
)

set(tcl_STRING_INDICES
"01"
"02"
"03"
"04"
"05"
"06"
"07"
"08"
"09"
"10"
"11"
"12"
"13"
"14"
"15"
"16"
"17"
"18"
"19"
"22"
)

set(tcl_SCRIPTS)
foreach(STRING_INDEX ${tcl_STRING_INDICES})
  set(tcl_FILES ${tcl_FILES} x${STRING_INDEX}.tcl)
  set(tcl_SCRIPTS ${tcl_SCRIPTS} x${STRING_INDEX})
endforeach(STRING_INDEX ${tcl_STRING_INDICES})

install(FILES ${tcl_FILES} DESTINATION ${DATA_DIR}/examples/tcl)
install(PROGRAMS ${tcl_SCRIPTS} DESTINATION ${DATA_DIR}/examples/tcl)

set(tclIndex_DEPENDS ${tcl_FILES} ${tcl_SCRIPTS})
set(tclIndex_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
if(BUILD_TEST AND
NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}"
)
  # equivalent to install commands but at "make" time rather than
  # "make install" time, to the build tree if different than the source
  # tree.
  set(tclIndex_WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  set(tclIndex_DEPENDS)
  foreach(file ${tcl_SCRIPTS} ${tcl_FILES})
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
  endforeach(file ${tcl_SCRIPTS} ${tcl_FILES})
  add_custom_target(tcl_examples ALL DEPENDS ${tclIndex_DEPENDS})
endif(BUILD_TEST AND
NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}"
)

add_custom_target(tclIndex_examples_tcl ALL
COMMAND ${TCL_TCLSH} ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
DEPENDS ${tclIndex_DEPENDS}
WORKING_DIRECTORY ${tclIndex_WORKING_DIRECTORY}
)

install(
FILES ${tclIndex_WORKING_DIRECTORY}/tclIndex
DESTINATION ${DATA_DIR}/examples/tcl
)
