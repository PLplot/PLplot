# subversion/bp.cmake
# This file should be included directly or indirectly from a top-level
# CMakeLists.txt file to configure the build of subversion.

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

set(BP_PACKAGE subversion)

# Protect against configuring a build twice in one CMake call
if(${BP_PACKAGE}_configured)
  return()
endif(${BP_PACKAGE}_configured)
set(${BP_PACKAGE}_configured ON)

# Data that is related to downloads.
set(${BP_PACKAGE}_URL http://apache.parentingamerica.com/subversion/subversion-1.7.10.tar.bz2)
# TEMPORARY local version for debugging
set(${BP_PACKAGE}_URL /home/software/${BP_PACKAGE}/${BP_PACKAGE}-1.7.10.tar.bz2)
set(${BP_PACKAGE}_URL_MD5 4088a77e14232876c9b4ff1541e6e200)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  set(BP_PATH_NODLL "${BP_PATH}")
  set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}/dll;${BP_PATH_NODLL}")
  determine_msys_path(BP_PATH "${BP_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
else(MSYS_PLATFORM)
  set(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}")
endif(MSYS_PLATFORM)
#message(STATUS "modified BP_PATH for ${BP_PACKAGE} = ${BP_PATH}")

ExternalProject_Add(
  build_${BP_PACKAGE}
  URL ${${BP_PACKAGE}_URL}
  URL_MD5 ${${BP_PACKAGE}_URL_MD5}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${source_PATH}/${BP_CONFIGURE_COMMAND}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND} install
  TEST_BEFORE_INSTALL OFF
  TEST_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_MAKE_COMMAND} check
  # STEP_TARGETS download download_extra_source mkdir_fix configure build install test
  )

# Add custom command to run the special get-deps.sh script
# to update the source tree with the required
# dependent library sources to be included in the subversion
# build that are recommended by the subversion project.
ExternalProject_Add_Step(
  build_${BP_PACKAGE} download_extra_source
  COMMENT "Custom updating of ${BP_PACKAGE} with downloaded source code"
  DEPENDEES download
  DEPENDERS configure
  COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BASH_EXECUTABLE} get-deps.sh
  # N.B. no file dependencies are worthwhile since all data
  # are automatically downloaded.
  WORKING_DIRECTORY "${EP_BASE}/Source/build_${BP_PACKAGE}"
  )

# Add mkdir fix noted at
# http://mail-archives.apache.org/mod_mbox/subversion-users/201109.mbox/<838A6C12-013C-43AF-9645-A876CB0C5849@me.com>
# Why in the world is this simple fix not yet propagated upstream?
ExternalProject_Add_Step(
  build_${BP_PACKAGE} mkdir_fix
  COMMENT "Fix 'mkdir serf/auth' issue"
  DEPENDEES configure
  DEPENDERS build
  COMMAND mkdir serf/auth
  WORKING_DIRECTORY "${EP_BASE}/Build/build_${BP_PACKAGE}"
  )

list(APPEND build_target_LIST build_${BP_PACKAGE})
# Restore BP_PATH to original state.
set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${BP_PACKAGE} restored original BP_PATH = ${BP_PATH}")
