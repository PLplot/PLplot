#!/bin/bash

# This script potentially can be quite intrusive since it
# unconditionally removes trailing blanks and tabs from essentially all text files
# in the source tree.  So after running this script you should use
# "git diff" to make sure you agree with the source file changes generated
# by this script before committing these generated changes.

# Copyright (C) 2016 Alan W. Irwin
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

# Find absolute PATH of script without using readlink (since readlink is
# not available on all platforms).  Followed advice at
# http://fritzthomas.com/open-source/linux/551-how-to-get-absolute-path-within-shell-script-part2/
ORIGINAL_PATH="$(pwd)"
cd "$(dirname $0)"
# Absolute Path of the script
SCRIPT_PATH="$(pwd)"
cd "${ORIGINAL_PATH}"

# Assumption: top-level source tree is parent directory of where script
# is located.
SOURCE_TREE="$(dirname ${SCRIPT_PATH})"
cd $SOURCE_TREE

# Remove trailing blanks and tabs on all files in source tree with the following exceptions.
# * Exclude all files in the .git tree (don't want to mess with our repository).
# * Exclude all binary files recognized in .gitattributes.
# * Exclude all files in the debian, rpm, and lib trees (may review lib later).
# * Exclude all *.patch files.
# * Exclude libqhull/src/mem.h (may review this later when epa_build is updated).
# * Exclude COPYING.LIB (since we want to preserve exact LGPL licensing text).
# * Exclude test_tclmatrix.out (since this file is used to compare with a pltcl result
#   that does generate lines with some trailing blanks).

sed -i -e $'s?[\t ][\t ]*$??' $(find . -type f |grep -E -v '\.git|\.pgm|\.gif|\.jpg|\.cgm|\.map|\.dbf|\.prj|\.shp|\.shx|\.fnt|debian/|rpm/|lib/|\.patch|libqhull/src/mem.h|COPYING.LIB|test_tclmatrix.out' | xargs grep -l $'[\t ][\t ]*$')
