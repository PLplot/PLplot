## Copyright (C) 1996 Kurt Hornik
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

## usage:  tdeblank (s)
##
## Remove trailing amd leading blanks from the string s.

## Author: Kurt Hornik <Kurt.Hornik@ci.tuwien.ac.at>
## Adapted-By: jwe
## Modified: jc

function t = tdeblank (s)

  if (nargin != 1)
    usage ("tdeblank (s)");
  endif

  if (isstr (s))

    [nr, nc] = size (s);
    len = nr * nc;

    if (len == 0)
      t = s;
    else
      s = reshape (s, 1, len);
      k = max (find (s != " "));
      l = min (find (s != " "));
      if (isempty(k) || isempty(l))
	  	t = "";
	  else
		t = s (l:k);
	  endif
    endif

  else
    error ("tdeblank: expecting string argument");
  endif

endfunction
