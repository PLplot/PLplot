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

function p5

  x=-pi:0.01:pi;
  y=sin(x).+cos(3*x);

  t = automatic_replot;
  automatic_replot = 0;

  grid "on"
  title("Polar example");
  polar(x,y)
  grid "off"

  automatic_replot = t;

endfunction
