# drivers/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006-2016 Alan W. Irwin
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

install(
  FILES README.drivers README.wxwidgets
  DESTINATION ${DOC_DIR}
  )

if(ENABLE_DYNDRIVERS)
  include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_SOURCE_DIR}/lib/nistcd
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    )

  option(TEST_DYNDRIVERS "Test dynamic drivers" ON)
  if(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir)

    set_source_files_properties(
      test-drv-info.c
      PROPERTIES COMPILE_FLAGS "-I${LTDL_INCLUDE_DIR}"
      )
    add_executable(test-drv-info test-drv-info.c)
    if(BUILD_SHARED_LIBS)
      set_target_properties(test-drv-info PROPERTIES
	COMPILE_DEFINITIONS "USINGDLL"
	)
    endif(BUILD_SHARED_LIBS)
    target_link_libraries(test-drv-info
      plplot
      ${LTDL_LIBRARIES}
      )

    # in windows we move test-drv-info to the dll directory
    # otherwise we use the RPATH functionality
    if(USE_DLL_SUBDIRECTORY)
      set_target_properties(
	test-drv-info
	PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/dll
	)
    else(USE_DLL_SUBDIRECTORY)
      set(test-drv-info_RPATH
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_BINARY_DIR}/src
        ${CMAKE_BINARY_DIR}/lib/csa
        ${CMAKE_BINARY_DIR}/lib/nn
        ${CMAKE_BINARY_DIR}/lib/qsastime
        ${libplplot_RPATH}
        )

      set_target_properties(
        test-drv-info
        PROPERTIES
        INSTALL_RPATH "${test-drv-info_RPATH}"
        BUILD_WITH_INSTALL_RPATH ON
        )
    endif(USE_DLL_SUBDIRECTORY)
    set(test_dyndrivers_TDEPENDS test-drv-info)

  endif(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)

  foreach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
    #message("${SOURCE_ROOT_NAME}_SOURCE = ${${SOURCE_ROOT_NAME}_SOURCE}")
    if(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
      set_source_files_properties(
	${${SOURCE_ROOT_NAME}_SOURCE}
	PROPERTIES COMPILE_FLAGS "${${SOURCE_ROOT_NAME}_COMPILE_FLAGS}"
	)
      #message("${SOURCE_ROOT_NAME}_COMPILE_FLAGS = ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS}")
    endif(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)

    # ${SOURCE_ROOT_NAME}_LINK_FLAGS is ideally a list of the full path names
    # to libraries determined with find_library.  However, the list can also
    # include link flags such as the -L and -l form of specifying libraries,
    # but that way of doing things only works on Unix, and even for Unix,
    # cmake does not correctly set the rpath for the build tree results
    # for non-standard locations with the -L and -l forms.
    #message("${SOURCE_ROOT_NAME}_LINK_FLAGS = ${${SOURCE_ROOT_NAME}_LINK_FLAGS}")
    # ${SOURCE_ROOT_NAME}_TARGETS is a list of PLplot CMake targets that the
    # device driver depends on.
    #message("${SOURCE_ROOT_NAME}_TARGETS = ${${SOURCE_ROOT_NAME}_TARGETS}")

    if(SOURCE_ROOT_NAME STREQUAL "qt")
      if(ANY_QT_DEVICE)
	add_library(${SOURCE_ROOT_NAME} MODULE ${${SOURCE_ROOT_NAME}_SOURCE})
	if(PLPLOT_USE_QT5)
	  # According to advice from Steve Kelly on the Cmake list, the Qt5
	  # Gui component is a dependency of the Qt5 Svg component so is not
	  # needed here, but I will leave it in since it is informative.
	  target_link_libraries(
	    ${SOURCE_ROOT_NAME}
	    plplot
	    ${MATH_LIB}
	    ${${SOURCE_ROOT_NAME}_TARGETS}
	    Qt5::Svg Qt5::Gui Qt5::PrintSupport
	    )
	else(PLPLOT_USE_QT5)
	  if(NOT QT_LIBRARIES)
	    message(FATAL_ERROR "Internal build system inconsistency. QT_LIBRARIESis empty but it should be populated")
	  endif(NOT QT_LIBRARIES)

	  target_link_libraries(
	    ${SOURCE_ROOT_NAME}
	    plplot
	    ${MATH_LIB}
	    ${QT_LIBRARIES}
	    ${${SOURCE_ROOT_NAME}_TARGETS}
	    )
	  # Update the target COMPILE_DEFINITIONS and INCLUDE_DIRECTORIES
	  set_qt4_target_properties(${SOURCE_ROOT_NAME})
	endif(PLPLOT_USE_QT5)
      else(ANY_QT_DEVICE)
	message(FATAL_ERROR "Internal build system inconsistency.  Attempt to build dynamic qt device when ANY_QT_DEVICE is false.")
      endif(ANY_QT_DEVICE)
    elseif(SOURCE_ROOT_NAME STREQUAL "wxwidgets")
      add_library(${SOURCE_ROOT_NAME} MODULE ${${SOURCE_ROOT_NAME}_SOURCE})
      target_link_libraries(
	${SOURCE_ROOT_NAME}
	plplot
	${MATH_LIB}
	${RT_LIB}
	${${SOURCE_ROOT_NAME}_LINK_FLAGS}
	${${SOURCE_ROOT_NAME}_TARGETS}
	)
      if(PLD_wxwidgets OR PLD_wxpng)
	set_property(TARGET ${SOURCE_ROOT_NAME}
	  APPEND PROPERTY
	  INCLUDE_DIRECTORIES ${CMAKE_BINARY_DIR}/bindings/wxwidgets
	  )
      endif(PLD_wxwidgets OR PLD_wxpng)

    else(SOURCE_ROOT_NAME STREQUAL "qt")
      add_library(${SOURCE_ROOT_NAME} MODULE ${${SOURCE_ROOT_NAME}_SOURCE})
      target_link_libraries(
	${SOURCE_ROOT_NAME}
	plplot
	${MATH_LIB}
	${${SOURCE_ROOT_NAME}_LINK_FLAGS}
	${${SOURCE_ROOT_NAME}_TARGETS}
	)
    endif(SOURCE_ROOT_NAME STREQUAL "qt")

    # ${SOURCE_ROOT_NAME}_RPATH originally set in cmake/modules files for
    # each driver in ${DRIVERS_LIST}.  This is only used for the
    # install-tree rpath since cmake handles the build-tree rpath
    # automatically (so long as full pathnames to libraries are used).
    # Order is important here because of /usr/lib concerns.
    set(
      ${SOURCE_ROOT_NAME}_RPATH
      ${LIB_DIR}
      ${${SOURCE_ROOT_NAME}_RPATH}
      ${libplplot_RPATH}
      )
    # Our implementation of dynamic loading using libltdl assumes
    # the prefix should always be nothing (rather than lib) and the suffix
    # should be set in a platform-dependent manner in order for libltdl
    # to find the dynamic device.
    if(WIN32_OR_CYGWIN)
      # strace showed that .dll was a must for libltdl on Cygwin.
      set(DYNAMIC_SUFFIX ".dll")
    else(WIN32_OR_CYGWIN)
      # So far this works on all non-Cygwin systems, but only Linux and
      # Mac OS X have been tested so far.
      set(DYNAMIC_SUFFIX ".so")
    endif(WIN32_OR_CYGWIN)

    #message("${SOURCE_ROOT_NAME}_RPATH = ${${SOURCE_ROOT_NAME}_RPATH}")
    if(USE_RPATH)
      set_target_properties(
	${SOURCE_ROOT_NAME}
	PROPERTIES
	PREFIX ""
	SUFFIX ${DYNAMIC_SUFFIX}
	INSTALL_RPATH
	"${${SOURCE_ROOT_NAME}_RPATH}"
	)
    else(USE_RPATH)
      set_target_properties(
	${SOURCE_ROOT_NAME}
	PROPERTIES
	PREFIX ""
	SUFFIX ${DYNAMIC_SUFFIX}
	)
    endif(USE_RPATH)
    if(BUILD_SHARED_LIBS)
      set_target_properties(${SOURCE_ROOT_NAME} PROPERTIES
	COMPILE_DEFINITIONS "USINGDLL"
	)
    endif(BUILD_SHARED_LIBS)

    if(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)
      add_custom_command(
	OUTPUT
	${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir/${SOURCE_ROOT_NAME}.driver_info
	COMMAND test-drv-info
	${SOURCE_ROOT_NAME}
	> ${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir/${SOURCE_ROOT_NAME}.driver_info
	COMMAND ${CMAKE_COMMAND} -E compare_files
	${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir/${SOURCE_ROOT_NAME}.driver_info
	${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.driver_info
	DEPENDS
	${SOURCE_ROOT_NAME}
	test-drv-info
	)
      add_custom_target(test_${SOURCE_ROOT_NAME}_dyndriver
	DEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir/${SOURCE_ROOT_NAME}.driver_info
	)
      set_property(GLOBAL
	PROPERTY FILE_DEPENDS_${SOURCE_ROOT_NAME}_dyndriver
	${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir/${SOURCE_ROOT_NAME}.driver_info
	)
      add_dependencies(test_${SOURCE_ROOT_NAME}_dyndriver
	test-drv-info
	${SOURCE_ROOT_NAME}
	)
      list(APPEND test_dyndrivers_TDEPENDS test_${SOURCE_ROOT_NAME}_dyndriver)
    endif(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)

    install(TARGETS ${SOURCE_ROOT_NAME}
      EXPORT export_plplot
      DESTINATION ${DRV_DIR}
      )
    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.driver_info
      DESTINATION ${DRV_DIR}
      )
  endforeach(SOURCE_ROOT_NAME ${DRIVERS_LIST})

  # The tk device driver depends internally on the xwin device driver.
  # Therefore make target tk depend on target xwin so
  # xwin will always be built first.
  if(TARGET tk)
    if(TARGET xwin)
      add_dependencies(tk xwin)
    else(TARGET xwin)
      message(FATAL_ERROR "Internal build system inconsistency where the tk target but not the xwin target are configured.")
    endif(TARGET xwin)
  endif(TARGET tk)

  if(PLD_tkwin AND USE_TCL_TK_STUBS)
    set_target_properties(
      tkwin
      PROPERTIES COMPILE_DEFINITIONS "USE_TCL_STUBS;USE_TK_STUBS"
      )
  endif(PLD_tkwin AND USE_TCL_TK_STUBS)

  if(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)
    add_custom_target(test_dyndrivers ALL
      )
    add_dependencies(test_dyndrivers ${test_dyndrivers_TDEPENDS})
  endif(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)

endif(ENABLE_DYNDRIVERS)
