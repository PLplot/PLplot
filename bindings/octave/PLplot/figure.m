## Copyright (C) 1998, 1999, 2000, 2001 Joao Cardoso.
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

## usage: [id driver intp] = figure ()
##
## usage: [id driver intp] = figure (n)
##
## usage: [id driver intp] = figure (n, device)
##
## usage: [id driver intp] = figure (n, device, file)
##
## usage: [id driver intp] = figure (n, device, file , win_id)
##
## usage: [id driver intp] = figure (n, device, file , win_id, tk_file, plot_frame)
##
## Set the current plot window to plot window n or creates a new figure,
##    returning the window id and driver;
## If the driver is tk, also returns its interpreter name 
## If device is specified, it is opened. Default is 'xwin'.
## If file is specified, it is passed to PLplot for use. Can be a real file for
##    plot output (e.g. postscript), or a  Xserver DISPLAY name. 
## If win_id is specified, the plot will use the X window identified by win_id
##   if the driver is "xwin". Use program `xwininfo' to get its hexadecimal id
##   and octave `hex2dec' to get its decimal id.
##
## For use with an external .tcl script:
##
## If tk_file is specified, the tk driver executes this file. In this case,
##    where tk_file tipically draws a tk GUI, plot_frame should be the name of
##    an empty frame where the actual plot will take place.
##
## if file == "" or win_id == 0 no action is taken for this arguments.

function [n, driver, intp]= figure (n, device, file, win_id, tk_file, plot_frame)

  global __pl __tk_name

  if (!exist("__pl") || !struct_contains (__pl,"inited"))
    plplot_stub;

    ## closeallfig must be called once before atexit is called!
    closeallfig;
    atexit("closeallfig");

    __pl.inited = 1;
    if (nargin == 0)
      n=0; driver=""; intp="";
      return
    endif
  endif

  if (nargin == 0)
    n = plgstrm;
  else  	
    __pl.hold(plgstrm+1) = ishold; # save current hold state
    plsstrm(n);
    n = plgstrm;
    __pl_strm = n+1;

    if ( plglevel == 0)	# no device open yet

      if (nargin >= 4 && win_id != 0 ) # use the specified X window
	plsxwin(win_id)
      endif

      if (nargin >= 3 && !isempty(file)) # use the specified file
	plsfnam(file)
      endif

      if (nargin >= 2 )
	if (isstr(device))
	  plsdev(device);
	else
	  error("figure: `device' must be a string");
	endif
      else
	device = plsetopt ("get", "dev"); # possible user defaults
	if (isempty(device))
	  device="xwin";
	endif
	plsdev(device);
      endif

      __pl.type = 1;
      __pl.legend_xpos(__pl_strm) = 1;	# legend x position
      __pl.legend_ypos(__pl_strm) = 1;	# legend y position
      __pl.legend(__pl_strm) = 2;	# legend type
      __pl.lab_pos(__pl_strm) = 0;	# label position
      __pl.lab_lsty(__pl_strm,1) = 0;	# label (line) style
      __pl.lab_col(__pl_strm,1) = 0;	# label color
      __pl.multi_row(__pl_strm) = 1;	# multiplot state
      __pl.multi_col(__pl_strm) = 1;	# multiplot columns
      __pl.multi_cur(__pl_strm) = 1;	# current multiplot
      __pl.multi(__pl_strm) = 0;	# multiplot	rows
      __pl.axis_st(__pl_strm) = 0;	# axis state
      __pl.axis(__pl_strm,:) = zeros(1,6);	# current axis
      __pl.xticks(__pl_strm,:) = [0, 0]; # xtick interval, number of minor xticks
      __pl.yticks(__pl_strm,:) = [0, 0];
      __pl.zticks(__pl_strm,:) = [0, 0]; 		
      __pl.lstlyle(__pl_strm) = 1;       # increase line style after plot
      __pl.az(__pl_strm) = -60;	         # azimuth
      __pl.alt(__pl_strm) = 30;		 # altitude
      __pl.grid(__pl_strm) = 0;		 # grid state
      __pl.plcol(__pl_strm) = 1;	 # current plot color
      __pl.pllsty(__pl_strm) = 1;	 # current plot style
      __pl.line_count(__pl_strm) = 1;    # current label counter
      __pl.hold(__pl_strm) = 0; hold off;  # hold state
      __pl.open(__pl_strm) = 1;		   # figure state
      __pl.margin(__pl_strm) = 0;	   # keep a small margin around box
      
      if (struct_contains(__pl, "xlabel"))
	__pl.xlabel = __pl_matstr( __pl.xlabel, "X", __pl_strm); # x,y,z,title labels text
	__pl.ylabel = __pl_matstr( __pl.ylabel, "Y", __pl_strm);
	__pl.zlabel = __pl_matstr( __pl.zlabel, "Z", __pl_strm);
	__pl.tlabel = __pl_matstr( __pl.tlabel, "Title", __pl_strm);
      else
	__pl.xlabel(__pl_strm,:) = "X";
	__pl.ylabel(__pl_strm,:) = "Y";
	__pl.zlabel(__pl_strm,:) = "Z";
	__pl.tlabel(__pl_strm,:) = "Title";
      endif

      if (struct_contains(__pl, "intp"))
	__pl.intp = __pl_matstr(__pl.intp, " ", __pl_strm); # tk interpreter name		
      else
	__pl.intp(__pl_strm,:) = " ";
      endif

      ## the tk stuff
      if (strcmp("tk", sprintf("%s",plgdev')))
	if (! exist("tk_start") && nargin == 6)
	  error("Can't use this Tk feature of PLplot until tk_octave \
	      is installed!\n")
	elseif (exist("tk_start"))
	 if (!exist("__tk_name"))
	  tk_init;
	 endif

	  init_file = tmpnam();
	  fp = fopen (init_file,"w");

	  fprintf(fp, "set octave_interp {%s}\n", __tk_name);
	  fprintf(fp, "set octave_interp_pid %d\n", getpid);
	  fprintf(fp, "send -async $octave_interp to_octave intp=\"[tk appname]\"\n");

	  fprintf(fp, "proc to_octave {a args} {\n");
	  fprintf(fp, "global octave_interp octave_interp_pid;\n");
	  fprintf(fp, "send -async $octave_interp to_octave \"$a $args\";\n");
	  fprintf(fp, "#exec kill -16 $octave_interp_pid}\n");
	endif
	
	if (nargin == 6)
	  fprintf(fp, "source {%s}\n", tk_file);
	  fclose(fp);
	  plSetOpt ("plwindow", plot_frame);
	  plSetOpt ("tk_file", init_file);
	elseif (exist("__tk_name"))
	  fclose(fp);
	  plSetOpt ("tk_file", init_file);
	  plSetOpt("plwindow", sprintf(".figure_%d",n));
	else
  	  plSetOpt("plwindow", sprintf(".figure_%d",n));
	endif

	intp = sprintf("figure_%d",n);
	__pl.intp = __pl_matstr(__pl.intp, intp, __pl_strm);	# tk interpreter name
      else
	plSetOpt("plwindow", sprintf("Figure %d",n));
      endif

      plSetOpt("geometry", "400x400+800+1");
      plSetOpt("np", "");
      pldef	# user can override above defaults or add other options
      plsetopt("apply"); # override/add momentary options.

      ## init driver and make changes apply
      plinit
      pladv(0)
      plflush;pleop;
      
      if ( exist("__tk_name") & (strcmp("tk", sprintf("%s",plgdev'))))
	eval(tk_receive(1));
	__pl.intp = __pl_matstr(__pl.intp, intp, __pl_strm);	# tk interpreter name					
	unlink(init_file);
      else
	intp = deblank(__pl.intp(__pl_strm,:));
      endif

    else
      if (__pl.hold(__pl_strm))
   	hold on
      endif
      intp = deblank(__pl.intp(__pl_strm,:));
      ## warning("figure already opened");
    endif
  endif

  driver = sprintf("%s",plgdev');

  if (!exist("intp"))
    intp = deblank(__pl.intp(n+1,:));
  endif

endfunction
