##  $Id:$
##
##  Drawing "spirograph" curves - epitrochoids, cycolids, roulettes
##
##  Copyright (C) 2007 Arjen Markus
##  Copyright (C) 2008 Andrew Ross
##
##  This file is part of PLplot.
##   
##  PLplot is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Library Public License as published
##  by the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
##  
##  PLplot is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU Library General Public License for more details.
##   
##  You should have received a copy of the GNU Library General Public License
##  along with PLplot; if not, write to the Free Software
##  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
##



1;

function ix27c

##--------------------------------------------------------------------------*\
## Generates two kinds of plots:
##   - construction of a cycloid (animated)
##   - series of epitrochoids and hypotrochoids
##--------------------------------------------------------------------------*/

  ## R, r, p, N
  params = [
	    21.0,  7.0,  7.0,  3.0;  ## Deltoid
	    21.0,  7.0, 10.0,  3.0;
	    21.0, -7.0, 10.0,  3.0;
	    20.0,  3.0,  7.0, 20.0;
	    20.0,  3.0, 10.0, 20.0;
	    20.0, -3.0, 10.0, 20.0;
	    20.0, 13.0,  7.0, 20.0;
	    20.0, 13.0, 20.0, 20.0;
	    20.0,-13.0, 20.0, 20.0];
  
  if (!exist("plinit"))
    plplot_stub
  endif

  if(isempty(sprintf("%s",plgdev')))
    plsdev("xwin");
  endif

  ## Parse and process command line arguments

  ## (void) plparseopts(&argc, argv, PL_PARSE_FULL);

  ## Initialize plplot
  plinit();

  ## Illustrate the construction of a cycloid

  cycloid();

  ## Loop over the various curves
  ## First an overview, then all curves one by one

  plssub(3, 3); ## Three by three window

  for i = 1:9
    pladv(0);
    plvpor( 0.0, 1.0, 0.0, 1.0 );
    spiro( params(i,:) );
  endfor

  pladv(0);
  plssub(1, 1); ## One window per curve
  
  for i=1:9
    pladv(0);
    plvpor( 0.0, 1.0, 0.0, 1.0 );
    spiro( params(i,:) );
  endfor

  ## Don't forget to call plend() to finish off!

  plend1();
end

## ===============================================================

function cycloid()
    ## TODO
endfunction

## ===============================================================

function spiro(params)
  
  NPNT=20000;

  ## Fill the coordinates

  windings = floor(params(4));
  steps    = floor(NPNT/windings);
  dphi     = 8.0*acos(-1.0)/steps;

  xmin = 0.0; ## This initialisation is safe!
  xmax = 0.0;
  ymin = 0.0;
  ymax = 0.0;

  i = (0:windings*steps)';
  phi = i*dphi;
  phiw = (params(1)-params(2))/params(2)*phi;
  xcoord = (params(1)-params(2))*cos(phi) + params(3)*cos(phiw);
  ycoord = (params(1)-params(2))*sin(phi) - params(3)*sin(phiw);

  xmin = min(xcoord);
  xmax = max(xcoord);
  ymin = min(ycoord);
  ymax = max(ycoord);

  scale = max(xmax-xmin,ymax-ymin);
  xmin = - 0.65 * scale;
  xmax =   0.65 * scale;
  ymin = - 0.65 * scale;
  ymax =   0.65 * scale;
  
  plwind( xmin, xmax, ymin, ymax );
  
  plcol0(1);
  plline( xcoord, ycoord );

endfunction
## Log-linear plot.

ix27c
