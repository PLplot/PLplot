# bindings/d/CMakeLists.txt
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

if(ENABLE_d)
  set(d_interface_INSTALLED_HEADERS plplot.d )

  install(FILES ${d_interface_INSTALLED_HEADERS}
    DESTINATION ${INCLUDE_DIR}
  )

  # Build d bindings
  set(plplotdmd${LIB_TAG}_LIB_SRCS
    plplot.d
  )

  add_library(plplotdmd${LIB_TAG} STATIC ${plplotdmd${LIB_TAG}_LIB_SRCS})

  install(TARGETS plplotdmd${LIB_TAG}
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
  )
  install(EXPORT export_plplot
    DESTINATION ${DATA_DIR}/examples/cmake/modules
    FILE export_plplotd.cmake
    )

endif(ENABLE_d)
