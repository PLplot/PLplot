# examples/ada/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2007 Alan W. Irwin
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

set(ada_STRING_INDICES
"01"
"02"
"03"
"04"
"05"
"10"
"12"
"thick01"
"thick02"
"thick03"
"thick04"
"thick05"
"thick10"
"thick12"
)

if(BUILD_TEST)
  remove_definitions("-DHAVE_CONFIG_H")
  include_directories(
  ${CMAKE_SOURCE_DIR}/examples/ada
  ${CMAKE_BINARY_DIR}/bindings/ada
  )
endif(BUILD_TEST)
foreach(STRING_INDEX ${ada_STRING_INDICES})
  set(
  ada_SRCS
  ${ada_SRCS}
  x${STRING_INDEX}a.adb
  )
  if(BUILD_TEST)
    # Note due to limitations of the Ada language support for CMake, the the
    # nominal source file for add_executable is compiled (which is why a
    # real file has to be specified), but otherwise it is ignored except for
    # its .adb suffix which identifies the Ada language.  The actual source
    # file name for the internal gnatmake command that creates the
    # executable is constructed from <TARGET>.adb.  Since no directory can
    # be specified from this construction (don't ask), you must specify the
    # directory of <TARGET>.adb with a -aI option (see below).
    add_executable(
    x${STRING_INDEX}a
    ${CMAKE_CURRENT_SOURCE_DIR}/x${STRING_INDEX}a.adb
    )
  
    # The first -aI option is required to find <TARGET>.adb (see above
    # comment). The second -aI option gives access to the libplplotada spec
    # files. The -aL option gives access to the libplplotada *.ali files.
    # (That location currently works but is probably not guaranteed
    # indefinitely for future versions of CMake.)
 
    set_target_properties(x${STRING_INDEX}a
    PROPERTIES
    LINK_FLAGS
    "-aI${CMAKE_SOURCE_DIR}/examples/ada -aI${CMAKE_BINARY_DIR}/bindings/ada -aL${CMAKE_BINARY_DIR}/bindings/ada/CMakeFiles/plplotada${LIB_TAG}.dir"
    )
    target_link_libraries(x${STRING_INDEX}a plplotada${LIB_TAG})
  endif(BUILD_TEST)
endforeach(STRING_INDEX ${ada_STRING_INDICES})
