# plplot_test/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2014 Alan W. Irwin
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
  set(SCRIPTS)
  set(TEST_ENVIRONMENT
    "EXAMPLES_DIR=${CMAKE_BINARY_DIR}/examples SRC_EXAMPLES_DIR=${CMAKE_SOURCE_DIR}/examples"
    )

  set(TEST_DEVICE psc CACHE STRING "device used for ctest")
  # There is no PLD_psc or PLD_psttfc and the PLD_ps and PLD_psttf
  # variables must be used instead.  All other PLD_* names correspond
  # to exact device names.  So adjust for this complicated naming
  # scheme in a general way in case some other TEST_DEVICE is chosen
  # in the future.
  if(TEST_DEVICE STREQUAL "psc")
    set(PLD_TEST_DEVICE ${PLD_ps})
  elseif(TEST_DEVICE STREQUAL "psttfc")
    set(PLD_TEST_DEVICE ${PLD_psttf})
  else(TEST_DEVICE STREQUAL "psc")
    set(PLD_TEST_DEVICE ${PLD_${TEST_DEVICE}})
  endif(TEST_DEVICE STREQUAL "psc")

  set(TEST_SCRIPT "./plplot-test.sh --verbose")
  set(TEST_SCRIPT_DEVICE "${TEST_SCRIPT} --device=${TEST_DEVICE}")

  set(JAVA_TEST_ENVIRONMENT "${TEST_ENVIRONMENT} PLPLOT_JAVA_WRAP_DIR=${CMAKE_BINARY_DIR}/bindings/java/ PLPLOT_CLASSPATH=${CMAKE_BINARY_DIR}/examples/java/plplot.jar")

  #Build-tree configuration
  set(CONFIGURED_EXAMPLES_DIR EXAMPLES_DIR)
  configure_file(
    plplot-test.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/plplot-test.sh
    @ONLY
    )

  #Install-tree configuration
  set(CONFIGURED_EXAMPLES_DIR SRC_EXAMPLES_DIR)
  configure_file(
    plplot-test.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/plplot-test.sh_install
    @ONLY
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

  if(WIN32_AND_NOT_CYGWIN)
    set(SHELL_TEST_WXWIDGETS 1)
  else(WIN32_AND_NOT_CYGWIN)
    # FIXME.
    # The wxwidgets device on Unix generates "forced mousing" screen
    # refreshes for the wxwidgets-3.0.0 case so it is temporarily excluded
    # from the interactive tests done for the traditional build system
    # for the installed examples.
    set(SHELL_TEST_WXWIDGETS 0)
  endif(WIN32_AND_NOT_CYGWIN)

  configure_file(
    plplot-test-interactive.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/plplot-test-interactive.sh
    @ONLY
    )

  configure_file(
    test_c.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/test_c.sh
    @ONLY
    )
  list(APPEND SCRIPTS test_c.sh)

  configure_file(
    test_c_interactive.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/test_c_interactive.sh
    @ONLY
    )
  list(APPEND SCRIPTS test_c_interactive.sh)

  set(test_output_files_LIST)
  set(examples_compare_DEPENDS)

  if(PLD_TEST_DEVICE)
    add_test(examples_c
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=c"
      )
    list(APPEND examples_compare_DEPENDS examples_c)
    list_example_files(. ${TEST_DEVICE} c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_TEST_DEVICE)

  if(ENABLE_cxx)
    configure_file(
      test_cxx.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_cxx.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_cxx.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_cxx
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=cxx"
	)
      list(APPEND examples_compare_DEPENDS examples_cxx)
      list_example_files(. ${TEST_DEVICE} cxx output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_cxx)

  if(ENABLE_f95)
    configure_file(
      test_f95.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_f95.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_f95.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_f95
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=f95"
	)
      list(APPEND examples_compare_DEPENDS examples_f95)
      list_example_files(. ${TEST_DEVICE} f95 output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_f95)

  if(ENABLE_java)
    configure_file(
      test_java.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_java.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_java.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_java
	${SH_EXECUTABLE} -c "${JAVA_TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=java"
	)
      list(APPEND examples_compare_DEPENDS examples_java)
      list_example_files(. ${TEST_DEVICE} j output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_java)

  if(ENABLE_octave)

    configure_file(
      test_octave_interactive.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_octave_interactive.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_octave_interactive.sh)

    configure_file(
      test_octave.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_octave.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_octave.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_octave
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=octave"
	)
      list(APPEND examples_compare_DEPENDS examples_octave)
      list_example_files(. ${TEST_DEVICE} o output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_octave)

  if(ENABLE_python)
    configure_file(
      test_python.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_python.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_python.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_python
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=python"
	)
      list(APPEND examples_compare_DEPENDS examples_python)
      list_example_files(. ${TEST_DEVICE} p output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_python)

  if(ENABLE_tcl)
    get_target_property(pltcl_LOC pltcl LOCATION)
    string(REGEX REPLACE "/pltcl.*$" "" PLTCL_DIR ${pltcl_LOC})
    # Experience with MSYS bash shows that variables used to set a
    # PATH component must transform the drive-letter form to the
    # leading-slash Unix form of the PATH component.  The case of the
    # drive letter doesn't matter.  For example converting
    # z:/whatever/path ==> /z/whatever/path or /Z/whatever/path works
    # fine for MSYS bash.
    string(REGEX REPLACE "^(.):/" "/\\1/" PLTCL_DIR ${PLTCL_DIR})


    configure_file(test_tcl.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_tcl.sh
      @ONLY
      )
    if(PLD_TEST_DEVICE)
      add_test(examples_tcl
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=tcl"
	)
      list(APPEND examples_compare_DEPENDS examples_tcl)
      list_example_files(. ${TEST_DEVICE} t output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
    set(PLTCL_DIR ${BIN_DIR})
    # Transform drive-letter form to leading-slash form, see comment above.
    string(REGEX REPLACE "^(.):" "/\\1/" PLTCL_DIR ${PLTCL_DIR})

    configure_file(test_tcl.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_tcl.sh_install
      @ONLY
      )
  endif(ENABLE_tcl)

  if(ENABLE_pdl)
    configure_file(
      test_pdl.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_pdl.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_pdl.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_pdl
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=pdl"
	)
      list(APPEND examples_compare_DEPENDS examples_pdl)
      list_example_files(. ${TEST_DEVICE} pdl output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_pdl)

  if(ENABLE_ada)
    configure_file(
      test_ada.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_ada.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_ada.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_ada
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=ada"
	)
      list(APPEND examples_compare_DEPENDS examples_ada)
      list_example_files(. ${TEST_DEVICE} a output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_ada)

  if(ENABLE_ocaml)
    configure_file(
      test_ocaml.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_ocaml.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_ocaml.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_ocaml
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=ocaml"
	)
      list(APPEND examples_compare_DEPENDS examples_ocaml)
      list_example_files(. ${TEST_DEVICE} ocaml output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_ocaml)

  if(ENABLE_lua)
    configure_file(
      test_lua.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_lua.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_lua.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_lua
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=lua"
	)
      list(APPEND examples_compare_DEPENDS examples_lua)
      list_example_files(. ${TEST_DEVICE} lua output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_lua)

  if(ENABLE_d)
    configure_file(
      test_d.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_d.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_d.sh)
    if(PLD_TEST_DEVICE)
      add_test(examples_d
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT_DEVICE} --front-end=d"
	)
      list(APPEND examples_compare_DEPENDS examples_d)
      list_example_files(. ${TEST_DEVICE} d output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(PLD_TEST_DEVICE)
  endif(ENABLE_d)

  # Run C examples with different drivers
  if(PLD_ps)
    if (NOT TEST_DEVICE STREQUAL "psc")
      add_test(examples_psc
	${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=psc"
	)
      list_example_files(. psc c output_list)
      list(APPEND test_output_files_LIST ${output_list})
    endif(NOT TEST_DEVICE STREQUAL "psc")
  endif(PLD_ps)

  if(PLD_psttf)
    add_test(examples_psttfc
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=psttfc"
      )
    list_example_files(. psttfc c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_psttf)

  if(PLD_pstex)
    add_test(examples_pstex
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=pstex"
      )
    list_example_files(. pstex c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_pstex)

  if(PLD_png)
    add_test(examples_png
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=png"
      )
    list_example_files(. png c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_png)

  if(PLD_svg)
    add_test(examples_svg
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=svg"
      )
    list_example_files(. svg c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_svg)

  if(PLD_plmeta)
    add_test(examples_plmeta
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=plmeta"
      )
    list_example_files(. plmeta c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_plmeta)

  if(PLD_pscairo)
    add_test(examples_pscairo
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=pscairo"
      )
    list_example_files(. pscairo c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_pscairo)

  if(PLD_pngcairo)
    add_test(examples_pngcairo
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=pngcairo"
      )
    list_example_files(. pngcairo c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_pngcairo)

  if(PLD_xfig)
    add_test(examples_xfig
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=xfig"
      )
    list_example_files(. xfig c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_xfig)

  if(PLD_gif)
    add_test(examples_gif
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=gif"
      )
    list_example_files(. gif c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_gif)

  if(PLD_cgm)
    add_test(examples_cgm
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=cgm"
      )
    list_example_files(. cgm c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_cgm)

  if(PLD_bmpqt)
    add_test(examples_bmpqt
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=bmpqt"
      )
    list_example_files(. bmpqt c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_bmpqt)

  if(PLD_jpgqt)
    add_test(examples_jpgqt
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=jpgqt"
      )
    list_example_files(. jpgqt c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_jpgqt)

  if(PLD_pngqt)
    add_test(examples_pngqt
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=pngqt"
      )
    list_example_files(. pngqt c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_pngqt)

  if(PLD_ppmqt)
    add_test(examples_ppmqt
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=ppmqt"
      )
    list_example_files(. ppmqt c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_ppmqt)

  if(PLD_tiffqt)
    add_test(examples_tiffqt
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=tiffqt"
      )
    list_example_files(. tiffqt c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_tiffqt)

  if(PLD_svgqt)
    add_test(examples_svgqt
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=svgqt"
      )
    list_example_files(. svgqt c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_svgqt)

  if(PLD_epsqt)
    add_test(examples_epsqt
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=epsqt"
      )
    list_example_files(. epsqt c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_epsqt)

  if(PLD_pdfqt)
    add_test(examples_pdfqt
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=pdfqt"
      )
    list_example_files(. pdfqt c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_pdfqt)

  if(PLD_pdf)
    add_test(examples_pdf
      ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c --device=pdf"
      )
    list_example_files(. pdf c output_list)
    list(APPEND test_output_files_LIST ${output_list})
  endif(PLD_pdf)

  if(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
    configure_file(
      test_diff.sh.in
      ${CMAKE_CURRENT_BINARY_DIR}/test_diff.sh
      @ONLY
      )
    list(APPEND SCRIPTS test_diff.sh)
    add_test(examples_compare
      ${SH_EXECUTABLE} -c "./test_diff.sh"
      )
    # There is a bug in 2.8.x with x < 5 where tests are run out of
    # the order in which they are declared by default.  Also, 2.8.x
    # can run repeat tests out of order due to some optimizations that
    # have been implemented.  Thus, always force examples_compare to
    # be run after the other tests it depends on.
    #message(STATUS "DEBUG: examples_compare_DEPENDS = ${examples_compare_DEPENDS}")
    set_tests_properties(examples_compare
      PROPERTIES
      DEPENDS "${examples_compare_DEPENDS}"
      )
  endif(CMP_EXECUTABLE OR DIFF_EXECUTABLE AND TAIL_EXECUTABLE)

  if(BUILD_TEST)
    # Make a copy of lena.pgm to the test subdirectory of the build
    # tree so that example 20 will run from there.
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${CMAKE_SOURCE_DIR}/examples/lena.pgm
      ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
      DEPENDS ${CMAKE_SOURCE_DIR}/examples/lena.pgm
      )
    add_custom_target(lena_file ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm)
    if(ENABLE_octave)
      # Also need a copy of lena.img for the octave bindings
      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lena.img
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_SOURCE_DIR}/examples/octave/lena.img
	${CMAKE_CURRENT_BINARY_DIR}/lena.img
	DEPENDS ${CMAKE_SOURCE_DIR}/examples/octave/lena.img
	)
      add_custom_target(lena_octave_file ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lena.img)
    endif(ENABLE_octave)

    # Custom target to remove everything created by ctest
    # N.B. the test_output_files_LIST is extremely long and
    # overflows the maximum line length of the shell if you
    # replace "." above with ${CMAKE_CURRENT_BINARY_DIR}
    add_custom_target(clean_ctest_plot_files
      COMMAND ${CMAKE_COMMAND} -E remove ${test_output_files_LIST}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
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
    FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot-test-interactive.sh
    DESTINATION ${DATA_DIR}/examples
    PERMISSIONS ${PERM_SCRIPTS}
    )

endif(SH_EXECUTABLE)
