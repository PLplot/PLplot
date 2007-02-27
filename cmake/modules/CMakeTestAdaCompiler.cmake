# This file is used by EnableLanguage in cmGlobalGenerator to
# determine that that selected Ada compiler can actually compile
# and link the most basic of programs.   If not, a fatal error
# is set and cmake stops processing commands and will not generate
# any makefiles or projects.
IF(NOT CMAKE_Ada_COMPILER_WORKS)
  MESSAGE(STATUS "Check for working Ada compiler: ${CMAKE_Ada_COMPILER}")
  FILE(WRITE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/testadacompiler.adb
  "procedure testAdaCompiler is\n"
  "     begin\n"
  "       null;\n"
  "     end;\n"
  )
  TRY_COMPILE(CMAKE_Ada_COMPILER_WORKS ${CMAKE_BINARY_DIR} 
    ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/testadacompiler.adb
    OUTPUT_VARIABLE OUTPUT) 
  SET(ADA_TEST_WAS_RUN 1)
ENDIF(NOT CMAKE_Ada_COMPILER_WORKS)

IF(NOT CMAKE_Ada_COMPILER_WORKS)
  MESSAGE(STATUS "Check for working Ada compiler: ${CMAKE_Ada_COMPILER} -- broken")
  FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
    "Determining if the Ada compiler works failed with "
    "the following output:\n${OUTPUT}\n\n")
  MESSAGE(FATAL_ERROR "The Ada compiler \"${CMAKE_Ada_COMPILER}\" "
    "is not able to compile a simple test program.\nIt fails "
    "with the following output:\n ${OUTPUT}\n\n"
    "CMake will not be able to correctly generate this project.")
ELSE(NOT CMAKE_Ada_COMPILER_WORKS)
  IF(ADA_TEST_WAS_RUN)
    MESSAGE(STATUS "Check for working Ada compiler: ${CMAKE_Ada_COMPILER} -- works")
    FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
      "Determining if the Ada compiler works passed with "
      "the following output:\n${OUTPUT}\n\n") 
  ENDIF(ADA_TEST_WAS_RUN)
  SET(CMAKE_Ada_COMPILER_WORKS 1 CACHE INTERNAL "")
ENDIF(NOT CMAKE_Ada_COMPILER_WORKS)
