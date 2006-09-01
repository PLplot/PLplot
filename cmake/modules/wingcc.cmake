# cmake/modules/wingcc.cmake
#
# Copyright (C) 2006 Werner Smekal
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

# Module for determining all configuration variables related to the wingcc
# device driver.
# The following variables are set/modified:
# PLD_wingcc		  - ON means the wingcc device is enabled.
# wingcc_COMPILE_FLAGS	  - individual COMPILE_FLAGS required to compile wingcc
# 			    device.
# wingcc_LINK_FLAGS	  - individual LINK_FLAGS for dynamic wingcc device.
# DRIVERS_LINK_FLAGS	  - list of LINK_FLAGS for all static devices.

if(PLD_wingcc)
  message(STATUS "Looking for gdi32 header and library")
  find_package(GDI32)
  if(GDI32_FOUND)
    message(STATUS "Looking for gdi32 header and library - found")
    set(wingcc_LINK_FLAGS "${GDI32_LIBRARIES};comdlg32")
    if(WITH_FREETYPE)
      set(
      wingcc_COMPILE_FLAGS
      "${wingcc_COMPILE_FLAGS} -I${FREETYPE_INCLUDE_DIR}"
      )
      set(
      wingcc_LINK_FLAGS
      ${wingcc_LINK_FLAGS}
      ${FREETYPE_LIBRARIES}
      )
    endif(WITH_FREETYPE)
    set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${wingcc_LINK_FLAGS})
  else(GDI32_FOUND)
    message(STATUS "Looking for gdi32 header and library - not found")
    message(STATUS "WARNING: Setting PLD_wingcc to OFF.")
    set(PLD_wingcc OFF CACHE BOOL "Enable wingcc device" FORCE)    
  endif(GDI32_FOUND)
endif(PLD_wingcc)

