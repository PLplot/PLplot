# cmake/modules/xwin.cmake
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

# Module for determining all configuration variables related to the xwin
# device driver.
# The following variables are set/modified:
# PLD_xwin		  - ON means the xwin device is enabled.
# xwin_INCLUDE_DIR	  - individual INCLUDE_DIR required to compile xwin
# 			    device.
# xwin_LIBRARIES	  - individual LIBRARIES for dynamic xwin device.
# DRIVERS_LIBRARIES	  - list of LIBRARIES for all static devices.
# HAVE_PTHREAD		  - ON means use pthreads with xwin driver.
# PLPLOT_MUTEX_RECURSIVE  - Portable definition for PTHREAD_MUTEX_RECURSIVE
if(PLD_xwin)
  find_package(X11)
  if(X11_FOUND)
    set(xwin_INCLUDE_DIR ${X11_INCLUDE_DIR})
    set(xwin_LIBRARIES ${X11_LIBRARIES})
    option(HAVE_PTHREAD "use pthreads with the xwin driver" OFF)
    if(HAVE_PTHREAD)
      find_package(Threads)
      if(CMAKE_USE_PTHREADS_INIT)
        set(xwin_LIBRARIES ${xwin_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
	if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	  set(PLPLOT_MUTEX_RECURSIVE "PTHREAD_MUTEX_RECURSIVE_NP")
	else(CMAKE_SYSTEM_NAME STREQUAL "Linux")
	  set(PLPLOT_MUTEX_RECURSIVE "PTHREAD_MUTEX_RECURSIVE")
	endif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
      else(CMAKE_USE_PTHREADS_INIT)
        # I am being super-careful here to follow the autotools model.  In
        # fact, it is possible other thread systems will work as well as
	# pthreads.  So something to investigate for later.
        set(HAVE_PTHREAD OFF)
      endif(CMAKE_USE_PTHREADS_INIT)
    endif(HAVE_PTHREAD)
    set(DRIVERS_LIBRARIES ${DRIVERS_LIBRARIES} ${xwin_LIBRARIES})
  else(X11_FOUND)
    set(PLD_xwin OFF)
  endif(X11_FOUND)
endif(PLD_xwin)

