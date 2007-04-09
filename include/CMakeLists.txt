# include/CMakeLists.txt for PLplot
###
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

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plConfig.h.cmake
${CMAKE_CURRENT_BINARY_DIR}/plConfig.h
)

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/plDevs.h.cmake
${CMAKE_CURRENT_BINARY_DIR}/plDevs.h
)

set(plhershey-unicode-gen_SRCS
${CMAKE_SOURCE_DIR}/fonts/plhershey-unicode-gen.c
)

add_executable(plhershey-unicode-gen ${plhershey-unicode-gen_SRCS})

get_target_property(
plhershey-unicode-gen_LOCATION
plhershey-unicode-gen
LOCATION
)

add_custom_command(
OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plhershey-unicode.h
COMMAND ${plhershey-unicode-gen_LOCATION}
${CMAKE_SOURCE_DIR}/fonts/plhershey-unicode.csv
${CMAKE_CURRENT_BINARY_DIR}/plhershey-unicode.h
DEPENDS
${plhershey-unicode-gen_LOCATION}
${CMAKE_SOURCE_DIR}/fonts/plhershey-unicode.csv
)

# For cross-directory dependencies....
add_custom_target(
plhershey-unicode.h_built
DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plhershey-unicode.h
)

set(include_INSTALLED_HEADERS
disptab.h
drivers.h
pdf.h
pldebug.h
pldll.h
plevent.h
plplot.h
plplotP.h
plstrm.h
plxwd.h
gcw.h
plplotcanvas.h
${CMAKE_CURRENT_BINARY_DIR}/plConfig.h
${CMAKE_CURRENT_BINARY_DIR}/plDevs.h
)
install(FILES ${include_INSTALLED_HEADERS} DESTINATION ${INCLUDE_DIR})
