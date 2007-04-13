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
# gcw_TARGETS		  - list of targets which the gcw dynamic device
# 			    depends on.
# DRIVERS_LINK_FLAGS	  - list of LINK_FLAGS for case when 
# 			    ENABLE_DYNDRIVERS OFF.  (gcw_TARGETS not included
#			    since source code used instead of this target
#			    for ENABLE_DYNDRIVERS OFF.)
# gcw_SOURCE		  - list of source files other than gcw.c
if(PLD_gcw)
  if(NOT PKGCONFIG_EXECUTABLE)
    message(STATUS 
    "WARNING: pkg-config not found. Setting PLD_gcw to OFF."
    )
    set(PLD_gcw OFF CACHE BOOL "Enable gcw device" FORCE)
  endif(NOT PKGCONFIG_EXECUTABLE)
endif(PLD_gcw)
if(PLD_gcw)
# linkflags and cflags ignored because not used in autotools side and
# there is a question whether all that ton of extra potentially interfering
# libraries might be causing a segfault that is observed for the
# dynamic devices case.
#  pkgconfig(libgnomeui-2.0 includedir libdir linkflags cflags)
#  if(linkflags AND cflags)
    pkgconfig(libgnomeprintui-2.2 includedir libdir linkflags1 cflags1)
    if(linkflags1 AND cflags1)
      # N.B. pkgconfig returns blank-delimited strings so must concatanate
      # results the same way.
      #set(gcw_COMPILE_FLAGS "${cflags} ${cflags1}")
      #set(gcw_LINK_FLAGS "${linkflags} ${linkflags1}")
      set(gcw_COMPILE_FLAGS "${cflags1}")
      set(gcw_LINK_FLAGS "${linkflags1}")
    else(linkflags1 AND cflags1)
      message(STATUS
      "WARNING: libgnomeprintui-2.2 not found.  Setting PLD_gcw "
      "to OFF."
      )
      set(PLD_gcw OFF CACHE BOOL "Enable gcw device" FORCE)
    endif(linkflags1 AND cflags1)
#  else(linkflags AND cflags)
#    message(STATUS "WARNING: libgnomeui-2.0 not found.  "
#    "Setting PLD_gcw to OFF."
#    )
#    set(PLD_gcw OFF CACHE BOOL "Enable gcw device" FORCE)
#  endif(linkflags AND cflags)
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
    message(STATUS
    "WARNING: Required headers not found. "
    "Setting PLD_gcw to OFF."
    )
    set(PLD_gcw OFF CACHE BOOL "Enable gcw device" FORCE)
  endif(NOT gcw_HEADERS)
endif(PLD_gcw)

if(DEFAULT_NO_BINDINGS)
  option(ENABLE_gnome2 "Enable Gnome2 bindings" OFF)
  option(ENABLE_pygcw "Enable Python Gnome2 bindings" OFF)
else(DEFAULT_NO_BINDINGS)
  option(ENABLE_gnome2 "Enable Gnome2 bindings" ON)
  option(ENABLE_pygcw "Enable Python Gnome2 bindings" ON)
endif(DEFAULT_NO_BINDINGS)

if(ENABLE_gnome2 AND NOT PLD_gcw)
  message(STATUS
  "WARNING: PLD_gcw is OFF so "
  "setting ENABLE_gnome2 to OFF."
  )
  set(ENABLE_gnome2 OFF CACHE BOOL "Enable Gnome2 bindings" FORCE)
endif(ENABLE_gnome2 AND NOT PLD_gcw)

if(ENABLE_pygcw AND NOT PLD_gcw)
  message(STATUS
  "WARNING: PLD_gcw is OFF so "
  "setting ENABLE_pygcw to OFF."
  )
  set(ENABLE_pygcw OFF CACHE BOOL "Enable Python Gnome2 bindings" FORCE)
endif(ENABLE_pygcw AND NOT PLD_gcw)

if(ENABLE_gnome2)
  set(gcw_true "")
  set(gcw_false "#")
else(ENABLE_gnome2)
  set(gcw_true "#")
  set(gcw_false "")
endif(ENABLE_gnome2)

if(PLD_gcw)
  if(ENABLE_DYNDRIVERS)
    if(ENABLE_gnome2)
      set(gcw_SOURCE
      ${CMAKE_SOURCE_DIR}/drivers/plplotcanvas-hacktext.c
      )
      set(gcw_TARGETS plplotgnome2${LIB_TAG})
    else(ENABLE_gnome2)
      # if gnome2 disabled, then must include full source and forget
      # gcw_TARGETS for this dynamic device.
      set(gcw_SOURCE
      ${CMAKE_SOURCE_DIR}/bindings/gnome2/lib/plplotcanvas.c
      ${CMAKE_SOURCE_DIR}/bindings/gnome2/lib/gcw-lib.c
      ${CMAKE_SOURCE_DIR}/drivers/plplotcanvas-hacktext.c
      )
    endif(ENABLE_gnome2)
  else(ENABLE_DYNDRIVERS)
    # N.B. no gcw_TARGETS here since use appropriate source code (see below)
    # instead to break circular linking.
    set(
    DRIVERS_LINK_FLAGS
    ${DRIVERS_LINK_FLAGS}
    ${gcw_LINK_FLAGS}
    )
    set(gcw_SOURCE
    ${CMAKE_SOURCE_DIR}/bindings/gnome2/lib/plplotcanvas.c
    ${CMAKE_SOURCE_DIR}/bindings/gnome2/lib/gcw-lib.c
    ${CMAKE_SOURCE_DIR}/drivers/plplotcanvas-hacktext.c
    )
  endif(ENABLE_DYNDRIVERS)
endif(PLD_gcw)

if(ENABLE_pygcw AND NOT ENABLE_python)
  message(STATUS
  "WARNING: Python is disabled so "
  "Setting ENABLE_pygcw to OFF."
  )
  set(ENABLE_pygcw OFF CACHE BOOL "Enable Python Gnome2 bindings" FORCE)
endif(ENABLE_pygcw AND NOT ENABLE_python)

if(ENABLE_pygcw)
  pkgconfig(pygtk-2.0 includedir libdir linkflags cflags)
  if(linkflags AND cflags)
    pkgconfig(gnome-python-2.0 includedir libdir linkflags1 cflags1)
    # This logic needs review.  It was copied right out of cf/gcw.ac
    # which checks for the existence of gnome-python-2.0 but uses no
    # compile or link flag returned by that check.  That made sense
    # at the time because the returned values were blank, but now (2006-11-27)
    # Ubuntu dapper drake, for example, returns an interesting value
    # "-I/usr/include/gnome-python-2.0" for --cflags1.
    if(linkflags1 AND cflags1)
      set(PYGCW_CFLAGS "${cflags}")
      set(PYGCW_LIBS "${linkflags}")
      set(CMAKE_REQUIRED_FLAGS "${PYGCW_CFLAGS}")
      check_include_files(glib-object.h pygcw_HEADERS)
      set(CMAKE_REQUIRED_FLAGS)
      if(pygcw_HEADERS)
        EXEC_PROGRAM(${PKGCONFIG_EXECUTABLE}
	ARGS pygtk-2.0 --variable=codegendir
	OUTPUT_VARIABLE codegen
	)
	if(NOT codegen)
          message(STATUS "WARNING: pygtk/codegen not found.  "
	  "Setting ENABLE_pygcw to OFF."
	  )
          set(ENABLE_pygcw OFF CACHE BOOL "Enable Python Gnome2 bindings" FORCE)
	endif(NOT codegen)
      else(pygcw_HEADERS)
        message(STATUS "WARNING: required headers not found.  "
	"Setting ENABLE_pygcw to OFF."
	)
        set(ENABLE_pygcw OFF CACHE BOOL "Enable Python Gnome2 bindings" FORCE)
      endif(pygcw_HEADERS)
    else(linkflags1 AND cflags1)
      message(STATUS "WARNING: gnome-python-2.0 not found. "
      "Setting ENABLE_pygcw to OFF."
      )
      set(ENABLE_pygcw OFF CACHE BOOL "Enable Python Gnome2 bindings" FORCE)
    endif(linkflags1 AND cflags1)
  else(linkflags AND cflags)
    message(STATUS "WARNING: pygtk-2.0 not found.  "
    "Setting ENABLE_pygcw to OFF"
    )
    set(ENABLE_pygcw OFF)
  endif(linkflags AND cflags)
endif(ENABLE_pygcw)
