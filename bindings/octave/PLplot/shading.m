## Copyright (C) 2002 Joao Cardoso.
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

## usage: shading "flat" || "faceted"
##
## sets the type of shading to use by surf()

function type = shading(type)

  global __pl
  __pl_strm = __pl_init;

  if (nargin == 0)
    type = __pl.shading(__pl_strm,:);
  elseif (nargin == 1)
    if (strcmp(type, "flat") || strcmp(type,"faceted"))
      __pl.shading = __pl_matstr(__pl.shading, type, __pl_strm);
    else
      usage("shading");
      return;
    endif

  else
    usage("shading");
    return;
  endif

endfunction
