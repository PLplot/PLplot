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
##
## Use `/usr/local/share/plplot_octave' as the default location of plplot_octave.
## Use plplot_octave_path("path") to change it.

## File: use_plplot.m
## Author: Rafael Laboissiere <rafael@icp.inpg.fr>
## Modified: Joao Cardoso <jcardoso@inescn.pt>
## Created on: Sun Oct 18 22:03:10 CEST 1998
## Last modified on: March 2001
## 
## Copyright (C) 1998 Rafael Laboissiere
## 
## This file is free software, distributable under the GPL. No
## warranties, use it at your own risk.  It  was originally written for
## inclusion in the Debian octave-plplot package.

function use_plplot(action)

  global __pl_plplot_octave_path

  if (!exist("__pl_plplot_octave_path"))
    __pl_plplot_octave_path = "PLPLOT_OCTAVE_PATH";
  endif

  if (nargin == 0)
    action = "on";
  endif

  ix = findstr( LOADPATH,  __pl_plplot_octave_path);
  if (!isempty(ix))
    LOADPATH(ix, len( __pl_plplot_octave_path))= "";
  endif

  lcd = pwd;
  cd (plplot_octave_path);
  t = glob("*.m");
  for i=t';clear(deblank(i'));end
  cd (lcd);

  if (strcmp(action, "on"))
    LOADPATH = [__pl_plplot_octave_path, ":", LOADPATH];
    plplot_stub;
  elseif (strcmp(action, "off"))
    LOADPATH = [LOADPATH, ":", __pl_plplot_octave_path];
  else
    help "use_plplot"
  endif
  
endfunction


