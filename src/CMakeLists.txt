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
mt19937ar.c
pltime.c
)
if(LTDL_WIN32)
  set(plplot${LIB_TAG}_LIB_SRCS ${plplot${LIB_TAG}_LIB_SRCS} ltdl_win32.c)
endif(LTDL_WIN32)

if(BUILD_SHARED_LIBS)
  SET_SOURCE_FILES_PROPERTIES(${plplot${LIB_TAG}_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
endif(BUILD_SHARED_LIBS)

if(ENABLE_DYNDRIVERS)
  get_source_file_property(PLCORE_COMPILE_PROPS plcore.c COMPILE_FLAGS)
  # Deal with NOTFOUND case.
  if(NOT PLCORE_COMPILE_PROPS)
    set(PLCORE_COMPILE_PROPS)
  endif(NOT PLCORE_COMPILE_PROPS)
  set_source_files_properties(
  plcore.c
  PROPERTIES COMPILE_FLAGS "${PLCORE_COMPILE_PROPS} -I${LTDL_INCLUDE_DIR}"
  )
else(ENABLE_DYNDRIVERS)
  if(ENABLE_tcl)
    set(
    tclgenfiles
    ${CMAKE_BINARY_DIR}/bindings/tcl/tclgen.h
    ${CMAKE_BINARY_DIR}/bindings/tcl/tclgen_s.h
    ${CMAKE_BINARY_DIR}/bindings/tcl/tclgen.c
    )

    # Only need to set up this special command if either the tk and tkwin
    # non-dynamic devices are enabled (see below) since at build time
    # this special command is only fired in that case.
    add_custom_command(
    OUTPUT ${tclgenfiles}
    COMMAND ${TCL_TCLSH}
    ${CMAKE_SOURCE_DIR}/bindings/tcl/pltclgen.tcl
    ${CMAKE_SOURCE_DIR}/bindings/tcl
    DEPENDS
    ${CMAKE_SOURCE_DIR}/bindings/tcl/pltclgen.tcl
    ${CMAKE_SOURCE_DIR}/bindings/tcl/plapi.tpl
    ${CMAKE_SOURCE_DIR}/bindings/tcl/tclcmd.tpl
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bindings/tcl
    )
  endif(ENABLE_tcl)

  # Static device drivers must be included in the core PLplot library.
  set(DRIVERS_RPATH)
  foreach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
    #message("${SOURCE_ROOT_NAME}_SOURCE = ${${SOURCE_ROOT_NAME}_SOURCE}")
    list(APPEND plplot${LIB_TAG}_LIB_SRCS ${${SOURCE_ROOT_NAME}_SOURCE})
    if(SOURCE_ROOT_NAME STREQUAL "qt")
      set(MOC_OPTIONS)
      if(PLD_bmpqt)
	list(APPEND MOC_OPTIONS -DPLD_bmpqt)
      endif(PLD_bmpqt)
      if(PLD_jpgqt)
	list(APPEND MOC_OPTIONS -DPLD_jpgqt)
      endif(PLD_jpgqt)
      if(PLD_pngqt)
	list(APPEND MOC_OPTIONS -DPLD_pngqt)
      endif(PLD_pngqt)
      if(PLD_ppmqt)
	list(APPEND MOC_OPTIONS -DPLD_ppmqt)
      endif(PLD_ppmqt)
      if(PLD_tiffqt)
	list(APPEND MOC_OPTIONS -DPLD_tiffqt)
      endif(PLD_tiffqt)
      if(PLD_svgqt)
	list(APPEND MOC_OPTIONS -DPLD_svgqt)
      endif(PLD_svgqt)
      if(PLD_epsqt)
	list(APPEND MOC_OPTIONS -DPLD_epsqt)
      endif(PLD_epsqt)
      if(PLD_pdfqt)
	list(APPEND MOC_OPTIONS -DPLD_pdfqt)
      endif(PLD_pdfqt)
      if(PLD_qtwidget)
	list(APPEND MOC_OPTIONS -DPLD_qtwidget)
      endif(PLD_qtwidget)
      if(PLD_extqt)
	list(APPEND MOC_OPTIONS -DPLD_extqt)
      endif(PLD_extqt)
      if(MOC_OPTIONS)
	qt4_wrap_cpp(
          QT_MOC_OUTFILES
          ${CMAKE_SOURCE_DIR}/drivers/qt.h
          OPTIONS ${MOC_OPTIONS}
          )
	# message("QT_MOC_OUTFILES = ${QT_MOC_OUTFILES}")
	set_source_files_properties(
          ${QT_MOC_OUTFILES}
          PROPERTIES GENERATED "ON"
          )
	list(APPEND plplot${LIB_TAG}_LIB_SRCS ${QT_MOC_OUTFILES})
      endif(MOC_OPTIONS)
    endif(SOURCE_ROOT_NAME STREQUAL "qt")

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
    if(${SOURCE_ROOT_NAME}_RPATH)
      set(DRIVERS_RPATH ${DRIVERS_RPATH} ${${SOURCE_ROOT_NAME}_RPATH})
    endif(${SOURCE_ROOT_NAME}_RPATH)
  endforeach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
endif(ENABLE_DYNDRIVERS)

include_directories(
${CMAKE_SOURCE_DIR}/include
${CMAKE_SOURCE_DIR}/lib/qsastime
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)
add_library(plplot${LIB_TAG} ${plplot${LIB_TAG}_LIB_SRCS})
add_dependencies(plplot${LIB_TAG} plhershey-unicode.h_built)

set(LIB_INSTALL_RPATH ${LIB_DIR})
if(ENABLE_DYNDRIVERS)
  set(libplplot${LIB_TAG}_LINK_LIBRARIES ${LTDL_LIBRARIES})
  #message("libplplot${LIB_TAG}_LINK_LIBRARIES =  ${libplplot${LIB_TAG}_LINK_LIBRARIES}")
  #message("libplplot${LIB_TAG}_RPATH = ${libplplot${LIB_TAG}_RPATH}")
  set(LIB_INSTALL_RPATH ${LIB_INSTALL_RPATH} ${libplplot${LIB_TAG}_RPATH})
else(ENABLE_DYNDRIVERS)
  set(libplplot${LIB_TAG}_LINK_LIBRARIES ${DRIVERS_LINK_FLAGS})
  set(LIB_INSTALL_RPATH ${LIB_INSTALL_RPATH} ${DRIVERS_RPATH})
endif(ENABLE_DYNDRIVERS)
#message("LIB_INSTALL_RPATH = ${LIB_INSTALL_RPATH}")

if(MATH_LIB)
  set(
  libplplot${LIB_TAG}_LINK_LIBRARIES
  ${libplplot${LIB_TAG}_LINK_LIBRARIES}
  ${MATH_LIB}
  )
endif(MATH_LIB)

# Transform "${libplplot${LIB_TAG}_LINK_LIBRARIES}" string to the 
# standard pkg-config form.
pkg_config_link_flags(
libplplot${LIB_TAG}_LINK_FLAGS
"${libplplot${LIB_TAG}_LINK_LIBRARIES}"
)

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
  include_directories(
    ${QHULL_INCLUDE_DIRS}
  )
endif(HAVE_QHULL)

if(NOT ENABLE_DYNDRIVERS AND PLD_cgm)
  list(APPEND  libplplot${LIB_TAG}_LINK_LIBRARIES nistcd)
  set(
  libplplot${LIB_TAG}_LINK_FLAGS
  "${libplplot${LIB_TAG}_LINK_FLAGS} -lnistcd"
  )
  include_directories(${CMAKE_SOURCE_DIR}/lib/nistcd)
endif(NOT ENABLE_DYNDRIVERS AND PLD_cgm)

set(
  libplplot${LIB_TAG}_LINK_LIBRARIES
  ${libplplot${LIB_TAG}_LINK_LIBRARIES}
  qsastime
  )
set(
  libplplot${LIB_TAG}_LINK_FLAGS
  "${libplplot${LIB_TAG}_LINK_FLAGS} -lqsastime"
  )

if(WITH_FREETYPE)
  get_source_file_property(PLFREETYPE_COMPILE_PROPS plfreetype.c COMPILE_FLAGS)
  # Deal with NOTFOUND case.
  if(NOT PLFREETYPE_COMPILE_PROPS)
    set(PLFREETYPE_COMPILE_PROPS)
  endif(NOT PLFREETYPE_COMPILE_PROPS)
  set_source_files_properties(plfreetype.c PROPERTIES COMPILE_FLAGS "${PLFREETYPE_COMPILE_PROPS} -I${FREETYPE_INCLUDE_DIR}")
  set(
  libplplot${LIB_TAG}_LINK_LIBRARIES
  ${libplplot${LIB_TAG}_LINK_LIBRARIES}
  ${FREETYPE_LIBRARIES}
  )
  
  # Convert to -L... -l... form.
  string(REGEX REPLACE "(/[^ ]*)/lib([^ ]*)\\.so" "-L\\1 -l\\2"
  _FREETYPE_LINK_FLAGS
  ${FREETYPE_LIBRARIES}
  )
  string(REGEX REPLACE "(/[^ ]*)/lib([^ ]*)\\.so" "-L\\1 -l\\2"
  FREETYPE_LINK_FLAGS
  ${_FREETYPE_LINK_FLAGS}
  )
  set(
  libplplot${LIB_TAG}_LINK_FLAGS
  "${libplplot${LIB_TAG}_LINK_FLAGS} ${FREETYPE_LINK_FLAGS}"
  )
endif(WITH_FREETYPE)

#message(STATUS
#"libplplot${LIB_TAG}_LINK_LIBRARIES = ${libplplot${LIB_TAG}_LINK_LIBRARIES}"
#)

target_link_libraries(
plplot${LIB_TAG}
${libplplot${LIB_TAG}_LINK_LIBRARIES}
)

if(USE_RPATH)
  set_target_properties(
  plplot${LIB_TAG}
  PROPERTIES 
  SOVERSION ${plplot_SOVERSION}
  VERSION ${plplot_VERSION}
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
else(USE_RPATH)
  set_target_properties(
  plplot${LIB_TAG}
  PROPERTIES 
  SOVERSION ${plplot_SOVERSION}
  VERSION ${plplot_VERSION}
  INSTALL_NAME_DIR "${LIB_DIR}"
  )
endif(USE_RPATH)

install(TARGETS plplot${LIB_TAG}
ARCHIVE DESTINATION ${LIB_DIR}
LIBRARY DESTINATION ${LIB_DIR}
RUNTIME DESTINATION ${BIN_DIR}
)

#message("libplplot${LIB_TAG}_LINK_FLAGS = ${libplplot${LIB_TAG}_LINK_FLAGS}")

string(REGEX REPLACE ";" " " 
libplplot${LIB_TAG}_COMPILE_FLAGS
"${libplplot${LIB_TAG}_COMPILE_FLAGS}"
)
#message("libplplot${LIB_TAG}_COMPILE_FLAGS = ${libplplot${LIB_TAG}_COMPILE_FLAGS}")

# Configure pkg-config *.pc file corresponding to libplplot${LIB_TAG}
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
  set(PC_DATA "c:::plplot${LIB_TAG}")

  string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
  set(PC_FILE_SUFFIX "")
  set(PC_REQUIRES "")
  string(REGEX REPLACE "^.*:(.*):.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:(.*):.*$" "\\1" PC_LONG_NAME ${PC_DATA})
  string(REGEX REPLACE "^.*:.*:.*:(.*)$" "\\1" PC_LIBRARY_NAME ${PC_DATA})
  set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
  set(PC_LINK_FLAGS "-l${PC_LIBRARY_NAME} ${PC_LINK_FLAGS}")
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
endif(PKG_CONFIG_EXECUTABLE)

