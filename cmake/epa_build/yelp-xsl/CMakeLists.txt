# yelp-xsl/CMakeLists.txt
# N.B. this file is generated so if you edit it you will lose all your
# changes the next time it is generated (typically by running
# either/both the update_added_packages.sh or update_pango_packages.sh
# scripts).  If those scripts do not provide good results, then
# consider changing their source files (e.g., by editing the files
# used in those scripts) or add a patch to be run by those scripts.

# The CMake logic in this file should be exercised by the
# add_subdirectory command that is exercised in the top-level
# CMakeLists.txt file to configure the build of yelp-xsl.

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

# The top-level CMakeLists.txt file that adds this subdirectory should
# have run "include(ExternalProject)" and set the EPA_BASE variable (and
# corresponding directory property) as well as various EPA variables
# used below that configure how the External_Project functions
# operate.

set(PACKAGE yelp-xsl)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(ignored_dependencies_LIST pkg-config;bison;flex;python2-devel;libXft ${extra_ignored_dependencies_list})

set(dependencies_LIST intltool;itstool;libxml2;libxslt)
# Remove dependencies that should be ignored.
if(dependencies_LIST)
  list(REMOVE_ITEM dependencies_LIST ${ignored_dependencies_LIST})
endif(dependencies_LIST)

# Add this subdirectory to the list of subdirectories that have
# already been processed by the top-level CMakeLists.txt logic.
list(APPEND EPA_processed_subdirectories_LIST ${PACKAGE})

set(dependencies_targets)
foreach(build_configuration ${dependencies_LIST})
  if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    # Add to the list of subdirectories that must be processed by the
    # top-level CMakeLists.txt logic.
    list(APPEND EPA_needed_subdirectories_LIST ${build_configuration})
    list(APPEND dependencies_targets build_${build_configuration})
  else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
    message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of ${PACKAGE} has been installed on your platform by means other than epa_build.")
  endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/CMakeLists.txt)
endforeach(build_configuration ${dependences_LIST})

list(APPEND EPA_build_target_LIST build_${PACKAGE})

# Propagate list changes to parent scope.
set(EPA_build_target_LIST ${EPA_build_target_LIST} PARENT_SCOPE)
set(EPA_processed_subdirectories_LIST ${EPA_processed_subdirectories_LIST} PARENT_SCOPE)
set(EPA_needed_subdirectories_LIST ${EPA_needed_subdirectories_LIST} PARENT_SCOPE)

# Data that is related to downloads.
set(URL http://download.gnome.org/sources/yelp-xsl/3.8/yelp-xsl-3.8.1.tar.xz)
set(DOWNLOAD_HASH_TYPE SHA256)
set(DOWNLOAD_HASH afc519be07ffcbdcc20d1acfd1d901cfd03c7b4cf0667036534f2726bb68b40d)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  determine_msys_path(EPA_PATH "${EPA_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  set(SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
else(MSYS_PLATFORM)
  set(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
  set(SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

set(SET_CXXFLAGS "CXXFLAGS=$ENV{CXXFLAGS}")

# Drop -fvisibility=hidden since that option does not work for a
# number of software packages that are configured automatically using
# this template.
string(REGEX REPLACE "-fvisibility=hidden" "" SET_CFLAGS "${SET_CFLAGS}")
string(REGEX REPLACE "-fvisibility=hidden" "" SET_CXXFLAGS "${SET_CXXFLAGS}")

ExternalProject_Add(
  build_${PACKAGE}
  DEPENDS ${dependencies_targets}
  URL ${URL}
  URL_HASH ${DOWNLOAD_HASH_TYPE}=${DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${SET_CFLAGS} ${SET_CXXFLAGS} ${source_PATH}/${EPA_CONFIGURE_COMMAND} 
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${BP_PARALLEL_MAKE_COMMAND} 
  BUILD_IN_SOURCE OFF
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${BP_PARALLEL_MAKE_COMMAND}  install
  )
