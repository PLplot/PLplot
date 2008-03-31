# examples/d/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2008 Werner Smekal
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

set(d_STRING_INDICES
"03"
"04"
"05"
"06"
"07"
"08"
"10"
"12"
"13"
)

foreach(STRING_INDEX ${d_STRING_INDICES})
  set(d_SRCS ${d_SRCS} x${STRING_INDEX}d.d)
endforeach(STRING_INDEX ${d_STRING_INDICES})

install(FILES ${d_SRCS}
DESTINATION ${DATA_DIR}/examples/d
)

#set(DD ${CMAKE_D_COMPILER})
#configure_file(
#${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
#${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
#)

#install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
#DESTINATION ${DATA_DIR}/examples/d
#RENAME Makefile
#)
