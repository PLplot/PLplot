# Find and check Ada compiler
enable_language(Ada)
if(NOT CMAKE_Ada_COMPILER_WORKS)
  message(FATAL_ERROR "Required working Ada compiler not found.")
endif(NOT CMAKE_Ada_COMPILER_WORKS)

# Find the gnat version used in order to search for the right version of libgnat 
execute_process(COMMAND ${CMAKE_Ada_COMPILER} --version OUTPUT_VARIABLE ADA_OUTPUT)
string(REGEX MATCH "gcc [(][^)]*[)] ([0-9]*)([.][0-9]*)[.][0-9]" ADA_OUTPUT_TRIM ${ADA_OUTPUT})
set(GNATMAJVERSION ${CMAKE_MATCH_1})
set(GNATVERSION ${CMAKE_MATCH_1}${CMAKE_MATCH_2})
message(STATUS "gnat version = ${GNATVERSION}")
find_library(GNAT_LIB NAMES gnat gnat-${GNATVERSION} gnat-${GNATMAJVERSION})
if(NOT GNAT_LIB)
  message(FATAL_ERROR "Required gnat library not found.")
endif(NOT GNAT_LIB)
