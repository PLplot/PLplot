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
  TARGET_LINK_LIBRARIES(get-drv-info
  plplot${LIB_TAG}
  ${LTDL_LIBRARIES}
  )

  SET(plplotdrv_STAT_SRCS
  )

#INSTALL_FILES(  FILES  $(drivers_LTLIBRARIES:.la=.rc) )
#INSTALL_FILES(  FILES  README.drivers README.wxwidgets )
endif(ENABLE_DYNDRIVERS)

