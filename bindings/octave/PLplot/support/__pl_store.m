## Copyright (C) 1998-2003 Joao Cardoso.
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

function __pl_store(x, y, fmt)

  global __pl
  strm = __pl_init;

  if (!struct_contains(__pl, "items"))
    __pl.items(strm) = 0;
  endif

  items = __pl.items(strm);

  ## find the max/min x/y values. Currently reset at __plt__
  __pl.lxm(strm) = min([__pl.lxm(strm), min(min(x))]);
  __pl.lxM(strm) = max([__pl.lxM(strm), max(max(x))]);

  __pl.lym(strm) = min([__pl.lym(strm), min(min(y))]);
  __pl.lyM(strm) = max([__pl.lyM(strm), max(max(y))]);

  eval(sprintf("__pl.x%d_%d=x; __pl.y%d_%d=y; __pl.fmt%d_%d=fmt;",\
	       items, strm, items, strm, items, strm));
  
  __pl.items(strm) = __pl.items(strm) + 1;

endfunction
