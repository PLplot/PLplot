# bindings/d/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2008 Werner Smekal
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
    add_library(plplotdmd ${plplotdmd_LIB_SRCS})

    if(USE_RPATH)
      get_target_property(LIB_INSTALL_RPATH plplot INSTALL_RPATH)
    endif(USE_RPATH)
    set_library_properties(plplotdmd)
  else(plplotdmd_SHARED)
    add_library(plplotdmd STATIC ${plplotdmd_LIB_SRCS})
  endif(plplotdmd_SHARED)

  if(NON_TRANSITIVE)
    target_link_libraries(plplotdmd PRIVATE plplot)
  else(NON_TRANSITIVE)
    target_link_libraries(plplotdmd PUBLIC plplot)
  endif(NON_TRANSITIVE)

  install(TARGETS plplotdmd
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
  )

  # Configure pkg-config *.pc file corresponding to libplplotdmd
  pkg_config_file("d" "D" " D binding" "plplotdmd" "" "-lplplot")
endif(ENABLE_d)
