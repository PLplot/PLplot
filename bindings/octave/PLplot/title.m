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

## usage: title (text)
##
## Defines a title for the next plots
##   The special character `#' followed by another special character may be
##   used for special text manipulation, eg ';x#u2#d;' prints `x^2' in math,
##	 and `title("#gS#u5#b#d#dn=1#u") prints \sum_{n=1}^{5}.
##
##		#u superscript, end with #d
##		#d subscript, end with #u
##		#b backspace
##		## # itself
##		#+ toogle overline mode
##		#- toogle underline mode
##		#gx greek letter x
##		#fn switch to normal font
##		#fr switch to roman font
##		#fi switch to italic font
##		#fs switch to script font
##		#(nnn) hershey character nnn
##
##		For special fonts, the extended charset must be loaded first,
##	 	use `plfontld(1)' to set, and `plfontld(0)' to return to normal.
##
## See also: pllab, bottom_title, xlabel, ylabel, zlabel

## Author: jwe
## Adapted-By: jwe
## Modified: jc

function text = title (text)

  global __pl

  __pl_strm = __pl_init;

  if (nargin > 1)
    usage ("title (text)");
  endif
  
  if (nargin == 1 && isempty(text))
    text = " ";
  endif
  
  if (nargin == 0)
    text = __pl.tlabel(__pl_strm,:);
  else
    __pl.tlabel = __pl_matstr(__pl.tlabel, text, __pl_strm);
  endif

  if (automatic_replot)
    __pl_plotit;
  endif

endfunction

