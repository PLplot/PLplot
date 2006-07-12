# bindings/c++/CMakeLists.txt
### Process this file with cmake to produce Makefile
###
# Copyright (C) 2006 Andrew Ross
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

IF(ENABLE_c++)

SET(plplotcxx${LIB_TAG}_LIB_SRCS
plstream.cc
)

# Set the include path
INCLUDE_DIRECTORIES(
${CMAKE_SOURCE_DIR}/include
${CMAKE_BINARY_DIR}
${CMAKE_BINARY_DIR}/include
)

# Add the src directory to the link path to pick up plplot library
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

# Create plplotcxx[d] library
ADD_LIBRARY(plplotcxx${LIB_TAG} ${plplotcxx${LIB_TAG}_LIB_SRCS})

TARGET_LINK_LIBRARIES(plplotcxx${LIB_TAG} plplot${LIB_TAG})

# Set version number
SET_TARGET_PROPERTIES(plplotcxx${LIB_TAG}
PROPERTIES SOVERSION ${plplotcxx_SOVERSION} VERSION ${plplotcxx_VERSION}
)

# Install library in lib/
install(TARGETS plplotcxx${LIB_TAG} DESTINATION ${LIB_INSTALL_DIR})

ENDIF(ENABLE_c++)
