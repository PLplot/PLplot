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


function p14

  ## And now, in the Theatre near you, "Octave, the movie"
  ##
  ## For a smoother effect the "double buffer" option in 'pldef'
  ##  must be set

  title("Octave, the movie");
  ylabel "Use double buffer"
  xlabel "in pldef.m"
  
  l = 2;
  x = y = linspace(-l, l, 30);

  [xx,yy] = meshgrid(x,y);
  r = yy .* exp(-(xx.^2 .+ yy.^2));
  
  axis([-l l -l l -0.5 0.5]);
  
  for k = linspace(0, 4*pi, 60)
    mesh(x, y, sin(k).*r);
  endfor

	axis;

endfunction


