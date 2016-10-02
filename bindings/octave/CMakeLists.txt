# bindings/octave/CMakeLists.txt for PLplot
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

# Build and install octave interface to PLplot and octave PLplot examples.
if(ENABLE_octave)

  add_subdirectory(PLplot)
  add_subdirectory(misc)

  # Install renamed documentation files.
  set(docfiles
    BUGS
    FGA
    INSTALL
    README
    ToDo
    USAGE
    )
  foreach(doc ${docfiles})
    install(FILES ${doc} DESTINATION ${DOC_DIR} RENAME ${doc}.octave)
  endforeach(doc ${docfiles})

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/octave_version.pl.in
    ${CMAKE_CURRENT_BINARY_DIR}/octave_version.pl
    @ONLY
    )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/octaverc.in
    ${CMAKE_CURRENT_BINARY_DIR}/.octaverc
    @ONLY
    )

  # Build octave interface.
  add_custom_command(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_stub.m
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/plplot_stub_hand_crafted.m
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_stub.m
    DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/plplot_stub_hand_crafted.m
    )

  add_custom_target(
    plplot_stub.m_built ALL
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_stub.m
    )

  set(octave_interface_INCLUDE_PATHS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
    ${OCTAVE_INCLUDE_PATH}
    ${CMAKE_SOURCE_DIR}/bindings/swig-support
    )
  include_directories(${octave_interface_INCLUDE_PATHS})
  set(CMAKE_SWIG_FLAGS -DSWIG_OCTAVE)
  set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})

  set_source_files_properties(plplot_octave.i
    PROPERTIES
    SWIG_MODULE_NAME plplot_octave
    CPLUSPLUS ON
    )

  set(SWIG_MODULE_plplot_octave_EXTRA_DEPS
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/swig_documentation.i
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i
    )

  # Set up swig + c wrapper.
  swig_add_module(plplot_octave octave plplot_octave.i)
  swig_link_libraries(
    plplot_octave
    plplot
    "${OCTAVE_LIBRARIES}"
    "${OCTINTERP_LIBRARIES}"
    )

  # Make sure plplot_stub.m is copied to build tree before plplot_octave
  # is created so can use plplot_octave in normal way.
  add_dependencies(plplot_octave plplot_stub.m_built)

  # Need access elsewhere (examples/octave) to the file depends of
  # this custom target.
  set_property(GLOBAL PROPERTY
    FILES_plplot_stub.m_built
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_stub.m
    )

  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_stub.m
    DESTINATION ${PLPLOT_OCTAVE_DIR}
    )

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot INSTALL_RPATH)
    # (Reasonable) assumption here is that OCTAVE_LIBRARIES and
    # OCTINTERP_LIBRARIES have the same path.
    get_filename_component(OCTAVE_INSTALL_RPATH "${OCTAVE_LIBRARIES}" PATH)
    set(LIB_INSTALL_RPATH ${LIB_INSTALL_RPATH} ${OCTAVE_INSTALL_RPATH})
    set_target_properties(
      plplot_octave
      PROPERTIES
      PREFIX ""
      SUFFIX ".oct"
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      )
  else(USE_RPATH)
    set_target_properties(
      plplot_octave
      PROPERTIES
      PREFIX ""
      SUFFIX ".oct"
      INSTALL_NAME_DIR "${OCTAVE_OCT_DIR}"
      )
  endif(USE_RPATH)

  # Have to be specific about permissions for some reason (probably
  # oct suffix).  Use SO_PERMISSIONS to be consistent with permissions
  # used for other shared objects created by the add_library command.

  install(TARGETS plplot_octave
    EXPORT export_plplot
    LIBRARY
    DESTINATION ${OCTAVE_OCT_DIR}
    PERMISSIONS ${SO_PERMISSIONS}
    )

endif(ENABLE_octave)
