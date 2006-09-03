# examples/tk/CMakeLists.txt
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

set(tk_FILES
README.tkdemos
runAllDemos.tcl
runExtendedDemos.tcl
tkdemos.tcl
)

set(tk_STRING_INDICES
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

foreach(STRING_INDEX ${tk_STRING_INDICES})
  set(_file x${STRING_INDEX}.tcl)
  add_custom_command(
  OUTPUT ${_file}
  COMMAND ${CMAKE_COMMAND}
  -E copy
  ${CMAKE_SOURCE_DIR}/examples/tcl/${_file}
  ${CMAKE_CURRENT_SOURCE_DIR}/${_file}
  DEPENDS ${CMAKE_SOURCE_DIR}/examples/tcl/${_file}
  )
  set(tk_FILES ${tk_FILES} ${_file})
endforeach(STRING_INDEX ${tk_STRING_INDICES})

install(FILES ${tk_FILES} DESTINATION ${DATA_DIR}/examples/tk)

add_custom_target(tclIndex_examples_tk ALL
COMMAND ${TCL_TCLSH} ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
DEPENDS ${tk_FILES}
WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

install(FILES tclIndex DESTINATION ${DATA_DIR}/examples/tk)

set(tk_SCRIPTS
tk01
tk02
tk03
tk04
)

install(PROGRAMS ${tk_SCRIPTS} DESTINATION ${DATA_DIR}/examples/tk)

set(tk_SRC
xtk01.c
xtk02.c
xtk04.c
)

install(FILES ${tk_SRC} DESTINATION ${DATA_DIR}/examples/tk)

set(CC ${CMAKE_C_COMPILER})
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
DESTINATION ${DATA_DIR}/examples/tk
RENAME Makefile
)

