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

function __pl_mesh(x, y, z, s, type)

  global __pl
  __pl_strm = __pl_init;

  xm = min(min(x)); xM = max(max(x));
  ym = min(min(y)); yM = max(max(y));
  zm = min(min(z)); zM = max(max(z));

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
  __pl_plenv(-1.6, 1.6, -1.6, 2.6, 0, -2);
  plw3d(2, 2, 2, xm, xM, ym, yM, zm, zM, __pl.alt(__pl_strm), __pl.az(__pl_strm))
  plbox3("bnstu", tdeblank(__pl.xlabel(__pl_strm,:)), 0.0, 0,
	 "bnstu", tdeblank(__pl.ylabel(__pl_strm,:)), 0.0, 0,
	 "bcmnstuv", tdeblank(__pl.zlabel(__pl_strm,:)), 0.0, 0);
  plcol(1)

  if (type == 0)
    plmesh(x, y, z, 3);
  elseif (type == 1)
    pllightsource(s(1), s(2), s(3));
    plotsh3d(x, y, z, 0)
  elseif (type == 2)
    plotfc3d(x, y, z, 0)
  endif	
  
  plcol(15);
  pllab(" ", " ", tdeblank(__pl.tlabel(__pl_strm,:)));
  plflush;

endfunction
