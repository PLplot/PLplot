# lib/csa/CMakeLists.txt for PLplot
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

if(WITH_CSA)

set(csirocsa_LIB_SRCS
csa.c
)

if(WIN32 AND BUILD_SHARED_LIBS)
  SET_SOURCE_FILES_PROPERTIES(${csirocsa_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DMAKINGPLDLL" )
  include_directories(
      ${CMAKE_SOURCE_DIR}/include
  )
endif(WIN32 AND BUILD_SHARED_LIBS)

add_library(csirocsa ${csirocsa_LIB_SRCS})
SET_TARGET_PROPERTIES(csirocsa
PROPERTIES SOVERSION ${csirocsa_SOVERSION} VERSION ${csirocsa_VERSION}
INSTALL_NAME_DIR "${LIB_DIR}"
)
install(TARGETS csirocsa DESTINATION ${LIB_DIR})

set(csa_DOCFILES
README
README.1st
)
install(FILES README DESTINATION ${DOC_INSTALL_DIR} RENAME README.csa)
install(FILES README.1st DESTINATION ${DOC_INSTALL_DIR} RENAME README.1st.csa)

endif(WITH_CSA)
