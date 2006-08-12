# cmake/modules/aqt.cmake
#
# Copyright (C) 2006  Hazen Babcock
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

# Configuration for the aquaterm device driver
#
# The following variables are set / modified
#
# PLD_aqt             - ON means the aqt device is enabled
# aqt_COMPILE_FLAGS   - COMPILE_FLAGS required to compile aqt device driver.
# aqt_LINK_FLAGS      - LINK_FLAGS for dynamic aqt device driver.
# DRIVERS_LINK_FLAGS  - list of LINK_FLAGS for all static device drivers.
#

include(FindAQT)

IF (AQT_FOUND)
	MESSAGE(STATUS "Found AQT: ${AQT_FRAMEWORK}")
	IF (PLD_aqt)
		SET(aqt_COMPILE_FLAGS "-ObjC")
		SET(aqt_LINK_FLAGS "-framework AquaTerm -framework Foundation")
		SET(DRIVERS_LINK_FLAGS ${DRIVERS_LINK_FLAGS} ${aqt_LINK_FLAGS})
	ENDIF (PLD_aqt)
ELSE (AQT_FOUND)
	SET(PLD_aqt OFF CACHE BOOL "Enable aqt device" FORCE)
ENDIF (AQT_FOUND)
