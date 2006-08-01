# cmake/modules/psttf.cmake
#
# Copyright (C) 2006  Alan W. Irwin
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

# Configuration for the psttf device driver (supporting the psttf and 
# psttfc devices).
#
# The following variables are set / modified
#
# PLD_psttf            - ON means the psttf and psttfc devices are enabled.
# psttf_COMPILE_FLAGS  - COMPILE_FLAGS (string) required to compile psttf
# 		         device driver.
# psttf_LINK_FLAGS     - LINK_FLAGS (string) for dynamic psttf device driver.
# psttf_RPATH	       - RPATH directory list for psttf device driver.
# psttf_TARGETS	       - Full Name of libLASi so that cmake can figure out
# 		       	 RPATH stuff in the build tree.
# Include file searches use FindPath. To add extra search directories
# set the environment variable CMAKE_INCLUDE_PATH.
# Library searches use FindLibrary. To add extra search directories
# set the environment variable CMAKE_LIBRARY_PATH.
# See cmake documentation for further details.

# Look for psttf headers and libraries with pkg-config
if(PLD_psttf)
  if(NOT PKGCONFIG_EXECUTABLE)
    message(STATUS 
    "WARNING: pkg-config not found. Setting PLD_psttf to OFF."
    )
    set(PLD_psttf OFF CACHE BOOL "Enable psttf device" FORCE)
  endif(NOT PKGCONFIG_EXECUTABLE)
endif(PLD_psttf)

# Must have dynamic devices enabled since psttf is written in C++ which
# in the static devices case is put in with C objects in libplplot(d)
# which won't work for some platforms.
if(PLD_psttf AND NOT ENABLE_DYNDRIVERS)
  message(STATUS 
     "WARNING: This device requires ENABLE_DYNDRIVERS ON, but it is OFF.\n"
  "   Setting PLD_psttf to OFF."
  )
  set(PLD_psttf OFF CACHE BOOL "Enable psttf device" FORCE)
endif(PLD_psttf AND NOT ENABLE_DYNDRIVERS)

if(PLD_psttf)
  pkgconfig("pango;pangoft2;lasi" includedir libdir linkflags cflags)
  if(linkflags AND cflags)
    set(psttf_COMPILE_FLAGS "${cflags}")
    # N.B. as long as we support lasi-1.0.5 we must compensate for bug
    # of dropped -lLASi option in liblasi.pc file
    set(psttf_LINK_FLAGS "${linkflags} -lLASi")
    # Convert from blank-delimited to a cmake list
    string(REGEX REPLACE " +" ";" psttf_RPATH ${libdir})
    # Put libLASI pkg-config information into a form that
    # target_link_libraries can interpret properly with respect to RPATH
    # for the build tree.
    find_library(psttf_TARGETS LASi ${psttf_RPATH})
    if(NOT psttf_TARGETS)
      message(FATAL_ERROR
         "libLASi not in location specified by pkg-config."
      )
    endif(NOT psttf_TARGETS)
    get_filename_component(psttf_RPATH ${psttf_TARGETS} PATH)
  else(linkflags AND cflags)
    message(STATUS
       "WARNING: pango, pangoft2, or lasi not found with pkg-config.\n"
    "   Setting PLD_psttf to OFF.  Please install all of these packages\n"
    "   and/or set the environment variable PKG_CONFIG_PATH appropriately."
    )
    set(PLD_psttf OFF CACHE BOOL "Enable psttf device" FORCE)
  endif(linkflags AND cflags)
endif(PLD_psttf)
# Test for correct version of liblasi by looking
# for API that was added for 1.0.5 which is required by PLplot.???
