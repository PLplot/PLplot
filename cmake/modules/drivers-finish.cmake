# cmake/modules/drivers-finish.cmake
#
# Finalize driver initializations
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

# Module to finalize device options.

# Results are contained in the following variables:
# DEVICES_LIST: list of devices (e.g. png, jpeg),where PLD_devicename is ON
# DRIVERS_LIST: list of device drivers (e.g., gd for the png and jpeg devices)
#   where at least one of the associated devices is enabled.

foreach(DRIVERS_DEVICE ${DRIVERS_DEVICE_LIST})
  string(REGEX REPLACE "^(.*):.*:.*$" "\\1" DEVICE ${DRIVERS_DEVICE})
  string(REGEX REPLACE "^.*:(.*):.*$" "\\1" DRIVER ${DRIVERS_DEVICE})
  if(PLD_${DEVICE})
    set(DEVICES_LIST ${DEVICES_LIST} ${DEVICE})
    set(APPEND_DRIVER ON)
    foreach(DRIVER_IN_LIST ${DRIVERS_LIST})
      if(DRIVER STREQUAL "${DRIVER_IN_LIST}")
        set(APPEND_DRIVER OFF)
      endif(DRIVER STREQUAL "${DRIVER_IN_LIST}")
    endforeach(DRIVER_IN_LIST ${DRIVERS_LIST})
    if(APPEND_DRIVER)
      if(ENABLE_DYNDRIVERS)
        file(STRINGS
	  ${CMAKE_SOURCE_DIR}/drivers/${DRIVER}.rc.in
	  ${DRIVER}_INFO
	  )
      endif(ENABLE_DYNDRIVERS)
      set(DRIVERS_LIST ${DRIVERS_LIST} ${DRIVER})
      if(DRIVER STREQUAL "wxwidgets")
        set(${DRIVER}_SOURCE 
          ${CMAKE_SOURCE_DIR}/drivers/${DRIVER}.cpp
	        ${CMAKE_SOURCE_DIR}/drivers/${DRIVER}_app.cpp
          ${CMAKE_SOURCE_DIR}/drivers/${DRIVER}_dc.cpp
          ${CMAKE_SOURCE_DIR}/drivers/${DRIVER}_gc.cpp
          ${${DRIVER}_SOURCE}
	      )
        if(HAVE_AGG)
          set(${DRIVER}_SOURCE 
            ${CMAKE_SOURCE_DIR}/drivers/${DRIVER}_agg.cpp
            ${${DRIVER}_SOURCE}
          )
        endif(HAVE_AGG)
      elseif(DRIVER STREQUAL "psttf")
        set(${DRIVER}_SOURCE
	${CMAKE_SOURCE_DIR}/drivers/${DRIVER}.cc
	${${DRIVER}_SOURCE}
	)
	elseif(DRIVER STREQUAL "qt")
        set(${DRIVER}_SOURCE
	${CMAKE_SOURCE_DIR}/drivers/${DRIVER}.cpp
	${${DRIVER}_SOURCE}
	)
      else(DRIVER STREQUAL "wxwidgets")
        set(${DRIVER}_SOURCE
	${CMAKE_SOURCE_DIR}/drivers/${DRIVER}.c
	${${DRIVER}_SOURCE}
	)
      endif(DRIVER STREQUAL "wxwidgets")
    endif(APPEND_DRIVER)
  endif(PLD_${DEVICE})
endforeach(DRIVERS_DEVICE)

foreach(DRIVERS_DEVICE ${DRIVERS_DEVICE_LIST})
  string(REGEX REPLACE "^(.*):.*:.*$" "\\1" DEVICE ${DRIVERS_DEVICE})
  string(REGEX REPLACE "^.*:(.*):.*$" "\\1" DRIVER ${DRIVERS_DEVICE})
  if(${DRIVER}_INFO)
    if(NOT PLD_${DEVICE})
      set(DEVICE_INFO_MATCHED)
      # Must remove corresponding data from ${DRIVER}_INFO
      #message("DEVICE = ${DEVICE}")
      foreach(DEVICE_INFO ${${DRIVER}_INFO})
        string(REGEX REPLACE "^(.*):.*:.*:.*:.*:.*$" "\\1" DEVICE_INFO_NAME ${DEVICE_INFO})
        #message("DEVICE_INFO_NAME = ${DEVICE_INFO_NAME}")
	if(DEVICE STREQUAL "${DEVICE_INFO_NAME}")
	  #There should one and only one match.
	  set(DEVICE_INFO_MATCHED ${DEVICE_INFO})
	endif(DEVICE STREQUAL "${DEVICE_INFO_NAME}")
      endforeach(DEVICE_INFO ${${DRIVER}_INFO})
      if(DEVICE_INFO_MATCHED)
        list(REMOVE_ITEM ${DRIVER}_INFO ${DEVICE_INFO_MATCHED})
      else(DEVICE_INFO_MATCHED)
        message(FATAL_ERROR "${CMAKE_SOURCE_DIR}/drivers/${DRIVER}.rc.in not consistent with ${CMAKE_SOURCE_DIR}/cmake/modules/drivers-init.cmake")
      endif(DEVICE_INFO_MATCHED)
    endif(NOT PLD_${DEVICE})
  endif(${DRIVER}_INFO)
endforeach(DRIVERS_DEVICE)

foreach(DRIVERS_DEVICE ${DRIVERS_DEVICE_LIST})
  string(REGEX REPLACE "^.*:(.*):.*$" "\\1" DRIVER ${DRIVERS_DEVICE})
  if(${DRIVER}_INFO)
    file(WRITE ${CMAKE_BINARY_DIR}/drivers/${DRIVER}.rc "")
    foreach(DEVICE_INFO ${${DRIVER}_INFO})
      file(APPEND ${CMAKE_BINARY_DIR}/drivers/${DRIVER}.rc "${DEVICE_INFO}\n")
    endforeach(DEVICE_INFO ${${DRIVER}_INFO})
  endif(${DRIVER}_INFO)
endforeach(DRIVERS_DEVICE ${DRIVERS_DEVICE_LIST})
