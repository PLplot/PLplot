// -*- coding: utf-8; -*-
//
// $Id$
//
// Demonstrate most pllegend capability including unicode symbols.
//
// Copyright (C) 2010 Alan Irwin
// Copyright (C) 2011 Andrew Ross
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Library Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//

// This example designed just for devices (e.g., the cairo-related and
// qt-related devices) where the best choice of glyph is automatically
// selected by the related libraries (pango/cairo or Qt4) for each
// unicode character depending on what system fonts are installed.  Of
// course, you must have the appropriate TrueType fonts installed to
// have access to all the required glyphs.

#include "plc++demos.h"

#ifdef PL_USE_NAMESPACE
using namespace std;
#endif

class x33 {
public:
    x33( int, const char ** );

private:
    // Class data
    plstream           *pls;

    static const PLINT position_options[16];
    static const char  *special_symbols[5];

    void plcolorbar_example_1( PLINT, PLFLT, PLINT, PLINT, PLFLT *, const char * );
    void plcolorbar_example_2( PLINT, PLFLT, PLFLT, PLINT, PLFLT *, const char * );

    PLFLT MIN( PLFLT x, PLFLT y ) { return ( x < y ? x : y ); };
    PLFLT MAX( PLFLT x, PLFLT y ) { return ( x > y ? x : y ); };
};

const PLINT x33::position_options[16] = {
    PL_LEGEND_LEFT | PL_LEGEND_UPPER | PL_LEGEND_OUTSIDE,
    PL_LEGEND_UPPER | PL_LEGEND_OUTSIDE,
    PL_LEGEND_RIGHT | PL_LEGEND_UPPER | PL_LEGEND_OUTSIDE,
    PL_LEGEND_RIGHT | PL_LEGEND_OUTSIDE,
    PL_LEGEND_RIGHT | PL_LEGEND_LOWER | PL_LEGEND_OUTSIDE,
    PL_LEGEND_LOWER | PL_LEGEND_OUTSIDE,
    PL_LEGEND_LEFT | PL_LEGEND_LOWER | PL_LEGEND_OUTSIDE,
    PL_LEGEND_LEFT | PL_LEGEND_OUTSIDE,
    PL_LEGEND_LEFT | PL_LEGEND_UPPER | PL_LEGEND_INSIDE,
    PL_LEGEND_UPPER | PL_LEGEND_INSIDE,
    PL_LEGEND_RIGHT | PL_LEGEND_UPPER | PL_LEGEND_INSIDE,
    PL_LEGEND_RIGHT | PL_LEGEND_INSIDE,
    PL_LEGEND_RIGHT | PL_LEGEND_LOWER | PL_LEGEND_INSIDE,
    PL_LEGEND_LOWER | PL_LEGEND_INSIDE,
    PL_LEGEND_LEFT | PL_LEGEND_LOWER | PL_LEGEND_INSIDE,
    PL_LEGEND_LEFT | PL_LEGEND_INSIDE
};

// Pick 5 arbitrary UTF-8 symbols useful for plotting points (✠✚✱✪✽✺✰✴✦).
const char *x33::special_symbols[5] = {
    "✰",
    "✴",
    "✱",
    "✽",
    "✦"
};

void
x33::plcolorbar_example_1( PLINT bar_type, PLFLT ticks, PLINT sub_ticks, PLINT n, PLFLT *values, const char *title )
{
    pls->adv( 0 );
    // Setup color palette 1
    pls->spal1( "cmap1_blue_red.pal", 1 );

    PLFLT colors[n];
    int   i;
    PLFLT color_step;
    color_step = 1.0 / (PLFLT) ( n - 1 );
    for ( i = 0; i < n; i++ )
    {
        colors[i] = 0.0 + color_step * (PLFLT) ( i );
    }

    PLINT opt;
    opt = PL_COLORBAR_LEFT | bar_type | PL_COLORBAR_LABEL_LEFT |
          PL_COLORBAR_CAP_HIGH;

    const char *axis_opts_1, *axis_opts_2;
    if ( bar_type & PL_COLORBAR_SHADE_LABEL )
    {
        axis_opts_1 = "iv";
        axis_opts_2 = "i";
    }
    else
    {
        if ( sub_ticks != 0 )
        {
            axis_opts_1 = "stv";
            axis_opts_2 = "st";
        }
        else
        {
            axis_opts_1 = "tv";
            axis_opts_2 = "t";
        }
    }

    pls->colorbar( opt, 0.1, 0.1, 0.5, 0.1,
        ticks, sub_ticks,
        axis_opts_1, "Test label - Left, High Cap",
        n, colors, values );

    opt = PL_COLORBAR_RIGHT | bar_type | PL_COLORBAR_LABEL_RIGHT |
          PL_COLORBAR_CAP_LOW;

    pls->colorbar( opt, 0.1, 0.4, 0.5, 0.1,
        ticks, sub_ticks,
        axis_opts_1, "Test label - Right, Low Cap",
        n, colors, values );

    opt = PL_COLORBAR_UPPER | bar_type | PL_COLORBAR_LABEL_UPPER |
          PL_COLORBAR_CAP_HIGH;

    pls->colorbar( opt, 0.1, 0.1, 0.5, 0.1,
        ticks, sub_ticks,
        axis_opts_2, "Test label - Upper, High Cap",
        n, colors, values );

    opt = PL_COLORBAR_LOWER | bar_type | PL_COLORBAR_LABEL_LOWER |
          PL_COLORBAR_CAP_LOW;

    pls->colorbar( opt, 0.4, 0.1, 0.5, 0.1,
        ticks, sub_ticks,
        axis_opts_2, "Test label - Lower, Low Cap",
        n, colors, values );

    pls->vpor( 0.0, 1.0, 0.0, 1.0 );
    pls->wind( 0.0, 1.0, 0.0, 1.0 );
    pls->ptex( 0.5, 0.5, 0.0, 0.0, 0.5, title );
}

void
x33::plcolorbar_example_2( PLINT bar_type, PLFLT ticks, PLFLT sub_ticks, PLINT n, PLFLT *values, const char *title )
{
    pls->adv( 0 );
    // Setup color palette 1
    pls->spal1( "cmap1_blue_yellow.pal", 1 );

    PLFLT colors[n];
    int   i;
    PLFLT color_step;
    color_step = 1.0 / (PLFLT) ( n - 1 );
    for ( i = 0; i < n; i++ )
    {
        colors[i] = 0.0 + color_step * (PLFLT) ( i );
    }

    PLINT opt;
    opt = PL_COLORBAR_LEFT | bar_type | PL_COLORBAR_LABEL_LEFT |
          PL_COLORBAR_CAP_LOW;

    const char *axis_opts_1, *axis_opts_2;
    if ( bar_type == PL_COLORBAR_SHADE_LABEL )
    {
        axis_opts_1 = "";
        axis_opts_2 = "";
    }
    else
    {
        if ( sub_ticks != 0 )
        {
            axis_opts_1 = "stv";
            axis_opts_2 = "st";
        }
        else
        {
            axis_opts_1 = "tv";
            axis_opts_2 = "t";
        }
    }

    pls->colorbar( opt, 0.1, 0.1, 0.5, 0.1,
        ticks, sub_ticks,
        axis_opts_1, "Test label - Left, Low Cap",
        n, colors, values );

    opt = PL_COLORBAR_RIGHT | bar_type | PL_COLORBAR_LABEL_RIGHT |
          PL_COLORBAR_CAP_HIGH;

    pls->colorbar( opt, 0.1, 0.4, 0.5, 0.1,
        ticks, sub_ticks,
        axis_opts_1, "Test label - Right, High Cap",
        n, colors, values );

    opt = PL_COLORBAR_UPPER | bar_type | PL_COLORBAR_LABEL_UPPER |
          PL_COLORBAR_CAP_LOW;

    pls->colorbar( opt, 0.1, 0.1, 0.5, 0.1,
        ticks, sub_ticks,
        axis_opts_2, "Test label - Upper, Low Cap",
        n, colors, values );

    opt = PL_COLORBAR_LOWER | bar_type | PL_COLORBAR_LABEL_LOWER |
          PL_COLORBAR_CAP_HIGH;

    pls->colorbar( opt, 0.4, 0.1, 0.5, 0.1,
        ticks, sub_ticks,
        axis_opts_2, "Test label - Lower, High Cap",
        n, colors, values );

    pls->vpor( 0.0, 1.0, 0.0, 1.0 );
    pls->wind( 0.0, 1.0, 0.0, 1.0 );
    pls->ptex( 0.5, 0.5, 0.0, 0.0, 0.5, title );
}

//--------------------------------------------------------------------------
// x33
//
// Demonstrate most pllegend capability including unicode symbols.
//--------------------------------------------------------------------------

#define MAX_NLEGEND    7

x33::x33( int argc, const char **argv )
{
    int        i, k;
    PLINT      opt;
    PLINT      nlegend, nturn;
    PLINT      opt_array[MAX_NLEGEND];
    PLINT      text_colors[MAX_NLEGEND];
    PLINT      box_colors[MAX_NLEGEND];
    PLINT      box_patterns[MAX_NLEGEND];
    PLFLT      box_scales[MAX_NLEGEND];
    PLINT      box_line_widths[MAX_NLEGEND];
    PLINT      line_colors[MAX_NLEGEND];
    PLINT      line_styles[MAX_NLEGEND];
    PLINT      line_widths[MAX_NLEGEND];
    PLINT      symbol_numbers[MAX_NLEGEND], symbol_colors[MAX_NLEGEND];
    PLFLT      symbol_scales[MAX_NLEGEND];
    char       *text[MAX_NLEGEND];
    const char *symbols[MAX_NLEGEND];
    PLFLT      legend_width, legend_height, x, y, xstart, ystart;
    PLFLT      max_height, text_scale;
    PLINT      opt_base, nrow, ncolumn;

    pls = new plstream();

    // Create space to contain legend text.
    for ( k = 0; k < MAX_NLEGEND; k++ )
        text[k] = new char[200];

    // Parse and process command line arguments
    pls->parseopts( &argc, argv, PL_PARSE_FULL );

    // Initialize plplot
    pls->init();

    // First page illustrating the 16 standard positions.
    pls->adv( 0 );
    pls->vpor( 0.25, 0.75, 0.25, 0.75 );
    pls->wind( 0.0, 1.0, 0.0, 1.0 );
    pls->box( "bc", 0.0, 0, "bc", 0.0, 0 );
    pls->sfont( PL_FCI_SANS, -1, -1 );
    pls->mtex( "t", 8.0, 0.5, 0.5, "The 16 standard legend positions with" );
    pls->mtex( "t", 6.0, 0.5, 0.5, "the same (0.05) offset in x and y" );

    nlegend = 1;
    // Only specify legend data that are required according to the
    // value of opt_array for that entry.
    opt_base          = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX;
    opt_array[0]      = PL_LEGEND_LINE | PL_LEGEND_SYMBOL;
    line_styles[0]    = 1;
    line_widths[0]    = 1;
    symbol_scales[0]  = 1.;
    symbol_numbers[0] = 4;
    symbols[0]        = "*";

    // Use monotype fonts so that all legends are the same size.
    pls->sfont( PL_FCI_MONO, -1, -1 );
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    for ( k = 0; k < 16; k++ )
    {
        opt = opt_base | position_options[k];
        sprintf( text[0], "%2.2d", k );
        text_colors[0]   = 1 + ( k % 8 );
        line_colors[0]   = 1 + ( k % 8 );
        symbol_colors[0] = 1 + ( k % 8 );

        pls->legend( &legend_width, &legend_height, opt, 0.05, 0.05,
            0.1, 15, 1, 1, 0, 0,
            nlegend, opt_array, 1.0, 1.0, 2.0,
            1., text_colors, (const char **) text,
            NULL, NULL, NULL, NULL,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );
    }

    // Second page illustrating effect of nrow, ncolumn for the same legend
    // data.;
    pls->adv( 0 );
    pls->vpor( 0.25, 0.75, 0.25, 0.75 );
    pls->wind( 0.0, 1.0, 0.0, 1.0 );
    pls->box( "bc", 0.0, 0, "bc", 0.0, 0 );
    pls->sfont( PL_FCI_SANS, -1, -1 );
    pls->mtex( "t", 8.0, 0.5, 0.5, "The effect of nrow, ncolumn, PL_LEGEND_ROW_MAJOR," );
    pls->mtex( "t", 6.0, 0.5, 0.5, "and position for the same legend data" );

    nlegend = 7;

    // Only specify legend data that are required according to the
    // value of opt_array for that entry.
    opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX;
    for ( k = 0; k < nlegend; k++ )
    {
        opt_array[k]      = PL_LEGEND_LINE | PL_LEGEND_SYMBOL;
        line_styles[k]    = 1;
        line_widths[k]    = 1;
        symbol_scales[k]  = 1.;
        symbol_numbers[k] = 2;
        symbols[k]        = "*";
        sprintf( text[k], "%2.2d", k );
        text_colors[k]   = 1 + ( k % 8 );
        line_colors[k]   = 1 + ( k % 8 );
        symbol_colors[k] = 1 + ( k % 8 );
    }

    // Use monotype fonts so that all legends are the same size.
    pls->sfont( PL_FCI_MONO, -1, -1 );
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    opt     = opt_base | PL_LEGEND_UPPER | PL_LEGEND_OUTSIDE;
    x       = 0.;
    y       = 0.1;
    nrow    = 1;
    ncolumn = nlegend;
    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    opt     = opt_base | PL_LEGEND_LOWER | PL_LEGEND_OUTSIDE;
    x       = 0.;
    y       = 0.1;
    nrow    = 1;
    ncolumn = nlegend;
    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    opt     = opt_base | PL_LEGEND_LEFT | PL_LEGEND_OUTSIDE;
    x       = 0.1;
    y       = 0.;
    nrow    = nlegend;
    ncolumn = 1;
    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    opt     = opt_base | PL_LEGEND_RIGHT | PL_LEGEND_OUTSIDE;
    x       = 0.1;
    y       = 0.;
    nrow    = nlegend;
    ncolumn = 1;
    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    opt     = opt_base | PL_LEGEND_LEFT | PL_LEGEND_UPPER | PL_LEGEND_INSIDE;
    x       = 0.;
    y       = 0.;
    nrow    = 6;
    ncolumn = 2;
    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    opt     = opt_base | PL_LEGEND_RIGHT | PL_LEGEND_UPPER | PL_LEGEND_INSIDE | PL_LEGEND_ROW_MAJOR;
    x       = 0.;
    y       = 0.;
    nrow    = 6;
    ncolumn = 2;
    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    opt     = opt_base | PL_LEGEND_LOWER | PL_LEGEND_INSIDE | PL_LEGEND_ROW_MAJOR;;
    x       = 0.;
    y       = 0.;
    nrow    = 3;
    ncolumn = 3;
    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.05, 15, 1, 1, nrow, ncolumn,
        nlegend, opt_array, 1.0, 1.0, 2.0,
        1., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

    // Third page demonstrating legend alignment
    pls->adv( 0 );
    pls->vpor( 0., 1., 0., 1. );
    pls->wind( 0.0, 1.0, 0.0, 1.0 );
    pls->sfont( PL_FCI_SANS, -1, -1 );
    pls->mtex( "t", -2.0, 0.5, 0.5, "Demonstrate legend alignment" );

    x        = 0.1;
    y        = 0.1;
    nturn    = 4;
    nlegend  = 0;
    opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX | PL_LEGEND_UPPER;
    opt      = opt_base | PL_LEGEND_LEFT;
    for ( i = 0; i < 9; i++ )
    {
        // Set up legend arrays with the correct size, type.
        if ( i <= nturn )
            nlegend += 1;
        else
            nlegend -= 1;
        nlegend = MAX( 1, nlegend );
        // nly specify legend data that are required according to the
        //  value of opt_array for that entry.
        for ( k = 0; k < nlegend; k++ )
        {
            opt_array[k]      = PL_LEGEND_LINE | PL_LEGEND_SYMBOL;
            line_styles[k]    = 1;
            line_widths[k]    = 1;
            symbol_scales[k]  = 1.;
            symbol_numbers[k] = 2;
            symbols[k]        = "*";
            sprintf( text[k], "%2.2d", k );
            text_colors[k]   = 1 + ( k % 8 );
            line_colors[k]   = 1 + ( k % 8 );
            symbol_colors[k] = 1 + ( k % 8 );
        }
        // Use monotype fonts so that all legends are the same size.
        pls->sfont( PL_FCI_MONO, -1, -1 );
        pls->scol0a( 15, 32, 32, 32, 0.70 );

        nrow    = MIN( 3, nlegend );
        ncolumn = 0;

        pls->legend( &legend_width, &legend_height, opt, x, y,
            0.025, 15, 1, 1, nrow, ncolumn,
            nlegend, opt_array, 1.0, 1.0, 1.5,
            1., text_colors, (const char **) text,
            NULL, NULL, NULL, NULL,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );

        if ( i == nturn )
        {
            opt = opt_base | PL_LEGEND_RIGHT;
            x   = 1. - x;
            y  += legend_height;
        }
        else
        {
            x += legend_width;
            y += legend_height;
        }
    }

    // Fourth page illustrating various kinds of legends
    max_height = 0.;
    xstart     = 0.0;
    ystart     = 0.1;
    x          = xstart;
    y          = ystart;
    text_scale = 0.90;
    pls->adv( 0 );
    pls->vpor( 0.0, 1., 0.0, 0.90 );
    pls->wind( 0.0, 1.0, 0.0, 1.0 );
    //plbox("bc", 0.0, 0, "bc", 0.0, 0);
    pls->sfont( PL_FCI_SANS, -1, -1 );
    pls->mtex( "t", 2.0, 0.5, 0.5, "Demonstrate Various Kinds of Legends" );

    nlegend = 5;
    // Only specify legend data that are required according to the
    // value of opt_array for that entry.
    opt_base = PL_LEGEND_BACKGROUND | PL_LEGEND_BOUNDING_BOX | PL_LEGEND_LEFT | PL_LEGEND_UPPER | PL_LEGEND_TEXT_LEFT;

    // Set up None, Box, Line, Symbol, and Line & Symbol legend entries.
    opt_array[0] = PL_LEGEND_NONE;
    sprintf( text[0], "%s", "None" );
    text_colors[0] = 1;

    opt_array[1] = PL_LEGEND_COLOR_BOX;
    sprintf( text[1], "%s", "Box" );
    text_colors[1]     = 2;
    box_colors[1]      = 2;
    box_patterns[1]    = 0;
    box_scales[1]      = 0.8;
    box_line_widths[1] = 1;

    opt_array[2] = PL_LEGEND_LINE;
    sprintf( text[2], "%s", "Line" );
    text_colors[2] = 3;
    line_colors[2] = 3;
    line_styles[2] = 1;
    line_widths[2] = 1;

    opt_array[3] = PL_LEGEND_SYMBOL;
    sprintf( text[3], "%s", "Symbol" );
    text_colors[3]    = 4;
    symbol_colors[3]  = 4;
    symbol_scales[3]  = text_scale;
    symbol_numbers[3] = 4;
    symbols[3]        = special_symbols[2];

    opt_array[4] = PL_LEGEND_SYMBOL | PL_LEGEND_LINE;
    sprintf( text[4], "%s", "L & S" );
    text_colors[4]    = 5;
    line_colors[4]    = 5;
    line_styles[4]    = 1;
    line_widths[4]    = 1;
    symbol_colors[4]  = 5;
    symbol_scales[4]  = text_scale;
    symbol_numbers[4] = 4;
    symbols[4]        = special_symbols[2];

    opt = opt_base;
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        box_colors, box_patterns, box_scales, box_line_widths,
        line_colors, line_styles, line_widths,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );
    max_height = MAX( max_height, legend_height );

    // Set up symbol legend entries with various symbols.
    for ( i = 0; i < nlegend; i++ )
    {
        opt_array[i] = PL_LEGEND_SYMBOL;
        sprintf( text[i], "%s%s", "Symbol ", special_symbols[i] );
        text_colors[i]    = i + 1;
        symbol_colors[i]  = i + 1;
        symbol_scales[i]  = text_scale;
        symbol_numbers[i] = 4;
        symbols[i]        = special_symbols[i];
    }

    opt = opt_base;
    x  += legend_width;
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );
    max_height = MAX( max_height, legend_height );

    // Set up symbol legend entries with various numbers of symbols.
    for ( i = 0; i < nlegend; i++ )
    {
        opt_array[i] = PL_LEGEND_SYMBOL;
        sprintf( text[i], "%s %d", "Symbol Number", i + 2 );
        text_colors[i]    = i + 1;
        symbol_colors[i]  = i + 1;
        symbol_scales[i]  = text_scale;
        symbol_numbers[i] = i + 2;
        symbols[i]        = special_symbols[2];
    }

    opt = opt_base;
    x  += legend_width;
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL,
        symbol_colors, symbol_scales, symbol_numbers, (const char **) symbols );
    max_height = MAX( max_height, legend_height );

    // Set up box legend entries with various colours.
    for ( i = 0; i < nlegend; i++ )
    {
        opt_array[i] = PL_LEGEND_COLOR_BOX;
        sprintf( text[i], "%s %d", "Box Color", i + 1 );
        text_colors[i]     = i + 1;
        box_colors[i]      = i + 1;
        box_patterns[i]    = 0;
        box_scales[i]      = 0.8;
        box_line_widths[i] = 1;
    }

    opt = opt_base;
    // Use new origin
    x          = xstart;
    y         += max_height;
    max_height = 0.;
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        box_colors, box_patterns, box_scales, box_line_widths,
        NULL, NULL, NULL,
        NULL, NULL, NULL, NULL );
    max_height = MAX( max_height, legend_height );

    // Set up box legend entries with various patterns.
    for ( i = 0; i < nlegend; i++ )
    {
        opt_array[i] = PL_LEGEND_COLOR_BOX;
        sprintf( text[i], "%s %d", "Box Pattern", i );
        text_colors[i]     = 2;
        box_colors[i]      = 2;
        box_patterns[i]    = i;
        box_scales[i]      = 0.8;
        box_line_widths[i] = 1;
    }

    opt = opt_base;
    x  += legend_width;
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        box_colors, box_patterns, box_scales, box_line_widths,
        NULL, NULL, NULL,
        NULL, NULL, NULL, NULL );
    max_height = MAX( max_height, legend_height );

    // Set up box legend entries with various box pattern line widths.
    for ( i = 0; i < nlegend; i++ )
    {
        opt_array[i] = PL_LEGEND_COLOR_BOX;
        sprintf( text[i], "%s %d", "Box Line Width", i + 1 );
        text_colors[i]     = 2;
        box_colors[i]      = 2;
        box_patterns[i]    = 3;
        box_scales[i]      = 0.8;
        box_line_widths[i] = i + 1;
    }

    opt = opt_base;
    x  += legend_width;
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        box_colors, box_patterns, box_scales, box_line_widths,
        NULL, NULL, NULL,
        NULL, NULL, NULL, NULL );
    max_height = MAX( max_height, legend_height );

    // Set up line legend entries with various colours.
    for ( i = 0; i < nlegend; i++ )
    {
        opt_array[i] = PL_LEGEND_LINE;
        sprintf( text[i], "%s %d", "Line Color", i + 1 );
        text_colors[i] = i + 1;
        line_colors[i] = i + 1;
        line_styles[i] = 1;
        line_widths[i] = 1;
    }

    opt = opt_base;
    // Use new origin
    x          = xstart;
    y         += max_height;
    max_height = 0.;
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        NULL, NULL, NULL, NULL );
    max_height = MAX( max_height, legend_height );

    // Set up line legend entries with various styles.
    for ( i = 0; i < nlegend; i++ )
    {
        opt_array[i] = PL_LEGEND_LINE;
        sprintf( text[i], "%s %d", "Line Style", i + 1 );
        text_colors[i] = 2;
        line_colors[i] = 2;
        line_styles[i] = i + 1;
        line_widths[i] = 1;
    }

    opt = opt_base;
    x  += legend_width;
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        NULL, NULL, NULL, NULL );
    max_height = MAX( max_height, legend_height );

    // Set up line legend entries with various widths.
    for ( i = 0; i < nlegend; i++ )
    {
        opt_array[i] = PL_LEGEND_LINE;
        sprintf( text[i], "%s %d", "Line Width", i + 1 );
        text_colors[i] = 2;
        line_colors[i] = 2;
        line_styles[i] = 1;
        line_widths[i] = i + 1;
    }

    opt = opt_base;
    x  += legend_width;
    pls->scol0a( 15, 32, 32, 32, 0.70 );

    pls->legend( &legend_width, &legend_height, opt, x, y,
        0.1, 15, 1, 1, 0, 0,
        nlegend, opt_array, 1.0, text_scale, 2.0,
        0., text_colors, (const char **) text,
        NULL, NULL, NULL, NULL,
        line_colors, line_styles, line_widths,
        NULL, NULL, NULL, NULL );
    max_height = MAX( max_height, legend_height );

    // Color bar examples
    PLFLT values_small[2]  = { 0.0, 1.0 };
    PLFLT values_uneven[9] = { 0.0, 2.0, 2.6, 3.4, 6.0, 7.0, 8.0, 9.0, 10.0 };
    PLFLT values_even[9]   = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
    plcolorbar_example_1( PL_COLORBAR_IMAGE, 0.0, 0, 2, values_small, "Image Color Bars" );
    plcolorbar_example_2( PL_COLORBAR_IMAGE, 0.0, 0, 2, values_small, "Image Color Bars" );
    plcolorbar_example_1( PL_COLORBAR_SHADE | PL_COLORBAR_SHADE_LABEL, 0.0, 0, 9, values_uneven,
        "Shade Color Bars - Uneven Steps" );
    plcolorbar_example_2( PL_COLORBAR_SHADE, 3.0, 3, 9, values_even,
        "Shade Color Bars - Even Steps" );
    plcolorbar_example_1( PL_COLORBAR_GRADIENT, 0.5, 5, 2, values_small,
        "Gradient Color Bars" );
    plcolorbar_example_2( PL_COLORBAR_GRADIENT, 0.5, 5, 2, values_small,
        "Gradient Color Bars" );

    // Free space that contained legend text.
    for ( k = 0; k < MAX_NLEGEND; k++ )
        delete text[k];
    //plend();
    delete pls;
}

int main( int argc, const char ** argv )
{
    x33 *x = new x33( argc, argv );

    delete x;
}
