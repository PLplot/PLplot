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

## usage: subwindow ([xn, yn])
##
## Sets subwindow position in multiplot mode for next plot. The
## multiplot mode has to be previously initialized using multiplot()
## command, else this command just becomes an aliad to multiplot()
##
## With no argument, reports current subwindow

## Author: Vinayak Dutt <Dutt.Vinayak@mayo.EDU>
## Created: 3 July 95
## Adapted-By: jwe
## Modified: jc

function subwindow (xn, yn)

  global __pl __pl_inited

  if (!exist("__pl_inited") || plglevel == 0)
    figure(0)
  endif

  __pl_strm = plgstrm + 1;

  if (nargin != 2 && nargin != 0)
    usage ("subwindow (xn, yn)");
  endif

  if (nargin == 0)
    __pl.multi_row(__pl_strm)
    __pl.multi_col(__pl_strm)
    return
  endif

  ## check for scalar inputs

  if (! (is_scalar (xn) && is_scalar (yn)))
    error ("subwindow: xn and yn have to be scalars");
  endif
  
  xn = round (xn);
  yn = round (yn);

  if (! __pl.multi(__pl_strm))
    multiplot (xn, yn);
    return;
  endif
  
  if (xn < 1 || xn > __pl.multi_row(__pl_strm) ||
      yn < 1 || yn > __pl.multi_col(__pl_strm))
    error ("subwindow: incorrect xn and yn");
  endif
  
  __pl.multi_cur(__pl_strm) = (yn-1)*__pl.multi_row(__pl_strm) + xn;
  pladv(__pl.multi_cur(__pl_strm));
  
endfunction
