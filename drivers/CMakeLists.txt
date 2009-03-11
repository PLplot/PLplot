# drivers/CMakeLists.txt for PLplot
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

install(
  FILES README.drivers README.wxwidgets 
  DESTINATION ${DOC_DIR}
  )

#qt.h only useful if anyone wants to build a Qt application with the Qt widget driver
if(PLD_qtwidget)
  install(
    FILES qt.h 
    DESTINATION ${INCLUDE_DIR}
    )
endif(PLD_qtwidget)

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
      PROPERTIES COMPILE_FLAGS "-I${LTDL_INCLUDE_DIR} -DUSINGDLL"
      )
    
    add_executable(test-drv-info test-drv-info.c)
    target_link_libraries(test-drv-info
      plplot${LIB_TAG}
      ${LTDL_LIBRARIES}
      )
    set(test-drv-info_RPATH
      ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_BINARY_DIR}/src
      ${CMAKE_BINARY_DIR}/lib/csa
      ${CMAKE_BINARY_DIR}/lib/nn
      ${CMAKE_BINARY_DIR}/lib/qsastime
      ${libplplot${LIB_TAG}_RPATH}
      )
    
    set_target_properties(
      test-drv-info
      PROPERTIES
      INSTALL_RPATH "${test-drv-info_RPATH}"
      BUILD_WITH_INSTALL_RPATH ON
      )
    get_target_property(test-drv-info_LOCATION test-drv-info LOCATION)
    set(test_dyndrivers_FDEPENDS)
    set(test_dyndrivers_TDEPENDS test-drv-info)

  endif(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)
  
  foreach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
    #message("${SOURCE_ROOT_NAME}_SOURCE = ${${SOURCE_ROOT_NAME}_SOURCE}")
    if(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
      set_source_files_properties(
	${${SOURCE_ROOT_NAME}_SOURCE}
	PROPERTIES COMPILE_FLAGS "${${SOURCE_ROOT_NAME}_COMPILE_FLAGS} -DUSINGDLL"
	)
      #message("${SOURCE_ROOT_NAME}_COMPILE_FLAGS = ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS}")
    else(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
      set_source_files_properties(
	${${SOURCE_ROOT_NAME}_SOURCE}
	PROPERTIES COMPILE_FLAGS "-DUSINGDLL"
	)
    endif(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
    if(SOURCE_ROOT_NAME STREQUAL "qt" AND PLD_qtwidget)
      qt4_wrap_cpp(
        QT_MOC_OUTFILES 
	${CMAKE_CURRENT_SOURCE_DIR}/qt.h
	OPTIONS "-DPLD_qtwidget=ON"
	)
      # message("QT_MOC_OUTFILES = ${QT_MOC_OUTFILES}")
      set_source_files_properties(
	${QT_MOC_OUTFILES}
	PROPERTIES GENERATED "ON"
	)
      add_library(${SOURCE_ROOT_NAME} MODULE ${${SOURCE_ROOT_NAME}_SOURCE} ${QT_MOC_OUTFILES})
    else(SOURCE_ROOT_NAME STREQUAL "qt" AND PLD_qtwidget)
      add_library(${SOURCE_ROOT_NAME} MODULE ${${SOURCE_ROOT_NAME}_SOURCE})
    endif(SOURCE_ROOT_NAME STREQUAL "qt" AND PLD_qtwidget)
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
    target_link_libraries(
      ${SOURCE_ROOT_NAME}
      plplot${LIB_TAG}
      ${MATH_LIB}
      ${${SOURCE_ROOT_NAME}_LINK_FLAGS}
      ${${SOURCE_ROOT_NAME}_TARGETS}
      )
    # ${SOURCE_ROOT_NAME}_RPATH originally set in cmake/modules files for
    # each driver in ${DRIVERS_LIST}.  This is only used for the 
    # install-tree rpath since cmake handles the build-tree rpath
    # automatically (so long as full pathnames to libraries are used).
    set(
      ${SOURCE_ROOT_NAME}_RPATH
      ${LIB_DIR}
      ${${SOURCE_ROOT_NAME}_RPATH}
      ${libplplot${LIB_TAG}_RPATH}
      )
    # Our implementation of dynamic loading using libltdl assumes
    # the prefix should always be nothing (rather than lib) and the suffix
    # should be set in a platform-dependent manner in order for libltdl
    # to find the dynamic device.
    if(WIN32)
      # strace showed that .dll was a must for libltdl on Cygwin.
      set(DYNAMIC_SUFFIX ".dll")
    else(WIN32)
      # So far this works on all non-Cygwin systems, but only Linux and
      # Mac OS X have been tested so far.
      set(DYNAMIC_SUFFIX ".so")
    endif(WIN32)
    
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
    if(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)
      get_target_property(
	${SOURCE_ROOT_NAME}_LOCATION
	${SOURCE_ROOT_NAME} 
	LOCATION
	)
      add_custom_command(
	OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir/${SOURCE_ROOT_NAME}.rc
	COMMAND test-drv-info
	${SOURCE_ROOT_NAME}
	> ${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir/${SOURCE_ROOT_NAME}.rc
	COMMAND ${CMAKE_COMMAND} -E compare_files
	${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir/${SOURCE_ROOT_NAME}.rc
	${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc
	DEPENDS
	${${SOURCE_ROOT_NAME}_LOCATION}
	${test-drv-info_LOCATION}
	)
      list(APPEND test_dyndrivers_FDEPENDS
	${CMAKE_CURRENT_BINARY_DIR}/test_dyndrivers_dir/${SOURCE_ROOT_NAME}.rc
	)
      list(APPEND test_dyndrivers_TDEPENDS ${SOURCE_ROOT_NAME})
    endif(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)
    
    install(TARGETS ${SOURCE_ROOT_NAME} DESTINATION ${DRV_DIR})
    install(
      FILES ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc 
      DESTINATION ${DRV_DIR}
      )
  endforeach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
  if(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)
    add_custom_target(test_dyndrivers
      DEPENDS ${test_dyndrivers_FDEPENDS}
      )
    add_dependencies(test_dyndrivers ${test_dyndrivers_TDEPENDS})
  endif(TEST_DYNDRIVERS AND NOT CMAKE_CROSSCOMPILING)

endif(ENABLE_DYNDRIVERS)
