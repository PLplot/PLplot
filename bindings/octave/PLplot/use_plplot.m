## usage: use_plplot() 
##        use_plplot("on")
##        use_plplot("off")
##
## Use this function to activate/deactivate the default library for the
## PLplot plotting function (plot, mesh, etc.), as both the native
## gnuplot and the new octave-plplot libraries use similar names to the
## functions. use_plplot prepends to LOADPATH the path for the PLplot
## functions when called with an argument "on", or remove it in the case
## the argument is "off" (default = "on").

## File: use_plplot.m
## Author: Rafael Laboissiere <rafael@icp.inpg.fr>
## Modified: Joao Cardoso
## Created on: Sun Oct 18 22:03:10 CEST 1998
## Last modified on: March 2001
## 
## Copyright (C) 1998 Rafael Laboissiere
## 
## This file is free software, distributable under the GPL. No
## warranties, use it at your own risk.  It  was originally written for
## inclusion in the Debian octave-plplot package.

function use_plplot(action)

  if (nargin == 0)
    action = "on";
  endif

  path = plplot_octave_path;
  ix = findstr( LOADPATH, path);
  if (!isempty(ix))
    LOADPATH(ix(1):ix(1)+length( path)-1)= "";
    LOADPATH = strrep (LOADPATH, "::", ":");
  endif

  ## don't work! Octave docs say that clear() can't be used inside a function
  lcd = pwd;
  cd (path);
  t = glob("*.m");
  for i = t'; clear(deblank(i')); end
  cd (lcd);

  if (strcmp(action, "on"))
    LOADPATH = [path, ":", LOADPATH];
    plplot_stub;
  elseif (strcmp(action, "off"))
    LOADPATH = [LOADPATH, ":", path];
  else
    help "use_plplot"
  endif
  
endfunction


