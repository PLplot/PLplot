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

# very preliminary! assumes that img is in octave image format, where each
# matrix element is an index in the colormap. Slow.
#
# ex: [img, map] = loadimage("default.img");
# plcolormap(map);
# plimage(img);

function plimage(img, levels)

entries = create_set(img); # this must be faster. use sort()

cmap = plcolormap;
ncol = length(cmap);

n = length(entries);

[nr, nc] = size(img);

[xx, yy] = meshdom(1:nc, 1:nr);

xxx = reshape(xx, nr*nc, 1);
yyy = reshape(yy, nr*nc, 1);

__pl_plenv(0, nc, 0, nr, 1, -1);

do_fortran_indexing = 1;

for i=entries	# sloooow

	plcol1(i/ncol);
	ix = find(img == i);
	plpoin(xxx(ix), yyy(ix),1);

endfor

plflush;pleop;

endfunction
