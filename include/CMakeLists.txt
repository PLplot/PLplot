# include/CMakeLists.txt for PLplot
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

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/plConfig.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/plConfig.h
  )

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/plDevs.h.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/plDevs.h
  )

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pldll.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/pldll.h
  )

set(plhershey-unicode-gen_SRCS
  ${CMAKE_SOURCE_DIR}/fonts/plhershey-unicode-gen.c
  )

if(NOT CMAKE_CROSSCOMPILING)
  add_executable(plhershey-unicode-gen ${plhershey-unicode-gen_SRCS})
else(NOT CMAKE_CROSSCOMPILING)
  SET(IMPORT_EXECUTABLES ${CMAKE_NATIVE_BINARY_DIR}/include/ImportExecutables.cmake)
  INCLUDE(${IMPORT_EXECUTABLES})
endif(NOT CMAKE_CROSSCOMPILING)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plhershey-unicode.h
  COMMAND plhershey-unicode-gen
  ${CMAKE_SOURCE_DIR}/fonts/plhershey-unicode.csv
  ${CMAKE_CURRENT_BINARY_DIR}/plhershey-unicode.h
  DEPENDS
  plhershey-unicode-gen
  ${CMAKE_SOURCE_DIR}/fonts/plhershey-unicode.csv
  )

if(NOT CMAKE_CROSSCOMPILING) 
  export(TARGETS plhershey-unicode-gen FILE ${CMAKE_CURRENT_BINARY_DIR}/ImportExecutables.cmake )
endif(NOT CMAKE_CROSSCOMPILING) 

# For cross-directory dependencies....
add_custom_target(
  plhershey-unicode.h_built
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/plhershey-unicode.h
  )

set(include_INSTALLED_HEADERS
  disptab.h
  drivers.h
  pdf.h
  pldebug.h
  ${CMAKE_CURRENT_BINARY_DIR}/pldll.h
  plevent.h
  plplot.h
  plplotP.h
  plstrm.h
  plxwd.h
  gcw.h
  plplotcanvas.h
  ${CMAKE_CURRENT_BINARY_DIR}/plConfig.h
  ${CMAKE_CURRENT_BINARY_DIR}/plDevs.h
  )

#qt.h only useful if anyone wants to build a Qt application with the Qt widget driver
if(PLD_qtwidget OR PLD_extqt)
  list(APPEND include_INSTALLED_HEADERS qt.h)
endif(PLD_qtwidget OR PLD_extqt)

if(ANY_QT_DEVICE)
  set(MOC_OPTIONS)
  if(PLD_bmpqt)
    list(APPEND MOC_OPTIONS -DPLD_bmpqt)
  endif(PLD_bmpqt)
  if(PLD_jpgqt)
    list(APPEND MOC_OPTIONS -DPLD_jpgqt)
  endif(PLD_jpgqt)
  if(PLD_pngqt)
    list(APPEND MOC_OPTIONS -DPLD_pngqt)
  endif(PLD_pngqt)
  if(PLD_ppmqt)
    list(APPEND MOC_OPTIONS -DPLD_ppmqt)
  endif(PLD_ppmqt)
  if(PLD_tiffqt)
    list(APPEND MOC_OPTIONS -DPLD_tiffqt)
  endif(PLD_tiffqt)
  if(PLD_svgqt)
    list(APPEND MOC_OPTIONS -DPLD_svgqt)
  endif(PLD_svgqt)
  if(PLD_epsqt)
    list(APPEND MOC_OPTIONS -DPLD_epsqt)
  endif(PLD_epsqt)
  if(PLD_pdfqt)
    list(APPEND MOC_OPTIONS -DPLD_pdfqt)
  endif(PLD_pdfqt)
  if(PLD_qtwidget)
    list(APPEND MOC_OPTIONS -DPLD_qtwidget)
  endif(PLD_qtwidget)
  if(PLD_extqt)
    list(APPEND MOC_OPTIONS -DPLD_extqt)
  endif(PLD_extqt)
  if(MOC_OPTIONS)
    # This creates a custom command with OUTPUT ${QT_MOC_OUTFILES}.
    qt4_wrap_cpp(
      QT_MOC_OUTFILES
      ${CMAKE_SOURCE_DIR}/include/qt.h
      OPTIONS ${MOC_OPTIONS}
      )
    # Use custom target to force correct target dependencies in
    # various directories.  This target is used in three possible
    # places (qt_gui, drivers, or src) depending on build options.
    # The CMake dependency parser creates the necessary file
    # dependency on qt.h for all qt-related source code so there is no
    # need to add the ${QT_MOC_OUTFILES} file dependencies for any
    # target dependency on moc_outfiles_generated.
    add_custom_target(moc_outfile_generated DEPENDS ${QT_MOC_OUTFILES})

    # #include ${QT_MOC_OUTFILES}) indirectly via the configured moc_files.h
    # header.
    configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/moc_files.h.in
      ${CMAKE_CURRENT_BINARY_DIR}/moc_files.h
      )
  else(MOC_OPTIONS)
    message(FATAL_ERROR "Internal build system inconsistency between ANY_QT_DEVICE true and MOC_OPTIONS false.")
  endif(MOC_OPTIONS)
endif(ANY_QT_DEVICE)

install(FILES ${include_INSTALLED_HEADERS} DESTINATION ${INCLUDE_DIR})
