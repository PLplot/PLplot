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
  set(plplot${LIB_TAG}_LIB_SRCS ${plplot${LIB_TAG}_LIB_SRCS} pldeprecated.c)
endif(PL_DEPRECATED)
if(LTDL_WIN32)
  set(plplot${LIB_TAG}_LIB_SRCS ${plplot${LIB_TAG}_LIB_SRCS} ltdl_win32.c)
endif(LTDL_WIN32)

if(BUILD_SHARED_LIBS)
  SET_SOURCE_FILES_PROPERTIES(${plplot${LIB_TAG}_LIB_SRCS}
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
    
    # qt device driver code within libplplot has some symbols that need to be
    # visible for qt_example in installed examples/c++ directory to link 
    # properly. 
    if(BUILD_SHARED_LIBS AND SOURCE_ROOT_NAME STREQUAL "qt")
      set(${SOURCE_ROOT_NAME}_COMPILE_FLAGS "${${SOURCE_ROOT_NAME}_COMPILE_FLAGS} -DUSINGDLL")
    endif(BUILD_SHARED_LIBS AND SOURCE_ROOT_NAME STREQUAL "qt")

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
add_dependencies(plplot${LIB_TAG} plhershey-unicode.h_built ${qt_dependency})

if(NOT ENABLE_DYNDRIVERS AND ANY_QT_DEVICE AND PLPLOT_USE_QT5)
  # According to advice from Steve Kelly on the Cmake list, the Qt5
  # Gui component is a dependency of the Qt5 Svg component so is not
  # needed here, but I will leave it in since it is informative.
  qt5_use_modules(plplot${LIB_TAG} Svg Gui)
endif(NOT ENABLE_DYNDRIVERS AND ANY_QT_DEVICE AND PLPLOT_USE_QT5)

if(NON_TRANSITIVE)
  # empty list ==> non-transitive linking for everything that links to
  # libplplotd in the shared libraries case.
  target_link_libraries(plplot${LIB_TAG} LINK_INTERFACE_LIBRARIES)
  # This configures the pkg-config method to use non-transitive linking.
  set(PC_REQUIRES_TAG "Requires.private")
else(NON_TRANSITIVE)
  # This configures the pkg-config method to use transitive linking
  set(PC_REQUIRES_TAG "Requires")
endif(NON_TRANSITIVE)

# Deal with external libraries.
set(LIB_INSTALL_RPATH ${LIB_DIR})
set(libplplot${LIB_TAG}_LINK_LIBRARIES)
if(ENABLE_DYNDRIVERS)
  list(APPEND libplplot${LIB_TAG}_LINK_LIBRARIES ${LTDL_LIBRARIES})
  #message("libplplot${LIB_TAG}_LINK_LIBRARIES =  ${libplplot${LIB_TAG}_LINK_LIBRARIES}")
  #message("libplplot${LIB_TAG}_RPATH = ${libplplot${LIB_TAG}_RPATH}")
  list(APPEND LIB_INSTALL_RPATH ${libplplot${LIB_TAG}_RPATH})
else(ENABLE_DYNDRIVERS)
  list(APPEND libplplot${LIB_TAG}_LINK_LIBRARIES ${DRIVERS_LINK_FLAGS})
  list(APPEND LIB_INSTALL_RPATH ${DRIVERS_RPATH})
  if(ANY_QT_DEVICE)
    # Update the target COMPILE_DEFINITIONS and INCLUDE_DIRECTORIES
    set_qt_target_properties(plplot${LIB_TAG})
  endif(ANY_QT_DEVICE)
endif(ENABLE_DYNDRIVERS)
#message("DEBUG: LIB_INSTALL_RPATH = ${LIB_INSTALL_RPATH}")

if(MATH_LIB)
  list(APPEND libplplot${LIB_TAG}_LINK_LIBRARIES ${MATH_LIB})
endif(MATH_LIB)

# Transform "${libplplot${LIB_TAG}_LINK_LIBRARIES}" string to the 
# standard pkg-config form.

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

  list(APPEND libplplot${LIB_TAG}_LINK_LIBRARIES ${SHAPELIB_LIBRARIES})
  list(APPEND LIB_INSTALL_RPATH ${SHAPELIB_RPATH})

endif(HAVE_SHAPELIB)

if(WITH_FREETYPE)
  get_source_file_property(PLFREETYPE_COMPILE_PROPS plfreetype.c COMPILE_FLAGS)
  # Deal with NOTFOUND case.
  if(NOT PLFREETYPE_COMPILE_PROPS)
    set(PLFREETYPE_COMPILE_PROPS)
  endif(NOT PLFREETYPE_COMPILE_PROPS)
  set_source_files_properties(plfreetype.c PROPERTIES COMPILE_FLAGS "${PLFREETYPE_COMPILE_PROPS} ${FREETYPE_INCLUDE_CFLAGS}")
  list(APPEND libplplot${LIB_TAG}_LINK_LIBRARIES ${FREETYPE_LIBRARIES})
  
endif(WITH_FREETYPE)

set(libstdcxx_full_path)
if(ENABLE_cxx AND NOT BUILD_SHARED_LIBS)
  # For this special case where the plplot library is a static C++
  # library, pkg-config needs to add -L and -l options corresponding
  # to libstd++.so to the link flags it delivers.  Another alternative
  # is to have the plplot library link statically to libstd++.a, but
  # static linking is not well-supported in CMake, and I am attempting
  # to keep this fix limited to just the configuration of the
  # plplotd.pc pkg-config file.  N.B. the logic below for finding the
  # C++ library is extremely specific to GNU g++ (either Unix or MinGW
  # version), but it is good to get at least this compiler to work for
  # the special case of a static C++ plplot library.
  if(EXISTS "${CMAKE_CXX_COMPILER}")
    execute_process(
      COMMAND ${CMAKE_CXX_COMPILER} --verbose --version
      RESULT_VARIABLE CXX_rc
      ERROR_VARIABLE CXX_string
      ERROR_STRIP_TRAILING_WHITESPACE
      OUTPUT_QUIET
      )
  else(EXISTS "${CMAKE_CXX_COMPILER}")
    set(CXX_rc 2)
  endif(EXISTS "${CMAKE_CXX_COMPILER}")
  if(CXX_rc STREQUAL "0" AND CXX_string MATCHES "LIBRARY_PATH=")
    string(REGEX REPLACE "^.*(LIBRARY_PATH=.*)\nCOLLECT_GCC_OPTIONS.*$" "\\1"
      CXX_library_path_string "${CXX_string}"
      )
    #message(STATUS "CXX_library_path_string = ${CXX_library_path_string}")
    if(CXX_library_path_string MATCHES "^LIBRARY_PATH=")
      string(REGEX REPLACE "^LIBRARY_PATH=" "" CXX_library_path "${CXX_library_path_string}")
      # For both Linux and MinGW, CXX_library_path is a semicolon
      # delimited list which is identical to the form of a CMake list.
      # Furthermore, for the MinGW case, the drive letters are in the
      # colon form, but experiments show that is exactly the form
      # required by find_library for the MinGW case.  Net result: no
      # conversion necessary for either Linux or MinGW.
      find_library(libstdcxx_full_path NAMES stdc++ PATHS ${CXX_library_path})
      message(STATUS "libstdcxx_full_path = ${libstdcxx_full_path}")
    else(CXX_library_path_string MATCHES "^LIBRARY_PATH=")
      message(STATUS "WARNING: execute_process failed to obtain C++ library needed for pkg-config")
      message(STATUS "CXX_string = ${CXX_string}")
      message(STATUS "CXX_library_path_string = ${CXX_library_path_string}")
    endif(CXX_library_path_string MATCHES "^LIBRARY_PATH=")
  else(CXX_rc STREQUAL "0" AND CXX_string MATCHES "LIBRARY_PATH=")
    message(STATUS "WARNING: execute_process failed to obtain C++ library needed for pkg-config")
    message(STATUS "CMAKE_CXX_COMPILER = ${CMAKE_CXX_COMPILER}")
    message(STATUS "CXX_rc = ${CXX_rc}")
    message(STATUS "CXX_string = ${CXX_string}")
  endif(CXX_rc STREQUAL "0" AND CXX_string MATCHES "LIBRARY_PATH=")
  if(NOT libstdcxx_full_path)
    message(STATUS "WARNING: failed to find libstdc++ so pkg-config link flags will be incomplete") 
  endif(NOT libstdcxx_full_path)
endif(ENABLE_cxx AND NOT BUILD_SHARED_LIBS)

# Transform current information in libplplot${LIB_TAG}_LINK_LIBRARIES
# into pkg-config (libplplot${LIB_TAG}_LINK_FLAGS) form.  Both variables
# will have data added to them afterward due to the internal PLplot libraries
# which have a different pkg-config treatment than the external libraries
# dealt with here.

if(libstdcxx_full_path)
  pkg_config_link_flags(
    libplplot${LIB_TAG}_LINK_FLAGS
    "${libplplot${LIB_TAG}_LINK_LIBRARIES};${libstdcxx_full_path}"
  )
else(libstdcxx_full_path)
  pkg_config_link_flags(
    libplplot${LIB_TAG}_LINK_FLAGS
    "${libplplot${LIB_TAG}_LINK_LIBRARIES}"
  )
endif(libstdcxx_full_path)

# The above completes dealing with the external libraries.  Now
# deal with the internal libraries that are built by the PLplot
# build system.

if(WITH_CSA)
  list(APPEND libplplot${LIB_TAG}_LINK_LIBRARIES csirocsa)
  set(
    libplplot${LIB_TAG}_LINK_FLAGS
    "${libplplot${LIB_TAG}_LINK_FLAGS} -lcsirocsa"
    )
endif(WITH_CSA)

if(PL_HAVE_QHULL)
  list(APPEND libplplot${LIB_TAG}_LINK_LIBRARIES csironn)
  if(QHULL_RPATH)
    set(
      libplplot${LIB_TAG}_LINK_FLAGS
      "${libplplot${LIB_TAG}_LINK_FLAGS} -lcsironn -L${QHULL_RPATH} -lqhull"
      )
  else(QHULL_RPATH)
    set(
      libplplot${LIB_TAG}_LINK_FLAGS
      "${libplplot${LIB_TAG}_LINK_FLAGS} -lcsironn -lqhull"
      )
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
  list(APPEND libplplot${LIB_TAG}_LINK_LIBRARIES nistcd)
  set(
    libplplot${LIB_TAG}_LINK_FLAGS
    "${libplplot${LIB_TAG}_LINK_FLAGS} -lnistcd"
    )
  include_directories(${CMAKE_SOURCE_DIR}/lib/nistcd)
endif(NOT ENABLE_DYNDRIVERS AND PLD_cgm)

list(APPEND libplplot${LIB_TAG}_LINK_LIBRARIES qsastime)
set(
  libplplot${LIB_TAG}_LINK_FLAGS
  "${libplplot${LIB_TAG}_LINK_FLAGS} -lqsastime"
  )

#message(STATUS
#"libplplot${LIB_TAG}_LINK_LIBRARIES = ${libplplot${LIB_TAG}_LINK_LIBRARIES}"
#)

target_link_libraries(
  plplot${LIB_TAG}
  ${libplplot${LIB_TAG}_LINK_LIBRARIES}
  )

if(USE_RPATH)
  filter_rpath(LIB_INSTALL_RPATH)
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
  EXPORT export_plplot
  ARCHIVE DESTINATION ${LIB_DIR}
  LIBRARY DESTINATION ${LIB_DIR}
  RUNTIME DESTINATION ${BIN_DIR}
  )

# This exports information for every target in the whole build that
# has the EXPORT export_plplot signature (as above) for the 
# install(TARGETS ... # command.
install(EXPORT export_plplot DESTINATION ${DATA_DIR}/examples/cmake/modules)

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
    ${CMAKE_SOURCE_DIR}/pkgcfg/plplot-template.pc.in
    ${PC_CONFIGURED_FILE}
    @ONLY
    )
  install(FILES ${PC_CONFIGURED_FILE} DESTINATION ${PKG_CONFIG_DIR})
endif(PKG_CONFIG_EXECUTABLE)
