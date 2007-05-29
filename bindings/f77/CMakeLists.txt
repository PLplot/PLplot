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

if(ENABLE_f77)

## Generate source files depending on type of PLFLT
## Note: Sed will possibly fail on windows. Would this be better
## done using the cmake configure_file option eventually?
if(PL_DOUBLE)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sfstubs.f
    COMMAND ${CMAKE_COMMAND}
    -E copy ${CMAKE_CURRENT_SOURCE_DIR}/sfstubs.fm4 ${CMAKE_CURRENT_BINARY_DIR}/sfstubs.f
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sfstubs.fm4
  )
else(PL_DOUBLE)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sfstubs.f
    COMMAND sed
    -f ${CMAKE_CURRENT_SOURCE_DIR}/double2single.sed ${CMAKE_CURRENT_SOURCE_DIR}/sfstubs.fm4a > ${CMAKE_CURRENT_BINARY_DIR}/sfstubs.f
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/sfstubs.fm4
  )
endif(PL_DOUBLE)

if(WIN32)
  # Set the flags for the C compiler. The C stubs need to have the
  # correction case and calling convention for the Fortran compiler
  IF(TARGET_FORTRAN MATCHES "IVF")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DIVF")
  ELSEIF(TARGET_FORTRAN MATCHES "CVF")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DCVF")
  ENDIF(TARGET_FORTRAN MATCHES "IVF")
endif(WIN32)

if(F77_CMD_LINE)
  set(HAVE_F77PARSE_CL_FALSE C)
else(F77_CMD_LINE)
  set(HAVE_F77PARSE_CL_TRUE C)
endif(F77_CMD_LINE)

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/configurable.f.cmake
${CMAKE_CURRENT_BINARY_DIR}/configurable.f
)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/configurable.f
PROPERTIES GENERATED ON)

# Set the include path
include_directories(
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)

## Build C part of F77 bindings
set(plplotf77c${LIB_TAG}_LIB_SRCS
sc3d.c
sccont.c
scstubs.c
)

if( WIN32 AND BUILD_SHARED_LIBS )
  SET_SOURCE_FILES_PROPERTIES( ${plplotf77c${LIB_TAG}_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DMAKINGPLDLL" )
endif( WIN32 AND BUILD_SHARED_LIBS )

add_library(plplotf77c${LIB_TAG} ${plplotf77c${LIB_TAG}_LIB_SRCS})

target_link_libraries(plplotf77c${LIB_TAG} plplot${LIB_TAG})

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
  set_target_properties(plplotf77c${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplotf77_SOVERSION}
  VERSION ${plplotf77_VERSION}
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
else(USE_RPATH)
  set_target_properties(plplotf77c${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplotf77_SOVERSION}
  VERSION ${plplotf77_VERSION}
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
endif(USE_RPATH)


install(TARGETS plplotf77c${LIB_TAG}
ARCHIVE DESTINATION ${LIB_DIR}
LIBRARY DESTINATION ${LIB_DIR}
RUNTIME DESTINATION ${BIN_DIR}
)

## Build fortran part of F77 bindings
set(plplotf77${LIB_TAG}_LIB_SRCS
strutil.f
sfstubs.f
configurable.f
)

add_library(plplotf77${LIB_TAG} ${plplotf77${LIB_TAG}_LIB_SRCS})

target_link_libraries(plplotf77${LIB_TAG}  plplotf77c${LIB_TAG})

if(USE_RPATH)
  set_target_properties(plplotf77${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplotf77_SOVERSION}
  VERSION ${plplotf77_VERSION}
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
else(USE_RPATH)
  set_target_properties(plplotf77${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplotf77_SOVERSION}
  VERSION ${plplotf77_VERSION}
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
endif(USE_RPATH)

install(TARGETS plplotf77${LIB_TAG}
ARCHIVE DESTINATION ${LIB_DIR}
LIBRARY DESTINATION ${LIB_DIR}
RUNTIME DESTINATION ${BIN_DIR}
)

# Configure pkg-config *.pc file corresponding to libplplotf77${LIB_TAG}
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
  set(PC_DATA "f77:F77:F77 bindings, :plplotf77${LIB_TAG}")

  string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
  set(PC_FILE_SUFFIX "-${BINDING}")
  set(PC_REQUIRES "plplot${LIB_TAG}")
  string(REGEX REPLACE "^.*:(.*):.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:(.*):.*$" "\\1" PC_LONG_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:.*:(.*)$" "\\1" PC_LIBRARY_NAME ${PC_DATA})
  set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
  set(PC_COMPILE_FLAGS "${lib${PC_LIBRARY_NAME}_COMPILE_FLAGS}")
  set(PC_LINK_FLAGS
  "-l${PC_LIBRARY_NAME} -lplplotf77c${LIB_TAG} ${PC_LINK_FLAGS}"
  )
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
endif(ENABLE_f77)
