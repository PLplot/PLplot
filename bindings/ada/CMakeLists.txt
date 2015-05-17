# bindings/ada/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2007-2015 Alan W. Irwin
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
  
  # These are Ada library information files built by gnat.  I am not 
  # sure whether the name suffixes correspond to the *.adb or *.ads files
  # above or the union of them.  In any case, if any of the names change
  # above, then this list will probably have to be changed as well.)
  # N.B. the absolute location prefix of these files may have to be changed
  # in the future since this is a CMake internal.
  set(ALI_PREFIX 
    ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/plplotada.dir
    )
  set(ALI_LIST
    ${ALI_PREFIX}/plplot.ali
    ${ALI_PREFIX}/plplot_thin.ali
    ${ALI_PREFIX}/plplot_traditional.ali
    ${ALI_PREFIX}/plplot_auxiliary.ali
    )

  # I am pretty sure this is a workaround for a MinGW gnatmake bug
  # since the installation of these files is not needed for Linux.
  # However, without this workaround the standard examples build on
  # MinGW/MSYS platforms without problems (so gnatmake is happy if the
  # *.o files are not installed which is another indication we are
  # dealing with a bug here), but at run-time the ada examples
  # immediately return with a return code of 3.
  if(MINGW)
    list(APPEND ALI_LIST
      ${ALI_PREFIX}/plplot.o
      ${ALI_PREFIX}/plplot_thin.o
      ${ALI_PREFIX}/plplot_traditional.o
      ${ALI_PREFIX}/plplot_auxiliary.o
      )
  endif(MINGW)

  set(plplotada_LIB_SRCS)
  foreach(SOURCE_FILE ${SOURCE_LIST})
    list(APPEND plplotada_LIB_SRCS 
      ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}
      )
  endforeach(SOURCE_FILE ${SOURCE_LIST})

  add_library(plplotada ${plplotada_LIB_SRCS})

  # Work around an issue in our CMake Ada language
  # support for MinGW/Windows.  FIXME.  This issue should be fixed
  # at the Ada language support level and not worked around here.
  if(USE_DLL_SUBDIRECTORY)
    add_custom_command(
      TARGET plplotada POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different 
      libplplotada.dll libplplotada.dll.a
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/dll
      VERBATIM
      )
  endif(USE_DLL_SUBDIRECTORY)

  if(NON_TRANSITIVE)
    target_link_libraries(plplotada PRIVATE ${GNAT_LIB})
  else(NON_TRANSITIVE)
    target_link_libraries(plplotada PUBLIC ${GNAT_LIB})
  endif(NON_TRANSITIVE)

  # N.B. nm evidence shows that examples that use
  # callbacks (e.g., plfill in x15a.adb) have unresolved references
  # to c_plfill, etc. that require a public link to plplot
  # regardless of how NON_TRANSITIVE is set.
  target_link_libraries(plplotada PUBLIC plplot)

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot INSTALL_RPATH)
    set_target_properties(plplotada
      PROPERTIES
      SOVERSION ${plplotada_SOVERSION}
      VERSION ${plplotada_VERSION}
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  else(USE_RPATH)
    set_target_properties(plplotada
      PROPERTIES
      SOVERSION ${plplotada_SOVERSION}
      VERSION ${plplotada_VERSION}
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  endif(USE_RPATH)

  install(FILES ${plplotada_LIB_SRCS} 
    DESTINATION ${ADA_INCLUDE_DIR}
    )
  
  # Permissions of *.ali files in accordance with
  # http://people.debian.org/~lbrenta/debian-ada-policy.html
  install(FILES ${ALI_LIST}
    DESTINATION ${ADA_LIB_DIR}
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
    )
  
  install(TARGETS plplotada
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  # Add generated .ali files to the list of additional files to be
  # removed by make clean
  SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${ALI_LIST}")

  # Configure pkg-config *.pc file corresponding to libplplotada
  # See remarks above concerning why plplot has to be public.
  pkg_config_file("ada" "Ada" " Ada binding" "plplotada" "" "${GNAT_LIB}" "-lplplot")

endif(ENABLE_ada)
