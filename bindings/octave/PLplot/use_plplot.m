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
## Last modified on: Fri Dec 11 17:09:58 CET 1998
## 
## Copyright (C) 1998 Rafael Laboissiere
## 
## This file is free software, distributable under the GPL. No
## warranties, use it at your own risk.  It  was originally written for
## inclusion in the Debian octave-plplot package.

function use_plplot(action)

global __pl_plplot_octave_path

if (!exist("__pl_plplot_octave_path"))
  __pl_plplot_octave_path = "/usr/local/share/plplot_octave";
endif

  if (nargin == 0)
    action = "on";
  endif

  old_empty = empty_list_elements_ok;
  empty_list_elements_ok = 1;
  dirs = split(LOADPATH, ":");
  n = size(dirs,1);

  plpath = load_path = [];
  for i=1:n
    if strcmp(deblank(dirs(i,:)),__pl_plplot_octave_path)
      plpath = [plpath, i];
    endif
  endfor

  j = 1;
  for i=1:n
    if find(plpath != i)
      if j==1
        load_path = deblank(dirs(i,:));
      else
        load_path = [load_path, ":", deblank(dirs(i,:))];
      endif
      j = j+1;
    endif
  endfor

  if (strcmp(action, "on"))
    LOADPATH = [__pl_plplot_octave_path, ":", load_path];
    plplot_stub;
  elseif (strcmp(action, "off"))
    LOADPATH = load_path;
  else
  	help "use_plplot"
  endif
  
 empty_list_elements_ok = old_empty;
    
endfunction


