# bindings/f95/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
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

  ## Build C part of F95 bindings
  if(WIN32_OR_CYGWIN)
    # Set the flags for the C compiler. The C stubs need to have the
    # correct case and calling convention for the Fortran compiler
    IF(TARGET_FORTRAN MATCHES "IVF")
      SET(DEFFILE "_ifort")
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DIVF")
    ELSEIF(TARGET_FORTRAN MATCHES "CVF")
      SET(DEFFILE "")
      SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DCVF")
    ENDIF(TARGET_FORTRAN MATCHES "IVF")
  endif(WIN32_OR_CYGWIN)

  # Build fortran part of F95 bindings.
  # Notes:

  # The CMake Fortran dependency scanner finds the Fortran include
  # files and deals properly with the dependencies on those files.
  # Therefore, there is no need to include those files in the SRCS
  # list or do anything special with their dependencies.

  # Check consistency of included_plplot_parameters.f90.
  add_custom_target(
    check_f95_parameters
    ${CMAKE_COMMAND} -E echo "Check that bindings/f95/included_plplot_parameters.f90 is consistent with the #defines in bindings/swig-support/plplotcapi.i"
    COMMAND ${CMAKE_COMMAND} -E remove -f
    ${CMAKE_CURRENT_BINARY_DIR}/included_plplot_parameters.f90_compare
    COMMAND
    sed -f ${CMAKE_CURRENT_SOURCE_DIR}/global_defines.sed <
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i >
    ${CMAKE_CURRENT_BINARY_DIR}/included_plplot_parameters.f90_compare
    COMMAND
    COMMAND
    cmp ${CMAKE_CURRENT_SOURCE_DIR}/included_plplot_parameters.f90
    ${CMAKE_CURRENT_BINARY_DIR}/included_plplot_parameters.f90_compare
    )

  add_dependencies(check_all check_f95_parameters)

  # Configure the Fortran source file that specifies the
  # types used for PLplot.
  if(PL_DOUBLE)
    set(ONE_IN_DEFAULT_PRECISION "1.0d0")
  else(PL_DOUBLE)
    set(ONE_IN_DEFAULT_PRECISION "1.0")
  endif(PL_DOUBLE)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/included_plplot_configured_types.f90.in
    ${CMAKE_CURRENT_BINARY_DIR}/included_plplot_configured_types.f90
    @ONLY
    )

  set(plplotf95_LIB_SRCS
    plplot.f90
    # The "single" and "double" here refer to user's real precision
    # rather than the actual PLFLT single or double precision so
    # we need to build both of these modules.
    plplot_single.f90
    plplot_double.f90
    # Every other PLplot module depends on these so must be kept
    # in a separate file.
    plplot_small_modules.f90
    )

  if(WIN32_AND_NOT_CYGWIN AND BUILD_SHARED_LIBS AND NOT MINGW)
    SET(plplotf95_LIB_SRCS ${plplotf95_LIB_SRCS} plplotf95${DEFFILE}.def)
  endif(WIN32_AND_NOT_CYGWIN AND BUILD_SHARED_LIBS AND NOT MINGW)

  add_library(plplotf95 ${plplotf95_LIB_SRCS})

  if(NON_TRANSITIVE)
    target_link_libraries(plplotf95 PRIVATE plplot)
  else(NON_TRANSITIVE)
    target_link_libraries(plplotf95 PUBLIC plplot)
  endif(NON_TRANSITIVE)

  set_library_properties(plplotf95)

  install(TARGETS plplotf95
    EXPORT export_plplot
    ARCHIVE DESTINATION ${LIB_DIR}
    LIBRARY DESTINATION ${LIB_DIR}
    RUNTIME DESTINATION ${BIN_DIR}
    )

  # While user programmes should only use the plplot module,
  # some compilers (e.g. Intel Fortran, but not gfortran) require access
  # to all the modules that the plplot module depends upon.
  install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/plplot.mod
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_double.mod
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_single.mod
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_private_utilities.mod
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_graphics.mod
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_private_exposed.mod
    ${CMAKE_CURRENT_BINARY_DIR}/plplot_types.mod
    DESTINATION ${F95_MOD_DIR}
    )

  # Configure pkg-config *.pc file corresponding to libplplotf95
  pkg_config_file("f95" "F95" " F95 binding" "plplotf95" "-I\"${F95_MOD_DIR}\"" "-lplplot" "")
endif(ENABLE_f95)
