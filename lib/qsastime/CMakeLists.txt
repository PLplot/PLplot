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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
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

add_executable(qsastime_test qsastime_test.c qsastime_extra.c)
target_link_libraries(qsastime_test qsastime)

# Routine for testing just the library functions against their library
# inverses (to test self-consistency when such inverses exist) and also 
# equivalent C library routines.
# N.B. the routine immediately errors out if sizeof(time_t) < 8 or
# sizeof(int) !=4.  The time_t constraint is because we want access to a large
# C library date range to do the tests.  The int constraint is so we
# emulate library results on 32-bit systems.
# The test executable normally only passes these two criteria for
# 64-bit platforms.
add_executable(qsastime_testlib qsastime_testlib.c)
target_link_libraries(qsastime_testlib qsastime)
