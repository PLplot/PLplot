# Find the required gnat library, then enable Ada language support.

find_library(GNAT_LIB NAMES gnat gnat-4.1 gnat-4.2 gnat-4.3)
if(NOT GNAT_LIB)
  message(FATAL_ERROR "Required gnat library not found.")
else(NOT GNAT_LIB)
  message(STATUS "FOUND gnat library ${GNAT_LIB}")
endif(NOT GNAT_LIB)
enable_language(Ada)
