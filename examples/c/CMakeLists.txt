# examples/c/CMakeLists.txt
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

set(c_SRCS
plcdemos.h
tutor.c
)

set(c_STRING_INDICES
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
)

if(BUILD_TEST)
  remove_definitions("-DHAVE_CONFIG_H")
  include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_BINARY_DIR}/include)
endif(BUILD_TEST)
foreach(STRING_INDEX ${c_STRING_INDICES})
  set(c_SRCS ${c_SRCS} x${STRING_INDEX}c.c)
  if(BUILD_TEST)
    add_executable(x${STRING_INDEX}c x${STRING_INDEX}c.c)
    if(WIN32 AND BUILD_SHARED_LIBS)
        SET_SOURCE_FILES_PROPERTIES(x${STRING_INDEX}c.c
          PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
    endif(WIN32 AND BUILD_SHARED_LIBS)
    target_link_libraries(x${STRING_INDEX}c plplot${LIB_TAG} ${MATH_LIB})
  endif(BUILD_TEST)
endforeach(STRING_INDEX ${c_STRING_INDICES})

set(plplotcanvas_SRCS
plplotcanvas_demo.c
plplotcanvas_animation.c
)

set(plplotcanvas_DOCS
README.plplotcanvas
)

install(FILES ${c_SRCS} ${plplotcanvas_SRCS} ${plplotcanvas_DOCS}
DESTINATION ${DATA_DIR}/examples/c
)

set(CC ${CMAKE_C_COMPILER})
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
DESTINATION ${DATA_DIR}/examples/c
RENAME Makefile
)

# Install lena.pgm in installed examples directory so that all implementations
# of example 20 in the various examples subdirectories can conveniently
# access this file.
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm
DESTINATION ${DATA_DIR}/examples
)

# Do the same as above for the build tree for those who want to individually
# run the various example 20 implementations from the examples subdirectories
# of the build tree.

if(BUILD_TEST)
  add_custom_command(
  OUTPUT ${CMAKE_BINARY_DIR}/examples/lena.pgm
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm
  ${CMAKE_BINARY_DIR}/examples/lena.pgm
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm
  )
  add_custom_target(examples_lena_file ALL
  DEPENDS ${CMAKE_BINARY_DIR}/examples/lena.pgm
  )
endif(BUILD_TEST)
