# bindings/d/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2008 Werner Smekal
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
  set(plplotdmd${LIB_TAG}_LIB_SRCS
    plplot.d
  )
  # Set this option to OFF by default since the STATIC library is
  # the only one that seems to work for now on Debian Lenny and
  # potentially other Linux systems.
  option(plplotdmd_SHARED "Treat libplplotdmd as a shared library" OFF)
  
  if(plplotdmd_SHARED)
    add_library(plplotdmd${LIB_TAG} ${plplotdmd${LIB_TAG}_LIB_SRCS})

    if(BUILD_SHARED_LIBS)
      set_source_files_properties(${plplotdmd${LIB_TAG}_LIB_SRCS}
	PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
    endif(BUILD_SHARED_LIBS)

    target_link_libraries(plplotdmd${LIB_TAG} plplot${LIB_TAG})

    if(USE_RPATH)
      get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
      set_target_properties(plplotdmd${LIB_TAG}
	PROPERTIES
	SOVERSION ${plplotdmd_SOVERSION}
	VERSION ${plplotdmd_VERSION}
	INSTALL_RPATH "${LIB_INSTALL_RPATH}"
	INSTALL_NAME_DIR "${LIB_DIR}"
	)
    else(USE_RPATH)
      set_target_properties(plplotdmd${LIB_TAG}
	PROPERTIES
	SOVERSION ${plplotdmd_SOVERSION}
	VERSION ${plplotdmd_VERSION}
	INSTALL_NAME_DIR "${LIB_DIR}"
	)
    endif(USE_RPATH)
  else(plplotdmd_SHARED)
    add_library(plplotdmd${LIB_TAG} STATIC ${plplotdmd${LIB_TAG}_LIB_SRCS})
    target_link_libraries(plplotdmd${LIB_TAG} plplot${LIB_TAG})
  endif(plplotdmd_SHARED)

  install(TARGETS plplotdmd${LIB_TAG}
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
  )
  install(EXPORT export_plplot
    DESTINATION ${DATA_DIR}/examples/cmake/modules
    FILE export_plplotd.cmake
    )

  # Configure pkg-config *.pc file corresponding to libplplotdmd${LIB_TAG}
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
    set(PC_DATA "d:D:D bindings, :plplotdmd${LIB_TAG}")

    string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
    set(PC_FILE_SUFFIX "-${BINDING}")
    set(PC_REQUIRES "plplot${LIB_TAG}")
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
endif(ENABLE_d)
