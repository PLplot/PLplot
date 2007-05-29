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

if(ENABLE_f95)

# Set the include path
include_directories(
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_CURRENT_BINARY_DIR}
)

# Build plflt to determine KIND for PLFLT
set(plflt_SRC
plflt.c
)

add_executable(plflt ${plflt_SRC})

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plflt.inc
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/plflt
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plflt
)

set_source_files_properties(plflt.inc PROPERTIES GENERATED ON)

## Build C part of F95 bindings
set(plplotf95c${LIB_TAG}_LIB_SRCS
plstubs.h
sc3d.c
sccont.c
scstubs.c
)

add_library(plplotf95c${LIB_TAG} ${plplotf95c${LIB_TAG}_LIB_SRCS})

target_link_libraries(plplotf95c${LIB_TAG}  plplot${LIB_TAG})

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
  set_target_properties(plplotf95c${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplotf95_SOVERSION}
  VERSION ${plplotf95_VERSION}
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
else(USE_RPATH)
  set_target_properties(plplotf95c${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplotf95_SOVERSION}
  VERSION ${plplotf95_VERSION}
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
endif(USE_RPATH)

install(TARGETS plplotf95c${LIB_TAG}
ARCHIVE DESTINATION ${LIB_DIR}
LIBRARY DESTINATION ${LIB_DIR}
RUNTIME DESTINATION ${BIN_DIR}
)


## Build fortran part of F95 bindings
set(plplotf95${LIB_TAG}_LIB_SRCS
strutil.f90
configurable.f90
sfstubsf95.f90
sfstubs.h
)

if(WIN32 AND BUILD_SHARED_LIBS)
  SET(plplotf95${LIB_TAG}_LIB_SRCS ${plplotf95${LIB_TAG}_LIB_SRCS} plplotf95.def)
  SET_SOURCE_FILES_PROPERTIES(${plplotf95c${LIB_TAG}_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DMAKINGPLDLL" )
endif(WIN32 AND BUILD_SHARED_LIBS)

# Explicitly include dependencies for sfstubsf95.f90
include(AddFileDependencies)
add_file_dependencies(sfstubsf95.f90 ${CMAKE_CURRENT_BINARY_DIR}/plflt.inc)

add_library(plplotf95${LIB_TAG} ${plplotf95${LIB_TAG}_LIB_SRCS})

target_link_libraries(plplotf95${LIB_TAG}  plplotf95c${LIB_TAG})

if(USE_RPATH)
  set_target_properties(plplotf95${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplotf95_SOVERSION}
  VERSION ${plplotf95_VERSION}
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
else(USE_RPATH)
  set_target_properties(plplotf95${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplotf95_SOVERSION}
  VERSION ${plplotf95_VERSION}
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
endif(USE_RPATH)

install(TARGETS plplotf95${LIB_TAG}
ARCHIVE DESTINATION ${LIB_DIR}
LIBRARY DESTINATION ${LIB_DIR}
RUNTIME DESTINATION ${BIN_DIR}
)

# Yuk! All Makefiles are run from the top level build directory and
# so the f90 .mod files end up there rather than in the bindings/f95
# directory. Ifort and pgf90 both have a -module command line option to
# override this location, but I'm not sure how portable that?
install(
FILES ${CMAKE_BINARY_DIR}/plplot.mod ${CMAKE_BINARY_DIR}/plplotp.mod ${CMAKE_BINARY_DIR}/plplot_flt.mod
DESTINATION ${F95_MOD_DIR}
)

# Configure pkg-config *.pc file corresponding to libplplotf95${LIB_TAG}
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
  set(PC_DATA "f95:F95:F95 bindings, :plplotf95${LIB_TAG}")

  string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
  set(PC_FILE_SUFFIX "-${BINDING}")
  set(PC_REQUIRES "plplot${LIB_TAG}")
  string(REGEX REPLACE "^.*:(.*):.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:(.*):.*$" "\\1" PC_LONG_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:.*:(.*)$" "\\1" PC_LIBRARY_NAME ${PC_DATA})
  set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
  set(PC_COMPILE_FLAGS "${lib${PC_LIBRARY_NAME}_COMPILE_FLAGS}")
  set(PC_LINK_FLAGS
  "-l${PC_LIBRARY_NAME} -lplplotf95c${LIB_TAG} ${PC_LINK_FLAGS}"
  )
  set(PC_COMPILE_FLAGS "-I${F95_MOD_DIR} ${PC_COMPILE_FLAGS}")
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
endif(ENABLE_f95)
