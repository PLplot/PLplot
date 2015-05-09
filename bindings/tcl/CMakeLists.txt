# bindings/tcl/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2015 Alan W. Irwin
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

if(ENABLE_tcl)

  # Check consistency of plplot_parameters.h.
  add_custom_target(
    check_tcl_parameters
    ${CMAKE_COMMAND} -E echo "Check that bindings/tcl/plplot_parameters.h is consistent with the #defines in bindings/swig-support/plplotcapi.i"
    COMMAND ${CMAKE_COMMAND} -E remove -f
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_parameters.h_compare
    COMMAND
    sed -f ${CMAKE_CURRENT_SOURCE_DIR}/global_defines.sed <
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i >
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_parameters.h_compare
    COMMAND
    COMMAND
    cmp ${CMAKE_CURRENT_SOURCE_DIR}/plplot_parameters.h
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_parameters.h_compare
    )

  add_dependencies(check_all check_tcl_parameters)

  # tclmatrix
  set(tclmatrix_LIB_SRCS
    tclMatrix.c
    matrixInit.c
    )

  include_directories(
    ${TCL_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    )

  set(
    libplplottcltk_COMPILE_FLAGS
    "-I\"${TCL_INCLUDE_PATH}\""
    )

  if(BUILD_SHARED_LIBS)
    set_source_files_properties(${tclmatrix_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
  endif(BUILD_SHARED_LIBS)

  add_library(tclmatrix ${tclmatrix_LIB_SRCS})

  if(USE_TCL_TK_STUBS)
    target_link_libraries(tclmatrix ${TCL_STUB_LIBRARY})
    set_target_properties(
      tclmatrix
      PROPERTIES COMPILE_DEFINITIONS USE_TCL_STUBS
      )
  else(USE_TCL_TK_STUBS)
    target_link_libraries(tclmatrix ${TCL_LIBRARY})
  endif(USE_TCL_TK_STUBS)

  if(USE_RPATH)
    set(LIB_INSTALL_RPATH ${LIB_DIR} ${TCL_RPATH})
    filter_rpath(LIB_INSTALL_RPATH)
    set_target_properties(tclmatrix
      PROPERTIES
      SOVERSION ${tclmatrix_SOVERSION}
      VERSION ${tclmatrix_VERSION}
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  else(USE_RPATH)
    set_target_properties(tclmatrix
      PROPERTIES
      SOVERSION ${tclmatrix_SOVERSION}
      VERSION ${tclmatrix_VERSION}
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  endif(USE_RPATH)

  install(TARGETS tclmatrix
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  set(tclmatrix_INSTALLED_HEADERS
    tclMatrix.h
    )

  install(
    FILES ${tclmatrix_INSTALLED_HEADERS}
    DESTINATION ${INCLUDE_DIR}
    )
  # end tclmatrix

  # plplottcltk
  set(
    tclgenfiles
    ${CMAKE_CURRENT_BINARY_DIR}/tclgen.h
    ${CMAKE_CURRENT_BINARY_DIR}/tclgen_s.h
    ${CMAKE_CURRENT_BINARY_DIR}/tclgen.c
    )

  add_custom_command(
    OUTPUT ${tclgenfiles}
    COMMAND ${TCL_TCLSH}
    ${CMAKE_CURRENT_SOURCE_DIR}/pltclgen.tcl ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/pltclgen.tcl
    ${CMAKE_CURRENT_SOURCE_DIR}/plapi.tpl
    ${CMAKE_CURRENT_SOURCE_DIR}/tclcmd.tpl
    )

  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  set_source_files_properties(
    tclAPI.c
    PROPERTIES OBJECT_DEPENDS "${tclgenfiles}"
    )

  set(plplottcltk_LIB_SRCS
    tclAPI.c
    )

  set(plplottcltk_Main_LIB_SRCS
    tclMain.c
    )

  if(BUILD_SHARED_LIBS)
    set_source_files_properties(${plplottcltk_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL"
      )
    set_source_files_properties(${plplottcltk_Main_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL"
      )
  endif(BUILD_SHARED_LIBS)

  if(USE_TCL_TK_STUBS)
    set(
      plplottcltk_link_libraries
      tclmatrix
      plplot
      ${TCL_STUB_LIBRARY}
      )
    set(
      libplplottcltk_LINK_FLAGS
      -ltclmatrix
      ${TCL_STUB_LIBRARY}
      )
  else(USE_TCL_TK_STUBS)
    set(
      plplottcltk_link_libraries
      tclmatrix
      plplot
      ${TCL_LIBRARY}
      )
    set(
      libplplottcltk_LINK_FLAGS
      -ltclmatrix
      ${TCL_LIBRARY}
      )
  endif(USE_TCL_TK_STUBS)

  set(
    plplottcltk_Main_link_libraries
    plplot
    ${TCL_LIBRARY}
    )
  set(
    libplplottcltk_Main_LINK_FLAGS
     ${TCL_LIBRARY}
    )

  message(STATUS "ENABLE_itcl: ${ENABLE_itcl}")
  if(ENABLE_itcl)
    include_directories(${ITCL_INCLUDE_PATH})

    # tkMain.c is the only code that currently has a dependence on
    # itcl library routines.  Since that code is now part of the source code
    # for libplplottcltk_Main, only flags for that library (but not
    # libplplottcltk any more) are modified below.

    set(
      libplplottcltk_Main_COMPILE_FLAGS
      "${libplplottcltk_Main_COMPILE_FLAGS} -I\"${ITCL_INCLUDE_PATH}\""
      )
    set(plplottcltk_Main_link_libraries
      ${plplottcltk_Main_link_libraries}
      ${ITCL_LIBRARY}
      )
    set(
      libplplottcltk_Main_LINK_FLAGS
      ${libplplottcltk_Main_LINK_FLAGS}
      ${ITCL_LIBRARY}
      )
  endif(ENABLE_itcl)
  message(STATUS "Itcl libraries: ${plplottcltk_Main_link_libraries}")

  if(ENABLE_tkX)
    set(plplottcltk_LIB_SRCS
      ${plplottcltk_LIB_SRCS}
      ${CMAKE_SOURCE_DIR}/bindings/tk/Pltk_Init.c
      ${CMAKE_SOURCE_DIR}/bindings/tk/plframe.c
      ${CMAKE_SOURCE_DIR}/bindings/tk/plr.c
      ${CMAKE_SOURCE_DIR}/bindings/tk/tcpip.c
      )

    set(plplottcltk_Main_LIB_SRCS
      ${plplottcltk_Main_LIB_SRCS}
      ${CMAKE_SOURCE_DIR}/bindings/tk/tkMain.c
      )

    include_directories(${TK_INCLUDE_PATH})
    # Transform TK_INCLUDE_PATH (which is a list) to blank-delimited flag form.
    string(REGEX REPLACE ";" "\" -I\"" TKLIB_COMPILE_FLAGS "-I\"${TK_INCLUDE_PATH}\"")
    set(
      libplplottcltk_COMPILE_FLAGS
      "${libplplottcltk_COMPILE_FLAGS} ${TKLIB_COMPILE_FLAGS}"
      )

    set(
      libplplottcltk_Main_COMPILE_FLAGS
      "${libplplottcltk_Main_COMPILE_FLAGS} ${TKLIB_COMPILE_FLAGS}"
      )

    if(BUILD_SHARED_LIBS)
      set_source_files_properties(${plplottcltk_LIB_SRCS}
	PROPERTIES COMPILE_FLAGS "-DUSINGDLL"
	)
      set_source_files_properties(${plplottcltk_Main_LIB_SRCS}
	PROPERTIES COMPILE_FLAGS "-DUSINGDLL"
	)
    endif(BUILD_SHARED_LIBS)

    if(USE_TCL_TK_STUBS)
      set(plplottcltk_link_libraries
	${plplottcltk_link_libraries}
	${TK_STUB_LIBRARY}
	${X11_X11_LIB}
	)
      set(
	libplplottcltk_LINK_FLAGS
	${libplplottcltk_LINK_FLAGS}
	${TK_STUB_LIBRARY}
	)
    else(USE_TCL_TK_STUBS)
      set(plplottcltk_link_libraries
	${plplottcltk_link_libraries}
	${TK_LIBRARY}
	${X11_X11_LIB}
	)
      set(
	libplplottcltk_LINK_FLAGS
	${libplplottcltk_LINK_FLAGS}
	${TK_LIBRARY}
	)
    endif(USE_TCL_TK_STUBS)

    set(plplottcltk_Main_link_libraries
      ${plplottcltk_Main_link_libraries}
      ${TK_LIBRARY}
      ${X11_X11_LIB}
      )
    set(
      libplplottcltk_Main_LINK_FLAGS
      ${libplplottcltk_Main_LINK_FLAGS}
      ${TK_LIBRARY}
      )

    if(ENABLE_itkX)
      include_directories(${ITK_INCLUDE_PATH})
      # tkMain.c is the only code that currently has a dependence on
      # itk library routines.  Since that code is now part of the source code
      # for libplplottcltk_Main, only flags for that library (but not
      # libplplottcltk any more) are modified below.

      set(
	libplplottcltk_Main_COMPILE_FLAGS
	"${libplplottcltk_Main_COMPILE_FLAGS} -I\"${ITK_INCLUDE_PATH}\""
	)
      set(plplottcltk_Main_link_libraries
	${plplottcltk_Main_link_libraries}
	${ITK_LIBRARY}
	)
      set(
	libplplottcltk_Main_LINK_FLAGS
	${libplplottcltk_Main_LINK_FLAGS}
	${ITK_LIBRARY}
	)
    endif(ENABLE_itkX)
  endif(ENABLE_tkX)

  add_library(plplottcltk ${plplottcltk_LIB_SRCS})
  add_library(plplottcltk_Main ${plplottcltk_Main_LIB_SRCS})

  if(USE_TCL_TK_STUBS)
    if(ENABLE_tkX)
      set_target_properties(
	plplottcltk
	PROPERTIES COMPILE_DEFINITIONS "USE_TCL_STUBS;USE_TK_STUBS"
	)
    else(ENABLE_tkX)
      set_target_properties(
	plplottcltk
	PROPERTIES COMPILE_DEFINITIONS USE_TCL_STUBS
	)
    endif(ENABLE_tkX)
  endif(USE_TCL_TK_STUBS)

  if(NON_TRANSITIVE)
    # empty list ==> non-transitive linking for everything that links to
    # libtclmatrixd and libplplottcltkd in the shared libraries case.
    target_link_libraries(tclmatrix LINK_INTERFACE_LIBRARIES)
    target_link_libraries(plplottcltk LINK_INTERFACE_LIBRARIES)
    target_link_libraries(plplottcltk_Main LINK_INTERFACE_LIBRARIES)
  endif(NON_TRANSITIVE)

  #message(STATUS
  #"plplottcltk_link_libraries =
  #${plplottcltk_link_libraries}"
  #)
  target_link_libraries(
    plplottcltk
    ${plplottcltk_link_libraries}
    )

  target_link_libraries(
    plplottcltk_Main
    ${plplottcltk_Main_link_libraries}
    )

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot INSTALL_RPATH)
    list(APPEND LIB_INSTALL_RPATH ${TCL_TK_RPATH})
    filter_rpath(LIB_INSTALL_RPATH)
    set_target_properties(plplottcltk
      PROPERTIES
      SOVERSION ${plplottcltk_SOVERSION}
      VERSION ${plplottcltk_VERSION}
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )

    # This version of LIB_INSTALL_RPATH also used below for pltcl.
    list(APPEND LIB_INSTALL_RPATH ${TCL_TK_ITCL_ITK_RPATH})
    filter_rpath(LIB_INSTALL_RPATH)
    set_target_properties(plplottcltk_Main
      PROPERTIES
      SOVERSION ${plplottcltk_Main_SOVERSION}
      VERSION ${plplottcltk_Main_VERSION}
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  else(USE_RPATH)
    set_target_properties(plplottcltk
      PROPERTIES
      SOVERSION ${plplottcltk_SOVERSION}
      VERSION ${plplottcltk_VERSION}
      INSTALL_NAME_DIR "${LIB_DIR}"
      )

    set_target_properties(plplottcltk_Main
      PROPERTIES
      SOVERSION ${plplottcltk_Main_SOVERSION}
      VERSION ${plplottcltk_Main_VERSION}
      INSTALL_NAME_DIR "${LIB_DIR}"
      )

  endif(USE_RPATH)

  install(TARGETS plplottcltk
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  install(TARGETS plplottcltk_Main
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  set(plplottcltk_INSTALLED_HEADERS
    pltcl.h
    )

  install(
    FILES ${plplottcltk_INSTALLED_HEADERS}
    DESTINATION ${INCLUDE_DIR}
    )

  # end plplottcltk

  # Generate plplot.tcl and tclIndex in build tree.  At least the first one
  # is always needed for build-tree tests of tcl examples.

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot.tcl
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_SOURCE_DIR}/bindings/tk/plplot.tcl
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.tcl
    DEPENDS
    ${CMAKE_SOURCE_DIR}/bindings/tk/plplot.tcl
    )

  # _target suffix to avoid nameclash with filename plplot.tcl.  This
  # nameclash screwed up ../tk/configuration which had a non-generated file
  # of the same name which was a dependency of a custom target.
  add_custom_target(plplot.tcl_target ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot.tcl
    )
  set_property(GLOBAL PROPERTY
    FILES_plplot.tcl_target ${CMAKE_CURRENT_BINARY_DIR}/plplot.tcl
    )

  # A Tcl expert should insert the file depends here.
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
    COMMAND ${TCL_TCLSH} ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

  add_custom_target(tclIndex_tcl ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
    )
  set_property(GLOBAL PROPERTY
    FILES_tclIndex_tcl ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
    )

  # I don't think this is necessary, but I am not sure of the file
  # depends of the custom_command associated with tclIndex_tcl and
  # serializing these targets doesn't slow down parallel builds very
  # much.
  add_dependencies(tclIndex_tcl plplot.tcl_target)

  if(NOT ENABLE_tk)
    # Only install these two files if the equivalent files are not going to
    # be installed by the action of ../tk/CMakeLists.txt

    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot.tcl
      DESTINATION ${TCL_DIR}
      )

    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
      DESTINATION ${TCL_DIR}
      )
  endif(NOT ENABLE_tk)

  set(LIB_HARDDIR ${LIB_DIR})

  set(DLNAME_LIBPLPLOTTCLTK $<TARGET_FILE_NAME:plplottcltk>)

  # N.B. Results only available at generate time.
  configure_file_generate(
    ${CMAKE_CURRENT_SOURCE_DIR}/pkgIndex.tcl.in
    ${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl
    @ONLY
    )

  pkg_config_file("tcl" "Tcl/Tk" " Tcl/Tk bindings" "plplottcltk" "${libplplottcltk_COMPILE_FLAGS}" "${libplplottcltk_LINK_FLAGS}")
  pkg_config_file("tcl_Main" "Tcl_Main/Tk_Main" " Tcl_Main/Tk_Main bindings" "plplottcltk_Main" "${libplplottcltk_Main_COMPILE_FLAGS}" "${libplplottcltk_Main_LINK_FLAGS}")

  if(BUILD_SHARED_LIBS)
    set_source_files_properties(pltcl.c
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL")
  endif(BUILD_SHARED_LIBS)
  add_executable(pltcl pltcl.c)
  if(ENABLE_itcl)
    target_link_libraries(pltcl plplot plplottcltk_Main plplottcltk ${ITCL_LIBRARY} ${TCL_LIBRARY})
  else(ENABLE_itcl)
    target_link_libraries(pltcl plplot plplottcltk_Main plplottcltk ${TCL_LIBRARY})
  endif(ENABLE_itcl)

  if(USE_RPATH)
    set_target_properties(pltcl
      PROPERTIES
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      )
  endif(USE_RPATH)

  install(TARGETS pltcl
    EXPORT export_plplot
    DESTINATION ${BIN_DIR}
    )
endif(ENABLE_tcl)
