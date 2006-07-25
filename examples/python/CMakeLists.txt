# examples/python/CMakeLists.txt
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

set(python_STRING_INDICES
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
"15"
"16"
"18"
"19"
"22"
"23"
"24"
"26"
)

set(python_SCRIPTS)
set(python_DATA)
foreach(STRING_INDEX ${python_STRING_INDICES})
  set(python_SCRIPTS ${python_SCRIPTS} x${STRING_INDEX})
  set(python_DATA ${python_DATA} xw${STRING_INDEX}.py)
endforeach(STRING_INDEX ${python_STRING_INDICES})

# This list contains standalone scripts which are executable and which
# should be installed in the examples/python directory.
set(
python_SCRIPTS
${python_SCRIPTS}
prova.py
pythondemos.py
x14
x17
xw14.py
xw17.py
)
if(ENABLE_pygcw)
  set(
  python_SCRIPTS 
  ${python_SCRIPTS}
  plplotcanvas_demo.py
  plplotcanvas_animation.py
 )
endif(ENABLE_pygcw)

set(PERM_SCRIPTS
OWNER_READ
OWNER_WRITE
OWNER_EXECUTE
GROUP_READ
GROUP_EXECUTE
WORLD_READ
WORLD_EXECUTE
)
install(FILES ${python_SCRIPTS} 
DESTINATION ${DATA_DIR}/examples/python
PERMISSIONS ${PERM_SCRIPTS}
)

# This list contains python modules which are not executable on their own and
# data files that should be installed in the examples/python directory.
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plplot_python_start.py.in
${CMAKE_CURRENT_BINARY_DIR}/plplot_python_start.py
)

set(
python_DATA
${python_DATA}
README.pythondemos
qplplot.py
${CMAKE_CURRENT_BINARY_DIR}/plplot_python_start.py
)

if(ENABLE_pygcw)
  set(
  python_DATA
  ${python_DATA}
  README.plplotcanvas
  )
endif(ENABLE_pygcw)

set(PERM_DATA
OWNER_READ
OWNER_WRITE
GROUP_READ
WORLD_READ
)
install(FILES ${python_DATA} 
DESTINATION ${DATA_DIR}/examples/python
PERMISSIONS ${PERM_DATA}
)
