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

## legend ([status [, xposition [, yposition]])
##
## turns the plot legend status = "off" || "on" == "transparent" || "opaque",
## left top corner legend at position (left)(botton) 0 <= position <= 1 (right)(top)

function legend (x, xpos, ypos)

  global __pl __pl_inited

  if (!exist("__pl_inited") || plglevel == 0)
    figure(0)
  endif

  __pl_strm = plgstrm+1;

  if (nargin == 0)
    __pl.legend(__pl_strm) = 1;
    __pl.legend_xpos(__pl_strm) = __pl.legend_ypos(__pl_strm) = 1;
  endif
  
  if (nargin >= 1)
    if (isstr (x))
      if (strcmp ("off", x))
        __pl.legend(__pl_strm) = 0;
      elseif (strcmp ("on", x) || strcmp ("transparent", x))
        __pl.legend(__pl_strm) = 1;
      elseif (strcmp ("opaque", x))
        __pl.legend(__pl_strm) = 2;
      endif
    else
      error ("legend: argument must be a string");
    endif
  endif
  
  if (nargin == 3)
    __pl.legend_xpos(__pl_strm) = xpos;
    __pl.legend_ypos(__pl_strm) = ypos;
  endif

endfunction
