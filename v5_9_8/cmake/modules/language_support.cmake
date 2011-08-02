# cmake/modules/language_support.cmake
#
# Temporary additional general language support is contained within this
# file.  

# This additional function definition is needed to provide a workaround for
# CMake bug 9220.

option(ENABLE_workaround_9220 "Enable a workaround for cmake bug 9220" ON)

if(ENABLE_workaround_9220)
  function(workaround_9220 language language_works)
    #message("DEBUG: language = ${language}")
    set(text
      "project(test C)
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
	language_support/cmake/CMakeAdaCompiler.cmake.in
	language_support/cmake/CMakeAdaInformation.cmake
	language_support/cmake/CMakeDetermineAdaCompiler.cmake
	language_support/cmake/CMakeTestAdaCompiler.cmake
	)
    elseif(language STREQUAL "D")
      set(language_special ON)
      set(language_files
	language_support/cmake/CMakeDCompiler.cmake.in
	language_support/cmake/CMakeDInformation.cmake
	language_support/cmake/CMakeDetermineDCompiler.cmake
	language_support/cmake/CMakeTestDCompiler.cmake
	language_support/cmake/Platform/Linux-dmd.cmake
	language_support/cmake/Platform/Linux-gdc.cmake
	language_support/cmake/Platform/Windows-dmd.cmake
	language_support/cmake/Platform/Windows-gdc.cmake
	)
    elseif(language STREQUAL "Fortran")
      if(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION EQUAL 6)
	set(language_special ON)
	set(language_files
	  language_support/cmake-2.6/CMakeFortranInformation.cmake
	  language_support/cmake-2.6/Platform/Cygwin-GNU-Fortran.cmake
	  language_support/cmake-2.6/Platform/Windows-GNU-Fortran.cmake
	  language_support/cmake-2.6/Platform/Windows-df.cmake
	  language_support/cmake-2.6/Platform/Windows-f90.cmake
	  )
      else(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION EQUAL 6)
	set(language_special OFF)
      endif(CMAKE_MAJOR_VERSION EQUAL 2 AND CMAKE_MINOR_VERSION EQUAL 6)
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
else(ENABLE_workaround_9220)
  function(workaround_9220 language language_works)
    # Dummy version that always returns success.
    set(${language_works} ON CACHE INTERNAL "")
  endfunction(workaround_9220)
endif(ENABLE_workaround_9220)

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


