# cmake/modules/language_support.cmake
#
# Temporary additional general language support is contained within this
# file.  

# This additional function definition is needed to provide a workaround for
# CMake bug 9220.

function(workaround_9220 language language_works)
  #message("DEBUG: language = ${language}")
  set(text
    "project(test NONE)
# Location where PLplot cmake build system first looks for cmake modules.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules)
cmake_minimum_required(VERSION 2.6.0)
message(STATUS \"CMAKE_GENERATOR = ${CMAKE_GENERATOR}\")
#enable_language(${language} OPTIONAL)
enable_language(${language})
"
    )
  file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/language_tests/${language})
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/language_tests/${language})
  file(WRITE ${CMAKE_BINARY_DIR}/language_tests/${language}/CMakeLists.txt
    ${text})
  # Special language support files for various languages:
  # N.B. This list of files has to be maintained to be consistent
  # with anything special we do in terms of language support.
  
  if(language STREQUAL "Ada")
    set(language_special ON)
    set(language_files
      CMakeAdaCompiler.cmake.in
      CMakeAdaInformation.cmake
      CMakeDetermineAdaCompiler.cmake
      CMakeTestAdaCompiler.cmake
      )
  elseif(language STREQUAL "D")
    set(language_special ON)
    set(language_files
      CMakeDCompiler.cmake.in
      CMakeDInformation.cmake
      CMakeDetermineDCompiler.cmake
      CMakeTestDCompiler.cmake
      Platform/Linux-dmd.cmake
      Platform/Linux-gdc.cmake
      Platform/Windows-dmd.cmake
      Platform/Windows-gdc.cmake
      )
  elseif(language STREQUAL "Fortran")
    set(language_special ON)
    set(language_files
      CMakeFortranInformation.cmake
      Platform/Cygwin-GNU-Fortran.cmake
      Platform/Windows-GNU-Fortran.cmake
      Platform/Windows-df.cmake
      Platform/Windows-f90.cmake
      )
  else(language STREQUAL "Ada")
    set(language_special OFF)
  endif(language STREQUAL "Ada")

  if(language_special)
    file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/language_tests/${language}/cmake/modules/Platform)
    foreach(file ${language_files})
      configure_file(
	${CMAKE_SOURCE_DIR}/cmake/modules/${file}
	${CMAKE_BINARY_DIR}/language_tests/${language}/cmake/modules/${file}
	COPYONLY
	)
    endforeach(file ${language_files})
  endif(language_special)

  execute_process(
    COMMAND ${CMAKE_COMMAND} -G ${CMAKE_GENERATOR} .
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/language_tests/${language}
    RESULT_VARIABLE return_code
    #OUTPUT_VARIABLE output
    #ERROR_VARIABLE error
    #OUTPUT_STRIP_TRAILING_WHITESPACE
    #ERROR_STRIP_TRAILING_WHITESPACE
    )
  if(return_code EQUAL 0)
    set(${language_works} ON CACHE INTERNAL "")
  else(return_code EQUAL 0)
    set(${language_works} OFF CACHE INTERNAL "")
    #message(STATUS "A test cmake run with language = ${language} enabled failed with the following stdout and stderr:")
    #message(STATUS "stdout = ${output}")
    #message(STATUS "stderr = ${error}")
  endif(return_code EQUAL 0)
endfunction(workaround_9220)

# Temporary tests of the above function.
#workaround_9220(CXX CXX_language_works)
#message("CXX_language_works = ${CXX_language_works}")
#workaround_9220(CXXp CXXp_language_works)
#message("CXXp_language_works = ${CXXp_language_works}")
#workaround_9220(Ada Ada_language_works)
#message("Ada_language_works = ${Ada_language_works}")
#workaround_9220(D D_language_works)
#message("D_language_works = ${D_language_works}")
#workaround_9220(Fortran Fortran_language_works)
#message("Fortran_language_works = ${Fortran_language_works}")
#workaround_9220(Java Java_language_works)
#message("Java_language_works = ${Java_language_works}")
# Just to terminate temporary test for convenience.
#message(FATAL_ERROR "")


