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

function __pl_mesh(caller, ...)

  global __pl
  __pl_strm = __pl_init;
  
  caller(length(caller)+1:5)=" "; ## compatibility with 2.0.16. Notice
                                  ## the blank spaces at the "case"s bellow.

  switch (caller)
    case ("mesh ")
      type = 0;
    case ("meshc")
      type = 1;
    case ("meshz")
      type = 2;
    case ("surf ")
      type = 3;
    case ("surfc")
      type = 4;
    case ("surfl")
      type = 5;     
    otherwise
      error("__pl_mesh: FIXME")
  endswitch
  
  if (nargin == 2)
    z = va_arg ();
    if (is_matrix (z))
      [rz, cz] = size(z);
      x = (1:cz)'; 
      y = (1:rz)'; 
    else
      error ("mesh: argument must be a matrix.\n");
    endif
  elseif (nargin == 4 )
    x = va_arg ();
    y = va_arg ();
    z = va_arg ();
    if (is_vector (x) && is_vector (y) && is_matrix (z))
      xlen = length (x);
      ylen = length (y);
      if (xlen == columns (z) && ylen == rows (z))
        if (rows (y) == 1)
          y = y';
        endif
        if (rows (x) == 1)
          x = x';
        endif
      else
        msg = "mesh: rows (z) must be the same as length (x) and";
        msg = sprintf ("%s\ncolumns (z) must be the same as length (y).\n", msg);
        error (msg);
      endif
    elseif (is_matrix (x) && is_matrix (y) && is_matrix (z))			
      error("x,y and z all matrices not yet implemented.\n")
    else
      error ("mesh: x and y must be vectors and z must be a matrix.\n");
    endif
  else
    help (caller);
    return
  endif

  xm = min(min(x)); xM = max(max(x));
  ym = min(min(y)); yM = max(max(y));
  zm = min(min(z)); zM = max(max(z));

  if (xm == xM)
    xM = xm +10*eps;
  endif
  if (ym == yM)
    yM = ym +10*eps;
  endif
  if (zm == zM)
    zM = zm +10*eps;
  endif

  if (__pl.axis_st(__pl_strm))
    xm = __pl.axis(__pl_strm,1); xM = __pl.axis(__pl_strm,2);	# at least x always exist

    if (length(__pl.axis) >= 4)	
      ym = __pl.axis(__pl_strm,3); yM = __pl.axis(__pl_strm,4);
    else
      __pl.axis(__pl_strm,3) = ym; __pl.axis(__pl_strm,4) = yM;
    endif
    if (length(__pl.axis) == 6)
      zm = __pl.axis(__pl_strm,5); zM = __pl.axis(__pl_strm,6);
    else
      __pl.axis(__pl_strm,5) = zm; __pl.axis(__pl_strm,6) = zM;		
    endif
  else	# make axis() return current axis
    __pl.axis(__pl_strm,1) = xm; __pl.axis(__pl_strm,2) = xM;
    __pl.axis(__pl_strm,3) = ym; __pl.axis(__pl_strm,4) = yM;
    __pl.axis(__pl_strm,5) = zm; __pl.axis(__pl_strm,6) = zM;		
  endif
  
  plcol(15);pllsty(1);
  ##__pl_plenv(-1.6, 1.6, -1.6, 2.6, 0, -2);
  if (__pl.multi(__pl_strm) == 1)	# multiplot, erase current subwindow
    plclear;
  else
    pladv(0);
  endif

  plvpor(0, 1, 0, 0.95);
  plwind(-1.6, 1.6, -1.3, 2.2);
  plw3d(2, 2, 2, xm, xM, ym, yM, zm, zM, __pl.alt(__pl_strm), __pl.az(__pl_strm))
  plbox3("bnstu", tdeblank(__pl.xlabel(__pl_strm,:)), 0.0, 0,
	 "bnstu", tdeblank(__pl.ylabel(__pl_strm,:)), 0.0, 0,
	 "bcmnstuv", tdeblank(__pl.zlabel(__pl_strm,:)), 0.0, 0);
  plcol(1)

  LINE_XY = 3;
  MAG_COLOR = 4;
  SURF_CONT = hex2dec("10");
  BASE_CONT = hex2dec("20");
  DRAW_SIDES = hex2dec("40");
  FACETED = hex2dec("80");
    
  switch (type)
    case  0
      plmesh(x, y, z', LINE_XY + MAG_COLOR);
    case 1
      plmesh(x, y, z', LINE_XY + MAG_COLOR + BASE_CONT);
    case 2
      plot3d(x, y, z', LINE_XY + MAG_COLOR, 1); 
    case 3
      plsurf3d(x, y, z', MAG_COLOR, 0);
    case 4
      plsurf3d(x, y, z', MAG_COLOR + BASE_CONT, linspace(min(min(z)), max(max(z)), 10)') 
   case 5
     pllightsource(__pl.light(1), __pl.light(2), __pl.light(3));
     plsurf3d(x, y, z', 0, 0);
 endswitch	
  
  plcol(15);
  ##pllab(" ", " ", tdeblank(__pl.tlabel(__pl_strm,:)));
  plmtex("t", 1, 0.5,0.5, tdeblank(__pl.tlabel(__pl_strm,:)));
  plflush;

endfunction
