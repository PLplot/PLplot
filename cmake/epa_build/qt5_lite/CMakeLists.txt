# qt5_lite/CMakeLists.txt

# Configure the build of qt5_lite.

# Copyright (C) 2014 Alan W. Irwin

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

# Many of the details of this epa_build configuration were derived from 
# <http://www.linuxfromscratch.org/blfs/view/svn/x/qt5.html>.

set(PACKAGE qt5_lite)

# List of dependencies (most of which are build tools) which should be
# ignored.
set(ignored_dependencies_LIST ${extra_ignored_dependencies_list})

# The assumption here is that for Windows platforms Qt5 is self-contained
# and therefore does not rely on external libraries (at least if you configure
# the in-house version of those).  For Linux platforms the dependencies will
# just have to be resolved by system versions rather than building a lot
# of additional packages.
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

set(CFLAGS "$ENV{CFLAGS}")
set(CXXFLAGS "$ENV{CXXFLAGS}")

# Drop -fvisibility=hidden since that option may not work for Qt5.
# (May test this later if I get everything to work.)
string(REGEX REPLACE "-fvisibility=hidden" "" CFLAGS "${CFLAGS}")
string(REGEX REPLACE "-fvisibility=hidden" "" CXXFLAGS "${CXXFLAGS}")

# Data that is related to downloads.
set(URL http://download.qt-project.org/official_releases/qt/5.2/5.2.1/single/qt-everywhere-opensource-src-5.2.1.tar.xz)
# Temporary to save download bandwidth/time while debugging this.
# set(URL /home/software/build_qt5/qt-everywhere-opensource-src-5.2.1.tar.xz)
set(DOWNLOAD_HASH_TYPE MD5)
set(DOWNLOAD_HASH 0c8d2aa45f38be9c3f7c9325eb059d9d)

# Many of these are recommended by BLFS, but I have tried to use
# a consistent install prefix and I have also turned off as
# much as possible for this "lite" version.
set(config_arguments

  # Configure options:
  -release
  -confirm-license
  -opensource

  # Try to get rid of everything to do with sql by eliminating all possible
  # drivers.
  -no-sql-db2
  -no-sql-ibase
  -no-sql-mysql
  -no-sql-oci
  -no-sql-odbc
  -no-sql-psql
  -no-sql-sqlite
  -no-sql-sqlite2
  -no-sql-tds

  -no-javascript-jit
  -no-qml-debug
  # Third party libraries (drop if possible but if needed by PLplot, use
  # in-house Qt5 versions of these to reduce dependencies):
  -qt-zlib
  -no-mtdev
  -no-gif
  -qt-libpng
  -qt-libjpeg
  -qt-freetype
  -no-openssl
  -qt-pcre
  -qt-xcb
  -qt-xkbcommon
  -no-xinput2
  -no-xcb-xlib
  -no-glib
  # Additional options:

  # Drop all components other than libs
  -nomake tools
  -nomake examples
  -nomake tests

  -optimized-qmake

  # Need gui which happens by default (there is no -gui option) 
  # if you do not specify -no-gui

  -no-nis
  -no-cups
  -no-iconv
  -no-icu
  -no-dbus

  # Qt/X11 only: (leave everything default)
  )

# Note, setting the QT5DIR environment variable to the install prefix
# as below is recommended by method 2 of BLFS.
set(QT5DIR ${EPA_CMAKE_INSTALL_PREFIX}) 

ExternalProject_Add(
  build_qt5_lite
  DEPENDS ${dependencies_targets}
  URL ${URL}
  URL_HASH ${DOWNLOAD_HASH_TYPE}=${DOWNLOAD_HASH}
  CONFIGURE_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} QT5DIR=${QT5DIR} "CFLAGS=${CFLAGS}" "CXXFLAGS=${CXXFLAGS}" ${source_PATH}/${EPA_CONFIGURE_COMMAND} ${config_arguments}
  BUILD_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} QT5DIR=${QT5DIR} ${EPA_PARALLEL_MAKE_COMMAND}
  BUILD_IN_SOURCE 0
  INSTALL_COMMAND ${ENV_EXECUTABLE} PATH=${EPA_PATH} QT5DIR=${QT5DIR} ${EPA_PARALLEL_MAKE_COMMAND} install
  #STEP_TARGETS update configure build install test
  )

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/install_png.sh
"install -v -dm755 ${EPA_CMAKE_INSTALL_PREFIX}/usr/share/pixmaps/
install -v -Dm644 ${EPA_BASE}/Source/build_${PACKAGE}/qttools/src/assistant/assistant/images/assistant-128.png ${EPA_CMAKE_INSTALL_PREFIX}/usr/share/pixmaps/assistant-qt5.png
install -v -Dm644 ${EPA_BASE}/Source/build_${PACKAGE}/qttools/src/designer/src/designer/images/designer.png ${EPA_CMAKE_INSTALL_PREFIX}/usr/share/pixmaps/designer-qt5.png
install -v -Dm644 ${EPA_BASE}/Source/build_${PACKAGE}/qttools/src/linguist/linguist/images/icons/linguist-128-32.png ${EPA_CMAKE_INSTALL_PREFIX}/usr/share/pixmaps/linguist-qt5.png
install -v -Dm644 ${EPA_BASE}/Source/build_${PACKAGE}/qttools/src/qdbus/qdbusviewer/images/qdbusviewer-128.png ${EPA_CMAKE_INSTALL_PREFIX}/usr/share/pixmaps/qdbusviewer-qt5.png
install -dm755 ${EPA_CMAKE_INSTALL_PREFIX}/usr/share/applications 
"
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-install
  COMMAND ${CMAKE_COMMAND} -E echo "install some addition png files recommended by BLFS."
  COMMAND ${BASH_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/install_png.sh
  APPEND
  )

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/install_fixups.sh
"find ${QT5DIR} -name qt_lib_bootstrap_private.pri -exec sed -i -e \"s:$PWD/qtbase:/${QT5DIR}/lib/:g\" {} \;
find ${QT5DIR} -name \\*.prl -exec sed -i -e '/^QMAKE_PRL_BUILD_DIR/d' {} \;
"
  )

add_custom_command(
  OUTPUT
  ${EPA_BASE}/Stamp/build_${PACKAGE}/build_${PACKAGE}-install
  COMMAND ${CMAKE_COMMAND} -E echo "install fixups recommended by BLFS."
  COMMAND ${BASH_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/install_fixups.sh
  APPEND
  )

# N.B. I have explicitly ignored all post-install Qt5 configuration recommended
# by BLFS because I don't think that will be needed by PLplot.
