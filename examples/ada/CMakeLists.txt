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

if(BUILD_TEST)

# Note due to limitations of the Ada language support for CMake, the
# the nominal source file for add_executable is compiled (which is why a
# real file has to be specified), but otherwise it is ignored except for its
# .adb suffix which identifies the Ada language.  The actual source file
# name for the internal gnatmake command that creates the executable is
# constructed from <TARGET>.adb.  Since no directory can be specified from
# this construction (don't ask), you must specify the directory of
# <TARGET>.adb with a -aI option (see below).
  
  add_executable(ada_plplot ${CMAKE_CURRENT_SOURCE_DIR}/ada_plplot.adb)
  
# The first -aI option is required to find <TARGET>.adb (see above comment).
# The second -aI option gives access to the libplplotada spec files.
# The -aL option gives access to the libplplotada *.ali files.
# (That location currently works but is probably not guaranteed indefinitely
# for future versions of CMake.)
  set_target_properties(ada_plplot
  PROPERTIES
  LINK_FLAGS
  "\\
  -aI${CMAKE_SOURCE_DIR}/examples/ada \\
  -aI${CMAKE_SOURCE_DIR}/bindings/ada \\
  -aL${CMAKE_BINARY_DIR}/bindings/ada/CMakeFiles/plplotada${LIB_TAG}.dir \\
  "
  )
  
  target_link_libraries(ada_plplot plplotada${LIB_TAG})

endif(BUILD_TEST)
