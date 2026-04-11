# bindings/fortran/CMakeLists.txt
# Copyright (C) 2006 Andrew Ross
# Copyright (C) 2006-2026 Alan W. Irwin
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

if(ENABLE_fortran)

  # Set the include path
  include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/qsastime
    ${CMAKE_BINARY_DIR}
    ${CMAKE_BINARY_DIR}/include
    ${CMAKE_BINARY_DIR}/lib/qsastime
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    )

  ## Build C part of Fortran bindings
  if(WIN32_OR_CYGWIN)
    # Set the flags for the C compiler. The C stubs need to have the
    # correct case and calling convention for the Fortran compiler.

    # Note that TARGET_FORTRAN is set to GFORTRAN, IFORT, or IFX
    # in cmake/modules/fortran.cmake depending on the value of
    # CMAKE_Fortran_COMPILER_ID.

    # Maintenance 2026: I (AWI) have assumed the IFX case is the same
    # as IFORT below, but tests with ifx (the modern successor to
    # ifort) on windows and/or Cygwin will be required to find out
    # whether that is correct.  Furthermore, for the IFORT case the
    # google AI states the following:

    # "The MSVC /DIVF option was a historical compiler flag used in
    # mixed-language programming to ensure binary compatibility
    # between C code and Fortran code compiled with older versions of
    # Digital Visual Fortran (DVF) or early versions of Intel Visual
    # Fortran (IVF).  ...  Modern Alternative: In modern development
    # with Intel oneAPI Fortran, this flag is largely
    # obsolete. Developers should instead use the ISO_C_BINDING module
    # in Fortran and the BIND(C) attribute. This provides a
    # standardized, portable way to handle calling conventions and
    # name mangling without relying on legacy compiler-specific
    # flags."

    # Therefore, since we already do use the ISO_C_BINDING module, I have
    # commented out appending the /DIVF MSVC compile option below, but that
    # change needs to be reviewed for both ifort and ifx on Windows and/or Cygwin.
    # Also according to the google, AI ifort and ifx ignore public/private code
    # attributes when deciding what symbols are exported from a Fortran library so
    # DEFFILE is still needed to identify the plplotfortran${DEFFILE}.def
    # module definition file for ifort below, and I have assumed that ifx can use
    # this same file, but that also has to be reviewed.

    if(TARGET_FORTRAN STREQUAL "IFORTRAN" OR TARGET_FORTRAN STREQUAL "IFX")
      set(DEFFILE "_ifort")
      #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /DIVF")
    endif(TARGET_FORTRAN STREQUAL "IFORTRAN" OR TARGET_FORTRAN STREQUAL "IFX")
  endif(WIN32_OR_CYGWIN)

  # Build fortran part of Fortran bindings.
  # Notes:

  # The CMake Fortran dependency scanner finds the Fortran include
  # files and deals properly with the dependencies on those files.
  # Therefore, there is no need to include those files in the SRCS
  # list or do anything special with their dependencies.

  # Check consistency of included_plplot_parameters.f90.
  add_custom_target(
    check_fortran_parameters
    ${CMAKE_COMMAND} -E echo "Check that bindings/fortran/included_plplot_parameters.f90 is consistent with the #defines in bindings/swig-support/plplotcapi.i"
    COMMAND ${CMAKE_COMMAND} -E remove -f
    ${CMAKE_CURRENT_BINARY_DIR}/included_plplot_parameters.f90_compare
    COMMAND
    sed -f ${CMAKE_CURRENT_SOURCE_DIR}/global_defines.sed <
    ${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i >
    ${CMAKE_CURRENT_BINARY_DIR}/included_plplot_parameters.f90_compare
    COMMAND
    cmp ${CMAKE_CURRENT_SOURCE_DIR}/included_plplot_parameters.f90
    ${CMAKE_CURRENT_BINARY_DIR}/included_plplot_parameters.f90_compare
    )
  # check_plplotcapi_defines should be done first since
  # ${CMAKE_SOURCE_DIR}/bindings/swig-support/plplotcapi.i
  # might be changed as a result of that check.
  add_dependencies(check_fortran_parameters check_plplotcapi_defines)
  add_dependencies(check_all check_fortran_parameters)

  # Check consistency of plplotfortran_ifort.def using Unix commands
  # First write header of generated plplotfortran_ifort.def file
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/plplotfortran_ifort.def_header
    "; You should not normally edit this generated file.  Instead, check its consistency
; with the symbols in the plplotfortran library using the
; (Unix) target \"check_ifort_definitions_file\".  If that target reports an
; inconsistency (via a cmp message) between the generated
; plplotfortran_ifort.def_compare file in the build tree and
; plplotfortran_ifort.def in the source tree, then copy that generated
; plplotfortran_ifort.def_compare on top of plplotfortran_ifort.def and
; check in that result.
LIBRARY plplotfortran.dll
EXPORTS
")

  # Create a configurable file that contains the bash command to create plplotfortran_ifort.def_compare
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/plplotfortran_ifort.def.sh.in
    "nm --defined-only \$<TARGET_FILE:PLPLOT::plplotfortran> |tr '[:lower:]' '[:upper:]' |sed -f ${CMAKE_CURRENT_SOURCE_DIR}/generate_ifort_deffile.sed |sort --key=3 | cat ${CMAKE_CURRENT_BINARY_DIR}/plplotfortran_ifort.def_header - >| ${CMAKE_CURRENT_BINARY_DIR}/plplotfortran_ifort.def_compare
")

  # Configure that file (which contains the generator expression, $<TARGET_FILE:PLPLOT::plplotfortran>) :
  configure_file_generate(
  ${CMAKE_CURRENT_BINARY_DIR}/plplotfortran_ifort.def.sh.in
  ${CMAKE_CURRENT_BINARY_DIR}/plplotfortran_ifort.def.sh
  @ONLY
)

  add_custom_target(
    check_ifort_definitions_file
    ${CMAKE_COMMAND} -E echo "Check that bindings/fortran/plplotfortran_ifort.def is consistent with the symbols in the plplotfortran library"
    COMMAND ${CMAKE_COMMAND} -E remove -f
    ${CMAKE_CURRENT_BINARY_DIR}/plplotfortran_ifort.def_compare
    COMMAND
    bash ${CMAKE_CURRENT_BINARY_DIR}/plplotfortran_ifort.def.sh
    COMMAND
    cmp ${CMAKE_CURRENT_SOURCE_DIR}/plplotfortran_ifort.def
    ${CMAKE_CURRENT_BINARY_DIR}/plplotfortran_ifort.def_compare
    )

  add_dependencies(check_ifort_definitions_file PLPLOT::plplotfortran)
  add_dependencies(check_all check_ifort_definitions_file)

  # Configure the Fortran source file that specifies the
  # types used for PLplot.
  if(PL_DOUBLE)
    set(FORTRAN_EQUIVALENT_PLFLT "c_double")
  else(PL_DOUBLE)
    set(FORTRAN_EQUIVALENT_PLFLT "c_float")
  endif(PL_DOUBLE)

  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/included_plplot_configured_types.f90.in
    ${CMAKE_CURRENT_BINARY_DIR}/included_plplot_configured_types.f90
    @ONLY
    )

  set(plplotfortran_LIB_SRCS
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

  if(WIN32_AND_NOT_CYGWIN AND BUILD_SHARED_LIBS AND NOT MINGW AND NOT MSYS)
    set(plplotfortran_LIB_SRCS ${plplotfortran_LIB_SRCS} plplotfortran${DEFFILE}.def)
  endif(WIN32_AND_NOT_CYGWIN AND BUILD_SHARED_LIBS AND NOT MINGW AND NOT MSYS)


  # plplotfortran depends on the plplot library.
  set(LIB_INSTALL_RPATH ${LIB_DIR})

  configure_library_build(plplotfortran "" "${plplotfortran_LIB_SRCS}" PLPLOT::plplot "${LIB_INSTALL_RPATH}")

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
    DESTINATION ${FORTRAN_MOD_DIR}
    )

  # Configure pkg-config *.pc file corresponding to libplplotfortran
  pkg_config_file("fortran" "Fortran" " Fortran binding" "plplotfortran" "-I\"${FORTRAN_MOD_DIR}\"" "-lplplot" "")
endif(ENABLE_fortran)
