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
## Copyright (C) 2003 Rafael Laboissiere
##
## This file is free software, distributable under the GPL. No
## warranties, use it at your own risk.  It  was originally written for
## inclusion in the Debian octave-plplot package.

1;

if ! exist ("use_plplot_state")
  global use_plplot_state
  use_plplot_state = "on";
else
  if strcmp (use_plplot_state, "on")
    use_plplot_state = "off";
  else
    use_plplot_state = "on";
  endif
endif

path = plplot_octave_path;
ix = findstr (LOADPATH, path);
if (!isempty (ix))
  LOADPATH (ix(1):ix(1)+length( path)-1)= "";
  LOADPATH = strrep (LOADPATH, "::", ":");
endif

if (strcmp (use_plplot_state, "on"))
  LOADPATH = [path, ":", LOADPATH];
  plplot_stub;
elseif (strcmp (use_plplot_state, "off"))
  LOADPATH = [LOADPATH, ":", path];
endif

lcd = pwd;
cd (path);
t = [ glob ("*.m "); glob ("support/*.m ") ];
for i = t'
  clear (strrep (strrep (deblank(i'), ".m", ""), "support/", ""));
end

printf ("Use PLplot: %s\n", use_plplot_state);
