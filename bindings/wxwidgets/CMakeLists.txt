# bindings/wxwidgets/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2007 Werner Smekal
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

if(ENABLE_wxwidgets)
  set(plplotwxwidgets${LIB_TAG}_LIB_SRCS
    wxPLplotstream.cpp
    wxPLplotwindow.cpp
  )

  # configure header file - we need to make sure, that the user of the bindings
  # can't set the wrong options (use freetype/agg)
  CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/wxPLplotstream.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/wxPLplotstream.h
  )

  set(plplotwxwidgets${LIB_TAG}_INSTALLED_HEADERS
    ${CMAKE_CURRENT_BINARY_DIR}/wxPLplotstream.h
    wxPLplotwindow.h
  )

  # Set the include path
  include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/bindings/c++
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
  )

  # Create plplotwxwidgets[d] library
  add_library(plplotwxwidgets${LIB_TAG} ${plplotwxwidgets${LIB_TAG}_LIB_SRCS})

  if(WIN32 AND BUILD_SHARED_LIBS)
    SET_SOURCE_FILES_PROPERTIES(${plplotwxwidgets${LIB_TAG}_LIB_SRCS}
        PROPERTIES COMPILE_FLAGS "-DMAKINGPLDLL" )
  endif(WIN32 AND BUILD_SHARED_LIBS)

  INCLUDE_DIRECTORIES( ${wxWidgets_INCLUDE_DIRS} )
  ADD_DEFINITIONS( ${wxWidgets_DEFINITIONS} )

  target_link_libraries(plplotwxwidgets${LIB_TAG} plplotcxx${LIB_TAG} ${wxWidgets_LIBRARIES})

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
    set_target_properties(plplotwxwidgets${LIB_TAG}
    PROPERTIES
    SOVERSION ${plplotwxwidgets_SOVERSION}
    VERSION ${plplotwxwidgets_VERSION}
    INSTALL_RPATH "${LIB_INSTALL_RPATH}"
    INSTALL_NAME_DIR "${LIB_DIR}"
    )
  else(USE_RPATH)
    set_target_properties(plplotwxwidgets${LIB_TAG}
    PROPERTIES
    SOVERSION ${plplotwxwidgets_SOVERSION}
    VERSION ${plplotwxwidgets_VERSION}
    INSTALL_NAME_DIR "${LIB_DIR}"
    )
  endif(USE_RPATH)

  # Install library in lib/
  install(TARGETS plplotwxwidgets${LIB_TAG}
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
  )

  install(FILES
    ${plplotwxwidgets${LIB_TAG}_INSTALLED_HEADERS}
    DESTINATION ${INCLUDE_DIR}
  )

  # Configure pkg-config *.pc file corresponding to libplplotwxwidgets${LIB_TAG}
  if(PKG_CONFIG_EXECUTABLE)
    if(LIB_TAG)
      set(PC_PRECISION "double")
    else(LIB_TAG)
      set(PC_PRECISION "single")
    endif(LIB_TAG)
    
    # Determine libplplotwxwidgets${LIB_TAG}_COMPILE_FLAGS
    STRING(REGEX REPLACE ";" " "
    libplplotwxwidgets${LIB_TAG}_COMPILE_FLAGS
    "${wxWidgets_DEFINITIONS}"
    )
    STRING(REGEX REPLACE ";" " -I"
    wxWidgets_IFLAGS
    "-I${wxWidgets_INCLUDE_DIRS}"
    )
    set(libplplotwxwidgets${LIB_TAG}_COMPILE_FLAGS
    "${libplplotwxwidgets${LIB_TAG}_COMPILE_FLAGS} ${wxWidgets_IFLAGS}"
    )
    
    # Determine libplplotwxwidgets${LIB_TAG}_LINK_FLAGS
    pc_transform_link_flags(
    libplplotwxwidgets${LIB_TAG}_LINK_FLAGS
    "${wxWidgets_LIBRARIES}"
    )

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
    set(PC_DATA "wxwidgets:wxWidgets:wxWidgets bindings, :plplotwxwidgets${LIB_TAG}")

    string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
    set(PC_FILE_SUFFIX "-${BINDING}")
    set(PC_REQUIRES "plplot${LIB_TAG}-c++")
    string(REGEX REPLACE "^.*:(.*):.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
    string(REGEX REPLACE "^.*:.*:(.*):.*$" "\\1" PC_LONG_NAME ${PC_DATA})
    string(REGEX REPLACE "^.*:.*:.*:(.*)$" "\\1" PC_LIBRARY_NAME ${PC_DATA})
    set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
    set(PC_COMPILE_FLAGS "${lib${PC_LIBRARY_NAME}_COMPILE_FLAGS}")
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
endif(ENABLE_wxwidgets)
