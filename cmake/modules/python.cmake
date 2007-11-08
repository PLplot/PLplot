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
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_python "Enable Python bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_python "Enable Python bindings" ON)
endif(DEFAULT_NO_BINDINGS)

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

option(HAVE_NUMPY "Use numpy rather than deprecated Numeric" ON)

if(ENABLE_python AND NOT NUMERIC_INCLUDE_PATH)
  if(HAVE_NUMPY)
    # First check for new version of numpy (replaces Numeric)
    execute_process(
    COMMAND
    ${PYTHON_EXECUTABLE} -c "import numpy; print numpy.get_include()"
    OUTPUT_VARIABLE NUMPY_INCLUDE_PATH
    RESULT_VARIABLE NUMPY_ERR
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if(NUMPY_ERR)
      set(HAVE_NUMPY OFF CACHE BOOL "Use numpy rather than deprecated Numeric" FORCE)
    endif(NUMPY_ERR)
  endif(HAVE_NUMPY)
  if(HAVE_NUMPY)
    # Set include path to find numpy headers
    set(NUMERIC_INCLUDE_PATH ${NUMPY_INCLUDE_PATH}/numpy CACHE PATH "Path to python Numeric/numpy include files")
    set(PYTHON_NUMERIC_NAME numpy CACHE INTERNAL "")
  else(HAVE_NUMPY)
    # Check for Python Numeric header in same include path or Numeric
    # subdirectory of that path to avoid version mismatch.
    find_path(
    NUMERIC_INCLUDE_PATH
    arrayobject.h
    ${PYTHON_INCLUDE_PATH} ${PYTHON_INCLUDE_PATH}/Numeric
    )
    if (NUMERIC_INCLUDE_PATH)
      set(PYTHON_NUMERIC_NAME Numeric CACHE INTERNAL "")
    endif (NUMERIC_INCLUDE_PATH)
  endif(HAVE_NUMPY)
  if(NUMERIC_INCLUDE_PATH)
    set(
    PYTHON_INCLUDE_PATH
    ${PYTHON_INCLUDE_PATH} ${NUMERIC_INCLUDE_PATH}
    CACHE INTERNAL "")
  else(NUMERIC_INCLUDE_PATH)
    if(HAVE_NUMPY)
      message(STATUS "WARNING: "
      "NumPy header not found. Disabling python bindings")
    else(HAVE_NUMPY)
      message(STATUS "WARNING: "
      "Numeric header not found. Disabling python bindings")
    endif(HAVE_NUMPY)
    set(ENABLE_python OFF CACHE BOOL "Enable Python bindings" FORCE)
  endif(NUMERIC_INCLUDE_PATH)
endif(ENABLE_python AND NOT NUMERIC_INCLUDE_PATH)

if(ENABLE_python)
  # N.B. This is a nice way to obtain all sorts of python information
  # using distutils.
  execute_process(
  COMMAND
  ${PYTHON_EXECUTABLE} -c "from distutils import sysconfig; print sysconfig.get_python_lib(1,0,prefix='${CMAKE_INSTALL_EXEC_PREFIX}')"
  OUTPUT_VARIABLE PYTHON_INSTDIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
endif(ENABLE_python)
