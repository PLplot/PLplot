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
## It is based on the corresponding demo function of PLplot.

## Does a series of mesh plots for a given data set, with different
## viewing options in each plot.

1;

function go

  XPTS = 35;		## Data points in x */
  YPTS = 46;		## Datat points in y */

  opt = [1, 2, 3, 3]';
  alt = [60.0, 20.0, 60.0, 60.0]';
  az = [30.0, 60.0, 120.0, 160.0]';

  title = ["#frPLplot Example 11 - Alt=60, Az=30, Opt=1",
	   "#frPLplot Example 11 - Alt=20, Az=60, Opt=2",
	   "#frPLplot Example 11 - Alt=60, Az=120, Opt=3",
	   "#frPLplot Example 11 - Alt=60, Az=160, Opt=3"];

  ## Parse and process command line arguments */

  ## (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

  ## Initialize plplot */

  global pldevice
  plsdev(pldevice)
  
  plinit();

  for i=0: XPTS-1
    x(i+1) =   (i - (XPTS / 2)) /   (XPTS / 2);
  endfor

  for i=0:YPTS-1
    y(i+1) =   (i - (YPTS / 2)) /   (YPTS / 2);
  endfor
  
  for i=0:XPTS-1
    xx = x(i+1);
    for j=0:YPTS-1
      yy = y(j+1);
      z(i+1,j+1) = cos(2.0 * pi * xx) * sin(2.0 * pi * yy);
    endfor
  endfor

  for k=0:3
    pladv(0);
    plcol0(1);
    plvpor(0.0, 1.0, 0.0, 0.8);
    plwind(-1.0, 1.0, -1.0, 1.5);
    
    plw3d(1.0, 1.0, 1.2, -1.0, 1.0, -1.0, 1.0, -1.5, 1.5, alt(k+1), az(k+1));
    plbox3("bnstu", "x axis", 0.0, 0,
	   "bnstu", "y axis", 0.0, 0,
	   "bcdmnstuv", "z axis", 0.0, 4);
    
    plcol0(2);
    plmesh(x, y, z, opt(k+1));
    plcol0(3);
    plmtex("t", 1.0, 0.5, 0.5, title(k+1,:));
  endfor
  plend();
endfunction

go
