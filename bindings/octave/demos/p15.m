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

    
function p15

	title "Lighted and shaded surface"
	xlabel "";
	ylabel "";
	[x, y, z] = rosenbrock; z = log(z);
	set_view(30, -60);
	plcolormap('default');
	surfl(x, y, z);
	pause(1);
	set_view(50, 40);
	plcolormap(gray);
	surfl(x, y, z);
		
endfunction

