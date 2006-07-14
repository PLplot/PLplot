# bindings/f95/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
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

IF(ENABLE_f95)

# Set the include path
INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
)

# Build plflt to determine KIND for PLFLT
SET(plflt_SRC
plflt.c
)

ADD_EXECUTABLE(plflt ${plflt_SRC})

ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plflt.inc
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/plflt
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plflt
)

## Build C part of F95 bindings

SET(plplotf95c${LIB_TAG}_LIB_SRCS
plstubs.h
sc3d.c
sccont.c
scstubs.c
)

ADD_LIBRARY(plplotf95c${LIB_TAG} SHARED ${plplotf95c${LIB_TAG}_LIB_SRCS})

TARGET_LINK_LIBRARIES(plplotf95c${LIB_TAG}  plplot${LIB_TAG})

SET_TARGET_PROPERTIES(plplotf95c${LIB_TAG}
PROPERTIES SOVERSION ${plplotf95_SOVERSION} VERSION ${plplotf95_VERSION}
)

INSTALL(TARGETS plplotf95c${LIB_TAG} DESTINATION ${LIB_INSTALL_DIR})


## Build fortran part of F95 bindings
SET(plplotf95${LIB_TAG}_LIB_SRCS
strutil.f90 
configurable.f90 
sfstubsf95.f90 
sfstubs.h
)

# Explicitly include dependancies for sfstubsf95.f90
INCLUDE(AddFileDependencies)
ADD_FILE_DEPENDENCIES(sfstubsf95.f90 ${CMAKE_CURRENT_BINARY_DIR}/plflt.inc)

ADD_LIBRARY(plplotf95${LIB_TAG} SHARED ${plplotf95${LIB_TAG}_LIB_SRCS})

TARGET_LINK_LIBRARIES(plplotf95${LIB_TAG}  plplotf95${LIB_TAG})

SET_TARGET_PROPERTIES(plplotf95${LIB_TAG}
PROPERTIES SOVERSION ${plplotf95_SOVERSION} VERSION ${plplotf95_VERSION}
LINKER_LANGUAGE C
)

INSTALL(TARGETS plplotf95${LIB_TAG} DESTINATION ${LIB_INSTALL_DIR})

# Yuk! All Makefiles are run from the top level build directory and
# so the f90 .mod files end up there rather than in the bindings/f95
# directory. Ifort and pgf90 both have a -module command line option to 
# override this location, but I'm not sure how portable that?
INSTALL( 
FILES ${CMAKE_BINARY_DIR}/plplot.mod ${CMAKE_BINARY_DIR}/plplotp.mod ${CMAKE_BINARY_DIR}/plplot_flt.mod  
DESTINATION ${LIB_INSTALL_DIR}/fortran/modules/plplot
)

ENDIF(ENABLE_f95)




#original Makefile.am contents follow:

## bindings/f95/Makefile.am for PLplot
####
#### Process this file with automake to produce Makefile.in
####
## Copyright (C) 2002, 2003, 2004  Rafael Laboissiere
## Copyright (C) 2002, 2004  Alan W. Irwin
##
## This file is part of PLplot.
##
## PLplot is free software; you can redistribute it and/or modify it
## under the terms of the GNU Library General Public License as published by
## the Free Software Foundation; version 2 of the License.
##
## PLplot is distributed in the hope that it will be useful, but WITHOUT
## ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
## FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
## License for more details.
##
## You should have received a copy of the GNU Library General Public License
## along with the file PLplot; if not, write to the Free Software Foundation, Inc.,
## 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#
#libplplot = $(top_builddir)/src/libplplot$(LIB_TAG).la
#
#if enable_f95
## Location to install fortran 95  "module" files (which allows users of
## fortran libraries to access library-define modules with the "use" statement).
## This location picked using fairly generic advice from the LFHS mailing list.
#plplotmoduledir = $(libdir)/fortran/modules/plplot
#
## order is important here since libplplotf95c@LIB_TAG@.la must be
## created first (since libplplotf95@LIB_TAG@.la depends upon it).
#lib_LTLIBRARIES = libplplotf95c@LIB_TAG@.la libplplotf95@LIB_TAG@.la
#
#plplotmodule_DATA = \
#	plplot.mod \
#	plplotp.mod \
#	plplot_flt.mod
#
#endif
#
#AM_CPPFLAGS = -I$(top_srcdir)/include $(INCLTDL)
## -I. flag needed so that for separate build tree the fortran compiler
## can find the plflt.inc that is generated in the build tree.
#AM_FCFLAGS = -I$(srcdir) -I.
#
#version = -version-info $(F95_SOVERSION)
#
## Keep Fortran and C libraries completely separate so libplplot will know how
## to link them without relying on poorly known (for most platforms)
## FLIBS information detailing how to combine C and fortran source in the
## same library.
#
#fortran_sources = \
#	strutil.f90 \
#	configurable.f90 \
#	sfstubsf95.f90 \
#	sfstubs.h
#
#generated_fortran_sources = plflt.inc
#
#c_sources = \
#	plstubs.h \
#	sc3d.c \
#	sccont.c \
#	scstubs.c
#
## This creates the fortran source part of the fortran interface to libplplot.
## It wraps calls to libplplotf95c@LIB_TAG@.la
#libplplotf95@LIB_TAG@_la_SOURCES = $(fortran_sources)
#nodist_libplplotf95@LIB_TAG@_la_SOURCES = $(generated_fortran_sources)
#libplplotf95@LIB_TAG@_la_LDFLAGS  = $(version) $(RPATH) -no-undefined libplplotf95c@LIB_TAG@.la
#
## This creates the C source part of the fortran interface to libplplot.
## libplplotf95@LIB_TAG@.la wraps libplplotf95c@LIB_TAG@.la which wraps
## libplplot.  Thus, these two libraries together provide
## a fortran wrapper for the "common" API part of libplplot.
#libplplotf95c@LIB_TAG@_la_SOURCES  = $(c_sources)
#libplplotf95c@LIB_TAG@_la_LDFLAGS  = $(version) $(RPATH) -no-undefined $(libplplot)
#
## Must be explicit here with dependencies.
#sfstubsf95.lo: plflt.inc sfstubs.f90 sfstubs.h
#
#EXTRA_DIST = readme_f95.txt sfstubs.f90
#
#BUILT_SOURCES = $(generated_fortran_sources)
#CLEANFILES = $(generated_fortran_sources) $(plplotmodule_DATA)
#
#noinst_PROGRAMS = plflt
#plflt_SOURCES = plflt.c
#plflt_CFLAGS = -I../../include
#
#plflt.inc: plflt$(EXEEXT)
#	./plflt$(EXEEXT)
