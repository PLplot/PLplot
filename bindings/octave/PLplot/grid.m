## Copyright (C) 1996 John W. Eaton
##
## This file is part of Octave.
##
## Octave is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

## usage: grid ("on" | "off" | "minor")
##
## Turn grid lines at major ticks "on" or "off" for plotting.
## "minor" means to draw the grid at minor tiks.
## To setup the grid at your wish, use `x(y)ticks'
## 
##
## If the argument is omitted, "on" is assumed.
##
## See also: plot, semilogx, semilogy, loglog, polar, mesh, contour,
##           bar, stairs, gplot, gsplot, replot, xlabel, ylabel, title

## Author: jwe
## Modified: jc

function grid (x)

  global __pl

  if (!struct_contains (__pl,"inited") || plglevel == 0)
    figure(0);
  endif

  __pl_strm = plgstrm+1;

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

endfunction
