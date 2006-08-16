# Find gdi32 header and library for wingcc driver
#

# This module defines the following uncached variables:
#  GDI32_FOUND, if false, do not try to use gdi32.
#  GDI32_INCLUDE_DIRS, where to find cd.h.
#  GDI32_LIBRARIES, the libraries to link against to use gdi32
#  GDI32_LIBRARY_DIRS, the directory where libgdi32.a is found.

SET(GDI32_LIBRARY gdi32)

TRY_COMPILE(TESTGDI32
  ${CMAKE_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/cmake/modules/FindGDI32.c
  CMAKE_FLAGS -DLINK_LIBRARIES=${GDI32_LIBRARY}
  OUTPUT_VARIABLE OUTPUT)
FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
  "Determining if gdi32 is available with "
  "the following output:\n${OUTPUT}\n\n")
IF(TESTGDI32)
  set(GDI32_FOUND ON)
  set(GDI32_INCLUDE_DIRS "")
  set(GDI32_LIBRARY_DIRS "")
  set(GDI32_LIBRARIES ${GDI32_LIBRARY})
ELSE(TESTGDI32)
  set(GDI32_FOUND OFF)
  set(GDI32_INCLUDE_DIRS "")
  set(GDI32_LIBRARY_DIRS "")
  set(GDI32_LIBRARIES "")
ENDIF(TESTGDI32)

if(GDI32_FOUND)
  if(NOT GDI32_FIND_QUIETLY)
    message(STATUS "FindGDI32: Found gdi32 header file and library")
  endif(NOT GDI32_FIND_QUIETLY)
else(GDI32_FOUND)
  if(GDI32_FIND_REQUIRED)
    message(FATAL_ERROR "FindGDI32: Could not find gdi32 header file and/or library")
  endif(GDI32_FIND_REQUIRED)
endif(GDI32_FOUND)
