## usage: toggle_plplot_use
##
## Use this script to activate/deactivate the default library for the
## PLplot plotting function (plot, mesh, etc.), as both the native
## gnuplot and the new octave-plplot libraries use similar names to the
## functions. toggle_plplot_use post-/pre-pends to LOADPATH the path for
## the PLplot functions in successive calls.

## File: toggle_plplot_use.m
## Author: Rafael Laboissiere <rafael@debian.org>
## Modified: Joao Cardoso
## Created on: Sun Oct 18 22:03:10 CEST 1998
## Last modified on: Fri Mar  7 09:38:38 CET 2003
##
## Copyright (C) 2003  Rafael Laboissiere
##
## PLplot is free software, distributable under the GPL. No
## warranties, use it at your own risk.  It  was originally written for
## inclusion in the Debian octave-plplot package.

1;

if ! exist ("use_plplot_state")
  global use_plplot_state
  use_plplot_state = "on";
  # Warn user about spurious warnings with octave 2.9
  ver = str2num(split(version,"."));
  if ((ver(1) == 2 && ver(2) == 9) || (ver(1) >= 3)) 
    if (warning("query","Octave:built-in-variable-assignment").state == "on")
      warning("You may want to call\n  warning(\"off\",\"Octave:built-in-variable-assignment\");\nto prevent spurious warnings from the plplot code for compatability with octave 2.0/2.1.\n");
    endif
  endif
else
  if strcmp (use_plplot_state, "on")
    use_plplot_state = "off";
  else
    use_plplot_state = "on";
  endif
endif

use_plplot_path = plplot_octave_path;
# Strip of trailing // for octave >= 2.9
if (!exist("LOADPATH"))
  use_plplot_path = use_plplot_path(1:end-2);
endif
if (exist("LOADPATH"))
  use_plplot_i = findstr (LOADPATH, use_plplot_path);
  if (!isempty (use_plplot_i))
    LOADPATH (use_plplot_i(1):use_plplot_i(1)+length(use_plplot_path)-1)= "";
    LOADPATH = strrep (LOADPATH, "::", ":");
  endif
else
  plplot_path_to_remove = split(genpath(use_plplot_path),pathsep);
  for i=1:size(plplot_path_to_remove)(1)
    if (findstr(path,deblank(plplot_path_to_remove(i,:))) > 0)
      rmpath(deblank(plplot_path_to_remove(i,:)));
    endif
  endfor
endif

if (strcmp (use_plplot_state, "on"))
  if (exist("LOADPATH"))
    LOADPATH = [use_plplot_path, ":", LOADPATH];
  else
    addpath(genpath(use_plplot_path));
  endif
  plplot_stub;
elseif (strcmp (use_plplot_state, "off"))
  if (exist("LOADPATH"))
    LOADPATH = [LOADPATH, ":", use_plplot_path];
  else
    addpath(genpath(use_plplot_path),"-end");
  endif
endif

use_plplot_lcd = pwd;
cd (use_plplot_path);
for  use_plplot_i = [ char(glob("*.m ")); char(glob("support/*.m ")) ]'
  clear (strrep (strrep (deblank(use_plplot_i'), ".m", ""), "support/", ""));
end
cd (use_plplot_lcd);

clear use_plplot_path use_plplot_lcd use_plplot_i plplot_path_to_remove

printf ("Use PLplot: %s\n", use_plplot_state);

