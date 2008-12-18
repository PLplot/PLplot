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

# Configure Makefile.examples with some specific variables

if(PLD_ps)
  set(PSC_ALL x01c.psc)
else(PLD_ps)
  set(PSC_ALL_COMMENT "#")
endif(PLD_ps)

if(PLD_pdfcairo)
  set(PDFCAIRO x01c.pdfcairo)
else(PLD_pdfcairo)
  set(PDFCAIRO_COMMENT "#")
endif(PLD_pdfcairo)

if(PLD_pngcairo)
  set(PNGCAIRO x01c01.pngcairo)
else(PLD_pngcairo)
  set(PNGCAIRO_COMMENT "#")
endif(PLD_pngcairo)

if(PLD_pscairo)
  set(PSCAIRO x01c.pscairo)
else(PLD_pscairo)
  set(PSCAIRO_COMMENT "#")
endif(PLD_pscairo)

if(PLD_svgcairo)
  set(SVGCAIRO x01c01.svgcairo)
else(PLD_svgcairo)
  set(SVGCAIRO_COMMENT "#")
endif(PLD_svgcairo)

if(PLD_gif)
  set(GIF x01c01.gif)
else(PLD_gif)
  set(GIF_COMMENT "#")
endif(PLD_gif)

if(PLD_jpeg)
  set(JPEG x01c01.jpeg)
else(PLD_jpeg)
  set(JPEG_COMMENT "#")
endif(PLD_jpeg)

if(PLD_png)
  set(PNG x01c01.png)
else(PLD_png)
  set(PNG_COMMENT "#")
endif(PLD_png)

if(PLD_psttf)
  set(PSTTFC x01c.psttfc)
else(PLD_psttf)
  set(PSTTFC_COMMENT "#")
endif(PLD_psttf)

if(PLD_svg)
  set(SVG x01c01.svg)
else(PLD_svg)
  set(SVG_COMMENT "#")
endif(PLD_svg)

if(PLD_xfig)
  set(XFIG x01c01.xfig)
else(PLD_xfig)
  set(XFIG_COMMENT "#")
endif(PLD_xfig)

if(DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
  set(COMPARE compare)
else(DIFF_EXECUTABLE AND TAIL_EXECUTABLE)
  set(COMPARE_COMMENT "#")
endif(DIFF_EXECUTABLE AND TAIL_EXECUTABLE)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Makefile.examples.in
  ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples
  @ONLY
  )

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Makefile.examples 
DESTINATION ${DATA_DIR}/examples
RENAME Makefile
)
