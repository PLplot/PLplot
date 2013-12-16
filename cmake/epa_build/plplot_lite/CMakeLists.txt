# plplot_lite/CMakeLists.txt

# Configure the build of a "lite" version of PLplot (with fewer dependencies
# but with fewer features as well).

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

set(PACKAGE plplot_lite)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(dependencies_LIST
  libagg
  libharu
  libqhull
  shapelib
  )

# Do boilerplate tasks that must be done for each different project
# that is configured as part of epa_build.
epa_boilerplate(
  ignored_dependencies_LIST
  PACKAGE
  dependencies_LIST
  dependencies_targets
  EPA_PATH
  source_PATH
  ) 

set(CFLAGS "$ENV{CFLAGS}")
set(CXXFLAGS "$ENV{CXXFLAGS}")
set(FFLAGS "$ENV{FFLAGS}")

# Turn off wxwidgets, cairo, and qt for plplot_lite.
set(cmake_args
  -DPLD_wxwidgets=OFF
  -DENABLE_wxwidgets=OFF
  -DDEFAULT_NO_CAIRO_DEVICES:BOOL=ON
  -DDEFAULT_NO_QT_DEVICES:BOOL=ON
  -DENABLE_qt=OFF
  )

# Assumption that the top-level local PLplot source tree is two directories
# up from the present top-level directory for epa_build.
# This assumption is correct if you are accessing epa_build as
# a subset of the PLplot source tree so that its location is in
# cmake/epa_build within that source tree.
# But it is not the case if you have independently
# checked out just the epa_build subset of the normal PLplot source
# tree so check that the result really is a plplot source tree.
get_filename_component(PLPLOT_LOCAL_SOURCE_DIR ${CMAKE_SOURCE_DIR} PATH)
get_filename_component(PLPLOT_LOCAL_SOURCE_DIR ${PLPLOT_LOCAL_SOURCE_DIR} PATH)
find_file(IS_PLPLOT_SOURCE_TREE plcore.c 
  HINTS ${PLPLOT_LOCAL_SOURCE_DIR}/src 
  NO_DEFAULT_PATH
)

if(NOT IS_PLPLOT_SOURCE_TREE)
  message(FATAL_ERROR "epa_build not located in cmake/epa_build in a PLplot source tree")
endif(NOT IS_PLPLOT_SOURCE_TREE)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  set(EPA_PATH_NODLL "${EPA_PATH}")
  set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
endif(MSYS_PLATFORM)

if(ENABLE_COMPREHENSIVE_PLPLOT_TEST)
  string(REGEX REPLACE ";" " " blank_delimited_cmake_args "${cmake_args}")
  file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/comprehensive_test.sh
    "echo yes | ${EPA_BASE}/Source/build_${PACKAGE}/scripts/comprehensive_test.sh --cmake_added_options \"${blank_delimited_cmake_args}\""
    )
  set(TEST_COMMAND TEST_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${BASH_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/comprehensive_test.sh)
endif(ENABLE_COMPREHENSIVE_PLPLOT_TEST)

ExternalProject_Add(
  build_${PACKAGE}
  DEPENDS "${dependencies_targets}"
  DOWNLOAD_COMMAND ${CMAKE_COMMAND} -E copy_directory ${PLPLOT_LOCAL_SOURCE_DIR} ${EPA_BASE}/Source/build_${PACKAGE}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} "CFLAGS=${CFLAGS}" "CXXFLAGS=${CXXFLAGS}" "FFLAGS=${FFLAGS}" ${EPA_CMAKE_COMMAND} -DBUILD_TEST=ON ${cmake_args} ${EPA_BASE}/Source/build_${PACKAGE}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND} install
  TEST_BEFORE_INSTALL OFF
  ${TEST_COMMAND}
  STEP_TARGETS configure build install test
  )
