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

include(shapelib/bp.cmake)
include(libqhull/bp.cmake)
include(libharu/bp.cmake)
include(libagg/bp.cmake)

# Build PLplot itself.
# (Not implemented yet, but create empty custom target with correct
# target dependencies as if that target were actually a PLplot build).
add_custom_target(build_plplot)
add_dependencies(build_plplot
  build_shapelib
  build_libqhull
  build_libharu
  build_libagg
  )



