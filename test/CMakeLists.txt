
set(TEST_ENVIRONMENT
  "EXAMPLES_DIR=${CMAKE_BINARY_DIR}/examples SRC_EXAMPLES_DIR=${CMAKE_SOURCE_DIR}/examples PLPLOT_LIB=${CMAKE_SOURCE_DIR}/data/"
)

set(JAVA_TEST_ENVIRONMENT "${TEST_ENVIRONMENT} PLPLOT_JAVA_WRAP_DIR=${CMAKE_BINARY_DIR}/bindings/java/ PLPLOT_CLASSPATH=${CMAKE_BINARY_DIR}/examples/java/plplot.jar")

configure_file(
  plplot-test.sh.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/plplot-test.sh 
  @ONLY
  )

configure_file(test_c.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_c.sh @ONLY)
set(SCRIPTS test_c.sh)
if (BUILD_TEST)
add_test(examples_c
  sh -c "${TEST_ENVIRONMENT} ./plplot-test.sh --front-end=c"
  )
endif (BUILD_TEST)

if(ENABLE_c++)
  configure_file(test_cxx.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_cxx.sh @ONLY)
  set(SCRIPTS ${SCRIPTS} test_cxx.sh)
  if (BUILD_TEST)
  add_test(examples_cxx
    sh -c "${TEST_ENVIRONMENT} ./plplot-test.sh --front-end=cxx"
  )
  endif (BUILD_TEST)
endif(ENABLE_c++)

if(ENABLE_f77)
  configure_file(test_f77.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_f77.sh @ONLY)
  set(SCRIPTS ${SCRIPTS} test_f77.sh)
endif(ENABLE_f77)

if(ENABLE_f95)
  configure_file(test_f95.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_f95.sh @ONLY)
  set(SCRIPTS ${SCRIPTS} test_f95.sh)
endif(ENABLE_f95)

if(ENABLE_java)
  configure_file(
    test_java.sh.in 
    ${CMAKE_CURRENT_BINARY_DIR}/test_java.sh 
    @ONLY
    )
  set(SCRIPTS ${SCRIPTS} test_java.sh)
  if (BUILD_TEST)
  add_test(examples_java
    sh -c "${JAVA_TEST_ENVIRONMENT} ./plplot-test.sh --front-end=java" 
  )
  endif (BUILD_TEST)
endif(ENABLE_java)

if(ENABLE_octave)
  configure_file(
    test_octave.sh.in 
    ${CMAKE_CURRENT_BINARY_DIR}/test_octave.sh 
    @ONLY
    )
  set(SCRIPTS ${SCRIPTS} test_octave.sh)
endif(ENABLE_octave)

if(ENABLE_python)
  configure_file(
    test_python.sh.in 
    ${CMAKE_CURRENT_BINARY_DIR}/test_python.sh 
    @ONLY
    )
  set(SCRIPTS ${SCRIPTS} test_python.sh)
endif(ENABLE_python)

if(ENABLE_tcl)
  configure_file(test_tcl.sh.in ${CMAKE_CURRENT_BINARY_DIR}/test_tcl.sh @ONLY)
  set(SCRIPTS ${SCRIPTS} test_tcl.sh)
endif(ENABLE_tcl)

if(ENABLE_perl)
  configure_file(
    test_perl.sh.in 
    ${CMAKE_CURRENT_BINARY_DIR}/test_perl.sh 
    @ONLY
    )
  set(SCRIPTS ${SCRIPTS} test_perl.sh)
endif(ENABLE_perl)


foreach(SCRIPT ${SCRIPTS})
  install(  FILES ${CMAKE_CURRENT_BINARY_DIR}/${SCRIPT} DESTINATION ${DATA_DIR}/examples)
endforeach(SCRIPT ${SCRIPTS})
install(  FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot-test.sh DESTINATION ${DATA_DIR}/examples)

