# lib/nn/CMakeLists.txt for PLplot
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

if(HAVE_QHULL)

set(csironn_LIB_SRCS
delaunay.c
hash.c
istack.c
lpi.c
nnai.c
nnpi.c
nncommon.c
)

add_library(csironn ${csironn_LIB_SRCS})
set_target_properties(
csironn
PROPERTIES
SOVERSION ${csironn_SOVERSION}
VERSION ${csironn_VERSION}
INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
)
target_link_libraries(csironn ${QHULL_LIB})
install(TARGETS csironn DESTINATION ${LIB_INSTALL_DIR})

set(nn_DOCFILES
README
README.1st
)
install(FILES README DESTINATION ${DOC_INSTALL_DIR} RENAME README.nn)
install(FILES README.1st DESTINATION ${DOC_INSTALL_DIR} RENAME README.1st.nn)

endif(HAVE_QHULL)
