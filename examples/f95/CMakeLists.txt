# examples/f95/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2015 Alan W. Irwin
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

# N.B. This file is used for both the core build (which installs the examples
# and optionally [depending on BUILD_TEST} builds them) and the installed
# examples build.  The core build has BUILD_TEST OFF or ON at user option
# and CORE_BUILD always ON.  The installed examples build always has
# BUILD_TEST ON and CORE_BUILD OFF.

set(f95_STRING_INDICES
  "00"
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
  "33"
  )

# plf95demos.inc, which is included by x21f.f90, is located
# in ${CMAKE_CURRENT_BINARY_DIR} for the core build tree and
# in ${CMAKE_CURRENT_SOURCE_DIR} for the installed examples.
# Note, some Fortran compilers (e.g., nagfor) by default do not look for included
# files in the location of the source file when the build is occurring in
# a different directory.  To solve this issue in general
# always specify the location of plf95demos.inc using
# ${CMAKE_CURRENT_BINARY_DIR} or ${CMAKE_CURRENT_SOURCE_DIR}
# as appropriate.
if(CORE_BUILD)
  # For CMake-2.6.0 and above, the module files are created by
  # default during the library build in the bindings/f95 directory.
  include_directories(${CMAKE_BINARY_DIR}/bindings/f95 ${CMAKE_CURRENT_BINARY_DIR})
else(CORE_BUILD)
  include_directories(${INCLUDE_DIR} ${F95_MOD_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
endif(CORE_BUILD)

if(CORE_BUILD)
  set(plf95demolib_LIB_SRCS plf95demolib.f90)

  add_library(plf95demolib STATIC ${plf95demolib_LIB_SRCS})
  target_link_libraries(plf95demolib plplotf95)

  install(TARGETS plf95demolib
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/plf95demolib.mod
    DESTINATION ${F95_MOD_DIR}
    )

  set(f95_SRCS)
  foreach(STRING_INDEX ${f95_STRING_INDICES})
    set(f95_SRCS ${f95_SRCS} x${STRING_INDEX}f.f90)
  endforeach(STRING_INDEX ${f95_STRING_INDICES})

  # Process fortran header file to define isnan
  if(FORTRAN_HAVE_ISNAN)
    set(HAVE_F95_ISNAN_FALSE !)
  else(FORTRAN_HAVE_ISNAN)
    set(HAVE_F95_ISNAN_TRUE !)
  endif(FORTRAN_HAVE_ISNAN)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/plf95demos.inc.in
    ${CMAKE_CURRENT_BINARY_DIR}/plf95demos.inc
    )
  set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/plf95demos.inc
    PROPERTIES GENERATED ON)
  include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    )

  install(FILES ${f95_SRCS} README_precision
    DESTINATION ${DATA_DIR}/examples/f95
    )

  set(FC "${CMAKE_Fortran_COMPILER} ${CMAKE_Fortran_COMPILER_ARG1} ${CMAKE_Fortran_FLAGS}")
  if(CMAKE_Fortran_COMPILER MATCHES "nagfor")
    # Extra layer of -Wl indirection required for the nagfor compiler
    string(REPLACE "-Wl," "-Wl,-Wl,," FORTRAN_RPATHCMD ${RPATHCMD})
  else(CMAKE_Fortran_COMPILER MATCHES "nagfor")
    set(FORTRAN_RPATHCMD ${RPATHCMD})
  endif(CMAKE_Fortran_COMPILER MATCHES "nagfor")
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

  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DESTINATION ${DATA_DIR}/examples/f95
    )
endif(CORE_BUILD)

if(BUILD_TEST)

  set_property(GLOBAL PROPERTY TARGETS_examples_f95)
  # nm analysis shows only these libraries are required to link the new Fortran binding
  set(fortran_library_targets plplotf95)
  foreach(STRING_INDEX ${f95_STRING_INDICES})
    # Need unique target name
    add_executable(x${STRING_INDEX}f95 x${STRING_INDEX}f.f90)
    set_target_properties(
      x${STRING_INDEX}f95 PROPERTIES
      OUTPUT_NAME x${STRING_INDEX}f
      )
    target_link_libraries(x${STRING_INDEX}f95 plf95demolib ${fortran_library_targets})
    set_property(GLOBAL APPEND PROPERTY TARGETS_examples_f95 x${STRING_INDEX}f95)
  endforeach(STRING_INDEX ${f95_STRING_INDICES})

  if(CORE_BUILD)
    # Build code that tests auxiliary functions in plf95demolib
    add_executable(test_plf95demolib test_plf95demolib.f90)
    target_link_libraries(test_plf95demolib plf95demolib ${fortran_library_targets})
  endif(CORE_BUILD)
endif(BUILD_TEST)
