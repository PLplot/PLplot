## Copyright (C) 2002 Joao Cardoso.
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

function __pl_logplotit

  global __pl
  strm = __pl_init;

  ## plplot has no notion of log plots. What to plot for values <= 0?
  ## this is a kludge,  FIXME no matrix allowed!
  for i=0:__pl.items(strm)-1
    
    x = eval(sprintf("__pl.x%d_%d;", i, strm));
    y = eval(sprintf("__pl.y%d_%d;", i, strm));
    
    if (__pl.type(strm) == 10)
      if (any(x <= 0))
	expr = "ix = find(x > 0);";
      else
	break;
      endif
      
    elseif (__pl.type(strm) == 20)
      if (any(y <= 0))
	expr = "ix = find(y > 0);";
      else
	break;
      endif
    elseif (__pl.type(strm) == 30)
      if (any(y <= 0) || any(x <= 0))
	expr = "ix = find(y > 0 & x > 0);";
      else
	break;
      endif
    endif
    try 
      k = __pl.items(strm);
      while(!isempty(eval(expr)))
	ixx = ix(find(ix == (min(ix):rows(ix)+min(ix)-1)'));
	eval(sprintf("__pl.y%d_%d = y(ixx,:);", k, strm));
	eval(sprintf("__pl.x%d_%d = x(ixx,:);", k, strm));
	eval(sprintf("__pl.fmt%d_%d = __pl.fmt%d_%d;", k, strm, i, strm));
	## replace label string by special mark: check first that it is not empty!!
	## this is an eval mess!
	eval(sprintf("nofmt = isempty(__pl.fmt%d_%d);", k, strm));
	if (nofmt == 1) ## no format string, supply an empty one
	  eval(sprintf("__pl.fmt%d_%d = ';;';",k, strm));
	else
	  eval(sprintf("haslab = index(__pl.fmt%d_%d,';');", k, strm));
	  if (haslab == 0) ## format string has no label string, use original format plus empty label string
	    eval(sprintf("__pl.fmt%d_%d = __pl.fmt%d_%d;", i, strm, k, strm));
	    eval(sprintf("__pl.fmt%d_%d = [__pl.fmt%d_%d ';;'];", k, strm, k, strm));
	  else ## format string has label, replace label string with an empty one
	    eval(sprintf("__pl.fmt%d_%d(index(__pl.fmt%d_%d,';')+1:rindex(__pl.fmt%d_%d,';')-1) = '';", k, strm, k, strm, k, strm));
	    eval(sprintf("__pl.fmt%d_%d(index(__pl.fmt%d_d,';')+2:rindex(__pl.fmt%d_%d,';')-1) = '';",  k, strm, k, strm, k, strm));
	  endif
	endif
	x(ixx,:) = y(ixx,:) = []; k++;
      endwhile
      k--;
      eval(sprintf("__pl.y%d_%d = __pl.y%d_%d;", i, strm, k, strm));
      eval(sprintf("__pl.x%d_%d = __pl.x%d_%d;", i, strm, k, strm));
      __pl.items(strm) = k;
    catch
      empty_list_elements_ok = old_empty_list_elements_ok;
      error("__pl_plotit: FIXME. Logplot of matrices with negative elements is in some cases not allowed.\n");
    end
    
  endfor

endfunction