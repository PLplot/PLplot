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

SET(plplot${LIB_TAG}_LIB_SRCS
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
       set_source_files_properties(
       ${CMAKE_SOURCE_DIR}/drivers/${SOURCE_ROOT_NAME}.c
       PROPERTIES COMPILE_FLAGS -I${${SOURCE_ROOT_NAME}_INCLUDE_DIR})
  endforeach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
endif(NOT ENABLE_DYNDRIVERS)

INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)
ADD_LIBRARY(plplot${LIB_TAG} ${plplot${LIB_TAG}_LIB_SRCS})

if(NOT ENABLE_DYNDRIVERS)
  TARGET_LINK_LIBRARIES(plplot${LIB_TAG} ${DRIVERS_LIBRARIES})
endif(NOT ENABLE_DYNDRIVERS)

SET_TARGET_PROPERTIES(plplot${LIB_TAG}
PROPERTIES SOVERSION ${plplot_SOVERSION} VERSION ${plplot_VERSION}
)
install(TARGETS plplot${LIB_TAG} DESTINATION ${LIB_INSTALL_DIR})

########### install files ###############

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
