## Copyright (C) 1998-2002 Joao Cardoso.
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

## yticks ([y_interval [, num_minor])
##
## set the yticks interval, and the number of minor ticks between major yticks.
## No arguments makes it automatic (default).

function yticks (int, num)

  global __pl

  __pl_strm = __pl_init;
  
  if (nargin >= 3)
    help yticks;
    return
  endif

  if (nargin == 0)
    __pl.yticks(__pl_strm,1) = 0;
    __pl.yticks(__pl_strm,2) = 0;
  endif

  if (nargin == 2)
    __pl.yticks(__pl_strm,2) = num;
  endif

  if (nargin >= 1)
    __pl.yticks(__pl_strm,1) = int;
  endif

  if (automatic_replot)
    __pl_plotit;
  endif

endfunction
