## -*- coding: utf-8; -*-
## $Id$
##  Copyright (C) 2010 Alan W. Irwin
##
##  Demonstrate most pllegend capability including unicode symbols.
##
##  This file is part of PLplot.
##
##  PLplot is free software; you can redistribute it and/or modify
##  it under the terms of the GNU General Library Public License as published
##  by the Free Software Foundation; either version 2 of the License, or
##  (at your option) any later version.
###
##  PLplot is distributed in the hope that it will be useful,
##  but WITHOUT ANY WARRANTY; without even the implied warranty of
##  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##  GNU Library General Public License for more details.
###
##  You should have received a copy of the GNU Library General Public License
##  along with PLplot; if not, write to the Free Software
##  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
##
##  This example designed just for devices (e.g., the cairo-related and
##  qt-related devices) where the best choice of glyph is automatically
##  selected by the related libraries (pango/cairo or Qt4) for each
##  unicode character depending on what system fonts are installed.  Of
##  course, you must have the appropriate TrueType fonts installed to
##  have access to all the required glyphs.

1;

global position_options special_symbols
position_options = [
		    bitor(PL_POSITION_LEFT, bitor(PL_POSITION_UPPER, PL_POSITION_OUTSIDE));
                    bitor(PL_POSITION_UPPER, PL_POSITION_OUTSIDE);
                    bitor(PL_POSITION_RIGHT, bitor(PL_POSITION_UPPER, PL_POSITION_OUTSIDE));
                    bitor(PL_POSITION_RIGHT, PL_POSITION_OUTSIDE);
		    bitor(PL_POSITION_RIGHT, bitor(PL_POSITION_LOWER, PL_POSITION_OUTSIDE));
		    bitor(PL_POSITION_LOWER, PL_POSITION_OUTSIDE);
		    bitor(PL_POSITION_LEFT, bitor(PL_POSITION_LOWER, PL_POSITION_OUTSIDE));
		    bitor(PL_POSITION_LEFT, PL_POSITION_OUTSIDE);
		    bitor(PL_POSITION_LEFT, bitor(PL_POSITION_UPPER, PL_POSITION_INSIDE));
		    bitor(PL_POSITION_UPPER, PL_POSITION_INSIDE);
		    bitor(PL_POSITION_RIGHT, bitor(PL_POSITION_UPPER, PL_POSITION_INSIDE));
		    bitor(PL_POSITION_RIGHT, PL_POSITION_INSIDE);
		    bitor(PL_POSITION_RIGHT, bitor(PL_POSITION_LOWER, PL_POSITION_INSIDE));
		    bitor(PL_POSITION_LOWER, PL_POSITION_INSIDE);
		    bitor(PL_POSITION_LEFT, bitor(PL_POSITION_LOWER, PL_POSITION_INSIDE));
		    bitor(PL_POSITION_LEFT, PL_POSITION_INSIDE)
		    ];

## Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
special_symbols = [
		   "✰";
		   "✴";
		   "✱";
		   "✽";
		   "✦"
		   ];

function plcolorbar_example_1( bar_type, ticks, sub_ticks, values, title )
  global PL_COLORBAR_LEFT PL_COLORBAR_LABEL_LEFT PL_COLORBAR_CAP_HIGH PL_COLORBAR_SHADE_LABEL PL_COLORBAR_RIGHT PL_COLORBAR_LABEL_RIGHT PL_COLORBAR_CAP_LOW PL_COLORBAR_UPPER PL_COLORBAR_LABEL_UPPER PL_COLORBAR_CAP_HIGH PL_COLORBAR_LOWER PL_COLORBAR_LABEL_LOWER PL_COLORBAR_CAP_LOW 
  pladv( 0 );
  ## Setup color palette 1
  plspal1( "cmap1_blue_red.pal", 1 );

  n = length(values);
  color_step = 1.0 / (n - 1);
  colors = (0:n-1)'*color_step;

  opt = bitor(PL_COLORBAR_LEFT, bitor(bar_type, bitor(PL_COLORBAR_LABEL_LEFT, PL_COLORBAR_CAP_HIGH)));

  if (bitand(bar_type, PL_COLORBAR_SHADE_LABEL))
    axis_opts_1 = "iv";
    axis_opts_2 = "i";
  else
    if (sub_ticks != 0)
      axis_opts_1 = "stv";
      axis_opts_2 = "st";
    else
      axis_opts_1 = "tv";
      axis_opts_2 = "t";
    endif
  endif

  plcolorbar( opt, 0.1, 0.1, 0.5, 0.1,
             ticks, sub_ticks,
             axis_opts_1, "Test label - Left, High Cap",
             colors, values );

  opt = bitor(PL_COLORBAR_RIGHT, bitor(bar_type, bitor(PL_COLORBAR_LABEL_RIGHT, PL_COLORBAR_CAP_LOW)));

  plcolorbar( opt, 0.1, 0.4, 0.5, 0.1,
             ticks, sub_ticks,
             axis_opts_1, "Test label - Right, Low Cap",
             colors, values );

  opt = bitor(PL_COLORBAR_UPPER, bitor(bar_type, bitor(PL_COLORBAR_LABEL_UPPER, PL_COLORBAR_CAP_HIGH)));

  plcolorbar( opt, 0.1, 0.1, 0.5, 0.1,
             ticks, sub_ticks,
             axis_opts_2, "Test label - Upper, High Cap",
             colors, values );

  opt = bitor(PL_COLORBAR_LOWER, bitor(bar_type, bitor(PL_COLORBAR_LABEL_LOWER, PL_COLORBAR_CAP_LOW)));

  plcolorbar( opt, 0.4, 0.1, 0.5, 0.1,
             ticks, sub_ticks,
             axis_opts_2, "Test label - Lower, Low Cap",
             colors, values );

  plvpor( 0.0, 1.0, 0.0, 1.0 );
  plwind( 0.0, 1.0, 0.0, 1.0 );
  plptex( 0.5, 0.5, 0.0, 0.0, 0.5, title );
endfunction

function plcolorbar_example_2( bar_type, ticks, sub_ticks, values, title )
  global PL_COLORBAR_LEFT PL_COLORBAR_LABEL_LEFT PL_COLORBAR_CAP_HIGH PL_COLORBAR_SHADE_LABEL PL_COLORBAR_RIGHT PL_COLORBAR_LABEL_RIGHT PL_COLORBAR_CAP_LOW PL_COLORBAR_UPPER PL_COLORBAR_LABEL_UPPER PL_COLORBAR_CAP_HIGH PL_COLORBAR_LOWER PL_COLORBAR_LABEL_LOWER PL_COLORBAR_CAP_LOW 
  pladv( 0 );
  ## Setup color palette 1
  plspal1( "cmap1_blue_yellow.pal", 1 );

  n = length(values);
  color_step = 1.0 / (n - 1);
  colors = (0:n-1)'*color_step;
  opt = bitor(PL_COLORBAR_LEFT, bitor(bar_type, bitor(PL_COLORBAR_LABEL_LEFT, PL_COLORBAR_CAP_LOW)));

  if (bar_type == PL_COLORBAR_SHADE_LABEL)
    axis_opts_1 = "";
    axis_opts_2 = "";
  else
    if( sub_ticks != 0)
      axis_opts_1 = "stv";
      axis_opts_2 = "st";
    else
      axis_opts_1 = "tv";
      axis_opts_2 = "t";
    endif
  endif

  plcolorbar( opt, 0.1, 0.1, 0.5, 0.1,
             ticks, sub_ticks,
             axis_opts_1, "Test label - Left, Low Cap",
             colors, values );

  opt = bitor(PL_COLORBAR_RIGHT, bitor(bar_type, bitor(PL_COLORBAR_LABEL_RIGHT, PL_COLORBAR_CAP_HIGH)));

  plcolorbar( opt, 0.1, 0.4, 0.5, 0.1,
             ticks, sub_ticks,
             axis_opts_1, "Test label - Right, High Cap",
             colors, values );

  opt = bitor(PL_COLORBAR_UPPER, bitor(bar_type, bitor(PL_COLORBAR_LABEL_UPPER, PL_COLORBAR_CAP_LOW)));

  plcolorbar( opt, 0.1, 0.1, 0.5, 0.1,
             ticks, sub_ticks,
             axis_opts_2, "Test label - Upper, Low Cap",
             colors, values );

  opt = bitor(PL_COLORBAR_LOWER, bitor(bar_type, bitor(PL_COLORBAR_LABEL_LOWER, PL_COLORBAR_CAP_HIGH)));

  plcolorbar( opt, 0.4, 0.1, 0.5, 0.1,
             ticks, sub_ticks,
             axis_opts_2, "Test label - Lower, High Cap",
             colors, values );

  plvpor( 0.0, 1.0, 0.0, 1.0 );
  plwind( 0.0, 1.0, 0.0, 1.0 );
  plptex( 0.5, 0.5, 0.0, 0.0, 0.5, title );
endfunction

function ix33c()
  ## Parse and process command line arguments

  ## (void) plparseopts(&argc, argv, PL_PARSE_FULL);
  
  ## Initialize plplot
  ## set global attributes for PLplot variables used in this function.
  global PL_FCI_SANS PL_FCI_MONO PL_LEGEND_NONE PL_LEGEND_COLOR_BOX PL_LEGEND_LINE PL_LEGEND_SYMBOL PL_LEGEND_TEXT_LEFT PL_LEGEND_BACKGROUND PL_LEGEND_BOUNDING_BOX PL_LEGEND_ROW_MAJOR 
  global PL_POSITION_LEFT PL_POSITION_RIGHT PL_POSITION_UPPER PL_POSITION_LOWER PL_POSITION_INSIDE PL_POSITION_OUTSIDE 
  global PL_COLORBAR_IMAGE PL_COLORBAR_SHADE PL_COLORBAR_GRADIENT PL_COLORBAR_SHADE_LABEL
  global position_options special_symbols
  plinit();

  ## First page illustrating the 16 standard positions.
  pladv(0);
  plvpor(0.25, 0.75, 0.25, 0.75);
  plwind(0.0, 1.0, 0.0, 1.0);
  plbox("bc", 0.0, 0, "bc", 0.0, 0);
  plsfont(PL_FCI_SANS, -1, -1);
  plmtex("t", 8.0, 0.5, 0.5, "The 16 standard legend positions with");
  plmtex("t", 6.0, 0.5, 0.5, "the same (0.05) offset in x and y");

  ## Set up legend arrays with the correct size, type.  In octave this may
  ## be done by clearing the array (in case it was used before) and then
  ## writing the last element of the array with a placeholder of the right
  ## type.  Note that for strings the right placeholder length doesn't
  ## matter.  Octave keeps track of the longest string in each array and pads
  ## out all other members with blanks to that length if a subsequent string
  ## assignment for an array element exceeds that length.
  nlegend = 1;

  opt_array = 0;
  text_colors = 0;
  text = " ";
  box_colors = 0;
  box_patterns = 0;
  box_scales = 0.;
  box_line_widths = 0;
  line_colors = 0;
  line_styles = 0;
  line_widths = 0;
  symbol_colors = 0;
  symbol_scales = 0.;
  symbol_numbers = 0;
  symbols = " ";

  opt_array(nlegend,1) = 0;
  text_colors(nlegend,1) = 0;
  text(nlegend,1:length(" ")) = " ";
  line_colors(nlegend,1) = 0;
  line_styles(nlegend,1) = 0;
  line_widths(nlegend,1) = 0;
  symbol_colors(nlegend,1) = 0;
  symbol_scales(nlegend,1) = 0.;
  symbol_numbers(nlegend,1) = 0;
  symbols(nlegend,1:length(" ")) = " ";

  ## Only specify legend data that are required according to the
  ## value of opt_array for that entry.
  opt_base = bitor(PL_LEGEND_BACKGROUND, PL_LEGEND_BOUNDING_BOX);
  opt_array(1) = bitor(PL_LEGEND_LINE, PL_LEGEND_SYMBOL);
  line_styles(1) = 1;
  line_widths(1) = 1;
  symbol_scales(1) = 1.;  symbol_numbers(1) = 4;
  symbols(1,1:length("*")) = "*";

  ## Use monotype fonts so that all legends are the same size.
  plsfont( PL_FCI_MONO , -1, -1);
  plscol0a( 15, 32, 32, 32, 0.70 );

  for k=0:15
    position = position_options(k+1);
    opt = opt_base;
    string = sprintf("%2.2d", k);
    text(1,1:length(string)) = string;
    text_colors(1) = 1 + mod(k, 8);
    line_colors(1) = 1 + mod(k, 8);
    symbol_colors(1) = 1 + mod(k, 8);

    [legend_width, legend_height] = \
	pllegend( position, opt, 0.05, 0.05,
		 0.1, 15, 1, 1, 0, 0, opt_array, 1.0, 1.0, 2.0,
		 1., text_colors, text,
		 [], [], [], [],
		 line_colors, line_styles, line_widths,
		 symbol_colors, symbol_scales, symbol_numbers, symbols );
  endfor
  ## Second page illustrating effect of nrow, ncolumn for the same legend
  ## data.
  pladv(0);
  plvpor(0.25, 0.75, 0.25, 0.75);
  plwind(0.0, 1.0, 0.0, 1.0);
  plbox("bc", 0.0, 0, "bc", 0.0, 0);
  plsfont(PL_FCI_SANS, -1, -1);
  plmtex("t", 8.0, 0.5, 0.5, "The effect of nrow, ncolumn, PL_LEGEND_ROW_MAJOR,");
  plmtex("t", 6.0, 0.5, 0.5, "and position for the same legend data");

  ## Set up legend arrays with the correct size, type.  In octave this may
  ## be done by clearing the array (in case it was used before) and then
  ## writing the last element of the array with a placeholder of the right
  ## type.  Note that for strings the right placeholder length doesn't
  ## matter.  Octave keeps track of the longest string in each array and pads
  ## out all other members with blanks to that length if a subsequent string
  ## assignment for an array element exceeds that length.
  nlegend = 7;

  opt_array = 0;
  text_colors = 0;
  text = " ";
  box_colors = 0;
  box_patterns = 0;
  box_scales = 0.;
  box_line_widths = 0;
  line_colors = 0;
  line_styles = 0;
  line_widths = 0;
  symbol_colors = 0;
  symbol_scales = 0.;
  symbol_numbers = 0;
  symbols = " ";

  opt_array(nlegend,1) = 0;
  text_colors(nlegend,1) = 0;
  text(nlegend,1:length(" ")) = " ";
  line_colors(nlegend,1) = 0;
  line_styles(nlegend,1) = 0;
  line_widths(nlegend,1) = 0;
  symbol_colors(nlegend,1) = 0;
  symbol_scales(nlegend,1) = 0.;
  symbol_numbers(nlegend,1) = 0;
  symbols(nlegend,1:length(" ")) = " ";

  ## Only specify legend data that are required according to the
  ## value of opt_array for that entry.
  opt_base = bitor(PL_LEGEND_BACKGROUND, PL_LEGEND_BOUNDING_BOX);
  for k=0:nlegend-1
    opt_array(k+1) = bitor(PL_LEGEND_LINE, PL_LEGEND_SYMBOL);
    line_styles(k+1) = 1;
    line_widths(k+1) = 1;
    symbol_scales(k+1) = 1.;
    symbol_numbers(k+1) = 2;
    symbols(k+1,1:length("*")) = "*";
    string = sprintf("%2.2d",k);
    text(k+1,1:length(string)) = string;
    text_colors(k+1) = 1 + mod(k,8);
    line_colors(k+1) = 1 + mod(k,8);
    symbol_colors(k+1) = 1 + mod(k,8);
  endfor

  ## Use monotype fonts so that all legends are the same size.
  plsfont(PL_FCI_MONO, -1, -1);
  plscol0a( 15, 32, 32, 32, 0.70 );

  position = bitor(PL_POSITION_UPPER, PL_POSITION_OUTSIDE);
  opt = opt_base;
  x = 0.;
  y = 0.1;
  nrow = 1;
  ncolumn = nlegend;
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
	       0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
	       1., text_colors, text,
	       [], [], [], [],
	       line_colors, line_styles, line_widths,
	       symbol_colors, symbol_scales, symbol_numbers, symbols );

  position = bitor(PL_POSITION_LOWER, PL_POSITION_OUTSIDE);
  opt = opt_base;
  x = 0.;
  y = 0.1;
  nrow = 1;
  ncolumn = nlegend;
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
	       0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
	       1., text_colors, text,
	       [], [], [], [],
	       line_colors, line_styles, line_widths,
	       symbol_colors, symbol_scales, symbol_numbers, symbols );

    
  position = bitor(PL_POSITION_LEFT, PL_POSITION_OUTSIDE);
  opt = opt_base;
  x = 0.1;
  y = 0.;
  nrow = nlegend;
  ncolumn = 1;
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
	       0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
	       1., text_colors, text,
	       [], [], [], [],
	       line_colors, line_styles, line_widths,
	       symbol_colors, symbol_scales, symbol_numbers, symbols );

  position = bitor(PL_POSITION_RIGHT, PL_POSITION_OUTSIDE);
  opt = opt_base;
  x = 0.1;
  y = 0.;
  nrow = nlegend;
  ncolumn = 1;
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
	       0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
	       1., text_colors, text,
	       [], [], [], [],
	       line_colors, line_styles, line_widths,
	       symbol_colors, symbol_scales, symbol_numbers, symbols );

  position = bitor(PL_POSITION_LEFT, bitor(PL_POSITION_UPPER, PL_POSITION_INSIDE));
  opt = opt_base;
  x = 0.;
  y = 0.;
  nrow = 6;
  ncolumn = 2;
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
	       0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
	       1., text_colors, text,
	       [], [], [], [],
	       line_colors, line_styles, line_widths,
	       symbol_colors, symbol_scales, symbol_numbers, symbols );

  position = bitor(PL_POSITION_RIGHT, bitor(PL_POSITION_UPPER, PL_POSITION_INSIDE));
  opt = bitor(opt_base, PL_LEGEND_ROW_MAJOR);
  x = 0.;
  y = 0.;
  nrow = 6;
  ncolumn = 2;
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
	       0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
	       1., text_colors, text,
	       [], [], [], [],
	       line_colors, line_styles, line_widths,
	       symbol_colors, symbol_scales, symbol_numbers, symbols );

  position = bitor(PL_POSITION_LOWER, PL_POSITION_INSIDE);
  opt = bitor(opt_base, PL_LEGEND_ROW_MAJOR);
  x = 0.;
  y = 0.;
  nrow = 3;
  ncolumn = 3;
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
	       0.05, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 2.0,
	       1., text_colors, text,
	       [], [], [], [],
	       line_colors, line_styles, line_widths,
	       symbol_colors, symbol_scales, symbol_numbers, symbols );

  ## Third page demonstrating legend alignment
  pladv(0);
  plvpor(0., 1., 0., 1.);
  plwind(0.0, 1.0, 0.0, 1.0);
  plsfont(PL_FCI_SANS, -1, -1);
  plmtex("t", -2.0, 0.5, 0.5, "Demonstrate legend alignment");

  x = 0.1;
  y = 0.1;
  nturn = 4;
  nlegend = 0;
  position = bitor(PL_POSITION_LEFT, PL_POSITION_UPPER);
  opt_base = bitor(PL_LEGEND_BACKGROUND, PL_LEGEND_BOUNDING_BOX);
  opt = opt_base;
  for i=0:8
    ## Set up legend arrays with the correct size, type.  In octave this may
    ## be done by clearing the array (in case it was used before) and then
    ## writing the last element of the array with a placeholder of the right
    ## type.  Note that for strings the right placeholder length doesn't
    ## matter.  Octave keeps track of the longest string in each array and pads
    ## out all other members with blanks to that length if a subsequent string
    ## assignment for an array element exceeds that length.
    if( i <= nturn)
      nlegend += 1;
    else
      nlegend -= 1;
    endif
    nlegend = max(1, nlegend);

    opt_array = 0;
    text_colors = 0;
    text = " ";
    box_colors = 0;
    box_patterns = 0;
    box_scales = 0.;
    box_line_widths = 0;
    line_colors = 0;
    line_styles = 0;
    line_widths = 0;
    symbol_colors = 0;
    symbol_scales = 0.;
    symbol_numbers = 0;
    symbols = " ";

    opt_array(nlegend,1) = 0;
    text_colors(nlegend,1) = 0;
    text(nlegend,1:length(" ")) = " ";
    box_colors(nlegend,1) = 0;
    box_patterns(nlegend,1) = 0;
    box_scales(nlegend,1) = 0.;
    box_line_widths(nlegend,1) = 0;
    line_colors(nlegend,1) = 0;
    line_styles(nlegend,1) = 0;
    line_widths(nlegend,1) = 0;
    symbol_colors(nlegend,1) = 0;
    symbol_scales(nlegend,1) = 0.;
    symbol_numbers(nlegend,1) = 0;
    symbols(nlegend,1:length(" ")) = " ";

    ## Only specify legend data that are required according to the
    ## value of opt_array for that entry.
    for k=0:nlegend-1
      opt_array(k+1) = bitor(PL_LEGEND_LINE, PL_LEGEND_SYMBOL);
      line_styles(k+1) = 1;
      line_widths(k+1) = 1;
      symbol_scales(k+1) = 1.;
      symbol_numbers(k+1) = 2;
      symbols(k+1,1:length("*")) = "*";
      string = sprintf("%2.2d",k);
      text(k+1,1:length(string)) = string;
      text_colors(k+1) = 1 + mod(k,8);
      line_colors(k+1) = 1 + mod(k,8);
      symbol_colors(k+1) = 1 + mod(k,8);
    endfor

    ## Use monotype fonts so that all legends are the same size.
    plsfont(PL_FCI_MONO, -1, -1);
    plscol0a( 15, 32, 32, 32, 0.70 );

    nrow = min(3, nlegend);
    ncolumn = 0;

    [legend_width, legend_height] = \
        pllegend( position, opt, x, y,
                 0.025, 15, 1, 1, nrow, ncolumn, opt_array, 1.0, 1.0, 1.5,
                 1., text_colors, text,
		 [], [], [], [],
                 line_colors, line_styles, line_widths,
                 symbol_colors, symbol_scales, symbol_numbers, symbols );
    if(i == nturn)
      position = bitor(PL_POSITION_RIGHT, PL_POSITION_UPPER);
      opt = opt_base;
      x = 1. - x;
      y += legend_height;
    else
      x += legend_width;
      y += legend_height;
    endif
  endfor

  ## Fourth page illustrating various kinds of legends
  max_height = 0.;
  xstart = 0.0;
  ystart = 0.1;
  x = xstart;
  y = ystart;
  text_scale = 0.90;
  pladv(0);
  plvpor(0.0, 1., 0.0, 0.90);
  plwind(0.0, 1.0, 0.0, 1.0);
  ## plbox("bc", 0.0, 0, "bc", 0.0, 0);
  plsfont(PL_FCI_SANS, -1, -1);
  plmtex("t", 2.0, 0.5, 0.5, "Demonstrate Various Kinds of Legends");

  ## Set up legend arrays with the correct size, type.  In octave this may
  ## be done by clearing the array (in case it was used before) and then
  ## writing the last element of the array with a placeholder of the right
  ## type.  Note that for strings the right placeholder length doesn't
  ## matter.  Octave keeps track of the longest string in each array and pads
  ## out all other members with blanks to that length if a subsequent string
  ## assignment for an array element exceeds that length.
  nlegend = 5;

  opt_array = 0;
  text_colors = 0;
  text = " ";
  box_colors = 0;
  box_patterns = 0;
  box_scales = 0.;
  box_line_widths = 0;
  line_colors = 0;
  line_styles = 0;
  line_widths = 0;
  symbol_colors = 0;
  symbol_scales = 0.;
  symbol_numbers = 0;
  symbols = " ";

  opt_array(nlegend,1) = 0;
  text_colors(nlegend,1) = 0;
  text(nlegend,1:length(" ")) = " ";
  box_colors(nlegend,1) = 0;
  box_patterns(nlegend,1) = 0;
  box_scales(nlegend,1) = 0.;
  box_line_widths(nlegend,1) = 0;
  line_colors(nlegend,1) = 0;
  line_styles(nlegend,1) = 0;
  line_widths(nlegend,1) = 0;
  symbol_colors(nlegend,1) = 0;
  symbol_scales(nlegend,1) = 0.;
  symbol_numbers(nlegend,1) = 0;
  symbols(nlegend,1:length(" ")) = " ";

  ## Only specify legend data that are required according to the
  ## value of opt_array for that entry.
  position = bitor(PL_POSITION_LEFT, PL_POSITION_UPPER);
  opt_base = bitor(PL_LEGEND_BACKGROUND, bitor(PL_LEGEND_BOUNDING_BOX, PL_LEGEND_TEXT_LEFT));

  ## Set up None, Box, Line, Symbol, and Line & Symbol legend entries.
  opt_array(1) = PL_LEGEND_NONE;
  text(1,1:length("None")) = "None";
  text_colors(1) = 1;

  opt_array(2) = PL_LEGEND_COLOR_BOX;
  text(2,1:length("Box")) = "Box";
  text_colors(2) = 2;
  box_colors(2) = 2;
  box_patterns(2) = 0;
  box_scales(2) = 0.8;
  box_line_widths(2) = 1;

  opt_array(3) = PL_LEGEND_LINE;
  text(3,1:length("Line")) = "Line";
  text_colors(3) = 3;
  line_colors(3) = 3;
  line_styles(3) = 1;
  line_widths(3) = 1;

  opt_array(4) = PL_LEGEND_SYMBOL;
  text(4,1:length("Symbol")) = "Symbol";
  text_colors(4) = 4;
  symbol_colors(4) = 4;
  symbol_scales(4) = text_scale;
  symbol_numbers(4) = 4;
  symbols(4,1:length(special_symbols(3,:))) = special_symbols(3,:);

  opt_array(5) = bitor(PL_LEGEND_SYMBOL, PL_LEGEND_LINE);
  text(5,1:length("L & S")) = "L & S";
  text_colors(5) = 5;
  line_colors(5) = 5;
  line_styles(5) = 1;
  line_widths(5) = 1;
  symbol_colors(5) = 5;
  symbol_scales(5) = text_scale;
  symbol_numbers(5) = 4;
  symbols(5,1:length(special_symbols(3,:))) = special_symbols(3,:);

  opt = opt_base;
  plscol0a( 15, 32, 32, 32, 0.70 );
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
               0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
               0., text_colors, text,
               box_colors, box_patterns, box_scales, box_line_widths,
               line_colors, line_styles, line_widths,
               symbol_colors, symbol_scales, symbol_numbers, symbols );
  max_height = max(max_height, legend_height);

  ## Set up symbol legend entries with various symbols.
  ## Reset text and symbols, because otherwise legend may come out with
  ## the wrong length text/symbol with prior results embedded.
  text = " ";
  text(nlegend,1:length(" ")) = " ";
  symbols = " ";
  symbols(nlegend,1:length(" ")) = " ";

  for i=0:nlegend-1
    opt_array(i+1) = PL_LEGEND_SYMBOL;
    string = ["Symbol ",special_symbols(i+1,:)];
    text(i+1,1:length(string)) = string;
    text_colors(i+1) = i+1;
    symbol_colors(i+1) = i+1;
    symbol_scales(i+1) = text_scale;
    symbol_numbers(i+1) = 4;
    symbols(i+1,1:length(special_symbols(i+1,:))) = special_symbols(i+1,:);
  endfor

  opt = opt_base;
  x += legend_width;
  plscol0a( 15, 32, 32, 32, 0.70 );
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
	       0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
	       0., text_colors, text,
	       [], [], [], [],
	       [], [], [],
	       symbol_colors, symbol_scales, symbol_numbers, symbols );
  max_height = max(max_height, legend_height);

  ## Set up symbol legend entries with various numbers of symbols.
  ## Reset text and symbols, because otherwise legend may come out with
  ## the wrong length text/symbol with prior results embedded.
  text = " ";
  text(nlegend,1:length(" ")) = " ";
  symbols = " ";
  symbols(nlegend,1:length(" ")) = " ";

  for i=0:nlegend-1
    opt_array(i+1) = PL_LEGEND_SYMBOL;
    string = sprintf("Symbol Number %d", (i+2));
    text(i+1,1:length(string)) = string;
    text_colors(i+1) = i+1;
    symbol_colors(i+1) = i+1;
    symbol_scales(i+1) = text_scale;
    symbol_numbers(i+1) = i+2;
    symbols(i+1,1:length(special_symbols(3,:))) = special_symbols(3,:);
  endfor

  opt = opt_base;
  x += legend_width;
  plscol0a( 15, 32, 32, 32, 0.70 );
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
	       0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
	       0., text_colors, text,
	       [], [], [], [],
	       [], [], [],
	       symbol_colors, symbol_scales, symbol_numbers, symbols );
  max_height = max(max_height, legend_height);

  ## Set up box legend entries with various colours.
  ## Reset text and symbols, because otherwise legend may come out with
  ## the wrong length text/symbol with prior results embedded.
  text = " ";
  text(nlegend,1:length(" ")) = " ";
  symbols = " ";
  symbols(nlegend,1:length(" ")) = " ";

  for i=0:nlegend-1
    opt_array(i+1) = PL_LEGEND_COLOR_BOX;
    string = sprintf("%s %d", "Box Color",i+1);
    text(i+1,1:length(string)) = string;
    text_colors(i+1) = i+1;
    box_colors(i+1) = i+1;
    box_patterns(i+1) = 0;
    box_scales(i+1) = 0.8;
    box_line_widths(i+1) = 1;
  endfor

  opt = opt_base;
  ## Use new origin
  x = xstart;
  y += max_height;
  max_height = 0.;
  plscol0a( 15, 32, 32, 32, 0.70 );
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
               0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
               0., text_colors, text,
               box_colors, box_patterns, box_scales, box_line_widths,
	       [], [], [],
	       [], [], [], [] );
  max_height = max(max_height, legend_height);

  ## Set up box legend entries with various patterns.
  ## Reset text and symbols, because otherwise legend may come out with
  ## the wrong length text/symbol with prior results embedded.
  text = " ";
  text(nlegend,1:length(" ")) = " ";
  symbols = " ";
  symbols(nlegend,1:length(" ")) = " ";

  for i=0:nlegend-1
    opt_array(i+1) = PL_LEGEND_COLOR_BOX;
    string = sprintf("%s %d", "Box Pattern",i);
    text(i+1,1:length(string)) = string;
    text_colors(i+1) = 2;
    box_colors(i+1) = 2;
    box_patterns(i+1) = i;
    box_scales(i+1) = 0.8;
    box_line_widths(i+1) = 1;
  endfor

  opt = opt_base;
  x += legend_width;
  plscol0a( 15, 32, 32, 32, 0.70 );
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
               0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
               0., text_colors, text,
               box_colors, box_patterns, box_scales, box_line_widths,
	       [], [], [],
	       [], [], [], [] );
  max_height = max(max_height, legend_height);

  ## Set up box legend entries with various box pattern line widths.
  ## Reset text and symbols, because otherwise legend may come out with
  ## the wrong length text/symbol with prior results embedded.
  text = " ";
  text(nlegend,1:length(" ")) = " ";
  symbols = " ";
  symbols(nlegend,1:length(" ")) = " ";

  for i=0:nlegend-1
    opt_array(i+1) = PL_LEGEND_COLOR_BOX;
    string = sprintf("%s %d", "Box Line Width",i+1);
    text(i+1,1:length(string)) = string;
    text_colors(i+1) = 2;
    box_colors(i+1) = 2;
    box_patterns(i+1) = 3;
    box_scales(i+1) = 0.8;
    box_line_widths(i+1) = i+1;
  endfor

  opt = opt_base;
  x += legend_width;
  plscol0a( 15, 32, 32, 32, 0.70 );
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
               0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
               0., text_colors, text,
               box_colors, box_patterns, box_scales, box_line_widths,
	       [], [], [],
	       [], [], [], [] );
  max_height = max(max_height, legend_height);

  ## Set up line legend entries with various colours.
  ## From here on use the cell array of strings paradigm for
  ## text (and symbols) to show that the PLplot interface works properly for
  ## this case (and also to demonstrate how to use a cell array of strings
  ## with PLplot).
  ## Reset text and symbols cell arrays.
  text = {};
  text{nlegend} = "";
  symbols = {};
  symbols{nlegend} = "";

  for i=0:nlegend-1
    opt_array(i+1) = PL_LEGEND_LINE;
    text{i+1} = sprintf("%s %d", "Line Color",i+1);
    text_colors(i+1) = i+1;
    line_colors(i+1) = i+1;
    line_styles(i+1) = 1;
    line_widths(i+1) = 1;
  endfor

  opt = opt_base;
  ## Use new origin
  x = xstart;
  y += max_height;
  max_height = 0.;
  plscol0a( 15, 32, 32, 32, 0.70 );
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
               0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
               0., text_colors, text',
	       [], [], [], [], 
               line_colors, line_styles, line_widths,
	       [], [], [], [] );
  max_height = max(max_height, legend_height);

  ## Set up line legend entries with various styles
  ## Reset text and symbols cell arrays.
  text = {};
  text{nlegend} = "";
  symbols = {};
  symbols{nlegend} = "";

  for i=0:nlegend-1
    opt_array(i+1) = PL_LEGEND_LINE;
    text{i+1} = sprintf("%s %d", "Line Style",i+1);
    text_colors(i+1) = 2;
    line_colors(i+1) = 2;
    line_styles(i+1) = i+1;
    line_widths(i+1) = 1;
  endfor
  opt = opt_base;
  x += legend_width;
  plscol0a( 15, 32, 32, 32, 0.70 );
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
               0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
               0., text_colors, text',
	       [], [], [], [], 
               line_colors, line_styles, line_widths,
	       [], [], [], [] );
  max_height = max(max_height, legend_height);

  ## Set up line legend entries with various widths.
  ## Reset text and symbols cell arrays.
  text = {};
  text{nlegend} = "";
  symbols = {};
  symbols{nlegend} = "";

  for i=0:nlegend-1
    opt_array(i+1) = PL_LEGEND_LINE;
    text{i+1} = sprintf("%s %d", "Line Width",i+1);
    text_colors(i+1) = 2;
    line_colors(i+1) = 2;
    line_styles(i+1) = 1;
    line_widths(i+1) = i+1;
  endfor

  opt = opt_base;
  x += legend_width;
  plscol0a( 15, 32, 32, 32, 0.70 );
  [legend_width, legend_height] = \
      pllegend( position, opt, x, y,
               0.1, 15, 1, 1, 0, 0, opt_array, 1.0, text_scale, 2.0,
               0., text_colors, text',
	       [], [], [], [], 
               line_colors, line_styles, line_widths,
	       [], [], [], [] );
  max_height = max(max_height, legend_height);

  ## Color bar examples
  values_small = [ 0.0; 1.0 ];
  values_uneven = [ 0.0; 2.0; 2.6; 3.4; 6.0; 7.0; 8.0; 9.0; 10.0 ];
  values_even = [ 0.0; 1.0; 2.0; 3.0; 4.0; 5.0; 6.0; 7.0; 8.0 ];
  plcolorbar_example_1( PL_COLORBAR_IMAGE, 0.0, 0, values_small, "Image Color Bars" );
  plcolorbar_example_2( PL_COLORBAR_IMAGE, 0.0, 0, values_small, "Image Color Bars" )
  plcolorbar_example_1( bitor(PL_COLORBAR_SHADE, PL_COLORBAR_SHADE_LABEL), 0.0, 0, values_uneven, "Shade Color Bars - Uneven Steps" );
  plcolorbar_example_2( PL_COLORBAR_SHADE, 3.0, 3, values_even, "Shade Color Bars - Even Steps" );
  plcolorbar_example_1( PL_COLORBAR_GRADIENT, 0.5, 5, values_small, "Gradient Color Bars" );
  plcolorbar_example_2( PL_COLORBAR_GRADIENT, 0.5, 5, values_small, "Gradient Color Bars" );
  plend1();
endfunction

ix33c()
