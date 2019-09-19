# bindings/d/CMakeLists.txt
# Copyright (C) 2006-2019 Alan W. Irwin
# Copyright (C) 2008-2009 Werner Smekal
# Copyright (C) 2011 Andrew Ross
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

  # plplotdmd depends on the plplot library.
  set(LIB_INSTALL_RPATH ${LIB_DIR})

  configure_library_build(plplotdmd "" "${plplotdmd_LIB_SRCS}" PLPLOT::plplot "${LIB_INSTALL_RPATH}")

  #debug_target_properties(no_target "COMPILE_DEFINITIONS;COMPILE_OPTIONS;INTERFACE_COMPILE_DEFINITIONS;INTERFACE_COMPILE_OPTIONS;LINK_LIBRARIES;LINK_OPTIONS;INTERFACE_LINK_LIBRARIES;INTERFACE_LINK_OPTIONS")

  # Careful! Threads::Threads is an interface library.  Therefore,
  # this call of debug_target_properties will error out unless the
  # properties list is a subset of the white-listed properties for
  # interface libraries.
  #debug_target_properties(Threads::Threads "INTERFACE_COMPILE_DEFINITIONS;INTERFACE_COMPILE_OPTIONS;INTERFACE_LINK_LIBRARIES;INTERFACE_LINK_OPTIONS")
  #debug_target_properties(Qt5::Core "COMPILE_DEFINITIONS;COMPILE_OPTIONS;INTERFACE_COMPILE_DEFINITIONS;INTERFACE_COMPILE_OPTIONS;LINK_LIBRARIES;LINK_OPTIONS;INTERFACE_LINK_LIBRARIES;INTERFACE_LINK_OPTIONS")
  #debug_target_properties(PLPLOT::plplot "COMPILE_DEFINITIONS;COMPILE_OPTIONS;INTERFACE_COMPILE_DEFINITIONS;INTERFACE_COMPILE_OPTIONS;LINK_LIBRARIES;LINK_OPTIONS;INTERFACE_LINK_LIBRARIES;INTERFACE_LINK_OPTIONS")
  #debug_target_properties(PLPLOT::plplotdmd "COMPILE_DEFINITIONS;COMPILE_OPTIONS;INTERFACE_COMPILE_DEFINITIONS;INTERFACE_COMPILE_OPTIONS;LINK_LIBRARIES;LINK_OPTIONS;INTERFACE_LINK_LIBRARIES;INTERFACE_LINK_OPTIONS")

  # Configure pkg-config *.pc file corresponding to libplplotdmd
  pkg_config_file("d" "D" " D binding" "plplotdmd" "" "-lplplot")

endif(ENABLE_d)
