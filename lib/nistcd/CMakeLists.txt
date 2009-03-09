# lib/nistcd/CMakeLists.txt for PLplot
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

# build nistcd library (a.k.a., "cd" library put into the public domain by
# NIST) required by the PLplot cgm device.
if(PLD_cgm)
  include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  )
  
  set( cd_LIB_SRCS cd.c )
  set( cd_LIB_HDRS cd.h defines.h )

  # Bypass "#include <malloc.h>" in all code where it appears since
  # being too specific like this fails on some platforms (e.g., OS X), and
  # the proper form of the malloc.h include should be dragged in by 
  # ""#include stdlib.h" in any case.
  # There may be some arcane platforms where we should not define this,
  # but for now, at least, try it for all platforms.
  set( cd_ADD_DEFINES "-DNOMALLOCH" )

  if(BUILD_SHARED_LIBS)
    set_source_files_properties( ${cd_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL ${cd_ADD_DEFINES}"
      )
  endif(BUILD_SHARED_LIBS)

  add_library( nistcd ${cd_LIB_SRCS} )
  set_target_properties(
    nistcd
    PROPERTIES
    SOVERSION ${nistcd_SOVERSION}
    VERSION ${nistcd_VERSION}
    INSTALL_NAME_DIR "${LIB_DIR}"
    )

  option(TEST_NISTCD "Test libnistcd" ON)
  if(TEST_NISTCD)
    # build and run applications to test libnistcd.
    set( cd_EXE color16 cdsimple cdtext cdmulti cdexpert )
    foreach(EXE ${cd_EXE})
      add_executable( ${EXE} ${EXE}.c )
      if(APPLE)
        set_source_files_properties( ${EXE}.c
          PROPERTIES COMPILE_FLAGS "-DNOMALLOCH"
          )
      endif(APPLE)    
      target_link_libraries( ${EXE} nistcd )
    endforeach(EXE ${cd_EXE})
  endif(TEST_NISTCD)

  # install library and binaries
  install( TARGETS
    nistcd
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  # install library header files.
  install(
    FILES ${cd_LIB_HDRS}
    DESTINATION ${INCLUDE_DIR}
    )
endif(PLD_cgm)
