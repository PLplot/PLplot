## Copyright (C) 2000-2002 Joao Cardoso.
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

## Assumes that img is in octave image format, where each
## matrix element is an index in the colormap. Slow.
##
## ex:
##  [img, map] = loadimage("default.img"); # "lena.img" can also be used
##   colormap(map);
##   plimage(img);
##
## 


function plimage(img)

  global __pl
  __pl_strm = __pl_init;
  
  plenv(0, 1, 0, 1, 1, -2);
  pplimage (fliplr(img'), 0, 1, 0, 1,
	    min(min(img)), max(max(img)), 0, 1, 0, 1);
  pllab(tdeblank(__pl.xlabel(__pl_strm,:)),
	tdeblank(__pl.ylabel(__pl_strm,:)),
	tdeblank(__pl.tlabel(__pl_strm,:)));
  plflush;pleop;

endfunction
