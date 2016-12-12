# plplot_test/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2016 Alan W. Irwin
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

# Only set up test environment, configure test files, and install them if
# a shell is available.

if(SH_EXECUTABLE)

  set(CTEST_EXAMPLES_OUTPUT_DIR ${CMAKE_BINARY_DIR}/ctest_examples_output_dir)
  file(MAKE_DIRECTORY ${CTEST_EXAMPLES_OUTPUT_DIR})

  set(SCRIPTS)
  string(REPLACE " " "\\ " CMAKE_BINARY_DIR_ESCAPED ${CMAKE_BINARY_DIR})
  string(REPLACE " " "\\ " CMAKE_SOURCE_DIR_ESCAPED ${CMAKE_SOURCE_DIR})
  string(REPLACE " " "\\ " CTEST_EXAMPLES_OUTPUT_DIR_ESCAPED ${CTEST_EXAMPLES_OUTPUT_DIR})
  if(MSVC)
    set (VC_CTEST_DIRECTORY /$<CONFIGURATION>)
  endif(MSVC)
  set(TEST_ENVIRONMENT
    "EXAMPLES_DIR=${CMAKE_BINARY_DIR_ESCAPED}/examples SRC_EXAMPLES_DIR=${CMAKE_SOURCE_DIR_ESCAPED}/examples OUTPUT_DIR=${CTEST_EXAMPLES_OUTPUT_DIR_ESCAPED} VC_CTEST_DIRECTORY=${VC_CTEST_DIRECTORY}"
    )
  message(STATUS "TEST_ENVIRONMENT = ${TEST_ENVIRONMENT}")
  set(PLPLOT_TEST_DEVICE psc CACHE STRING "device used for ctest")
  # There is no PLD_psc or PLD_psttfc and the PLD_ps and PLD_psttf
  # variables must be used instead.  All other PLD_* names correspond
  # to exact device names.  So adjust for this complicated naming
  # scheme in a general way.
  if(PLPLOT_TEST_DEVICE STREQUAL "psc")
    set(PLD_TEST_DEVICE ${PLD_ps})
  elseif(PLPLOT_TEST_DEVICE STREQUAL "psttfc")
    set(PLD_TEST_DEVICE ${PLD_psttf})
  else(PLPLOT_TEST_DEVICE STREQUAL "psc")
    set(PLD_TEST_DEVICE ${PLD_${PLPLOT_TEST_DEVICE}})
  endif(PLPLOT_TEST_DEVICE STREQUAL "psc")

  set(TEST_SCRIPT "./plplot-test.sh --verbose")
  set(TEST_SCRIPT_DEVICE "${TEST_SCRIPT} --device=${PLPLOT_TEST_DEVICE}")

  set(JAVA_TEST_ENVIRONMENT "${TEST_ENVIRONMENT} PLPLOT_JAVA_WRAP_DIR=${CMAKE_BINARY_DIR}/bindings/java/ PLPLOT_CLASSPATH=${CMAKE_BINARY_DIR}/examples/java/plplot.jar")

  if(CYGWIN)
    set(PLATFORM CYGWIN)
  elseif(MSYS)
    set(PLATFORM MSYS)
  elseif(UNIX)
    set(PLATFORM UNIX)
  elseif(MINGW)
    set(PLATFORM MINGW)
  elseif(MSVC)
    set(PLATFORM MSVC)
  else(CYGWIN)
    message(FATAL_ERROR "Internal build-system error; unable to determine platform.")
  endif(CYGWIN)

  #Build-tree configuration
  set(CONFIGURED_EXAMPLES_DIR EXAMPLES_DIR)
  set(configured_PL_DEPRECATED_f95 ${PL_DEPRECATED_f95})
  configure_file(
    plplot-test.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/plplot-test.sh
    @ONLY
    NEWLINE_STYLE UNIX
    )

  # Set up optional tests in the configured plplot-test-interactive.sh file
  # which is used by the traditional build system for the installed
  # examples to do some interactive testing.

  set(pkgIndex_LOCATION ${DATA_DIR})
  set(SHELL_TEST_PLTCL_STANDARD_EXAMPLES 1)
  set(SHELL_TEST_PLSERVER_STANDARD_EXAMPLES 1)
  # FIXME.  Currently plserver_runAllDemos needs work so drop it for now.
  set(SHELL_TEST_PLSERVER_RUNALLDEMOS 0)

  # All Tcl/Tk tests that use the "package require" approach only
  # work with shared libraries.
  if(BUILD_SHARED_LIBS)
    set(SHELL_TEST_TCLSH_STANDARD_EXAMPLES 1)
    # FIXME.  Currently wish_standard_examples segfaults on exit.  So
    # drop it for now.
    set(SHELL_TEST_WISH_STANDARD_EXAMPLES 0)
    # FIXME.  Currently the wish_runAllDemos needs work and also has
    # an intermittent bug (x?? not recognized as a command).
    set(SHELL_TEST_WISH_RUNALLDEMOS 0)
  else(BUILD_SHARED_LIBS)
    set(SHELL_TEST_TCLSH_STANDARD_EXAMPLES 0)
    set(SHELL_TEST_WISH_STANDARD_EXAMPLES 0)
    set(SHELL_TEST_WISH_RUNALLDEMOS 0)
  endif(BUILD_SHARED_LIBS)

  # Set this value to 0 if you want to temporarily disable wxwidgets tests
  # for the traditional build.
  set(SHELL_TEST_WXWIDGETS 1)

  # Set this value to 0 if you want to temporarily disable qt_example tests
  # for the traditional build.
  if(PLPLOT_USE_QT5)
    # FIXME once the Qt5 memory management issues are fixed
    set(SHELL_TEST_QT_EXAMPLE 0)
  else(PLPLOT_USE_QT5)
    set(SHELL_TEST_QT_EXAMPLE 1)
  endif(PLPLOT_USE_QT5)

  configure_file(
    plplot-test-interactive.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/plplot-test-interactive.sh
    @ONLY
    NEWLINE_STYLE UNIX
    )

  # Install-tree configuration of plplot-test.sh and plplot-test-interactive.sh
  # No Tcl/Tk prefix for these cases.
  set(SAVE_TCL_TK_SRC_PREFIX ${TCL_TK_SRC_PREFIX})
  set(TCL_TK_SRC_PREFIX)

  set(CONFIGURED_EXAMPLES_DIR SRC_EXAMPLES_DIR)
  set(configured_PL_DEPRECATED_f95 OFF)
  configure_file(
    plplot-test.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/plplot-test.sh_install
    @ONLY
    NEWLINE_STYLE UNIX
    )

  configure_file(
    plplot-test-interactive.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/plplot-test-interactive.sh_install
    @ONLY
    NEWLINE_STYLE UNIX
    )
  set(TCL_TK_SRC_PREFIX ${SAVE_TCL_TK_SRC_PREFIX})

  configure_file(
    test_c.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/test_c.sh
    @ONLY
    NEWLINE_STYLE UNIX
    )
  list(APPEND SCRIPTS test_c.sh)

  configure_file(
    test_c_interactive.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/test_c_interactive.sh
    @ONLY
    NEWLINE_STYLE UNIX
    )
  list(APPEND SCRIPTS test_c_interactive.sh)

  set(examples_compare_DEPENDS)

  if(PLD_TEST_DEVICE)
    add_test(NAME examples_c
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=c"
      )
    list(APPEND examples_compare_DEPENDS examples_c)
  endif(PLD_TEST_DEVICE)

  if(ENABLE_cxx)
    configure_file(
      test_cxx.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_cxx.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_cxx.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_cxx
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=cxx"
	)
      list(APPEND examples_compare_DEPENDS examples_cxx)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_cxx)

  if(ENABLE_f95)
    configure_file(
      test_f95.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_f95.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_f95.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_f95
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=f95"
	)
      list(APPEND examples_compare_DEPENDS examples_f95)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_f95)

  if(ENABLE_java)
    configure_file(
      test_java.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_java.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_java.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_java
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${JAVA_TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=java"
	)
      list(APPEND examples_compare_DEPENDS examples_java)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_java)

  if(ENABLE_octave)

    configure_file(
      test_octave_interactive.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_octave_interactive.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_octave_interactive.sh)

    configure_file(
      test_octave.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_octave.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_octave.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_octave
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=octave"
	)
      list(APPEND examples_compare_DEPENDS examples_octave)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_octave)

  if(ENABLE_python)
    configure_file(
      test_python.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_python.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_python.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_python
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=python"
	)
      list(APPEND examples_compare_DEPENDS examples_python)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_python)

  if(ENABLE_tcl)

    set(PLTCL_DIR $<TARGET_FILE_DIR:pltcl>)
    # N.B. Results only available at generate time.
    configure_file_generate(
      ${CMAKE_CURRENT_SOURCE_DIR}/test_tcl.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_tcl.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )

    if(PLD_TEST_DEVICE)
      add_test(NAME examples_tcl
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=tcl"
	)
      list(APPEND examples_compare_DEPENDS examples_tcl)
    endif(PLD_TEST_DEVICE)

    set(PLTCL_DIR ${BIN_DIR})
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/test_tcl.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_tcl.sh_install
      @ONLY
      NEWLINE_STYLE UNIX
      )
  endif(ENABLE_tcl)

  if(ENABLE_pdl)
    configure_file(
      test_pdl.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_pdl.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_pdl.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_pdl
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=pdl"
	)
      list(APPEND examples_compare_DEPENDS examples_pdl)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_pdl)

  if(ENABLE_ada)
    configure_file(
      test_ada.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_ada.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_ada.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_ada
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=ada"
	)
      list(APPEND examples_compare_DEPENDS examples_ada)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_ada)

  if(ENABLE_ocaml)
    configure_file(
      test_ocaml.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_ocaml.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_ocaml.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_ocaml
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=ocaml"
	)
      list(APPEND examples_compare_DEPENDS examples_ocaml)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_ocaml)

  if(ENABLE_lua)
    configure_file(
      test_lua.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_lua.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_lua.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_lua
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=lua"
	)
      list(APPEND examples_compare_DEPENDS examples_lua)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_lua)

  if(ENABLE_d)
    configure_file(
      test_d.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_d.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_d.sh)
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_d
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=d"
	)
      list(APPEND examples_compare_DEPENDS examples_d)
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_d)

  if(PLD_plmeta)
    set(PLRENDER_DIR ${CMAKE_BINARY_DIR}/utils)
    configure_file(
      test_plrender.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_plrender.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_plrender.sh)
    set(PLRENDER_DIR ${BIN_DIR})
    configure_file(
      test_plrender.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_plrender.sh_install
      @ONLY
      NEWLINE_STYLE UNIX
      )
    if(PLD_TEST_DEVICE)
      add_test(NAME examples_plrender
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=plrender"
	)
      list(APPEND examples_compare_DEPENDS examples_plrender)
    endif(PLD_TEST_DEVICE)
  endif(PLD_plmeta)

  # Add tests for all enabled file devices (except for PLPLOT_TEST_DEVICE) for C language.
  if(PLD_ps)
    list(APPEND FILE_DEVICES_LIST psc:ps:OFF)
  endif(PLD_ps)
  if(PLD_psttf)
    list(APPEND FILE_DEVICES_LIST psttfc:psttf:OFF)
  endif(PLD_psttf)

  # FILE_DEVICES_LIST is set in cmake/modules/drivers-finish.cmake
  # for all enabled _file_ devices and adjusted above for the
  # special case of the psc and psttfc devices.
  # Each list member consists of a colon-separated device name, driver
  # name, and a Boolean variable that indicates if the device has familied
  # output in plplot-test.sh (i.e., requires familying).  Only the
  # enabled device name is important in this context.

  foreach(file_devices_info ${FILE_DEVICES_LIST})
    string(REGEX REPLACE "^(.*):.*:.*$" "\\1" device ${file_devices_info})
    if (NOT PLPLOT_TEST_DEVICE STREQUAL device)
      add_test(NAME examples_${device}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=${device}"
	)
    endif(NOT PLPLOT_TEST_DEVICE STREQUAL device)
  endforeach(file_devices_info ${FILE_DEVICES_LIST})

  if(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
    configure_file(
      test_diff.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_diff.sh
      @ONLY
      NEWLINE_STYLE UNIX
      )
    list(APPEND SCRIPTS test_diff.sh)
    string(REPLACE " " "\\ " TEST_DIFF_SH "${CMAKE_CURRENT_BINARY_DIR}/test_diff.sh")
    add_test(NAME examples_compare
      WORKING_DIRECTORY ${CTEST_EXAMPLES_OUTPUT_DIR}
      COMMAND ${SH_EXECUTABLE} -c "${TEST_DIFF_SH} --device=${PLPLOT_TEST_DEVICE}"
      )
    # Always force examples_compare to be run after the other tests it depends on.
    #message(STATUS "DEBUG: examples_compare_DEPENDS = ${examples_compare_DEPENDS}")
    set_tests_properties(examples_compare
      PROPERTIES
      DEPENDS "${examples_compare_DEPENDS}"
      )
  endif(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)

  if(BUILD_TEST)
    # Make a copy of Chloe.pgm to the test subdirectory of the build
    # tree so that example 20 will run from there.
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Chloe.pgm
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_SOURCE_DIR}/examples/Chloe.pgm
      ${CMAKE_CURRENT_BINARY_DIR}/Chloe.pgm
      DEPENDS ${CMAKE_SOURCE_DIR}/examples/Chloe.pgm
      )
    add_custom_target(Chloe_file ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Chloe.pgm)

    # Implement custom target to remove all examples output files
    # created by ctest in a convenient way. Use special directory to
    # store these files since otherwise an attempt to remove these
    # files by name from a multi-use directory can exceed the maximum
    # command-line length for the "MinGW Makefiles" cmd environment.
    add_custom_target(clean_ctest_plot_files
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${CTEST_EXAMPLES_OUTPUT_DIR}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CTEST_EXAMPLES_OUTPUT_DIR}
      )

  endif(BUILD_TEST)

  set(PERM_SCRIPTS
    OWNER_READ
    OWNER_WRITE
    OWNER_EXECUTE
    GROUP_READ
    GROUP_EXECUTE
    WORLD_READ
    WORLD_EXECUTE
    )

  foreach(SCRIPT ${SCRIPTS})
    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT}
      DESTINATION ${DATA_DIR}/examples
      PERMISSIONS ${PERM_SCRIPTS}
      )
  endforeach(SCRIPT ${SCRIPTS})

  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot-test.sh_install
    DESTINATION ${DATA_DIR}/examples
    PERMISSIONS ${PERM_SCRIPTS}
    RENAME plplot-test.sh
    )

  if(ENABLE_tcl)
    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/test_tcl.sh_install
      DESTINATION ${DATA_DIR}/examples
      PERMISSIONS ${PERM_SCRIPTS}
      RENAME test_tcl.sh
      )
  endif(ENABLE_tcl)
  install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot-test-interactive.sh_install
    DESTINATION ${DATA_DIR}/examples
    PERMISSIONS ${PERM_SCRIPTS}
    RENAME plplot-test-interactive.sh
    )
  if(PLD_plmeta)
    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/test_plrender.sh_install
      DESTINATION ${DATA_DIR}/examples
      PERMISSIONS ${PERM_SCRIPTS}
      RENAME test_plrender.sh
      )
  endif(PLD_plmeta)

endif(SH_EXECUTABLE)
