## Copyright (C) 1998, 1999, 2000 Joao Cardoso.
## 
## This program is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by the
## Free Software Foundation; either version 2 of the License, or (at your
## option) any later version.
## 
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## This file is part of plplot_octave.

# zoom(arg)
#
# No, it is just a frontend to set axis :(
# Select left botton and right top area for setting axis.
# Clicking with right mouse button or arg == "off" set axis "autoscale"
# Due to a bug in ginput, the cross-hair remains in the plot after zoom terminate
# Use the tk driver for zooming and panning.

function zoom(arg)

if (nargin == 1)
	if strcmp(arg, "off")
		axis;
		return
	endif
endif
	
global __pl

[x1, y1, b1] = ginput(1)
if (b1 == 3)
	axis;
	return;
endif
	
[x2, y2, b2] = ginput(1)

if (b2 == 3)
	axis;
	return
else
# FIXME: this is buggy
	if (__pl.type == 11 || __pl.type == 31)
		x1 = 10^x1; x1 = 10^x2;
	endif
	if (__pl.type == 21 || __pl.type == 31)
		y1 = 10^y1; y2 = 10^y2;
	endif
	axis([min(x1, x2), max(x1, x2), min(y1, y2), max(y1, y2)]);
endif


endfunction
