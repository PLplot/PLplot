# bindings/gnome2/lib/CMakeLists.txt
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

if(ENABLE_gnome2)

  set(plplotgnome2_LIB_SRCS
    plplotcanvas.c
    gcw-lib.c
    )

  if(BUILD_SHARED_LIBS)
    SET_SOURCE_FILES_PROPERTIES(
      ${plplotgnome2_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL"
      )
  endif(BUILD_SHARED_LIBS)
  
  get_source_file_property(PLGNOME2_COMPILE_PROPS plplotcanvas.c COMPILE_FLAGS)
  # Deal with NOTFOUND case.
  if(NOT PLGNOME2_COMPILE_PROPS)
    set(PLGNOME2_COMPILE_PROPS)
  endif(NOT PLGNOME2_COMPILE_PROPS)
  set_source_files_properties(
    ${plplotgnome2_LIB_SRCS}
    PROPERTIES COMPILE_FLAGS "${PLGNOME2_COMPILE_PROPS} ${gcw_COMPILE_FLAGS}"
    )

  # Set the include path
  include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    )

  # Create plplotgnome2[d] library
  add_library(plplotgnome2 ${plplotgnome2_LIB_SRCS})

  target_link_libraries(
    plplotgnome2
    plplot
    ${gcw_LINK_FLAGS}
    )

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot INSTALL_RPATH)
    set_target_properties(plplotgnome2
      PROPERTIES 
      SOVERSION ${plplotgnome2_SOVERSION} 
      VERSION ${plplotgnome2_VERSION}
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  else(USE_RPATH)
    set_target_properties(plplotgnome2
      PROPERTIES 
      SOVERSION ${plplotgnome2_SOVERSION} 
      VERSION ${plplotgnome2_VERSION}
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  endif(USE_RPATH)

  install(TARGETS plplotgnome2
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  # Configure pkg-config *.pc file corresponding to libplplotgnome2
  pkg_config_file("gnome2" "GNOME2" " GNOME2/GTK binding" "plplotgnome2" "${gcw_COMPILE_FLAGS}" "${gcw_LINK_FLAGS}")
endif(ENABLE_gnome2)
