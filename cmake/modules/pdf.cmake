# cmake/modules/pdf.cmake
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

# Module for determining all configuration variables related to the pdf device
# driver.
# The following variables are set/modified:
# PLD_pdf		  - ON means the pdf device is enabled.
# pdf_COMPILE_FLAGS	  - individual COMPILE_FLAGS required to compile pdf
# 			    device.
# pdf_LINK_FLAGS	  - individual LINK_FLAGS for dynamic pdf device.
# DRIVERS_LINK_FLAGS	  - list of LINK_FLAGS for all static devices.

if(PLD_pdf)
  message(STATUS "Looking for haru pdf header and library")
  find_package(hpdf)
  if(hpdf_FOUND)
    message(STATUS "Looking for haru pdf header and library - found")
    if(WIN32)
      set(pdf_COMPILE_FLAGS "-I${hpdf_INCLUDE_DIRS} -DHPDF_DLL")
    else(WIN32)
      set(pdf_COMPILE_FLAGS "-I${hpdf_INCLUDE_DIRS} -DHPDF_SHARED")
    endif(WIN32)
    set(pdf_LINK_FLAGS "${hpdf_LIBRARIES}")
    set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${pdf_LINK_FLAGS})
  else(hpdf_FOUND)
    message(STATUS "Looking for haru pdf header and library - not found")
    message(STATUS "WARNING: Setting PLD_pdf to OFF.")
    set(PLD_pdf OFF CACHE BOOL "Enable pdf device" FORCE)    
  endif(hpdf_FOUND)
endif(PLD_pdf)

