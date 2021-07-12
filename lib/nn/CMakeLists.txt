# lib/nn/CMakeLists.txt for PLplot
# Copyright (C) 2006-2019 Alan W. Irwin
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

if(PL_HAVE_QHULL)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/nndll.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/nndll.h
    )

  set(nn_LIB_HDRS
    ${CMAKE_CURRENT_SOURCE_DIR}/nn.h
    ${CMAKE_CURRENT_BINARY_DIR}/nndll.h
    )

  set(csironn_LIB_SRCS
    delaunay.c
    hash.c
    istack.c
    lpi.c
    nnai.c
    nnpi.c
    nncommon.c
    )

  set(LIB_INSTALL_RPATH ${QHULL_RPATH})
  configure_library_build(csironn "" "${csironn_LIB_SRCS}" Qhull::qhull_r "${QHULL_RPATH}")
  target_include_directories(${WRITEABLE_TARGET}csironn
    PUBLIC
    $<INSTALL_INTERFACE:${INCLUDE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )

  set(nn_DOCFILES
    README
    )
  install(FILES README DESTINATION ${DOC_DIR} RENAME README.nn)

  install(FILES ${nn_LIB_HDRS} DESTINATION ${INCLUDE_DIR})

endif(PL_HAVE_QHULL)
