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

## st = axis_set
##
## return current axis status
## st = 0 => autoscale
## st = 1 => axis are set

function st = axis_set

  global __pl

  if (!struct_contains (__pl,"inited") || plglevel == 0)
    figure(0);
  endif

  st = __pl.axis_st(plgstrm + 1);

endfunction
