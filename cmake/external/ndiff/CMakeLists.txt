# The autoconf-based build of ndiff is beginning to break down so this
# is the start of the implementation of an equivalent build system
# using CMake.  This is just a first proof-of-concept attempt to build
# ndiff and not even bother to test or install it.  But at least the
# build appears to work and so does the resulting ndiff executable.

# Top-level CMakeLists.txt boilerplate.
project(ndiff C)
cmake_minimum_required(VERSION 2.8.0 FATAL_ERROR)

# Set paths for installation
set(
CMAKE_INSTALL_EXEC_PREFIX
${CMAKE_INSTALL_PREFIX}
CACHE PATH "install location for architecture-dependent files"
)

set(
CMAKE_INSTALL_BINDIR
${CMAKE_INSTALL_EXEC_PREFIX}/bin
CACHE PATH "install location for user executables"
)

set(
CMAKE_INSTALL_DATADIR
${CMAKE_INSTALL_PREFIX}/share
CACHE PATH "install location for read-only architecture-independent data"
)

set(
CMAKE_INSTALL_MANDIR
${CMAKE_INSTALL_DATADIR}/man
CACHE PATH "install location for man documentation"
)

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

if(HAVE_GMP)
  set(ndiff_LINK_LIBRARIES
  ${ndiff_LINK_LIBRARIES}
  gmp
  )
endif(HAVE_GMP)

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

target_link_libraries(
  ndiff
  ${ndiff_LINK_LIBRARIES}
)

install(TARGETS ndiff DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES ndiff.man DESTINATION ${CMAKE_INSTALL_MANDIR}/man1 RENAME ndiff.1)

# That wasn't so hard, was it?
