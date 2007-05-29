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
    target_link_libraries(
    ${SOURCE_ROOT_NAME}
    plplot${LIB_TAG}
    ${MATH_LIB}
    ${${SOURCE_ROOT_NAME}_LINK_FLAGS}
    ${${SOURCE_ROOT_NAME}_TARGETS}
    )
    #message("${SOURCE_ROOT_NAME}_LINK_FLAGS = ${${SOURCE_ROOT_NAME}_LINK_FLAGS}")
    #message("${SOURCE_ROOT_NAME}_TARGETS = ${${SOURCE_ROOT_NAME}_TARGETS}")
    set(
    ${SOURCE_ROOT_NAME}_RPATH
    ${${SOURCE_ROOT_NAME}_RPATH}
    ${libplplot${LIB_TAG}_RPATH}
    ${LIB_DIR}
    )
    # Our implementation of dynamic loading using libltdl assumes
    # the prefix should always be nothing (rather than lib) and the suffix
    # should always be ".so" regardless of platform so that libltdl can
    # always find the result.
    #message("${SOURCE_ROOT_NAME}_RPATH = ${${SOURCE_ROOT_NAME}_RPATH}")
    if(USE_RPATH)
      set_target_properties(
      ${SOURCE_ROOT_NAME}
      PROPERTIES
      PREFIX ""
      SUFFIX ".so"
      INSTALL_RPATH
      "${${SOURCE_ROOT_NAME}_RPATH}"
      )
    else(USE_RPATH)
      set_target_properties(
      ${SOURCE_ROOT_NAME}
      PROPERTIES
      PREFIX ""
      SUFFIX ".so"
      )
    endif(USE_RPATH)
# This is a clumsy way to do it since we have to put in dependencies on
# get-drv-info so it will always be built first, but the better 
# alternative below does not work (probably a bug in 2.4.2).
    add_dependencies(${SOURCE_ROOT_NAME} get-drv-info)
    add_custom_command(
    TARGET
    ${SOURCE_ROOT_NAME}
    POST_BUILD
    COMMAND
    ${CMAKE_CURRENT_BINARY_DIR}/get-drv-info
    ${SOURCE_ROOT_NAME}
    > ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc
    )
# Should work, but does not (for cmake 2.4.2, at least) so comment out.
#    add_custom_command(
#    OUTPUT 
#    ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc
#    COMMAND
#    ${CMAKE_CURRENT_BINARY_DIR}/get-drv-info
#    ${SOURCE_ROOT_NAME}
#    > ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc
#    DEPENDS
#    ${CMAKE_CURRENT_BINARY_DIR}/get-drv-info 
#    ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.so
#    )
    install(TARGETS ${SOURCE_ROOT_NAME} DESTINATION ${DRV_DIR})
    install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_ROOT_NAME}.rc 
    DESTINATION ${DRV_DIR}
    )
  endforeach(SOURCE_ROOT_NAME ${DRIVERS_LIST})

endif(ENABLE_DYNDRIVERS)
