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

# st = autostyle "on" | "off"
#
# if set increases line style after each plot.
# return current status;
# Usefull for B&W plots

function st = autostyle(x)

global __pl __pl_inited

if (!exist("__pl_inited") || plglevel == 0)
	figure(0)
endif

__pl_strm = plgstrm+1;

  if (nargout == 1)
  	st = __pl.lstlyle(__pl_strm);
  endif
  
  if (nargin == 1)
    if (isstr (x))
      if (strcmp ("off", x))
        __pl.lstlyle(__pl_strm) = 0;
      elseif (strcmp ("on", x))
        __pl.lstlyle(__pl_strm) = 1;
      else
    help autostyle
      endif
    else
      error ("autostyle: argument must be a string");
    endif
  endif

endfunction

