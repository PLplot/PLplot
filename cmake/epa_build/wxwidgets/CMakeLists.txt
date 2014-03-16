# wxwidgets/CMakeLists.txt

# Configure the build of wxwidgets.

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

set(PACKAGE wxwidgets)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(dependencies_LIST)

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

if(MSYS_PLATFORM)
  set(CXXFLAGS "-fno-keep-inline-dllexport $ENV{CXXFLAGS}")
  # No further dependencies required on Windows.
else(MSYS_PLATFORM)
  set(CXXFLAGS "$ENV{CXXFLAGS}")
  # To avoid having to build all of gtk with epa_build in a consistent
  # manner I tried several alternatives to the gtk toolkit.  
  # e.g., 
  # set(toolkit_options --with-x11)
  # set(toolkit_options --with-motif)
  # But neither of those worked because they are not capable of
  # supplying the unicode (see --enable-unicode below) capability
  # that is provided by the default gtk toolkit and needed by the
  # PLplot build of the wxwidgets device.
  # The inevitable conclusion is that on Unix, we must use the gtk toolkit
  # and therefore (now that epa_build has a gtk+ configuration)
  # we must specificially add gtk+ as a dependency of wxwidgets.
  list(APPEND dependencies_LIST gtk+)
endif(MSYS_PLATFORM)

set(CFLAGS "$ENV{CFLAGS}")

# Drop -fvisibility=hidden since that option may not work for this package.
string(REGEX REPLACE "-fvisibility=hidden" "" CFLAGS "${CFLAGS}")
string(REGEX REPLACE "-fvisibility=hidden" "" CXXFLAGS "${CXXFLAGS}")

# Data that is related to downloads.
set(URL http://prdownloads.sourceforge.net/wxwindows/wxWidgets-2.8.12.tar.gz)
set(URL_MD5 2fa39da14bc06ea86fe902579fedc5b1)

ExternalProject_Add(
  build_${PACKAGE}
  URL ${URL}
  URL_MD5 ${URL_MD5}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} "CFLAGS=${CFLAGS}" "CXXFLAGS=${CXXFLAGS}" ${source_PATH}/${EPA_CONFIGURE_COMMAND} --enable-shared --enable-unicode --enable-debug --enable-debug_gdb ${toolkit_options}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND}
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} ${EPA_PARALLEL_MAKE_COMMAND} install
  )
