# plplot/CMakeLists.txt
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
# have run "include(ExternalProject)" and set EPA_BASE variable (and
# corresponding directory property) as well as various EPA variables
# used below that configure how the External_Project functions
# operate.
# Configure build of PLplot dependencies.

# N.B. early stanzas of this logic before the includes
# cannot set or use the PACKAGE variable since the includes
# reset that.

# Protect against configuring a build twice in one CMake call
if(plplot_configured)
  return()
endif(plplot_configured)
set(plplot_configured ON)

# Distinguish between plplot_lite dependencies and complete dependencies
# for a heavy-duty build of PLplot.
set(plplot_lite_dependencies_LIST
  libagg
  libharu
  libqhull
  shapelib
)

# Turn off wxwidgets devices and library, and cairo device drivers for lite version
set(plplot_lite_cmake_args
  -DPLD_wxwidgets=OFF
  -DENABLE_wxwidgets=OFF
  -DDEFAULT_NO_CAIRO_DEVICES:BOOL=ON
  -DDEFAULT_NO_QT_DEVICES:BOOL=ON
  -DENABLE_qt=OFF
  )

set(plplot_lite_dependencies_targets)
foreach(build_configuration ${plplot_lite_dependencies_LIST})
  list(APPEND plplot_lite_dependencies_targets build_${build_configuration})
endforeach(build_configuration ${plplot_lite_dependences_LIST})

set(plplot_dependencies_LIST
  ${plplot_lite_dependencies_LIST}
  wxwidgets
  pango
  )

set(plplot_dependencies_targets)
foreach(build_configuration ${plplot_dependencies_LIST})
  include(${build_configuration}/CMakeLists.txt)
  list(APPEND plplot_dependencies_targets build_${build_configuration})
endforeach(build_configuration ${plplot_dependences_LIST})

# Leave eveything ON by default for normal plplot.
set(plplot_cmake_args)

# This can be safely done only after above includes.
set(PACKAGE plplot)

# Now use better method of accessing the latest PLplot so comment out
# the choices below.
#if(PREFER_DOWNLOAD)
#  set(PLPLOT_DOWNLOAD_UPDATE 
#    URL http://prdownloads.sourceforge.net/plplot/plplot/5.9.10%20Source/plplot-5.9.10.tar.gz
#    URL_HASH SHA256=6be3e20d6992cb2afd132a00cbc812aa5db170abe5855c44eb01481ac4b0b723
#    PATCH_COMMAND ${PATCH_EXECUTABLE} -p1 < ${CMAKE_CURRENT_SOURCE_DIR}/plplot/plplot.patch
#    )
#else(PREFER_DOWNLOAD)
#  set(PLPLOT_DOWNLOAD_UPDATE 
#    SVN_REPOSITORY http://svn.code.sf.net/p/plplot/code/trunk
#    )
#endif(PREFER_DOWNLOAD)

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

set(tags "" "_lite")
foreach(tag IN LISTS tags)
  # Data that is related to the PATH that must be used.
  if(MSYS_PLATFORM)
    set(EPA_PATH_NODLL "${EPA_PATH}")
    set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}${tag}/dll;${EPA_PATH_NODLL}")
    message(STATUS "Original EPA_PATH for ${PACKAGE}${tag} = ${EPA_PATH}")
    determine_msys_path(EPA_PATH "${EPA_PATH}")
  endif(MSYS_PLATFORM)
  message(STATUS "modified EPA_PATH for ${PACKAGE}${tag} = ${EPA_PATH}")

  ExternalProject_Add(
    build_${PACKAGE}${tag}
    DEPENDS "${${PACKAGE}${tag}_dependencies_targets}"
    DOWNLOAD_COMMAND ${CMAKE_COMMAND} -E copy_directory ${PLPLOT_LOCAL_SOURCE_DIR} ${EPA_BASE}/Source/build_${PACKAGE}${tag}
    CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_CMAKE_COMMAND} -DBUILD_TEST=ON ${${PACKAGE}${tag}_cmake_args} ${EPA_BASE}/Source/build_${PACKAGE}${tag}
    BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND}
    INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND} install
    TEST_BEFORE_INSTALL OFF
    TEST_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_BUILD_COMMAND} test_noninteractive
    STEP_TARGETS configure build install test
    )

  # Add custom commands to the current test step.
  # One commented-out fragment to show how to do it....
#  add_custom_command(
#    OUTPUT
#    ${EPA_BASE}/Stamp/build_${PACKAGE}${tag}/build_${PACKAGE}${tag}-test
#    COMMAND echo made_it_to_extra_test
#    COMMENT "Test installed examples from ${PACKAGE}${tag}"
#    APPEND
#    )

  list(APPEND build_target_LIST build_${PACKAGE}${tag})

  # Restore EPA_PATH to original state.
  set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
  #message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
endforeach(tag IN LISTS tags)
