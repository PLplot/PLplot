# examples/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2017 Alan W. Irwin
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

  # cxx and tk subdirectories are special cases.
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
    set(PSC_F95_COMMENT "#")
    set(PSC_JAVA_COMMENT "#")
    set(PSC_OCAML_COMMENT "#")
    set(PSC_OCTAVE_COMMENT "#")
    set(PSC_PERL_COMMENT "#")
    set(PSC_PYTHON_COMMENT "#")
    set(PSC_TCL_COMMENT "#")
    set(PSC_LUA_COMMENT "#")
  endif(PLD_ps)

  if(NOT(ENABLE_cxx AND (PLD_ps OR ENABLE_qt OR ENABLE_wxwidgets)))
    set(CXX_COMMENT "#")
  endif(NOT(ENABLE_cxx AND (PLD_ps OR ENABLE_qt OR ENABLE_wxwidgets)))

  if(NOT ENABLE_tkX)
    set(TK_COMMENT "#")
  endif(NOT ENABLE_tkX)

  if(NOT PLD_pdfcairo)
    set(PDFCAIRO_COMMENT "#")
  endif(NOT PLD_pdfcairo)

  if(NOT PLD_pngcairo)
    set(PNGCAIRO_COMMENT "#")
  endif(NOT PLD_pngcairo)

  if(NOT PLD_pscairo)
    set(PSCAIRO_COMMENT "#")
  endif(NOT PLD_pscairo)

  if(NOT PLD_epscairo)
    set(EPSCAIRO_COMMENT "#")
  endif(NOT PLD_epscairo)

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

  # Note that the results configured in the Requires,
  # Requires.private, Libs, and Libs.private lines of the PLplot *.pc
  # files used in our traditional build of the installed examples
  # depend on whether NON_TRANSITIVE is ON or OFF.
  if(BUILD_SHARED_LIBS)
    # pkg-config will not use the --static pkg-config option for the
    # traditional installed examples build, i.e., the configured
    # Requires.private and Libs.private results will be completely
    # ignored in the PLplot *.pc files.
    set(PC_STATIC_OPTION)
  else(BUILD_SHARED_LIBS)
    # pkg-config will use the --static pkg-config option for the
    # traditional installed examples build, i.e., the configured
    # Requires.private results will be appended to the
    # Requires results and the configured Libs.private results will
    # be appended to the Libs results.
    set(PC_STATIC_OPTION "--static")
  endif(BUILD_SHARED_LIBS)

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
    get_target_property(_LIB_INSTALL_RPATH plplot INSTALL_RPATH)
    if(_LIB_INSTALL_RPATH)
      string(REGEX REPLACE ";" ":" LIB_INSTALL_RPATH "${_LIB_INSTALL_RPATH}")
      set(RPATHCMD "-Wl,-rpath -Wl,\"${LIB_INSTALL_RPATH}\"")
      if(PLD_extqt)
	if(ENABLE_DYNDRIVERS)
	  get_target_property(_qt_INSTALL_RPATH qt INSTALL_RPATH)
	  if(_qt_INSTALL_RPATH)
	    string(REGEX REPLACE ";" ":" qt_INSTALL_RPATH "${_qt_INSTALL_RPATH}")
	    set(qt_RPATHCMD "-Wl,-rpath -Wl,\"${LIB_INSTALL_RPATH}:${qt_INSTALL_RPATH}:${DRV_DIR}\"")
	  else(_qt_INSTALL_RPATH)
	    set(qt_RPATHCMD)
	  endif(_qt_INSTALL_RPATH)
	else(ENABLE_DYNDRIVERS)
	  set(qt_RPATHCMD ${RPATHCMD})
	endif(ENABLE_DYNDRIVERS)
      endif(PLD_extqt)
      if(ENABLE_tcl)
	get_target_property(_plplottcltk_Main_INSTALL_RPATH plplottcltk_Main INSTALL_RPATH)
	if(_plplottcltk_Main_INSTALL_RPATH)
	  string(REGEX REPLACE ";" ":" plplottcltk_Main_INSTALL_RPATH "${_plplottcltk_Main_INSTALL_RPATH}")
	  set(plplottcltk_Main_RPATHCMD "-Wl,-rpath -Wl,\"${LIB_INSTALL_RPATH}:${plplottcltk_Main_INSTALL_RPATH}\"")
	else(_plplottcltk_Main_INSTALL_RPATH)
	  set(plplottcltk_Main_RPATHCMD)
	endif(_plplottcltk_Main_INSTALL_RPATH)
      endif(ENABLE_tcl)
    else(_LIB_INSTALL_RPATH)
      set(RPATHCMD)
      set(qt_RPATHCMD)
      set(plplottcltk_Main_RPATHCMD)
    endif(_LIB_INSTALL_RPATH)

  endif(USE_RPATH)

  # Install Chloe.pgm in installed examples directory so that all implementations
  # of example 20 in the various examples subdirectories can conveniently
  # access this file.
  install(
    FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/Chloe.pgm
    ${CMAKE_CURRENT_SOURCE_DIR}/README.Chloe
    DESTINATION
    ${DATA_DIR}/examples
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
    f95:f95:f95
    java:java:j
    lua:lua:lua
    ocaml:ocaml:ocaml
    octave:octave:o
    pdl:perl:pdl
    python:python:p
    tcl:tcl:t
    plrender:c:plm
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

  # function support for CMake-based build system for installed examples.
  install(FILES
    ${CMAKE_SOURCE_DIR}/cmake/modules/plplot_functions.cmake
    DESTINATION ${DATA_DIR}/examples/cmake/modules
    )

  # pkg-config support for CMake-based build system for installed examples.
  install(FILES
    ${CMAKE_SOURCE_DIR}/cmake/modules/pkg-config.cmake
    DESTINATION ${DATA_DIR}/examples/cmake/modules
    )

  # Install soft-landing support for compiler detection/testing.
  install(FILES
    ${CMAKE_SOURCE_DIR}/cmake/modules/language_support.cmake
    DESTINATION ${DATA_DIR}/examples/cmake/modules
    )

  # Install qt support file.
  if(ANY_QT_DEVICE)
    install(FILES
      ${CMAKE_SOURCE_DIR}/cmake/modules/ndp_UseQt4.cmake
      DESTINATION ${DATA_DIR}/examples/cmake/modules
      )
  endif(ANY_QT_DEVICE)

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
      language_support/cmake/Platform:CYGWIN-GNU-Ada.cmake
      language_support/cmake/Platform:Darwin-GNU-Ada.cmake
      language_support/cmake/Platform:Linux-GNU-Ada.cmake
      language_support/cmake/Platform:Windows-GNU-Ada.cmake
      language_support/cmake/Compiler:GNU-Ada.cmake
      )
  endif(ENABLE_ada)

  if(ENABLE_d)
    set(LANG_SUPPORT_FILES ${LANG_SUPPORT_FILES}
      language_support/cmake:CMakeD_Copyright.txt
      language_support/cmake:CMakeDCompiler.cmake.in
      language_support/cmake:CMakeDInformation.cmake
      language_support/cmake:CMakeDetermineDCompiler.cmake
      language_support/cmake:CMakeTestDCompiler.cmake
      language_support/cmake/Platform:Darwin-dmd.cmake
      language_support/cmake/Platform:Linux-dmd.cmake
      language_support/cmake/Platform:Linux-gdc.cmake
      language_support/cmake/Platform:Windows-dmd.cmake
      language_support/cmake/Platform:Windows-gdc.cmake
      language_support/cmake/Platform:Windows-gdc.cmake
      )
  endif(ENABLE_d)

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

  # MAINTENANCE 2016-12-12
  # This logic copied exactly (including version numbers which change
  # from time to time) from the top-level CMakeLists.txt file except
  # for the name of the project.
  cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
  cmake_policy(VERSION 3.0.2)
  project(installed_plplot_examples NONE)
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    if(NOT CMAKE_VERSION VERSION_LESS 3.2 AND CMAKE_VERSION VERSION_LESS 3.3.2)
      message(STATUS "WARNING: CMAKE_VERSION = ${CMAKE_VERSION} is in the range from 3.2 through 3.3.1 which has
   a compromised find ability that was fixed in 3.3.2.  Please upgrade to 3.3.2 or greater.")
    endif(NOT CMAKE_VERSION VERSION_LESS 3.2 AND CMAKE_VERSION VERSION_LESS 3.3.2)
  else(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    # Latest CMake version as of 2016-02 for all platforms
    # other than Linux.
    cmake_minimum_required(VERSION 3.6.2 FATAL_ERROR)
    # Keep policy consistent for all platforms.
    cmake_policy(VERSION 3.0.2)
  endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")

  # It is a fatal error if no working C compiler is available to build
  # the PLplot core C library and core C examples.  All other compilers
  # required by our bindings are optional in that if no working compiler
  # of the kind needed is available, the associated bindings and
  # examples are disabled.
  enable_language(C)

  message(STATUS "CMake version = ${CMAKE_VERSION}")
  message(STATUS "CMAKE_SYSTEM_NAME = ${CMAKE_SYSTEM_NAME}")

  message(STATUS "CMAKE_PLATFORM_INFO_DIR = ${CMAKE_PLATFORM_INFO_DIR}")
  # Locations where PLplot cmake build system first looks for cmake modules.
  set(CMAKE_MODULE_PATH
    ${PROJECT_SOURCE_DIR}/cmake/modules
    ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake
    ${PROJECT_SOURCE_DIR}/cmake/modules/language_support/cmake-2.8
    )

  #Configure variables and enable languages as needed.
  include(plplot_configure)

  find_package(plplot)

endif(CORE_BUILD)

if(CORE_BUILD AND BUILD_TEST)
  remove_definitions("-DPLPLOT_HAVE_CONFIG_H")
endif(CORE_BUILD AND BUILD_TEST)

# Decide on device to be used for generic interactive tests.

if(PLD_xwin)
  # Typically for Unix with X
  set(generic_interactive_device xwin)
elseif(PLD_ntk)
  # Typically for all platforms if ntk is available but not xwin
  set(generic_interactive_device ntk)
elseif(PLD_wingcc)
  # Typically for Windows if -dev ntk is not available
  set(generic_interactive_device wingcc)
else(PLD_xwin)
  set(generic_interactive_device)
endif(PLD_xwin)

# language_info_LIST excludes the tk subdirectly since noninteractive
# tests and file results do not involve tk.  However, the tk
# subdirectory does have to be processed just like the rest
# so add it to this loop.
foreach(language_info ${language_info_LIST} "tk:tk:nothing")
  string(REGEX REPLACE "^(.*):.*:.*$" "\\1" language ${language_info})
  string(REGEX REPLACE "^.*:(.*):.*$" "\\1" subdir ${language_info})
  #message(STATUS "DEBUG: language, subdir = ${language},${subdir}")
  if(ENABLE_${language})
    if(NOT ${language} STREQUAL "plrender")
      if(${language} STREQUAL "f95")
	if(PL_DEPRECATED_f95)
	  add_subdirectory(old_${subdir})
	else(PL_DEPRECATED_f95)
	  add_subdirectory(${subdir})
	endif(PL_DEPRECATED_f95)
      elseif(${language} STREQUAL "tcl" OR ${language} STREQUAL "tk")
	add_subdirectory(${TCL_TK_SRC_PREFIX}${subdir} ${subdir})
      else(${language} STREQUAL "f95")
	add_subdirectory(${subdir})
      endif(${language} STREQUAL "f95")
    endif(NOT ${language} STREQUAL "plrender")
    get_property(targets_examples_${language} GLOBAL PROPERTY TARGETS_examples_${language})
    get_property(files_examples_${language} GLOBAL PROPERTY FILES_examples_${language})
  endif(ENABLE_${language})
endforeach(language_info ${language_info_LIST} "tk:tk:nothing")

# Set up TARGET_tclIndex_tcl and TARGET_tclIndex_tk to account for
# the non-core build where the required files are installed and
# these actual targets do not exist.
if(TARGET tclIndex_tcl)
  set(TARGET_tclIndex_tcl tclIndex_tcl)
else(TARGET tclIndex_tcl)
  set(TARGET_tclIndex_tcl)
endif(TARGET tclIndex_tcl)
if(TARGET tclIndex_tk)
  set(TARGET_tclIndex_tk tclIndex_tk)
else(TARGET tclIndex_tk)
  set(TARGET_tclIndex_tk)
endif(TARGET tclIndex_tk)

if(WORKAROUND_FILE_GENERATE_BUG)
  # This bug workaround must be set up after last call to configure_file_generate
  # (which currently happens in plplot_test).
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/touched_workaround_file_generate_bug
    COMMAND ${CMAKE_COMMAND} -E touch
    ${CMAKE_CURRENT_BINARY_DIR}/touched_workaround_file_generate_bug
    )

  get_property(files_WORKAROUND_FILE_GENERATE_BUG GLOBAL PROPERTY FILES_WORKAROUND_FILE_GENERATE_BUG)

  foreach(output_file ${files_WORKAROUND_FILE_GENERATE_BUG})
    set(intermediate_file ${output_file}${configure_file_generate_suffix})
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/touched_workaround_file_generate_bug
      COMMAND chmod --reference=${intermediate_file} ${output_file}
      APPEND
      )
  endforeach(output_file ${files_WORKAROUND_FILE_GENERATE_BUG})
  # ALL option means the permissions will be correct on
  # all installed files and everything tested by ctest (since
  # all target must be run before ctest).  However,
  # will have to use add_dependencies(<target> workaround_file_generate_bug)
  # in various spots in the examples subtree within the build tree to
  # apply this permissions workaround when running PLplot test targets
  # such as test_interactive.
  add_custom_target(
    workaround_file_generate_bug ALL
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/touched_workaround_file_generate_bug
    )
else(WORKAROUND_FILE_GENERATE_BUG)
  # if WORKAROUND_FILE_GENERATE_BUG is false (which happens some times
  # for both core builds and installed examples builds) then create a
  # vacuous target (needed below).
  add_custom_target(workaround_file_generate_bug)
endif(WORKAROUND_FILE_GENERATE_BUG)

if(BUILD_TEST)
  # Chloe.pgm needed in build tree for the examples that are run from there.
  # Done at cmake time without dependencies because cannot see how this
  # file will ever change on short time scales.
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/Chloe.pgm
    ${CMAKE_CURRENT_BINARY_DIR}/Chloe.pgm
    )

  find_program(VALGRIND_EXECUTABLE valgrind)
  if(VALGRIND_EXECUTABLE)
    option(VALGRIND_ALL_TESTS "Apply valgrind to all runs of plplot_test/plplot-test.sh" OFF)
    if(VALGRIND_ALL_TESTS)
      set(plplot_test_debug --debug)
    else(VALGRIND_ALL_TESTS)
      set(plplot_test_debug)
    endif(VALGRIND_ALL_TESTS)
  else(VALGRIND_EXECUTABLE)
    set(plplot_test_debug)
  endif(VALGRIND_EXECUTABLE)

  set(TEST_EXAMPLES_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/test_examples_output_dir)
  file(MAKE_DIRECTORY ${TEST_EXAMPLES_OUTPUT_DIR})
  if(CORE_BUILD)
    set(custom_test_script ${CMAKE_BINARY_DIR}/plplot_test/plplot-test.sh)
    set(custom_env EXAMPLES_DIR=${CMAKE_BINARY_DIR}/examples SRC_EXAMPLES_DIR=${CMAKE_SOURCE_DIR}/examples OUTPUT_DIR=${TEST_EXAMPLES_OUTPUT_DIR})
    set(java_custom_env ${custom_env} PLPLOT_JAVA_WRAP_DIR=${CMAKE_BINARY_DIR}/bindings/java/ PLPLOT_CLASSPATH=${CMAKE_BINARY_DIR}/examples/java/plplot.jar)
    set(compare_script ${CMAKE_BINARY_DIR}/plplot_test/test_diff.sh)
  else(CORE_BUILD)
    set(custom_test_script ${CMAKE_CURRENT_SOURCE_DIR}/plplot-test.sh)
    set(custom_env SRC_EXAMPLES_DIR=${CMAKE_CURRENT_SOURCE_DIR} OUTPUT_DIR=${TEST_EXAMPLES_OUTPUT_DIR})
    set(java_custom_env ${custom_env})
    set(compare_script ${CMAKE_CURRENT_SOURCE_DIR}/test_diff.sh)
  endif(CORE_BUILD)

  set(custom_test_command ${SH_EXECUTABLE} ${custom_test_script})
  set(compare_command ${SH_EXECUTABLE} ${compare_script})

  set(tcl_targets_LIST)
  if(PLD_psc)
    set(compare_file_depends ${custom_test_script})
    if(ENABLE_DYNDRIVERS)
      # The ps shared object implements both the psc (and ps) devices.
      get_property(ps_file_depends GLOBAL PROPERTY FILE_DEPENDS_ps_dyndriver)
      # ps_file_depends contains the filename associated with testing ps.
      # It is empty if not a core build.
      # The file dependency on ps takes care of all
      # target and file dependencies associated with ps.
      list(APPEND compare_file_depends ${ps_file_depends} ps)
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

	list_example_files(${TEST_EXAMPLES_OUTPUT_DIR} psc ${suffix} output_list)
	add_custom_command(
	  OUTPUT ${output_list}
	  COMMAND ${CMAKE_COMMAND} -E echo "Generate ${language} results for psc device"
	  COMMAND env ${environment} ${custom_test_command} --verbose --front-end=${language} --device=psc ${plplot_test_debug}
	  DEPENDS ${compare_file_depends_${language}}
	  VERBATIM
	  )
	add_custom_target(test_${language}_psc
	  DEPENDS ${output_list}
	  )
	list(APPEND diff_targets_LIST test_${language}_psc)
	list(APPEND diff_files_LIST ${output_list})

	if(language STREQUAL "tcl")
	  list(APPEND tcl_targets_LIST test_${language}_psc)
	  add_dependencies(test_${language}_psc workaround_file_generate_bug)
	endif(language STREQUAL "tcl")

	# If the subdirectory used an add_custom_target (as indicated
	# by both files_examples_${language} and targets_examples_${language}
	# being true), then for that special case must add a target-level
	# dependency as demanded by the CMake implementation.
	if(files_examples_${language} AND targets_examples_${language})
	  #message(STATUS "DEBUG: targets_examples_${language} = ${targets_examples_${language}}")
	  add_dependencies(test_${language}_psc ${targets_examples_${language}})
	endif(files_examples_${language} AND targets_examples_${language})
	# ps_file_depends is empty if test_ps_dyndriver target is not defined.
	if(TARGET test_ps_dyndriver)
	  add_dependencies(test_${language}_psc test_ps_dyndriver)
	endif(TARGET test_ps_dyndriver)
      endif(ENABLE_${language})
    endforeach(language_info ${language_info_LIST})

    if(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
      # Note this target has complete file and target dependencies for
      # a comparison of stdout and PostScript results.
      add_custom_target(test_diff_psc COMMAND ${compare_command}
	DEPENDS ${diff_files_LIST}
	WORKING_DIRECTORY ${TEST_EXAMPLES_OUTPUT_DIR}
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
    set(device_depends ${custom_test_script})
    # See explanation above about what to do with files_examples_${language} and
    # targets_examples_${language}.
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

      set(${device}_file_device_depends ${device_depends})
      if(ENABLE_DYNDRIVERS)
	get_property(${driver}_file_depends
	  GLOBAL PROPERTY FILE_DEPENDS_${driver}_dyndriver
	  )
	list(APPEND ${device}_file_device_depends
	  ${${driver}_file_depends} ${driver}
	  )
      endif(ENABLE_DYNDRIVERS)
      #message("DEBUG:${device}_file_device_depends = ${${device}_file_device_depends}")
      list_example_files(${TEST_EXAMPLES_OUTPUT_DIR} ${device} c output_list)
      add_custom_command(
	OUTPUT ${output_list}
	COMMAND ${CMAKE_COMMAND} -E echo "Generate C results for ${device} file device"
	COMMAND env ${custom_env} ${custom_test_command} --verbose --front-end=c --device=${device} ${plplot_test_debug}
	DEPENDS
	${${device}_file_device_depends}
	VERBATIM
	)
      add_custom_target(test_c_${device}
	DEPENDS ${output_list}
	)
      if(TARGET test_${driver}_dyndriver)
	add_dependencies(test_c_${device} test_${driver}_dyndriver)
      endif(TARGET test_${driver}_dyndriver)

      list(APPEND noninteractive_targets_LIST test_c_${device})
      # Follow what was done above.
      if(files_examples_c AND targets_examples_c)
	add_dependencies(test_c_${device} ${targets_examples_c})
      endif(files_examples_c AND targets_examples_c)
      endforeach(file_devices_info ${FILE_DEVICES_LIST})

    set(interactive_targets_LIST)
    set(tk_targets_LIST)
    set(qt_targets_LIST)

    # EXTERNAL_DEVICES_LIST determined here from what external devices
    # are enabled.  The required PLD_* data are available both for the
    # core build and (via the configured plplot_configure.cmake) the
    # installed examples build.  Each list member consists of a
    # colon-separated external device name and driver name.

    set(EXTERNAL_DEVICES_LIST)
    if(PLD_extcairo)
      list(APPEND EXTERNAL_DEVICES_LIST "extcairo:cairo")
    endif(PLD_extcairo)
    if(PLD_extqt)
      list(APPEND EXTERNAL_DEVICES_LIST "extqt:qt")
    endif(PLD_extqt)

    # <driver>_targets determined in the loop below over all enabled
    # external devices and interactive devices are used both for (one)
    # custom command DEPENDS below and for several add_dependencies
    # commands below.

    # N.B. INTERACTIVE_DEVICES_LIST is set in
    # cmake/modules/drivers-finish.cmake for all enabled _interactive_
    # devices and is thus available for the core build.  It is also
    # configured in plplot_configure.cmake and is thus available
    # for the installed examples tests.  Each list member consists of
    # a colon-separated device name and driver name.

    foreach(interactive_devices_info IN LISTS EXTERNAL_DEVICES_LIST INTERACTIVE_DEVICES_LIST)
      string(REGEX REPLACE "^(.*):.*$" "\\1" device ${interactive_devices_info})
      string(REGEX REPLACE "^.*:(.*)$" "\\1" driver ${interactive_devices_info})

      # Only process first instance of a particular driver in the list because multiple
      # devices in the list can map to a single driver.
      if(NOT ${driver}_targets)
	if(ENABLE_DYNDRIVERS)
	  set(${driver}_targets ${driver})
	  if(TARGET test_${driver}_dyndriver)
	    list(APPEND ${driver}_targets test_${driver}_dyndriver)
	  endif(TARGET test_${driver}_dyndriver)
	  if(driver STREQUAL "tk" AND TARGET xwin)
	    list(APPEND ${driver}_targets xwin)
	    if(TARGET test_xwin_dyndriver)
	      list(APPEND ${driver}_targets test_xwin_dyndriver)
	    endif(TARGET test_xwin_dyndriver)
	  endif(driver STREQUAL "tk" AND TARGET xwin)
	else(ENABLE_DYNDRIVERS)
	  set(${driver}_targets plplot)
	endif(ENABLE_DYNDRIVERS)

	if(driver STREQUAL "tk")
	  list(APPEND ${driver}_targets ${TARGET_tclIndex_tk} ${TARGET_tclIndex_tcl} plserver)
	endif(driver STREQUAL "tk")

	if(driver STREQUAL "wxwidgets" AND TARGET wxPLViewer)
	  # Append wxPLViewer to the list of wxwidgets targets so that application
	  # will be built when needed.
	  list(APPEND ${driver}_targets wxPLViewer)
	endif(driver STREQUAL "wxwidgets" AND TARGET wxPLViewer)

      endif(NOT ${driver}_targets)
    endforeach(interactive_devices_info IN LISTS EXTERNAL_DEVICES_LIST EXTERNAL_DEVICES_LIST)

    # Create some custom interactive test targets using list of interactive devices.
    foreach(interactive_devices_info IN LISTS INTERACTIVE_DEVICES_LIST)
      string(REGEX REPLACE "^(.*):.*$" "\\1" device ${interactive_devices_info})
      string(REGEX REPLACE "^.*:(.*)$" "\\1" driver ${interactive_devices_info})

      add_custom_target(test_c_${device}
	COMMAND ${CMAKE_COMMAND} -E echo "Generate C results for ${device} interactive device"
	COMMAND env ${custom_env} ${custom_test_command} --verbose --interactive --device=${device} ${plplot_test_debug}
	VERBATIM
	)
      add_dependencies(test_c_${device} ${${driver}_targets} ${targets_examples_c})
      if(PLPLOT_USE_QT5 AND device STREQUAL "qtwidget")
#	message(STATUS "WARNING: The test_c_qtwidget target can be run independently
#   but is not reliable (see <https://sourceforge.net/p/plplot/bugs/158/>) when
#   PLPLOT_USE_QT5 is set to ${PLPLOT_USE_QT5}.  So until this issue can be straightened out
#   temporarily exclude this target from being a dependency of other more general interactive
#   test targets")
	list(APPEND interactive_targets_LIST test_c_${device})
	list(APPEND qt_targets_LIST test_c_${device})
      else(PLPLOT_USE_QT5 AND device STREQUAL "qtwidget")
	list(APPEND interactive_targets_LIST test_c_${device})
	if(device STREQUAL "tk" OR device STREQUAL "ntk")
	  list(APPEND tk_targets_LIST test_c_${device})
	elseif(device STREQUAL "qtwidget")
	  list(APPEND qt_targets_LIST test_c_${device})
	endif(device STREQUAL "tk" OR device STREQUAL "ntk")
      endif(PLPLOT_USE_QT5 AND device STREQUAL "qtwidget")

      if(ENABLE_octave)
	get_property(targets_examples_octave GLOBAL PROPERTY TARGETS_examples_octave)
	add_custom_target(test_octave_${device}
	  COMMAND ${CMAKE_COMMAND} -E echo "Generate interactive octave results for ${device} interactive device"
	  COMMAND env ${custom_env} ${custom_test_command} --verbose --interactive_octave --device=${device} ${plplot_test_debug}
	  VERBATIM
	  )
	add_dependencies(test_octave_${device} ${targets_examples_octave} ${${driver}_targets})
	message(STATUS "WARNING: The test_octave_${device} target can be run independently but does not
   work reliably so it is temporarily excluded from being a dependency of other
   more general interactive test targets")
	#list(APPEND interactive_targets_LIST test_octave_${device})
	#if(device STREQUAL "tk" OR device STREQUAL "ntk")
	#  list(APPEND tk_targets_LIST test_octave_${device})
	#endif(device STREQUAL "tk" OR device STREQUAL "ntk")
      endif(ENABLE_octave)
    endforeach(interactive_devices_info IN LISTS INTERACTIVE_DEVICES_LIST)

    if(ENABLE_DYNDRIVERS)

      if(TARGET tkwin)
	# tkwin is an external device rather than an interactive one
	# so tkwin_targets (needed for a special case below) is not
	# defined in the above loop and must be defined here instead.
	set(tkwin_targets tkwin)
	if(TARGET test_tkwin_dyndriver)
	  list(APPEND tkwin_targets test_tkwin_dyndriver)
	endif(TARGET test_tkwin_dyndriver)
      endif(TARGET tkwin)

      if(TARGET cairo)
	# <driver>_file_depends only needed for custom command DEPENDS.  This is
	# the only one of these interactive devices that has a noninteractive
	# test and associated custom_command so it is the only interactive
	# device where <driver>_file_depends needs to be defined.
	get_property(cairo_file_depends
	  GLOBAL PROPERTY FILE_DEPENDS_cairo_dyndriver
	  )
      endif(TARGET cairo)

      # This special target needed for some cases below.
      if(TARGET plplottcltk)
	set(plplottcltk_targets plplottcltk)
      endif(TARGET plplottcltk)

    else(ENABLE_DYNDRIVERS)

      # MAINTENANCE: the variables set here must be the same as
      # in the above if block
      set(tkwin_targets plplot)
      set(cairo_file_depends)
      set(plplottcltk_targets plplot)
    endif(ENABLE_DYNDRIVERS)

    if(PLD_extcairo)
      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/ext-cairo-test.ps
	COMMAND ext-cairo-test -drvopt set_background=1
	DEPENDS
	ext-cairo-test
	${cairo_file_depends}
	${cairo_targets}
	)
      add_custom_target(test_extcairo
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/ext-cairo-test.ps
	)
      if(TARGET test_cairo_dyndriver)
	add_dependencies(test_extcairo test_cairo_dyndriver)
      endif(TARGET test_cairo_dyndriver)
      list(APPEND noninteractive_targets_LIST test_extcairo)
    endif(PLD_extcairo)

    add_custom_target(test_noninteractive)
    if(noninteractive_targets_LIST)
      add_dependencies(test_noninteractive ${noninteractive_targets_LIST})
    endif(noninteractive_targets_LIST)

    if(PLD_xcairo AND TARGET extXdrawable_demo)
      add_custom_target(test_extXdrawable
	COMMAND extXdrawable_demo
	)
      add_dependencies(test_extXdrawable extXdrawable_demo ${cairo_targets})
      list(APPEND interactive_targets_LIST test_extXdrawable)
    endif(PLD_xcairo AND TARGET extXdrawable_demo)

  endif(ENABLE_c)

  if(ENABLE_cxx)
    if(ENABLE_wxwidgets)
      add_custom_target(test_wxPLplotDemo
	COMMAND wxPLplotDemo
	)
      add_dependencies(test_wxPLplotDemo wxPLplotDemo ${wxwidgets_targets})
      list(APPEND interactive_targets_LIST test_wxPLplotDemo)
    endif(ENABLE_wxwidgets)

    if(PLD_extqt AND TARGET qt_example)
      add_custom_target(test_qt_example
	COMMAND qt_example
	)
      add_dependencies(test_qt_example qt_example ${qt_targets})
      if(PLPLOT_USE_QT5)
	message(STATUS "WARNING: The test_qt_example target can be run independently
   but is not reliable (see <https://sourceforge.net/p/plplot/bugs/158/>) when
   PLPLOT_USE_QT5 is set to ${PLPLOT_USE_QT5}.  So until this issue can be straightened out
   temporarily exclude this target from being a dependency of other more general interactive
   test targets")
	#list(APPEND qt_targets_LIST test_qt_example)
	#list(APPEND interactive_targets_LIST test_qt_example)
      else(PLPLOT_USE_QT5)
	list(APPEND qt_targets_LIST test_qt_example)
	list(APPEND interactive_targets_LIST test_qt_example)
      endif(PLPLOT_USE_QT5)
    endif(PLD_extqt AND TARGET qt_example)

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
	_plplotcmodule_fixed
	${qt_targets}
	)
    else(CORE_BUILD)
      add_custom_target(test_pyqt4_example
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/python/pyqt4_example.py
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
	)
      add_dependencies(test_pyqt4_example
	plplot_pyqt4
	_plplotcmodule
	${qt_targets}
	)
    endif(CORE_BUILD)

    list(APPEND interactive_targets_LIST test_pyqt4_example)
  endif(ENABLE_pyqt4)

  if(ENABLE_pyqt5)
    if(CORE_BUILD)
      add_custom_target(test_pyqt5_example
	COMMAND ${CMAKE_CURRENT_BINARY_DIR}/python/pyqt5_example.py
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python
	)
      add_dependencies(test_pyqt5_example
	plplot_pyqt5
	python_examples
	_plplotcmodule_fixed
	${qt_targets}
	)
    else(CORE_BUILD)
      add_custom_target(test_pyqt5_example
	COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/python/pyqt5_example.py
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
	)
      add_dependencies(test_pyqt5_example
	plplot_pyqt5
	_plplotcmodule
	${qt_targets}
	)
    endif(CORE_BUILD)
    message(STATUS "WARNING: The test_pyqt5_example target can be run
   independently but it intermittently segfaults (at least for
   Qt-5.3.2) so it is temporarily excluded from being a dependency of
   other more general interactive test targets")
    # list(APPEND interactive_targets_LIST test_pyqt5_example)
  endif(ENABLE_pyqt5)

  if(ENABLE_tcl AND PLD_${generic_interactive_device})
    add_custom_target(test_pltcl_standard_examples
      COMMAND env ${CMAKE_CURRENT_BINARY_DIR}/tcl/pltcl_standard_examples -dev ${generic_interactive_device} -np
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tcl
      )
    add_dependencies(test_pltcl_standard_examples
      workaround_file_generate_bug
      pltcl
      tcl_examples
      ${TARGET_tclIndex_tcl}
      ${${generic_interactive_device}_targets}
      )
    list(APPEND interactive_targets_LIST test_pltcl_standard_examples)
    list(APPEND tcl_targets_LIST test_pltcl_standard_examples)

  endif(ENABLE_tcl AND PLD_${generic_interactive_device})

  if(ENABLE_tcl AND BUILD_SHARED_LIBS AND PLD_tk)
    # tests that use "package require Pltcl" only work if
    # BUILD_SHARED_LIBS is true and PLD_tk is true.
    add_custom_target(test_tclsh_standard_examples
      COMMAND env ${CMAKE_CURRENT_BINARY_DIR}/tcl/tclsh_standard_examples
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tcl
      )

    if(tcltk_in_plplot_library)
      add_dependencies(test_tclsh_standard_examples
	plplot
	tcl_examples
	${TARGET_tclIndex_tcl}
	${tk_targets}
	)
    else(tcltk_in_plplot_library)
      add_dependencies(test_tclsh_standard_examples
	${plplottcltk_target}
	tcl_examples
	${TARGET_tclIndex_tcl}
	${tk_targets}
	)
    endif(tcltk_in_plplot_library)

    list(APPEND interactive_targets_LIST test_tclsh_standard_examples)
    list(APPEND tcl_targets_LIST test_tclsh_standard_examples)
  endif(ENABLE_tcl AND BUILD_SHARED_LIBS AND PLD_tk)

  if(ENABLE_tkX)
    set(targets_examples_tk)

    add_custom_target(test_tk_01
      COMMAND xtk01 -f ${CMAKE_CURRENT_BINARY_DIR}/tk/tk01
      )
    add_dependencies(test_tk_01
      workaround_file_generate_bug
      ${TARGET_tclIndex_tcl}
      ${TARGET_tclIndex_tk}
      ${xwin_targets}
      )
    list(APPEND targets_examples_tk test_tk_01)

    add_custom_target(test_tk_03
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tk/tk03
      )
    add_dependencies(test_tk_03
      workaround_file_generate_bug
      plserver
      ${TARGET_tclIndex_tcl}
      ${TARGET_tclIndex_tk}
      ${xwin_targets}
      )
    list(APPEND targets_examples_tk test_tk_03)

    add_custom_target(test_tk_plgrid
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/tk/plgrid
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tcl
      )

    add_dependencies(test_tk_plgrid
      workaround_file_generate_bug
      plserver
      tcl_examples
      ${TARGET_tclIndex_tcl}
      ${TARGET_tclIndex_tk}
      ${xwin_targets}
      )
    list(APPEND targets_examples_tk test_tk_plgrid)

    add_custom_target(test_plserver_standard_examples
      COMMAND env ${CMAKE_CURRENT_BINARY_DIR}/tk/plserver_standard_examples
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tk
      )
    add_dependencies(test_plserver_standard_examples
      workaround_file_generate_bug
      plserver
      tcl_examples
      ${TARGET_tclIndex_tcl}
      ${TARGET_tclIndex_tk}
      tclIndex_examples_tk
      ${xwin_targets}
      )
    list(APPEND targets_examples_tk test_plserver_standard_examples)

    # AND (ENABLE_DYNDRIVERS OR PLD_tk) Boolean logic required to
    # insure correct code to support plplot_widgetmodule is in
    # libplplot if ENABLE_DYNDRIVERS is OFF.
    if(ENABLE_python AND (ENABLE_DYNDRIVERS OR PLD_tk))
      if(CORE_BUILD)
	add_custom_target(test_pytkdemo
	  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/python/pytkdemo
	  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/python
	  )
	add_dependencies(test_pytkdemo
	  _plplotcmodule_fixed
	  plplot_widgetmodule
	  python_examples
	  ${TARGET_tclIndex_tcl}
	  ${xwin_targets}
	  )
      else(CORE_BUILD)
	add_custom_target(test_pytkdemo
	  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/python/pytkdemo
	  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python
	  )
	add_dependencies(test_pytkdemo
	  _plplotcmodule
	  plplot_widgetmodule
	  ${TARGET_tclIndex_tcl}
	  ${xwin_targets}
	  )
      endif(CORE_BUILD)
      #message(STATUS "WARNING: The test_pytkdemo target can be run independently but does not
#   work reliably so it is temporarily excluded from being a dependency of other
#   more general interactive test targets")
      list(APPEND interactive_targets_LIST test_pytkdemo)
      list(APPEND tk_targets_LIST test_pytkdemo)
    endif(ENABLE_python AND (ENABLE_DYNDRIVERS OR PLD_tk))

    if(BUILD_SHARED_LIBS AND PLD_tk)
      # tests that use "package require Pltk" only work if
      # BUILD_SHARED_LIBS is true and PLD_tk is true.
      add_custom_target(test_wish_standard_examples
	COMMAND env ${CMAKE_CURRENT_BINARY_DIR}/tk/wish_standard_examples
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tk
	)
      add_dependencies(test_wish_standard_examples
	${plplottcltk_target}
	${tk_targets}
	${xwin_targets}
	tcl_examples
	${TARGET_tclIndex_tcl}
	${TARGET_tclIndex_tk}
	tclIndex_examples_tk
	)
      # Temporarily suspend adding this target to the rest of the Tk
      # interactive targets (and therefore also this target is not added
      # to the overall list of targets run by the test_interactive
      # target) because of a segfault generated by this target.  The
      # cause of that segfault is the Tcl exit command is somehow not
      # compatible with how we have implemented the Tcl plframe command
      # that is run by tkdemos.tcl in the wish case, but that is as much
      # as we know at this stage.
      message(STATUS "WARNING: The test_wish_standard_examples target can be run independently but
   it segfaults at the end so it is temporarily excluded from being a dependency
   of other more general interactive test targets")
      # list(APPEND targets_examples_tk test_wish_standard_examples)
    endif(BUILD_SHARED_LIBS AND PLD_tk)

    if(ENABLE_itkX)
      add_custom_target(test_tk_02
	COMMAND xtk02 -f ${CMAKE_CURRENT_BINARY_DIR}/tk/tk02
	)
      add_dependencies(test_tk_02
	workaround_file_generate_bug
	${TARGET_tclIndex_tcl}
	${TARGET_tclIndex_tk}
	${xwin_targets}
	)

      if(USE_INCRTCL_VERSION_4)
	message(STATUS "WARNING: The test_tk_02 target can be run independently but it currently
   does not work for version 4 of Itcl and friends so it is temporarily excluded
   from being a dependency of other more general interactive test targets")
      else(USE_INCRTCL_VERSION_4)
	list(APPEND targets_examples_tk test_tk_02)
      endif(USE_INCRTCL_VERSION_4)

      add_custom_target(test_tk_04
	COMMAND xtk04 -f ${CMAKE_CURRENT_BINARY_DIR}/tk/tk04
	)
      add_dependencies(test_tk_04
        workaround_file_generate_bug
	${TARGET_tclIndex_tcl}
	${TARGET_tclIndex_tk}
	${xwin_targets}
	)
      if(USE_INCRTCL_VERSION_4)
	message(STATUS "WARNING: The test_tk_04 target can be run independently but it currently
   does not work for version 4 of Itcl and friends so it is temporarily excluded
   from being a dependency of other more general interactive test targets")
      else(USE_INCRTCL_VERSION_4)
	list(APPEND targets_examples_tk test_tk_04)
      endif(USE_INCRTCL_VERSION_4)
    endif(ENABLE_itkX)

    add_custom_target(test_plserver_runAllDemos
      COMMAND env ${CMAKE_CURRENT_BINARY_DIR}/tk/plserver_runAllDemos
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tk
      )
    add_dependencies(test_plserver_runAllDemos
      workaround_file_generate_bug
      plserver
      tcl_examples
      ${TARGET_tclIndex_tcl}
      ${TARGET_tclIndex_tk}
      tclIndex_examples_tk
      ${xwin_targets}
      )

    message(STATUS "WARNING: The test_plserver_runAllDemos target can be run independently but
   there are several issues with this target so it is temporarily excluded from being a
   dependency of other more general interactive test targets")
    #list(APPEND targets_examples_tk test_plserver_runAllDemos)

    if(BUILD_SHARED_LIBS AND PLD_tkwin)
      # tests that use "package require Plplotter" only work if
      # BUILD_SHARED_LIBS is true and PLD_tkwin is true.
      add_custom_target(test_wish_runAllDemos
	COMMAND env ${CMAKE_CURRENT_BINARY_DIR}/tk/wish_runAllDemos
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tk
	)
      add_dependencies(test_wish_runAllDemos
	${plplottcltk_target}
	${tkwin_targets}
	tcl_examples
	${TARGET_tclIndex_tcl}
	${TARGET_tclIndex_tk}
	tclIndex_examples_tk
	)
      message(STATUS "WARNING: The test_wish_runAllDemos target can be run independently but
   there are several issues with this target so it is temporarily excluded from being a
   dependency of other more general interactive test targets")
      # list(APPEND targets_examples_tk test_wish_runAllDemos)
    endif(BUILD_SHARED_LIBS AND PLD_tkwin)

  endif(ENABLE_tkX)

  if(targets_examples_tk)
    list(APPEND interactive_targets_LIST ${targets_examples_tk})
    list(APPEND tk_targets_LIST ${targets_examples_tk})
  endif(targets_examples_tk)

  add_custom_target(test_interactive)
  if(interactive_targets_LIST)
    add_dependencies(test_interactive ${interactive_targets_LIST})
  endif(interactive_targets_LIST)

  add_custom_target(test_tcl)
  if(tcl_targets_LIST)
    add_dependencies(test_tcl ${tcl_targets_LIST})
  endif(tcl_targets_LIST)

  add_custom_target(test_tk)
  if(tk_targets_LIST)
    add_dependencies(test_tk ${tk_targets_LIST})
  endif(tk_targets_LIST)

  # List of custom targets that test all of qt devices/special examples.
  # Must be maintained.
  set(qt_test_target_LIST
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
    test_qt_example
    )
  if(ENABLE_pyqt4)
    list(APPEND qt_test_target_LIST test_pyqt4_example)
  elseif(ENABLE_pyqt5)
    list(APPEND qt_test_target_LIST test_pyqt5_example)
  endif(ENABLE_pyqt4)

  set(qt_test_target_DEPENDS)
  foreach(qt_test_target ${qt_test_target_LIST})
    if(TARGET ${qt_test_target})
      list(APPEND qt_test_target_DEPENDS ${qt_test_targets})
    endif(TARGET ${qt_test_target})
  endforeach(qt_test_target ${qt_test_target_LIST})
  if(qt_test_target_DEPENDS)
    add_custom_target(test_all_qt)
    add_dependencies(test_all_qt ${qt_test_target_DEPENDS})
  endif(qt_test_target_DEPENDS)

  # List of custom targets that test all of cairo devices/special examples.
  # Must be maintained.
  set(cairo_test_target_LIST
    test_c_pdfcairo
    test_c_pngcairo
    test_c_pscairo
    test_c_epscairo
    test_c_svgcairo
    test_extcairo
    test_c_xcairo
    test_extXdrawable
    )

  set(cairo_test_target_DEPENDS)
  foreach(cairo_test_target ${cairo_test_target_LIST})
    if(TARGET ${cairo_test_target})
      list(APPEND cairo_test_target_DEPENDS ${cairo_test_target})
    endif(TARGET ${cairo_test_target})
  endforeach(cairo_test_target ${cairo_test_target_LIST})
  if(cairo_test_target_DEPENDS)
    add_custom_target(test_all_cairo)
    add_dependencies(test_all_cairo ${cairo_test_target_DEPENDS})
  endif(cairo_test_target_DEPENDS)

endif(BUILD_TEST)

if(NOT CORE_BUILD)
  set(summary_results "
Summary of CMake build system results for the installed examples

Noninteractive device drivers:

PLD_pdfcairo:		${PLD_pdfcairo}
PLD_pngcairo:		${PLD_pngcairo}
PLD_pscairo:		${PLD_pscairo}
PLD_epscairo:		${PLD_epscairo}
PLD_svgcairo:		${PLD_svgcairo}
PLD_cgm:		${PLD_cgm}
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
PLD_pdf:		${PLD_pdf}
PLD_ps:			${PLD_ps}
PLD_psc:		${PLD_psc}
PLD_pstex:		${PLD_pstex}
PLD_psttf:		${PLD_psttf}
PLD_psttfc:		${PLD_psttfc}
PLD_svg:		${PLD_svg}
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
with those device drivers so we don't bother to retrieve them from the
core build system or output them here.

Noninteractive bindings:

ENABLE_ada:		${ENABLE_ada}
ENABLE_c:		${ENABLE_c}
ENABLE_cxx:		${ENABLE_cxx}
ENABLE_d:		${ENABLE_d}
ENABLE_f95:		${ENABLE_f95}
ENABLE_java:		${ENABLE_java}
ENABLE_lua:		${ENABLE_lua}
ENABLE_ocaml:		${ENABLE_ocaml}
ENABLE_octave:		${ENABLE_octave}
ENABLE_pdl:		${ENABLE_pdl}
ENABLE_python:		${ENABLE_python}
ENABLE_tcl:		${ENABLE_tcl}

Interactive bindings:

ENABLE_qt:		${ENABLE_qt}
ENABLE_tk:		${ENABLE_tk}
ENABLE_wxwidgets:	${ENABLE_wxwidgets}
")

  message("${summary_results}")

endif(NOT CORE_BUILD)
