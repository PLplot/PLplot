## Copyright (C) 1996 John W. Eaton
##
## This file is part of Octave.
##
## Octave is free software; you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## Octave is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Octave; see the file COPYING.  If not, write to the Free
## Software Foundation, 59 Temple Place - Suite 330, Boston, MA
## 02111-1307, USA.

## usage: mplot (x, y)
##        mplot (x1, y1, x2, y2, ...)
##        mplot (x, y, fmt)
##
## This is a modified version of plot() command to work with
## multiplot to plot multiple plots per page.
## This plot version automatically updates the plot position to
## next plot position after making the plot in the given subplot
## position.
##
## See command plot() for the various options to this command
## as this is just mulitplot version of the same command.
##
##	plplot does this by itself.

## Author: Vinayak Dutt <Dutt.Vinayak@mayo.EDU>
## Adapted-By: jwe
## Modified: jc

function mplot (...)

  global __pl
  __pl_strm = __pl_init;

  __pl.multi_cur(__pl_strm) = rem(__pl.multi_cur(__pl_strm)+1,
				  __pl.multi_row(__pl_strm) * 
				  __pl.multi_col(__pl_strm)+1);
  if (__pl.multi_cur(__pl_strm) == 0 )
    __pl.multi_cur(__pl_strm) = 1;
  endif
  
  pladv(__pl.multi_cur(__pl_strm))
  __plt__ ("plot", all_va_args);

endfunction
