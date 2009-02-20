# examples/f95/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Alan W. Irwin
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

set(f95_SRCS)

set(f95_STRING_INDICES
  "01"
  "02"
  "03"
  "04"
  "05"
  "06"
  "07"
  "08"
  "09"
  "10"
  "11"
  "12"
  "13"
  "14"
  "15"
  "16"
  "16a"
  "17"
  "18"
  "19"
  "20"
  "21"
  "22"
  "23"
  "24"
  "25"
  "26"
  "27"
  "28"
  "29"
  "30"
  "31"
  )

# Process fortran header file to define isnan
if(FORTRAN_HAVE_ISNAN)
  set(HAVE_F77_ISNAN_FALSE !)
else(FORTRAN_HAVE_ISNAN)
  set(HAVE_F77_ISNAN_TRUE !)
endif(FORTRAN_HAVE_ISNAN)
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/plf95demos.inc.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/plf95demos.inc
  )
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/plf95demos.inc
  PROPERTIES GENERATED ON)
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_SOURCE_DIR}/lib/qsastime
  )

if(BUILD_TEST)
  remove_definitions("-DHAVE_CONFIG_H")

  # For CMake-2.6.0 and above, the module files are created by
  # default during the library build in the bindings/f95 directory.
  include_directories(${CMAKE_BINARY_DIR}/bindings/f95)
endif(BUILD_TEST)
foreach(STRING_INDEX ${f95_STRING_INDICES})
  set(f95_SRCS ${f95_SRCS} x${STRING_INDEX}f.f90)
  if(BUILD_TEST)
    # Need unique target name
    add_executable(x${STRING_INDEX}f95 x${STRING_INDEX}f.f90)
    set_target_properties(
      x${STRING_INDEX}f95 PROPERTIES
      OUTPUT_NAME x${STRING_INDEX}f
      )
    target_link_libraries(x${STRING_INDEX}f95 plplotf95${LIB_TAG})
  endif(BUILD_TEST)
endforeach(STRING_INDEX ${f95_STRING_INDICES})

install(FILES ${f95_SRCS}
  DESTINATION ${DATA_DIR}/examples/f95
  )

set(FC ${CMAKE_Fortran_COMPILER})
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
  ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
  )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
  DESTINATION ${DATA_DIR}/examples/f95
  RENAME Makefile
  )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plf95demos.inc
  DESTINATION ${DATA_DIR}/examples/f95
  )

