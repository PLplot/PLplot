# subversion/CMakeLists.txt
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
# have run "include(ExternalProject)" and set EPA_BASE variable (and
# corresponding directory property) as well as various EPA variables
# used below that configure how the External_Project functions
# operate.

set(PACKAGE subversion)

# Protect against configuring a build twice in one CMake call
if(configured)
  return()
endif(configured)
set(configured ON)

# Data that is related to downloads.
set(URL http://apache.parentingamerica.com/subversion/subversion-1.7.10.tar.bz2)
# TEMPORARY local version for debugging
set(URL /home/software/${PACKAGE}/${PACKAGE}-1.7.10.tar.bz2)
set(URL_MD5 4088a77e14232876c9b4ff1541e6e200)

# Data that is related to the PATH that must be used.
if(MSYS_PLATFORM)
  set(EPA_PATH_NODLL "${EPA_PATH}")
  set(EPA_PATH "${EPA_BASE}/Build/build_${PACKAGE}/dll;${EPA_PATH_NODLL}")
  determine_msys_path(EPA_PATH "${EPA_PATH}")
  # Must have all elements of env command in MSYS platform form
  determine_msys_path(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
else(MSYS_PLATFORM)
  set(source_PATH "${EPA_BASE}/Source/build_${PACKAGE}")
endif(MSYS_PLATFORM)
#message(STATUS "modified EPA_PATH for ${PACKAGE} = ${EPA_PATH}")

ExternalProject_Add(
  build_${PACKAGE}
  URL ${URL}
  URL_MD5 ${URL_MD5}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${source_PATH}/${EPA_CONFIGURE_COMMAND}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install
  TEST_BEFORE_INSTALL OFF
  TEST_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} check
  # STEP_TARGETS download download_extra_source mkdir_fix configure build install test
  )

# Add custom command to run the special get-deps.sh script
# to update the source tree with the required
# dependent library sources to be included in the subversion
# build that are recommended by the subversion project.
ExternalProject_Add_Step(
  build_${PACKAGE} download_extra_source
  COMMENT "Custom updating of ${PACKAGE} with downloaded source code"
  DEPENDEES download
  DEPENDERS configure
  COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${BASH_EXECUTABLE} get-deps.sh
  # N.B. no file dependencies are worthwhile since all data
  # are automatically downloaded.
  WORKING_DIRECTORY "${EPA_BASE}/Source/build_${PACKAGE}"
  )

# Add mkdir fix noted at
# http://mail-archives.apache.org/mod_mbox/subversion-users/201109.mbox/<838A6C12-013C-43AF-9645-A876CB0C5849@me.com>
# Why in the world is this simple fix not yet propagated upstream?
ExternalProject_Add_Step(
  build_${PACKAGE} mkdir_fix
  COMMENT "Fix 'mkdir serf/auth' issue"
  DEPENDEES configure
  DEPENDERS build
  COMMAND mkdir serf/auth
  WORKING_DIRECTORY "${EPA_BASE}/Build/build_${PACKAGE}"
  )

list(APPEND build_target_LIST build_${PACKAGE})
# Restore EPA_PATH to original state.
set(EPA_PATH "${EPA_ORIGINAL_NATIVE_PATH}")
#message(STATUS "${PACKAGE} restored original EPA_PATH = ${EPA_PATH}")
