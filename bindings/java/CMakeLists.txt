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
# All source files
set(JAVA_FILES
 ${JAVA_GEN_FILES}
 PLStream.java
)

# List of generated java files with full path names
# Need this otherwise cmake will look in the source directory for the
# .java files.
string( REGEX REPLACE "([a-zA-z]*)\\.java" "${CMAKE_CURRENT_BINARY_DIR}/\\1.java" JAVA_GEN_FILES_FULL "${JAVA_GEN_FILES}" )

# Class list
# A manual copy of this is also maintained in examples/java/CMakeLists.java
# in order to get the dependencies right for plplot.jar
string( REGEX REPLACE "([a-zA-z]*)\\.java" "${CMAKE_CURRENT_BINARY_DIR}/plplot/core/\\1.class" JAVA_CLASSES "${JAVA_FILES}" )
set(JAVA_FILES_FULL
  ${JAVA_GEN_FILES_FULL}
  ${CMAKE_CURRENT_SOURCE_DIR}/PLStream.java
)

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
if(SWIG_JAVA_NOPGCPP)
  #This version of swig supports the -nopgcpp option
  set(CMAKE_SWIG_FLAGS -DPL_DOUBLE -DSWIG_JAVA -package plplot.core -nopgcpp)
else(SWIG_JAVA_NOPGCPP)
  set(CMAKE_SWIG_FLAGS -DPL_DOUBLE -DSWIG_JAVA -package plplot.core)
endif(SWIG_JAVA_NOPGCPP)

set( CMAKE_SWIG_OUTDIR ${CMAKE_CURRENT_BINARY_DIR} )

set_source_files_properties(${JAVA_GEN_FILES_FULL} PROPERTIES GENERATED ON)

# Set up swig + c wrapper
swig_add_module( plplotjavac_wrap java plplotjavac.i )
swig_link_libraries( plplotjavac_wrap plplot${LIB_TAG} )

if(USE_RPATH)
  get_target_property(LIB_INSTALL_RPATH plplot${LIB_TAG} INSTALL_RPATH)
  set_target_properties(
  plplotjavac_wrap
  PROPERTIES
  INSTALL_RPATH "${LIB_INSTALL_RPATH}"
  INSTALL_NAME_DIR "${JAVAWRAPPER_HARDDIR}"
  )
else(USE_RPATH)
  set_target_properties(
  plplotjavac_wrap
  PROPERTIES
  INSTALL_NAME_DIR "${JAVAWRAPPER_HARDDIR}"
  )
endif(USE_RPATH)

install(TARGETS plplotjavac_wrap LIBRARY DESTINATION ${JAVAWRAPPER_HARDDIR})

foreach( srcfile ${JAVA_FILES_FULL} )
  get_filename_component( fileroot ${srcfile} NAME_WE )
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/plplot/core/${fileroot}.class
    COMMAND ${CMAKE_Java_COMPILER} 
    -classpath ${CMAKE_CURRENT_BINARY_DIR} ${srcfile} -d ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${srcfile}
  )
endforeach( srcfile ${JAVA_FILES_FULL} )
add_custom_target(plplot_core ALL DEPENDS ${JAVA_CLASSES})

# Ensure that swig is excecuted before we try to compile the java
# classes.
add_dependencies(plplot_core plplotjavac_wrap)

# Java compiler doesn't support -D option.
remove_definitions("-DHAVE_CONFIG_H")

# Installed as part of the example/java directory
#install(FILES ${CMAKE_BINARY_DIR}/plplot.jar DESTINATION ${JAR_DIR})

endif(ENABLE_java)

