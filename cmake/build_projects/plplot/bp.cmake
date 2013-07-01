# plplot/bp.cmake
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build and test of PLplot.

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
# Configure build of PLplot dependencies.

# N.B. early stanzas of this logic before the includes
# cannot set or use the BP_PACKAGE variable since the includes
# reset that.

# Protect against configuring a build twice in one CMake call
if(plplot_configured)
  return()
endif(plplot_configured)
set(plplot_configured ON)

set(plplot_dependencies_LIST
  libagg
  libharu
  libqhull
  shapelib
# TEMPORARY
# For debugging speed ignore wxwidgets soft dependency of PLplot for
# now.
#  wxwidgets
)

foreach(build_configuration ${plplot_dependencies_LIST})
  include(${build_configuration}/bp.cmake)
endforeach(build_configuration ${plplot_dependences_LIST})

# This can be safely done only after above includes.
set(BP_PACKAGE plplot)

set(${BP_PACKAGE}_dependencies_targets)
foreach(build_configuration ${${BP_PACKAGE}_dependencies_LIST})
  list(APPEND ${BP_PACKAGE}_dependencies_targets build_${build_configuration})
endforeach(build_configuration ${${BP_PACKAGE}_dependences_LIST})
message(STATUS "${BP_PACKAGE}_dependencies_targets = ${${BP_PACKAGE}_dependencies_targets}")

# Note could retrieve the latest release, but that is pretty dated.
# So ideally, would use a subversion client to get the trunk version,
# but that means (on Windows) you must build a subversion client.
# That build (of the apache version of subversion) should be
# straightforward assuming wget (available both for Unix and MSYS) is
# installed.  So I think ultimately a build of a subversion client
# will be configured and available for all platforms.  But for now
# just use a local directory where the trunk version of PLplot has
# been recently updated.
# TEMPORARY
set(PLPLOT_LOCAL_SOURCE_DIR /home/software/${BP_PACKAGE}_svn/HEAD/${BP_PACKAGE}_allura)
file(TO_NATIVE_PATH ${PLPLOT_LOCAL_SOURCE_DIR} PLPLOT_LOCAL_SOURCE_DIR)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  set(BP_PATH_NODLL "${BP_PATH}")
  set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}/dll;${BP_PATH_NODLL}")
  message(STATUS "Original BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")
  determine_msys_path(BP_PATH "${BP_PATH}")
  # TEMPORARY (already in the _required_ native form for the cmake -E
  # copy_directory command below except for the drive letter which is
  # z: for this local result.)
  set(PLPLOT_LOCAL_SOURCE_DIR z:${PLPLOT_LOCAL_SOURCE_DIR})
endif(MSYS_PLATFORM)
message(STATUS "modified BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")

# Build PLplot itself.  The pdf device is turned on since the haru library
# is built with a fix to avoid segfaults for example 24.
ExternalProject_Add(
  build_${BP_PACKAGE}
  DEPENDS "${${BP_PACKAGE}_dependencies_targets}"
  #TEMPORARY
  DOWNLOAD_COMMAND ${CMAKE_COMMAND} -E copy_directory ${PLPLOT_LOCAL_SOURCE_DIR} ${EP_BASE}/Source/build_${BP_PACKAGE}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_CMAKE_COMMAND} -DBUILD_TEST=ON -DPLD_pdf=ON ${EP_BASE}/Source/build_${BP_PACKAGE}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND} install
  TEST_BEFORE_INSTALL OFF
  TEST_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND} test_noninteractive
  STEP_TARGETS configure build install test
  )

# Add custom commands to the current test step.
add_custom_command(
  OUTPUT
  ${EP_BASE}/Stamp/build_${BP_PACKAGE}/build_${BP_PACKAGE}-test
  COMMAND echo made_it_to_extra_test
  COMMENT "Test installed examples from ${BP_PACKAGE}"
  APPEND
  )

list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
