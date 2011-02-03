# examples/f77/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006, 2007, 2008, 2009 Alan W. Irwin
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

set(f77_STRING_INDICES
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

if(CORE_BUILD)
  # Process .fm4 files to set PLFLT to real / double.
  if(PL_DOUBLE)
    foreach(STRING_INDEX ${f77_STRING_INDICES})
      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f
	COMMAND ${CMAKE_COMMAND}
	-E copy ${CMAKE_CURRENT_SOURCE_DIR}/x${STRING_INDEX}f.fm4 ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/x${STRING_INDEX}f.fm4
	)
    endforeach(STRING_INDEX ${f77_STRING_INDICES})
  else(PL_DOUBLE)
    foreach(STRING_INDEX ${f77_STRING_INDICES})
      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f
	COMMAND sed
	-f ${CMAKE_CURRENT_SOURCE_DIR}/double2single.sed ${CMAKE_CURRENT_SOURCE_DIR}/x${STRING_INDEX}f.fm4 > ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/x${STRING_INDEX}f.fm4
	)
    endforeach(STRING_INDEX ${f77_STRING_INDICES})
  endif(PL_DOUBLE)

  set(f77_SRCS)
  foreach(STRING_INDEX ${f77_STRING_INDICES})
    list(APPEND f77_SRCS ${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f)
    set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/x${STRING_INDEX}f.f
      PROPERTIES GENERATED ON)
  endforeach(STRING_INDEX ${c_STRING_INDICES})
  add_custom_target(f77_examples ALL DEPENDS ${f77_SRCS})

  # Process fortran header file to define isnan
  if(FORTRAN_HAVE_ISNAN)
    set(HAVE_F77_ISNAN_FALSE C)
  else(FORTRAN_HAVE_ISNAN)
    set(HAVE_F77_ISNAN_TRUE C)
  endif(FORTRAN_HAVE_ISNAN)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/plf77demos.inc.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/plf77demos.inc
    )

  install(FILES ${f77_SRCS}
    DESTINATION ${DATA_DIR}/examples/f77
    )

  set(F77 ${CMAKE_Fortran_COMPILER})
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
    ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
    DESTINATION ${DATA_DIR}/examples/f77
    RENAME Makefile
    )

  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plf77demos.inc
    DESTINATION ${DATA_DIR}/examples/f77
    )

  install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DESTINATION ${DATA_DIR}/examples/f77
    )
endif(CORE_BUILD)

if(BUILD_TEST)
  if(CORE_BUILD)
    include_directories(
      ${CMAKE_SOURCE_DIR}/include
      ${CMAKE_BINARY_DIR}/bindings/f77
      ${CMAKE_BINARY_DIR}/include
      )
    set(f77_directory ${CMAKE_CURRENT_BINARY_DIR})
  else(CORE_BUILD)
    include_directories(
      ${INCLUDE_DIR}
      ${F77_INCLUDE_DIR}
      )
    set(f77_directory ${CMAKE_CURRENT_SOURCE_DIR})
    set(f77_target)
  endif(CORE_BUILD)

  set_property(GLOBAL PROPERTY TARGETS_examples_f77)
  foreach(STRING_INDEX ${f77_STRING_INDICES})
    add_executable(x${STRING_INDEX}f ${f77_directory}/x${STRING_INDEX}f.f)
    if(STATIC_OPTS)
      target_link_libraries(x${STRING_INDEX}f plplotf77opts${LIB_TAG} plplotf77${LIB_TAG})
    else(STATIC_OPTS)
      target_link_libraries(x${STRING_INDEX}f plplotf77${LIB_TAG})
    endif(STATIC_OPTS)
    add_dependencies(x${STRING_INDEX}f build_plplot_parameters_h)
    set_property(GLOBAL APPEND PROPERTY TARGETS_examples_f77 x${STRING_INDEX}f)
  endforeach(STRING_INDEX ${f77_STRING_INDICES})
endif(BUILD_TEST)
