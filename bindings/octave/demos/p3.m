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


function p3

    xscale = 6.;
    yscale = 1.;
    xoff = 0.;
    yoff = 0.;

    m=60;
    
    for i=1:m
        x(i) = xoff + xscale * (i + 1) / m;
        y(i) = yoff + yscale * (x(i).^2.);
    endfor

    n=5;
    
    for i=1:n
        xs(i) = x(i * 10 + 3); xe(i) = rand;
        ys(i) = y(i * 10 + 3); yel(i) = ys(i) - 4*rand; yer(i) = ys(i) + 4*rand;
    endfor

# Plot the data points */
    title("#frPLplot Example 3 - y=x#u2 #d(with error bars)");
    
    plot(x, y);
    hold
    plot(xs,ys,'ob');
    plot([xs xe], [ys yel yer], '~g');
    hold

endfunction
