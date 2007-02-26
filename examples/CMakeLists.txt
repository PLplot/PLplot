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
