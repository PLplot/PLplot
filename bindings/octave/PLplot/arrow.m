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

## arrow (coord, mag, rot, col)
## arrow (col)
##
## plot one arrow of color col, magnitude mag, rotation rot
## with origin at point coord[2]
## magnitude = 1 means arrow lenght = 1 in world coordinates
##
## with only one argument, uses the mouse to define begin and end points
##
## NOTE: preliminary

function arrow (coord, mag, rot, col)

if (nargin == 1)	# col

[x1, y1, b1] = ginput(1);
if (b1 == 3)
	axis;
	return;
endif
	
[x2, y2, b2] = ginput(1);
	xi = min(x1, x2); yi = min(y1, y2);
	xf = max(x1, x2); yf = max(y1, y2);
	mag = sqrt((xf-xi).^2 + (yf-yi).^2);
	rot = 180*atan((yf-yi)/(xf-xi))/pi;	# FIXME
	arrow([xi, yi], mag, rot, coord);
	return
endif

if (nargin == 4)

	x = [0; 1; 0.75; 1; 0.75];
	y = [0; 0; 0.1; 0; -0.1];

	rot = -rot*pi/180;
	
	t = [cos(rot), -sin(rot)
		sin(rot), cos(rot)];
	
	xx = (x .* t(1,1) .+ y .* t(2,1)) .* mag + coord(1);
	yy = (x .* t(1,2) .+ y .* t(2,2)) .* mag + coord(2);
	plcol(col);plline(xx,yy);plflush;pleop;

endif	


endfunction
