# -*- mode: cmake -*-
# examples/ocaml/CMakeLists.txt
### Process this file with cmake to produce Makefile
#
#
# Copyright (C) 2008 Andrew Ross
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

# N.B. This file is used for both the core build (which installs the examples
# and optionally [depending on BUILD_TEST} builds them) and the installed
# examples build.  The core build has BUILD_TEST OFF or ON at user option
# and CORE_BUILD always ON.  The installed examples build always has
# BUILD_TEST ON and CORE_BUILD OFF.

set(ocaml_STRING_INDICES
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
  "plot01"
  )

if(CORE_BUILD)
  set(ocaml_SRCS)
  foreach(STRING_INDEX ${ocaml_STRING_INDICES})
    set(SOURCE_FILE x${STRING_INDEX}.ml)
    set(ocaml_SRCS ${ocaml_SRCS} ${SOURCE_FILE})
  endforeach(STRING_INDEX ${ocaml_STRING_INDICES})

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
    ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    )

  install(FILES ${ocaml_SRCS}
    DESTINATION ${DATA_DIR}/examples/ocaml
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    DESTINATION ${DATA_DIR}/examples/ocaml
    RENAME Makefile
    )

  install(FILES CMakeLists.txt
    DESTINATION ${DATA_DIR}/examples/ocaml
    )

endif(CORE_BUILD)

if(BUILD_TEST)
  set(ocaml_EXTRA_CLEAN_FILES)
  if(CORE_BUILD)
    set(I_OPTION ${CMAKE_BINARY_DIR}/bindings/ocaml/${OCAML_BUILD_DIR})
    set(ccopt_OPTION "-L ${CMAKE_BINARY_DIR}/src -Wl,-rpath -Wl,${CMAKE_BINARY_DIR}/src ")
  else(CORE_BUILD)
    set(I_OPTION ${OCAML_INSTALL_DIR}/plplot)
    set(ccopt_OPTION "-L ${CMAKE_INSTALL_LIBDIR} -Wl,-rpath -Wl,${CMAKE_INSTALL_LIBDIR} ")
  endif(CORE_BUILD)

  set_property(GLOBAL PROPERTY TARGETS_examples_ocaml)
  foreach(STRING_INDEX ${ocaml_STRING_INDICES})
    set(EXECUTABLE_NAME x${STRING_INDEX}ocaml)
    set(SOURCE_FILE x${STRING_INDEX}.ml)
    set(ocaml_EXTRA_CLEAN_FILES ${ocaml_EXTRA_CLEAN_FILES} x${STRING_INDEX}.cmo x${STRING_INDEX}.cmi)
    # Copy source code to build tree since ocamlc is a bit broken and will 
    # otherwise litter the source tree with intermediate files
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
      COMMAND ${CMAKE_COMMAND}
      -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}
      )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}
      COMMAND ${OCAMLC} 
      -g
      -I ${I_OPTION}
      -ccopt ${ccopt_OPTION}
      plplot.cma 
      -o ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME} 
      ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      VERBATIM
      )
    add_custom_target(
      target_${EXECUTABLE_NAME} ALL 
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE_NAME}
      )
    if(CORE_BUILD)
      add_dependencies(target_${EXECUTABLE_NAME} plplot_ocaml)
    endif(CORE_BUILD)
    set_property(GLOBAL APPEND PROPERTY TARGETS_examples_ocaml target_${EXECUTABLE_NAME})
  endforeach(STRING_INDEX ${ocaml_STRING_INDICES})
  set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ocaml_EXTRA_CLEAN_FILES}")
endif(BUILD_TEST)
