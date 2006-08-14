# cmake/modules/python.cmake
#
# Copyright (C) 2006  Alan W. Irwin
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
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Module for determining Python bindings configuration options

# Options to enable Python bindings
option(ENABLE_python "Enable Python bindings" ON)

if(ENABLE_python AND NOT BUILD_SHARED_LIBS)
  message(STATUS "WARNING: "
    "Python requires shared libraries. Disabling python bindings")
  set(ENABLE_python OFF CACHE BOOL "Enable Python bindings" FORCE)
endif(ENABLE_python AND NOT BUILD_SHARED_LIBS)

if(ENABLE_python AND NOT SWIG_FOUND)
  message(STATUS "WARNING: "
    "swig not found. Disabling python bindings")
  set(ENABLE_python OFF CACHE BOOL "Enable Python bindings" FORCE)
endif(ENABLE_python AND NOT SWIG_FOUND)

if(ENABLE_python)
  # Check for Python interpreter (which also defines PYTHON_EXECUTABLE)
  find_package(PythonInterp)
  if(NOT PYTHONINTERP_FOUND)
    message(STATUS "WARNING: "
    "python interpreter not found. Disabling python bindings")
    set(ENABLE_python OFF CACHE BOOL "Enable Python bindings" FORCE)
  endif(NOT PYTHONINTERP_FOUND)
endif(ENABLE_python)

if(ENABLE_python)
  # Check for Python libraries which defines
  #  PYTHON_LIBRARIES     = path to the python library
  #  PYTHON_INCLUDE_PATH  = path to where Python.h is found
  find_package(PythonLibs)
  if(NOT PYTHON_LIBRARIES OR NOT PYTHON_INCLUDE_PATH)
    message(STATUS "WARNING: "
    "python library and/or header not found. Disabling python bindings")
    set(ENABLE_python OFF CACHE BOOL "Enable Python bindings" FORCE)
  endif(NOT PYTHON_LIBRARIES OR NOT PYTHON_INCLUDE_PATH)
endif(ENABLE_python)

if(ENABLE_python)
  # Check for Python Numeric header in same include path or Numeric
  # subdirectory of that path to avoid version mismatch.
  find_path(
  NUMERIC_INCLUDE_PATH
  arrayobject.h
  ${PYTHON_INCLUDE_PATH} ${PYTHON_INCLUDE_PATH}/Numeric
  )
  if(NUMERIC_INCLUDE_PATH)
    set(
    PYTHON_INCLUDE_PATH
    ${PYTHON_INCLUDE_PATH} ${NUMERIC_INCLUDE_PATH}
    CACHE INTERNAL "")
  else(NUMERIC_INCLUDE_PATH)
    message(STATUS "WARNING: "
    "Numeric header not found. Disabling python bindings")
    set(ENABLE_python OFF CACHE BOOL "Enable Python bindings" FORCE)
  endif(NUMERIC_INCLUDE_PATH)
endif(ENABLE_python)

if(ENABLE_python)
  # N.B. This is a nice way to obtain all sorts of python information
  # using distutils.
  set(
  PYTHON_ARG_STRING
  "-c \"from distutils import sysconfig\;"
  "print sysconfig.get_python_lib(0,0,prefix='${CMAKE_INSTALL_PREFIX}')\""
  )
  exec_program(
  ${PYTHON_EXECUTABLE}
  ARGS ${PYTHON_ARG_STRING}
  OUTPUT_VARIABLE PYTHON_INSTDIR
  )
endif(ENABLE_python)
