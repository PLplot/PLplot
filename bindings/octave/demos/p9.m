
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

function p9

  legend "off"
  x = -2:.2:2;
  y = -2:.2:2;
  [xx,yy] = meshgrid(x,y);
  z = yy .* exp(-xx.^2 - yy.^2);
  [gy, gx] = gradn(z,.2,.2);
  title("Vector field example");
  contour(x,y,z)
  hold on, arrows(yy,xx,gx./2,gy./2), hold off

endfunction
