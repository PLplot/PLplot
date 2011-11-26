# bindings/qt_gui/CMakeLists.txt
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

if(ENABLE_qt)
  # Set up Qt4-based build environment.
  include(${QT_USE_FILE})
  if(NOT QT_LIBRARIES)
    message(FATAL_ERROR "include(${QT_USE_FILE}) failed in bindings/qt_gui subdirectory.")
  endif(NOT QT_LIBRARIES)
  set(qt_SOURCE plqt.cpp)
  include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_SOURCE_DIR}/lib/nistcd
    ${CMAKE_SOURCE_DIR}/drivers
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    )
  if(qt_COMPILE_FLAGS)
    set_source_files_properties(
      ${qt_SOURCE}
      PROPERTIES COMPILE_FLAGS "${qt_COMPILE_FLAGS} -DUSINGDLL"
      )
    #message("qt_COMPILE_FLAGS = ${qt_COMPILE_FLAGS}")
  else(qt_COMPILE_FLAGS)
    set_source_files_properties(
      ${qt_SOURCE}
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL"
      )
  endif(qt_COMPILE_FLAGS)
  if(ANY_QT_DEVICE)
    add_library(plplotqt${LIB_TAG} ${qt_SOURCE})
    add_dependencies(plplotqt${LIB_TAG} moc_outfile_generated)
  else(ANY_QT_DEVICE)
    message(FATAL_ERROR "Internal build system inconsistency between ENABLE_qt true and ANY_QT_DEVICE false.")
  endif(ANY_QT_DEVICE)
  target_link_libraries(
    plplotqt${LIB_TAG}
    plplot${LIB_TAG}
    ${MATH_LIB}
    ${QT_LIBRARIES}
    )
  if(NON_TRANSITIVE)
    # empty list ==> non-transitive linking for everything that links to
    # libplplotqtd in the shared libraries case.
    target_link_libraries(plplotqt${LIB_TAG} LINK_INTERFACE_LIBRARIES)
    # This configures the pkg-config method to use non-transitive linking.
    set(PC_REQUIRES_TAG "Requires.private")
  else(NON_TRANSITIVE)
    # This configures the pkg-config method to use transitive linking
    set(PC_REQUIRES_TAG "Requires")
  endif(NON_TRANSITIVE)
  # qt_RPATH set in cmake/modules/qt.cmake. This is only used for the 
  # install-tree rpath since cmake handles the build-tree rpath
  # automatically (so long as full pathnames to libraries are used).
  #message("qt_RPATH = ${qt_RPATH}")
  if(USE_RPATH)
    # Order is important here because of /usr/lib concerns.
    set(LIB_INSTALL_RPATH 
      ${LIB_DIR}
      ${qt_RPATH}
      ${libplplot${LIB_TAG}_RPATH}
      )
    #message("LIB_INSTALL_RPATH = ${LIB_INSTALL_RPATH}")
    set_target_properties(plplotqt${LIB_TAG}
      PROPERTIES
      SOVERSION ${plplotqt_SOVERSION}
      VERSION ${plplotqt_VERSION}
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  else(USE_RPATH)
    set_target_properties(plplotqt${LIB_TAG}
      PROPERTIES
      SOVERSION ${plplotqt_SOVERSION}
      VERSION ${plplotqt_VERSION}
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  endif(USE_RPATH)
  install(TARGETS plplotqt${LIB_TAG}
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  # Transform "${QT_LIBRARIES}" to the standard pkg-config form.
  pkg_config_link_flags(
    libplplotqt${LIB_TAG}_LINK_FLAGS
    "${QT_LIBRARIES}"
    )

  # Configure pkg-config *.pc file corresponding to the compile and link
  # flags needed by a Qt GUI executable using extqt to gain access to PLplot.
  if(PKG_CONFIG_EXECUTABLE)
    if(LIB_TAG)
      set(PC_PRECISION "double")
    else(LIB_TAG)
      set(PC_PRECISION "single")
    endif(LIB_TAG)
    # Each list element must consist of a colon-separated string with the
    # following fields which are parsed out in the foreach loop below and
    # used to configure the corresponding pkg-config *.pc file.
    # BINDING 	      	  - ENABLE_${BINDING} keeps track of whether a
    # 			    binding has been enabled (ON) or not (OFF).
    #			    Also, ${BINDING} used to determine PC_FILE_SUFFIX
    #			    which helps to determine name of configured 
    #			    *.pc file.
    # PC_SHORT_NAME	  - Used in *.pc NAME: field
    # PC_LONG_NAME	  - Used in *.pc Description: field
    # PC_LIBRARY_NAME	  - Used in *.pc Libs: field
    #                         Also used to determine PC_LINK_FLAGS and
    #                         PC_COMPILE_FLAGS used in *.pc Libs: and Cflags:
    #			    fields.
    set(PC_DATA "qt:Qt:Qt4 bindings, :plplotqt${LIB_TAG}")

    string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
    set(PC_FILE_SUFFIX "-${BINDING}")
    set(PC_REQUIRES "plplot${LIB_TAG}")
    string(REGEX REPLACE "^.*:(.*):.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
    string(REGEX REPLACE "^.*:.*:(.*):.*$" "\\1" PC_LONG_NAME ${PC_DATA})
    string(REGEX REPLACE "^.*:.*:.*:(.*)$" "\\1" PC_LIBRARY_NAME ${PC_DATA})
    set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
    set(PC_COMPILE_FLAGS "${qt_COMPILE_FLAGS}")
    set(PC_LINK_FLAGS "-l${PC_LIBRARY_NAME} ${PC_LINK_FLAGS}")
    set(PC_CONFIGURED_FILE
      ${CMAKE_BINARY_DIR}/pkgcfg/plplot${LIB_TAG}${PC_FILE_SUFFIX}.pc
      )
    configure_file(
      ${CMAKE_SOURCE_DIR}/pkgcfg/plplot-template.pc.cmake
      ${PC_CONFIGURED_FILE}
      @ONLY
      )
    install(FILES ${PC_CONFIGURED_FILE} DESTINATION ${PKG_CONFIG_DIR})
  endif(PKG_CONFIG_EXECUTABLE)

  add_subdirectory(pyqt4)
endif(ENABLE_qt)
