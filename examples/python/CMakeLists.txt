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
set(python_SCRIPTS_CREATE)
set(python_DATA)
set(python_DATA_CREATE)
foreach(STRING_INDEX ${python_STRING_INDICES})
  configure_file(xw${STRING_INDEX}.py.in ${CMAKE_CURRENT_BINARY_DIR}/xw${STRING_INDEX}.py)
  set(python_SCRIPTS ${python_SCRIPTS} x${STRING_INDEX})
  set(python_DATA_CREATE ${python_DATA_CREATE} ${CMAKE_CURRENT_BINARY_DIR}/xw${STRING_INDEX}.py)
endforeach(STRING_INDEX ${python_STRING_INDICES})

configure_file(xw14.py.in ${CMAKE_CURRENT_BINARY_DIR}/xw14.py)
configure_file(xw17.py.in ${CMAKE_CURRENT_BINARY_DIR}/xw17.py)
configure_file(prova.py.in ${CMAKE_CURRENT_BINARY_DIR}/prova.py)
configure_file(qplplot.py.in ${CMAKE_CURRENT_BINARY_DIR}/qplplot.py)
if (ENABLE_pygcw)
  configure_file(plplotcanvas_demo.py.in ${CMAKE_CURRENT_BINARY_DIR}/plplotcanvas_demo.py)
  configure_file(plplotcanvas_animation.py.in ${CMAKE_CURRENT_BINARY_DIR}/plplotcanvas_animation.py)
endif (ENABLE_pygcw)

# This list contains standalone scripts which are executable and which
# should be installed in the examples/python directory.
set(
python_SCRIPTS
${python_SCRIPTS}
pythondemos.py
x14
x17
)
set(
python_SCRIPTS_CREATE
${python_SCRIPTS_CREATE}
${CMAKE_CURRENT_BINARY_DIR}/prova.py
${CMAKE_CURRENT_BINARY_DIR}/xw14.py
${CMAKE_CURRENT_BINARY_DIR}/xw17.py
)
if(ENABLE_pygcw)
  set(
  python_SCRIPTS_CREATE
  ${python_SCRIPTS_CREATE}
  ${CMAKE_CURRENT_BINARY_DIR}/plplotcanvas_demo.py
  ${CMAKE_CURRENT_BINARY_DIR}/plplotcanvas_animation.py
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
install(FILES ${python_SCRIPTS_CREATE} 
DESTINATION ${DATA_DIR}/examples/python
PERMISSIONS ${PERM_SCRIPTS}
)

# This list contains python modules which are not executable on their own and
# data files that should be installed in the examples/python directory.

set(
python_DATA
${python_DATA}
README.pythondemos
)
set(
python_DATA_CREATE
${python_DATA_CREATE}
${CMAKE_CURRENT_BINARY_DIR}/qplplot.py
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
install(FILES ${python_DATA_CREATE} 
DESTINATION ${DATA_DIR}/examples/python
PERMISSIONS ${PERM_DATA}
)

if(BUILD_TEST AND
NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}"
)
  # equivalent to install commands but at "make" time rather than
  # "make install" time, to the build tree if different than the source
  # tree, and skipping plplot_python_start.py since it is already
  # in the build tree.
  set(command_depends)
  foreach(file ${python_SCRIPTS} ${python_DATA})
    set(
    command_DEPENDS
    ${command_DEPENDS}
    ${CMAKE_CURRENT_BINARY_DIR}/${file}
    )
    add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${file}
    COMMAND ${CMAKE_COMMAND} -E copy 
    ${CMAKE_CURRENT_SOURCE_DIR}/${file} ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${file}
    )
  endforeach(file ${python_SCRIPTS} ${python_DATA})
  add_custom_target(python_examples ALL DEPENDS ${command_DEPENDS})
endif(BUILD_TEST AND 
NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(python_location1 ${CMAKE_SOURCE_DIR}/bindings/python)
set(python_location2 ${CMAKE_BINARY_DIR}/bindings/python)
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plplot_python_start.py.cmake
${CMAKE_CURRENT_BINARY_DIR}/plplot_python_start.py
@ONLY
)

set(python_location1 ${PYTHON_INSTDIR})
set(python_location2)
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plplot_python_start.py.cmake
${CMAKE_CURRENT_BINARY_DIR}/plplot_python_start.py_install
@ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot_python_start.py_install
DESTINATION ${DATA_DIR}/examples/python
PERMISSIONS ${PERM_DATA}
RENAME plplot_python_start.py
)
