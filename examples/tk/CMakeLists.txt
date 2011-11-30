# examples/tk/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006, 2007, 2008, 2009, 2010 Alan W. Irwin
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

# N.B. This file is used for both the core build (which installs the examples
# and optionally [depending on BUILD_TEST} builds them) and the installed
# examples build.  The core build has BUILD_TEST OFF or ON at user option
# and CORE_BUILD always ON.  The installed examples build always has
# BUILD_TEST ON and CORE_BUILD OFF.

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
  "31"
  )

set(tk_SRC_FILES
  README.tkdemos
  runAllDemos.tcl
  runExtendedDemos.tcl
  tkdemos.tcl
  )

# Copy files to the binary directory (if different) for generating tclIndex
# This ensures no files are created in the source tree.
set(tk_FILES)
foreach(_file ${tk_SRC_FILES})
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file}
    COMMAND ${CMAKE_COMMAND}
    -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/${_file}
    ${CMAKE_CURRENT_BINARY_DIR}/${_file}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${_file}
    )
  set(tk_FILES ${tk_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${_file})
endforeach(_file ${tk_SRC_FILES})

if(CORE_BUILD)
  set(tcl_EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/examples/tcl)
else(CORE_BUILD)
  set(tcl_EXAMPLES_DIR ${CMAKE_SOURCE_DIR}/tcl)
endif(CORE_BUILD)

foreach(STRING_INDEX ${tk_STRING_INDICES})
  set(_file x${STRING_INDEX}.tcl)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_file}
    COMMAND ${CMAKE_COMMAND}
    -E copy_if_different
    ${tcl_EXAMPLES_DIR}/${_file}
    ${CMAKE_CURRENT_BINARY_DIR}/${_file}
    DEPENDS ${tcl_EXAMPLES_DIR}/${_file}
    )
  set(tk_FILES ${tk_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${_file})
endforeach(STRING_INDEX ${tk_STRING_INDICES})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plgrid.tcl
  COMMAND ${CMAKE_COMMAND}
  -E copy_if_different
  ${tcl_EXAMPLES_DIR}/plgrid.tcl
  ${CMAKE_CURRENT_BINARY_DIR}/plgrid.tcl
  DEPENDS ${tcl_EXAMPLES_DIR}/plgrid.tcl
  )
set(tk_FILES ${tk_FILES} ${CMAKE_CURRENT_BINARY_DIR}/plgrid.tcl)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
  COMMAND ${TCL_TCLSH} ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
  DEPENDS ${tk_FILES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )

add_custom_target(tclIndex_examples_tk ALL
  DEPENDS ${tk_FILES} ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
  )

# The third example is run using a configured shell script which invokes
# plserver so there is no source code to be compiled.
set(tk_SRC xtk01.c)
# The second and fourth Tk examples depend on Itk to work.
if(ENABLE_itk)
  list(APPEND tk_SRC xtk02.c xtk04.c)
endif(ENABLE_itk)

# The first, second, and fourth tk examples depend on configured bang
# scripts to work which depends directly on the executables, xtk01,
# xtk02, and xtk04.

# The third example is run with the configured shell script, tk03, which
# invokes plserver.

# plgrid is a configured shell script that runs the plgrid.tcl example
# (N.B., in the tcl directory) using plserver.

# standard_examples is a configured shell script that runs all the standard
# examples under plserver.
set(tk_SCRIPTS tk01 tk03 plgrid standard_examples)
# The second and fourth Tk examples depend on Itk to work.
if(ENABLE_itk)
  list(APPEND tk_SCRIPTS tk02 tk04)
endif(ENABLE_itk)

if(BUILD_TEST)
  if(CORE_BUILD)
    include_directories(
      ${TCL_INCLUDE_PATH}
      ${TK_INCLUDE_PATH}
      ${CMAKE_SOURCE_DIR}/bindings/tcl
      ${CMAKE_SOURCE_DIR}/bindings/tk
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_BINARY_DIR}/include
      )
  else(CORE_BUILD)
    include_directories(
      ${TCL_INCLUDE_PATH}
      ${TK_INCLUDE_PATH}
      ${INCLUDE_DIR}
      )
  endif(CORE_BUILD)

  set_property(GLOBAL PROPERTY TARGETS_examples_tk)
  foreach(TK_SRC_FILE ${tk_SRC})
    string(REGEX REPLACE ".c$" "" TK_EXE ${TK_SRC_FILE})
    add_executable(${TK_EXE} ${TK_SRC_FILE})
    if(BUILD_SHARED_LIBS AND CORE_BUILD)
      set_source_files_properties(${TK_SRC_FILE}
        PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
    endif(BUILD_SHARED_LIBS AND CORE_BUILD)
    target_link_libraries(${TK_EXE} plplottcltk${LIB_TAG} tclmatrix${LIB_TAG} plplot${LIB_TAG} ${TK_LIBRARY} ${TCL_LIBRARY} ${MATH_LIB})
    set_property(GLOBAL APPEND PROPERTY TARGETS_examples_tk ${TK_EXE})
  endforeach(TK_SRC_FILE ${tk_SRC})
  set_property(GLOBAL APPEND PROPERTY TARGETS_examples_tk tclIndex_examples_tk)

  # Provide locations for several tk-related targets that will be used
  # to configure the tk_SCRIPTS.
  foreach(tk_related_target xtk01 plserver xtk02 xtk04)
    if(TARGET ${tk_related_target})
      get_target_property(${tk_related_target}_LOCATION
	${tk_related_target} LOCATION
	)
    endif(TARGET ${tk_related_target})
  endforeach(tk_related_target xtk01 plserver xtk02 xtk04)

  # Configure the tk_SCRIPTS
  foreach(tk_SCRIPT ${tk_SCRIPTS})
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/${tk_SCRIPT}.in 
      ${CMAKE_CURRENT_BINARY_DIR}/${tk_SCRIPT}
      @ONLY
      )
  endforeach(tk_SCRIPT ${tk_SCRIPTS})
endif(BUILD_TEST)

if(CORE_BUILD)
  # Provide install locations for several tk-related targets that will
  # be used to configure the tk_SCRIPTS used in the traditional
  # Makefile+pkg-config build system for the installed examples.
  foreach(tk_related_target xtk01 plserver xtk02 xtk04)
    if(tk_related_target STREQUAL "plserver")
      set(install_dir ${BIN_DIR})
    else(tk_related_target STREQUAL "plserver")
      set(install_dir ${DATA_DIR}/examples/tk)
    endif(tk_related_target STREQUAL "plserver")
    set(${tk_related_target}_LOCATION 
      ${install_dir}/${tk_related_target}${EXEEXT}
      )
  endforeach(tk_related_target xtk01 plserver xtk02 xtk04)

  foreach(tk_SCRIPT ${tk_SCRIPTS})
    # Configure and install the tk_SCRIPTS used for the traditional build.
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/${tk_SCRIPT}.in 
      ${CMAKE_CURRENT_BINARY_DIR}/install_${tk_SCRIPT}
      @ONLY
      )
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/install_${tk_SCRIPT}
      DESTINATION ${DATA_DIR}/examples/tk
      RENAME ${tk_SCRIPT}
      )
    # install the template files used to configure tk_SCRIPTS for the
    # new installed examples build system.
    install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/${tk_SCRIPT}.in
      DESTINATION ${DATA_DIR}/examples/tk
      )
  endforeach(tk_SCRIPT ${tk_SCRIPTS})
endif(CORE_BUILD)

if(CORE_BUILD)
  install(FILES ${tk_FILES} DESTINATION ${DATA_DIR}/examples/tk)

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tclIndex DESTINATION ${DATA_DIR}/examples/tk)

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

  install(FILES CMakeLists.txt
    DESTINATION ${DATA_DIR}/examples/tk
    )
endif(CORE_BUILD)
