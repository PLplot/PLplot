# -*- mode: cmake -*-
# bindings/ocaml/plcairo/CMakeLists.txt
### Process this file with cmake to produce Makefile
#
#
# Copyright (C) 2008 Andrew Ross
# Copyright (C) 2009 Hezekiah M. Carty
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

if(PKG_CONFIG_EXECUTABLE)
  pkg_check_pkgconfig(cairo includedir libdir linkflags cflags version _CAIRO)
endif(PKG_CONFIG_EXECUTABLE)
if(ENABLE_ocaml AND OCAML_HAS_CAIRO AND linkflags)
  # Cairo compile and link flags
  # A hack to get the compile and link flags from pkg-config
  execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --cflags cairo
    OUTPUT_VARIABLE CAIRO_COMPILE_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --libs cairo
    OUTPUT_VARIABLE CAIRO_LINK_FLAGS
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )

  # locations of plplot.cma and plplot.cmxa
  set(PATH_TO_PLPLOT_CMA ${CMAKE_BINARY_DIR}/bindings/ocaml)
  set(PATH_TO_PLPLOT_CMXA ${CMAKE_BINARY_DIR}/bindings/ocaml)

  # ocamlc -c compiles *.c into *.o.
  # ocamlmklib links *.o into *.so and *.a
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo_impl.o
    ${CMAKE_CURRENT_BINARY_DIR}/dllplcairo_stubs.so
    ${CMAKE_CURRENT_BINARY_DIR}/libplcairo_stubs.a
    COMMAND ${OCAMLC} -ccopt "${CAIRO_COMPILE_FLAGS}" -cclib '${CAIRO_LINK_FLAGS}' -ccopt -I${CMAKE_SOURCE_DIR}/include -ccopt -I${CMAKE_BINARY_DIR}/include -ccopt -I${CMAKE_SOURCE_DIR}/lib/qsastime  -c ${CMAKE_CURRENT_SOURCE_DIR}/plcairo_impl.c
    COMMAND ${OCAMLMKLIB} -o plcairo_stubs "${CAIRO_LINK_FLAGS}" -L${CMAKE_BINARY_DIR}/src -lplplot${LIB_TAG} ${CMAKE_CURRENT_BINARY_DIR}/plcairo_impl.o
    DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/plcairo_impl.c
    plplot${LIB_TAG}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(target_lib_plcairo_stubs
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/dllplcairo_stubs.so
    ${CMAKE_CURRENT_BINARY_DIR}/libplcairo_stubs.a
    )

  # ocamlc -c compiles *.mli into *.cmi
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.mli
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmi
    # ocamlc *.mli source file must be in ${CMAKE_CURRENT_BINARY_DIR}.
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plcairo.mli ${CMAKE_CURRENT_BINARY_DIR}/plcairo.mli
    COMMAND ${OCAMLFIND} c -package cairo -c ${CMAKE_CURRENT_BINARY_DIR}/plcairo.mli
    DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/plcairo.mli
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(target_plcairo_cmi
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmi
    )

  # ocamlc -c compiles  *.ml into *.cmo and simultaneously checks against
  # *.cmi produced from *.mli above.
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmo
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.ml
    # ocamlc and ocamlopt *.ml source file must be in
    # ${CMAKE_CURRENT_BINARY_DIR}.
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plcairo.ml ${CMAKE_CURRENT_BINARY_DIR}/plcairo.ml
    COMMAND ${OCAMLFIND} c -package cairo -I ${PATH_TO_PLPLOT_CMA} plplot.cma -c ${CMAKE_CURRENT_BINARY_DIR}/plcairo.ml
    DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/plcairo.ml
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmi
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(target_plcairo_cmo
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmo
    )

  # ocamlc -a -custom builds a *.cma library from *.cmo
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cma
    COMMAND ${OCAMLC} -a -custom -o ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cma -I ${PATH_TO_PLPLOT_CMA} plplot.cma ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmo -dllib -lplcairo_stubs -ccopt -L${CMAKE_CURRENT_BINARY_DIR} -cclib -lplcairo_stubs -ccopt -L${CMAKE_BINARY_DIR}/src -cclib -lplplot${LIB_TAG} -dllpath ${CMAKE_BINARY_DIR}/src
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmo
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(target_plcairo_cma
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cma
    )

  # These targets depend on common files in their respective
  # file-dependency chains.  Therefore, to avoid screwing up parallel
  # builds must serialize with target depends.
  add_dependencies(target_plcairo_cmi target_lib_plcairo_stubs target_plplot_cma)
  add_dependencies(target_plcairo_cmo target_plcairo_cmi)
  add_dependencies(target_plcairo_cma target_plcairo_cmo)

  add_custom_target(plcairo_ocaml ALL)

  if(OCAMLOPT)
    # ocamlopt compiles *.ml into *.o and *.cmx and simultaneously
    # checks against *.cmi produced from *.mli above.
    add_custom_command(
      OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmx
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.o
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.ml
      # ocamlc and ocamlopt *.ml source file must be in
      # ${CMAKE_CURRENT_BINARY_DIR}.
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plcairo.ml ${CMAKE_CURRENT_BINARY_DIR}/plcairo.ml
      COMMAND ${OCAMLFIND} opt -package cairo -I ${PATH_TO_PLPLOT_CMXA} -c ${CMAKE_CURRENT_BINARY_DIR}/plcairo.ml
      DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/plcairo.ml
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmi
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(target_plcairo_cmx
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmx
      )

    # ocamlopt -a builds the libraries *.cmxa and *.a respectively from
    # the *.cmx and *.o files.  The plplot_stubs library also plays
    # a role.
    add_custom_command(
      OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmxa
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.a
      COMMAND ${OCAMLOPT} -a -o ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmxa -I ${PATH_TO_PLPLOT_CMXA} ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmx -ccopt -L${CMAKE_CURRENT_BINARY_DIR} -cclib -lplcairo_stubs -ccopt -L${CMAKE_BINARY_DIR}/src -cclib -lplplot${LIB_TAG}
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmx
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(target_plcairo_cmxa
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmxa
      )

    # Must serialize these targets against highest dependency ocamlc
    # target, target_plplot_cma, because of common custom commands in
    # their file-dependency chains which would be screwed up in a
    # parallel build without this serialization.

    add_dependencies(target_plcairo_cmx target_plcairo_cma target_plplot_cmxa)
    add_dependencies(target_plcairo_cmxa target_plcairo_cmx)

    add_dependencies(plcairo_ocaml target_plcairo_cmxa)

    # Need to keep track of file dependencies since this is a custom target.
    set_property(GLOBAL PROPERTY FILES_plcairo_ocaml
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmxa
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.a
      )
  else (OCAMLOPT)
    add_dependencies(plcairo_ocaml target_plcairo_cma)
    # Need to keep track of file dependencies since this is a custom target.
    set_property(GLOBAL PROPERTY FILES_plcairo_ocaml
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cma
      )
  endif(OCAMLOPT)

  # These targets depend on common files in their respective
  # file-dependency chains.  Therefore, to avoid screwing up parallel
  # builds must serialize with target depends.
  add_dependencies(target_plcairo_cmi target_lib_plcairo_stubs)
  # ocamlc
  add_dependencies(target_plcairo_cmo target_plcairo_cmi)
  add_dependencies(target_plcairo_cma target_plcairo_cmo)
  # ocamlopt
  add_dependencies(target_plcairo_cmx target_plcairo_cmi)
  add_dependencies(target_plcairo_cmxa target_plcairo_cmx)

  # Basic build done, now trying to finish up by adapting bits
  # and pieces of old build procedure below.

  # Configure the META file
  configure_file(META.cmake ${CMAKE_CURRENT_BINARY_DIR}/META)

  add_custom_target(plplot_ocaml_plcairo ALL)
  if (OCAMLOPT)
    add_dependencies(plplot_ocaml_plcairo target_plcairo_cma target_plcairo_cmxa)
  else (OCAMLOPT)
    add_dependencies(plplot_ocaml_plcairo target_plcairo_cma)
  endif (OCAMLOPT)

  set(OCAML_FULL_INSTALL_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cma
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmi
    ${CMAKE_CURRENT_BINARY_DIR}/libplcairo_stubs.a
    ${CMAKE_CURRENT_BINARY_DIR}/plcairo.mli
    )
  if (OCAMLOPT)
    set(OCAML_FULL_INSTALL_FILES
      ${OCAML_FULL_INSTALL_FILES}
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.cmxa
      ${CMAKE_CURRENT_BINARY_DIR}/plcairo.a
      )
  endif (OCAMLOPT)

  # Most files go in the plcairo subdirectory
  install(FILES ${OCAML_FULL_INSTALL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/META
    DESTINATION ${OCAML_INSTALL_DIR}/plcairo
    )

  # Shared library stubs go in stublibs
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/dllplcairo_stubs.so
    DESTINATION ${OCAML_INSTALL_DIR}/stublibs
    )

endif(ENABLE_ocaml AND OCAML_HAS_CAIRO)
