## Copyright (C) 1998, 1999, 2000, 2001, 2002 Joao Cardoso.
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

## shade(x, y, z [, levels [, contour] )
## shade(z [, levels [, contour] )
##
## plot shade of matrix z versus vectors x,y.
## level can be a scalar speficying the number of shade levels,
## or a vector, specifying where the shade level must be
## if contour exists, each shade level will be contoured with 'countour' color

function shade(x, y, z, levels, cont )

  global __pl
  __pl_strm = __pl_init;

  old_empty_list_elements_ok = empty_list_elements_ok;
  empty_list_elements_ok = 1;

  if (nargin == 1 && is_matrix(x))
    levels = 20;
    cont = 0;
    z = x; y = 1:rows(z); x = 1:columns(z);
  elseif (nargin == 2 && is_matrix(x))
    cont = 0; levels = y;
    z = x; y = 1:rows(z); x = 1:columns(z);
  elseif (nargin == 3 && is_scalar(z))
    levels = y; cont = z;
    z = x; y = 1:rows(z); x = 1:columns(z);
  elseif (nargin == 3)
    levels = 20;
    cont = 0;
  elseif (nargin == 4)
    cont = 0;
  endif

  if (is_scalar(levels) && levels == 1)	# segmentation violation!
    levels = 2;
  endif

  ## plot color and pen width of boundary of shade region

  max_color = 0; max_width = 0;
  
  if (cont)
    cont_color = cont; cont_width = 1;
  else
    cont_color = 0; cont_width = 0;
  endif

  xlen = length (x); ylen = length (y);
  
  if (ishold == 0)
    xmm = xm = min(min(x)); xM = max(max(x));
    ymm = ym = min(min(y)); yM = max(max(y));
    zm = min(min(z)); zM = max(max(z));
    
    if (__pl.axis_st(__pl_strm))
      xm = __pl.axis(__pl_strm,1); xM = __pl.axis(__pl_strm,2);
      ix = find(x >= xm & x <= xM); 
      x=x(ix); z=z(ix,:);
      xlen = length (x);
      xmm = min(x); 

      if (length(__pl.axis(__pl_strm,:)) >= 4)	
	ym = __pl.axis(__pl_strm,3); yM = __pl.axis(__pl_strm,4);
	iy = find(y >= ym & y <= yM);
	y=y(iy); z=z(:,iy);
	ylen = length (y);
	ymm = min(y);
      else
	__pl.axis(__pl_strm,3) = ym; __pl.axis(__pl_strm,4) = yM;
      endif
      if (length(__pl.axis(__pl_strm,:)) == 6)
	zm = __pl.axis(__pl_strm,5); zM = __pl.axis(__pl_strm,6);
      else
	__pl.axis(__pl_strm,5) = zm; __pl.axis(__pl_strm,6) = zM;
      endif
    else	# make axis() return current axis
      __pl.axis(__pl_strm,1) = xm; __pl.axis(__pl_strm,2) = xM;
      __pl.axis(__pl_strm,3) = ym; __pl.axis(__pl_strm,4) = yM;
      __pl.axis(__pl_strm,5) = zm; __pl.axis(__pl_strm,6) = zM;		
    endif
    
    __pl.plcol(__pl_strm) = 1;
    plcol(15);pllsty(1);
    __pl_plenv(xm, xM, ym, yM, 0, -2);
  else
    if (columns(__pl.axis(__pl_strm,:)) != 6)
      empty_list_elements_ok = old_empty_list_elements_ok;
      error("You must contour/shade plot something before entering hold mode");
    endif
    xmm = xm = __pl.axis(__pl_strm,1); xM = __pl.axis(__pl_strm,2);
    ymm = ym = __pl.axis(__pl_strm,3); yM = __pl.axis(__pl_strm,4);
    zm = __pl.axis(__pl_strm,5); zM = __pl.axis(__pl_strm,6);
    z = z(find(x >= xm & x <= xM), find(y >= ym & y <= yM));	
  endif

  maxx = max(x); maxy = max(y); minx = min(x); miny = min(y);
  
  
  if (!is_scalar(levels))
    n = length(levels)-1;
    clevel = levels;
  else
    n = levels;
    clevel = linspace(zm, zM, levels+1);
    cclevel = linspace(zm, zM, levels);
  endif

  __pl.type = -2;
  __pl.lab_str = "";
  __pl.plcol(__pl_strm) = 1;
  __pl.pllsty(__pl_strm) = 1;	
  __pl.lab_pos(__pl_strm) = 1;

  tr = [(maxx-minx)/(xlen-1); 0; xmm; 0; (maxy-miny)/(ylen-1); ymm];
  plpsty(0);
  if (1) ## it turns out that plshades() is slower than several calls to plshade() !? and plshades() sometimes fails ?!

    for i = 1:n
      plshade(z', '0', minx, maxx, miny, maxy, 
	      clevel(i), clevel(i+1),
	      1, (i-1) / (n-1), 1,
	      cont_color, cont_width, max_color, max_width, 1, tr);
    endfor
  else

    plshadesx(z, minx, maxx, miny, maxy, 
	     clevel', 1, cont_color, cont_width, 1, tr);
  endif

  for i = 1:n
    __pl.lab_str = [__pl.lab_str; sprintf("%#+.2G", cclevel(i))];
    __pl.lab_col(__pl_strm,__pl.lab_pos(__pl_strm)) = __pl.plcol(__pl_strm);
    __pl.lab_lsty(__pl_strm,__pl.lab_pos(__pl_strm)) = __pl.pllsty(__pl_strm);
    __pl.lab_pos(__pl_strm) = __pl.lab_pos(__pl_strm) + 1;				
    __pl.plcol(__pl_strm) = rem(__pl.plcol(__pl_strm), 15)+1;
    if  (__pl.lstlyle(__pl_strm))
      __pl.pllsty(__pl_strm) = rem(__pl.pllsty(__pl_strm), 8)+1;
    endif
  endfor

  if (__pl.grid(__pl_strm))		# this has to be done after shading
    plcol(15);
    plbox("bcnsgt",0,0,"bcnsgtv",0,0)
  else
    plcol(15);
    plbox("bcnst",0,0,"bcnstv",0,0)
  endif

  if (__pl.legend(__pl_strm))
    __pl_draw_legend
  endif

  plcol(15);
  pllab(tdeblank(__pl.xlabel(__pl_strm,:)), tdeblank(__pl.ylabel(__pl_strm,:)), tdeblank(__pl.tlabel(__pl_strm,:)));
  plflush;
  empty_list_elements_ok = old_empty_list_elements_ok;
  
endfunction
