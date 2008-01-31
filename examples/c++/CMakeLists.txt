# examples/c++/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
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

set(cxx_SRCS
plc++demos.h
)

set(cxx_STRING_INDICES
"01cc"
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
"28"
"29"
"30"
)

if(BUILD_TEST)
  remove_definitions("-DHAVE_CONFIG_H")
  include_directories(
  ${CMAKE_SOURCE_DIR}/bindings/c++
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}/include
  )
endif(BUILD_TEST)
foreach(STRING_INDEX ${cxx_STRING_INDICES})
  set(cxx_SRCS ${cxx_SRCS} x${STRING_INDEX}.cc)
  if(BUILD_TEST)
    add_executable(x${STRING_INDEX} x${STRING_INDEX}.cc)
    if(WIN32 AND BUILD_SHARED_LIBS)
      SET_SOURCE_FILES_PROPERTIES(x${STRING_INDEX}.cc
        PROPERTIES COMPILE_FLAGS "-DUSINGDLL")
    endif(WIN32 AND BUILD_SHARED_LIBS)
    target_link_libraries(x${STRING_INDEX} plplotcxx${LIB_TAG} ${MATH_LIB})
  endif(BUILD_TEST)
endforeach(STRING_INDEX ${cxx_STRING_INDICES})

if(PLD_wxwidgets)
  if(BUILD_TEST)
    add_executable(wxPLplotDemo WIN32 wxPLplotDemo.cpp)
    if(WIN32 AND BUILD_SHARED_LIBS)
      SET_SOURCE_FILES_PROPERTIES(wxPLplotDemo.cpp
        PROPERTIES COMPILE_FLAGS "-DUSINGDLL ${wxwidgets_COMPILE_FLAGS}")
    else(WIN32 AND BUILD_SHARED_LIBS)
      SET_SOURCE_FILES_PROPERTIES(wxPLplotDemo.cpp
        PROPERTIES COMPILE_FLAGS ${wxwidgets_COMPILE_FLAGS})
    endif(WIN32 AND BUILD_SHARED_LIBS)
    include_directories(
    ${CMAKE_SOURCE_DIR}/bindings/wxwidgets
    ${CMAKE_BINARY_DIR}/bindings/wxwidgets
    )
    target_link_libraries(wxPLplotDemo plplotwxwidgets${LIB_TAG} plplotcxx${LIB_TAG} ${MATH_LIB})
  endif(BUILD_TEST)
endif(PLD_wxwidgets)
set(wxPLplotDemo_SRCS
wxPLplotDemo.cpp
)

install(FILES ${cxx_SRCS} ${wxPLplotDemo_SRCS} DESTINATION ${DATA_DIR}/examples/c++)

set(CXX ${CMAKE_CXX_COMPILER})
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
DESTINATION ${DATA_DIR}/examples/c++
RENAME Makefile
)
