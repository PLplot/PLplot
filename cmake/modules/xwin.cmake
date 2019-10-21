# cmake/modules/xwin.cmake
#
# Copyright (C) 2006-2019 Alan W. Irwin
# Copyright (C) 2009 Andrew Ross
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

# Module for determining all configuration variables related to the xwin
# device driver.
# The following variables are set/modified:
# PLD_xwin		  - ON means the xwin device is enabled.
# xwin_COMPILE_FLAGS	  - individual COMPILE_FLAGS required to compile xwin
# 			    device.
# xwin_LINK_FLAGS	  - individual LINK_FLAGS for dynamic xwin device.
# DRIVERS_LINK_FLAGS	  - list of LINK_FLAGS for all static devices.
# PL_USE_PTHREADS_XWIN    - ON means use pthreads with xwin driver.
# PLPLOT_MUTEX_RECURSIVE  - Portable definition for PTHREAD_MUTEX_RECURSIVE
if(PLD_xwin)
  if(X11_FOUND)
    set(xwin_COMPILE_FLAGS "${X11_COMPILE_FLAGS}")
    # N.B. X11_LIBRARIES already converted to full path names with call to cmake_link_flags.
    set(xwin_LINK_FLAGS "${X11_LIBRARIES}")
    # This is deliberately set without Threads::Threads to avoid messing up pkg-config
    # results which indirectly depend on DRIVERS_LINK_FLAGS in src/CMakeLists.txt.
    # Of course, this also means that Threads has to be configured with special
    # logic in that file for both the configuration of the plplot library build
    # and also the configured pkg-config results for the plplot library.
    set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${xwin_LINK_FLAGS})

    if(PL_HAVE_PTHREADS)
      # Posix threads library has (previously) been found.
      option(PL_USE_PTHREADS_XWIN "Use pthreads with the xwin device driver" ON)
    else(PL_HAVE_PTHREADS)
      message(STATUS "WARNING: Posix threads library not found so cannot use pthreads with the xwin device driver")
      set(PL_USE_PTHREADS_XWIN OFF CACHE BOOL "Use pthreads with the xwin device driver" FORCE)
    endif(PL_HAVE_PTHREADS)

    if(PL_USE_PTHREADS_XWIN)
      set(xwin_LINK_FLAGS ${xwin_LINK_FLAGS} Threads::Threads)
      if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(PLPLOT_MUTEX_RECURSIVE "PTHREAD_MUTEX_RECURSIVE_NP")
      elseif(CMAKE_SYSTEM_NAME STREQUAL "kFreeBSD")
 	set(PLPLOT_MUTEX_RECURSIVE "PTHREAD_MUTEX_RECURSIVE_NP")
      else(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	set(PLPLOT_MUTEX_RECURSIVE "PTHREAD_MUTEX_RECURSIVE")
      endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    endif(PL_USE_PTHREADS_XWIN)
  else(X11_FOUND)
    set(PLD_xwin OFF CACHE BOOL "Enable xwin device" FORCE)
  endif(X11_FOUND)
endif(PLD_xwin)

