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

if(ENABLE_DYNDRIVERS)
  set_source_files_properties(
  get-drv-info.c
  PROPERTIES COMPILE_FLAGS "-I${LTDL_INCLUDE_DIR}"
  )
			   
  include_directories(
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/include
  )

  add_executable(get-drv-info get-drv-info.c)
  target_link_libraries(get-drv-info
  plplot${LIB_TAG}
  ${LTDL_LIBRARIES}
  )
  set(get-drv-info_RPATH
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/src
  ${CMAKE_BINARY_DIR}/lib/csa
  ${CMAKE_BINARY_DIR}/lib/nn
  ${libplplot${LIB_TAG}_RPATH}
  )
  
  set_target_properties(
  get-drv-info
  PROPERTIES
  INSTALL_RPATH "${get-drv-info_RPATH}"
  BUILD_WITH_INSTALL_RPATH ON
  )
  
  get_target_property(get-drv-info_LOCATION get-drv-info LOCATION)

  foreach(SOURCE_ROOT_NAME ${DRIVERS_LIST})
    #message("${SOURCE_ROOT_NAME}_SOURCE = ${${SOURCE_ROOT_NAME}_SOURCE}")
    if(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
      set_source_files_properties(
      ${${SOURCE_ROOT_NAME}_SOURCE}
      PROPERTIES COMPILE_FLAGS ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS}
      )
      #message("${SOURCE_ROOT_NAME}_COMPILE_FLAGS = ${${SOURCE_ROOT_NAME}_COMPILE_FLAGS}")
    endif(${SOURCE_ROOT_NAME}_COMPILE_FLAGS)
    add_library(${SOURCE_ROOT_NAME} MODULE ${${SOURCE_ROOT_NAME}_SOURCE})
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
    ${${SOURCE_ROOT_NAME}_RPATH}
    ${libplplot${LIB_TAG}_RPATH}
    ${LIB_DIR}
    )
    # Our implementation of dynamic loading using libltdl assumes
    # the prefix should always be nothing (rather than lib) and the suffix
    # should be set in a platform-dependent manner in order for libltdl
    # to find the dynamic device.
    if(CYGWIN)
      # strace showed that .dll was a must for libltdl on Cygwin.
      set(DYNAMIC_SUFFIX ".dll")
    else(CYGWIN)
      # So far this works on all non-Cygwin systems, but only Linux and
      # Mac OS X have been tested so far.
      set(DYNAMIC_SUFFIX ".so")
    endif(CYGWIN)
    
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
    get_target_property(
    ${SOURCE_ROOT_NAME}_LOCATION
    ${SOURCE_ROOT_NAME} 
    LOCATION
    )
    add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc
    COMMAND ${get-drv-info_LOCATION}
    ${SOURCE_ROOT_NAME}
    > ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc
    DEPENDS
    ${get-drv-info_LOCATION}
    ${${SOURCE_ROOT_NAME}_LOCATION}
    )
    add_custom_target(
    ${SOURCE_ROOT_NAME}_CHECK ALL
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc
    )
    # Workaround CMake 2-level make recursion issue for parallel builds.
    add_dependencies(${SOURCE_ROOT_NAME}_CHECK ${SOURCE_ROOT_NAME})
    
    install(TARGETS ${SOURCE_ROOT_NAME} DESTINATION ${DRV_DIR})
    install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc 
    DESTINATION ${DRV_DIR}
    )
  endforeach(SOURCE_ROOT_NAME ${DRIVERS_LIST})

endif(ENABLE_DYNDRIVERS)
