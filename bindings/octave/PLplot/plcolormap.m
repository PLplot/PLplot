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

## cmap = plcolormap([map])
## 
## Set colormap1 to map; map is an n row by 3 columns matrix, representing
##   red, green and blue components in the range [0..1]
##
## if map == 'default', returns and sets the default hsl colormap
## if map don't exist, return the current colormap

function ccmap = plcolormap(map)

  global __pl

  if (!struct_contains (__pl,"inited") || plglevel == 0)
    figure(0);
  endif

  if (nargin == 0)
    if (struct_contains(__pl, "colormap"))
      cmap = __pl.colormap;
    else
      ## try to obtain the default colormap (plctrl.c, plcmap1_def())
      if (0)
	plscmap1n(64); # 64 colors
	[r, g, b] = plgcolbg;
	vertex=(r+g+b)/3/255;
	if (vertex < 0.5)
	  vertex = 0.01;
	else
	  vertex = 0.99;
	endif
	
	i = [0; 0.45; 0.55; 1];	# intensity
	h = [260; 260; 0; 0];	# hue
	l = [0.5; vertex; vertex; 0.5];	# lightness
	s = [1; 1; 1; 1];		# saturation
	plscmap1l(0,i,h,l,s,zeros(4,1))
	plflush;#pleop;
      endif		

      ## Ok, it works, but I want the rgb map array to be stored in __pl.colormap. how?
      ## see plcmap1_calc() in plctrl.c. Meanwhile, hardcode it.

      n = 64;
      r = [zeros(n/2,1); linspace(0,1,n/2)'];
      g = zeros(n,1);
      b = [linspace(1,0,n/2)'; zeros(n/2,1)];
      cmap = __pl.colormap = [r, g, b];
    endif

    if (nargout)
      ccmap = cmap;
    endif
    return
  endif

  if (nargin == 1 && isstr(map) && strcmp(map, 'default'))
    plscmap1n(0);
    cmap = plcolormap;
    plflush;#pleop;
    if (nargout)
      ccmap = cmap;
    endif
    return
  endif

  [r, c] = size(map);
  if( c != 3)
    help plcolormap
    return
  endif

  __pl.colormap = map;
  cmap = map;
  map = map.*255;

  plscmap1n(rows(map)); # number of colors
  plscmap1(map(:,1), map(:,2), map(:,3));
  plflush;#pleop;

  if (nargout)
    ccmap = cmap;
  endif

endfunction
