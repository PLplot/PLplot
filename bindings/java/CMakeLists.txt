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

if(ENABLE_java)

# Swig generated java files
set(SWIG_JAVA_FILES
  plplotjavacJNI.java 
  PLGraphicsIn.java 
  plplotjavacConstants.java
  plplotjavac.java
)

# Full list of generated java files
set(JAVA_GEN_FILES
 config.java
 ${SWIG_JAVA_FILES}
)

# List of generated java files with full path names
# Need this otherwise cmake will look in the source directory for the
# .java files.
string( REGEX REPLACE "([a-zA-z]*)\\.java" "${CMAKE_CURRENT_BINARY_DIR}/\\1.java" JAVA_GEN_FILES_FULL "${JAVA_GEN_FILES}" )

# Create config.java
configure_file(
${CMAKE_CURRENT_SOURCE_DIR}/config.java.in
${CMAKE_CURRENT_BINARY_DIR}/config.java
)


# This is currently the include list for swig, the C wrapper and the
# the java classpath. Not particular pretty...
set(java_interface_INCLUDE_PATHS
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/include
${CMAKE_CURRENT_BINARY_DIR}
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
${JAVA_INCLUDE_PATH}
${CMAKE_SOURCE_DIR}/bindings/swig-support
${CMAKE_BINARY_DIR}/CMakeFiles/plplot.dir
)
# On some systems JAVA_INCLUDE_PATH2 returns JAVA_INCLUDE_PATH2-NOTFOUND
if(JAVA_INCLUDE_PATH2)
  set(java_interface_INCLUDE_PATHS
  ${java_interface_INCLUDE_PATHS}
  ${JAVA_INCLUDE_PATH2}
  )
endif(JAVA_INCLUDE_PATH2)
include_directories(${java_interface_INCLUDE_PATHS})

# Can't use source file properties as we have to quote the flags in that 
# case and it breaks swig. Doh! I would call this a cmake bug.
set(CMAKE_SWIG_FLAGS -DPL_DOUBLE -DSWIG_JAVA -package plplot.core)

set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR} )

set_source_files_properties(${JAVA_GEN_FILES_FULL} PROPERTIES GENERATED ON)

# Set up swig + c wrapper
swig_add_module( plplotjavac_wrap java plplotjavac.i )
swig_link_libraries( plplotjavac_wrap plplot${LIB_TAG} )

# Create java library
add_library(plplot STATIC ${JAVA_GEN_FILES_FULL} PLStream.java )

# Ensure that swig is excecuted before we try to compile the java
# classes.
add_dependencies(plplot plplotjavac_wrap)

# Java compiler doesn't support -D option.
remove_definitions("-DHAVE_CONFIG_H")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/plplot.jar DESTINATION ${JAR_DIR})

endif(ENABLE_java)

