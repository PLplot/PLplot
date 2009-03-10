# examples/CMakeLists.txt
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

# Sort out RPATH issues for build of installed examples.
if(USE_RPATH)
  get_target_property(_LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
  string(REGEX REPLACE ";" ":" LIB_INSTALL_RPATH "${_LIB_INSTALL_RPATH}")
  set(RPATHCMD "-Wl,-rpath -Wl,${LIB_INSTALL_RPATH}")
endif(USE_RPATH)

# Install lena.pgm in installed examples directory so that all implementations
# of example 20 in the various examples subdirectories can conveniently
# access this file.
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm
  DESTINATION ${DATA_DIR}/examples
  )

# Do the same as above for the build tree for those who want to individually
# run the various example 20 implementations from the examples subdirectories
# of the build tree.

if(BUILD_TEST)
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm
    ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/lena.pgm
    )
  add_custom_target(examples_lena_file ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/lena.pgm
    )
endif(BUILD_TEST)

add_subdirectory(c)
if(ENABLE_cxx)
  add_subdirectory(c++)
endif(ENABLE_cxx)
if(ENABLE_f77)
  add_subdirectory(f77)
endif(ENABLE_f77)
if(ENABLE_f95)
  add_subdirectory(f95)
endif(ENABLE_f95)
if(ENABLE_python)
  add_subdirectory(python)
endif(ENABLE_python)
if(ENABLE_tcl)
  add_subdirectory(tcl)
endif(ENABLE_tcl)
if(ENABLE_tk)
  add_subdirectory(tk)
endif(ENABLE_tk)
if(ENABLE_java)
  add_subdirectory(java)
endif(ENABLE_java)
if(ENABLE_pdl)
  add_subdirectory(perl)
endif(ENABLE_pdl)
if(ENABLE_ada)
  add_subdirectory(ada)
endif(ENABLE_ada)
if(ENABLE_d)
  add_subdirectory(d)
endif(ENABLE_d)
if(ENABLE_ocaml)
  add_subdirectory(ocaml)
endif(ENABLE_ocaml)
if(ENABLE_lua)
  add_subdirectory(lua)
endif(ENABLE_lua)

# Configure Makefile.examples with some specific variables

if(PLD_ps)
  if(NOT ENABLE_ada)
    set(PSC_ADA_COMMENT "#")
  endif(NOT ENABLE_ada)

  if(NOT ENABLE_cxx)
    set(PSC_CXX_COMMENT "#")
  endif(NOT ENABLE_cxx)

  if(NOT ENABLE_f77)
    set(PSC_F77_COMMENT "#")
  endif(NOT ENABLE_f77)

  if(NOT ENABLE_f95)
    set(PSC_F95_COMMENT "#")
  endif(NOT ENABLE_f95)

  if(NOT ENABLE_java)
    set(PSC_JAVA_COMMENT "#")
  endif(NOT ENABLE_java)

  if(NOT ENABLE_ocaml)
    set(PSC_OCAML_COMMENT "#")
  endif(NOT ENABLE_ocaml)

  if(NOT ENABLE_octave)
    set(PSC_OCTAVE_COMMENT "#")
  endif(NOT ENABLE_octave)

  if(NOT ENABLE_pdl)
    set(PSC_PERL_COMMENT "#")
  endif(NOT ENABLE_pdl)

  if(NOT ENABLE_python)
    set(PSC_PYTHON_COMMENT "#")
  endif(NOT ENABLE_python)

  if(NOT ENABLE_tcl)
    set(PSC_TCL_COMMENT "#")
  endif(NOT ENABLE_tcl)

  if(NOT ENABLE_lua)
    set(PSC_LUA_COMMENT "#")
  endif(NOT ENABLE_lua)

else(PLD_ps)
  set(PSC_ADA_COMMENT "#")
  set(PSC_C_COMMENT "#")
  set(PSC_CXX_COMMENT "#")
  set(PSC_F77_COMMENT "#")
  set(PSC_F95_COMMENT "#")
  set(PSC_JAVA_COMMENT "#")
  set(PSC_OCAML_COMMENT "#")
  set(PSC_OCTAVE_COMMENT "#")
  set(PSC_PERL_COMMENT "#")
  set(PSC_PYTHON_COMMENT "#")
  set(PSC_TCL_COMMENT "#")
endif(PLD_ps)

if(NOT ENABLE_tk)
  set(TK_COMMENT "#")
endif(NOT ENABLE_tk)

if(NOT PLD_pdfcairo)
  set(PDFCAIRO_COMMENT "#")
endif(NOT PLD_pdfcairo)

if(NOT PLD_pngcairo)
  set(PNGCAIRO_COMMENT "#")
endif(NOT PLD_pngcairo)

if(NOT PLD_pscairo)
  set(PSCAIRO_COMMENT "#")
endif(NOT PLD_pscairo)

if(NOT PLD_svgcairo)
  set(SVGCAIRO_COMMENT "#")
endif(NOT PLD_svgcairo)

if(NOT PLD_gif)
  set(GIF_COMMENT "#")
endif(NOT PLD_gif)

if(NOT PLD_jpeg)
  set(JPEG_COMMENT "#")
endif(NOT PLD_jpeg)

if(NOT PLD_png)
  set(PNG_COMMENT "#")
endif(NOT PLD_png)

if(NOT PLD_psttf)
  set(PSTTFC_COMMENT "#")
endif(NOT PLD_psttf)

if(NOT PLD_svg)
  set(SVG_COMMENT "#")
endif(NOT PLD_svg)

if(NOT PLD_xfig)
  set(XFIG_COMMENT "#")
endif(NOT PLD_xfig)

if(NOT PLD_pstex)
  set(PSTEX_COMMENT "#")
endif(NOT PLD_pstex)

if(NOT DIFF_EXECUTABLE OR NOT TAIL_EXECUTABLE)
  set(COMPARE_COMMENT "#")
endif(NOT DIFF_EXECUTABLE OR NOT TAIL_EXECUTABLE)

if(NOT PLD_cgm)
  set(CGM_COMMENT "#")
endif(NOT PLD_cgm)

if(NOT PLD_bmpqt)
  set(BMPQT_COMMENT "#")
endif(NOT PLD_bmpqt)

if(NOT PLD_jpgqt)
  set(JPGQT_COMMENT "#")
endif(NOT PLD_jpgqt)

if(NOT PLD_pngqt)
  set(PNGQT_COMMENT "#")
endif(NOT PLD_pngqt)

if(NOT PLD_ppmqt)
  set(PPMQT_COMMENT "#")
endif(NOT PLD_ppmqt)

if(NOT PLD_tiffqt)
  set(TIFFQT_COMMENT "#")
endif(NOT PLD_tiffqt)

if(NOT PLD_svgqt)
  set(SVGQT_COMMENT "#")
endif(NOT PLD_svgqt)

if(NOT PLD_epsqt)
  set(EPSQT_COMMENT "#")
endif(NOT PLD_epsqt)

if(NOT PLD_pdfqt)
  set(PDFQT_COMMENT "#")
endif(NOT PLD_pdfqt)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
  ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
  @ONLY
  )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
  DESTINATION ${DATA_DIR}/examples
  RENAME Makefile
  )
