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
OPTION(ENABLE_java "Enable Java bindings" ON)

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

IF (ENABLE_java)

# Check for java compiler
INCLUDE(CMakeDetermineJavaCompiler)

IF (CMAKE_Java_COMPILER MATCHES "NOTFOUND$")
  MESSAGE(STATUS "WARNING: "
    "java compiler not found. Disabling java bindings")
  SET(ENABLE_java OFF CACHE BOOL "Enable Java bindings" FORCE)
ENDIF (CMAKE_Java_COMPILER MATCHES "NOTFOUND$")

ENDIF (ENABLE_java)

IF (ENABLE_java)

# Check for java environment
ENABLE_LANGUAGE(Java)

FIND_PACKAGE(JNI)

# If CMake doesn't find jni.h you need set JAVA_INCLUDE_PATH
IF( "${JAVA_INCLUDE_PATH}" MATCHES "NOTFOUND$" )
  MESSAGE(STATUS
      "WARNING: jni.h header not found. Disabling Java bindings.\n" 
  "   Please install that header and/or set the environment variable\n"
  "   CMAKE_INCLUDE_PATH appropriately."
  )
  SET(ENABLE_java OFF CACHE BOOL "Enable Java bindings" FORCE)
ENDIF( "${JAVA_INCLUDE_PATH}" MATCHES "NOTFOUND$" )

# Set up installation locations for java specific files.
# Java .jar files.
set(JAR_INSTALL_DIR share/java)
set(JAR_DIR ${CMAKE_INSTALL_PREFIX}/${JAR_INSTALL_DIR})
get_filename_component(JAVADATA_HARDDIR ${JAR_DIR} ABSOLUTE)
# JNI .so files.
SET(JAVAWRAPPER_DIR ${LIB_DIR}/jni)
GET_FILENAME_COMPONENT(JAVAWRAPPER_HARDDIR ${JAVAWRAPPER_DIR} ABSOLUTE)
# Library name will be system dependent. For now assume .so
# We're not currently using libtool so we can't use the autotools
# trick to guess. 
# This will definitely need changing for windows.
SET(PLPLOTJAVAC_WRAP_DLL plplotjavac_wrap.so)

ENDIF (ENABLE_java)


