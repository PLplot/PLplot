# bindings/java/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
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

IF(ENABLE_java)

# Swig generated java files
SET(SWIG_JAVA_FILES
  plplotjavacJNI.java 
  PLGraphicsIn.java 
  plplotjavacConstants.java
  plplotjavac.java
)

# Full list of generated java files
SET(JAVA_GEN_FILES
 config.java
 ${SWIG_JAVA_FILES}
)

# List of generated java files with full path names
# Need this otherwise cmake will look in the source directory for the
# .java files.
STRING( REGEX REPLACE "([a-zA-z]*)\\.java" "${CMAKE_CURRENT_BINARY_DIR}/\\1.java" JAVA_GEN_FILES_FULL "${JAVA_GEN_FILES}" )

# Create config.java
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/config.java.in
${CMAKE_CURRENT_BINARY_DIR}/config.java
)


# This is currently the include list for swig, the C wrapper and the
# the java classpath. Not particular pretty...
INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_BINARY_DIR}
  ${CMAKE_BINARY_DIR}/include
  ${JAVA_INCLUDE_PATH}
  ${JAVA_INCLUDE_PATH2}
  ${CMAKE_SOURCE_DIR}/bindings/swig-support
  ${CMAKE_BINARY_DIR}/CMakeFiles/plplot.dir
)

# Can't use source file properties as we have to quote the flags in that 
# case and it breaks swig. Doh! I would call this a cmake bug.
SET(CMAKE_SWIG_FLAGS -DPL_DOUBLE -DSWIG_JAVA -package plplot.core)

SET( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR} )

SET_SOURCE_FILES_PROPERTIES(${JAVA_GEN_FILES_FULL} PROPERTIES GENERATED ON)

# Set up swig + c wrapper
SWIG_ADD_MODULE( plplotjavac_wrap java plplotjavac.i )
SWIG_LINK_LIBRARIES( plplotjavac_wrap plplot${LIB_TAG} )

# Create java library
ADD_LIBRARY(plplot STATIC ${JAVA_GEN_FILES_FULL} PLStream.java )

# Ensure that swig is excecuted before we try to compile the java
# classes.
ADD_DEPENDENCIES(plplot plplotjavac_wrap)

# Java compiler doesn't support -D option.
REMOVE_DEFINITIONS("-DHAVE_CONFIG_H")

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot.jar DESTINATION ${JAR_DIR})

ENDIF(ENABLE_java)

