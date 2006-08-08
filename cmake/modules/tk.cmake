# cmake/modules/tk.cmake
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

# Module for determining all configuration variables related to the Tk
# related device drivers (tk, ntk, tkwin).
# The following variables are set/modified for the ntk device:
# PLD_ntk		  - ON means the xwin device is enabled.
# ntk_COMPILE_FLAGS	  - individual COMPILE_FLAGS required to compile ntk
# 			    device.
# ntk_LINK_FLAGS	  - individual LINK_FLAGS for dynamic ntk device.
# DRIVERS_LINK_FLAGS	  - list of LINK_FLAGS for all static devices.

#temporary
set(PLD_tk OFF CACHE BOOL "Enable tk device" FORCE)

if(NOT ENABLE_tk)
  message(STATUS "WARNING: ENABLE_tk OFF.  Setting PLD_tk and PLD_ntk OFF.")
  set(PLD_tk OFF CACHE BOOL "Enable tk device" FORCE)
  set(PLD_ntk OFF CACHE BOOL "Enable ntk device" FORCE)
endif(NOT ENABLE_tk)

if(PLD_ntk)
  set(ntk_COMPILE_FLAGS "-I${TCL_INCLUDE_PATH} -I${TK_INCLUDE_PATH}")
  set(ntk_LINK_FLAGS ${TCL_LIBRARY} ${TK_LIBRARY})
  set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${ntk_LINK_FLAGS})
endif(PLD_ntk)

if(not_implemented_yet)
if(PLD_xwin)
  find_package(X11)
  if(X11_FOUND)
    string(REGEX REPLACE ";" ";-I" 
    xwin_COMPILE_FLAGS
    "-I${X11_INCLUDE_DIR}"
    )
    # N.B. X11_LIBRARY_DIR is used internally by FindX11.cmake but not
    # documented for external use so we may have to replace this
    # some day by the appropriate 
    # "GET_FILENAME_COMPONENT(X11_LIBRARY_DIR ??? PATH)" logic.
    # But this works for now....
    set(xwin_LINK_FLAGS "-L${X11_LIBRARY_DIR} ${X11_LIBRARIES}")
    option(HAVE_PTHREAD "Use pthreads with the xwin driver" OFF)
    if(HAVE_PTHREAD)
      find_package(Threads)
      if(CMAKE_USE_PTHREADS_INIT)
        set(xwin_LINK_FLAGS ${xwin_LINK_FLAGS} ${CMAKE_THREAD_LIBS_INIT})
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
    set(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${xwin_LINK_FLAGS})
  else(X11_FOUND)
    set(PLD_xwin OFF CACHE BOOL "Enable xwin device" FORCE)
  endif(X11_FOUND)
endif(PLD_xwin)
endif(not_implemented_yet)
