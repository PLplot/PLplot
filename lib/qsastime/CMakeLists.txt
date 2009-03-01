# lib/qsastime/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2009 Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

#
# Test a few properties of the current C compiler
#
set(INC_FILE "${CMAKE_CURRENT_BINARY_DIR}/qsastime_testlib.h")
set(C_FILE "${CMAKE_CURRENT_BINARY_DIR}/qsastime_probe.c")
set(MACROS LONGLONG __INT64)
set(CONTINUE ON)
foreach(TYPE_MACRO ${MACROS})
  if(CONTINUE)
    file(WRITE "${C_FILE}"
    "#define ${TYPE_MACRO}
#ifdef LONGLONG
typedef long long int longlong;
#endif
  #ifdef __INT64
typedef __int64 longlong;
#endif
int main( int argc, char *argv[]) {
  longlong l;
  exit(0);
}"
    )
    try_compile(TYPE_FOUND
      ${CMAKE_BINARY_DIR}
      ${C_FILE}
      OUTPUT_VARIABLE OUTPUT)
    if(TYPE_FOUND)
      message(STATUS "Check for 64-bits integer type - found")
      file(WRITE ${INC_FILE} "#define ${TYPE_MACRO}")
      file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
	"Determining if the C compiler has support for 64-bits integers succeeded with "
	"the following output:\n${OUTPUT}\n\n")
      set(CONTINUE OFF)
    else(TYPE_FOUND)
      file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
	"Determining if the C compiler has support for 64-bits integers failed with "
	"the following output:\n${OUTPUT}\n\n")
    endif(TYPE_FOUND)
  endif(CONTINUE)
endforeach(TYPE_MACRO ${MACROS})
if(NOT TYPE_FOUND)
  file(WRITE ${INC_FILE} "#define NO_LONG_LONG")
  message(STATUS "Check for 64-bits integer type - failed")
endif(NOT TYPE_FOUND)

set(MACROS HAVE_SETENV HAVE_PUTENV HAVE_UNDERSCORE_PUTENV)
set(CONTINUE ON)
foreach(ENV_MACRO ${MACROS})
  if(CONTINUE)
    file(WRITE "${C_FILE}"
      "#include <stdlib.h>
#define ${ENV_MACRO}
int main( int argc, char *argv[]) {
  char *tz;
  tz = getenv(\"TZ\");
#ifdef HAVE_SETENV
  setenv(\"TZ\", \"\", 1);
  if (tz) {
    setenv(\"TZ\", tz, 1);
  }
#endif
#ifdef HAVE_PUTENV
  putenv(\"TZ=\");
#endif
#ifdef HAVE_UNDERSCORE_PUTENV
  _putenv(\"TZ=\");
#endif
  exit(0);
}")
    try_compile(ENV_FOUND
      ${CMAKE_BINARY_DIR}
      ${C_FILE}
      OUTPUT_VARIABLE OUTPUT)
    if(ENV_FOUND)
      file(APPEND ${INC_FILE} "\n#define ${ENV_MACRO}")
      message(STATUS "Check for environment API - found")
      set(CONTINUE OFF)
    else(ENV_FOUND)
      file(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
	"Determining if the C compiler has support for setting environment variables failed with "
	"the following output:\n${OUTPUT}\n\n")
    endif(ENV_FOUND)
  endif(CONTINUE)
endforeach(ENV_MACRO ${MACROS})
if(NOT ENV_FOUND)
  file(APPEND ${INC_FILE} "\n#define NO_ENV_API")
  message(STATUS "Check for environment API - failed")
endif(NOT ENV_FOUND)

#
# The header file ${INC_FILE} has been written, now go on
#

# Routine for generating a header of data to be used by the
# TAI-UTC transformation code.
add_executable(tai-utc-gen ${CMAKE_CURRENT_SOURCE_DIR}/tai-utc-gen.c)
if(MATH_LIB)
  target_link_libraries(tai-utc-gen ${MATH_LIB})
endif(MATH_LIB)

get_target_property(
  tai-utc-gen_LOCATION
  tai-utc-gen
  LOCATION
  )

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/tai-utc.h
  COMMAND ${tai-utc-gen_LOCATION}
  ${CMAKE_CURRENT_SOURCE_DIR}/tai-utc.dat
  ${CMAKE_CURRENT_BINARY_DIR}/tai-utc.h
  DEPENDS
  ${tai-utc-gen_LOCATION}
  ${CMAKE_CURRENT_SOURCE_DIR}/tai-utc.dat
  )

# This target-level dependency needed below.
add_custom_target(
  tai-utc.h_built
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tai-utc.h
  )

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

set(qsastime_LIB_SRCS
  qsastime.c
  )

if(BUILD_SHARED_LIBS)
  SET_SOURCE_FILES_PROPERTIES( ${qsastime_LIB_SRCS}
    PROPERTIES COMPILE_FLAGS "-DUSINGDLL"
    )
endif(BUILD_SHARED_LIBS)

add_library(qsastime ${qsastime_LIB_SRCS})
add_dependencies(qsastime tai-utc.h_built)

if(MATH_LIB)
  target_link_libraries(qsastime ${MATH_LIB})
endif(MATH_LIB)
set_target_properties(
  qsastime
  PROPERTIES
  SOVERSION ${qsastime_SOVERSION}
  VERSION ${qsastime_VERSION}
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
set(qsastime_INSTALLED_HEADERS
  qsastime.h
  qsastimedll.h
  )

install(FILES ${qsastime_INSTALLED_HEADERS} DESTINATION ${INCLUDE_DIR})

install(TARGETS qsastime
  ARCHIVE DESTINATION ${LIB_DIR}
  LIBRARY DESTINATION ${LIB_DIR}
  RUNTIME DESTINATION ${BIN_DIR}
  )

# qsastime_extra.[ch] provides helper functions for qsastime_test that should
# not be part of the library.
# Add in qsastime.c rather than use the library because don't want
# to make private library API visible.

add_executable(qsastime_test qsastime_test.c qsastime_extra.c qsastime.c)
add_dependencies(qsastime_test tai-utc.h_built)
if(MATH_LIB)
  target_link_libraries(qsastime_test ${MATH_LIB})
endif(MATH_LIB)

# Routine for testing bhunt_search function in qsastime.c.
add_executable(bhunt_search_test bhunt_search_test.c qsastime.c)
add_dependencies(bhunt_search_test tai-utc.h_built)
if(MATH_LIB)
  target_link_libraries(bhunt_search_test ${MATH_LIB})
endif(MATH_LIB)

# Routine for testing just the library functions against their library
# inverses (to test self-consistency when such inverses exist) and also
# equivalent C library routines.
# N.B. the routine immediately errors out at run time if sizeof(time_t) < 8 or
# sizeof(int) !=4.  The time_t constraint is because we want access to a large
# C library date range to do the tests.  The int constraint is so we
# emulate library results on 32-bit systems.
# The test executable normally only passes these two criteria for
# 64-bit platforms.
# Add in qsastime.c rather than use the library because don't want
# to make private library API visible.

option(
  QSASTIME_TESTLIB
  "Option for building qsastime_testlib"
  OFF
  )

if(QSASTIME_TESTLIB)
  add_executable(qsastime_testlib qsastime_testlib.c qsastime.c)
  add_dependencies(qsastime_testlib tai-utc.h_built)
  if(MATH_LIB)
    target_link_libraries(qsastime_testlib ${MATH_LIB})
  endif(MATH_LIB)
endif(QSASTIME_TESTLIB)
