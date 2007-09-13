# cmake/modules/drivers-init.cmake
#
# Start driver initializations
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

# Module to initialize device options.

# Results are contained in the following variables:
# PLD_devicename (ON or OFF): whether each PLplot-related device is enabled
#   or not.  devicename is png, jpeg, etc.
# DRIVERS_DEVICE_LIST: see below for the definition

# We think of a "device" as the output medium.  Could be a machine
# style (Tektronix, X11), or a file format (Postscript).
#
# A "driver" is the PLplot code that produces the output on the
# medium.
#
# Some drivers support multiple devices.  But we we need to make sure
# we compile only the selected drivers.  That is, if a driver supports
# devices not selected, we don't want to compile it at all.  Or if it
# supports multiple selected devices, we only want to compile it
# once.  To support this, we build an association list between devices
# and drivers.  This will be inspected to build up the set of drivers
# to be compiled based on which devices are selected.

option(DEFAULT_NO_DEVICES
"Disable all (ON) or enable individually (OFF) devices by default"
OFF
)

option(DEFAULT_ALL_DEVICES
"Enable all (ON) or enable individually (OFF) devices by default"
OFF
)

if(DEFAULT_NO_DEVICES AND DEFAULT_ALL_DEVICES)
  message(STATUS
  "WARNING. DEFAULT_NO_DEVICES ON supersedes DEFAULT_ALL_DEVICES ON.")
  set(DEFAULT_ALL_DEVICES OFF
  CACHE BOOL
  "Enable all (ON) or enable individually (OFF) devices by default"
  FORCE
  )
endif(DEFAULT_NO_DEVICES AND DEFAULT_ALL_DEVICES)


# The DRIVERS_DEVICE_LIST defined below is a list of
# <device>:<drive>:<enable_default> items.  <enable_default> should be
# "ON" or "OFF" and this will reflect in inclusion/exclusion by default
# (as shown by ccmake).
#
# Ordered alphabetically by second in each pair (the driver) for human
# consumption, but this is not necessary.

set(DRIVERS_DEVICE_LIST
"aqt:aqt:ON"
# memcairo does not work and svgcairo does not render text so turn them off
# by default.  The remaining cairo devices work well so turn them on by
# default.
"memcairo:cairo:OFF"
"pdfcairo:cairo:ON"
"pngcairo:cairo:ON"
"pscairo:cairo:ON"
"svgcairo:cairo:OFF"
"xcairo:cairo:ON"
"cgm:cgm:ON"
"dg300:dg300:OFF"
"png:gd:ON" "jpeg:gd:ON" "gif:gd:ON"
# Do not implement gnome which is superseded by gcw
#"gnome:gnome:OFF"
"hp7470:hpgl:ON" "hp7580:hpgl:ON" "lj_hpgl:hpgl:ON"
"imp:impress:OFF"
"gcw:gcw:ON"
# Default off because poorly maintained (colours are incorrect)
# must use software fill, and must run as root.
"linuxvga:linuxvga:OFF"
"ljii:ljii:OFF"
"ljiip:ljiip:OFF"
"mem:mem:ON"
"ntk:ntk:OFF"
"null:null:ON"
"pbm:pbm:ON"
"pdf:pdf:OFF"
# (2007-09-01) As discussed on list, don't enable plmeta until we sort
# out the known issues (e.g., strings, aspect ratio, and TrueType fonts).
# This is going to take time/energy for some volunteer who has not volunteered
# yet.... :-)
"plmeta:plmeta:OFF"
"ps:ps:ON"
# No longer segfaults, but still default OFF because cleaner/better ways 
# (psttf and pscairo) to get modern fonts for postscript results.
"pstex:pstex:OFF"
"psttf:psttf:ON"
"xterm:tek:OFF" "tek4010:tek:OFF" "tek4107:tek:OFF" "mskermit:tek:OFF"
"versaterm:tek:OFF" "vlt:tek:OFF" "conex:tek:OFF" "tek4010f:tek:OFF"
"svg:svg:ON"
"tek4107f:tek:OFF"
"tk:tk:ON"
"tkwin:tkwin:ON"
"wingcc:wingcc:ON"
"wxwidgets:wxwidgets:ON"
"xfig:xfig:ON"
"xwin:xwin:ON"
)

set(PRESET_DEFAULT OFF)

if(DEFAULT_ALL_DEVICES)
  set(DEFAULT ON)
  set(PRESET_DEFAULT ON)
endif(DEFAULT_ALL_DEVICES)

if(DEFAULT_NO_DEVICES)
  set(DEFAULT OFF)
  set(PRESET_DEFAULT ON)
endif(DEFAULT_NO_DEVICES)

foreach(DRIVERS_DEVICE ${DRIVERS_DEVICE_LIST})
  string(REGEX REPLACE "^(.*):.*:.*$" "\\1" DEVICE ${DRIVERS_DEVICE})
  if(NOT PRESET_DEFAULT)
    string(REGEX REPLACE "^.*:.*:(.*)$" "\\1" DEFAULT ${DRIVERS_DEVICE})
  endif(NOT PRESET_DEFAULT)
  option(PLD_${DEVICE} "Enable ${DEVICE} device" ${DEFAULT})
endforeach(DRIVERS_DEVICE)
