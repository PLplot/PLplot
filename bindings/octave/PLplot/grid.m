## Copyright (C) 1998-2002 Joao Cardoso.
## 
## This program is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## This file is part of plplot_octave.

## usage: grid ("on" | "off" | "minor")
##
## Turn grid lines at major ticks "on" or "off" for plotting.
## "minor" means to draw the grid at minor tiks.
## To setup the grid at your wish, use `x(y)ticks'
## 
## If the argument is omitted, "on" is assumed.

function grid (x)

  global __pl
  __pl_strm = __pl_init;

  if (nargin == 0)
    __pl.grid(__pl_strm) = 1;
  elseif (nargin == 1)
    if (isstr (x))
      if (strcmp ("off", x))
        __pl.grid(__pl_strm) = 0;
      elseif (strcmp ("on", x))
        __pl.grid(__pl_strm) = 1;
      elseif (strcmp ("minor", x))
        __pl.grid(__pl_strm) = 2;
      else
	help grid
      endif
    else
      error ("grid: argument must be a string");
    endif
  else
    error ("usage: grid (\"on\" | \"off\")");
  endif

  if (automatic_replot)
    __pl_plotit;
  endif

endfunction
