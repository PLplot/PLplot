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

if(WIN32 AND BUILD_SHARED_LIBS)
  SET_SOURCE_FILES_PROPERTIES( ${csironn_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DMAKINGNNDLL"
  )
endif(WIN32 AND BUILD_SHARED_LIBS)

add_library(csironn ${csironn_LIB_SRCS})
set_target_properties(
csironn
PROPERTIES
SOVERSION ${csironn_SOVERSION}
VERSION ${csironn_VERSION}
COMPILE_FLAGS "-I${QHULL_INCLUDE_DIRS}"
INSTALL_NAME_DIR "${LIB_DIR}"
)
target_link_libraries(csironn ${QHULL_LIBRARIES})
install(TARGETS csironn
ARCHIVE DESTINATION ${LIB_DIR}
LIBRARY DESTINATION ${LIB_DIR}
RUNTIME DESTINATION ${BIN_DIR}
)

set(nn_DOCFILES
README
README.1st
)
install(FILES README DESTINATION ${DOC_DIR} RENAME README.nn)
install(FILES README.1st DESTINATION ${DOC_DIR} RENAME README.1st.nn)

endif(HAVE_QHULL)
