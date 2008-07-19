# -*- mode: cmake -*-
# bindings/ocaml/CMakeLists.txt
### Process this file with cmake to produce Makefile
#
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
# Configure pkg-config *.pc file corresponding to plplot.cma
if(PKG_CONFIG_EXECUTABLE)
  if(LIB_TAG)
    set(PC_PRECISION "double")
  else(LIB_TAG)
    set(PC_PRECISION "single")
  endif(LIB_TAG)
  # Each list element must consist of a colon-separated string with the
  # following fields which are parsed out in the foreach loop below and
  # used to configure the corresponding pkg-config *.pc file.
  # BINDING 	      	  - ENABLE_${BINDING} keeps track of whether a
  # 			    binding has been enabled (ON) or not (OFF).
  #			    Also, ${BINDING} used to determine PC_FILE_SUFFIX
  #			    which helps to determine name of configured 
  #			    *.pc file.
  # PC_SHORT_NAME	  - Used in *.pc NAME: field
  # PC_LONG_NAME	  - Used in *.pc Description: field
  # PC_LIBRARY_NAME	  - Used in *.pc Libs: field
  #                         Also used to determine PC_LINK_FLAGS and
  #                         PC_COMPILE_FLAGS used in *.pc Libs: and Cflags:
  #			    fields.
  set(PC_DATA "ocaml:::plplot${LIB_TAG}")

  string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
  set(PC_FILE_SUFFIX "-${BINDING}")
  set(PC_REQUIRES "plplot${LIB_TAG}")
  string(REGEX REPLACE "^.*:(.*):.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:(.*):.*$" "\\1" PC_LONG_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:.*:(.*)$" "\\1" PC_LIBRARY_NAME ${PC_DATA})
  set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
  set(PC_COMPILE_FLAGS "${lib${PC_LIBRARY_NAME}_COMPILE_FLAGS}")
  set(PC_LINK_FLAGS "-l${PC_LIBRARY_NAME} ${PC_LINK_FLAGS}")
  set(PC_CONFIGURED_FILE
  ${CMAKE_BINARY_DIR}/pkgcfg/plplot${LIB_TAG}${PC_FILE_SUFFIX}.pc
  )
  configure_file(
  ${CMAKE_SOURCE_DIR}/pkgcfg/plplot-template.pc.cmake
  ${PC_CONFIGURED_FILE}
  @ONLY
  )
  install(FILES ${PC_CONFIGURED_FILE} DESTINATION ${PKG_CONFIG_DIR})
  configure_file(META.cmake ${CMAKE_CURRENT_BINARY_DIR}/META)
endif(PKG_CONFIG_EXECUTABLE)

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
    COMMAND ${OCAMLBUILD} -tag debug -classic-display
    -build-dir ${OCAML_BUILD_DIR} plplot.cma
    DEPENDS ${DEPENDS_LIST} plplot${LIB_TAG}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  
  # Build native version.  Use DEPENDS to serialize against plplot.cma
  # since the two ocamlbuilds interfere with each other when in parallel
  # build mode.
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cmxa 
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.a
    COMMAND ${OCAMLBUILD} -tag debug -classic-display
    -build-dir ${OCAML_BUILD_DIR} plplot.cmxa
    DEPENDS ${DEPENDS_LIST} plplot${LIB_TAG}
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cma
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

  # This does not work as the clean rule will not remove directories which
  # are not empty
  set_directory_properties(PROPERTIES 
    ADDITIONAL_MAKE_CLEAN_FILES 
    ${OCAML_BUILD_DIR}
  )

endif(ENABLE_ocaml)
