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
SET(INC_FILE "${CMAKE_CURRENT_BINARY_DIR}/qsastime_testlib.h")
SET(C_FILE "${CMAKE_CURRENT_BINARY_DIR}/qsastime_probe.c")
SET(MACROS LONGLONG __INT64)
FOREACH(TYPE_MACRO ${MACROS})
  FILE(WRITE "${C_FILE}"
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
}")
  TRY_COMPILE(TYPE_FOUND
    ${CMAKE_BINARY_DIR}
    ${C_FILE}
    OUTPUT_VARIABLE OUTPUT)
  FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
    "Determining if the C compiler has support for 64-bits integers failed with "
    "the following output:\n${OUTPUT}\n\n")
  IF (TYPE_FOUND)
    MESSAGE(STATUS "Check for 64-bits integer type - found")
    FILE(WRITE ${INC_FILE} "#define ${TYPE_MACRO}")
  ELSE (TYPE_FOUND)
    FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
      "Determining if the C compiler has support for 64-bits integers failed with "
      "the following output:\n${OUTPUT}\n\n")
  ENDIF (TYPE_FOUND)
ENDFOREACH(TYPE_MACRO ${MACROS})
IF (NOT TYPE_FOUND)
  FILE(WRITE ${INC_FILE} "#define NO_LONG_LONG")
  MESSAGE(STATUS "Check for 64-bits integer type - failed")
ENDIF (NOT TYPE_FOUND)

SET(MACROS HAVE_SETENV HAVE_PUTENV HAVE_UNDERSCORE_PUTENV)
FOREACH(ENV_MACRO ${MACROS})
  FILE(WRITE "${C_FILE}"
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
  TRY_COMPILE(ENV_FOUND
    ${CMAKE_BINARY_DIR}
    ${C_FILE}
    OUTPUT_VARIABLE OUTPUT)
  IF (ENV_FOUND)
    FILE(APPEND ${INC_FILE} "\n#define ${ENV_MACRO}")
    MESSAGE(STATUS "Check for environment API - found")
  ELSE (ENV_FOUND)
    FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
      "Determining if the C compiler has support for setting environment variables failed with "
      "the following output:\n${OUTPUT}\n\n")
  ENDIF (ENV_FOUND)
ENDFOREACH(ENV_MACRO ${MACROS})
IF (NOT ENV_FOUND)
  FILE(APPEND ${INC_FILE} "\n#define NO_ENV_API")
  MESSAGE(STATUS "Check for environment API - failed")
ENDIF (NOT ENV_FOUND)

#
# The header file has been written, now go on
#
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

add_executable(qsastime_testlib qsastime_testlib.c qsastime.c)
if(MATH_LIB)
  target_link_libraries(qsastime_testlib ${MATH_LIB})
endif(MATH_LIB)
