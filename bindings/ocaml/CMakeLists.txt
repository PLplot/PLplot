# bindings/ocaml/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
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

if(ENABLE_ocaml)

  set(SOURCE_LIST
    extras.ml
    libplplot_stubs.clib
    plplot_h
    plplot_h.inc
    plplot.idl
    plplot_impl.c
    touchup.ml
    )
  set(GEN_SOURCE_LIST
    ${CMAKE_CURRENT_BINARY_DIR}/myocamlbuild.ml
    )
  
  set(OCAML_INSTALL_FILES
    plplot.cma
    plplot.cmxa
    plplot.a
    plplot.cmi
    libplplot_stubs.a
    )
  
  set(OCAML_FULL_INSTALL_FILES)
  foreach(OCAML_FILE ${OCAML_INSTALL_FILES})
    set(OCAML_FULL_INSTALL_FILES ${OCAML_FULL_INSTALL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/${OCAML_FILE})
  endforeach(OCAML_FILE ${OCAML_INSTALL_FILES})
    
  # Configure the META file 
  configure_file(META.cmake ${CMAKE_CURRENT_BINARY_DIR}/META)
  
  # Configure the file controlling the ocamlbuild process
  configure_file(myocamlbuild.ml.cmake ${CMAKE_CURRENT_BINARY_DIR}/myocamlbuild.ml)
  set(DEPENDS_LIST ${GEN_SOURCE_LIST})

  # Copy all source into the build tree since the whole project must be in
  # the same directory
  foreach(SRC_FILE ${SOURCE_LIST})
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SRC_FILE}
      COMMAND ${CMAKE_COMMAND}
      -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${SRC_FILE}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_FILE}
      )
    set(DEPENDS_LIST ${DEPENDS_LIST} ${CMAKE_CURRENT_BINARY_DIR}/${SRC_FILE})
  endforeach(SRC_FILE ${SOURCE_LIST})
  
  # Note: ocamlbuild requires build-dir to be a relative path
  # Also the build-dir must not be the project root - hence the 
  # new subdirectory
  # Build bytecode version
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cma 
    COMMAND ${OCAMLBUILD} -tag debug -classic-display -build-dir ${OCAML_BUILD_DIR} plplot.cma
    DEPENDS ${DEPENDS_LIST}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  
  # Build native version
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cmxa 
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.a
    COMMAND ocamlbuild -tag debug -build-dir ${OCAML_BUILD_DIR}  plplot.cmxa
    DEPENDS ${DEPENDS_LIST}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

  add_custom_target(
    plplot_ocaml ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cma ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cmxa ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.a
    )
  
  # Most files go in the plplot subdirectory
  install(FILES ${OCAML_FULL_INSTALL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/META
    DESTINATION ${OCAML_INSTALL_DIR}/plplot
    )
  
  # Shared library stubs go in stublibs
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/dllplplot_stubs.so
    DESTINATION ${OCAML_INSTALL_DIR}/stublibs
    )

endif(ENABLE_ocaml)
