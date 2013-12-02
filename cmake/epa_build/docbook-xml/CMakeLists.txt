# docbook-xml/CMakeLists.txt
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build of docbook-xml.

# Copyright (C) 2013 Alan W. Irwin

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this file; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

# The top-level CMakeLists.txt file that includes this file should
# have run "include(ExternalProject)" and set EPA_BASE variable (and
# corresponding directory property) as well as various EPA variables
# used below that configure how the External_Project functions
# operate.

# Protect against configuring a build twice in one CMake call
if(docbook-xml_configured)
  return()
endif(docbook-xml_configured)
set(docbook-xml_configured ON)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(EPA_ignored_dependencies_LIST pkg-config;bison;flex;python2-devel;libXft ${extra_ignored_dependencies_list})

set(docbook-xml_dependencies_LIST libxml2)
# Remove dependencies that should be ignored.
if(docbook-xml_dependencies_LIST)
  list(REMOVE_ITEM docbook-xml_dependencies_LIST ${EPA_ignored_dependencies_LIST})
endif(docbook-xml_dependencies_LIST)

set(docbook-xml_dependencies_targets)
foreach(build_configuration ${docbook-xml_dependencies_LIST})
  if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    include(${build_configuration}/CMakeLists.txt)
    list(APPEND docbook-xml_dependencies_targets build_${build_configuration})
  else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of docbook-xml has been installed another way.")
  endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
endforeach(build_configuration ${docbook-xml_dependences_LIST})

# This can be safely done only after above includes.
set(PACKAGE docbook-xml)

# Data that is related to downloads.
set(URL http://www.docbook.org/xml/4.5/docbook-xml-4.5.zip)
set(${PACKAGE}_DOWNLOAD_HASH_TYPE MD5)
set(${PACKAGE}_DOWNLOAD_HASH 03083e288e87a7e829e437358da7ef9e)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(EPA_PATH_NODLL "${EPA_PATH}")
  #set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  set(${PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
else(MSYS_PLATFORM)
  set(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  set(${PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

ExternalProject_Add(
  build_${PACKAGE}
  DEPENDS ${${PACKAGE}_dependencies_targets}
  URL ${URL}
  URL_HASH ${${PACKAGE}_DOWNLOAD_HASH_TYPE}=${${PACKAGE}_DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_CMAKE_COMMAND} ${EPA_BASE}/Source/build_${PACKAGE}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND}
  BUILD_IN_SOURCE OFF
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND} install
  )

# Add custom commands to the current no-command update step.
add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-update
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_SOURCE_DIR}/${PACKAGE}/${PACKAGE}_CMakeLists.txt
  ${EPA_BASE}/Source/build_${PACKAGE}/CMakeLists.txt
  COMMENT "Custom updating of ${PACKAGE}"
  DEPENDS
  ${CMAKE_SOURCE_DIR}/${PACKAGE}/${PACKAGE}_CMakeLists.txt
  APPEND
  )

list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
