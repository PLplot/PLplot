# Find qhull header and library.
#

# This module defines the following uncached variables:
#  QHULL_FOUND, if false, do not try to use qhull.
#  QHULL_INCLUDE_DIRS, where to find libqhull/qhull_a.h OR qhull/qhull_a.h.
#  HAS_LIBQHULL_INCLUDE
#    if true use the libqhull/qhull_a.h form of header,
#    if false use the qhull/qhull_a.h form of header.
#  QHULL_LIBRARIES, the libraries to link against to use the qhull library
#  QHULL_LIBRARY_DIRS, the directory where the qhull library is found.

include(FindPackageHandleStandardArgs)

find_path(QHULL_INCLUDE_DIR qhull/qhull_a.h)
if(QHULL_INCLUDE_DIR)
  set(HAS_LIBQHULL_INCLUDE OFF)
else(QHULL_INCLUDE_DIR)
  find_path(QHULL_INCLUDE_DIR libqhull/qhull_a.h)
  if(QHULL_INCLUDE_DIR)
    set(HAS_LIBQHULL_INCLUDE ON)
  endif(QHULL_INCLUDE_DIR)
endif(QHULL_INCLUDE_DIR)

if(QHULL_INCLUDE_DIR)
  set(QHULL_INCLUDE_DIRS ${QHULL_INCLUDE_DIR})
elseif(NOT QHULL_INCLUDE_DIR AND NOT QHULL_FIND_QUIETLY)
  message(STATUS "Neither qhull/qhull_a.h header nor libqhull/qhull_a.h could be found")
endif(QHULL_INCLUDE_DIR)

find_library(QHULL_LIBRARY qhull)
if( QHULL_LIBRARY )
  get_filename_component(QHULL_LIBRARY_DIRS ${QHULL_LIBRARY} PATH)
  # Set uncached variables as per standard.
  set(QHULL_LIBRARIES ${QHULL_LIBRARY})
elseif(NOT QHULL_LIBRARY AND NOT QHULL_FIND_QUIETLY)
  message(STATUS "qhull library could not be found")
endif(QHULL_LIBRARY)

find_package_handle_standard_args(QHULL 
  REQUIRED_VARS QHULL_INCLUDE_DIRS QHULL_LIBRARIES QHULL_LIBRARY_DIRS
  )
