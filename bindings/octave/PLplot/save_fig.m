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

## save_fig(file, [device [, reverse]])
##
## Save current plot figure in 'file' for 'device' type.
## If reverse is 1 and the background is black, reverse black and white.
## 	(This feature been tested with the xwin and tk driver only)
##
## If the global variable FIGDIR exists, `file' is appended to FIGDIR
## if `file' does not start with any of `/~.'
##
## The file name extension determines the device type, unless it is specified.
## Defaults are "psc" (Colour Postscript), and reverse
## The special output file `|lp' send the output to the printer.
## Use `lp_setup' to setup your printer.
##
## What is saved is all that is available in the plot buffer. To see what is
## available, type `plreplot'.
## To clear the plot buffer type `plclearplot' or `plclg'
##
## *Usual* devices are:
##	
##	ps         PostScript File (monochrome)
##	psc        PostScript File (color)
##	plmeta     PLPLOT Native Meta-File (can be displayed with 'plrender')
##	xfig       Xfig file
##	pbm        PDB (PPM) Driver
##	lj_hpgl    HP Laserjet III, HPGL emulation mode
##	hp7470     HP 7470 Plotter File (HPGL Cartridge, Small Plotter)
##	hp7580     HP 7580 Plotter File (Large Plotter)

function save_fig(file, device, rev)

  global __pl __pl_inited __lp_options FIGDIR

  if (!exist("__pl_inited") || plglevel == 0)
    warning("Nothing to save");
    return
  endif

  if (!exist("__lp_options"))
    __lp_options = "lp -c";
  endif

  __pl_strm = plgstrm + 1;

  if (nargin < 3)
    rev = 1;
  endif

  if (nargin < 2 )
    device = "ps";	
    ix = rindex(file, '.');
    if (ix)
      device = file(ix+1:length(file));
    endif
  endif

  ## FIXME -- this should be get from plgDevs()
  vdev = [ "xwin"; "tk"; "xterm"; "tekt"; "tek4107t"; "mskermit"; "versaterm";
	  "vlt"; "conex"; "dg300"; "plmeta"; "tekf"; "tek4107f"; "ps"; "psc";
	  "xfig"; "ljiip"; "ljii"; "hp7470"; "hp7580"; "lj_hpgl"; "imp"; "pbm"];

  dev = "";
  for i=1:rows(vdev)
    if (strcmp(device, tdeblank(vdev(i,:))))
      dev = device;
      break;
    endif
  endfor

  if (isempty(dev))
    dev = "psc";
    ##  file = [file, '.', dev];
    warning("Using Color Postscript. Valid device types are:\n"); disp(vdev);
  endif

  device = dev;

  if (nargin >= 1 && isstr(device) & isstr(file))
    
    to_prt = 0;
    if (strcmp(file,"|lp"))	# special output file, printer!
      to_prt = 1;
      file = tmpnam;
    elseif (file(1) == '~')
      file = tilde_expand(file);
    elseif (exist("FIGDIR") & file(1) != '/' & file(1) != '.')
      file = [FIGDIR, '/', file];
    endif

    eval(sprintf("save %s device", file),"error('Can\\'t create file\\n'); return");

    ## unfortunately the procedure is different for the xwin and tk driver

    if (strcmp("xwin", sprintf("%s",plgdev')))

      cur_fig = plgstrm;
      f = min(find(__pl.open == 0)); # find new stream and make it default
      if (isempty(f))
	f = plmkstrm;
      endif
      
      plsstrm(f); # set stream type and output file
      plsfnam(file);
      plsdev(device);

      rev_done = 0;
      if (rev == 1) # exchange background before plinit(), if needed
	[r, g, b] = plgcolbg;
	if (r == 0 && g == 0 && b == 0)	# black background
	  rev_done = 1;
	  plSetOpt("bg", "FFFFFF");	# set it white
	endif
      endif

      plinit;
      ## pladv(0); # This worked, but not any more. Changes in plplot?
      
      plcpstrm(cur_fig, 0); # copy parameters

      if (rev_done == 1) # and exchange black/white if needed, after plinit()
	plscol0(0, 255, 255, 255);
	plscol0(15, 0, 0, 0);
      endif

      plreplot;	# do the save

      plend1;
      plsstrm(cur_fig);	# return to previous stream

    elseif (strcmp("tk", sprintf("%s",plgdev')))
      if (! exist("tk_start"))
	if  (! exist("tk_init"))
	  error("Can't use this tk feature of PLplot until tk_octave is installed!\n")
	else
	  tk_init;
	endif
      endif

      [cur_fig, driver, intp] = figure;
      intp = tdeblank(intp);
      
      if (1)
	## This works! not anymore, `client' blocks!
	client = tk_cmd(intp, "set client_name",1);
	wind = tk_cmd(client,"set plwindow",1);
      else
	## A small patch to plwidget.tcl makes this work:
	## (make plwidget global and setting it to `w' in `plxframe'
	wind = tk_cmd(intp,"set plwidget",1);
      endif
      
      rev_done = 0;
      if (rev == 1)
	t = tk_cmd(intp, sprintf("%s.plwin configure -bg", wind), 1);
	t = split(t," ");
	if (strcmp(tdeblank(t(5,:)) , "black"))
	  rev_done = 1;
	  tk_cmd(intp, sprintf("%s.plwin configure -bg white", wind), 0);
	  tk_cmd(intp, sprintf("%s.plwin cmd plscol0 0 white", wind), 0);
	  tk_cmd(intp, sprintf("%s.plwin cmd plscol0 15 black", wind), 0);
	endif
      endif

      tk_cmd(intp, sprintf("%s.plwin save as %s %s", wind, device, file), 0);
      tk_cmd(intp, sprintf("%s.plwin save close", wind), 0);
      if (rev_done == 1)
	tk_cmd(intp, sprintf("%s.plwin configure -bg black", wind), 0);
	tk_cmd(intp, sprintf("%s.plwin cmd plscol0 0 black", wind), 0);
	tk_cmd(intp, sprintf("%s.plwin cmd plscol0 15 white", wind), 0);			
      endif
    endif

    if (to_prt == 1)
      system(sprintf("%s %s;", __lp_options, file));
      unlink(file);
    endif
  else
    help save_fig
  endif	

endfunction
