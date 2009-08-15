# examples/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006, 2007, 2008, 2009 Alan W. Irwin
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

if(CORE_BUILD)
  # Sort out RPATH issues for traditional build of installed examples using
  # */Makefiles configured in each * subdirectory from */Makefile.examples.in.
  if(USE_RPATH)
    get_target_property(_LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
    string(REGEX REPLACE ";" ":" LIB_INSTALL_RPATH "${_LIB_INSTALL_RPATH}")
    set(RPATHCMD "-Wl,-rpath -Wl,${LIB_INSTALL_RPATH}")
    if(PLD_extqt)
      if(ENABLE_DYNDRIVERS)
	get_target_property(_qt_INSTALL_RPATH qt INSTALL_RPATH)
	string(REGEX REPLACE ";" ":" qt_INSTALL_RPATH "${_qt_INSTALL_RPATH}")
	set(qt_RPATHCMD "-Wl,-rpath -Wl,${qt_INSTALL_RPATH}:${DRV_DIR}")
      else(ENABLE_DYNDRIVERS)
	set(qt_RPATHCMD ${RPATHCMD})
      endif(ENABLE_DYNDRIVERS)
    endif(PLD_extqt)
  endif(USE_RPATH)

  # Install lena.pgm in installed examples directory so that all implementations
  # of example 20 in the various examples subdirectories can conveniently
  # access this file.
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm
    DESTINATION ${DATA_DIR}/examples
    )

  # Do the same as above for the build tree for those who want to individually
  # run the various example 20 implementations from the examples subdirectories
  # of the build tree.

  if(BUILD_TEST)
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm
      ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm
      )
    add_custom_target(examples_lena_file ALL
      DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
      )
  endif(BUILD_TEST)

  # Compiled example directories first.
  add_subdirectory(c)
  get_property(targets_examples_c GLOBAL PROPERTY TARGETS_examples_c)

  if(ENABLE_ada)
    add_subdirectory(ada)
    get_property(targets_examples_ada GLOBAL PROPERTY TARGETS_examples_ada)
  endif(ENABLE_ada)

  if(ENABLE_cxx)
    add_subdirectory(c++)
    get_property(targets_examples_cxx GLOBAL PROPERTY TARGETS_examples_cxx)
  endif(ENABLE_cxx)

  if(ENABLE_d)
    add_subdirectory(d)
    get_property(targets_examples_d GLOBAL PROPERTY TARGETS_examples_d)
  endif(ENABLE_d)

  if(ENABLE_f77)
    add_subdirectory(f77)
    get_property(targets_examples_f77 GLOBAL PROPERTY TARGETS_examples_f77)
  endif(ENABLE_f77)

  if(ENABLE_f95)
    add_subdirectory(f95)
    get_property(targets_examples_f95 GLOBAL PROPERTY TARGETS_examples_f95)
  endif(ENABLE_f95)

  if(ENABLE_java)
    add_subdirectory(java)
    get_property(targets_examples_java GLOBAL PROPERTY TARGETS_examples_java)
  endif(ENABLE_java)

  if(ENABLE_ocaml)
    add_subdirectory(ocaml)
    get_property(targets_examples_ocaml GLOBAL PROPERTY TARGETS_examples_ocaml)
  endif(ENABLE_ocaml)

  if(ENABLE_tk)
    add_subdirectory(tk)
    get_property(targets_examples_tk GLOBAL PROPERTY TARGETS_examples_tk)
  endif(ENABLE_tk)
  
  # Non-compiled example directories next.
  if(ENABLE_lua)
    add_subdirectory(lua)
  endif(ENABLE_lua)

  if(ENABLE_pdl)
    add_subdirectory(perl)
  endif(ENABLE_pdl)

  if(ENABLE_python)
    add_subdirectory(python)
  endif(ENABLE_python)

  if(ENABLE_octave)
    add_subdirectory(octave)
  endif(ENABLE_octave)

  if(ENABLE_tcl)
    add_subdirectory(tcl)
  endif(ENABLE_tcl)

  # Configure Makefile.examples with some specific variables

  if(PLD_ps)
    if(NOT ENABLE_ada)
      set(PSC_ADA_COMMENT "#")
    endif(NOT ENABLE_ada)

    if(NOT ENABLE_cxx)
      set(PSC_CXX_COMMENT "#")
    endif(NOT ENABLE_cxx)

    if(NOT ENABLE_d)
      set(PSC_D_COMMENT "#")
    endif(NOT ENABLE_d)

    if(NOT ENABLE_f77)
      set(PSC_F77_COMMENT "#")
    endif(NOT ENABLE_f77)

    if(NOT ENABLE_f95)
      set(PSC_F95_COMMENT "#")
    endif(NOT ENABLE_f95)

    if(NOT ENABLE_java)
      set(PSC_JAVA_COMMENT "#")
    endif(NOT ENABLE_java)

    if(NOT ENABLE_ocaml)
      set(PSC_OCAML_COMMENT "#")
    endif(NOT ENABLE_ocaml)

    if(NOT ENABLE_octave)
      set(PSC_OCTAVE_COMMENT "#")
    endif(NOT ENABLE_octave)

    if(NOT ENABLE_pdl)
      set(PSC_PERL_COMMENT "#")
    endif(NOT ENABLE_pdl)

    if(NOT ENABLE_python)
      set(PSC_PYTHON_COMMENT "#")
    endif(NOT ENABLE_python)

    if(NOT ENABLE_tcl)
      set(PSC_TCL_COMMENT "#")
    endif(NOT ENABLE_tcl)

    if(NOT ENABLE_lua)
      set(PSC_LUA_COMMENT "#")
    endif(NOT ENABLE_lua)

  else(PLD_ps)
    set(PSC_ADA_COMMENT "#")
    set(PSC_C_COMMENT "#")
    set(PSC_CXX_COMMENT "#")
    set(PSC_D_COMMENT "#")
    set(PSC_F77_COMMENT "#")
    set(PSC_F95_COMMENT "#")
    set(PSC_JAVA_COMMENT "#")
    set(PSC_OCAML_COMMENT "#")
    set(PSC_OCTAVE_COMMENT "#")
    set(PSC_PERL_COMMENT "#")
    set(PSC_PYTHON_COMMENT "#")
    set(PSC_TCL_COMMENT "#")
  endif(PLD_ps)

  if(NOT ENABLE_tk)
    set(TK_COMMENT "#")
  endif(NOT ENABLE_tk)

  if(NOT PLD_pdfcairo)
    set(PDFCAIRO_COMMENT "#")
  endif(NOT PLD_pdfcairo)

  if(NOT PLD_pngcairo)
    set(PNGCAIRO_COMMENT "#")
  endif(NOT PLD_pngcairo)

  if(NOT PLD_pscairo)
    set(PSCAIRO_COMMENT "#")
  endif(NOT PLD_pscairo)

  if(NOT PLD_svgcairo)
    set(SVGCAIRO_COMMENT "#")
  endif(NOT PLD_svgcairo)

  if(NOT PLD_gif)
    set(GIF_COMMENT "#")
  endif(NOT PLD_gif)

  if(NOT PLD_jpeg)
    set(JPEG_COMMENT "#")
  endif(NOT PLD_jpeg)

  if(NOT PLD_png)
    set(PNG_COMMENT "#")
  endif(NOT PLD_png)

  if(NOT PLD_psttf)
    set(PSTTFC_COMMENT "#")
  endif(NOT PLD_psttf)

  if(NOT PLD_svg)
    set(SVG_COMMENT "#")
  endif(NOT PLD_svg)

  if(NOT PLD_xfig)
    set(XFIG_COMMENT "#")
  endif(NOT PLD_xfig)

  if(NOT PLD_pstex)
    set(PSTEX_COMMENT "#")
  endif(NOT PLD_pstex)

  if(NOT DIFF_EXECUTABLE OR NOT TAIL_EXECUTABLE)
    set(COMPARE_COMMENT "#")
  endif(NOT DIFF_EXECUTABLE OR NOT TAIL_EXECUTABLE)

  if(NOT PLD_cgm)
    set(CGM_COMMENT "#")
  endif(NOT PLD_cgm)

  if(NOT PLD_bmpqt)
    set(BMPQT_COMMENT "#")
  endif(NOT PLD_bmpqt)

  if(NOT PLD_jpgqt)
    set(JPGQT_COMMENT "#")
  endif(NOT PLD_jpgqt)

  if(NOT PLD_pngqt)
    set(PNGQT_COMMENT "#")
  endif(NOT PLD_pngqt)

  if(NOT PLD_ppmqt)
    set(PPMQT_COMMENT "#")
  endif(NOT PLD_ppmqt)

  if(NOT PLD_tiffqt)
    set(TIFFQT_COMMENT "#")
  endif(NOT PLD_tiffqt)

  if(NOT PLD_svgqt)
    set(SVGQT_COMMENT "#")
  endif(NOT PLD_svgqt)

  if(NOT PLD_epsqt)
    set(EPSQT_COMMENT "#")
  endif(NOT PLD_epsqt)

  if(NOT PLD_pdfqt)
    set(PDFQT_COMMENT "#")
  endif(NOT PLD_pdfqt)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
    ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    @ONLY
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
    DESTINATION ${DATA_DIR}/examples
    RENAME Makefile
    )

  # Start configuration/installation of CMake-based build system for
  # installed examples.
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DESTINATION ${DATA_DIR}/examples
    )

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/plplot_configure.cmake_installed_examples.in
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_configure.cmake_installed_examples
    @ONLY
    )

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_configure.cmake_installed_examples
    DESTINATION ${DATA_DIR}/examples/cmake/modules
    RENAME plplot_configure.cmake
    )

  # pkg-config support for CMake-based build system for installed examples.
  install(FILES
    ${CMAKE_SOURCE_DIR}/cmake/modules/pkg-config.cmake
    ${CMAKE_SOURCE_DIR}/cmake/modules/FindPkgConfig.cmake
    DESTINATION ${DATA_DIR}/examples/cmake/modules
    )

  # Install PLplot-specific language support files needed for CMake-based
  # build of installed examples.
  # N.B. This list of files must be maintained consistently with the file
  # lists in cmake/modules/language_support.cmake.

  set(LANG_SUPPORT_FILES)
  set(PLATFORM_SUPPORT_FILES)

  if(ENABLE_ada)
    set(LANG_SUPPORT_FILES ${LANG_SUPPORT_FILES}
      CMakeAdaCompiler.cmake.in
      CMakeAdaInformation.cmake
      CMakeDetermineAdaCompiler.cmake
      CMakeTestAdaCompiler.cmake
      )
  endif(ENABLE_ada)

  if(ENABLE_d)
    set(LANG_SUPPORT_FILES ${LANG_SUPPORT_FILES}
      CMakeD_Copyright.txt
      CMakeDCompiler.cmake.in
      CMakeDInformation.cmake
      CMakeDetermineDCompiler.cmake
      CMakeTestDCompiler.cmake
      )
    set(PLATFORM_SUPPORT_FILES ${PLATFORM_SUPPORT_FILES}
      Platform/Linux-dmd.cmake
      Platform/Linux-gdc.cmake
      Platform/Windows-dmd.cmake
      Platform/Windows-gdc.cmake
      )
  endif(ENABLE_d)

  if(ENABLE_f77 OR ENABLE_f95)
    set(LANG_SUPPORT_FILES ${LANG_SUPPORT_FILES}
      CMakeFortranInformation.cmake
      )
    set(PLATFORM_SUPPORT_FILES ${PLATFORM_SUPPORT_FILES}
      Platform/Cygwin-GNU-Fortran.cmake
      Platform/Windows-GNU-Fortran.cmake
      Platform/Windows-df.cmake
      Platform/Windows-f90.cmake
      )
  endif(ENABLE_f77 OR ENABLE_f95)

  if(LANG_SUPPORT_FILES)
    foreach(LANG_SUPPORT_FILE ${LANG_SUPPORT_FILES})
      install(FILES
	${CMAKE_SOURCE_DIR}/cmake/modules/${LANG_SUPPORT_FILE}
	DESTINATION ${DATA_DIR}/examples/cmake/modules
	)
    endforeach(LANG_SUPPORT_FILE ${LANG_SUPPORT_FILES})
  endif(LANG_SUPPORT_FILES)

  if(PLATFORM_SUPPORT_FILES)
    foreach(PLATFORM_SUPPORT_FILE ${PLATFORM_SUPPORT_FILES})
      install(FILES
	${CMAKE_SOURCE_DIR}/cmake/modules/${PLATFORM_SUPPORT_FILE}
	DESTINATION ${DATA_DIR}/examples/cmake/modules/Platform
	)
    endforeach(PLATFORM_SUPPORT_FILE ${PLATFORM_SUPPORT_FILES})
  endif(PLATFORM_SUPPORT_FILES)

else(CORE_BUILD)
  project(installed_plplot_examples NONE)

  CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4 FATAL_ERROR)
  # Location where PLplot cmake build system first looks for cmake modules.
  set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)

  #Configure variables and enable languages as needed.
  include(plplot_configure)

  include(export_plplot)

  if(ENABLE_c)
    add_subdirectory(c)
    get_property(targets_examples_c GLOBAL PROPERTY TARGETS_examples_c)
  endif(ENABLE_c)

  if(ENABLE_ada)
    add_subdirectory(ada)
    get_property(targets_examples_ada GLOBAL PROPERTY TARGETS_examples_ada)
  endif(ENABLE_ada)

  if(ENABLE_cxx)
    add_subdirectory(c++)
    get_property(targets_examples_cxx GLOBAL PROPERTY TARGETS_examples_cxx)
  endif(ENABLE_cxx)

  if(ENABLE_d)
    add_subdirectory(d)
    get_property(targets_examples_d GLOBAL PROPERTY TARGETS_examples_d)
  endif(ENABLE_d)

  if(ENABLE_f77)
    add_subdirectory(f77)
    get_property(targets_examples_f77 GLOBAL PROPERTY TARGETS_examples_f77)
  endif(ENABLE_f77)

  if(ENABLE_f95)
    add_subdirectory(f95)
    get_property(targets_examples_f95 GLOBAL PROPERTY TARGETS_examples_f95)
  endif(ENABLE_f95)

  if(ENABLE_java)
    add_subdirectory(java)
    get_property(targets_examples_java GLOBAL PROPERTY TARGETS_examples_java)
  endif(ENABLE_java)

  if(ENABLE_ocaml)
    add_subdirectory(ocaml)
    get_property(targets_examples_ocaml GLOBAL PROPERTY TARGETS_examples_ocaml)
  endif(ENABLE_ocaml)

  if(ENABLE_tk)
    add_subdirectory(tk)
    get_property(targets_examples_tk GLOBAL PROPERTY TARGETS_examples_tk)
  endif(ENABLE_tk)
  
  if(ENABLE_octave)
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/lena.img
      ${CMAKE_CURRENT_BINARY_DIR}/lena.img
      )
    execute_process(
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_CURRENT_SOURCE_DIR}/octave/lena.img
      ${CMAKE_CURRENT_BINARY_DIR}/octave/lena.img
      )
  endif(ENABLE_octave)

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm 
    ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
    )

  if(PLD_psc)
    set(output_LIST)
    set(noninteractive_targets_LIST)
    # language_info_LIST set in plplot_configure.cmake.  Each list
    # member consists of a colon-separated language name and the associated
    # filename suffix used by plplot-test.sh.
    foreach(language_info ${language_info_LIST})
      string(REGEX REPLACE "^(.*):.*$" "\\1" language ${language_info})
      string(REGEX REPLACE "^.*:(.*)$" "\\1" suffix ${language_info})

      if(ENABLE_${language})
	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x01${suffix}.psc
	  COMMAND ${CMAKE_COMMAND} -E echo "Generate ${language} results for psc device"
	  COMMAND SRC_EXAMPLES_DIR=${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/plplot-test.sh --verbose --front-end=${language} --device=psc
	  DEPENDS
	  ${CMAKE_CURRENT_SOURCE_DIR}/plplot-test.sh
	  )
	list(APPEND output_LIST ${CMAKE_CURRENT_BINARY_DIR}/x01${suffix}.psc)
	if(targets_examples_${language})
	  list(APPEND noninteractive_targets_LIST ${targets_examples_${language}})
	endif(targets_examples_${language})
      endif(ENABLE_${language})
    endforeach(language_info ${language_info_LIST})

    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/compare
      COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/compare
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test_diff.sh
      DEPENDS ${output_LIST}
      )
    set(output_LIST ${CMAKE_CURRENT_BINARY_DIR}/compare)
  else(PLD_psc)
    set(output_LIST)
  endif(PLD_psc)

  # noninteractive_devices_info_LIST set in plplot_configure.cmake.  Each list
  # member consists of a colon-separated device name and a Boolean variable
  # that indicates if device has familied output in plplot-test.sh.
  foreach(noninteractive_devices_info ${noninteractive_devices_info_LIST})
    string(REGEX REPLACE "^(.*):.*$" "\\1" device ${noninteractive_devices_info})
    string(REGEX REPLACE "^.*:(.*)$" "\\1" familied ${noninteractive_devices_info})

    # If familying turned on for this device in plplot-test.sh, then the OUTPUT
    # files appear in the form x??c??.${device} rather than x??c.${device}.
    # Put in a representative placeholder for the familying index in the
    # OUTPUT name.
    if(familied)
      set(index 01)
    else(familied)
      set(index)
    endif(familied)

    # ENABLE_c is always ON by construction in plplot_configure.cmake, but
    # make a logical test of it in any case just to be consistent with above
    # ENABLE_${language} logic where c is one of the languages.
    if(PLD_${device} AND ENABLE_c)
      #message("DEBUG: OUTPUT filename = x01c${index}.${device}")
      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x01c${index}.${device}
	COMMAND ${CMAKE_COMMAND} -E echo "Generate C results for ${device} device"
	COMMAND SRC_EXAMPLES_DIR=${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/plplot-test.sh --verbose --front-end=c --device=${device}
	DEPENDS
	${CMAKE_CURRENT_SOURCE_DIR}/plplot-test.sh
	)
      list(APPEND output_LIST ${CMAKE_CURRENT_BINARY_DIR}/x01c${index}.${device})
    endif(PLD_${device} AND ENABLE_c)
  endforeach(noninteractive_devices_info ${noninteractive_devices_info_LIST})

  add_custom_target(test_noninteractive 
    DEPENDS ${output_LIST}
    )
  if(noninteractive_targets_LIST)
    add_dependencies(test_noninteractive ${noninteractive_targets_LIST})
  endif(noninteractive_targets_LIST)

  set(interactive_targets_LIST ${targets_examples_c})

  if(targets_examples_tk)
    list(APPEND interactive_targets_LIST ${targets_examples_tk})
  endif(targets_examples_tk)

  if(TARGET plplotcanvas_demo)
    list(APPEND interactive_targets_LIST plplotcanvas_demo)
  endif(TARGET plplotcanvas_demo)

  if(TARGET plplotcanvas_animation)
    list(APPEND interactive_targets_LIST plplotcanvas_animation)
  endif(TARGET plplotcanvas_animation)

  if(TARGET extXdrawable_demo)
    list(APPEND interactive_targets_LIST extXdrawable_demo)
  endif(TARGET extXdrawable_demo)

  if(TARGET ext-cairo-test)
    list(APPEND interactive_targets_LIST ext-cairo-test)
  endif(TARGET ext-cairo-test)

  if(TARGET wxPLplotDemo)
    list(APPEND interactive_targets_LIST wxPLplotDemo)
  endif(TARGET wxPLplotDemo)

  if(TARGET qt_example)
    list(APPEND interactive_targets_LIST qt_example)
  endif(TARGET qt_example)

  add_custom_target(test_interactive
    COMMAND SRC_EXAMPLES_DIR=${CMAKE_CURRENT_SOURCE_DIR} EXAMPLES_DIR=${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/plplot-test-interactive.sh
    )

  if(interactive_targets_LIST)
    add_dependencies(test_interactive ${interactive_targets_LIST})
  endif(interactive_targets_LIST)

  set(summary_results "
Summary of CMake build system results for the installed examples

Noninteractive device drivers:

PLD_pdfcairo:		${PLD_pdfcairo}
PLD_pngcairo:		${PLD_pngcairo}
PLD_pscairo:		${PLD_pscairo}
PLD_svgcairo:		${PLD_svgcairo}
PLD_cgm:		${PLD_cgm}
PLD_dg300:		${PLD_dg300}
PLD_epsqt:		${PLD_epsqt}
PLD_pdfqt:		${PLD_pdfqt}
PLD_bmpqt:		${PLD_bmpqt}
PLD_jpgqt:		${PLD_jpgqt}
PLD_pngqt:		${PLD_pngqt}
PLD_ppmqt:		${PLD_ppmqt}
PLD_tiffqt:		${PLD_tiffqt}
PLD_svgqt:		${PLD_svgqt}
PLD_gif:		${PLD_gif}
PLD_jpeg:		${PLD_jpeg}
PLD_png:		${PLD_png}
PLD_hp7470:		${PLD_hp7470}
PLD_hp7580:		${PLD_hp7580}
PLD_lj_hpgl:		${PLD_lj_hpgl}
PLD_imp:		${PLD_imp}
PLD_ljii:		${PLD_ljii}
PLD_ljiip:		${PLD_ljiip}
PLD_pbm:		${PLD_pbm}
PLD_pdf:		${PLD_pdf}
PLD_ps:			${PLD_ps}
PLD_psc:		${PLD_psc}
PLD_pstex:		${PLD_pstex}
PLD_psttf:		${PLD_psttf}
PLD_psttfc:		${PLD_psttfc}
PLD_svg:		${PLD_svg}
PLD_tek4010f:		${PLD_tek4010f}
PLD_tek4107f:		${PLD_tek4107f}
PLD_wxpng:		${PLD_wxpng}
PLD_xfig:		${PLD_xfig}

Interactive device drivers:

PLD_xcairo:		${PLD_xcairo}
PLD_extcairo:		${PLD_extcairo}

N.B. The above devices are all that are currently configurable by the
installed examples build system.  Additional interactive device
drivers are likely supplied by the PLplot core and configured as part
of plplot-test-interactive.sh that is run by

make test_interactive


but the build system for the installed examples cannot configure anything
with those device drivers so we don't bother to retreive them from the
core build system or output them here.

Noninteractive bindings:

ENABLE_ada:		${ENABLE_ada}
ENABLE_c:		${ENABLE_c}
ENABLE_cxx:		${ENABLE_cxx}
ENABLE_d:		${ENABLE_d}
ENABLE_f77:		${ENABLE_f77}
ENABLE_f95:		${ENABLE_f95}
ENABLE_java:		${ENABLE_java}
ENABLE_lua:		${ENABLE_lua}
ENABLE_ocaml:		${ENABLE_ocaml}
ENABLE_octave:		${ENABLE_octave}
ENABLE_pdl:		${ENABLE_pdl}
ENABLE_python:		${ENABLE_python}
ENABLE_tcl:		${ENABLE_tcl}

Interactive bindings:

ENABLE_tk:		${ENABLE_tk}
ENABLE_gnome2:		${ENABLE_gnome2}
ENABLE_wxwidgets:	${ENABLE_wxwidgets}
ENABLE_qt:		${ENABLE_qt}
")

  message("${summary_results}")

endif(CORE_BUILD)
