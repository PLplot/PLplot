# Find cd header and library.  This is the public domain library 
# (see http://prdownloads.sourceforge.net/plplot/cd1.3.tar.gz?download
# for a copy) for creating CGM (Computer Graphics Metafile, see
# http://www.agocg.ac.uk/train/cgm/cgm.htm) files.
#

# This module defines the following uncached variables:
#  CD_FOUND, if false, do not try to use CD.
#  CD_INCLUDE_DIRS, where to find cd.h.
#  CD_LIBRARIES, the libraries to link against to use libcd.a
#  CD_LIBRARY_DIRS, the directory where libcd.a is found.

find_path(CD_INCLUDE_DIR cd.h /usr/local/include /usr/include)

if(CD_INCLUDE_DIR)
  find_library(CD_LIBRARY
  NAMES cd
  PATHS /usr/local/lib /usr/lib
  )
  if(CD_LIBRARY)
    set(CD_LIBRARY_DIR "")
    get_filename_component(CD_LIBRARY_DIRS ${CD_LIBRARY} PATH)
    # Set uncached variables as per standard.
    set(CD_FOUND ON)
    set(CD_INCLUDE_DIRS ${CD_INCLUDE_DIR})
    set(CD_LIBRARIES ${CD_LIBRARY})
  endif(CD_LIBRARY)
endif(CD_INCLUDE_DIR)
	    
if(CD_FOUND)
  if(NOT CD_FIND_QUIETLY)
    message(STATUS "FindCD: Found both cd.h and libcd.a")
  endif(NOT CD_FIND_QUIETLY)
else(CD_FOUND)
  if(CD_FIND_REQUIRED)
    message(FATAL_ERROR "FindCD: Could not find cd.h and/or libcd.a")
  endif(CD_FIND_REQUIRED)
endif(CD_FOUND)
