# Top-level CMakeLists.txt file to help (along with config.h.cmake) to
# implement a complete CMake-based build system for ndiff.

project(ndiff C)

message(STATUS "CMake version = ${CMAKE_VERSION}")
message(STATUS "CMAKE_SYSTEM = ${CMAKE_SYSTEM}")
message(STATUS "CMAKE_GENERATOR = ${CMAKE_GENERATOR}")
message(STATUS "CMAKE_INSTALL_PREFIX = ${CMAKE_INSTALL_PREFIX}")

# Maintenance 2020-02, for convenience keep this identical with
# the minimum CMake version used by PLplot.
cmake_minimum_required(VERSION 3.13.2 FATAL_ERROR)

# Copied from cmake/modules/plplot_functions, but given
# a different name (ndiff_ prefix) in anticipation
# that there will be an option in the future to
# build, test, and install ndiff both independently
# and as part of a PLplot build, test, and install.

function(ndiff_configure_file_generate)
  # Configure files that contain both normal items
  # to configure (referenced as ${VAR} or @VAR@) as well as
  # generator expressions (referenced as $<...>).
  # The arguments of this function have exactly the
  # same syntax and meaning as configure_file.

  list(GET ARGV 0 input_file)
  list(GET ARGV 1 output_file)
  set(configure_file_generate_suffix "_cf_only")
  set(intermediate_file ${output_file}${configure_file_generate_suffix})

  # Locally modify ARGV so that output file for configure file is
  # redirected to intermediate_file.
  list(REMOVE_AT ARGV 1)
  list(INSERT ARGV 1 ${intermediate_file})

  # Configure all but generator expressions.
  configure_file(${ARGV})

  # Configure generator expressions.
  # N.B. these ${output_file} results will only be available
  # at generate time.
  file(GENERATE
    OUTPUT ${output_file}
    INPUT ${intermediate_file}
    )
endfunction(ndiff_configure_file_generate)

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

# Implement test system.
find_program(BASH_EXECUTABLE bash)
find_program(CAT_EXECUTABLE cat)
find_program(CMP_EXECUTABLE cmp)
find_program(DIFF_EXECUTABLE diff)
if(BASH_EXECUTABLE AND CAT_EXECUTABLE AND CMP_EXECUTABLE AND DIFF_EXECUTABLE)
  set(BUILD_TEST ON)
else(BASH_EXECUTABLE AND CAT_EXECUTABLE AND CMP_EXECUTABLE AND DIFF_EXECUTABLE)
  message(STATUS "WARNING: bash, cat, cmp, or diff not available so disabling testing")
  set(BUILD_TEST OFF CACHE BOOL "Enable testing" FORCE)
endif(BASH_EXECUTABLE AND CAT_EXECUTABLE AND CMP_EXECUTABLE AND DIFF_EXECUTABLE)

if(BUILD_TEST)
  if(WIN32)
    find_program(TR_EXECUTABLE tr)
    find_program(SED_EXECUTABLE sed)
    if(TR_EXECUTABLE AND SED_EXECUTABLE)
      set(FILTER "| ${TR_EXECUTABLE} -d $'\\r' | sed -e's?\\([0-9]e[+-]*\\)0\\([0-9][0-9]\\)?\\1\\2?'")
      #message(STATUS "DEBUG: FILTER = ${FILTER}")
    else(TR_EXECUTABLE AND SED_EXECUTABLE)
      message(STATUS "WARNING: tr or sed not available so disabling testing on a Windows platform")
      set(BUILD_TEST OFF CACHE BOOL "Enable testing" FORCE)
    endif(TR_EXECUTABLE AND SED_EXECUTABLE)
  else(WIN32)
    set(FILTER)
  endif(WIN32)
endif(BUILD_TEST)

if(BUILD_TEST)
  enable_testing()
  set(check_LIST)
  foreach(index RANGE 1 26)
    if(index LESS "10")
      list(APPEND check_LIST "check00${index}")
    else(index LESS "10")
      list(APPEND check_LIST "check0${index}")
    endif(index LESS "10")
  endforeach(index RANGE 1 26)
  #message(STATUS "DEBUG: check_LIST = ${check_LIST}")

  set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
  if(HAVE_GMP)
    message(STATUS "Configuring tests using gmp precision")
    set(CHECK_DIR ${TEST_DIR}/okay-gmp)
  elseif(HAVE_LONG_DOUBLE)
    message(STATUS
      "Configuring tests using long double precision. For this case, many tests\n   will fail on platforms where long double corresponds to 80-bit (rather than\n   128-bit) floating point."
      )
    set(CHECK_DIR ${TEST_DIR}/okay-ldb)
  else(HAVE_GMP)
    message(STATUS "Configuring tests using double precision")
    set(CHECK_DIR ${TEST_DIR}/okay-dbl)
  endif(HAVE_GMP)

  file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_area)

  ndiff_configure_file_generate(
    ${CMAKE_CURRENT_SOURCE_DIR}/ndiff_check.sh.in
    ${CMAKE_CURRENT_BINARY_DIR}/test_area/ndiff_check.sh
    )

  foreach(check ${check_LIST})
    add_test(
      NAME ${check}
      COMMAND ${BASH_EXECUTABLE} ndiff_check.sh ${check}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_area
      )
  endforeach(check ${check_LIST})

  # N.B. AWI briefly considered testing ndiff.awk as well, but that is
  # not done in the original build system.  Furthermore, it would take
  # considerable effort to translate from the C version option list to
  # the form of option list acceptable to awk and ndiff.awk. But the
  # dealbreaker is the C version has some extra options (such as
  # -author, -copyright, -copyleft, -www) which the awk version
  # doesn't know about so the first 6 tests and the 26th test that
  # test these "extra" options would fail unless you faked it.

endif(BUILD_TEST)
