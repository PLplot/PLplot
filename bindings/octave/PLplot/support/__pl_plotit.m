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

function __pl_plotit

# __pl.type is the type of plot (polar, logx, etc.),
# __pl.items the number of triples (x, y, fmt) in the plot argument
# __pl.[x|y|items] stores the triples, as .x_i, .y_i, fmt_i,
# were _i varies from 1 to __pl_items

global __pl __pl_inited

if (!exist("__pl_inited") || plglevel == 0)
	figure(0);
endif

__pl_strm = plgstrm+1;

# character height, to correctly position labels

[a, ch_height] = plgchr;
ch_height = ch_height/100; # why 100? it works with me... docs say its units are mm

# plplot has no notion of log plots. What to plot for values <= 0?
# this is a kludge,  FIXME no matrix allowed!

i=0;	
while (i++ != __pl.items-1)
    if (__pl.type == 11 || __pl.type == 21 || __pl.type == 31)
		
		x = eval(sprintf("__pl.x%d;", i));
		y = eval(sprintf("__pl.y%d;", i));
		
	    if (__pl.type == 11)
			if (any(x <= 0))
		    	expr = "ix = find(x > 0);";
		    else
		    	break;
		    endif

		elseif (__pl.type == 21)
			if (any(y <= 0))
		    	expr = "ix = find(y > 0);";
		    else
		    	break;
		    endif
	    elseif (__pl.type == 31)
	    	if (any(y <= 0) || any(x <= 0))
			    expr = "ix = find(y > 0 & x > 0);";
			else
				break
			endif
	    endif
try    
		k = __pl.items;
    	while(!isempty(eval(expr)))
	    	ixx = ix(find(ix == (min(ix):rows(ix)+min(ix)-1)'));
	    	eval(sprintf("__pl.y%d = y(ixx,:);", k));
			eval(sprintf("__pl.x%d = x(ixx,:);", k));
			eval(sprintf("__pl.fmt%d = __pl.fmt%d;", k, i));
			x(ixx,:) = y(ixx,:) = []; k++;
	    endwhile
	    k--;
	    eval(sprintf("__pl.y%d = __pl.y%d;", i, k));
		eval(sprintf("__pl.x%d = __pl.x%d;", i, k));
		eval(sprintf("__pl.fmt%d = __pl.fmt%d;", i, k));		    
	    __pl.items = k;
catch
	error("FIXME: Logplot of matrices with negative elements is in some cases not allowed.\n");
end

	endif
endwhile

# in plplot one must define a viewport before plotting.
# In hold mode, the viewport will not be calculated for the current plot, and
# the last viewport (when not in hold mode) will be used, so plots made in
# hold mode can/will be clipped.

if (ishold == 0) 

	__pl_lxM = __pl_lyM = -realmax;
	__pl_lxm = __pl_lym = realmax;
		
	for i=1:__pl.items-1
	
		x = eval(sprintf("__pl.x%d;", i));
		y = eval(sprintf("__pl.y%d;", i));
			
		__pl_lxm = min([__pl_lxm, min(min(x))]);
		__pl_lxM = max([__pl_lxM, max(max(x))]);

		__pl_lym = min([__pl_lym, min(min(y))]);
		__pl_lyM = max([__pl_lyM, max(max(y))]);

# It seems that a viewport of (x, x, y, y) doesn't work

		if (__pl_lxm == __pl_lxM)		
			__pl_lxm = __pl_lxm - 10*eps; __pl_lxM = __pl_lxM + 10*eps;
		endif
		if (__pl_lym == __pl_lyM)
			__pl_lym = __pl_lym - 10*eps; __pl_lyM = __pl_lyM + 10*eps;
		endif
	endfor

# now check for user supplied axis ranges
# FIXME: possible polar plot inconsistency. User must specify axis in (x,y)
# coordinates, not (r,t). Advise user to use zoom() or ginput() to get (x,y)

	if (__pl.axis_st(__pl_strm))
		__pl_lxm = __pl.axis(__pl_strm,1); __pl_lxM = __pl.axis(__pl_strm,2);
		if (length(__pl.axis(__pl_strm,:)) >= 4)
			__pl_lym = __pl.axis(__pl_strm,3); __pl_lyM = __pl.axis(__pl_strm,4);
		else
			__pl.axis(__pl_strm,3) = __pl_lym; __pl.axis(__pl_strm,4) = __pl_lyM;
		endif
	else	# make axis() return current axis
		__pl.axis(__pl_strm,1) = __pl_lxm; __pl.axis(__pl_strm,2) = __pl_lxM;
		__pl.axis(__pl_strm,3) = __pl_lym; __pl.axis(__pl_strm,4) = __pl_lyM;
	endif

# for log plots,

    if (__pl.type == 11 || __pl.type == 31)
		__pl_lxm = log10(__pl_lxm); __pl_lxM = log10(__pl_lxM);
	endif
	if (__pl.type == 21 || __pl.type == 31)		
		__pl_lym = log10(__pl_lym); __pl_lyM = log10(__pl_lyM);
	endif
	
# start from the beginning

    __pl.plcol(__pl_strm) = 1;
    __pl.pllsty(__pl_strm) = 1;
    __pl.lab_pos(__pl_strm) = 1;
	__pl.lab_sym(__pl_strm,:) = 0;
	__pl.line_count(__pl_strm) = 1;
	__pl.lab_str = "";

    plcol(15); pllsty(1);
	if (__pl.type == -1)	# polar plot is special
		__pl_plenv(__pl_lxm, __pl_lxM, __pl_lym, __pl_lyM, 1, __pl.type);
	    if (__pl.grid(__pl_strm) == 1)
			__pl_polargrid(__pl_lxM, __pl_lyM, __pl_lxm, __pl_lym)
		endif
	else
		__pl_plenv(__pl_lxm, __pl_lxM, __pl_lym, __pl_lyM, 0, __pl.type + __pl.grid(__pl_strm));
	endif			    
endif

# get the plots to be done, previously stored in the __pl_struct structure

for items=1:__pl.items-1

	x = eval(sprintf("__pl.x%d;", items));
	y = eval(sprintf("__pl.y%d;", items));
	fmt = eval(sprintf("__pl.fmt%d;", items));

	if (__pl.type == 11 || __pl.type == 31)
		x = log10(x); 
	endif
	if (__pl.type == 21 || __pl.type == 31)
		y = log10(y);
	endif
	
# HERE - this is here just because of style variable
	if (isempty(fmt))
		[style, color, symbol, key_title] = __pl_opt(fmt);
	else
		[style, color, symbol, key_title] = __pl_opt(fmt(1,:));
	endif

	if (color != 20)
		__pl.plcol(__pl_strm) = color;
	endif

	xc = columns(x); yc = columns(y);

	if (is_matrix(x) && !is_vector(x) && is_matrix(y) && !is_vector(y)) 
		if (style != 9 && (xc != yc || rows(x) != rows(y)))
		      error ("__pl_plotit: matrix dimensions must match");
		endif
		range = "i;";	
	elseif (is_vector(x) || is_vector(y))
		range ="1:yc;";
	endif

	for i=1:xc
		for j=eval(range)

			if (is_strmatrix(fmt) && !is_strvector(fmt))
#				if (rows(fmt) < max(eval(range)))
# now reuse fmt
#					error ("__pl_plotit: matrix dimensions must match");
#				else
					[style, color, symbol, key_title] = __pl_opt(fmt(rem(j-1,rows(fmt))+1,:));
					if (color != 20)
						__pl.plcol(__pl_strm) = color;
					endif
#				endif
			endif

			if (isempty(key_title))
				if (!is_vector(x) || !is_vector(y))
					lab = sprintf("line %d.%d", __pl.line_count(__pl_strm), (i-1)*yc+j);
					if (i==xc && j==max(eval(range))); __pl.line_count(__pl_strm)=__pl.line_count(__pl_strm)+1; endif	# nothing more to do, hein?
				else
					lab = sprintf("line %d", __pl.line_count(__pl_strm));
					__pl.line_count(__pl_strm) = __pl.line_count(__pl_strm)+1;
				endif
			else
				if ((!is_vector(x) || !is_vector(y)) && is_strvector(fmt))
					lab = sprintf("%s %d", key_title, (i-1)*yc+j);
				else
					lab = sprintf("%s", key_title);
				endif
			endif

			if (__pl.legend(__pl_strm))
				__pl.lab_str = [__pl.lab_str; lab];
				__pl.lab_col(__pl_strm, __pl.lab_pos(__pl_strm)) = __pl.plcol(__pl_strm);
				__pl.lab_lsty(__pl_strm, __pl.lab_pos(__pl_strm)) = __pl.pllsty(__pl_strm);
				__pl.lab_pos(__pl_strm) = __pl.lab_pos(__pl_strm)+1;
				if (style == 1 || style == 5)
					__pl.lab_sym(__pl_strm, __pl.lab_pos(__pl_strm)-1) = symbol;
				else
					__pl.lab_sym(__pl_strm, __pl.lab_pos(__pl_strm)-1) = 0;
				endif
			endif

			plcol(__pl.plcol(__pl_strm));
			__pl.plcol(__pl_strm) = rem(__pl.plcol(__pl_strm), 15)+1;
			pllsty(__pl.pllsty(__pl_strm));
			if  (__pl.lstlyle(__pl_strm))
				__pl.pllsty(__pl_strm) = rem(__pl.pllsty(__pl_strm), 8)+1;
			endif

			switch (style)
				case (1)	# linespoints
					plline(x(:,i),y(:,j));
					plpoin(x(:,i),y(:,j), symbol);
				case (2)	# lines
					plline(x(:,i),y(:,j));
#				case (3) # boxerrorbars
#				case (4) # boxes
				case (5) # points
					plpoin(x(:,i),y(:,j), symbol);
				case (6) # dots
					plpoin(x(:,i),y(:,j), -1);								
#				case (7) # impulses
#				case (8) # steps
				case (9) # errorbars
				
					if ( (xc == 1 && yc == 1) || xc > 3 || yc > 3)
						error("plot with errorbars: either x or y or both must be 2 or 3 columns.\n\
				If x (or y) has two columns, then it is interpreted as (x, dx),\n\
				and a bar is plotted from x-dx to x+dx;\n\
				if it has three columns, it is interpreted as (x, xmin, xmax)");			
					endif
			
					if (xc == 2)
						xm = x(:,1) .- x(:,2); xM = x(:,1) .+ x(:,2);
					elseif (xc == 3)
						xm = x(:,2); xM = x(:,3);
					endif
					
					if (yc == 2)
						ym = y(:,1) .- y(:,2); yM = y(:,1) .+ y(:,2);
					elseif (yc == 3)
						ym = y(:,2); yM = y(:,3);
					endif
					
					if (xc != 1)
						plerrx( xm, xM, y(:,1));
					endif
					if (yc != 1)
						plerry( x(:,1), ym, yM);
					endif
						
				otherwise
					plline(x(:,i),y(:,j));
					warning("__pl_plotit: FIXME: format '%s' not implemented",fmt);
			endswitch
		if (style == 9);break;endif		# errorbars
		endfor # j=eval(range)
	endfor # i=1:xc
endfor # i=1:__pl.items-1

if (__pl.legend(__pl_strm))
__pl_draw_legend
endif

plcol(15);
pllab(tdeblank(__pl.xlabel(__pl_strm,:)), tdeblank(__pl.ylabel(__pl_strm,:)), tdeblank(__pl.tlabel(__pl_strm,:)));
plflush;pleop;

## if (!ishold)
__pl.items=1;
## endif

endfunction
