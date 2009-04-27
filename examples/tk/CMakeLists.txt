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

set(tk_FILES)

set(tk_SRC_FILES
  README.tkdemos
  runAllDemos.tcl
  runExtendedDemos.tcl
  tkdemos.tcl
  tk01
  tk02
  tk03
  tk04
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
  "20"
  "21"
  "22"
  "23"
  "24"
  "25"
  "26"
  "27"
  "28"
  "29"
  "30"
  )

# Copy files to the binary directory (if different) for generating tclIndex
# This ensures no files are created in the source tree.
if(NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
  foreach(_file ${tk_SRC_FILES})
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file}
      COMMAND ${CMAKE_COMMAND}
      -E copy
      ${CMAKE_CURRENT_SOURCE_DIR}/${_file}
      ${CMAKE_CURRENT_BINARY_DIR}/${_file}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_file}
      )
    set(tk_FILES ${tk_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${_file})
  endforeach(_file ${tk_SRC_FILES})
endif(NOT CMAKE_CURRENT_BINARY_DIR STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")  

foreach(STRING_INDEX ${tk_STRING_INDICES})
  set(_file x${STRING_INDEX}.tcl)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file}
    COMMAND ${CMAKE_COMMAND}
    -E copy
    ${CMAKE_SOURCE_DIR}/examples/tcl/${_file}
    ${CMAKE_CURRENT_BINARY_DIR}/${_file}
    DEPENDS ${CMAKE_SOURCE_DIR}/examples/tcl/${_file}
    )
  set(tk_FILES ${tk_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${_file})
endforeach(STRING_INDEX ${tk_STRING_INDICES})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plgrid.tcl
  COMMAND ${CMAKE_COMMAND}
  -E copy
  ${CMAKE_SOURCE_DIR}/examples/tcl/plgrid.tcl
  ${CMAKE_CURRENT_BINARY_DIR}/plgrid.tcl
  DEPENDS ${CMAKE_SOURCE_DIR}/examples/tcl/plgrid.tcl
  )
set(tk_FILES ${tk_FILES} ${CMAKE_CURRENT_BINARY_DIR}/plgrid.tcl)

install(FILES ${tk_FILES} DESTINATION ${DATA_DIR}/examples/tk)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
  COMMAND ${TCL_TCLSH} ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
  DEPENDS ${tk_FILES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tclIndex DESTINATION ${DATA_DIR}/examples/tk)

# The second and fourth Tk examples depend on Itk to work.
set(tk_SCRIPTS tk01 tk03)
set(tk_SRC xtk01.c)
  
if(ENABLE_itk)
  list(APPEND tk_SCRIPTS tk02 tk04)
  list(APPEND tk_SRC xtk02.c xtk04.c)
endif(ENABLE_itk)

if(BUILD_TEST)
  remove_definitions("-DHAVE_CONFIG_H")
  include_directories(
    ${TCL_INCLUDE_PATH}
    ${TK_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/bindings/tcl
    ${CMAKE_SOURCE_DIR}/bindings/tk
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_BINARY_DIR}/include
    )
  foreach(TK_SRC_FILE ${tk_SRC})
   string(REGEX REPLACE ".c$" "" TK_EXE ${TK_SRC_FILE})
   add_executable(${TK_EXE} ${TK_SRC_FILE})
    if(BUILD_SHARED_LIBS)
      SET_SOURCE_FILES_PROPERTIES(${TK_SRC_FILE}
        PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
    endif(BUILD_SHARED_LIBS)
    target_link_libraries(${TK_EXE} plplottcltk${LIB_TAG} tclmatrix${LIB_TAG} plplot${LIB_TAG} ${MATH_LIB})
  endforeach(TK_SRC_FILE ${tk_SRC})
endif(BUILD_TEST)

install(PROGRAMS ${tk_SCRIPTS} DESTINATION ${DATA_DIR}/examples/tk)

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

add_custom_target(tclIndex_examples_tk ALL
  DEPENDS ${tk_FILES} ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
  )
