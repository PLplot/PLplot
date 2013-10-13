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
  include(${build_configuration}/bp.cmake)
  list(APPEND plplot_dependencies_targets build_${build_configuration})
endforeach(build_configuration ${plplot_dependences_LIST})

# Leave eveything ON by default for normal plplot.
set(plplot_cmake_args)

# This can be safely done only after above includes.
set(BP_PACKAGE plplot)

set(tags "" "_lite")
foreach(tag IN LISTS tags)
  # Data that is related to the PATH that must be used.
  if(MSYS_PLATFORM)
    set(BP_PATH_NODLL "${BP_PATH}")
    set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}${tag}/dll;${BP_PATH_NODLL}")
    message(STATUS "Original BP_PATH for ${BP_PACKAGE}${tag} = ${BP_PATH}")
    determine_msys_path(BP_PATH "${BP_PATH}")
  endif(MSYS_PLATFORM)
  message(STATUS "modified BP_PATH for ${BP_PACKAGE}${tag} = ${BP_PATH}")

  # Build PLplot (or PLplot-lite). The pdf device is turned on since the
  # haru library is built with a fix to avoid segfaults for example 24.
  ExternalProject_Add(
    build_${BP_PACKAGE}${tag}
    DEPENDS "${${BP_PACKAGE}${tag}_dependencies_targets}"
    SVN_REPOSITORY http://svn.code.sf.net/p/plplot/code/trunk
    CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_CMAKE_COMMAND} -DBUILD_TEST=ON -DPLD_pdf=ON ${${BP_PACKAGE}${tag}_cmake_args} ${EP_BASE}/Source/build_${BP_PACKAGE}${tag}
    BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND}
    INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND} install
    TEST_BEFORE_INSTALL OFF
    TEST_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND} test_noninteractive
    #STEP_TARGETS configure build install test
    )

  # Add custom commands to the current test step.
  add_custom_command(
    OUTPUT
    ${EP_BASE}/Stamp/build_${BP_PACKAGE}${tag}/build_${BP_PACKAGE}${tag}-test
    COMMAND echo made_it_to_extra_test
    COMMENT "Test installed examples from ${BP_PACKAGE}${tag}"
    APPEND
    )

  list(APPEND build_target_LIST build_${BP_PACKAGE}${tag})

  # Restore BP_PATH to original state.
  set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
  #message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
endforeach(tag IN LISTS tags)
