# bindings/f77/CMakeLists.txt
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

IF(ENABLE_f77)

## Generate source files depending on type of PLFLT
## Note: Sed will possibly fail on windows. Would this be better 
## done using the cmake configure_file option eventually?
IF(PL_DOUBLE)
  ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sfstubs.f
    COMMAND ${CMAKE_COMMAND}
    ARGS -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sfstubs.fm4 ${CMAKE_CURRENT_BINARY_DIR}/sfstubs.f
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sfstubs.fm4
  )
ELSE(PL_DOUBLE)
  ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sfstubs.f
    COMMAND sed
    ARGS -f ${CMAKE_CURRENT_SOURCE_DIR}/double2single.sed ${CMAKE_CURRENT_SOURCE_DIR}/sfstubs.fm4a > ${CMAKE_CURRENT_BINARY_DIR}/sfstubs.f
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sfstubs.fm4
  )
ENDIF(PL_DOUBLE)

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/configurable.f.cmake
${CMAKE_CURRENT_BINARY_DIR}/configurable.f
)

# Set the include path
INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)


## Build C part of F77 bindings

SET(plplotf77c${LIB_TAG}_LIB_SRCS
sc3d.c 
sccont.c 
scstubs.c
)

ADD_LIBRARY(plplotf77c${LIB_TAG} SHARED ${plplotf77c${LIB_TAG}_LIB_SRCS})

TARGET_LINK_LIBRARIES(plplotf77c${LIB_TAG}  plplot${LIB_TAG})

SET_TARGET_PROPERTIES(plplotf77c${LIB_TAG} 
PROPERTIES SOVERSION ${plplotf77_SOVERSION} VERSION ${plplotf77_VERSION}
)

INSTALL(TARGETS plplotf77c${LIB_TAG} DESTINATION ${LIB_INSTALL_DIR})


## Build fortran part of F77 bindings

SET(plplotf77${LIB_TAG}_LIB_SRCS
strutil.f 
sfstubs.f 
configurable.f
)

ADD_LIBRARY(plplotf77${LIB_TAG} SHARED ${plplotf77${LIB_TAG}_LIB_SRCS})

TARGET_LINK_LIBRARIES(plplotf77${LIB_TAG}  plplotf77c${LIB_TAG})

SET_TARGET_PROPERTIES(plplotf77${LIB_TAG}  
PROPERTIES SOVERSION ${plplotf77_SOVERSION} VERSION ${plplotf77_VERSION}
LINKER_LANGUAGE C
)

INSTALL(TARGETS plplotf77${LIB_TAG} DESTINATION ${LIB_INSTALL_DIR})

ENDIF(ENABLE_f77)
