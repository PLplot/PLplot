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
  if(TEST_NISTCD AND NOT CMAKE_CROSSCOMPILING)
    # build applications to test libnistcd.
    set( cd_EXE cdexpert cdmulti cdsimple cdtest cdtext color16 )
    foreach(EXE ${cd_EXE})
      add_executable( ${EXE} ${EXE}.c )
      set_source_files_properties( ${EXE}.c
        PROPERTIES COMPILE_FLAGS "${cd_ADD_DEFINES}"
        )
      target_link_libraries( ${EXE} nistcd )
      get_target_property(${EXE}_LOC ${EXE} LOCATION)
    endforeach(EXE ${cd_EXE})
    # Create tests for libnistcd.
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir)

    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdexp1.cgm
      COMMAND ${cdexpert_LOC}
      COMMAND ${CMAKE_COMMAND} -E compare_files
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdexp1.cgm
      ${CMAKE_CURRENT_SOURCE_DIR}/cdexp1.cgm
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir
      DEPENDS ${cdexpert_LOC}
      )
    
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdmulti.cgm
      COMMAND ${cdmulti_LOC}
      COMMAND ${CMAKE_COMMAND} -E compare_files
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdmulti.cgm
      ${CMAKE_CURRENT_SOURCE_DIR}/cdmulti.cgm
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir
      DEPENDS ${cdmulti_LOC}
      )
    
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdsimple.cgm
      COMMAND ${cdsimple_LOC}
      COMMAND ${CMAKE_COMMAND} -E compare_files
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdsimple.cgm
      ${CMAKE_CURRENT_SOURCE_DIR}/cdsimple.cgm
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir
      DEPENDS ${cdsimple_LOC}
      )
    
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdout.cgm
      COMMAND ${cdtest_LOC}
      COMMAND ${CMAKE_COMMAND} -E compare_files
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdout.cgm
      ${CMAKE_CURRENT_SOURCE_DIR}/cdout.cgm
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir
      DEPENDS ${cdtest_LOC}
      )
    
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdtext.cgm
      COMMAND ${cdtext_LOC}
      COMMAND ${CMAKE_COMMAND} -E compare_files
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdtext.cgm
      ${CMAKE_CURRENT_SOURCE_DIR}/cdtext.cgm
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir
      DEPENDS ${cdtext_LOC}
      )
    
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/color16.cgm
      COMMAND ${color16_LOC}
      COMMAND ${CMAKE_COMMAND} -E compare_files
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/color16.cgm
      ${CMAKE_CURRENT_SOURCE_DIR}/color16.cgm
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir
      DEPENDS ${color16_LOC}
      )

    add_custom_target(test_nistcd ALL
      DEPENDS
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdexp1.cgm
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdmulti.cgm
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdsimple.cgm
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdout.cgm
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/cdtext.cgm
      ${CMAKE_CURRENT_BINARY_DIR}/test_nistcd_dir/color16.cgm
      )
    
  endif(TEST_NISTCD AND NOT CMAKE_CROSSCOMPILING) 

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
