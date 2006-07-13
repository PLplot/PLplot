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

if(NOT ENABLE_DYNDRIVERS)
  # Static device drivers must be included in the core PLplot library.
  foreach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
    message("SOURCE_ROOT_NAME = ${SOURCE_ROOT_NAME}")
    set(plplot${LIB_TAG}_LIB_SRCS 
       ${plplot${LIB_TAG}_LIB_SRCS} 
       ${CMAKE_SOURCE_DIR}/drivers/${SOURCE_ROOT_NAME}.c
       )
       if(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
         set_source_files_properties(
         ${CMAKE_SOURCE_DIR}/drivers/${SOURCE_ROOT_NAME}.c
         PROPERTIES COMPILE_FLAGS ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS})
	 set(libplplot${LIB_TAG}_COMPILE_FLAGS
	 ${libplplot${LIB_TAG}_COMPILE_FLAGS}
	 ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS}
	 )
       endif(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
  endforeach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
endif(NOT ENABLE_DYNDRIVERS)

include_directories(
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)
add_library(plplot${LIB_TAG} ${plplot${LIB_TAG}_LIB_SRCS})

if(NOT ENABLE_DYNDRIVERS)
  TARGET_LINK_LIBRARIES(plplot${LIB_TAG} ${DRIVERS_LINK_FLAGS})
  set(libplplot${LIB_TAG}_LINK_FLAGS
  ${libplplot${LIB_TAG}_LINK_FLAGS}
  ${DRIVERS_LINK_FLAGS}
  )
endif(NOT ENABLE_DYNDRIVERS)

set_target_properties(plplot${LIB_TAG}
PROPERTIES SOVERSION ${plplot_SOVERSION} VERSION ${plplot_VERSION}
)
install(TARGETS plplot${LIB_TAG} DESTINATION ${LIB_INSTALL_DIR})

string(REGEX REPLACE ";" " " 
libplplot${LIB_TAG}_LINK_FLAGS
"${libplplot${LIB_TAG}_LINK_FLAGS}"
)
message("libplplot${LIB_TAG}_LINK_FLAGS = ${libplplot${LIB_TAG}_LINK_FLAGS}")

string(REGEX REPLACE ";" " " 
libplplot${LIB_TAG}_COMPILE_FLAGS
"${libplplot${LIB_TAG}_COMPILE_FLAGS}"
)
message("libplplot${LIB_TAG}_COMPILE_FLAGS = ${libplplot${LIB_TAG}_COMPILE_FLAGS}")

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
  # PC_LINK_FLAGS	  - Used in *.pc Libs: field
  # PC_COMPILE_FLAGS	  - Used in *.pc Cflags: field
  set(PC_DATA
  "c++:C++:C++ bindings:${libplplot${LIB_TAG}_LINK_FLAGS}:${libplplot${LIB_TAG}_COMPILE_FLAGS}"
  )

  string(REGEX REPLACE "^(.*):.*:.*:.*:.*$" "\\1" BINDING ${PC_DATA})
  set(PC_FILE_SUFFIX "")
  set(PC_REQUIRES "")
  string(REGEX REPLACE "^.*:(.*):.*:.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:(.*):.*:.*$" "\\1" PC_LONG_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:.*:(.*):.*$" "\\1" PC_LINK_FLAGS ${PC_DATA})
  set(PC_LINK_FLAGS "-L${LIB_DIR} -lplplotd ${PC_LINK_FLAGS}")
  string(REGEX REPLACE "^.*:.*:.*:.*:(.*)$" "\\1" PC_COMPILE_FLAGS ${PC_DATA})
  set(PC_COMPILE_FLAGS "-I${INCLUDE_DIR} ${PC_COMPILE_FLAGS}")
  set(PC_CONFIGURED_FILE
  ${CMAKE_BINARY_DIR}/pkgcfg/plplot${LIB_TAG}${PC_FILE_SUFFIX}.pc
  )
  configure_file(
  ${CMAKE_SOURCE_DIR}/pkgcfg/plplot-template.pc.cmake
  ${PC_CONFIGURED_FILE}
  )
  install(FILES ${PC_CONFIGURED_FILE} DESTINATION ${PKG_CONFIG_DIR})
endif(PKGCONFIG_EXECUTABLE)

#original Makefile.am contents follow:

#
#lib_LTLIBRARIES = libplplot@LIB_TAG@.la
#
#version = -version-info $(SOVERSION)
#
#sources = \
#
#AM_CPPFLAGS = -I$(top_srcdir)/include $(FREETYPEINCCMD) $(INCLTDL) \
#  $(UNICODEINCS)
#
#libplplot@LIB_TAG@_la_SOURCES  = $(sources)
#libplplot@LIB_TAG@_la_LDFLAGS  = $(version) $(RPATH) -no-undefined \
#  $(FREETYPELIBCMD) $(CSALIBS) $(NNLIBS) -lm $(UNICODELIBS)
#if !enable_dyndrivers
#libplplot@LIB_TAG@_la_LIBADD = $(top_builddir)/drivers/libplplotdrv.la
#else
#libplplot@LIB_TAG@_la_LIBADD = $(LIBLTDL)
#endif
#
#
