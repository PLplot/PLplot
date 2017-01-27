# -*- mode: cmake -*-
# bindings/ocaml/CMakeLists.txt
### Process this file with cmake to produce Makefile
#
#
# Copyright (C) 2008 Andrew Ross
# Copyright (C) 2009 Hezekiah M. Carty
# Copyright (C) 2009-2017 Alan W. Irwin
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

  # Stack on a bunch of extra flags if we are building against a static PLplot core
  set(ocaml_STATIC_FLAGS)
  if(NOT BUILD_SHARED_LIBS)
    foreach(DEP ${plplotd_LIB_DEPENDS} ${csirocsa_LIB_DEPENDS} ${csironn_LIB_DEPENDS})
      if(DEP STREQUAL "csirocsa" OR DEP STREQUAL "csironn" OR DEP STREQUAL "qsastime")
        set(internal_LIB_DIR)
        if(DEP STREQUAL "csirocsa")
          set(internal_LIB_DIR "csa")
        elseif(DEP STREQUAL "csironn")
          set(internal_LIB_DIR "nn")
        elseif(DEP STREQUAL "qsastime")
          set(internal_LIB_DIR "qsastime")
        endif()
        set(ocaml_STATIC_FLAGS ${ocaml_STATIC_FLAGS} -cclib ${CMAKE_BINARY_DIR}/lib/${internal_LIB_DIR}/lib${DEP}.a)
      elseif(DEP STREQUAL "general")
        set(ocaml_STATIC_FLAGS ${ocaml_STATIC_FLAGS})
      else()
        if(DEP MATCHES "^-")
          set(ocaml_STATIC_FLAGS ${ocaml_STATIC_FLAGS} -ccopt ${DEP})
        else()
          set(ocaml_STATIC_FLAGS ${ocaml_STATIC_FLAGS} -cclib ${DEP})
        endif()
      endif()
    endforeach(DEP ${plplotd_LIB_DEPENDS})
  endif(NOT BUILD_SHARED_LIBS)

  # optional command to check consistency of plplot_h.inc.
  if(GENERATE_PLPLOT_H_INC)
    add_custom_target(
      check_plplot_h.inc
      COMMAND ${CMAKE_COMMAND} -E echo "Check that bindings/ocaml/plplot_h.inc is consistent with bindings/ocaml/touchup.ml and bindings/ocaml/plplot_h"
      COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_CURRENT_BINARY_DIR}/generated_plplot_h.inc
      COMMAND ${OCAML} ${CMAKE_CURRENT_SOURCE_DIR}/touchup.ml ${CMAKE_CURRENT_SOURCE_DIR}/plplot_h ${CMAKE_CURRENT_BINARY_DIR}/generated_plplot_h.inc
      COMMAND cmp ${CMAKE_CURRENT_SOURCE_DIR}/plplot_h.inc ${CMAKE_CURRENT_BINARY_DIR}/generated_plplot_h.inc
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )

    # This no longer works for unknown reasons as of plplot-5.12.0
    # (The error message was "Error: Unbound module Pcre" so
    # perhaps that namespace should be named "pcre" instead or
    # else there might be an installation issue.)  But there
    # is no time to investigate this now so drop this custom
    # target as a dependency of check_all
    # add_dependencies(check_all check_plplot_h.inc)

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
    COMMAND ${OCAMLC} -ccopt -I${CAMLIDL_LIB_DIR} -c ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.c
    COMMAND ${OCAMLC} -ccopt -I${CMAKE_SOURCE_DIR}/include -ccopt -I${CMAKE_BINARY_DIR}/include -ccopt -I${CMAKE_SOURCE_DIR}/lib/qsastime -ccopt -I${CMAKE_BINARY_DIR} -ccopt -I${CAMLIDL_LIB_DIR} -ccopt -DPLPLOT_HAVE_CONFIG_H -c ${CMAKE_CURRENT_SOURCE_DIR}/plplot_impl.c
    COMMAND ${OCAMLMKLIB} -o plplot_stubs -L${CAMLIDL_LIB_DIR} -lcamlidl -L${CMAKE_BINARY_DIR}/src -lplplot ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.o ${CMAKE_CURRENT_BINARY_DIR}/plplot_impl.o ${ocaml_STATIC_FLAGS}
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.c
    ${CMAKE_CURRENT_SOURCE_DIR}/plplot_impl.c
    plplot
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
    COMMAND ${OCAMLC} -a -custom -o ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmo ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmo -dllib -lplplot_stubs -ccopt -L${CMAKE_CURRENT_BINARY_DIR} -cclib -lplplot_stubs -ccopt -L${CAMLIDL_LIB_DIR} -cclib -lcamlidl -ccopt -L${CMAKE_BINARY_DIR}/src -cclib -lplplot -dllpath ${CMAKE_BINARY_DIR}/src ${ocaml_STATIC_FLAGS}
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
      COMMAND ${OCAMLOPT} -a -o ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmx ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmx -ccopt -L${CMAKE_CURRENT_BINARY_DIR} -cclib -lplplot_stubs -ccopt -L${CAMLIDL_LIB_DIR} -cclib -lcamlidl -ccopt -L${CMAKE_BINARY_DIR}/src -cclib -lplplot ${ocaml_STATIC_FLAGS}
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

  # Shared library stubs go in stublibs.  Use SO_PERMISSIONS to
  # be consistent with permissions used for shared objects created
  # by the add_library command.
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/dllplplot_stubs.so
    DESTINATION ${OCAML_INSTALL_DIR}/stublibs
    PERMISSIONS ${SO_PERMISSIONS}
    )

  # Configure pkg-config *.pc file corresponding to plplot.cma
  pkg_config_file("ocaml" "OCaml" " OCaml binding" "plplot" "" "")

endif(ENABLE_ocaml)
