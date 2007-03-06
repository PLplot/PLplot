# cmake/modules/java.cmake
#
# Java binding configuration
#
# Copyright (C) 2006  Andrew Ross
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

# Module for determining Java bindings configuration options

# Options to enable Java bindings
if(DEFAULT_NO_BINDINGS)
  option(ENABLE_java "Enable Java bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_java "Enable Java bindings" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_java AND NOT BUILD_SHARED_LIBS)
  message(STATUS "WARNING: "
    "Java requires shared libraries. Disabling java bindings")
  set(ENABLE_java OFF CACHE BOOL "Enable Java bindings" FORCE)
endif(ENABLE_java AND NOT BUILD_SHARED_LIBS)

if(ENABLE_java AND NOT SWIG_FOUND)
  message(STATUS "WARNING: "
    "swig not found. Disabling java bindings")
  set(ENABLE_java OFF CACHE BOOL "Enable Java bindings" FORCE)
endif(ENABLE_java AND NOT SWIG_FOUND)

if(ENABLE_java)
  # Check for java compiler
  include(CMakeDetermineJavaCompiler)
  if(NOT CMAKE_Java_COMPILER)
    message(STATUS "WARNING: "
      "java compiler not found. Disabling java bindings")
    set(ENABLE_java OFF CACHE BOOL "Enable Java bindings" FORCE)
  endif(NOT CMAKE_Java_COMPILER)
endif(ENABLE_java)

if(ENABLE_java)
  # Check for java environment
  enable_language(Java)
  find_package(JNI)
  # If CMake doesn't find jni.h you need set CMAKE_INCLUDE_PATH
  if(NOT JAVA_INCLUDE_PATH)
    message(STATUS
      "WARNING: jni.h header not found. Disabling Java bindings.\n" 
    "   Please install that header and/or set the environment variable\n"
    "   CMAKE_INCLUDE_PATH appropriately."
    )
    set(ENABLE_java OFF CACHE BOOL "Enable Java bindings" FORCE)
  endif(NOT JAVA_INCLUDE_PATH)
endif(ENABLE_java)

if(ENABLE_java)
  # Set up installation locations for java specific files.
  # Java .jar files.
  set(JAR_DIR ${CMAKE_INSTALL_DATADIR}/java)
  get_filename_component(JAVADATA_HARDDIR ${JAR_DIR} ABSOLUTE)
  # JNI .so files.
  set(JAVAWRAPPER_DIR ${LIB_DIR}/jni)
  get_filename_component(JAVAWRAPPER_HARDDIR ${JAVAWRAPPER_DIR} ABSOLUTE)
  # Library name suffix is system dependent.  Tests have shown
  # that ${CMAKE_SHARED_LIBRARY_SUFFIX} gives the correct suffix on both
  # Linux and windows, and from CMAKEPREFIX/share/cmake-2.4/Modules/Platform
  # it appears this CMake variable is defined on Mac OS X and other platforms.
  set(PLPLOTJAVAC_WRAP_DLL plplotjavac_wrap${CMAKE_SHARED_LIBRARY_SUFFIX})
endif(ENABLE_java)
