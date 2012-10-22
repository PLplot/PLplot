# Find shapelib header and library.
#

# This module defines the following uncached variables:
#  SHAPELIB_FOUND, if false, do not try to use shapelib.
#  SHAPELIB_INCLUDE_DIR, where to find shapefil.h.
#  SHAPELIB_LIBRARIES, the libraries to link against to use shapelib
#  is found.

find_path(SHAPELIB_INCLUDE_DIR shapefil.h /usr/local/include /usr/include)

if(SHAPELIB_INCLUDE_DIR)
  find_library(SHAPELIB_LIBRARY
  NAMES shp
  PATHS /usr/local/lib /usr/lib
  )
  if(SHAPELIB_LIBRARY)
    # Set uncached variables as per standard.
    set(SHAPELIB_FOUND ON)
    set(SHAPELIB_LIBRARIES ${SHAPELIB_LIBRARY})
  endif(SHAPELIB_LIBRARY)
endif(SHAPELIB_INCLUDE_DIR)
	    
if(SHAPELIB_FOUND)
  if(NOT SHAPELIB_FIND_QUIETLY)
    message(STATUS "FindShapelib: Found shapelib header directory, ${SHAPELIB_INCLUDE_DIR}, and library, ${SHAPELIB_LIBRARIES}.")
  endif(NOT SHAPELIB_FIND_QUIETLY)
else(SHAPELIB_FOUND)
  if(SHAPELIB_FIND_REQUIRED)
    message(FATAL_ERROR "FindShapelib: Could not find shapelib header and/or library.")
  endif(SHAPELIB_FIND_REQUIRED)
endif(SHAPELIB_FOUND)
