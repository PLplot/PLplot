# Find haru pdf header and library.
#

# This module defines the following uncached variables:
#  hpdf_FOUND, if false, do not try to use haru pdf.
#  hpdf_INCLUDE_DIRS, where to find hpdf.h.
#  hpdf_LIBRARIES, the libraries to link against to use haru pdf
#  hpdf_LIBRARY_DIRS, the directory where libhpdf.a is found.

find_path(hpdf_INCLUDE_DIR hpdf.h /usr/local/include /usr/include)

if(hpdf_INCLUDE_DIR)
  find_library(hpdf_LIBRARY
  NAMES hpdf
  PATHS /usr/local/lib /usr/lib
  )
  if(hpdf_LIBRARY)
    set(hpdf_LIBRARY_DIR "")
    get_filename_component(hpdf_LIBRARY_DIRS ${hpdf_LIBRARY} PATH)
    # Set uncached variables as per standard.
    set(hpdf_FOUND ON)
    set(hpdf_INCLUDE_DIRS ${hpdf_INCLUDE_DIR})
    set(hpdf_LIBRARIES ${hpdf_LIBRARY})
  endif(hpdf_LIBRARY)
endif(hpdf_INCLUDE_DIR)
	    
if(hpdf_FOUND)
  if(NOT hpdf_FIND_QUIETLY)
    message(STATUS "Findhpdf: Found both hpdf.h and libhpdf.a")
  endif(NOT hpdf_FIND_QUIETLY)
else(hpdf_FOUND)
  if(hpdf_FIND_REQUIRED)
    message(FATAL_ERROR "Findhpdf: Could not find hpdf.h and/or libhpdf.a")
  endif(hpdf_FIND_REQUIRED)
endif(hpdf_FOUND)
