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
 	
### set_view (alt, az)
### set_view (script)
### set_view ()
###
### Set up the altitude and azimute in degrees for posterior 3d plots.
###
### If called without arguments, rotates iteratively a cube using the mouse.
### If called with one string argument, instead of a cube `script' is evaluated
### and should do a 3D plot.
###
### button 1 press marks new origin
### button 1,3 drag, rotate cube
### button 2 press terminate
### button 3 press default position
###
### example of callback `script'. Call as set_view("f")
###
### function f ()
###  static x, y, z;
###  if (!exist ("x"))
###    [x, y, z] = rosenbrock;
###    z = log (z);
###  else
###    mesh (x, y, z) # surfl (x, y, z) 
###  endif;
### endfunction

function set_view (alt, az)

  global	__pl __pl_inited 

  if (!exist("__pl_inited") || plglevel == 0)
	 	figure(0)
  endif

  __pl_strm = plgstrm + 1;

	callback = 0;
  if (nargin == 2)
	 	__pl.az(__pl_strm) = az;
	 	__pl.alt(__pl_strm) = alt;
	 	return
 	elseif (nargin == 1 && isstr(alt))
	 	script = alt;
	 	callback = 1;
  endif

  az = __pl.az(__pl_strm);
  alt =  __pl.alt(__pl_strm);

	if (!callback)
  	x = y = [0; 1];
  	z = ones(2);
  	xm = ym = zm = 0;
  	xM = yM = zM = 1;

  	plcol(15);

  	__pl_plenv(-1.5, 1.5, -1.5, 2.5, 0, -2);
  	plw3d(2, 2, 2, xm, xM, ym, yM, zm, zM, alt, az)

  	plmtex("t", 3, 0.5, 0.5, sprintf("Alt=%d   Az=%d", alt, az));

  	plbox3("bnstu", "X axis", 0.0, 0,"bnstu", "Y axis", 0.0, 0,"bdcmnstuv",
					 "Z axis", 0.0, 0)

  	plot3d(x,y,z,3,1);
  	plflush;pleop;

  	xm = ym = zm = 0;
  	xM = yM = zM = 1;
	else
		title(sprintf("Alt=%d   Az=%d", alt, az));
		eval(script)
	endif
  
  odx = ody = 0;
  c_alt = c_az = 0;
  
  while (1)
	 	[status, state, keysym, button, string, pX, pY, dX, dY, wX, wY] = plGetCursor;

	 	if (button == 3) # default position
			az = -60; alt = 30;
			ox = dX; oy = dY;
			c_alt = c_az = 0;
	 	elseif(button == 2) # stop
			break
	 	elseif (button == 1) # mark position
			ox = dX; oy = dY;
			alt = alt + c_alt; az = az + c_az;
			c_alt = c_az = 0;
	 	elseif (button == 0) # drag
			c_az = (dX-ox)*100;
			c_alt = (oy-dY)*100;
	 	endif
	 	
	 	if (alt + c_alt > 90)
			c_alt = 90 - alt;
	 	elseif (alt + c_alt < 0)
			c_alt = -alt;
	 	endif

		##	 if (az < -180)
		##		az = -180;
		## elseif (az > 180)
		##		az = 180;
		##	 endif

	 	if (!callback)
			__pl_plenv(-1.5, 1.5, -1.5, 2.5, 0, -2);

			plw3d(2, 2, 2, xm, xM, ym, yM, zm, zM, alt+c_alt, az+c_az)

			plbox3("bnstu", "x axis", 0.0, 0,"bnstu", "y axis", 0.0,
						 0,"bdcmnstuv", "z axis", 0.0, 0)

			plmtex("t", 3, 0.5, 0.5, sprintf("Alt=%d   Az=%d", alt+c_alt, az+c_az));

			plot3d(x,y,z,3,1);
			plflush;pleop;
		else
			__pl.az(__pl_strm) = az + c_az; 
			__pl.alt(__pl_strm) = alt + c_alt;
			title(sprintf("Alt=%d   Az=%d", alt+c_alt, az+c_az));
			eval(script)
		endif
		
  endwhile

  __pl.az(__pl_strm) = az + c_az; 
  __pl.alt(__pl_strm) = alt + c_alt;

  
endfunction
