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

## Does several contour plots using different coordinate mappings.

1;

global	XPTS=35;		## Data points in x */
global	YPTS=46;		## Datat points in y */

global	tr = [2/(XPTS-1); 0.0; -1.0; 0.0; 2/(YPTS-1); -1.0];	

function [tx ty] = mypltr(x, y)

  global	XPTS
  global	YPTS
  global	tr

  tx = tr(1) * x + tr(2) * y + tr(3);
  ty = tr(4) * x + tr(5) * y + tr(6);
  
endfunction

function ix09c

  if (!exist("plinit"))
    plplot_stub
  endif

  if(isempty(sprintf("%s",plgdev')))
    plsdev("xwin");
  endif

  global	XPTS
  global	YPTS
  global	tr
  
  clevel = linspace(-1,1,11)';

  mark = 1500; space = 1500;

  ## Parse and process command line arguments */

  ##    (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

  ## Initialize plplot */

  plinit();

  ## Set up function arrays */

  for i=0:XPTS-1
    xx = (i - (XPTS / 2)) /  (XPTS / 2);
    yy = ((0:YPTS-1) - (YPTS / 2)) / (YPTS / 2) - 1.0;
    z(i+1,:) = xx * xx - yy .* yy;
    w(i+1,:) = 2 * xx .* yy;
  endfor

  ## Plot using identity transform */

  pl_setcontlabelparam(0.006, 0.3, 0.1, 0);
  plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
  plcol0(2);
  plcont(z, 1, XPTS, 1, YPTS, clevel, tr);
  plstyl(1, mark, space);
  plcol0(3);
  plcont(w, 1, XPTS, 1, YPTS, clevel, tr);
  plstyl(0, mark, space);
  plcol0(1);
  pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

  pl_setcontlabelparam(0.006, 0.3, 0.1, 1);
  plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
  plcol0(2);
  plcont(z, 1, XPTS, 1, YPTS, clevel, tr);
  plstyl(1, mark, space);
  plcol0(3);
  plcont(w, 1, XPTS, 1, YPTS, clevel, tr);
  plstyl(0, mark, space);
  plcol0(1);
  pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

  ## Set up grids */

  for i=0:XPTS-1
    [xx yy] = mypltr(i, (0:YPTS-1));

    argx = xx * pi/2;
    argy = yy * pi/2;
    distort = 0.4;

    xg3(i+1,:) = xx .+ distort .* cos(argx);
    yg3(i+1,:) = yy .- distort .* cos(argy);
    
    xg2(i+1,:) = xx .+ distort .* cos(argx) .* cos(argy);
    yg2(i+1,:) = yy .- distort .* cos(argx) .* cos(argy);
  endfor

  xg1 = xg3(:,1);
  yg1 = yg3(XPTS,:)';


  ## Plot using 1d coordinate transform */

  pl_setcontlabelparam(0.006, 0.3, 0.1, 0);
  plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
  plcol0(2);
  plcont1(z, 1, XPTS, 1, YPTS, clevel, xg1, yg1);
  plstyl(1, mark, space);
  plcol0(3);
  plcont1(w, 1, XPTS, 1, YPTS, clevel, xg1, yg1);
  plstyl(0, mark, space);
  plcol0(1);
  pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

  pl_setcontlabelparam(0.006, 0.3, 0.1, 1);
  plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
  plcol0(2);
  plcont1(z, 1, XPTS, 1, YPTS, clevel, xg1, yg1);
  plstyl(1, mark, space);
  plcol0(3);
  plcont1(w, 1, XPTS, 1, YPTS, clevel, xg1, yg1);
  plstyl(0, mark, space);
  plcol0(1);
  pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

  ## Plot using 2d coordinate transform */

  pl_setcontlabelparam(0.006, 0.3, 0.1, 0);
  plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
  plcol0(2);
  plcont2(z, 1, XPTS, 1, YPTS, clevel, xg2, yg2);
  plstyl(1, mark, space);
  plcol(3);
  plcont2(w, 1, XPTS, 1, YPTS, clevel, xg2, yg2);
  plstyl(0, mark, space);
  plcol(1);
  pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");
  
  pl_setcontlabelparam(0.006, 0.3, 0.1, 1);
  plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
  plcol0(2);
  plcont1(z, 1, XPTS, 1, YPTS, clevel, xg1, yg1);
  plstyl(1, mark, space);
  plcol0(3);
  plcont1(w, 1, XPTS, 1, YPTS, clevel, xg1, yg1);
  plstyl(0, mark, space);
  plcol0(1);
  pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");
  
  plend();

endfunction

ix09c
