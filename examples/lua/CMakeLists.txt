# examples/lua/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2009 Alan W. Irwin
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

set(lua_STRING_INDICES
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
"17"
"18"
"19"
"22"
"24"
"25"
"30"
)

set(lua_SCRIPTS)
foreach(STRING_INDEX ${lua_STRING_INDICES})
  set(lua_SCRIPTS ${lua_SCRIPTS} x${STRING_INDEX}.lua)
endforeach(STRING_INDEX ${lua_STRING_INDICES})

set(PERM_SCRIPTS
OWNER_READ
OWNER_WRITE
OWNER_EXECUTE
GROUP_READ
GROUP_EXECUTE
WORLD_READ
WORLD_EXECUTE
)

install(FILES ${lua_SCRIPTS} 
DESTINATION ${DATA_DIR}/examples/lua
PERMISSIONS ${PERM_SCRIPTS}
)

if(BUILD_TEST AND
NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}"
)
  # equivalent to install commands but at "make" time rather than
  # "make install" time, to the build tree if different than the source
  # tree.
  set(command_depends)
  foreach(file ${lua_SCRIPTS})
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
  endforeach(file ${lua_SCRIPTS} ${lua_DATA})
  add_custom_target(lua_examples ALL DEPENDS ${command_DEPENDS})
endif(BUILD_TEST AND 
NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}"
)

set(lua_lib_location ${CMAKE_BINARY_DIR}/bindings/lua)
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plplot_examples.lua.in
${CMAKE_CURRENT_BINARY_DIR}/plplot_examples.lua
@ONLY
)

set(lua_lib_location ${LUA_DIR})
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plplot_examples.lua.in
${CMAKE_CURRENT_BINARY_DIR}/plplot_examples.lua_install
@ONLY
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot_examples.lua_install
DESTINATION ${DATA_DIR}/examples/lua
PERMISSIONS ${PERM_DATA}
RENAME plplot_examples.lua
)
