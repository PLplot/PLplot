# bindings/lua/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2008  Werner Smekal
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

if(ENABLE_lua)

# This is currently the include list for swig.
set(lua_interface_INCLUDE_PATHS
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
${CMAKE_CURRENT_BINARY_DIR}
${LUA_INCLUDE_DIR}
${CMAKE_SOURCE_DIR}/bindings/swig-support
)
include_directories(${lua_interface_INCLUDE_PATHS})

set(CMAKE_SWIG_FLAGS -DSWIG_LUA)
set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})

set_source_files_properties(plplotluac.i 
PROPERTIES SWIG_MODULE_NAME plplotluac
)

set(SWIG_MODULE_plplotluac_EXTRA_DEPS
${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i)

# Set up swig + c wrapper.
swig_add_module(plplotluac lua plplotluac.i)
swig_link_libraries(plplotluac plplot${LIB_TAG} ${LUA_LIBRARIES})

endif(ENABLE_lua)
