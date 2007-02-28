# This file is used by EnableLanguage in cmGlobalGenerator to
# determine that that the Ada builder gnatmake can actually compile
# and link the most basic of programs.   If not, a fatal error
# is set and cmake stops processing commands and will not generate
# any makefiles or projects.
IF(NOT CMAKE_Ada_COMPILER_WORKS)
  MESSAGE(STATUS "Check for working Ada builder: gnatmake")
  FILE(WRITE ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp/testadacompiler.adb
  "procedure testAdaCompiler is\n"
  "     begin\n"
  "       null;\n"
  "     end;\n"
  )
  execute_process(
  COMMAND gnatmake testadacompiler.adb
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeTmp
  RESULT_VARIABLE CMAKE_Ada_COMPILER_WORKS
  OUTPUT_VARIABLE OUTPUT
  ERROR_VARIABLE OUTPUT
  )
  if(NOT CMAKE_Ada_COMPILER_WORKS)
    set(CMAKE_Ada_COMPILER_WORKS ON)
  else(NOT CMAKE_Ada_COMPILER_WORKS)
    set(CMAKE_Ada_COMPILER_WORKS OFF)
  endif(NOT CMAKE_Ada_COMPILER_WORKS)

  SET(ADA_TEST_WAS_RUN 1)
ENDIF(NOT CMAKE_Ada_COMPILER_WORKS)

IF(NOT CMAKE_Ada_COMPILER_WORKS)
  MESSAGE(STATUS "Check for working Ada builder: gnatmake -- broken")
  FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
    "Determining if the Ada builder works failed with "
    "the following output:\n${OUTPUT}\n\n")
  MESSAGE(FATAL_ERROR "The Ada builder \"gnatmake\" "
    "is not able to compile, bind, and link a simple test program.\nIt fails "
    "with the following output:\n ${OUTPUT}\n\n"
    "CMake will not be able to correctly generate this project.")
ELSE(NOT CMAKE_Ada_COMPILER_WORKS)
  IF(ADA_TEST_WAS_RUN)
    MESSAGE(STATUS "Check for working Ada builder: gnatmake -- works")
    FILE(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
      "Determining if the Ada builder works passed with "
      "the following output:\n${OUTPUT}\n\n") 
  ENDIF(ADA_TEST_WAS_RUN)
  SET(CMAKE_Ada_COMPILER_WORKS 1 CACHE INTERNAL "")
ENDIF(NOT CMAKE_Ada_COMPILER_WORKS)
