# -*- mode: cmake -*-
# bindings/ocaml/CMakeLists.txt
### Process this file with cmake to produce Makefile
#
#
# Copyright (C) 2008 Andrew Ross
# Copyright (C) 2009 Hezekiah M. Carty
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

if(ENABLE_ocaml)

  # Optionally build the Plcairo module
  add_subdirectory(plcairo)

  # optional command to generate generated_plplot_h.inc.  This generated
  # version of the file should be identical to plplot_h.inc.
  if(GENERATE_PLPLOT_H_INC)
    add_custom_command(
      OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/generated_plplot_h.inc
      COMMAND ${OCAML} ${CMAKE_CURRENT_SOURCE_DIR}/touchup.ml ${CMAKE_CURRENT_SOURCE_DIR}/plplot_h ${CMAKE_CURRENT_BINARY_DIR}/generated_plplot_h.inc
      DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/touchup.ml
      ${CMAKE_CURRENT_SOURCE_DIR}/plplot_h
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(generate_plplot_h.inc
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/generated_plplot_h.inc
      )
  endif(GENERATE_PLPLOT_H_INC)

  #Detailed CMake logic to build ocaml bindings for PLplot.
  set(camlidl_GENERATED_SOURCE
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.h
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.ml
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.mli
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.c
    )

  # camlidl produces most of the C and OCaml code required to bind PLplot
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.idl
    ${camlidl_GENERATED_SOURCE}
    # camlidl source file must be in ${CMAKE_CURRENT_BINARY_DIR}.
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plplot_core.idl ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.idl
    COMMAND ${CAMLIDL} -header -I ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.idl
    DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/plplot_core.idl
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

  # ocamlc -c compiles *.c into *.o.
  # ocamlmklib links *.o into *.so and *.a
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.o
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_impl.o
    ${CMAKE_CURRENT_BINARY_DIR}/dllplplot_stubs.so
    ${CMAKE_CURRENT_BINARY_DIR}/libplplot_stubs.a
    COMMAND ${OCAMLC} -c ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.c
    COMMAND ${OCAMLC} -ccopt -I${CMAKE_SOURCE_DIR}/include -ccopt -I${CMAKE_BINARY_DIR}/include -ccopt -I${CMAKE_SOURCE_DIR}/lib/qsastime  -c ${CMAKE_CURRENT_SOURCE_DIR}/plplot_impl.c
    COMMAND ${OCAMLMKLIB} -o plplot_stubs -L${CAMLIDL_LIB_DIR} -lcamlidl -L${CMAKE_BINARY_DIR}/src -lplplot${LIB_TAG} ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.o ${CMAKE_CURRENT_BINARY_DIR}/plplot_impl.o
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/plplot_impl.c
    plplot${LIB_TAG}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(target_lib_plplot_stubs
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/dllplplot_stubs.so
    ${CMAKE_CURRENT_BINARY_DIR}/libplplot_stubs.a
    )

  # ocamlc -c compiles *.mli into *.cmi
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmi
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.mli
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmi
    COMMAND ${OCAMLC} -c ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.mli
    # ocamlc *.mli source file must be in ${CMAKE_CURRENT_BINARY_DIR}.
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plplot.mli ${CMAKE_CURRENT_BINARY_DIR}/plplot.mli
    COMMAND ${OCAMLC} -c ${CMAKE_CURRENT_BINARY_DIR}/plplot.mli
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.mli
    ${CMAKE_CURRENT_SOURCE_DIR}/plplot.mli
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(target_plplot_cmi
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmi
    )

  # ocamlc -c compiles  *.ml into *.cmo and simultaneously checks against
  # *.cmi produced from *.mli above.
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmo
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmo
    COMMAND ${OCAMLC} -c ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.ml
    # ocamlc and ocamlopt *.ml source file must be in
    # ${CMAKE_CURRENT_BINARY_DIR}.
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plplot.ml ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
    COMMAND ${OCAMLC} -c ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.ml
    ${CMAKE_CURRENT_SOURCE_DIR}/plplot.ml
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmi
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmi
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(target_plplot_cmo
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmo
    )

  # ocamlc -a -custom builds a *.cma library from *.cmo
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma
    COMMAND ${OCAMLC} -a -custom -o ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmo ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmo -dllib -lplplot_stubs -ccopt -L${CMAKE_CURRENT_BINARY_DIR} -cclib -lplplot_stubs -ccopt -L${CAMLIDL_LIB_DIR} -cclib -lcamlidl -ccopt -L${CMAKE_BINARY_DIR}/src -cclib -lplplot${LIB_TAG} -dllpath ${CMAKE_BINARY_DIR}/src
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmo
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmo
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(target_plplot_cma
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma
    )

  # These targets depend on common files in their respective
  # file-dependency chains.  Therefore, to avoid screwing up parallel
  # builds must serialize with target depends.
  add_dependencies(target_plplot_cmi target_lib_plplot_stubs)
  add_dependencies(target_plplot_cmo target_plplot_cmi)
  add_dependencies(target_plplot_cma target_plplot_cmo)

  add_custom_target(plplot_ocaml ALL)

  if(OCAMLOPT)
    # ocamlopt compiles *.ml into *.o and *.cmx and simultaneously
    # checks against *.cmi produced from *.mli above.
    add_custom_command(
      OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmx
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.o
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmx
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.o
      COMMAND ${OCAMLOPT} -c ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.ml
      # ocamlc and ocamlopt *.ml source file must be in
      # ${CMAKE_CURRENT_BINARY_DIR}.
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plplot.ml ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
      COMMAND ${OCAMLOPT} -c ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.ml
      ${CMAKE_CURRENT_SOURCE_DIR}/plplot.ml
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmi
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmi
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(target_plplot_cmx
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmx
      )

    # ocamlopt -a builds the libraries *.cmxa and *.a respectively from
    # the *.cmx and *.o files.  The plplot_stubs library also plays
    # a role.
    add_custom_command(
      OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.a
      COMMAND ${OCAMLOPT} -a -o ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmx ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmx -ccopt -L${CMAKE_CURRENT_BINARY_DIR} -cclib -lplplot_stubs -ccopt -L${CAMLIDL_LIB_DIR} -cclib -lcamlidl -ccopt -L${CMAKE_BINARY_DIR}/src -cclib -lplplot${LIB_TAG}
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmx
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmx
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(target_plplot_cmxa
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa
      )

    # Must serialize these targets against highest dependency ocamlc
    # target, target_plplot_cma, because of common custom commands in
    # their file-dependency chains which would be screwed up in a
    # parallel build without this serialization.

    add_dependencies(target_plplot_cmx target_plplot_cma)
    add_dependencies(target_plplot_cmxa target_plplot_cmx)

    add_dependencies(plplot_ocaml target_plplot_cmxa)

    # Need to keep track of file dependencies since this is a custom target.
    set_property(GLOBAL PROPERTY FILES_plplot_ocaml
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.a
      )
  else (OCAMLOPT)
    add_dependencies(plplot_ocaml target_plplot_cma)
    # Need to keep track of file dependencies since this is a custom target.
    set_property(GLOBAL PROPERTY FILES_plplot_ocaml
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma
      )
  endif(OCAMLOPT)

  if(OCAMLDOC)
    # Build OCaml API reference documentation
    set(OCAMLDOC_FILE_LIST
      Plplot.Plot.html
      Plplot.Quick_plot.html
      Plplot.html
      index.html
      index_attributes.html
      index_class_types.html
      index_classes.html
      index_exceptions.html
      index_methods.html
      index_module_types.html
      index_modules.html
      index_types.html
      index_values.html
      style.css
      type_Plplot.Plot.html
      type_Plplot.Quick_plot.html
      type_Plplot.html
      )

    set(OCAMLDOC_FILES)
    foreach(html_file ${OCAMLDOC_FILE_LIST})
      list(APPEND OCAMLDOC_FILES ${CMAKE_CURRENT_BINARY_DIR}/${html_file})
    endforeach(html_file ${OCAMLDOC_FILE_LIST})
    # ocamldoc builds the module's documentation using specially formatted
    # comments in the source file.
    add_custom_command(
      OUTPUT ${OCAMLDOC_FILES}
      COMMAND ${OCAMLDOC} -html ${CMAKE_CURRENT_SOURCE_DIR}/plplot.mli
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.mli
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(target_build_ocaml_doc DEPENDS ${OCAMLDOC_FILES})
    # associated custom command has common file depends with custom command
    # that is associated with target_plplot_cmi.  Therefore must serialize
    # the two custom targets.
    add_dependencies(target_plplot_cmi target_build_ocaml_doc)
  endif(OCAMLDOC)

  # Basic build done, now trying to finish up by adapting bits
  # and pieces of old build procedure below.

  # Configure the META file
  configure_file(META.in ${CMAKE_CURRENT_BINARY_DIR}/META)
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
  endif(PKG_CONFIG_EXECUTABLE)

  set(OCAML_FULL_INSTALL_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmi
    ${CMAKE_CURRENT_BINARY_DIR}/libplplot_stubs.a
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.mli
    )
  if (OCAMLOPT)
    set(OCAML_FULL_INSTALL_FILES
      ${OCAML_FULL_INSTALL_FILES}
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.a
      )
  endif (OCAMLOPT)

  # Most files go in the plplot subdirectory
  install(FILES ${OCAML_FULL_INSTALL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/META
    DESTINATION ${OCAML_INSTALL_DIR}/plplot
    )

  # Shared library stubs go in stublibs
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/dllplplot_stubs.so
    DESTINATION ${OCAML_INSTALL_DIR}/stublibs
    )

endif(ENABLE_ocaml)
