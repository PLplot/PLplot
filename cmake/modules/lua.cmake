# cmake/modules/lua.cmake
#
# LUA binding configuration
#
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
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Module for determining LUA bindings configuration options

# Options to enable LUA bindings
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_lua "Enable LUA bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_lua "Enable LUA bindings" OFF)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_lua AND NOT BUILD_SHARED_LIBS)
  message(STATUS "WARNING: "
    "Lua requires shared libraries. Disabling lua bindings")
  set(ENABLE_lua OFF CACHE BOOL "Enable LUA bindings" FORCE)
endif(ENABLE_lua AND NOT BUILD_SHARED_LIBS)

if(ENABLE_lua AND NOT SWIG_FOUND)
  message(STATUS "WARNING: "
    "swig not found. Disabling LUA bindings")
  set(ENABLE_lua OFF CACHE BOOL "Enable LUA bindings" FORCE)
endif(ENABLE_lua AND NOT SWIG_FOUND)

if(ENABLE_lua)
  # Check for Lua libraries which defines
  #  LUA_LIBRARIES    = path to the lua library
  #  LUA_INCLUDE_DIR  = path to where lua.h is found
  find_package(Lua50)
  if(NOT LUA50_FOUND)
    message(STATUS "WARNING: "
      "lua library and/or header not found. Disabling lua bindings")
    set(ENABLE_lua OFF CACHE BOOL "Enable LUA bindings" FORCE)
  endif(NOT LUA50_FOUND)
endif(ENABLE_lua)
