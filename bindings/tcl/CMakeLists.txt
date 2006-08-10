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

set(include_INSTALLED_HEADERS
pltcl.h
tclMatrix.h
)

install(FILES ${include_INSTALLED_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR})

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

add_library(tclmatrix${LIB_TAG} ${tclmatrix${LIB_TAG}_LIB_SRCS})
target_link_libraries(tclmatrix${LIB_TAG} ${TCL_LIBRARY})

set(LIB_INSTALL_RPATH ${LIB_DIR})
set_target_properties(tclmatrix${LIB_TAG}
PROPERTIES
SOVERSION ${tclmatrix_SOVERSION}
VERSION ${tclmatrix_VERSION}
INSTALL_RPATH "${LIB_INSTALL_RPATH}"
)

install(TARGETS tclmatrix${LIB_TAG} DESTINATION ${LIB_INSTALL_DIR})

set(tclmatrix${LIB_TAG}_INSTALLED_HEADERS
tclMatrix.h
)

install(
FILES ${tclmatrix${LIB_TAG}_INSTALLED_HEADERS}
DESTINATION ${INCLUDE_INSTALL_DIR}
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
COMMAND ${PERL_EXECUTABLE}
ARGS ${CMAKE_CURRENT_SOURCE_DIR}/pltclgen ${CMAKE_CURRENT_SOURCE_DIR}
DEPENDS
${CMAKE_CURRENT_SOURCE_DIR}/pltclgen
${CMAKE_CURRENT_SOURCE_DIR}/plapi.tpl
${CMAKE_CURRENT_SOURCE_DIR}/tclcmd.tpl
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set_source_files_properties(
tclAPI.c
PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/tclgen.c
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
"-ltclmatrix${LIB_TAG} ${TCL_LIBRARY}"
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
  "${libplplottcltk${LIB_TAG}_LINK_FLAGS} ${ITCL_LIBRARY}"
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
  set(
  libplplottcltk${LIB_TAG}_COMPILE_FLAGS
  "${libplplottcltk${LIB_TAG}_COMPILE_FLAGS} -I${TK_INCLUDE_PATH}"
  )
  set(plplottcltk${LIB_TAG}_link_libraries
  ${plplottcltk${LIB_TAG}_link_libraries}
  ${TK_LIBRARY}
  )
  set(
  libplplottcltk${LIB_TAG}_LINK_FLAGS
  "${libplplottcltk${LIB_TAG}_LINK_FLAGS} ${TK_LIBRARY}"
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
    "${libplplottcltk${LIB_TAG}_LINK_FLAGS} ${ITK_LIBRARY}"
    )
  endif(ENABLE_itk)
endif(ENABLE_tk)

add_library(plplottcltk${LIB_TAG} ${plplottcltk${LIB_TAG}_LIB_SRCS})
target_link_libraries(
plplottcltk${LIB_TAG} 
${plplottcltk${LIB_TAG}_link_libraries}
)

if(ENABLE_DYNDRIVERS)
  set(LIB_INSTALL_RPATH ${LIB_INSTALL_RPATH} ${libplplot${LIB_TAG}_RPATH})
endif(ENABLE_DYNDRIVERS)

set_target_properties(plplottcltk${LIB_TAG}
PROPERTIES
SOVERSION ${plplottcltk_SOVERSION}
VERSION ${plplottcltk_VERSION}
INSTALL_RPATH "${LIB_INSTALL_RPATH}"
)

install(TARGETS plplottcltk${LIB_TAG} DESTINATION ${LIB_INSTALL_DIR})

set(plplottcltk${LIB_TAG}_INSTALLED_HEADERS
pltcl.h
)

install(
FILES ${plplottcltk${LIB_TAG}_INSTALLED_HEADERS}
DESTINATION ${INCLUDE_INSTALL_DIR}
)

# end plplottcltk

# Only install next two if the equivalent files are not going to
# be installed by the action of ../tk/CMakeLists.txt

if(NOT ENABLE_tk)
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

  install(
  FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot.tcl
  DESTINATION ${TCL_DIR}
  )

  add_custom_target(tclIndex_tcl ALL
  COMMAND ${MKTCLINDEX} ${MKTCLINDEX_ARGS}
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plplot.tcl
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

  install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/tclIndex
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
endif(PKGCONFIG_EXECUTABLE)
endif(ENABLE_tcl)
