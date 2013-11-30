# iwidgets/bp.cmake
# CMakeLists.txt file to configure the build of iwidgets4.0 and iwidgets

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
if(iwidgets_configured)
  return()
endif(iwidgets_configured)
set(iwidgets_configured ON)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(BP_ignored_dependencies_LIST ${extra_ignored_dependencies_list})

set(iwidgets4.0_dependencies_LIST itk3 itcl3 tk tcl)
set(iwidgets_dependencies_LIST itk tk tcl)

set(tags "4.0" "")
foreach(tag IN LISTS tags)
  # Remove dependencies that should be ignored.
  if(iwidgets${tag}_dependencies_LIST)
    list(REMOVE_ITEM iwidgets${tag}_dependencies_LIST ${BP_ignored_dependencies_LIST})
  endif(iwidgets${tag}_dependencies_LIST)

  set(iwidgets${tag}_dependencies_targets)
  foreach(build_configuration ${iwidgets${tag}_dependencies_LIST})
    if(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
      include(${build_configuration}/bp.cmake)
      list(APPEND iwidgets${tag}_dependencies_targets build_${build_configuration})
    else(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
      message(STATUS "Warning: A build_configuration for ${build_configuration} does not exist so it is assumed this dependency of iwidgets${tag} has been installed another way.")
    endif(EXISTS ${CMAKE_SOURCE_DIR}/${build_configuration}/bp.cmake)
  endforeach(build_configuration ${iwidgets${tag}_dependences_LIST})
endforeach(tag IN LISTS tags)

# This can be safely done only after above includes.
set(BP_PACKAGE iwidgets)

set(${BP_PACKAGE}4.0_VERSION 4.0.1)
set(${BP_PACKAGE}4.0_LIBVERSION 4.0)
set(${BP_PACKAGE}4.0_ITCL_VERSION 3.2)
set(${BP_PACKAGE}4.0_CMAKE_OPTION -DBUILD_IWIDGETS_4.1=OFF)

set(${BP_PACKAGE}_VERSION 4.1.0)
set(${BP_PACKAGE}_LIBVERSION 4.1)
set(${BP_PACKAGE}_ITCL_VERSION 4.0)
set(${BP_PACKAGE}_CMAKE_OPTION -DBUILD_IWIDGETS_4.1=ON)

foreach(tag IN LISTS tags)
  # Data that is related to downloads.
  set(${BP_PACKAGE}${tag}_URL http://downloads.sourceforge.net/project/incrtcl/%5bIncr%20Widgets%5d/${${BP_PACKAGE}4.0_VERSION}/iwidgets${${BP_PACKAGE}4.0_VERSION}.tar.gz)
  set(${BP_PACKAGE}${tag}_DOWNLOAD_HASH_TYPE MD5)
  set(${BP_PACKAGE}${tag}_DOWNLOAD_HASH 0e9c140e81ea6015b56130127c7deb03)

  # Data that is related to the PATH that must be used.
  if(MSYS_PLATFORM)
    #set(BP_PATH_NODLL "${BP_PATH}")
    #set(BP_PATH "${EP_BASE}/Build/build_${BP_PACKAGE}${tag}/dll;${BP_PATH_NODLL}")
    determine_msys_path(BP_PATH "${BP_PATH}")
    # Must have all elements of env command in MSYS platform form
    determine_msys_path(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}${tag}")
  else(MSYS_PLATFORM)
    set(source_PATH "${EP_BASE}/Source/build_${BP_PACKAGE}${tag}")
  endif(MSYS_PLATFORM)
  #message(STATUS "modified BP_PATH for ${BP_PACKAGE}${tag} = ${BP_PATH}")


  ExternalProject_Add(
    build_${BP_PACKAGE}${tag}
    DEPENDS ${${BP_PACKAGE}${tag}_dependencies_targets}
    URL ${${BP_PACKAGE}${tag}_URL}
    URL_HASH ${${BP_PACKAGE}${tag}_DOWNLOAD_HASH_TYPE}=${${BP_PACKAGE}${tag}_DOWNLOAD_HASH}
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_CMAKE_COMMAND} ${${BP_PACKAGE}${tag}_CMAKE_OPTION} ${EP_BASE}/Source/build_${BP_PACKAGE}${tag}
    BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND}
    INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${BP_PATH} ${BP_PARALLEL_BUILD_COMMAND} install
    )
  if(NOT "${tag}")
    # Note this logic block executed for iwidget4.1 which has an empty
    # tag.  For this (4.1) case we do not use the
    # http://downloads.sourceforge.net/project/kbskit/itk/iwidgets41.tar.gz
    # tarball because there are some problems (incorrect install
    # locations and only a subset of what is normally installed) for
    # that binary distribution of iwidgets4.1.  Instead, we build 4.1
    # similarly to 4.0 except we apply the following patch which was
    # derived as follows:
    #
    # diff -Naur iwidgets4.0.1/README iwidgets4.1/README >iwidgets4.1.patch
    # diff -Naur iwidgets4.0.1/license.terms iwidgets4.1/license.terms >>iwidgets4.1.patch
    # diff -Naur --exclude=pkgIndex.tcl iwidgets4.0.1/generic iwidgets4.1/library >> iwidgets4.1.patch 
    # diff -Naur iwidgets4.0.1/tests iwidgets4.1/tests >> iwidgets4.1.patch
    #
    # N.B. this patch is somewhat larger than
    # iwidgets4.1/iwidgets.patch because that latter patch is only
    # concerned with generic subdirectory differences and it also
    # appears to be taken against iwidgets-4.0.2 (which I cannot find
    # anywhere on the net) rather than iwidgets-4.0.1.
    add_custom_command(
      OUTPUT
      ${EP_BASE}/Stamp/build_${BP_PACKAGE}${tag}/build_${BP_PACKAGE}${tag}-patch
      COMMAND echo "Apply iwidgets4.1 source code patch"
      COMMAND ${PATCH_EXECUTABLE} --directory=${EP_BASE}/Source/build_${BP_PACKAGE}${tag} -p1 < ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/${BP_PACKAGE}4.1.patch
      COMMAND echo "Apply patch to support both iwidgets and Iwidgets package names"
      COMMAND ${PATCH_EXECUTABLE} --directory=${EP_BASE}/Source/build_${BP_PACKAGE}${tag} -p1 < ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/${BP_PACKAGE}4_case.patch
      APPEND
      )
  endif(NOT "${tag}")

  add_custom_command(
    OUTPUT
    ${EP_BASE}/Stamp/build_${BP_PACKAGE}${tag}/build_${BP_PACKAGE}${tag}-patch
    COMMAND echo "Add CMake-based build system"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different 
    ${CMAKE_SOURCE_DIR}/${BP_PACKAGE}/CMakeLists.txt
    ${EP_BASE}/Source/build_${BP_PACKAGE}${tag}/CMakeLists.txt
    APPEND
      )
  list(APPEND build_target_LIST build_${BP_PACKAGE}${tag})
  # Restore BP_PATH to original state.
  set(BP_PATH "${BP_ORIGINAL_NATIVE_PATH}")
  #message(STATUS "${BP_PACKAGE}${tag} restored original BP_PATH = ${BP_PATH}")
endforeach(tag IN LISTS tags)
