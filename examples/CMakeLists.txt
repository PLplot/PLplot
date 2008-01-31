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

install(FILES Makefile.examples 
DESTINATION ${DATA_DIR}/examples
RENAME Makefile
)
