# Find gdi32 header and library for wingcc driver
#

# This module defines the following uncached variables:
#  GDI32_FOUND, if false, do not try to use gdi32.
#  GDI32_INCLUDE_DIRS, where to find cd.h.
#  GDI32_LIBRARIES, the libraries to link against to use gdi32
#  GDI32_LIBRARY_DIRS, the directory where libgdi32.a is found.

check_include_files(windows.h HAVE_WINDOWS_H)

if(HAVE_WINDOWS_H)
  # finding the path where libgdi32.a resides is kind of a hack and only works for mingw
  # should the wingcc driver also work for visual c++?
  get_filename_component(GDI32_LIBRARY_DIRS ${CMAKE_CXX_COMPILER} PATH)
  SET( GDI32_LIBRARY_DIRS ${GDI32_LIBRARY_DIRS}/../lib)
  find_library(
    GDI32_LIBRARY
    NAMES gdi32
    PATHS ${GDI32_LIBRARY_DIRS}
    NO_SYSTEM_ENVIRONMENT_PATH
  )
  if(GDI32_LIBRARY)
    set(GDI32_FOUND ON)
    set(GDI32_INCLUDE_DIRS "")
    set(GDI32_LIBRARIES ${GDI32_LIBRARY})
  endif(GDI32_LIBRARY)
endif(HAVE_WINDOWS_H)
	    
if(GDI32_FOUND)
  if(NOT GDI32_FIND_QUIETLY)
    message(STATUS "FindGDI32: Found both windows.h and libgdi32.a")
  endif(NOT GDI32_FIND_QUIETLY)
else(GDI32_FOUND)
  if(GDI32_FIND_REQUIRED)
    message(FATAL_ERROR "FindGDI32: Could not find windows.h and/or libgdi32.a")
  endif(GDI32_FIND_REQUIRED)
endif(GDI32_FOUND)
