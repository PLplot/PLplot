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

## usage: axis ()
##        axis ([xmin, xmax])
##        axis ([xmin, xmax, ymin, ymax])
##        axis ([xmin, xmax, ymin, ymax, zmin, zmax])
##
## Sets the axis limits.
##
## With no arguments, turns "autoscaling" on, and returns the current axis
##
## The argument call should be fixed to allow independent axis scaling

## Author: jwe
## Modified: jc

function curr_axis = axis (ax)

  ## the idea is that _pl_axis always has the current max/min axis, either
  ## as setup by the user, or automaticaly. It interacts with hold mode.

  global __pl
  __pl_strm = __pl_init;

  if (nargin > 1)
    usage ("axis ([xmin, xmax, ymin, ymax, zmin, zmax])");
  endif

  if (nargin == 0)
    __pl.axis_st(__pl_strm) = 0;
    curr_axis = __pl.axis(__pl_strm,:);
  elseif (is_vector (ax))
    len = length (ax);

    if (len != 2 && len != 4 && len != 6)
      error ("axis: expecting vector with 2, 4, or 6 elements");
    endif
    
    __pl.axis_st(__pl_strm) = 1;

    if (len > 1)
      __pl.axis(__pl_strm, [1, 2]) = [ax(1), ax(2)];
    endif

    if (len > 3)
      __pl.axis(__pl_strm, [3, 4]) = [ax(3), ax(4)];
    endif

    if (len > 5)
      __pl.axis(__pl_strm, [5, 6]) = [ax(5), ax(6)];
    endif

  else
    error ("axis: expecting no args, or a vector with 2, 4, or 6 elements");
  endif

  if (automatic_replot)
    __pl_plotit;
  endif

endfunction
