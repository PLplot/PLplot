# examples/ocaml/CMakeLists.txt
### Process this file with cmake to produce Makefile
#
# -*- cmake-mode -*-
#
# Copyright (C) 2008 Andrew Ross
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

set(ocaml_STRING_INDICES
  "11"
  "19"
  )

set(ocaml_SRCS)
set(ocaml_EXTRA_CLEAN_FILES)
foreach(STRING_INDEX ${ocaml_STRING_INDICES})
  set(TARGET_NAME x${STRING_INDEX}ocaml)
  set(SOURCE_FILE x${STRING_INDEX}.ml)
  set(ocaml_SRCS ${ocaml_SRCS} ${SOURCE_FILE})
  set(ocaml_EXTRA_CLEAN_FILES ${ocaml_EXTRA_CLEAN_FILES} x${STRING_INDEX}.cmo x${STRING_INDEX}.cmi)
  if(BUILD_TEST)
    # Copy source code to build tree since ocamlc is a bit broken and will 
    # otherwise litter the source tree with intermediate files
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
      COMMAND ${CMAKE_COMMAND}
      -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}
      )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}
      COMMAND ${OCAMLC} -I ${CMAKE_BINARY_DIR}/bindings/ocaml/${OCAML_BUILD_DIR} -ccopt "-L ${CMAKE_BINARY_DIR}/src" plplot.cma -o ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME} ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(
      ${TARGET_NAME} ALL 
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME}
      )
  endif(BUILD_TEST)
endforeach(STRING_INDEX ${ocaml_STRING_INDICES})

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ocaml_EXTRA_CLEAN_FILES}")

install(FILES ${ocaml_SRCS}
  DESTINATION ${DATA_DIR}/examples/ocaml
  )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
  DESTINATION ${DATA_DIR}/examples/ocaml
  RENAME Makefile
  )
