# src/CMakeLists.txt for PLplot
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

set(plplot${LIB_TAG}_LIB_SRCS
pdfutils.c
plargs.c
plbox.c
plcont.c
plcore.c
plctrl.c
plcvt.c
pldeprecated.c
pldtik.c
plfill.c
plfreetype.c
plhist.c
plimage.c
plline.c
plmap.c
plot3d.c
plpage.c
plsdef.c
plshade.c
plstdio.c
plstripc.c
plsym.c
pltick.c
plvpor.c
plwind.c
plbuf.c
plgridd.c
plvect.c
)

if(ENABLE_DYNDRIVERS)
  set_source_files_properties(
  plcore.c
  PROPERTIES COMPILE_FLAGS "-I${LTDL_INCLUDE_DIR}"
  )
else(ENABLE_DYNDRIVERS)
  set(
  tclgenfiles
  ${CMAKE_BINARY_DIR}/bindings/tcl/tclgen.h
  ${CMAKE_BINARY_DIR}/bindings/tcl/tclgen_s.h
  ${CMAKE_BINARY_DIR}/bindings/tcl/tclgen.c
  )

  add_custom_command(
  OUTPUT ${tclgenfiles}
  COMMAND ${PERL_EXECUTABLE}
  ${CMAKE_SOURCE_DIR}/bindings/tcl/pltclgen
  ${CMAKE_SOURCE_DIR}/bindings/tcl
  DEPENDS
  ${CMAKE_SOURCE_DIR}/bindings/tcl/pltclgen
  ${CMAKE_SOURCE_DIR}/bindings/tcl/plapi.tpl
  ${CMAKE_SOURCE_DIR}/bindings/tcl/tclcmd.tpl
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bindings/tcl
  )

  # Static device drivers must be included in the core PLplot library.
  foreach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
    #message("${SOURCE_ROOT_NAME}_SOURCE = ${${SOURCE_ROOT_NAME}_SOURCE}")
    set(plplot${LIB_TAG}_LIB_SRCS 
    ${plplot${LIB_TAG}_LIB_SRCS} 
    ${${SOURCE_ROOT_NAME}_SOURCE}
    )
    if(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
      set_source_files_properties(
      ${${SOURCE_ROOT_NAME}_SOURCE}
      PROPERTIES COMPILE_FLAGS ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS}
      )
      set(libplplot${LIB_TAG}_COMPILE_FLAGS
      ${libplplot${LIB_TAG}_COMPILE_FLAGS}
      ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS}
      )
      if(SOURCE_ROOT_NAME STREQUAL "tk")
        set_source_files_properties(
	${${SOURCE_ROOT_NAME}_SOURCE}
	PROPERTIES OBJECT_DEPENDS ${tclgenfiles}
	)
      endif(SOURCE_ROOT_NAME STREQUAL "tk")
      if(SOURCE_ROOT_NAME STREQUAL "tkwin")
        set_source_files_properties(
	${${SOURCE_ROOT_NAME}_SOURCE}
	PROPERTIES OBJECT_DEPENDS ${tclgenfiles}
	)
      endif(SOURCE_ROOT_NAME STREQUAL "tkwin")
    endif(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
  endforeach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
endif(ENABLE_DYNDRIVERS)

include_directories(
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)
add_library(plplot${LIB_TAG} ${plplot${LIB_TAG}_LIB_SRCS})

set(LIB_INSTALL_RPATH ${LIB_DIR})
if(ENABLE_DYNDRIVERS)
  set(libplplot${LIB_TAG}_LINK_LIBRARIES ${LTDL_LIBRARIES})
  #message("libplplot${LIB_TAG}_LINK_LIBRARIES =  ${libplplot${LIB_TAG}_LINK_LIBRARIES}")
  #message("libplplot${LIB_TAG}_RPATH = ${libplplot${LIB_TAG}_RPATH}")
  set(LIB_INSTALL_RPATH ${LIB_INSTALL_RPATH} ${libplplot${LIB_TAG}_RPATH})
else(ENABLE_DYNDRIVERS)
  set(libplplot${LIB_TAG}_LINK_LIBRARIES ${DRIVERS_LINK_FLAGS})
endif(ENABLE_DYNDRIVERS)

string(REGEX REPLACE ";" " " 
libplplot${LIB_TAG}_LINK_FLAGS
"${libplplot${LIB_TAG}_LINK_LIBRARIES}"
)
if(MATH_LIB)
  set(
  libplplot${LIB_TAG}_LINK_LIBRARIES
  ${libplplot${LIB_TAG}_LINK_LIBRARIES}
  ${MATH_LIB}
  )
  set(
  libplplot${LIB_TAG}_LINK_FLAGS
  "${libplplot${LIB_TAG}_LINK_FLAGS} -l${MATH_LIB}"
  )
endif(MATH_LIB)

if(WITH_CSA)
  set(
  libplplot${LIB_TAG}_LINK_LIBRARIES
  ${libplplot${LIB_TAG}_LINK_LIBRARIES}
  csirocsa
  )
  set(
  libplplot${LIB_TAG}_LINK_FLAGS
  "${libplplot${LIB_TAG}_LINK_FLAGS} -lcsirocsa"
  )
endif(WITH_CSA)

if(HAVE_QHULL)
  set(
  libplplot${LIB_TAG}_LINK_LIBRARIES
  ${libplplot${LIB_TAG}_LINK_LIBRARIES}
  csironn
  )
  set(
  libplplot${LIB_TAG}_LINK_FLAGS
  "${libplplot${LIB_TAG}_LINK_FLAGS} -lcsironn -lqhull"
  )
endif(HAVE_QHULL)

if(WITH_FREETYPE)
  set_source_files_properties(plfreetype.c PROPERTIES COMPILE_FLAGS "-I${FREETYPE_INCLUDE_DIR}")
  set(
  libplplot${LIB_TAG}_LINK_LIBRARIES
  ${libplplot${LIB_TAG}_LINK_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  )
  set(
  libplplot${LIB_TAG}_LINK_FLAGS
  "${libplplot${LIB_TAG}_LINK_FLAGS} ${FREETYPE_LIBRARIES}"
  )
endif(WITH_FREETYPE)

target_link_libraries(
plplot${LIB_TAG}
${libplplot${LIB_TAG}_LINK_LIBRARIES}
)

set_target_properties(
plplot${LIB_TAG}
PROPERTIES 
SOVERSION ${plplot_SOVERSION}
VERSION ${plplot_VERSION}
INSTALL_RPATH "${LIB_INSTALL_RPATH}"
INSTALL_NAME_DIR "${LIB_DIR}"
)

install(TARGETS plplot${LIB_TAG} DESTINATION ${LIB_DIR})

#message("libplplot${LIB_TAG}_LINK_FLAGS = ${libplplot${LIB_TAG}_LINK_FLAGS}")

string(REGEX REPLACE ";" " " 
libplplot${LIB_TAG}_COMPILE_FLAGS
"${libplplot${LIB_TAG}_COMPILE_FLAGS}"
)
#message("libplplot${LIB_TAG}_COMPILE_FLAGS = ${libplplot${LIB_TAG}_COMPILE_FLAGS}")

# Configure pkg-config *.pc file corresponding to libplplot${LIB_TAG}
if(PKGCONFIG_EXECUTABLE)
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
  set(PC_DATA "c:::plplot${LIB_TAG}")

  string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
  set(PC_FILE_SUFFIX "")
  set(PC_REQUIRES "")
  string(REGEX REPLACE "^.*:(.*):.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:(.*):.*$" "\\1" PC_LONG_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:.*:(.*)$" "\\1" PC_LIBRARY_NAME ${PC_DATA})
  set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
  set(PC_LINK_FLAGS "-l${PC_LIBRARY_NAME} ${MATH_LIB_FLAG} ${PC_LINK_FLAGS}")
  set(PC_COMPILE_FLAGS "${lib${PC_LIBRARY_NAME}_COMPILE_FLAGS}")
  set(PC_CONFIGURED_FILE
  ${CMAKE_BINARY_DIR}/pkgcfg/plplot${LIB_TAG}${PC_FILE_SUFFIX}.pc
  )
  configure_file(
  ${CMAKE_SOURCE_DIR}/pkgcfg/plplot-template.pc.cmake
  ${PC_CONFIGURED_FILE}
  @ONLY
  )
  install(FILES ${PC_CONFIGURED_FILE} DESTINATION ${PKG_CONFIG_DIR})
endif(PKGCONFIG_EXECUTABLE)

