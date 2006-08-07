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

add_custom_target(tclIndex ALL
COMMAND ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
)

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
