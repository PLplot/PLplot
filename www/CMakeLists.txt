# www/CMakeLists.txt for PLplot
###
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2008 Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Configure documentation file sizes.
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/documentation.php.in
  ${CMAKE_CURRENT_BINARY_DIR}/documentation.php @ONLY
  )

# Configure style files
add_subdirectory(css)

# Set up Makefile rules to generate announcements and validate their DocBook-XML source.
add_subdirectory(announce)

set(PHP_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/corefunctions.php
  ${CMAKE_CURRENT_SOURCE_DIR}/credits.php
  ${CMAKE_CURRENT_SOURCE_DIR}/development.php
  ${CMAKE_CURRENT_SOURCE_DIR}/download.php
  ${CMAKE_CURRENT_SOURCE_DIR}/examples.php
  ${CMAKE_CURRENT_SOURCE_DIR}/index.php
  ${CMAKE_CURRENT_SOURCE_DIR}/template.php
  ${CMAKE_CURRENT_BINARY_DIR}/documentation.php
  )
