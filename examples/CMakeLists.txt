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
  # Configure Makefile.examples with some specific variables for the
  # traditional Makefile + pkg-config installed examples build.
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

  if(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
    set(COMPARE_COMMENT)
  else(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
    set(COMPARE_COMMENT "#")
  endif(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)

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

endif(CORE_BUILD)

if(CORE_BUILD)
  # These variables (normally not set or needed for a core build) are
  # needed below for the tests of the languages and devices.  They
  # are also needed to configure plplot_configure.cmake whose template
  # file is plplot_configure.cmake_installed_examples.in and which
  # ends up installed in examples/cmake/module.
  set(ENABLE_c ON)
  set(PLD_psc ${PLD_ps})
  set(PLD_psttfc ${PLD_psttf})
  # The psc device handled separately below so don't add it to
  # FILE_DEVICES_LIST.
  if(PLD_psttfc)
    list(APPEND FILE_DEVICES_LIST psttfc:psttf:OFF)
  endif(PLD_psttfc)

  # Start configuration/installation of CMake-based build system for
  # installed examples.
  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DESTINATION ${DATA_DIR}/examples
    )

  # language_info_LIST contains information about the languages used in
  # the noninteractive tests.  The first field is language (which must be
  # consistent with the list of languages configured by set commands in
  # examples/plplot_configure.cmake_installed_examples.in).  The second
  # field is the subdirectory corresponding to the language.  The third
  # field is the file suffix on plplot-test.sh results for that language.
  # N.B. this list is configured in plplot_configure.cmake below and
  # is therefore available both for the core build of the tests and
  # the installed-examples build of the tests below.

  set(language_info_LIST
    ada:ada:a
    c:c:c
    cxx:c++:cxx
    d:d:d
    f77:f77:f
    f95:f95:f95
    java:java:j
    lua:lua:lua
    ocaml:ocaml:ocaml
    octave:octave:o
    pdl:perl:pdl
    python:python:p
    tcl:tcl:t
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

  # Install soft-landing support for compiler detection/testing.
  install(FILES
    ${CMAKE_SOURCE_DIR}/cmake/modules/language_support.cmake 
    DESTINATION ${DATA_DIR}/examples/cmake/modules
    )

  # Install PLplot-specific language support files needed for CMake-based
  # build of installed examples.
  # N.B. This list of files must be maintained consistently with the file
  # lists in cmake/modules/language_support.cmake.

  set(LANG_SUPPORT_FILES)

  if(ENABLE_ada)
    set(LANG_SUPPORT_FILES ${LANG_SUPPORT_FILES}
      language_support/cmake:CMakeAdaCompiler.cmake.in
      language_support/cmake:CMakeAdaInformation.cmake
      language_support/cmake:CMakeDetermineAdaCompiler.cmake
      language_support/cmake:CMakeTestAdaCompiler.cmake
      )
  endif(ENABLE_ada)

  if(ENABLE_d)
    set(LANG_SUPPORT_FILES ${LANG_SUPPORT_FILES}
      language_support/cmake:CMakeD_Copyright.txt
      language_support/cmake:CMakeDCompiler.cmake.in
      language_support/cmake:CMakeDInformation.cmake
      language_support/cmake:CMakeDetermineDCompiler.cmake
      language_support/cmake:CMakeTestDCompiler.cmake
      language_support/cmake/Platform:/Linux-dmd.cmake
      language_support/cmake/Platform:Linux-gdc.cmake
      language_support/cmake/Platform:Windows-dmd.cmake
      language_support/cmake/Platform:Windows-gdc.cmake
      )
  endif(ENABLE_d)

  if(ENABLE_f77 OR ENABLE_f95)
    if(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION EQUAL 6)
      set(LANG_SUPPORT_FILES ${LANG_SUPPORT_FILES}
	language_support/cmake-2.6:CMakeFortranInformation.cmake
	language_support/cmake-2.6/Platform:Cygwin-GNU-Fortran.cmake
	language_support/cmake-2.6/Platform:Windows-GNU-Fortran.cmake
	language_support/cmake-2.6/Platform:Windows-df.cmake
	language_support/cmake-2.6/Platform:Windows-f90.cmake
	)
    endif(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION EQUAL 6)
  endif(ENABLE_f77 OR ENABLE_f95)

  if(LANG_SUPPORT_FILES)
    foreach(LANG_SUPPORT_info ${LANG_SUPPORT_FILES})
      string(REGEX REPLACE "^(.*):.*$" "\\1" LANG_SUPPORT_DIR ${LANG_SUPPORT_info})
      string(REGEX REPLACE "^.*:(.*)$" "\\1" LANG_SUPPORT_FILE ${LANG_SUPPORT_info})
      install(FILES
	${CMAKE_SOURCE_DIR}/cmake/modules/${LANG_SUPPORT_DIR}/${LANG_SUPPORT_FILE}
	DESTINATION ${DATA_DIR}/examples/cmake/modules/${LANG_SUPPORT_DIR}
	)
    endforeach(LANG_SUPPORT_info ${LANG_SUPPORT_FILES})
  endif(LANG_SUPPORT_FILES)

else(CORE_BUILD)
  project(installed_plplot_examples NONE)

  CMAKE_MINIMUM_REQUIRED(VERSION 2.6.4 FATAL_ERROR)
  # Locations where PLplot cmake build system first looks for cmake modules.
  if(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION EQUAL 6)
    set(CMAKE_MODULE_PATH
      ${PROJECT_SOURCE_DIR}/cmake/modules
      ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake
      ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake-2.6
      )
  else(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION EQUAL 6)
    set(CMAKE_MODULE_PATH
      ${PROJECT_SOURCE_DIR}/cmake/modules
      ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake
      ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake-2.8
      )
  endif(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION EQUAL 6)

  #Configure variables and enable languages as needed.
  include(plplot_configure)

  include(export_plplot)

endif(CORE_BUILD)

if(CORE_BUILD AND BUILD_TEST)
  remove_definitions("-DHAVE_CONFIG_H")
endif(CORE_BUILD AND BUILD_TEST)

# language_info_LIST excludes the tk subdirectly since noninteractive
# tests and file results do not involve tk.  However, the tk
# subdirectory does have to be processed just like the rest
# so add it to this loop.
foreach(language_info ${language_info_LIST} "tk:tk:nothing")
  string(REGEX REPLACE "^(.*):.*:.*$" "\\1" language ${language_info})
  string(REGEX REPLACE "^.*:(.*):.*$" "\\1" subdir ${language_info})
  #message(STATUS "DEBUG: language, subdir = ${language},${subdir}")
  if(ENABLE_${language})
    add_subdirectory(${subdir})
    get_property(targets_examples_${language} GLOBAL PROPERTY TARGETS_examples_${language})
    get_property(files_examples_${language} GLOBAL PROPERTY FILES_examples_${language})
  endif(ENABLE_${language})
endforeach(language_info ${language_info_LIST} "tk:tk:nothing")

if(BUILD_TEST)
  # lena.pgm needed in build tree for the examples that are run from there.
  # Done at cmake time without dependencies because cannot see how this
  # file will ever change on short time scales.
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm 
    ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
    )

  if(CORE_BUILD)
    set(custom_test_command ${CMAKE_BINARY_DIR}/plplot_test/plplot-test.sh)
    set(custom_env EXAMPLES_DIR=${CMAKE_BINARY_DIR}/examples SRC_EXAMPLES_DIR=${CMAKE_SOURCE_DIR}/examples)
    set(java_custom_env ${custom_env} PLPLOT_JAVA_WRAP_DIR=${CMAKE_BINARY_DIR}/bindings/java/ PLPLOT_CLASSPATH=${CMAKE_BINARY_DIR}/examples/java/plplot.jar)
    set(compare_command ${CMAKE_BINARY_DIR}/plplot_test/test_diff.sh)
  else(CORE_BUILD)
    set(custom_test_command ${CMAKE_CURRENT_SOURCE_DIR}/plplot-test.sh)
    set(custom_env SRC_EXAMPLES_DIR=${CMAKE_CURRENT_SOURCE_DIR})
    set(java_custom_env ${custom_env})
    set(compare_command ${CMAKE_CURRENT_SOURCE_DIR}/test_diff.sh)
  endif(CORE_BUILD)

  if(PLD_psc)
    set(compare_file_depends ${custom_test_command})
    if(ENABLE_DYNDRIVERS)
      # The ps shared object implements both the psc (and ps) devices.
      get_property(FILE_DEPENDS_ps GLOBAL PROPERTY FILE_DEPENDS_ps_dyndriver)
      # FILE_DEPENDS_ps contains the filename associated with testing ps.
      # It is empty if not a core build.
      # The file dependency on ps takes care of all
      # target and file dependencies associated with ps.
      list(APPEND compare_file_depends ${FILE_DEPENDS_ps} ps)
    endif(ENABLE_DYNDRIVERS)

    set(diff_targets_LIST)
    set(diff_files_LIST)
    # language_info_LIST set above. Each list member consists of a
    # colon-separated language name, subdirectory name,
    # and the associated filename suffix used by plplot-test.sh.
    foreach(language_info ${language_info_LIST})
      string(REGEX REPLACE "^(.*):.*:.*$" "\\1" language ${language_info})
      string(REGEX REPLACE "^.*:.*:(.*)$" "\\1" suffix ${language_info})

      if(ENABLE_${language})
	set(compare_file_depends_${language} ${compare_file_depends})
        #message(STATUS "DEBUG: files_examples_${language} = ${file_examples_${language}}")
        #message(STATUS "DEBUG: targets_examples_${language} = ${targets_examples_${language}}")
	if(files_examples_${language})
	  # files_examples_${language} is true if the
	  # subdirectory created targets with add_custom_target which
	  # depended on an add_custom_command.  For this special case,
	  # you cannot use the created targets for file dependencies
	  # because of CMake implementation constraints.  Instead you
	  # must use _files_ (contained in files_examples_${language}
	  # for file dependencies.
	  # Another case covered here is when the subdirectory
          # contains non-generated examples files.  In this case also you must
	  # (of course) use _files_ for file dependencies.
	  list(APPEND compare_file_depends_${language} ${files_examples_${language}})
	else(files_examples_${language})
	  # files_examples_${language} is only false if the
	  # subdirectory creates targets with add_executable.  For
	  # this special case, CMake allows use of the created targets
	  # for file dependencies.
	  if(targets_examples_${language})
	    list(APPEND compare_file_depends_${language} ${targets_examples_${language}})
	  endif(targets_examples_${language})
	endif(files_examples_${language})
	#message(STATUS "DEBUG: compare_file_depends_${language} = ${compare_file_depends_${language}}")

	if(language STREQUAL "java")
	  set(environment ${java_custom_env})
	else(language STREQUAL "java")
	  set(environment ${custom_env})
	endif(language STREQUAL "java")

	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x01${suffix}.psc
	  COMMAND ${CMAKE_COMMAND} -E echo "Generate ${language} results for psc device"
	  COMMAND env ${environment} ${custom_test_command} --verbose --front-end=${language} --device=psc
	  DEPENDS ${compare_file_depends_${language}}
	  VERBATIM
	  )
	add_custom_target(test_${language}_psc
	  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/x01${suffix}.psc
	  )
	list(APPEND diff_targets_LIST test_${language}_psc)
	list(APPEND diff_files_LIST ${CMAKE_CURRENT_BINARY_DIR}/x01${suffix}.psc)
	# If the subdirectory used an add_custom_target (as indicated
	# by both files_examples_${language} and targets_examples_${language}
	# being true), then for that special case must add a target-level
	# dependency as demanded by the CMake implementation.
	if(files_examples_${language} AND targets_examples_${language})
	  #message(STATUS "DEBUG: targets_examples_${language} = ${targets_examples_${language}}")
	  add_dependencies(test_${language}_psc ${targets_examples_${language}})
	endif(files_examples_${language} AND targets_examples_${language})
	# FILE_DEPENDS_ps is empty if test_ps_dyndriver target is not defined.
	if(FILE_DEPENDS_ps)
	  add_dependencies(test_${language}_psc test_ps_dyndriver)
	endif(FILE_DEPENDS_ps)
      endif(ENABLE_${language})
    endforeach(language_info ${language_info_LIST})

    if(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
      # Note this target has complete file and target dependencies for
      # a comparison of stdout and PostScript results.
      add_custom_target(test_diff_psc COMMAND ${compare_command}
	DEPENDS ${diff_files_LIST}
	)
      add_dependencies(test_diff_psc ${diff_targets_LIST})
      set(noninteractive_targets_LIST test_diff_psc)
    else(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
      set(noninteractive_targets_LIST)
    endif(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
  else(PLD_psc)
    set(noninteractive_targets_LIST)
  endif(PLD_psc)

  # ENABLE_c is always ON by construction, but test it anyway for
  # consistency sake.
  if(ENABLE_c)
    set(device_depends ${custom_test_command})
    # See explanation above about what to do with files_examples_c and
    # targets_examples_c.
    if(files_examples_c)
      list(APPEND device_depends ${files_examples_c})
    else(files_examples_c)
      if(targets_examples_c)
	list(APPEND device_depends ${targets_examples_c})
      endif(targets_examples_c)
    endif(files_examples_c)

    # FILE_DEVICES_LIST is set in cmake/modules/drivers-finish.cmake
    # for all enabled _file_ devices and adjusted above for the
    # special case of the psttfc device.  It is thus available for the
    # core build.  It is also configured in plplot_configure.cmake and
    # is thus available for the installed examples tests.  Each list
    # member consists of a colon-separated device name, driver name,
    # and a Boolean variable that indicates if device has familied
    # output in plplot-test.sh.
    # N.B. device psc is deliberately excluded from list because it is
    # handled in a different way above.
    foreach(file_devices_info ${FILE_DEVICES_LIST})
      string(REGEX REPLACE "^(.*):.*:.*$" "\\1" device ${file_devices_info})
      string(REGEX REPLACE "^.*:(.*):.*$" "\\1" driver ${file_devices_info})
      string(REGEX REPLACE "^.*:.*:(.*)$" "\\1" familied ${file_devices_info})
      
      # If familying turned on for this device in plplot-test.sh, then the OUTPUT
      # files appear in the form x??c??.${device} rather than x??c.${device}.
      # Put in a representative placeholder for the familying index in the
      # OUTPUT name.
      if(familied)
	set(index 01)
      else(familied)
	set(index)
      endif(familied)
      
      set(file_device_depends_${device} ${device_depends})
      if(ENABLE_DYNDRIVERS)
	get_property(FILE_DEPENDS_${driver} 
	  GLOBAL PROPERTY FILE_DEPENDS_${driver}_dyndriver
	  )
	list(APPEND file_device_depends_${device}
	  ${FILE_DEPENDS_${driver}} ${driver}
	  )
      endif(ENABLE_DYNDRIVERS)
      #message("DEBUG: OUTPUT filename = x01c${index}.${device}")
      #message("DEBUG:file_device_depends_${device} = ${file_device_depends_${device}}")
      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x01c${index}.${device}
	COMMAND ${CMAKE_COMMAND} -E echo "Generate C results for ${device} file device"
	COMMAND env ${custom_env} ${custom_test_command} --verbose --front-end=c --device=${device}
	DEPENDS
	${file_device_depends_${device}}
	VERBATIM
	)
      add_custom_target(test_c_${device}
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/x01c${index}.${device}
	)
      list(APPEND noninteractive_targets_LIST test_c_${device})
      # Follow what was done above.
      if(files_examples_c AND targets_examples_c)
	add_dependencies(test_c_${device} ${targets_examples_c})
      endif(files_examples_c AND targets_examples_c)
      if(FILE_DEPENDS_${driver})
	add_dependencies(test_c_${device} test_${driver}_dyndriver)
      endif(FILE_DEPENDS_${driver})
    endforeach(file_devices_info ${FILE_DEVICES_LIST})

    if(ENABLE_DYNDRIVERS)
      # These required for some of the following special targets that are
      # not handled by the foreach loop over interactive devices.  Therefore,
      # this list of variables does not need to be complete over all
      # interactive devices.
      get_property(FILE_DEPENDS_cairo 
	GLOBAL PROPERTY FILE_DEPENDS_cairo_dyndriver
	)
      set(cairo_target ${FILE_DEPENDS_cairo} cairo)

      get_property(FILE_DEPENDS_gcw 
	GLOBAL PROPERTY FILE_DEPENDS_gcw_dyndriver
	)
      set(gcw_target ${FILE_DEPENDS_gcw} gcw)

      get_property(FILE_DEPENDS_qt
	GLOBAL PROPERTY FILE_DEPENDS_qt_dyndriver
	)
      set(qt_target ${FILE_DEPENDS_qt} qt)

      get_property(FILE_DEPENDS_wxwidgets 
	GLOBAL PROPERTY FILE_DEPENDS_wxwidgets_dyndriver
	)
      set(wxwidgets_target ${FILE_DEPENDS_wxwidgets} wxwidgets)

      get_property(FILE_DEPENDS_xwin
	GLOBAL PROPERTY FILE_DEPENDS_xwin_dyndriver
	)
      set(xwin_target ${FILE_DEPENDS_xwin} xwin)

    endif(ENABLE_DYNDRIVERS)
    if(PLD_extcairo)
	add_custom_command(
	  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ext-cairo-test.ps
	  COMMAND ext-cairo-test -drvopt set_background=1
	  DEPENDS 
	  ext-cairo-test
	  ${cairo_target}
	  )
      add_custom_target(test_extcairo
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ext-cairo-test.ps
	)
      if(FILE_DEPENDS_cairo)
	add_dependencies(test_extcairo test_cairo_dyndriver)
      endif(FILE_DEPENDS_cairo)
      list(APPEND noninteractive_targets_LIST test_extcairo)
    endif(PLD_extcairo)

    add_custom_target(test_noninteractive)
    if(noninteractive_targets_LIST)
      add_dependencies(test_noninteractive ${noninteractive_targets_LIST})
    endif(noninteractive_targets_LIST)

    set(interactive_targets_LIST)
    # INTERACTIVE_DEVICES_LIST is set in
    # cmake/modules/drivers-finish.cmake for all enabled _interactive_
    # devices and is thus available for the core build.  It is also
    # configured in plplot_configure.cmake and is thus available
    # for the installed examples tests.  Each list member consists of
    # a colon-separated device name and driver name.
    foreach(interactive_devices_info ${INTERACTIVE_DEVICES_LIST})
      string(REGEX REPLACE "^(.*):.*$" "\\1" device ${interactive_devices_info})
      string(REGEX REPLACE "^.*:(.*)$" "\\1" driver ${interactive_devices_info})
      
      set(interactive_device_depends_${device})
      # See explanation above about what to do with files_examples_c and
      # targets_examples_c.
      if(files_examples_c)
	list(APPEND interactive_device_depends_${device} ${files_examples_c})
      else(files_examples_c)
	if(targets_examples_c)
	  list(APPEND interactive_device_depends_${device} ${targets_examples_c})
	endif(targets_examples_c)
      endif(files_examples_c)

      if(ENABLE_DYNDRIVERS)
	get_property(FILE_DEPENDS_${driver}
	  GLOBAL PROPERTY FILE_DEPENDS_${driver}_dyndriver
	  )
	list(APPEND interactive_device_depends_${device}
	  ${FILE_DEPENDS_${driver}} ${driver}
	  )
	if(device STREQUAL "tk")
	  list(APPEND interactive_device_depends_${device}
	    ${FILE_DEPENDS_xwin} xwin
	    )
	endif(device STREQUAL "tk")
      endif(ENABLE_DYNDRIVERS)
      add_custom_target(test_c_${device}
	COMMAND ${CMAKE_COMMAND} -E echo "Generate C results for ${device} interactive device"
	COMMAND env ${custom_env} ${custom_test_command} --verbose --interactive --device=${device}
	DEPENDS
	${interactive_device_depends_${device}}
	VERBATIM
	)
      if(FILE_DEPENDS_${driver})
	add_dependencies(test_c_${device} test_${driver}_dyndriver)
      endif(FILE_DEPENDS_${driver})
      if(device STREQUAL "tk")
	if(CORE_BUILD)
	  add_dependencies(test_c_tk tclIndex_tk tclIndex_tcl plserver)
	  if(FILE_DEPENDS_xwin)
	    add_dependencies(test_c_${device} test_xwin_dyndriver)
	  endif(FILE_DEPENDS_xwin)
	else(CORE_BUILD)
	  add_dependencies(test_c_tk plserver)
	endif(CORE_BUILD)
      endif(device STREQUAL "tk")
      # Follow what was done above.
      if(files_examples_c AND targets_examples_c)
	add_dependencies(test_c_${device} ${targets_examples_c})
      endif(files_examples_c AND targets_examples_c)
      list(APPEND interactive_targets_LIST test_c_${device})

      if(ENABLE_octave)
	get_property(targets_examples_octave GLOBAL PROPERTY TARGETS_examples_octave)
	get_property(files_examples_octave GLOBAL PROPERTY FILES_examples_octave)
	add_custom_target(test_octave_${device}
	  COMMAND ${CMAKE_COMMAND} -E echo "Generate interactive octave results for ${device} interactive device"
	  COMMAND env ${custom_env} ${custom_test_command} --verbose --interactive_octave --device=${device}
	  DEPENDS
	  ${interactive_device_depends_${device}}
	  ${files_examples_octave}
	  VERBATIM
	  )
	add_dependencies(test_octave_${device} ${targets_examples_octave})
	if(FILE_DEPENDS_${driver})
	  add_dependencies(test_octave_${device} test_${driver}_dyndriver)
	endif(FILE_DEPENDS_${driver})
	if(device STREQUAL "tk")
	  if(CORE_BUILD)
	    add_dependencies(test_octave_tk tclIndex_tk tclIndex_tcl plserver)
	    if(FILE_DEPENDS_xwin)
	      add_dependencies(test_octave_tk test_xwin_dyndriver)
	    endif(FILE_DEPENDS_xwin)
	  else(CORE_BUILD)
	    add_dependencies(test_octave_tk plserver)
	  endif(CORE_BUILD)
	endif(device STREQUAL "tk")
	# Comment this out until interactive octave tests work better.
	#list(APPEND interactive_targets_LIST test_octave_${device})
      endif(ENABLE_octave)
    endforeach(interactive_devices_info ${INTERACTIVE_DEVICES_LIST})

    if(PLD_xcairo)
      add_custom_target(test_extXdrawable
	COMMAND extXdrawable_demo
	DEPENDS 
	extXdrawable_demo
	${cairo_target}
	)
      if(FILE_DEPENDS_cairo)
	add_dependencies(test_extXdrawable test_cairo_dyndriver)
      endif(FILE_DEPENDS_cairo)
      list(APPEND interactive_targets_LIST test_extXdrawable)
    endif(PLD_xcairo)

    if(ENABLE_gnome2)
      add_custom_target(test_plplotcanvas
	COMMAND plplotcanvas_demo
	COMMAND plplotcanvas_animation
	DEPENDS 
	plplotcanvas_demo
	plplotcanvas_animation
	${gcw_target}
	)
      if(FILE_DEPENDS_gcw)
	add_dependencies(test_plplotcanvas test_gcw_dyndriver)
      endif(FILE_DEPENDS_gcw)
      list(APPEND interactive_targets_LIST test_plplotcanvas)
    endif(ENABLE_gnome2)

  endif(ENABLE_c)

  if(ENABLE_cxx)
    if(ENABLE_wxwidgets)
      add_custom_target(test_wxPLplotDemo
	COMMAND wxPLplotDemo
	DEPENDS 
	wxPLplotDemo
	${wxwidgets_target}
	)
      if(FILE_DEPENDS_wxwidgets)
	add_dependencies(test_wxPLplotDemo test_wxwidgets_dyndriver)
      endif(FILE_DEPENDS_wxwidgets)
      list(APPEND interactive_targets_LIST test_wxPLplotDemo)
    endif(ENABLE_wxwidgets)

    if(PLD_extqt)
      add_custom_target(test_qt_example
	COMMAND qt_example
	DEPENDS 
	qt_example
	${qt_target}
	)
      if(FILE_DEPENDS_qt)
	add_dependencies(test_qt_example test_qt_dyndriver)
      endif(FILE_DEPENDS_qt)
      list(APPEND interactive_targets_LIST test_qt_example)
    endif(PLD_extqt)

  endif(ENABLE_cxx)

  if(ENABLE_pyqt4)
    if(CORE_BUILD)
      add_custom_target(test_pyqt4_example
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/python/pyqt4_example.py
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python
	)
      add_dependencies(test_pyqt4_example
	plplot_pyqt4
	python_examples
	_plplotcmodule
	qt
	)
      if(FILE_DEPENDS_qt)
	add_dependencies(test_pyqt4_example test_qt_dyndriver)
      endif(FILE_DEPENDS_qt)
    else(CORE_BUILD)
      add_custom_target(test_pyqt4_example
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/python/pyqt4_example.py
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
	)
      add_dependencies(test_pyqt4_example
	plplot_pyqt4
	_plplotcmodule
	qt
	)
    endif(CORE_BUILD)
    
    list(APPEND interactive_targets_LIST test_pyqt4_example)
  endif(ENABLE_pyqt4)

  if(ENABLE_pygcw)
    if(CORE_BUILD)
      add_custom_target(test_pygcw
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/python/plplotcanvas_demo.py
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/python/plplotcanvas_animation.py
	DEPENDS
	gcwmodule
	cplplotcanvasmodule
	${gcw_target}
	_plplotcmodule
	${CMAKE_CURRENT_BINARY_DIR}/python/plplotcanvas_demo.py
	${CMAKE_CURRENT_BINARY_DIR}/python/plplotcanvas_animation.py
	WORKING_DIRECTORY
	${CMAKE_CURRENT_BINARY_DIR}/python
	)
      add_dependencies(test_pygcw copy_plplotcanvas)
      if(FILE_DEPENDS_gcw)
	add_dependencies(test_pygcw test_gcw_dyndriver)
      endif(FILE_DEPENDS_gcw)
    else(CORE_BUILD)
      add_custom_target(test_pygcw
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/python/plplotcanvas_demo.py
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/python/plplotcanvas_animation.py
	DEPENDS
	gcwmodule
	cplplotcanvasmodule
	${gcw_target}
	_plplotcmodule
	WORKING_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/python
	)
    endif(CORE_BUILD)
    list(APPEND interactive_targets_LIST test_pygcw)
  endif(ENABLE_pygcw)

  if(ENABLE_tk AND PLD_tk)
    set(targets_examples_tk)

    add_custom_target(test_tk_01
      COMMAND xtk01 -f ${CMAKE_CURRENT_BINARY_DIR}/tk/tk01
      DEPENDS ${xwin_target}
      )
    add_dependencies(test_tk_01
      tclIndex_tcl
      tclIndex_tk
      xwin
      )
    if(FILE_DEPENDS_xwin)
      add_dependencies(test_tk_01 test_xwin_dyndriver)
    endif(FILE_DEPENDS_xwin)
    list(APPEND targets_examples_tk test_tk_01)
      
    add_custom_target(test_tk_03
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tk/tk03
      DEPENDS ${xwin_target}
      )
    add_dependencies(test_tk_03
      plserver
      tclIndex_tcl
      tclIndex_tk
      xwin
      )
    if(FILE_DEPENDS_xwin)
      add_dependencies(test_tk_03 test_xwin_dyndriver)
    endif(FILE_DEPENDS_xwin)
    list(APPEND targets_examples_tk test_tk_03)

    if(CORE_BUILD)
      add_custom_target(test_tk_plgrid
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tk/plgrid
	DEPENDS ${xwin_target}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tcl
	)
    else(CORE_BUILD)
      add_custom_target(test_tk_plgrid
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tk/plgrid
	DEPENDS ${xwin_target}
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tcl
	)
    endif(CORE_BUILD)
    add_dependencies(test_tk_plgrid
      plserver
      tcl_examples
      tclIndex_tcl
      tclIndex_tk
      xwin
      )
    if(FILE_DEPENDS_xwin)
      add_dependencies(test_tk_plgrid test_xwin_dyndriver)
    endif(FILE_DEPENDS_xwin)
    list(APPEND targets_examples_tk test_tk_plgrid)

    if(CORE_BUILD)
      add_custom_target(test_tk_standard_examples
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tk/standard_examples
	DEPENDS ${xwin_target}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tk
	)
    else(CORE_BUILD)
      add_custom_target(test_tk_standard_examples
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tk/standard_examples
	DEPENDS ${xwin_target}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tk
	)
    endif(CORE_BUILD)
    add_dependencies(test_tk_standard_examples
      plserver
      tcl_examples
      tclIndex_tcl
      tclIndex_tk
      tclIndex_examples_tk
      xwin
      )
    if(FILE_DEPENDS_xwin)
      add_dependencies(test_tk_standard_examples test_xwin_dyndriver)
    endif(FILE_DEPENDS_xwin)
    list(APPEND targets_examples_tk test_tk_standard_examples)

    if(ENABLE_itk)
      add_custom_target(test_tk_02
	COMMAND xtk02 -f ${CMAKE_CURRENT_BINARY_DIR}/tk/tk02
	DEPENDS ${xwin_target}
	)
      add_dependencies(test_tk_02
	tclIndex_tcl
	tclIndex_tk
	xwin
	)
      if(FILE_DEPENDS_xwin)
	add_dependencies(test_tk_02 test_xwin_dyndriver)
      endif(FILE_DEPENDS_xwin)
      list(APPEND targets_examples_tk test_tk_02)
      
      add_custom_target(test_tk_04
	COMMAND xtk04 -f ${CMAKE_CURRENT_BINARY_DIR}/tk/tk04
	DEPENDS ${xwin_target}
	)
      add_dependencies(test_tk_04
	tclIndex_tcl
	tclIndex_tk
	xwin
	)
      if(FILE_DEPENDS_xwin)
	add_dependencies(test_tk_04 test_xwin_dyndriver)
      endif(FILE_DEPENDS_xwin)
      list(APPEND targets_examples_tk test_tk_04)
    endif(ENABLE_itk)
      
  endif(ENABLE_tk AND PLD_tk)

  if(targets_examples_tk)
    list(APPEND interactive_targets_LIST ${targets_examples_tk})
  endif(targets_examples_tk)

  add_custom_target(test_interactive)
  if(interactive_targets_LIST)
    add_dependencies(test_interactive ${interactive_targets_LIST})
  endif(interactive_targets_LIST)

  # Custom target to test all of qt devices/special examples.
  # Must be maintained.
  add_custom_target(test_all_qt)
  add_dependencies(test_all_qt
    test_c_bmpqt
    test_c_epsqt
    test_c_jpgqt
    test_c_pdfqt
    test_c_pngqt
    test_c_ppmqt
    test_c_qtwidget
    test_c_svgqt
    test_c_tiffqt
    #test_octave_qtwidget
    test_pyqt4_example
    test_qt_example
    )

  # Custom target to test all of cairo devices/special examples.
  # Must be maintained.
  add_custom_target(test_all_cairo)
  add_dependencies(test_all_cairo
    test_c_pdfcairo
    test_c_pngcairo
    test_c_pscairo
    test_c_svgcairo
    test_extcairo
    test_c_xcairo
    test_extXdrawable
    )

endif(BUILD_TEST)

if(NOT CORE_BUILD)
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

endif(NOT CORE_BUILD)
