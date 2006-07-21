# cmake/modules/gcw.cmake
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

# Module for determining additional configuration variables related to the gcw
# device driver.
# The following variables are set/modified:
# PLD_gcw		  - ON means the gcw device is enabled.
# gcw_COMPILE_FLAGS	  - individual COMPILE_FLAGS required to compile gcw
# 			    device.
# gcw_LINK_FLAGS	  - individual LINK_FLAGS for dynamic gcw device.
# DRIVERS_LINK_FLAGS	  - list of LINK_FLAGS for all static devices.
if(PLD_gcw)
  if(NOT PKGCONFIG_EXECUTABLE)
    message(STATUS "WARNING: "
    "because pkg-config not found setting PLD_gcw and ENABLE_pygcw to OFF"
    )
    set(PLD_gcw OFF CACHE BOOL "enable gcw device" FORCE)
    set(ENABLE_pygcw OFF)
  endif(NOT PKGCONFIG_EXECUTABLE)
endif(PLD_gcw)
if(PLD_gcw)
  if(ENABLE_python)
    set(ENABLE_pygcw ON)
  else(ENABLE_python)
    set(ENABLE_pygcw OFF)
  endif(ENABLE_python)
  pkgconfig(libgnomeui-2.0 includedir libdir linkflags cflags)
  if(linkflags AND cflags)
    pkgconfig(libgnomeprintui-2.2 includedir libdir linkflags1 cflags1)
    if(linkflags1 AND cflags1)
      # N.B. pkgconfig returns blank-delimited strings so must concatanate
      # results the same way.
      set(gcw_COMPILE_FLAGS "${cflags} ${cflags1}")
      set(gcw_LINK_FLAGS "${linkflags} ${linkflags1}")
    else(linkflags1 AND cflags1)
      message(STATUS "WARNING: because libgnomeprintui-2.2 not found "
      "setting PLD_gcw and ENABLE_pygcw to OFF"
      )
      set(PLD_gcw OFF CACHE BOOL "enable gcw device" FORCE)
      set(ENABLE_pygcw OFF)
    endif(linkflags1 AND cflags1)
  else(linkflags AND cflags)
    message(STATUS "WARNING: because libgnomeui-2.0 not found "
    "setting PLD_gcw and ENABLE_pygcw to OFF"
    )
    set(PLD_gcw OFF CACHE BOOL "enable gcw device" FORCE)
    set(ENABLE_pygcw OFF)
  endif(linkflags AND cflags)
endif(PLD_gcw)
if(PLD_gcw)
#  Check for gthread-2.0 done in cf/gcw.ac, but result not actually used
#  anywhere in the autotools configuration so we comment it out here.
#  pkgconfig(gthread-2.0 includedir libdir linkflags cflags)
  set(include_files_to_check
  "math.h"
  "glib.h"
  "gtk/gtk.h"
  "libgnomecanvas/libgnomecanvas.h"
  "libart_lgpl/libart.h"
  "libgnomeprint/gnome-print.h"
  )
  set(CMAKE_REQUIRED_FLAGS "${gcw_COMPILE_FLAGS}")
  check_include_files("${include_files_to_check}" gcw_HEADERS)
  set(CMAKE_REQUIRED_FLAGS)
  if(NOT gcw_HEADERS)
    message(STATUS "WARNING: because required headers not found "
    "setting PLD_gcw and ENABLE_pygcw to OFF"
    )
    set(PLD_gcw OFF CACHE BOOL "enable gcw device" FORCE)
    set(ENABLE_pygcw OFF)
  endif(NOT gcw_HEADERS)
endif(PLD_gcw)
if(PLD_gcw)
  if(ENABLE_DYNDRIVERS)
    set(gcw_SOURCE
    ${CMAKE_SOURCE_DIR}/drivers/plplotcanvas-hacktext.c
    )
  else(ENABLE_DYNDRIVERS)
    set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${gcw_LINK_FLAGS})
    set(gcw_SOURCE
    ${CMAKE_SOURCE_DIR}/bindings/gnome2/lib/plplotcanvas.c
    ${CMAKE_SOURCE_DIR}/bindings/gnome2/lib/gcw-lib.c
    ${CMAKE_SOURCE_DIR}/drivers/plplotcanvas-hacktext.c
    )
  endif(ENABLE_DYNDRIVERS)
endif(PLD_gcw)
if(PLD_gcw AND ENABLE_pygcw)
  pkgconfig(pygtk-2.0 includedir libdir linkflags cflags)
  if(linkflags AND cflags)
    pkgconfig(gnome-python-2.0 includedir libdir linkflags1 cflags1)
    if(linkflags1 AND cflags1)
      set(PYGCW_CFLAGS cflags)
      set(PYGCW_LIBS linkflags)
      set(CMAKE_REQUIRED_FLAGS ${PYGCW_CFLAGS})
      check_include_files(glib-object.h pygcw_HEADERS)
      set(CMAKE_REQUIRED_FLAGS)
      if(pygcw_HEADERS)
        EXEC_PROGRAM(${PKGCONFIG_EXECUTABLE}
	ARGS pygtk-2.0 --variable=codegendir
	OUTPUT_VARIABLE codegen
	)
	if(NOT codegen)
          message(STATUS "WARNING: because pygtk/codegen not found "
	  "setting ENABLE_pygcw to OFF"
	  )
	  set(ENABLE_pygcw OFF)
	endif(NOT codegen)
      else(pygcw_HEADERS)
        message(STATUS "WARNING: because required headers not found "
	"setting ENABLE_pygcw to OFF"
	)
	set(ENABLE_pygcw OFF)
      endif(pygcw_HEADERS)
    else(linkflags1 AND cflags1)
      message(STATUS "WARNING: because gnome-python-2.0 not found "
      "setting ENABLE_pygcw to OFF"
      )
      set(ENABLE_pygcw OFF)
    endif(linkflags1 AND cflags1)
  else(linkflags AND cflags)
    message(STATUS "WARNING: because pygtk-2.0 not found "
    "setting ENABLE_pygcw to OFF"
    )
    set(ENABLE_pygcw OFF)
  endif(linkflags AND cflags)
endif(PLD_gcw AND ENABLE_pygcw)
if(PLD_gcw)
  set(gcw_true "")
  set(gcw_false "#")
  set(ENABLE_gnome2 ON)
else(PLD_gcw)
  set(gcw_true "#")
  set(gcw_false "")
  set(ENABLE_gnome2 OFF)
endif(PLD_gcw)
