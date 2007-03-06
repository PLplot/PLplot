# cmake/modules/cxx.cmake
#
# C++ binding configuration
#
# Copyright (C) 2006  Andrew Ross
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
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA

# Module for determining C++ bindings configuration options

# Option to enable C++ bindings
if(DEFAULT_NO_BINDINGS)
  OPTION(ENABLE_cxx "Enable C++ bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  OPTION(ENABLE_cxx "Enable C++ bindings" ON)
endif(DEFAULT_NO_BINDINGS)

IF (ENABLE_cxx)
# Check if "using namespace std;" works
INCLUDE(TestForNamespace)

# Check for <cmath>
INCLUDE(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(cmath HAVE_CMATH)

# Need to add check for broken cmath with isnan missing (BROKEN_ISNAN_CXX)
INCLUDE(TestBrokenIsnanCXX)

# Need to add check if stdint.h can be used from c++ (HAVE_CXX_STDINT_H)
INCLUDE(TestForStdintCXX)

ENDIF(ENABLE_cxx)
