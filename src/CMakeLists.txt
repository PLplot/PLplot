# src/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2015 Alan W. Irwin
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

set(plplot_LIB_SRCS
  pdfutils.c
  plmem.c
  plaffine.c
  plarc.c
  plargs.c
  plbox.c
  plcont.c
  plcore.c
  plctrl.c
  plcvt.c
  pldtik.c
  plf2ops.c
  plfill.c
  plfreetype.c
  plgradient.c
  plhist.c
  plimage.c
  plline.c
  plmap.c
  plmetafile.c
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
  pllegend.c
  )
if(PL_DEPRECATED)
  set(plplot_LIB_SRCS ${plplot_LIB_SRCS} pldeprecated.c)
endif(PL_DEPRECATED)
if(LTDL_WIN32)
  set(plplot_LIB_SRCS ${plplot_LIB_SRCS} ltdl_win32.c)
endif(LTDL_WIN32)

if(BUILD_SHARED_LIBS)
  set_source_files_properties(${plplot_LIB_SRCS}
    PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
endif(BUILD_SHARED_LIBS)

option(HERSHEY_FALLBACK "Hershey fallback for Unicode fonts" ON)

if(NOT HERSHEY_FALLBACK)
  get_source_file_property(PLSYM_COMPILE_PROPS plsym.c COMPILE_FLAGS)
  if(NOT PLSYM_COMPILE_PROPS)
    set(PLSYM_COMPILE_PROPS)
  endif(NOT PLSYM_COMPILE_PROPS)
  set_source_files_properties(
    plsym.c
    PROPERTIES COMPILE_FLAGS "${PLSYM_COMPILE_PROPS} -DPL_TEST_FOR_MISSING_GLYPHS"
    )
endif(NOT HERSHEY_FALLBACK)

set(libplplot_COMPILE_FLAGS)

get_source_file_property(PLCORE_COMPILE_PROPS plcore.c COMPILE_FLAGS)
# Deal with NOTFOUND case.
if(NOT PLCORE_COMPILE_PROPS)
  set(PLCORE_COMPILE_PROPS)
endif(NOT PLCORE_COMPILE_PROPS)
if(WIN32_OR_CYGWIN)
  set(PLCORE_COMPILE_PROPS "${PLCORE_COMPILE_PROPS} -DIGNORECASE")
endif(WIN32_OR_CYGWIN)

if(ENABLE_DYNDRIVERS)
  set(PLCORE_COMPILE_PROPS "${PLCORE_COMPILE_PROPS} -I${LTDL_INCLUDE_DIR}")
endif(ENABLE_DYNDRIVERS)

set_source_files_properties(
  plcore.c
  PROPERTIES COMPILE_FLAGS "${PLCORE_COMPILE_PROPS}"
  )

if(NOT ENABLE_DYNDRIVERS)
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
    list(APPEND plplot_LIB_SRCS ${${SOURCE_ROOT_NAME}_SOURCE})

    if(SOURCE_ROOT_NAME STREQUAL "qt")
      if(ANY_QT_DEVICE)
	# For this case libplot must have a target dependency on the
	# moc files generated in the binary include directory.
	set(qt_dependency moc_outfile_generated)

	# Qt5 handled a completely different way.
	if(NOT PLPLOT_USE_QT5)
	  if(NOT QT_LIBRARIES)
	    message(FATAL_ERROR "include(${QT_USE_FILE}) failed in src subdirectory.")
	  endif(NOT QT_LIBRARIES)
	  set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${QT_LIBRARIES})
	endif(NOT PLPLOT_USE_QT5)
	
      else(ANY_QT_DEVICE)
	message(FATAL_ERROR "Internal build system inconsistency.  Attempt to build static qt device when ANY_QT_DEVICE is false.")
      endif(ANY_QT_DEVICE)
    endif(SOURCE_ROOT_NAME STREQUAL "qt")

    # qt and tk or tkwin device driver code within libplplot has some
    # symbols that need to be visible for qt_example in examples/c++
    # and various Tcl/Tk PLplot applications to link properly.
    if(BUILD_SHARED_LIBS AND (SOURCE_ROOT_NAME STREQUAL "qt" OR SOURCE_ROOT_NAME STREQUAL "tk" OR SOURCE_ROOT_NAME STREQUAL "tkwin"))
      set(${SOURCE_ROOT_NAME}_COMPILE_FLAGS "${${SOURCE_ROOT_NAME}_COMPILE_FLAGS} -DUSINGDLL")
    endif(BUILD_SHARED_LIBS AND (SOURCE_ROOT_NAME STREQUAL "qt" OR SOURCE_ROOT_NAME STREQUAL "tk" OR SOURCE_ROOT_NAME STREQUAL "tkwin"))

    if(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
      set_source_files_properties(
	${${SOURCE_ROOT_NAME}_SOURCE}
	PROPERTIES COMPILE_FLAGS ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS}
	)
      list(APPEND libplplot_COMPILE_FLAGS ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS})
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
endif(NOT ENABLE_DYNDRIVERS)

include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/lib/qsastime
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/include
  )
add_library(plplot ${plplot_LIB_SRCS})
add_dependencies(plplot plhershey-unicode.h_built ${qt_dependency})

if(NOT ENABLE_DYNDRIVERS AND ANY_QT_DEVICE AND PLPLOT_USE_QT5)
  # According to advice from Steve Kelly on the Cmake list, the Qt5
  # Gui component is a dependency of the Qt5 Svg component so is not
  # needed here, but I will leave it in since it is informative.
  qt5_use_modules(plplot Svg Gui PrintSupport)
endif(NOT ENABLE_DYNDRIVERS AND ANY_QT_DEVICE AND PLPLOT_USE_QT5)


# Deal with external libraries.
set(LIB_INSTALL_RPATH ${LIB_DIR})
set(libplplot_LINK_LIBRARIES)
if(ENABLE_DYNDRIVERS)
  list(APPEND libplplot_LINK_LIBRARIES ${LTDL_LIBRARIES})
  #message("libplplot_LINK_LIBRARIES =  ${libplplot_LINK_LIBRARIES}")
  #message("libplplot_RPATH = ${libplplot_RPATH}")
  list(APPEND LIB_INSTALL_RPATH ${libplplot_RPATH})
else(ENABLE_DYNDRIVERS)
  list(APPEND libplplot_LINK_LIBRARIES ${DRIVERS_LINK_FLAGS})
  list(APPEND LIB_INSTALL_RPATH ${DRIVERS_RPATH})
  if(ANY_QT_DEVICE AND NOT PLPLOT_USE_QT5)
    # Update the target COMPILE_DEFINITIONS and INCLUDE_DIRECTORIES
    set_qt_target_properties(plplot)
  endif(ANY_QT_DEVICE AND NOT PLPLOT_USE_QT5)
endif(ENABLE_DYNDRIVERS)
#message("DEBUG: LIB_INSTALL_RPATH = ${LIB_INSTALL_RPATH}")

if(MATH_LIB)
  list(APPEND libplplot_LINK_LIBRARIES ${MATH_LIB})
endif(MATH_LIB)

if(HAVE_SHAPELIB)
  get_source_file_property(PLMAP_COMPILE_PROPS plmap.c COMPILE_FLAGS)
  # Deal with NOTFOUND case.
  if(NOT PLMAP_COMPILE_PROPS)
    set(PLMAP_COMPILE_PROPS)
  endif(NOT PLMAP_COMPILE_PROPS)
  set_source_files_properties(plmap.c
    PROPERTIES
    COMPILE_FLAGS "${PLMAP_COMPILE_PROPS} -I${SHAPELIB_INCLUDE_DIR}"
    )
  if(HAVE_SAHOOKS)
  set_source_files_properties(plmap.c
    PROPERTIES
    COMPILE_DEFINITIONS HAVE_SAHOOKS
    )
  endif(HAVE_SAHOOKS)

  list(APPEND libplplot_LINK_LIBRARIES ${SHAPELIB_LIBRARIES})
  list(APPEND LIB_INSTALL_RPATH ${SHAPELIB_RPATH})

endif(HAVE_SHAPELIB)

if(WITH_FREETYPE)
  get_source_file_property(PLFREETYPE_COMPILE_PROPS plfreetype.c COMPILE_FLAGS)
  # Deal with NOTFOUND case.
  if(NOT PLFREETYPE_COMPILE_PROPS)
    set(PLFREETYPE_COMPILE_PROPS)
  endif(NOT PLFREETYPE_COMPILE_PROPS)
  set_source_files_properties(plfreetype.c PROPERTIES COMPILE_FLAGS "${PLFREETYPE_COMPILE_PROPS} ${FREETYPE_INCLUDE_CFLAGS}")
  list(APPEND libplplot_LINK_LIBRARIES ${FREETYPE_LIBRARIES})
endif(WITH_FREETYPE)

# Copy current information in libplplot_LINK_LIBRARIES
# into libplplot_LINK_FLAGS, which contains the equivalent list information
# in a combination of library list form and pkg-config form that will
# eventually be transformted into pure pkg-config form.  Both variables
# will have data added to them afterward due to the internal PLplot libraries
# which have a different pkg-config treatment than the external libraries
# dealt with here.
set(libplplot_LINK_FLAGS ${libplplot_LINK_LIBRARIES})

# The above completes dealing with the external libraries.  Now
# deal with the internal libraries that are built by the PLplot
# build system.

if(WITH_CSA)
  list(APPEND libplplot_LINK_LIBRARIES csirocsa)
  list(APPEND libplplot_LINK_FLAGS -lcsirocsa)
endif(WITH_CSA)

if(PL_HAVE_QHULL)
  list(APPEND libplplot_LINK_LIBRARIES csironn)
  if(QHULL_RPATH)
    list(APPEND libplplot_LINK_FLAGS -lcsironn -L${QHULL_RPATH} -lqhull)
  else(QHULL_RPATH)
    list(APPEND libplplot_LINK_FLAGS -lcsironn -lqhull)
  endif(QHULL_RPATH)

  # Needed by plgridd.c.
  include_directories(
    ${QHULL_INCLUDE_DIRS}
    )
  # Needed by the traditional pkg-config approach for installed examples
  # as well as the new CMake-based build system for the installed examples.
  list(APPEND LIB_INSTALL_RPATH ${QHULL_RPATH})
endif(PL_HAVE_QHULL)

if(NOT ENABLE_DYNDRIVERS AND PLD_cgm)
  list(APPEND libplplot_LINK_LIBRARIES nistcd)
  list(APPEND libplplot_LINK_FLAGS -lnistcd)
  include_directories(${CMAKE_SOURCE_DIR}/lib/nistcd)
endif(NOT ENABLE_DYNDRIVERS AND PLD_cgm)

list(APPEND libplplot_LINK_LIBRARIES qsastime)
list(APPEND libplplot_LINK_FLAGS -lqsastime)

if(NOT ENABLE_DYNDRIVERS AND PLD_wxwidgets)
  list(APPEND libplplot_LINK_LIBRARIES ${RT_LIB})
endif(NOT ENABLE_DYNDRIVERS AND PLD_wxwidgets)

#message(STATUS "libplplot_LINK_LIBRARIES = ${libplplot_LINK_LIBRARIES}")

if(NON_TRANSITIVE)
  target_link_libraries(
    plplot PRIVATE
    ${libplplot_LINK_LIBRARIES}
    )
else(NON_TRANSITIVE)
  target_link_libraries(
    plplot PUBLIC
    ${libplplot_LINK_LIBRARIES}
    )
endif(NON_TRANSITIVE)

set_library_properties(plplot)

install(TARGETS plplot
  EXPORT export_plplot
  ARCHIVE DESTINATION ${LIB_DIR}
  LIBRARY DESTINATION ${LIB_DIR}
  RUNTIME DESTINATION ${BIN_DIR}
  )

# This exports information for every target in the whole build that
# has the EXPORT export_plplot signature (as above) for the
# install(TARGETS ... # command.
install(EXPORT export_plplot DESTINATION ${LIB_DIR}/cmake/plplot)

# Install associated CMake package config file.

install(FILES plplotConfig.cmake DESTINATION ${LIB_DIR}/cmake/plplot)

if(NOT ENABLE_DYNDRIVERS AND ANY_QT_DEVICE AND PLPLOT_USE_QT5)
  # For this special case QT_LIBRARIES is not populated so must
  # use this alternative method to add Qt(5) libraries to the pkg-config file
  # for the plplot library.
  list(APPEND libplplot_LINK_FLAGS ${Qt5_library_LINK_FLAGS})
endif(NOT ENABLE_DYNDRIVERS AND ANY_QT_DEVICE AND PLPLOT_USE_QT5)

message("libplplot_LINK_FLAGS = ${libplplot_LINK_FLAGS}")

string(REGEX REPLACE ";" " "
  libplplot_COMPILE_FLAGS
  "${libplplot_COMPILE_FLAGS}"
  )
#message("libplplot_COMPILE_FLAGS = ${libplplot_COMPILE_FLAGS}")

# Configure pkg-config *.pc file corresponding to libplplot
pkg_config_file("c" "" " Core C library" "plplot" "${libplplot_COMPILE_FLAGS}" "${libplplot_LINK_FLAGS};${cxx_compiler_library_pathname_list}")
