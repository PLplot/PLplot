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

function __pl_contour(x, y, z, n)

  global __pl

  __pl_strm = plgstrm+1;
  old_empty_list_elements_ok = empty_list_elements_ok;
  empty_list_elements_ok = 1;

  grid = 0;
  if (__pl.grid(__pl_strm))
    grid = 2;
  endif

  if (ishold == 0)
    xm = min(min(x)); xM = max(max(x));
    ym = min(min(y)); yM = max(max(y));
    zm = min(min(z)); zM = max(max(z));

    if (__pl.axis_st(__pl_strm))
      ## at least x always exist
      xm = __pl.axis(__pl_strm,1); xM = __pl.axis(__pl_strm,2);	
      z = z(find(x >= xm & x <= xM), :);

      if (length(__pl.axis) >= 4)	
	ym = __pl.axis(__pl_strm,3); yM = __pl.axis(__pl_strm,4);
	z = z(:, find(y >= ym & y <= yM));
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
    
    __pl.type = -3;
    __pl.plcol(__pl_strm) = 1;
    __pl.pllsty(__pl_strm) = 1;	
    __pl.lab_pos(__pl_strm) = 1;
    __pl.lab_str = "";
    plcol(15); pllsty(1);
    __pl_plenv(xm, xM, ym, yM, 0, grid);
  else
    if (columns(__pl.axis(__pl_strm,:)) != 6)
      empty_list_elements_ok = old_empty_list_elements_ok;
      error("You must contour/shade plot something before entering hold mode");
    endif
    xm = __pl.axis(__pl_strm,1); xM = __pl.axis(__pl_strm,2);
    ym = __pl.axis(__pl_strm,3); yM = __pl.axis(__pl_strm,4);
    zm = __pl.axis(__pl_strm,5); zM = __pl.axis(__pl_strm,6);
    z = z(find(x >= xm & x <= xM), find(y >= ym & y <= yM));
  endif

  [xlen, ylen] = size(z);
  tr = [(xM-xm)/(xlen-1); 0; xm; 0; (yM-ym)/(ylen-1); ym];

  if (!is_scalar(n))
    st = (zM-zm)/length(n)/2;
    clevel = n; n = length(n);
  else
    st = (zM-zm)/n/2;
    clevel = (linspace(zm+st, zM-st, n))';
  endif	

  for i=1:n
    plcol(__pl.plcol(__pl_strm)); pllsty(__pl.pllsty(__pl_strm));
    plcont(z, 1, xlen, 1, ylen, clevel(i), tr);
    __pl.lab_str = [__pl.lab_str; sprintf("% .2G", clevel(i))];
    __pl.lab_col(__pl_strm,__pl.lab_pos(__pl_strm)) = __pl.plcol(__pl_strm);
    __pl.lab_lsty(__pl_strm,__pl.lab_pos(__pl_strm)) = __pl.pllsty(__pl_strm);
    __pl.lab_pos(__pl_strm) = __pl.lab_pos(__pl_strm) + 1;				
    __pl.plcol(__pl_strm) = rem(__pl.plcol(__pl_strm), 15)+1;
    if  (__pl.lstlyle(__pl_strm))
      __pl.pllsty(__pl_strm) = rem(__pl.pllsty(__pl_strm), 8)+1;
    endif
  endfor

  ## to not mess up the legend, print it only after ploting all contour lines	
  if (__pl.legend(__pl_strm))
    __pl_draw_legend
  endif

  plcol(15);
  pllab(tdeblank(__pl.xlabel(__pl_strm,:)),
	tdeblank(__pl.ylabel(__pl_strm,:)),
	tdeblank(__pl.tlabel(__pl_strm,:)));
  plflush;
  empty_list_elements_ok = old_empty_list_elements_ok;

endfunction
