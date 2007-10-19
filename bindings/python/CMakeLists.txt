# bindings/python/CMakeLists.txt
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

if(ENABLE_python)

# Configure the python scripts to use the correct version of the Numeric library
if (HAVE_NUMPY)
  configure_file(plplot.py.numpy ${CMAKE_CURRENT_BINARY_DIR}/plplot.py COPYONLY)
else (HAVE_NUMPY)
  configure_file(plplot.py.Numeric ${CMAKE_CURRENT_BINARY_DIR}/plplot.py COPYONLY)
endif (HAVE_NUMPY)


# This is currently the include list for swig, the C wrapper and the
# the Python headers. Not particular pretty...
set(python_interface_INCLUDE_PATHS
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
${CMAKE_CURRENT_BINARY_DIR}
${PYTHON_INCLUDE_PATH}
${CMAKE_SOURCE_DIR}/bindings/swig-support
)
include_directories(${python_interface_INCLUDE_PATHS})

set(CMAKE_SWIG_FLAGS -DPL_DOUBLE -DSWIG_PYTHON -python)

set(CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR})

# Set up swig + c wrapper.
# N.B. the python target has an underscore prepended automatically.
swig_add_module(plplotcmodule python plplotcmodule.i)

swig_link_libraries(plplotcmodule plplot${LIB_TAG} ${PYTHON_LIBRARIES})

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
  set_target_properties(
  _plplotcmodule
  PROPERTIES
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  )
endif(USE_RPATH)

add_library(plplot_widgetmodule MODULE plplot_widgetmodule.c)
set_target_properties(plplot_widgetmodule PROPERTIES PREFIX "")
target_link_libraries(plplot_widgetmodule plplot${LIB_TAG} ${PYTHON_LIBRARIES})
if(USE_RPATH)
  set_target_properties(
  plplot_widgetmodule
  PROPERTIES
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
  )
else(USE_RPATH)
  set_target_properties(
  plplot_widgetmodule
  PROPERTIES
  INSTALL_NAME_DIR "${PYTHON_INSTDIR}"
  )
endif(USE_RPATH)

install(
TARGETS plplot_widgetmodule _plplotcmodule 
LIBRARY 
DESTINATION ${PYTHON_INSTDIR}
)

set(PERM_SCRIPTS
OWNER_READ
OWNER_WRITE
OWNER_EXECUTE
GROUP_READ
GROUP_EXECUTE
WORLD_READ
WORLD_EXECUTE
)
install(
FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot.py ${CMAKE_CURRENT_BINARY_DIR}/plplotc.py
DESTINATION ${PYTHON_INSTDIR}
)

endif(ENABLE_python)
