# cmake/modules/python.cmake
#
# Copyright (C) 2006-2026  Alan W. Irwin
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

# Module for determining Python binding configuration options

# Options to enable Python binding
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_python "Enable Python binding" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_python "Enable Python binding" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_python AND NOT SED_EXECUTABLE)
  message(STATUS "WARNING: "
    "Python binding fixup requires sed which was not found. Disabling Python binding")
  set(ENABLE_python OFF CACHE BOOL "Enable Python binding" FORCE)
endif(ENABLE_python AND NOT SED_EXECUTABLE)

if(ENABLE_python AND NOT BUILD_SHARED_LIBS)
  message(STATUS "WARNING: "
    "Python requires shared libraries. Disabling Python binding")
  set(ENABLE_python OFF CACHE BOOL "Enable Python binding" FORCE)
endif(ENABLE_python AND NOT BUILD_SHARED_LIBS)

if(ENABLE_python AND NOT SWIG_FOUND)
  message(STATUS "WARNING: "
    "swig not found. Disabling Python binding")
  set(ENABLE_python OFF CACHE BOOL "Enable Python binding" FORCE)
endif(ENABLE_python AND NOT SWIG_FOUND)

if(ENABLE_python)
  find_package(Python3 COMPONENTS Interpreter Development NumPy)
  if(NOT Python3_FOUND)
    message(STATUS "WARNING: "
      "Some/all of Interpreter Development NumPy Python3 components not found. Disabling Python binding")
    set(ENABLE_python OFF CACHE BOOL "Enable Python binding" FORCE)
  endif(NOT Python3_FOUND)
endif(ENABLE_python)

if(ENABLE_python)
  # Success means Python3_FOUND is true.  In this case the subset of
  # Python3 targets and variables that *could be* relevant to PLplot
  # are the Python3::Interpreter, Python3::Module (Python3 library),
  # Python3::NumPy (Numpy library for Python 3) targets, and (in
  # addition to Python3_FOUND), the Python3_EXECUTABLE (full pathname
  # consistent with the Python3::Interpreter target where
  # Python3_EXECUTABLE can be used in contexts where
  # $<TARGET_FILE:Python3::Interpreter> cannot be used),
  # Python3_SITEARCH (result of sysconfig.get_path('platlib'))
  # directory), Python3_SOABI (extension suffix for modules),
  # Python3_INCLUDE_DIRS, Python3_LIBRARIES, Python3_LIBRARY_DIRS,
  # Python3_VERSION, Python3_NumPy_INCLUDE_DIRS, and
  # Python3_NumPy_VERSION variables.  N.B. If Linking with the
  # Python3::Module and Python3::NumPy imported library targets is
  # done correctly, then many of the above variables become
  # irrelevant, i.e., the list of python-related variables used by our
  # build system should shrink to
  # Python3_FOUND, Python3_EXECUTABLE, Python3_SITEARCH, Python3_SOABI,
  # Python3_VERSION, and Python3_NumPy_VERSION.

  if(CMAKE_INSTALL_EXEC_PREFIX)
    set(PYTHON_INSTALL_TEMPLATE "${CMAKE_INSTALL_EXEC_PREFIX}")
  else(CMAKE_INSTALL_EXEC_PREFIX)
    # if CMAKE_INSTALL_EXEC_PREFIX is an empty string (which I believe can
    # happen if the user forgot to specify CMAKE_INSTALL_PREFIX), must replace
    # it with "/" to make PYTHON_INSTALL_TEMPLATE an absolute path to be
    # consistent with all other installation paths for this case.
    set(PYTHON_INSTALL_TEMPLATE "/")
  endif(CMAKE_INSTALL_EXEC_PREFIX)

  # Determine cache variable PYTHON_INSTDIR by replacing
  # Python3_SITEARCH prefix with PLplot installation prefix.
  string(REGEX REPLACE "^.*/python" "${PYTHON_INSTALL_TEMPLATE}/lib/python" _python_instdir "${Python3_SITEARCH}")
  set(PYTHON_INSTDIR
    "${_python_instdir}"
    CACHE PATH "PLplot install location for Python extension modules"
  )

  list(APPEND INSTALL_LOCATION_VARIABLES_LIST PYTHON_INSTDIR)

  # This should always work for modern swig (version >= 1.3.38)
  # and Python3 (any version).
  set(PYTHON_HAVE_PYBUFFER ON)
endif(ENABLE_python)
