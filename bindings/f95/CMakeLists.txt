# bindings/f95/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
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

if(ENABLE_f95)

  # Set the include path
  include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )

  # Configure the Fortran include file that specifies the kind of
  # floating-point (single vs double) PLplot supports.
  if(PL_DOUBLE)
    set(ONE_IN_DEFAULT_PRECISION "1.0d0")
  else(PL_DOUBLE)
    set(ONE_IN_DEFAULT_PRECISION "1.0")
  endif(PL_DOUBLE)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/plflt.inc.in
    ${CMAKE_CURRENT_BINARY_DIR}/plflt.inc
    @ONLY
    )

  ## Build C part of F95 bindings
  if(WIN32)
    # Set the flags for the C compiler. The C stubs need to have the
    # correct case and calling convention for the Fortran compiler
    IF(TARGET_FORTRAN MATCHES "IVF")
      SET(DEFFILE "_ifort")
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DIVF")
    ELSEIF(TARGET_FORTRAN MATCHES "CVF")
      SET(DEFFILE "")
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DCVF")
    ENDIF(TARGET_FORTRAN MATCHES "IVF")
  endif(WIN32)

  set(plplotf95c${LIB_TAG}_LIB_SRCS
    sc3d.c
    sccont.c
    scstubs.c
    )

  if(BUILD_SHARED_LIBS)
    SET_SOURCE_FILES_PROPERTIES(${plplotf95c${LIB_TAG}_LIB_SRCS}
      PROPERTIES COMPILE_FLAGS "-DUSINGDLL" )
  endif(BUILD_SHARED_LIBS)

  add_library(plplotf95c${LIB_TAG} ${plplotf95c${LIB_TAG}_LIB_SRCS})

  target_link_libraries(plplotf95c${LIB_TAG}  plplot${LIB_TAG})

  if(USE_RPATH)
    get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
    set_target_properties(plplotf95c${LIB_TAG}
      PROPERTIES
      SOVERSION ${plplotf95_SOVERSION}
      VERSION ${plplotf95_VERSION}
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  else(USE_RPATH)
    set_target_properties(plplotf95c${LIB_TAG}
      PROPERTIES
      SOVERSION ${plplotf95_SOVERSION}
      VERSION ${plplotf95_VERSION}
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  endif(USE_RPATH)

  install(TARGETS plplotf95c${LIB_TAG}
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  # Build fortran part of F95 bindings.
  # Notes:

  # configurable.f90 is actually not currently configurable.

  # The CMake Fortran dependency scanner finds the Fortran include
  # file, sfstub.h, and the configured Fortran include file,
  # ${CMAKE_CURRENT_BINARY_DIR}/plflt.inc, and also deals properly with
  # the dependencies on those files.  Therefore, there is no need to
  # include those files in the SRCS list or do anything special with
  # their dependencies.

  # Check consistency of plplot_parameters.h.
  add_custom_target(
    check_f95_parameters
    COMMAND ${CMAKE_COMMAND} -E remove -f 
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_parameters.h_compare
    COMMAND
    sed -f ${CMAKE_CURRENT_SOURCE_DIR}/global_defines.sed < 
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i > 
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_parameters.h_compare
    COMMAND
    ${CMAKE_COMMAND} -E echo "Check that plplot_parameters.h is consistent with the #defines in bindings/swig-support/plplotcapi.i"
    COMMAND
    cmp ${CMAKE_CURRENT_SOURCE_DIR}/plplot_parameters.h
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_parameters.h_compare
    )

  if(STATIC_OPTS)
    set(plplotf95opts${LIB_TAG}_LIB_SRCS
      configurable.f90
      )
    set(plplotf95${LIB_TAG}_LIB_SRCS
      strutil.f90
      sfstubsf95.f90
      )
  else(STATIC_OPTS)
    set(plplotf95${LIB_TAG}_LIB_SRCS
      strutil.f90
      configurable.f90
      sfstubsf95.f90
      )
  endif(STATIC_OPTS)

  if(WIN32 AND BUILD_SHARED_LIBS AND NOT MINGW AND NOT CYGWIN)
    SET(plplotf95${LIB_TAG}_LIB_SRCS ${plplotf95${LIB_TAG}_LIB_SRCS} plplotf95${DEFFILE}.def)
  endif(WIN32 AND BUILD_SHARED_LIBS AND NOT MINGW AND NOT CYGWIN)

  add_library(plplotf95${LIB_TAG} ${plplotf95${LIB_TAG}_LIB_SRCS})

  if(NON_TRANSITIVE)
    # empty list ==> non-transitive linking for everything that links to
    # libplplotf95cd and libplplotf95d in the shared libraries case.
    target_link_libraries(plplotf95c${LIB_TAG} LINK_INTERFACE_LIBRARIES)
    target_link_libraries(plplotf95${LIB_TAG} LINK_INTERFACE_LIBRARIES)
    # This configures the pkg-config method to use non-transitive linking.
    set(PC_REQUIRES_TAG "Requires.private")
  else(NON_TRANSITIVE)
    # This configures the pkg-config method to use transitive linking
    set(PC_REQUIRES_TAG "Requires")
  endif(NON_TRANSITIVE)

  if(STATIC_OPTS)
    add_library(plplotf95opts${LIB_TAG} STATIC ${plplotf95opts${LIB_TAG}_LIB_SRCS})
    target_link_libraries(plplotf95${LIB_TAG}  plplotf95c${LIB_TAG})
    target_link_libraries(plplotf95opts${LIB_TAG}  plplotf95${LIB_TAG} plplotf95c${LIB_TAG})
  else(STATIC_OPTS)
    target_link_libraries(plplotf95${LIB_TAG}  plplotf95c${LIB_TAG})
  endif(STATIC_OPTS)

  if(USE_RPATH)
    set_target_properties(plplotf95${LIB_TAG}
      PROPERTIES
      SOVERSION ${plplotf95_SOVERSION}
      VERSION ${plplotf95_VERSION}
      INSTALL_RPATH "${LIB_INSTALL_RPATH}"
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  else(USE_RPATH)
    set_target_properties(plplotf95${LIB_TAG}
      PROPERTIES
      SOVERSION ${plplotf95_SOVERSION}
      VERSION ${plplotf95_VERSION}
      INSTALL_NAME_DIR "${LIB_DIR}"
      )
  endif(USE_RPATH)

  # Set the path and the suffix for the (shared) libraries
  if(CYGWIN)
    if(BUILD_SHARED_LIBS)
      set(SHLIB_DIR "${BIN_DIR}")
    endif(BUILD_SHARED_LIBS)
    set(VERSION_SUFFIX "-${plplotf95_VERSION}")
  else(CYGWIN)
    set(VERSION_SUFFIX "")
  endif(CYGWIN)

  install(TARGETS plplotf95${LIB_TAG}
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  if(STATIC_OPTS)
    install(TARGETS plplotf95opts${LIB_TAG}
      EXPORT export_plplot
      ARCHIVE DESTINATION ${LIB_DIR}
      LIBRARY DESTINATION ${LIB_DIR}
      RUNTIME DESTINATION ${BIN_DIR}
      )
  endif(STATIC_OPTS)

  # For CMake-2.6.0 and above, the module files are created by
  # default during the library build in the bindings/f95 directory.
  install(
    FILES
    ${CMAKE_BINARY_DIR}/bindings/f95/plplot.mod
    ${CMAKE_BINARY_DIR}/bindings/f95/plplotp.mod
    ${CMAKE_BINARY_DIR}/bindings/f95/plplot_flt.mod
    DESTINATION ${F95_MOD_DIR}
    )

  # Configure pkg-config *.pc file corresponding to libplplotf95${LIB_TAG}
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
    set(PC_DATA "f95:F95:F95 bindings, :plplotf95${LIB_TAG}${VERSION_SUFFIX}")

    string(REGEX REPLACE "^(.*):.*:.*:.*$" "\\1" BINDING ${PC_DATA})
    set(PC_FILE_SUFFIX "-${BINDING}")
    set(PC_REQUIRES "plplot${LIB_TAG}")
    string(REGEX REPLACE "^.*:(.*):.*:.*$" "\\1" PC_SHORT_NAME ${PC_DATA})
    string(REGEX REPLACE "^.*:.*:(.*):.*$" "\\1" PC_LONG_NAME ${PC_DATA})
    string(REGEX REPLACE "^.*:.*:.*:(.*)$" "\\1" PC_LIBRARY_NAME ${PC_DATA})
    set(PC_LINK_FLAGS "${lib${PC_LIBRARY_NAME}_LINK_FLAGS}")
    set(PC_COMPILE_FLAGS "${lib${PC_LIBRARY_NAME}_COMPILE_FLAGS}")
    set(PC_LINK_FLAGS
      "-l${PC_LIBRARY_NAME} -lplplotf95c${LIB_TAG}${VERSION_SUFFIX} ${PC_LINK_FLAGS}"
      )
    set(PC_COMPILE_FLAGS "-I${F95_MOD_DIR} ${PC_COMPILE_FLAGS}")
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
endif(ENABLE_f95)
