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

## usage: xlabel (text)
##
## Defines a label for the x-axis of the next plots
##
## See also: title, pllab, bottom_title, xlabel, ylabel, zlabel

## Author: jwe
## Modified: jc

function text = xlabel (text)

  global __pl

  __pl_strm = __pl_init;

  if (nargin > 1)
    usage ("xlable (text)");
  endif
  
  if (nargin == 1 && isempty(text))
    text = " ";
  endif
  
  if (nargin == 0)
    text = __pl.xlabel(__pl_strm,:);
  else
    __pl.xlabel = __pl_matstr(__pl.xlabel, text, __pl_strm);
  endif

endfunction
