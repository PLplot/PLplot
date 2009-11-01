# bindings/CMakeLists.txt for PLplot
###
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

add_subdirectory(c++)
add_subdirectory(f77)
add_subdirectory(f95)
add_subdirectory(tcl)
add_subdirectory(tk)
add_subdirectory(tk-x-plat)
add_subdirectory(python)
add_subdirectory(octave)
add_subdirectory(java)
add_subdirectory(gnome2)
add_subdirectory(wxwidgets)
add_subdirectory(ada)
add_subdirectory(d)
add_subdirectory(ocaml)
add_subdirectory(lua)
add_subdirectory(qt_gui)

set(index)
if(ENABLE_tcl)
  set(index ${index} ${CMAKE_CURRENT_BINARY_DIR}/tcl/pkgIndex.tcl)
endif(ENABLE_tcl)
if(ENABLE_tk)
  set(index ${index} ${CMAKE_CURRENT_BINARY_DIR}/tk/pkgIndex.tcl)
endif(ENABLE_tk)
if(PLD_tkwin)
  set(index ${index} ${CMAKE_CURRENT_BINARY_DIR}/tk-x-plat/pkgIndex.tcl)
endif(PLD_tkwin)

if(index)
  find_program(CAT NAMES cat)
  if(CAT)
      add_custom_target(gen_pkgIndex.tcl ALL
      COMMAND ${CAT} ${index} > pkgIndex.tcl
      DEPENDS ${index}
      )
  else(CAT)
    find_program(COMMAND_ENVIRONMENT NAMES cmd command)
    if(COMMAND_ENVIRONMENT)
      set(COPY "copy"  CACHE INTERNAL "program to concatenate files" FORCE)
      message(STATUS "Copy: ${COPY} - ${COMMAND_ENVIRONMENT}")
      add_custom_target(gen_pkgIndex.tcl ALL
      COMMAND ${COPY} ${index} pkgIndex.tcl
      DEPENDS ${index}
      )
    else(COMMAND_ENVIRONMENT)
      message(FATAL_ERROR "program to concatenate files not found")
    endif(COMMAND_ENVIRONMENT)
  endif(CAT)
  install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl
  DESTINATION ${DATA_DIR}
  )
endif(index)
