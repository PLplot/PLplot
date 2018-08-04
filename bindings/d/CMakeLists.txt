# bindings/d/CMakeLists.txt
# Copyright (C) 2008 Werner Smekal
# Copyright (C) 2015-2018 Alan W. Irwin
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
  set(plplotdmd_LIB_SRCS
    plplot.d
  )

  if(plplotdmd_SHARED)
    if(USE_RPATH)
      get_target_property(LIB_INSTALL_RPATH PLPLOT::plplot INSTALL_RPATH)
    endif(USE_RPATH)
    configure_library_build(plplotdmd "" "${plplotdmd_LIB_SRCS}" PLPLOT::plplot)
  else(plplotdmd_SHARED)
    configure_library_build(plplotdmd STATIC "${plplotdmd_LIB_SRCS}" PLPLOT::plplot)
  endif(plplotdmd_SHARED)

  install(TARGETS ${WRITEABLE_TARGET}plplotdmd
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
  )

  # Configure pkg-config *.pc file corresponding to libplplotdmd
  pkg_config_file("d" "D" " D binding" "plplotdmd" "" "-lplplot")
endif(ENABLE_d)
