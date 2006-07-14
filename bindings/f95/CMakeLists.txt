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

