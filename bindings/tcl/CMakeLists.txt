# bindings/tcl/CMakeLists.txt
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

if(ENABLE_tcl)

# tclmatrix
set(tclmatrix${LIB_TAG}_LIB_SRCS
tclMatrix.c
matrixInit.c
)

include_directories(
${TCL_INCLUDE_PATH}
${CMAKE_SOURCE_DIR}/include
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)

set(
libplplottcltk${LIB_TAG}_COMPILE_FLAGS
"-I${TCL_INCLUDE_PATH}"
)

if(WIN32 AND BUILD_SHARED_LIBS)
  SET(libvar "tclmatrix${LIB_TAG}_LIB_SRCS")
  SET_SOURCE_FILES_PROPERTIES(${${libvar}}
      PROPERTIES COMPILE_FLAGS "-DMAKINGPLDLL" )
  include_directories(
      ${CMAKE_SOURCE_DIR}/include
  )
endif(WIN32 AND BUILD_SHARED_LIBS)

add_library(tclmatrix${LIB_TAG} ${tclmatrix${LIB_TAG}_LIB_SRCS})
target_link_libraries(tclmatrix${LIB_TAG} ${TCL_LIBRARY})

if(USE_RPATH)
  set(LIB_INSTALL_RPATH ${LIB_DIR})
  set_target_properties(tclmatrix${LIB_TAG}
  PROPERTIES
  SOVERSION ${tclmatrix_SOVERSION}
  VERSION ${tclmatrix_VERSION}
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
else(USE_RPATH)
  set_target_properties(tclmatrix${LIB_TAG}
  PROPERTIES
  SOVERSION ${tclmatrix_SOVERSION}
  VERSION ${tclmatrix_VERSION}
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
endif(USE_RPATH)

install(TARGETS tclmatrix${LIB_TAG}
ARCHIVE DESTINATION ${LIB_DIR}
LIBRARY DESTINATION ${LIB_DIR}
RUNTIME DESTINATION ${BIN_DIR}
)

set(tclmatrix${LIB_TAG}_INSTALLED_HEADERS
tclMatrix.h
)

install(
FILES ${tclmatrix${LIB_TAG}_INSTALLED_HEADERS}
DESTINATION ${INCLUDE_DIR}
)
# end tclmatrix

# plplottcltk
set(
tclgenfiles
${CMAKE_CURRENT_BINARY_DIR}/tclgen.h
${CMAKE_CURRENT_BINARY_DIR}/tclgen_s.h
${CMAKE_CURRENT_BINARY_DIR}/tclgen.c
)

add_custom_command(
OUTPUT ${tclgenfiles}
COMMAND ${TCL_TCLSH}
${CMAKE_CURRENT_SOURCE_DIR}/pltclgen.tcl ${CMAKE_CURRENT_SOURCE_DIR}
DEPENDS
${CMAKE_CURRENT_SOURCE_DIR}/pltclgen.tcl
${CMAKE_CURRENT_SOURCE_DIR}/plapi.tpl
${CMAKE_CURRENT_SOURCE_DIR}/tclcmd.tpl
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set_source_files_properties(
tclAPI.c
PROPERTIES OBJECT_DEPENDS "${tclgenfiles}"
)

set(plplottcltk${LIB_TAG}_LIB_SRCS
tclAPI.c
tclMain.c
)

set(
plplottcltk${LIB_TAG}_link_libraries
tclmatrix${LIB_TAG}
plplot${LIB_TAG}
${TCL_LIBRARY}
)
set(
libplplottcltk${LIB_TAG}_LINK_FLAGS
-ltclmatrix${LIB_TAG} 
${TCL_LIBRARY}
)

if(ENABLE_itcl)
  include_directories(${ITCL_INCLUDE_PATH})
  set(
  libplplottcltk${LIB_TAG}_COMPILE_FLAGS
  "${libplplottcltk${LIB_TAG}_COMPILE_FLAGS} -I${ITCL_INCLUDE_PATH}"
  )
  set(plplottcltk${LIB_TAG}_link_libraries
  ${plplottcltk${LIB_TAG}_link_libraries}
  ${ITCL_LIBRARY}
  )
  set(
  libplplottcltk${LIB_TAG}_LINK_FLAGS
  ${libplplottcltk${LIB_TAG}_LINK_FLAGS}
  ${ITCL_LIBRARY}
  )
endif(ENABLE_itcl)

if(ENABLE_tk)
  set(plplottcltk${LIB_TAG}_LIB_SRCS
  ${plplottcltk${LIB_TAG}_LIB_SRCS}
  ${CMAKE_SOURCE_DIR}/bindings/tk/Pltk_Init.c
  ${CMAKE_SOURCE_DIR}/bindings/tk/plframe.c
  ${CMAKE_SOURCE_DIR}/bindings/tk/plr.c
  ${CMAKE_SOURCE_DIR}/bindings/tk/tcpip.c
  ${CMAKE_SOURCE_DIR}/bindings/tk/tkMain.c
  )
  include_directories(${TK_INCLUDE_PATH})
  # Transform TK_INCLUDE_PATH (which is a list) to blank-delimited flag form.
  string(REGEX REPLACE ";" " -I" TKLIB_COMPILE_FLAGS "-I${TK_INCLUDE_PATH}")
  set(
  libplplottcltk${LIB_TAG}_COMPILE_FLAGS
  "${libplplottcltk${LIB_TAG}_COMPILE_FLAGS} ${TKLIB_COMPILE_FLAGS}"
  )
  set(plplottcltk${LIB_TAG}_link_libraries
  ${plplottcltk${LIB_TAG}_link_libraries}
  ${TK_LIBRARY}
  )
  set(
  libplplottcltk${LIB_TAG}_LINK_FLAGS
  ${libplplottcltk${LIB_TAG}_LINK_FLAGS}
  ${TK_LIBRARY}
  )
  if(ENABLE_itk)
    include_directories(${ITK_INCLUDE_PATH})
    set(
    libplplottcltk${LIB_TAG}_COMPILE_FLAGS
    "${libplplottcltk${LIB_TAG}_COMPILE_FLAGS} -I${ITK_INCLUDE_PATH}"
    )
    set(plplottcltk${LIB_TAG}_link_libraries
    ${plplottcltk${LIB_TAG}_link_libraries}
    ${ITK_LIBRARY}
    )
    set(
    libplplottcltk${LIB_TAG}_LINK_FLAGS
    ${libplplottcltk${LIB_TAG}_LINK_FLAGS}
    ${ITK_LIBRARY}
    )
  endif(ENABLE_itk)
endif(ENABLE_tk)

# Transform "${libplplottcltk${LIB_TAG}_LINK_FLAGS}" string to the 
# standard pkg-config form.
pc_transform_link_flags(
libplplottcltk${LIB_TAG}_LINK_FLAGS
"${libplplottcltk${LIB_TAG}_LINK_FLAGS}"
)

add_library(plplottcltk${LIB_TAG} ${plplottcltk${LIB_TAG}_LIB_SRCS})

#message(STATUS 
#"plplottcltk${LIB_TAG}_link_libraries = 
#${plplottcltk${LIB_TAG}_link_libraries}"
#)
target_link_libraries(
plplottcltk${LIB_TAG}
${plplottcltk${LIB_TAG}_link_libraries}
)

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
  set_target_properties(plplottcltk${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplottcltk_SOVERSION}
  VERSION ${plplottcltk_VERSION}
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
else(USE_RPATH)
  set_target_properties(plplottcltk${LIB_TAG}
  PROPERTIES
  SOVERSION ${plplottcltk_SOVERSION}
  VERSION ${plplottcltk_VERSION}
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
endif(USE_RPATH)

install(TARGETS plplottcltk${LIB_TAG}
ARCHIVE DESTINATION ${LIB_DIR}
LIBRARY DESTINATION ${LIB_DIR}
RUNTIME DESTINATION ${BIN_DIR}
)

set(plplottcltk${LIB_TAG}_INSTALLED_HEADERS
pltcl.h
)

install(
FILES ${plplottcltk${LIB_TAG}_INSTALLED_HEADERS}
DESTINATION ${INCLUDE_DIR}
)

# end plplottcltk

# Generate plplot.tcl and tclIndex in build tree.  At least the first one
# is always needed for build-tree tests of tcl examples.

# _target suffix to avoid nameclash with filename plplot.tcl.  This
# nameclash screwed up ../tk/configuration which had a non-generated file
# of the same name which was a dependency of a custom target.
add_custom_target(plplot.tcl_target ALL
COMMAND ${CMAKE_COMMAND}
-E copy
${CMAKE_SOURCE_DIR}/bindings/tk/plplot.tcl
plplot.tcl
DEPENDS
${CMAKE_SOURCE_DIR}/bindings/tk/plplot.tcl
)

add_custom_target(tclIndex_tcl ALL
COMMAND ${TCL_TCLSH} ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
add_dependencies(tclIndex_tcl plplot.tcl_target)

if(NOT ENABLE_tk)
  # Only install these two files if the equivalent files are not going to
  # be installed by the action of ../tk/CMakeLists.txt

  install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot.tcl
  DESTINATION ${TCL_DIR}
  )

  install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/tclIndex
  DESTINATION ${TCL_DIR}
  )
endif(NOT ENABLE_tk)

set(LIB_HARDDIR ${LIB_DIR})

get_target_property(
DLNAME_LIBPLPLOTTCLTK
plplottcltk${LIB_TAG}
LOCATION
)
get_filename_component(
DLNAME_LIBPLPLOTTCLTK
${DLNAME_LIBPLPLOTTCLTK}
NAME
)
string(REPLACE "plplottcltkd" "plplottcltk"
DLNAME_LIBPLPLOTTCLTK
${DLNAME_LIBPLPLOTTCLTK}
)
string(REPLACE "plplottcltk" "plplottcltkd"
DLNAME_LIBPLPLOTTCLTKD
${DLNAME_LIBPLPLOTTCLTK}
)

configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/pkgIndex.tcl.in
${CMAKE_CURRENT_BINARY_DIR}/pkgIndex.tcl
@ONLY
)

# Configure pkg-config *.pc file corresponding to libplplottcltk${LIB_TAG}
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
  set(PC_DATA "tcl:Tcl/Tk:Tcl/Tk bindings, :plplottcltk${LIB_TAG}")

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
endif(ENABLE_tcl)
