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

  option(OCAML_NEW_BUILD "Try new detailed OCaml build method?" OFF)
  if(OCAML_NEW_BUILD)

    #Detailed CMake logic to build ocaml bindings for PLplot.
    set(camlidl_GENERATED_SOURCE
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.h
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.ml
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.mli
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.c
      )

    add_custom_command(
      OUTPUT 
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.idl
      ${camlidl_GENERATED_SOURCE}
      # camlidl source file must be in ${CMAKE_CURRENT_BINARY_DIR}. 
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plplot_core.idl ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.idl
      COMMAND camlidl -header -I ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.idl
      DEPENDS
      ${CMAKE_CURRENT_SOURCE_DIR}/plplot_core.idl
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )

    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.o
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot_impl.o
      ${CMAKE_CURRENT_BINARY_DIR}/dllplplot_stubs.so
      ${CMAKE_CURRENT_BINARY_DIR}/libplplot_stubs.a
      COMMAND ocamlc -verbose -c ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.c
      COMMAND ocamlc -verbose -ccopt -I${CMAKE_SOURCE_DIR}/include -ccopt -I${CMAKE_BINARY_DIR}/include -ccopt -I${CMAKE_SOURCE_DIR}/lib/qsastime  -c ${CMAKE_CURRENT_SOURCE_DIR}/plplot_impl.c
      COMMAND ocamlmklib -v -o plplot_stubs -L${CAMLIDL_LIB_DIR} -lcamlidl -L${CMAKE_BINARY_DIR}/src -dllpath ${CMAKE_BINARY_DIR}/src -lplplot${LIB_TAG} ${CMAKE_CURRENT_BINARY_DIR}/plplot_core_stubs.o ${CMAKE_CURRENT_BINARY_DIR}/plplot_impl.o
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

    add_custom_command(
      OUTPUT 
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmi
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.mli
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmi
      COMMAND ocamlc -verbose -c ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.mli
      # ocamlc *.mli source file must be in ${CMAKE_CURRENT_BINARY_DIR}.
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plplot.mli ${CMAKE_CURRENT_BINARY_DIR}/plplot.mli
      COMMAND ocamlc -verbose -c ${CMAKE_CURRENT_BINARY_DIR}/plplot.mli
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.mli
      ${CMAKE_CURRENT_SOURCE_DIR}/plplot.mli
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(target_plplot_core_cmi
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmi
      )

    add_custom_command(
      OUTPUT 
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmo
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmx
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.o
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmo
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmx
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.o
      COMMAND ocamlc -verbose -c ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.ml
      COMMAND ocamlopt -verbose -c ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.ml
      # ocamlc and ocamlopt *.ml source file must be in 
      # ${CMAKE_CURRENT_BINARY_DIR}.
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/plplot.ml ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
      COMMAND ocamlc -verbose -c ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
      COMMAND ocamlopt -verbose -c ${CMAKE_CURRENT_BINARY_DIR}/plplot.ml
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.ml
      ${CMAKE_CURRENT_SOURCE_DIR}/plplot.ml
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmi
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmi
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(target_plplot_core_cmo
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmo
      )

    add_custom_command(
      OUTPUT
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.a
      COMMAND ocamlc -verbose -a -custom -ccopt -L${CMAKE_CURRENT_BINARY_DIR} -cclib -lplplot_stubs -ccopt -L${CMAKE_BINARY_DIR}/src -cclib -lplplot${LIB_TAG} -o ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmo ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmo
#      COMMAND ocamlc -verbose -a -custom -o ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmo ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmo
      COMMAND ocamlopt -verbose -a -ccopt -L${CMAKE_CURRENT_BINARY_DIR} -cclib -lplplot_stubs -ccopt -L${CMAKE_BINARY_DIR}/src -cclib -lplplot${LIB_TAG} -o ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmx ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmx
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmo
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmo
      ${CMAKE_CURRENT_BINARY_DIR}/plplot_core.cmx
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmx
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      )
    add_custom_target(target_plplot_cma
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa
      )

    # These targets depend on common files in their respective
    # file-dependency chains.  Therefore, to avoid screwing up parallel
    # builds must serialize with target depends.  
    add_dependencies(target_plplot_core_cmi target_lib_plplot_stubs)
    add_dependencies(target_plplot_core_cmo target_plplot_core_cmi)
    add_dependencies(target_plplot_cma target_plplot_core_cmo)

    # Basic build done, now trying to finish up by adapting bits
    # and pieces of old build procedure below.
    
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

    add_custom_target(plplot_ocaml ALL)
    add_dependencies(plplot_ocaml target_plplot_cma)

    # Need to keep track of file dependencies since this is a custom target.
    set_property(GLOBAL PROPERTY FILES_plplot_ocaml
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cma
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.cmxa 
      ${CMAKE_CURRENT_BINARY_DIR}/plplot.a
      )
    
    set(OCAML_FULL_INSTALL_FILES
      plplot.cma
      plplot.cmxa
      plplot.a
      plplot.cmi
      libplplot_stubs.a
      plplot.mli
      )
  
    # Most files go in the plplot subdirectory
    install(FILES ${OCAML_FULL_INSTALL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/META
      DESTINATION ${OCAML_INSTALL_DIR}/plplot
      )
    
    # Shared library stubs go in stublibs
    install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/dllplplot_stubs.so
      DESTINATION ${OCAML_INSTALL_DIR}/stublibs
      )

  else(OCAML_NEW_BUILD)

  set(SOURCE_LIST
    libplplot_stubs.clib
    plplot_h
    plplot_h.inc
    plplot_core.idl
    plplot_impl.c
    plplot.ml
    plplot.mli
    touchup.ml
    )
  
  set(OCAML_INSTALL_FILES
    plplot.cma
    plplot.cmxa
    plplot.a
    plplot.cmi
    libplplot_stubs.a
    plplot.mli
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
  # Don't bother with dependency on configured myocamlbuild.ml since
  # that file is created at initial cmake time.
  set(DEPENDS_LIST)

  # Copy all source into the build tree since the whole project must be in
  # the same directory
  foreach(SRC_FILE ${SOURCE_LIST})
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SRC_FILE}
      COMMAND ${CMAKE_COMMAND}
      -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${SRC_FILE}
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SRC_FILE}
      )
    list(APPEND DEPENDS_LIST ${CMAKE_CURRENT_BINARY_DIR}/${SRC_FILE})
  endforeach(SRC_FILE ${SOURCE_LIST})
  
  # Note: ocamlbuild requires build-dir to be a relative path
  # Also the build-dir must not be the project root - hence the 
  # new subdirectory

  # N.B. "touch_nocreate" commands below are used to always update the
  # date on the OUTPUT files since under certain circumstances (a
  # rebuild with new plplot${LIB_TAG}) ${OCAMLBUILD} does not update
  # the date itself which screws up file dependencies.

  # Build bytecode version.
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cma 
    COMMAND ${OCAMLBUILD} -tag debug -classic-display
    -build-dir ${OCAML_BUILD_DIR} plplot.cma
    COMMAND ${CMAKE_COMMAND} -E touch_nocreate
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cma
    DEPENDS ${DEPENDS_LIST} plplot${LIB_TAG}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(
    plplot_ocaml_bytecode
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cma
    )
  
  # Build native version.
  add_custom_command(
    OUTPUT 
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cmxa 
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.a
    COMMAND ${OCAMLBUILD} -tag debug -classic-display
    -build-dir ${OCAML_BUILD_DIR} plplot.cmxa
    COMMAND ${CMAKE_COMMAND} -E touch_nocreate
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cmxa 
    COMMAND ${CMAKE_COMMAND} -E touch_nocreate
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.a
    DEPENDS ${DEPENDS_LIST} plplot${LIB_TAG}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
  add_custom_target(
    plplot_ocaml_native
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cmxa 
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.a
    )
  # Serialize these two with a target dependency since the two builds
  # interfere with each other if done in parallel.
  add_dependencies(plplot_ocaml_native plplot_ocaml_bytecode)

  add_custom_target(plplot_ocaml ALL)
  add_dependencies(plplot_ocaml plplot_ocaml_native)

  # Need to keep track of file dependencies since this is a custom target.
  set_property(GLOBAL PROPERTY FILES_plplot_ocaml
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cma
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.cmxa 
    ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/plplot.a
    )
  
  # Most files go in the plplot subdirectory
  install(FILES ${OCAML_FULL_INSTALL_FILES} ${CMAKE_CURRENT_BINARY_DIR}/META
    DESTINATION ${OCAML_INSTALL_DIR}/plplot
    )
  
  # Shared library stubs go in stublibs
  install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${OCAML_BUILD_DIR}/dllplplot_stubs.so
    DESTINATION ${OCAML_INSTALL_DIR}/stublibs
    )

  # Remove entire contents of ${OCAML_BUILD_DIR} when the clean target
  # is run.  This proved to work fine on cmake-2.6.4, but may not for
  # earlier versions according to a remark made here before.
  set_directory_properties(PROPERTIES 
    ADDITIONAL_MAKE_CLEAN_FILES 
    ${OCAML_BUILD_DIR}
    )
  endif(OCAML_NEW_BUILD)

endif(ENABLE_ocaml)
