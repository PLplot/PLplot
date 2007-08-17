# Only set up test environment, configure test files, and install them if
# a shell is available.

if(SH_EXECUTABLE)

set(TEST_ENVIRONMENT
  "EXAMPLES_DIR=${CMAKE_BINARY_DIR}/examples SRC_EXAMPLES_DIR=${CMAKE_SOURCE_DIR}/examples PLPLOT_LIB=${CMAKE_SOURCE_DIR}/data/"
)

set(TEST_DEVICE psc CACHE STRING "device used for ctest")
set(TEST_SCRIPT "./plplot-test.sh --device=${TEST_DEVICE}")

set(JAVA_TEST_ENVIRONMENT "${TEST_ENVIRONMENT} PLPLOT_JAVA_WRAP_DIR=${CMAKE_BINARY_DIR}/bindings/java/ PLPLOT_CLASSPATH=${CMAKE_BINARY_DIR}/examples/java/plplot.jar")

configure_file(
  plplot-test.sh.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/plplot-test.sh 
  @ONLY
  )

configure_file(test_c.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_c.sh @ONLY)
set(SCRIPTS test_c.sh)
add_test(examples_c
  ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=c"
  )

if(ENABLE_cxx)
  configure_file(test_cxx.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_cxx.sh @ONLY)
  set(SCRIPTS ${SCRIPTS} test_cxx.sh)
  add_test(examples_cxx
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=cxx"
  )
endif(ENABLE_cxx)

if(ENABLE_f77)
  configure_file(test_f77.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_f77.sh @ONLY)
  set(SCRIPTS ${SCRIPTS} test_f77.sh)
  add_test(examples_f77
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=f77"
  )
endif(ENABLE_f77)

if(ENABLE_f95)
  configure_file(test_f95.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_f95.sh @ONLY)
  set(SCRIPTS ${SCRIPTS} test_f95.sh)
  add_test(examples_f95
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=f95"
  )
endif(ENABLE_f95)

if(ENABLE_java)
  configure_file(
    test_java.sh.in 
    ${CMAKE_CURRENT_BINARY_DIR}/test_java.sh 
    @ONLY
    )
  set(SCRIPTS ${SCRIPTS} test_java.sh)
  add_test(examples_java
    ${SH_EXECUTABLE} -c "${JAVA_TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=java"
  )
endif(ENABLE_java)

if(ENABLE_octave)
  configure_file(
    test_octave.sh.in 
    ${CMAKE_CURRENT_BINARY_DIR}/test_octave.sh 
    @ONLY
    )
  set(SCRIPTS ${SCRIPTS} test_octave.sh)
  add_test(examples_octave
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=octave"
  )
endif(ENABLE_octave)

if(ENABLE_python)
  if(HAVE_NUMPY)
    # The following examples only work with numpy
    set(NUMPY_EXAMPLES "21")
  else(HAVE_NUMPY)
    set(NUMPY_EXAMPLES)
  endif(HAVE_NUMPY)
  configure_file(
    test_python.sh.in 
    ${CMAKE_CURRENT_BINARY_DIR}/test_python.sh 
    @ONLY
    )
  set(SCRIPTS ${SCRIPTS} test_python.sh)
  add_test(examples_python
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=python"
  )
endif(ENABLE_python)

if(ENABLE_tcl)
  configure_file(test_tcl.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_tcl.sh @ONLY)
  set(SCRIPTS ${SCRIPTS} test_tcl.sh)
  add_test(examples_tcl
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=tcl"
  )
endif(ENABLE_tcl)

if(ENABLE_pdl)
  configure_file(
    test_perl.sh.in 
    ${CMAKE_CURRENT_BINARY_DIR}/test_perl.sh 
    @ONLY
    )
  set(SCRIPTS ${SCRIPTS} test_perl.sh)
  add_test(examples_perl
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=perl"
  )
endif(ENABLE_pdl)

if(ENABLE_ada)
  configure_file(
    test_ada.sh.in 
    ${CMAKE_CURRENT_BINARY_DIR}/test_ada.sh 
    @ONLY
    )
  set(SCRIPTS ${SCRIPTS} test_ada.sh)
  add_test(examples_ada
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ${TEST_SCRIPT} --front-end=ada"
  )
endif(ENABLE_ada)

# Run C examples with different drivers
if(PLD_psc)
  if (NOT TEST_DEVICE STREQUAL "psc")
  add_test(examples_psttfc
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ./plplot-test.sh --front-end=c --device=psttfc"
  )
  endif(NOT TEST_DEVICE STREQUAL "psc")
endif(PLD_psc)

if(PLD_psttf)
  if (NOT TEST_DEVICE STREQUAL "psttfc")
  add_test(examples_psttfc
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ./plplot-test.sh --front-end=c --device=psttfc"
  )
  endif(NOT TEST_DEVICE STREQUAL "psttfc")
endif(PLD_psttf)

if(PLD_pstex)
  if (NOT TEST_DEVICE STREQUAL "pstex")
  add_test(examples_pstex
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ./plplot-test.sh --front-end=c --device=pstex"
  )
  endif(NOT TEST_DEVICE STREQUAL "pstex")
endif(PLD_pstex)

if(PLD_png)
  if (NOT TEST_DEVICE STREQUAL "png")
  add_test(examples_png
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ./plplot-test.sh --front-end=c --device=png"
  )
  endif(NOT TEST_DEVICE STREQUAL "png")
endif(PLD_png)

if(PLD_svg)
  if (NOT TEST_DEVICE STREQUAL "svg")
  add_test(examples_svg
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ./plplot-test.sh --front-end=c --device=svg"
  )
  endif(NOT TEST_DEVICE STREQUAL "svg")
endif(PLD_svg)

if(PLD_plmeta)
  if (NOT TEST_DEVICE STREQUAL "plmeta")
  add_test(examples_plmeta
    ${SH_EXECUTABLE} -c "${TEST_ENVIRONMENT} ./plplot-test.sh --front-end=c --device=plmeta"
  )
  endif(NOT TEST_DEVICE STREQUAL "plmeta")
endif(PLD_plmeta)

# Make a copy of lena.pgm to the test subdirectory of the build
# tree so that example 20 will run from there.
# Also need a copy of lena.img for the octave bindings
if(BUILD_TEST)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/examples/c/lena.pgm
    ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
    DEPENDS ${CMAKE_SOURCE_DIR}/examples/c/lena.pgm
  )
  add_custom_target(lena_file ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm)
  if(ENABLE_octave)
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lena.img
      COMMAND ${CMAKE_COMMAND} -E copy
      ${CMAKE_SOURCE_DIR}/bindings/octave/demos/lena.img
      ${CMAKE_CURRENT_BINARY_DIR}/lena.img
      DEPENDS ${CMAKE_SOURCE_DIR}/bindings/octave/demos/lena.img
    )
    add_custom_target(lena_octave_file ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lena.img)
  endif(ENABLE_octave)
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
FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot-test.sh
DESTINATION ${DATA_DIR}/examples
PERMISSIONS ${PERM_SCRIPTS}
)

endif(SH_EXECUTABLE)
