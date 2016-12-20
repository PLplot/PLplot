# bindings/wxwidgets/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2007 Werner Smekal
# Copyright (C) 2015-2016 Alan W. Irwin
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

# ENABLE_wxwidgets is only true if ENABLE_cxx is also true.
if(ENABLE_wxwidgets)
  if(OLD_WXWIDGETS)
    set(plplotwxwidgets_LIB_SRCS
      deprecated_wxPLplotstream.cpp
      deprecated_wxPLplotwindow.cpp)
    # configure header file - we need to make sure, that the user of the bindings
    # can't set the wrong options (use freetype/agg)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/deprecated_wxPLplotstream.h.in
      ${CMAKE_CURRENT_BINARY_DIR}/deprecated_wxPLplotstream.h
      )
    set(plplotwxwidgets_INSTALLED_HEADERS
      ${CMAKE_CURRENT_BINARY_DIR}/deprecated_wxPLplotstream.h
      deprecated_wxPLplotwindow.h
      )
  else(OLD_WXWIDGETS)
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/wxPLplot_nanosec.h.in
      ${CMAKE_CURRENT_BINARY_DIR}/wxPLplot_nanosec.h
      )
    set(plplotwxwidgets_LIB_SRCS
      wxPLplotstream.cpp
      )
    if(PLPLOT_WX_DEBUG_OUTPUT AND PLPLOT_WX_NANOSEC)
      list(APPEND plplotwxwidgets_LIB_SRCS wxPLplot_nanosec.cpp)
    endif(PLPLOT_WX_DEBUG_OUTPUT AND PLPLOT_WX_NANOSEC)

    set(plplotwxwidgets_INSTALLED_HEADERS
      wxPLplotstream.h
      wxPLplotwindow.h
      ${CMAKE_CURRENT_BINARY_DIR}/wxPLplot_nanosec.h
      )
  endif(OLD_WXWIDGETS)
  # Set the include path
  include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_SOURCE_DIR}/bindings/c++
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    )

  # Create plplotwxwidgets library
  add_library(plplotwxwidgets ${plplotwxwidgets_LIB_SRCS})

  set_source_files_properties(${plplotwxwidgets_LIB_SRCS}
    PROPERTIES COMPILE_FLAGS "${wxwidgets_COMPILE_FLAGS}"
    )

  INCLUDE_DIRECTORIES( ${wxWidgets_INCLUDE_DIRS} )
  IF(wxWidgets_DEFINITIONS)
    SET_PROPERTY(DIRECTORY APPEND
      PROPERTY COMPILE_DEFINITIONS ${wxWidgets_DEFINITIONS})
  ENDIF(wxWidgets_DEFINITIONS)

  IF(wxWidgets_DEFINITIONS_DEBUG)
    SET_PROPERTY(DIRECTORY APPEND
      PROPERTY COMPILE_DEFINITIONS_DEBUG ${wxWidgets_DEFINITIONS_DEBUG})
  ENDIF(wxWidgets_DEFINITIONS_DEBUG)

  if(NON_TRANSITIVE)
    target_link_libraries(plplotwxwidgets PRIVATE plplot plplotcxx ${wxwidgets_LINK_FLAGS})
  else(NON_TRANSITIVE)
    target_link_libraries(plplotwxwidgets PUBLIC plplot plplotcxx ${wxwidgets_LINK_FLAGS})
  endif(NON_TRANSITIVE)

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot INSTALL_RPATH)
    list(APPEND LIB_INSTALL_RPATH ${wxwidgets_RPATH})
  endif(USE_RPATH)
  set_library_properties(plplotwxwidgets)

  # Install library in lib/
  install(TARGETS plplotwxwidgets
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  install(FILES
    ${plplotwxwidgets_INSTALLED_HEADERS}
    DESTINATION ${INCLUDE_DIR}
    )

  # Determine libplplotwxwidgets_COMPILE_FLAGS
  set(wxWidgets_DEFINITIONS_all "${wxWidgets_DEFINITIONS}")
  if(wxWidgets_DEFINITIONS_DEBUG)
    set(wxWidgets_DEFINITIONS_all "${wxWidgets_DEFINITIONS};${wxWidgets_DEFINITIONS_DEBUG}")
  endif(wxWidgets_DEFINITIONS_DEBUG)

  STRING(REGEX REPLACE ";" " -D"
    libplplotwxwidgets_COMPILE_FLAGS
    "-D${wxWidgets_DEFINITIONS_all}"
    )
  STRING(REGEX REPLACE ";" " -I"
    wxWidgets_IFLAGS
    "-I${wxWidgets_INCLUDE_DIRS}"
    )
  set(libplplotwxwidgets_COMPILE_FLAGS
    "${libplplotwxwidgets_COMPILE_FLAGS} ${wxWidgets_IFLAGS}"
    )

  # Configure pkg-config *.pc file corresponding to libplplotwxwidgets
  pkg_config_file("wxwidgets" "wxWidgets" " wxWidgets binding" "plplotwxwidgets" "${libplplotwxwidgets_COMPILE_FLAGS}" "-lplplot;-lplplotcxx;${wxwidgets_LINK_FLAGS}")

endif(ENABLE_wxwidgets)
