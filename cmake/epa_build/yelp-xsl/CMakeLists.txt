# yelp-xsl/bp.cmake
# N.B. this file is generated so if you edit it you will lose all your
# changes the next time it is generated (typically by running
# either/both the update_added_packages.sh or update_pango_packages.sh
# scripts).  If those scripts do not provide good results, then
# consider changing their source files (e.g., by editing the files
# used in those scripts) or add a patch to be run by those scripts).

# This file should be included directly or indirectly from a top-level
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

# The top-level CMakeLists.txt file that includes this file should
# have run "include(ExternalProject)" and set EP_BASE variable (and
# corresponding directory property) as well as various BP variables
# used below that configure how the External_Project functions
# operate.

# Protect against configuring a build twice in one CMake call
if(yelp-xsl_configured)
  return()
endif(yelp-xsl_configured)
set(yelp-xsl_configured ON)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(BP_ignored_dependencies_LIST pkg-config;bison;flex;python2-devel;libXft ${extra_ignored_dependencies_list})

set(yelp-xsl_dependencies_LIST intltool;itstool;libxml2;libxslt)
# Remove dependencies that should be ignored.
if(yelp-xsl_dependencies_LIST)
  list(REMOVE_ITEM yelp-xsl_dependencies_LIST ${BP_ignored_dependencies_LIST})
endif(yelp-xsl_dependencies_LIST)

set(yelp-xsl_dependencies_targets)
foreach(build_configuration ${yelp-xsl_dependencies_LIST})
  if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
    include(${build_configuration}/bp.cmake)
    list(APPEND yelp-xsl_dependencies_targets build_${build_configuration})
  else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
    message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of yelp-xsl has been installed another way.")
  endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
endforeach(build_configuration ${yelp-xsl_dependences_LIST})

# This can be safely done only after above includes.
set(BP_PACKAGE yelp-xsl)

# Data that is related to downloads.
set(${BP_PACKAGE}_URL http://download.gnome.org/sources/yelp-xsl/3.8/yelp-xsl-3.8.1.tar.xz)
set(${BP_PACKAGE}_DOWNLOAD_HASH_TYPE SHA256)
set(${BP_PACKAGE}_DOWNLOAD_HASH afc519be07ffcbdcc20d1acfd1d901cfd03c7b4cf0667036534f2726bb68b40d)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  #set(BP_PATH_NODLL "${BP_PATH}")
  #set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
  set(${BP_PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
else(MSYS_PLATFORM)
  set(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
  set(${BP_PACKAGE}_SET_CFLAGS "CFLAGS=$ENV{CFLAGS}")
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")

set(${BP_PACKAGE}_SET_CXXFLAGS "CXXFLAGS=$ENV{CXXFLAGS}")

# Drop -fvisibility=hidden since that option does not work for a
# number of software packages that are configured automatically using
# this template.
string(REGEX REPLACE "-fvisibility=hidden" "" ${BP_PACKAGE}_SET_CFLAGS "${${BP_PACKAGE}_SET_CFLAGS}")
string(REGEX REPLACE "-fvisibility=hidden" "" ${BP_PACKAGE}_SET_CXXFLAGS "${${BP_PACKAGE}_SET_CXXFLAGS}")

ExternalProject_Add(
  build_${BP_PACKAGE}
  DEPENDS ${${BP_PACKAGE}_dependencies_targets}
  URL ${${BP_PACKAGE}_URL}
  URL_HASH ${${BP_PACKAGE}_DOWNLOAD_HASH_TYPE}=${${BP_PACKAGE}_DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${${BP_PACKAGE}_SET_CFLAGS} ${${BP_PACKAGE}_SET_CXXFLAGS} ${source_PATH}/${BP_CONFIGURE_COMMAND} 
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND} 
  BUILD_IN_SOURCE OFF
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND}  install
  )

list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
