## Copyright (C) 1998-2003 Joao Cardoso.
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
  strm = __pl_init;

  if (nargin == 0)
    __pl.grid(strm) = 1;
  elseif (nargin == 1)
    if (isstr (x))
      x(length(x)+1:5) = " "; ## compatibility with 2.0.16.
      switch (x)
	case "off  "
          __pl.grid(strm) = 0;
	case "on   "   
          __pl.grid(strm) = 1;
	case "minor"
          __pl.grid(strm) = 2;
	otherwise
	  help grid
      endswitch
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
