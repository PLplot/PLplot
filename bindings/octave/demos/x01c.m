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

# Simple line plot and multiple windows demo.

1;

function ix01c

global device file

  if (!exist("plinit"))
    plplot_stub
  endif

  if (exist("device"))
    plsdev(device);
  else
    plsdev("xwin");
  endif

  if (exist("file"))
    plsfnam(file);
  endif

  ## plplot initialization */
  ## Divide page into 2x2 plots unless user overrides */

  plssub(2, 2);

  ## Parse and process command line arguments */

  ##    plMergeOpts(options, "x01c options", notes);
  ##    plParseOpts(&argc, argv, PL_PARSE_FULL);

  ## Get version number, just for kicks */

  ver=plgver';
  printf("Plplot library version: %s\n", ver);

  ## Initialize plplot */
  plinit;

  ## Set up the data */
  ## Original case */

  xscale = 6.;
  yscale = 1.;
  xoff = 0.;
  yoff = 0.;

  ## Do a plot */

  plot1(xscale, yscale, xoff, yoff);

  ## Set up the data */

  xscale = 1.;
  yscale = 0.0014;
  yoff = 0.0185;

  ## Do a plot */

  digmax = 5;
  plsyax(digmax, 0);

  plot1(xscale, yscale, xoff, yoff);

  plot2;

  plot3;

  ## Let's get some user input */

  ## the "gin" structure is as follows:
				#
  ## int state;     /* key or button mask */
  ## int keysym;    /* key selected */
  ## int button;    /* mouse button selected */
  ## char string; 	 /* translated string */
  ## int pX, pY;    /* absolute device coordinates of pointer */
  ## float dX, dY;  /* relative device coordinates of pointer */
  ## float wX, wY;  /* world coordinates of pointer */

  if (!exist("file") || (exist("file") && isempty(file)))
    printf("\n\nYou are in Locate mode. Click any mouse button or press any key\n\
	and a printout will give you some info. Terminate with the <Esc> ot <Enter> key\n\
	or clicking or pressing a key while the cursor is outside a sub-plot window.\n\n\
	Dont't forget to finish the plot with the <Enter> key\n");
    fflush(stdout);

    while(1)

      [status, gin.state, gin.keysym, gin.button, gin.string, gin.pX, gin.pY, \
       gin.dX, gin.dY, gin.wX, gin.wY] = plGetCursor;

      if (status == 0) break; endif
      if (gin.keysym == hex2dec("1B")) break; endif

      if (gin.keysym < hex2dec("FF") && isprint(gin.keysym)) 
	printf("wx = %f,  wy = %f, dx = %f,  dy = %f,  c = '%c'\n",
	       gin.wX, gin.wY, gin.dX, gin.dY, gin.keysym);
      else
	printf("wx = %f,  wy = %f, dx = %f,  dy = %f,  c = 0x%02x\n",
	       gin.wX, gin.wY, gin.dX, gin.dY, gin.keysym);
      endif
      fflush(stdout);
    endwhile
  endif
  ## Don't forget to call plend  to finish off! */

  plend;

endfunction

function plot1(xscale, yscale, xoff, yoff)

  m=60;
  
  for i=1:m
    x(i) = xoff + xscale * (i + 1) / m;
    y(i) = yoff + yscale * (x(i).^2.);
  endfor

  xmin = min(x);
  xmax = max(x);
  ymin = min(y);
  ymax = max(y);

  n=5;
  
  for i=1:n
    xs(i) = x(i * 10 + 3);
    ys(i) = y(i * 10 + 3);
  endfor

  ## Set up the viewport and window using PLENV. The range in X is 
  ## * 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are 
  ## * scaled separately (just = 0), and we just draw a labelled 
  ## * box (axis = 0). 

  plcol0(1);
  plenv(xmin, xmax, ymin, ymax, 0, 0);
  plcol0(2);
  pllab("(x)", "(y)", "#frPLplot Example 1 - y=x#u2");

  ## Plot the data points */

  plcol0(4);
  plpoin(xs, ys, 9);

  ## Draw the line through the data */

  plcol0(3);
  plline(x, y);

endfunction

function plot2 

  ## Set up the viewport and window using PLENV. The range in X is -2.0 to
  ## * 10.0, and the range in Y is -0.4 to 2.0. The axes are scaled separately
  ## * (just = 0), and we draw a box with axes (axis = 1). 

  plcol0(1);
  plenv(-2.0, 10.0, -0.4, 1.2, 0, 1);
  plcol0(2);
  pllab("(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function");

  ## Fill up the arrays */

  m=100;
  
  for i=1:m
    x(i) = (i - 19.0) / 6.0;
    y(i) = 1.0;
    if (x(i) != 0.0)
      y(i) = sin(x(i)) / x(i);
    endif
  endfor

  ## Draw the line */

  plcol0(3);
  plline(x, y);

endfunction

function plot3 

  space0 = 0; mark0 = 0; space1 = 1500; mark1 = 1500;

  ## For the final graph we wish to override the default tick intervals, and
  ## * so do not use plenv .

  pladv(0);

  ## Use standard viewport, and define X range from 0 to 360 degrees, Y range
  ## * from -1.2 to 1.2. 

  plvsta;
  plwind(0.0, 360.0, -1.2, 1.2);

  ## Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y. */

  plcol0(1);
  plbox("bcnst", 60.0, 2, "bcnstv", 0.2, 2);

  ## Superimpose a dashed line grid, with 1.5 mm marks and spaces. 

  plstyl(1, mark1, space1);
  plcol0(2);
  plbox("g", 30.0, 0, "g", 0.2, 0);
  plstyl(0, mark0, space0);

  plcol0(3);
  pllab("Angle (degrees)", "sine", "#frPLplot Example 1 - Sine function");

  for i=1:100
    x(i) = 3.6 * i;
    y(i) = sin(x(i) * pi / 180.0);
  endfor

  plcol0(4);
  plline(x, y);
  
endfunction

ix01c


