# The autoconf-based build of ndiff is beginning to break down so this
# is the start of the implementation of an equivalent build system
# using CMake.  This is just a first proof-of-concept attempt to build
# ndiff and not even bother to test or install it.  But at least the
# build appears to work and so does the resulting ndiff executable.

# Top-level CMakeLists.txt boilerplate.
project(ndiff C)
cmake_minimum_required(VERSION 2.8.2 FATAL_ERROR)

# Configure everything in config.h which must have
# specific values of 1 or 0

set(HAVE_LONG_DOUBLE 0)
find_file(HAVE_CTYPE_H ctype.h)
if(HAVE_CTYPE_H)
  set(HAVE_CTYPE_H 1)
else(HAVE_CTYPE_H)
  set(HAVE_CTYPE_H 0)
endif(HAVE_CTYPE_H)

find_file(HAVE_LIMITS_H limits.h)
if(HAVE_LIMITS_H)
  set(HAVE_LIMITS_H 1)
else(HAVE_LIMITS_H)
  set(HAVE_LIMITS_H 0)
endif(HAVE_LIMITS_H)

find_file(HAVE_STDDEF_H stddef.h)
if(HAVE_STDDEF_H)
  set(HAVE_STDDEF_H 1)
else(HAVE_STDDEF_H)
  set(HAVE_STDDEF_H 0)
endif(HAVE_STDDEF_H)

find_file(HAVE_STDIO_H stdio.h)
if(HAVE_STDIO_H)
  set(HAVE_STDIO_H 1)
else(HAVE_STDIO_H)
  set(HAVE_STDIO_H 0)
endif(HAVE_STDIO_H)

find_file(HAVE_STDLIB_H stdlib.h)
if(HAVE_STDLIB_H)
  set(HAVE_STDLIB_H 1)
else(HAVE_STDLIB_H)
  set(HAVE_STDLIB_H 0)
endif(HAVE_STDLIB_H)

find_file(HAVE_STRING_H string.h)
if(HAVE_STRING_H)
  set(HAVE_STRING_H 1)
else(HAVE_STRING_H)
  set(HAVE_STRING_H 0)
endif(HAVE_STRING_H)

# Code uses HAVE_GMP rather than the expected HAVE_GMP_H
find_file(HAVE_GMP gmp.h)
if(HAVE_GMP)
  set(HAVE_GMP 1)
else(HAVE_GMP)
  set(HAVE_GMP 0)
endif(HAVE_GMP)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/config.h
  @ONLY
)

# Build the ndiff executable:
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

set(ndiff_SRC
  ndiff.c 
  store.c 
  awklib.c
  regexp/regexp.c
  regexp/regerror.c
  )

add_executable(ndiff ${ndiff_SRC}) 

# That wasn't so hard, was it?