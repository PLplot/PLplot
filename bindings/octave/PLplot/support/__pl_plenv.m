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

function __pl_plenv(xm, xM, ym, yM, scale, axis)

	## this is equivalent to plenv, except that no padv(0) is performed.
	## this is important for multiplot, as pladv() advances the subwindow,
	## and pladv(0) advances the page, clearing all plot. However, not
	## calling pladv(0), keeps all plots in the plot buffer,
	## so "plreplot" replots all plots since the last call to
	## pladv(0)/plbop
	## (nice way of doing "movie")
	##
	## instead, the (sub)window is cleared, and the subplot position
	## remains
	## the same. Of course, it is not fast. Changed: The above described
	## behaviour is only valid in multiplot mode,
	## else, pladv(0) is issued.

	global __pl
	__pl_strm = plgstrm + 1;

	if (__pl.multi(__pl_strm) == 1)	# multiplot, erase current subwindow
		plvpor(0,1,0,1)
		plwind(0,1,0,1)
		plcol(0);plpsty(0)
		plfill([0;1;1;0],[0;0;1;1]);
	else
		pladv(0)
	endif
		
  if (scale)
  	plvasp(1)
  else
  	plvsta
  endif

	xrg = yrg = 0;
	if (__pl.margin(__pl_strm))
		xrg = (xM-xm)/50; yrg = (yM-ym)/50;
	endif
		
  plwind(xm-xrg, xM+xrg, ym-yrg, yM+yrg);
  
	## axis=-2 : draw no box, axis or labels
	## axis=-1 : draw box only
	## axis= 0 : Draw box and label with coordinates
	## axis= 1 : Also draw the coordinate axes
	## axis= 2 : Draw a grid at major tick positions
	## axis=10 : Logarithmic X axis, Linear Y axis, No X=0 axis
	## axis=11 : + coordinate axis
	## axis=12 : + grid
	## axis=20 : Linear X axis, Logarithmic Y axis, No Y=0 axis
	## axis=21 : + coordinate axis
	## axis=22 : + grid
	## axis=30 : Logarithmic X and Y axes
	## axis=31 : + coordinate axes
	## axis=32 : + grid

	switch(axis)
    case -2
    	xopt=""; yopt="";
    case -1
			xopt="bc"; yopt="bc";
    case 0
			xopt="bcnst"; yopt="bcnstv";
    case 1
			xopt="bcnsta"; yopt="bcnstva";
    case 2
			xopt="bcnstag"; yopt="bcnstvag";
    case 3
			xopt="bcnstagh"; yopt="bcnstvagh";		
    case 10
			xopt="bcnstl"; yopt="bcnstv";
    case 11
			xopt="bcnstl"; yopt="bcnstva";
    case 12
			xopt="bcgnstl"; yopt="bcnstvag";
    case 13
			xopt="bcgnstlh"; yopt="bcnstvagh";		
    case 20
			xopt="bcnst"; yopt="bcnstvl";
    case 21
			xopt="bcnst"; yopt="bcnstval";
    case 22
			xopt="bcnstg"; yopt="bcnstvagl";
    case 23
			xopt="bcnstgh"; yopt="bcnstvaglh";		
    case 30
			xopt="bcnstl"; yopt="bcnstvl";
    case 31
			xopt="bcnstl"; yopt="bcnstval";
    case 32
			xopt="bcngstl"; yopt="abcnstvgl";
    case 33
			xopt="bcngstlh"; yopt="abcnstvglh";
	endswitch

	plcol(15); pllsty(1);
	plbox(xopt, __pl.xticks(__pl_strm,1), __pl.xticks(__pl_strm,2),
				yopt, __pl.yticks(__pl_strm,1), __pl.yticks(__pl_strm,2));            

endfunction
