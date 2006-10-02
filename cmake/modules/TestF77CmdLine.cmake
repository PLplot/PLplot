# cmake/modules/TestF77CmdLine.cmake
#
# F77 binding configuration
#
# Copyright (C) 2006  Andrew Ross
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
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Check if iargc() works
IF(NOT DEFINED CMAKE_F77_CMD_LINE)
  MESSAGE(STATUS "Check for using namespace support")
  TRY_COMPILE(CMAKE_F77_CMD_LINE
    ${CMAKE_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/cmake/modules/TestF77CmdLine.f
    OUTPUT_VARIABLE OUTPUT)

# Iargc support is broken for with g77 and shared libraries on cygwin
# (as of 2005-12-05, but this problem has been known
# for several years, see http://cygwin.com/ml/cygwin/2005-11/msg00891.html).
  IF (CYGWIN AND CMAKE_COMPILER_IS_GNUG77)
    SET (CMAKE_F77_CMD_LINE OFF)
  ENDIF (CYGWIN AND CMAKE_COMPILER_IS_GNUG77)

  IF (CMAKE_F77_CMD_LINE)
    MESSAGE(STATUS "Check for f77 command line support - found")
    SET (F77_CMD_LINE 1 CACHE INTERNAL
      "Does the f77 compiler support command line arguments")
      FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeOutput.log
      "Determining if the f77 compiler has command line argument support passed with "
      "the following output:\n${OUTPUT}\n\n")
  ELSE (CMAKE_F77_CMD_LINE)
    MESSAGE(STATUS "Check for f77 command line support - not found")
    SET (F77_CMD_LINE 0 CACHE INTERNAL
      "Does the f77 compiler support command line arguments")
      FILE(APPEND ${CMAKE_BINARY_DIR}/CMakeFiles/CMakeError.log
      "Determining if the f77 compiler has command line argument support failedd with "
      "the following output:\n${OUTPUT}\n\n")
  ENDIF (CMAKE_F77_CMD_LINE)
ENDIF(NOT DEFINED CMAKE_F77_CMD_LINE)

