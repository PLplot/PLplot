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

##	plshade demo, using color fill.

1;

## Set up transformation function */

global nx = 35;		## Default number of data points in x */
global ny = 46;		## Default number of data points in y */
global	tr = [2/(nx-1); 0.0; -1.0; 0.0; 2/(ny-1); -1.0];

function  [tx ty] = mypltr( x, y)
  global tr
  tx = tr(1) * x + tr(2) * y + tr(3);
  ty = tr(4) * x + tr(5) * y + tr(6);
endfunction

function ix16c

  if (!exist("plinit"))
    plplot_stub
  endif

  if(isempty(sprintf("%s",plgdev')))
    plsdev("xwin");
  endif

  global tr; global nx; global ny;
  ## Fundamental settings.  See notes[] for more info. */

  ns = 20;		## Default number of shade levels */

  notes = [
	   "To get smoother color variation, increase ns, nx, and ny.  To get faster",
	   "response (especially on a serial link), decrease them.  A decent but quick",
	   "test results from ns around 5 and nx, ny around 25."];

  sh_cmap = 1;
  min_color = 1; min_width = 0; max_color = 0; max_width = 0;

  ## Parse and process command line arguments */

  ##    plMergeOpts(options, "x16c options", notes);
  ##    plParseOpts(&argc, argv, PL_PARSE_FULL);

  ## Reduce colors in cmap 0 so that cmap 1 is useful on a 16-color display */

  plscmap0n(3);

  ## Initialize plplot */
  plinit();

  ## Set up data array */

  for i = 0:nx-1
    x = (i - (nx / 2)) / (nx / 2);
    j = 0:ny-1;
    y = (j .- (ny / 2)) ./ (ny / 2) - 1.0;

    z(i+1,:) = - sin(7.*x) .* cos(7.*y) .+ x*x - y.*y;
    w(i+1,:) = - cos(7.*x) .* sin(7.*y) .+ 2 .* x .* y;
  endfor

  zmin=min(min(z));
  zmax=max(max(z));

  i = 0:ns-1;
  clevel = (zmin .+ (zmax - zmin) .* (i + 0.5) ./ ns)';
  
  ## Set up coordinate grids */

  for i = 0:nx-1
    j = 0:ny-1;
    [x y] = mypltr( i,  j);

    argx = x * pi/2;
    argy = y * pi/2;
    distort = 0.4;

    xg3(i+1,:) = x .+ distort .* cos(argx);
    yg3(i+1,:) = y .- distort .* cos(argy);

    xg2(i+1,:) = x .+ distort .* cos(argx) .* cos(argy);
    yg2(i+1,:) = y .- distort .* cos(argx) .* cos(argy);
  endfor

  xg1 = xg3(:,1);
  yg1 = yg3(nx,:)';
  
  ## Plot using identity transform */

  pladv(0);
  plvpor(0.1, 0.9, 0.1, 0.9);
  plwind(-1.0, 1.0, -1.0, 1.0);

  for i = 0:ns-1
    shade_min = zmin + (zmax - zmin) * i / ns;
    shade_max = zmin + (zmax - zmin) * (i +1) / ns;
    sh_color = i / (ns-1);
    sh_width = 2;
    plpsty(0);

    plshade(z, "0", -1., 1., -1., 1., 
	    shade_min, shade_max, 
	    sh_cmap, sh_color, sh_width,
	    min_color, min_width, max_color, max_width, 1, zeros(6,1));
  endfor

  plcol(1);
  plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
  plcol(2);
  
  plcont(w, 1, nx, 1, ny, clevel, tr);
  
  pllab("distance", "altitude", "Bogon density 1");

  ## Plot using 1d coordinate transform */

  pladv(0);
  plvpor(0.1, 0.9, 0.1, 0.9);
  plwind(-1.0, 1.0, -1.0, 1.0);

  for i = 0:ns-1
    shade_min = zmin + (zmax - zmin) * i / ns;
    shade_max = zmin + (zmax - zmin) * (i +1) / ns;
    sh_color = i / (ns-1);
    sh_width = 2;
    plpsty(0);

    plshade1(z, "0", -1., 1., -1., 1., 
	     shade_min, shade_max, 
	     sh_cmap, sh_color, sh_width,
	     min_color, min_width, max_color, max_width,
	     1, xg1, yg1);
  endfor

  plcol(1);
  plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
  plcol(2);

  plcont1(w, 1, nx, 1, ny, clevel, xg1, yg1);

  pllab("distance", "altitude", "Bogon density 2");

  ## Plot using 2d coordinate transform */

  pladv(0);
  plvpor(0.1, 0.9, 0.1, 0.9);
  plwind(-1.0, 1.0, -1.0, 1.0);

  for i = 0:ns-1
    shade_min = zmin + (zmax - zmin) * i /  ns;
    shade_max = zmin + (zmax - zmin) * (i +1) / ns;
    sh_color = i / (ns-1);
    sh_width = 2;
    plpsty(0);

    plshade2(z, "0", -1., 1., -1., 1., 
	     shade_min, shade_max, 
	     sh_cmap, sh_color, sh_width,
	     min_color, min_width, max_color, max_width,
	     0, xg2, yg2);
  endfor

  plcol(1);
  plbox("bcnst", 0.0, 0, "bcnstv", 0.0, 0);
  plcol(2);
  plcont2(w, 1, nx, 1, ny, clevel, xg2, yg2);

  pllab("distance", "altitude", "Bogon density, with streamlines");

  ## Clean up */

  plend();

endfunction

ix16c
