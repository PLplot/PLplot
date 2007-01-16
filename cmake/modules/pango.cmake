# cmake/modules/pango.cmake
#
# Copyright (C) 2007 Alan W. Irwin
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

# Module for determining all configuration variables for libpango.

if(PKGCONFIG_EXECUTABLE)
  # This is the deprecated cmake pkgconfig command.  Convert to the more
  # modern command if more pkg-config information is required.
  pkgconfig(
  pango
  PANGO_INCLUDE_DIR PANGO_LINK_DIR PANGO_LINK_FLAGS PANGO_CFLAGS
  )

  pkgconfig(
  pangoft2
  PANGOFT2_INCLUDE_DIR PANGOFT2_LINK_DIR PANGOFT2_LINK_FLAGS PANGOFT2_CFLAGS
  )
else(PKGCONFIG_EXECUTABLE)
  message(STATUS 
  "WARNING: pango not found because pkg-config not available."
  )
endif(PKGCONFIG_EXECUTABLE)

if(PANGO_INCLUDE_DIR AND PANGOFT2_INCLUDE_DIR)
  set(HAVE_PANGO ON)
endif(PANGO_INCLUDE_DIR AND PANGOFT2_INCLUDE_DIR)
