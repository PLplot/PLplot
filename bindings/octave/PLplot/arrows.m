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

# arrows( X, Y, dx, dy)
# plot an array of arrows (partly emulates matlab quiver)
#
# NOTE: preliminary

function arrows( X, Y, dx, dy)

	of = do_fortran_indexing;do_fortran_indexing=1;
	for i=1:rows(X)*columns(X)
		xx = [X(i); X(i) + dx(i)];
		yy = [Y(i); Y(i) + dy(i)];
		plline(xx,yy);plpoin(X(i) + dx(i),  Y(i) + dy(i), 20);
	endfor
	plflush;pleop;
	do_fortran_indexing = of;
	
endfunction
