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
## Does a series of 3-d plots for a given data set, with different
## viewing options in each plot.

1;

function cmap1_init(g)
  i = [0; 1];           # left boundary, right boundary
  if (g)
    h = [0; 0]; 	# hue -- low: red (arbitrary if s=0) -- high: red (arbitrary if s=0)
    l = [0.5; 1];	# lightness -- low: half-dark -- high: light
    s = [0; 0];		# minimum saturation 
  else
    h = [240; 0];       # blue -> green -> yellow -> red
    l = [0.6; 0.6];
    s = [0.8; 0.8];
  endif

  plscmap1n(256);
  plscmap1l(0, i, h, l, s, zeros(2,1));
endfunction

  function ix08c

    if (!exist("plinit"))
      plplot_stub
    endif

    if(isempty(sprintf("%s",plgdev')))
      plsdev("xwin");
    endif

    XPTS=35;		## Data points in x
    YPTS=46;		## Datat points in y

    MAG_COLOR = 4;
    SURF_CONT = hex2dec("10");
    BASE_CONT = hex2dec("20");
    FACETED = hex2dec("80");

    opt=[1, 2, 3, 3];
    alt=[60.0, 20.0, 60.0, 60.0];
    az =[30.0, 60.0, 120.0, 160.0];

    title=["#frPLplot Example 8 - Alt=60, Az=30",
	   "#frPLplot Example 8 - Alt=20, Az=60",
	   "#frPLplot Example 8 - Alt=60, Az=120",
	   "#frPLplot Example 8 - Alt=60, Az=160"];


    ## Parse and process command line arguments

    ## (void) plParseOpts(&argc, argv, PL_PARSE_FULL);

    ## Initialize plplot
    plinit();

    x = ((0:XPTS-1) - (XPTS / 2)) / (XPTS / 2);
    y = ((0:YPTS-1) - (YPTS / 2)) / (YPTS / 2);
    [xx, yy] = meshgrid(x,y);
    r = sqrt(xx .* xx + yy .* yy);
    z = exp(-r .* r) .* cos(2.0 * 3.141592654 .* r);

    pllightsource(1.,1.,1.);

    n_col = 100;
    plscmap1n(n_col);
    
    nlevel = 10;
    zmax = max(max(z));
    zmin = min(min(z));
    clevel = linspace(zmin, zmax, nlevel)';

    for k=1:4
      for ifshade=0:5

	pladv(0);
	plvpor(0.0, 1.0, 0.0, 0.9);
	plwind(-1.0, 1.0, -0.9, 1.1);
	plcol0(3);
	plmtex("t", 1.0, 0.5, 0.5, title(k,:));
	plcol0(1);
	plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, alt(k), az(k));
	plbox3("bnstu", "x axis", 0.0, 0,
	       "bnstu", "y axis", 0.0, 0,
	       "bcdmnstuv", "z axis", 0.0, 0);      
	plcol0(2);

	switch(ifshade)
	  case 0
	    plot3d(x', y', z', opt(k), 1);
	  case 1
	    cmap1_init(0);
	    plmesh(x', y', z', opt(k) + MAG_COLOR);
	  case 2
	    cmap1_init(1);
	    plsurf3d(x', y', z', 0, 0);
	  case 3
	    cmap1_init(0);
	    plsurf3d(x', y', z', MAG_COLOR, 0);
	  case 4
	    cmap1_init(0);
	    plsurf3d(x', y', z', MAG_COLOR + FACETED, 0);
	  otherwise
	    plsurf3d(x', y', z',MAG_COLOR + SURF_CONT + BASE_CONT, clevel);	    
	endswitch
      endfor
    endfor
    plend();
  endfunction

ix08c
