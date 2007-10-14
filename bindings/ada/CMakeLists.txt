# bindings/ada/CMakeLists.txt
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

if(ENABLE_ada)

  set(SOURCE_LIST
  plplot.adb
  plplot.ads
  plplot_thin.adb
  plplot_thin.ads
  plplot_traditional.adb
  plplot_traditional.ads
  plplot_auxiliary.adb
  plplot_auxiliary.ads
  )

  set(plplotada${LIB_TAG}_LIB_SRCS)
  foreach(SOURCE_FILE ${SOURCE_LIST})
    configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
    )
    set(plplotada${LIB_TAG}_LIB_SRCS
    ${plplotada${LIB_TAG}_LIB_SRCS}
    ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_FILE}
    )
  endforeach(SOURCE_FILE ${SOURCE_LIST})

  add_library(plplotada${LIB_TAG} ${plplotada${LIB_TAG}_LIB_SRCS})

  target_link_libraries(plplotada${LIB_TAG}
  plplot${LIB_TAG} ${GNAT_LIB}
  )

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
    set_target_properties(plplotada${LIB_TAG}
    PROPERTIES
    SOVERSION ${plplotada_SOVERSION}
    VERSION ${plplotada_VERSION}
    INSTALL_RPATH "${LIB_INSTALL_RPATH}"
    INSTALL_NAME_DIR "${LIB_DIR}"
    )
  else(USE_RPATH)
    set_target_properties(plplotada${LIB_TAG}
    PROPERTIES
    SOVERSION ${plplotada_SOVERSION}
    VERSION ${plplotada_VERSION}
    INSTALL_NAME_DIR "${LIB_DIR}"
    )
  endif(USE_RPATH)

# The commands below have been tested to work fine for the shared libraries,
# but comment this out until we can decide on an install location for
# all the Ada stuff that is required beyond just the library.
#  install(TARGETS plplotada${LIB_TAG}
#  ARCHIVE DESTINATION ${LIB_DIR}
#  LIBRARY DESTINATION ${LIB_DIR}
#  RUNTIME DESTINATION ${BIN_DIR}
#  )

endif(ENABLE_ada)
