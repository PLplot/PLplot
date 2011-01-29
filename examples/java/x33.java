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

package plplot.examples;

import plplot.core.*;

import java.lang.Math;
import java.text.*;

class x33 {
    PLStream pls = new PLStream();

    public static void main( String[] args )
    {
        new x33( args );
    }


    static String[] special_symbols = {
        "✰",
        "✴",
        "✱",
        "✽",
        "✦"
    };

    void plcolorbar_example_1( int bar_type, double ticks, int sub_ticks, int n, double[] values, String title )
    {
        pls.adv( 0 );
        // Setup color palette 1
        pls.spal1( "cmap1_blue_red.pal", true );

        double[] colors = new double[n];
        int    i;
        double color_step;
        color_step = 1.0 / (double) ( n - 1 );
        for ( i = 0; i < n; i++ )
        {
            colors[i] = 0.0 + color_step * (double) ( i );
        }

        int opt;
        opt = PLStream.PL_POSITION_LEFT | bar_type | PLStream.PL_COLORBAR_LABEL_LEFT |
              PLStream.PL_COLORBAR_CAP_HIGH;

        String axis_opts_1, axis_opts_2;
        if ( ( bar_type & PLStream.PL_COLORBAR_SHADE_LABEL ) > 0 )
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

        pls.colorbar( opt, 0.1, 0.1, 0.5, 0.1,
            ticks, sub_ticks,
            axis_opts_1, "Test label - Left, High Cap",
            colors, values );

        opt = PLStream.PL_POSITION_RIGHT | bar_type | PLStream.PL_COLORBAR_LABEL_RIGHT |
              PLStream.PL_COLORBAR_CAP_LOW;

        pls.colorbar( opt, 0.1, 0.4, 0.5, 0.1,
            ticks, sub_ticks,
            axis_opts_1, "Test label - Right, Low Cap",
            colors, values );

        opt = PLStream.PL_POSITION_TOP | bar_type | PLStream.PL_COLORBAR_LABEL_TOP |
              PLStream.PL_COLORBAR_CAP_HIGH;

        pls.colorbar( opt, 0.1, 0.1, 0.5, 0.1,
            ticks, sub_ticks,
            axis_opts_2, "Test label - Upper, High Cap",
            colors, values );

        opt = PLStream.PL_POSITION_BOTTOM | bar_type | PLStream.PL_COLORBAR_LABEL_BOTTOM |
              PLStream.PL_COLORBAR_CAP_LOW;

        pls.colorbar( opt, 0.4, 0.1, 0.5, 0.1,
            ticks, sub_ticks,
            axis_opts_2, "Test label - Lower, Low Cap",
            colors, values );

        pls.vpor( 0.0, 1.0, 0.0, 1.0 );
        pls.wind( 0.0, 1.0, 0.0, 1.0 );
        pls.ptex( 0.5, 0.5, 0.0, 0.0, 0.5, title );
    }

    void plcolorbar_example_2( int bar_type, double ticks, double sub_ticks, int n, double[] values, String title )
    {
        pls.adv( 0 );
        // Setup color palette 1
        pls.spal1( "cmap1_blue_yellow.pal", true );

        double[] colors = new double[n];
        int    i;
        double color_step;
        color_step = 1.0 / (double) ( n - 1 );
        for ( i = 0; i < n; i++ )
        {
            colors[i] = 0.0 + color_step * (double) ( i );
        }

        int opt;
        opt = PLStream.PL_POSITION_LEFT | bar_type | PLStream.PL_COLORBAR_LABEL_LEFT |
              PLStream.PL_COLORBAR_CAP_LOW;

        String axis_opts_1, axis_opts_2;
        if ( bar_type == PLStream.PL_COLORBAR_SHADE_LABEL )
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

        pls.colorbar( opt, 0.1, 0.1, 0.5, 0.1,
            ticks, sub_ticks,
            axis_opts_1, "Test label - Left, Low Cap",
            colors, values );

        opt = PLStream.PL_POSITION_RIGHT | bar_type | PLStream.PL_COLORBAR_LABEL_RIGHT |
              PLStream.PL_COLORBAR_CAP_HIGH;

        pls.colorbar( opt, 0.1, 0.4, 0.5, 0.1,
            ticks, sub_ticks,
            axis_opts_1, "Test label - Right, High Cap",
            colors, values );

        opt = PLStream.PL_POSITION_TOP | bar_type | PLStream.PL_COLORBAR_LABEL_TOP |
              PLStream.PL_COLORBAR_CAP_LOW;

        pls.colorbar( opt, 0.1, 0.1, 0.5, 0.1,
            ticks, sub_ticks,
            axis_opts_2, "Test label - Upper, Low Cap",
            colors, values );

        opt = PLStream.PL_POSITION_BOTTOM | bar_type | PLStream.PL_COLORBAR_LABEL_BOTTOM |
              PLStream.PL_COLORBAR_CAP_HIGH;

        pls.colorbar( opt, 0.4, 0.1, 0.5, 0.1,
            ticks, sub_ticks,
            axis_opts_2, "Test label - Lower, High Cap",
            colors, values );

        pls.vpor( 0.0, 1.0, 0.0, 1.0 );
        pls.wind( 0.0, 1.0, 0.0, 1.0 );
        pls.ptex( 0.5, 0.5, 0.0, 0.0, 0.5, title );
    }

//--------------------------------------------------------------------------
// x33
//
// Demonstrate most pllegend capability including unicode symbols.
//--------------------------------------------------------------------------

    public x33( String[] args )
    {
        int i, k;
        int opt;
        int nlegend, nturn;
        int[]      opt_array;
        int[]      text_colors;
        int[]      box_colors;
        int[]      box_patterns;
        double[]      box_scales;
        int[]      box_line_widths;
        int[]      line_colors;
        int[]      line_styles;
        int[]      line_widths;
        int[]      symbol_numbers;
        int[]      symbol_colors;
        double[]      symbol_scales;
        String[] text;
        String[] symbols;
        double[]      legend_width = new double[1], legend_height = new double[1];
        double x, y, xstart, ystart;
        double max_height, text_scale;
        int    position, opt_base, nrow, ncolumn;
        int[] position_options = {
            PLStream.PL_POSITION_LEFT | PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_OUTSIDE,
            PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_OUTSIDE,
            PLStream.PL_POSITION_RIGHT | PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_OUTSIDE,
            PLStream.PL_POSITION_RIGHT | PLStream.PL_POSITION_OUTSIDE,
            PLStream.PL_POSITION_RIGHT | PLStream.PL_POSITION_BOTTOM | PLStream.PL_POSITION_OUTSIDE,
            PLStream.PL_POSITION_BOTTOM | PLStream.PL_POSITION_OUTSIDE,
            PLStream.PL_POSITION_LEFT | PLStream.PL_POSITION_BOTTOM | PLStream.PL_POSITION_OUTSIDE,
            PLStream.PL_POSITION_LEFT | PLStream.PL_POSITION_OUTSIDE,
            PLStream.PL_POSITION_LEFT | PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_INSIDE,
            PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_INSIDE,
            PLStream.PL_POSITION_RIGHT | PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_INSIDE,
            PLStream.PL_POSITION_RIGHT | PLStream.PL_POSITION_INSIDE,
            PLStream.PL_POSITION_RIGHT | PLStream.PL_POSITION_BOTTOM | PLStream.PL_POSITION_INSIDE,
            PLStream.PL_POSITION_BOTTOM | PLStream.PL_POSITION_INSIDE,
            PLStream.PL_POSITION_LEFT | PLStream.PL_POSITION_BOTTOM | PLStream.PL_POSITION_INSIDE,
            PLStream.PL_POSITION_LEFT | PLStream.PL_POSITION_INSIDE
        };

        NumberFormat nf = NumberFormat.getNumberInstance();
        nf.setMinimumIntegerDigits( 2 );

        // Parse and process command line arguments
        pls.parseopts( args, PLStream.PL_PARSE_FULL | PLStream.PL_PARSE_NOPROGRAM );

        // Initialize plplot
        pls.init();

        // First page illustrating the 16 standard positions.
        pls.adv( 0 );
        pls.vpor( 0.25, 0.75, 0.25, 0.75 );
        pls.wind( 0.0, 1.0, 0.0, 1.0 );
        pls.box( "bc", 0.0, 0, "bc", 0.0, 0 );
        pls.sfont( PLStream.PL_FCI_SANS, -1, -1 );
        pls.mtex( "t", 8.0, 0.5, 0.5, "The 16 standard legend positions with" );
        pls.mtex( "t", 6.0, 0.5, 0.5, "the same (0.05) offset in x and y" );

        nlegend        = 1;
        opt_array      = new int[nlegend];
        text_colors    = new int[nlegend];
        line_colors    = new int[nlegend];
        line_styles    = new int[nlegend];
        line_widths    = new int[nlegend];
        symbol_numbers = new int[nlegend];
        symbol_colors  = new int[nlegend];
        symbol_scales  = new double[nlegend];
        text           = new String[nlegend];
        symbols        = new String[nlegend];
        // Only specify legend data that are required according to the
        // value of opt_array for that entry.
        opt_base          = PLStream.PL_LEGEND_BACKGROUND | PLStream.PL_LEGEND_BOUNDING_BOX;
        opt_array[0]      = PLStream.PL_LEGEND_LINE | PLStream.PL_LEGEND_SYMBOL;
        line_styles[0]    = 1;
        line_widths[0]    = 1;
        symbol_scales[0]  = 1.;
        symbol_numbers[0] = 4;
        symbols[0]        = "*";

        // Use monotype fonts so that all legends are the same size.
        pls.sfont( PLStream.PL_FCI_MONO, -1, -1 );
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        for ( k = 0; k < 16; k++ )
        {
            position         = position_options[k];
            opt              = opt_base;
            text[0]          = nf.format( k );
            text_colors[0]   = 1 + ( k % 8 );
            line_colors[0]   = 1 + ( k % 8 );
            symbol_colors[0] = 1 + ( k % 8 );

            pls.legend( legend_width, legend_height, position, opt, 0.05, 0.05,
                0.1, 15, 1, 1, 0, 0,
                opt_array, 1.0, 1.0, 2.0,
                1., text_colors, text,
                null, null, null, null,
                line_colors, line_styles, line_widths,
                symbol_colors, symbol_scales, symbol_numbers, symbols );
        }

        // Second page illustrating effect of nrow, ncolumn for the same legend
        // data.;
        pls.adv( 0 );
        pls.vpor( 0.25, 0.75, 0.25, 0.75 );
        pls.wind( 0.0, 1.0, 0.0, 1.0 );
        pls.box( "bc", 0.0, 0, "bc", 0.0, 0 );
        pls.sfont( PLStream.PL_FCI_SANS, -1, -1 );
        pls.mtex( "t", 8.0, 0.5, 0.5, "The effect of nrow, ncolumn, PL_LEGEND_ROW_MAJOR," );
        pls.mtex( "t", 6.0, 0.5, 0.5, "and position for the same legend data" );

        nlegend         = 7;
        opt_array       = new int[nlegend];
        text_colors     = new int[nlegend];
        line_colors     = new int[nlegend];
        line_styles     = new int[nlegend];
        line_widths     = new int[nlegend];
        box_colors      = new int[nlegend];
        box_patterns    = new int[nlegend];
        box_scales      = new double[nlegend];
        box_line_widths = new int[nlegend];
        symbol_numbers  = new int[nlegend];
        symbol_colors   = new int[nlegend];
        symbol_scales   = new double[nlegend];
        text            = new String[nlegend];
        symbols         = new String[nlegend];

        // Only specify legend data that are required according to the
        // value of opt_array for that entry.
        opt_base = PLStream.PL_LEGEND_BACKGROUND | PLStream.PL_LEGEND_BOUNDING_BOX;
        for ( k = 0; k < nlegend; k++ )
        {
            opt_array[k]      = PLStream.PL_LEGEND_LINE | PLStream.PL_LEGEND_SYMBOL;
            line_styles[k]    = 1;
            line_widths[k]    = 1;
            symbol_scales[k]  = 1.;
            symbol_numbers[k] = 2;
            symbols[k]        = "*";
            text[k]           = nf.format( k );
            text_colors[k]    = 1 + ( k % 8 );
            line_colors[k]    = 1 + ( k % 8 );
            symbol_colors[k]  = 1 + ( k % 8 );
        }

        // Use monotype fonts so that all legends are the same size.
        pls.sfont( PLStream.PL_FCI_MONO, -1, -1 );
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        position = PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_OUTSIDE;
        opt      = opt_base;
        x        = 0.;
        y        = 0.1;
        nrow     = 1;
        ncolumn  = nlegend;
        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols );

        position = PLStream.PL_POSITION_BOTTOM | PLStream.PL_POSITION_OUTSIDE;
        opt      = opt_base;
        x        = 0.;
        y        = 0.1;
        nrow     = 1;
        ncolumn  = nlegend;
        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols );

        position = PLStream.PL_POSITION_LEFT | PLStream.PL_POSITION_OUTSIDE;
        opt      = opt_base;
        x        = 0.1;
        y        = 0.;
        nrow     = nlegend;
        ncolumn  = 1;
        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols );

        position = PLStream.PL_POSITION_RIGHT | PLStream.PL_POSITION_OUTSIDE;
        opt      = opt_base;
        x        = 0.1;
        y        = 0.;
        nrow     = nlegend;
        ncolumn  = 1;
        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols );

        position = PLStream.PL_POSITION_LEFT | PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_INSIDE;
        opt      = opt_base;
        x        = 0.;
        y        = 0.;
        nrow     = 6;
        ncolumn  = 2;
        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols );

        position = PLStream.PL_POSITION_RIGHT | PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_INSIDE;
        opt      = opt_base | PLStream.PL_LEGEND_ROW_MAJOR;
        x        = 0.;
        y        = 0.;
        nrow     = 6;
        ncolumn  = 2;
        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols );

        position = PLStream.PL_POSITION_BOTTOM | PLStream.PL_POSITION_INSIDE;
        opt      = opt_base | PLStream.PL_LEGEND_ROW_MAJOR;
        x        = 0.;
        y        = 0.;
        nrow     = 3;
        ncolumn  = 3;
        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.05, 15, 1, 1, nrow, ncolumn,
            opt_array, 1.0, 1.0, 2.0,
            1., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols );

        // Third page demonstrating legend alignment
        pls.adv( 0 );
        pls.vpor( 0., 1., 0., 1. );
        pls.wind( 0.0, 1.0, 0.0, 1.0 );
        pls.sfont( PLStream.PL_FCI_SANS, -1, -1 );
        pls.mtex( "t", -2.0, 0.5, 0.5, "Demonstrate legend alignment" );

        x        = 0.1;
        y        = 0.1;
        nturn    = 4;
        nlegend  = 0;
        opt_base = PLStream.PL_LEGEND_BACKGROUND | PLStream.PL_LEGEND_BOUNDING_BOX;
        position = PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_LEFT;
        opt      = opt_base;
        for ( i = 0; i < 9; i++ )
        {
            // Set up legend arrays with the correct size, type.
            if ( i <= nturn )
                nlegend = nlegend + 1;
            else
                nlegend = nlegend - 1;
            nlegend         = Math.max( 1, nlegend );
            opt_array       = new int[nlegend];
            text_colors     = new int[nlegend];
            line_colors     = new int[nlegend];
            line_styles     = new int[nlegend];
            line_widths     = new int[nlegend];
            box_colors      = new int[nlegend];
            box_patterns    = new int[nlegend];
            box_scales      = new double[nlegend];
            box_line_widths = new int[nlegend];
            symbol_numbers  = new int[nlegend];
            symbol_colors   = new int[nlegend];
            symbol_scales   = new double[nlegend];
            text            = new String[nlegend];
            symbols         = new String[nlegend];

            // Only specify legend data that are required according to the
            //  value of opt_array for that entry.
            for ( k = 0; k < nlegend; k++ )
            {
                opt_array[k]      = PLStream.PL_LEGEND_LINE | PLStream.PL_LEGEND_SYMBOL;
                line_styles[k]    = 1;
                line_widths[k]    = 1;
                symbol_scales[k]  = 1.;
                symbol_numbers[k] = 2;
                symbols[k]        = "*";
                text[k]           = nf.format( k );
                text_colors[k]    = 1 + ( k % 8 );
                line_colors[k]    = 1 + ( k % 8 );
                symbol_colors[k]  = 1 + ( k % 8 );
            }
            // Use monotype fonts so that all legends are the same size.
            pls.sfont( PLStream.PL_FCI_MONO, -1, -1 );
            pls.scol0a( 15, 32, 32, 32, 0.70 );

            nrow    = Math.min( 3, nlegend );
            ncolumn = 0;

            pls.legend( legend_width, legend_height, position, opt, x, y,
                0.025, 15, 1, 1, nrow, ncolumn,
                opt_array, 1.0, 1.0, 1.5,
                1., text_colors, text,
                null, null, null, null,
                line_colors, line_styles, line_widths,
                symbol_colors, symbol_scales, symbol_numbers, symbols );

            if ( i == nturn )
            {
                position = PLStream.PL_POSITION_TOP | PLStream.PL_POSITION_RIGHT;
                opt      = opt_base;
                x        = 1. - x;
                y        = y + legend_height[0];
            }
            else
            {
                x = x + legend_width[0];
                y = y + legend_height[0];
            }
        }

        // Fourth page illustrating various kinds of legends
        max_height = 0.;
        xstart     = 0.0;
        ystart     = 0.1;
        x          = xstart;
        y          = ystart;
        text_scale = 0.90;
        pls.adv( 0 );
        pls.vpor( 0.0, 1., 0.0, 0.90 );
        pls.wind( 0.0, 1.0, 0.0, 1.0 );
        //plbox("bc", 0.0, 0, "bc", 0.0, 0);
        pls.sfont( PLStream.PL_FCI_SANS, -1, -1 );
        pls.mtex( "t", 2.0, 0.5, 0.5, "Demonstrate Various Kinds of Legends" );

        nlegend         = 5;
        opt_array       = new int[nlegend];
        text_colors     = new int[nlegend];
        line_colors     = new int[nlegend];
        line_styles     = new int[nlegend];
        line_widths     = new int[nlegend];
        box_colors      = new int[nlegend];
        box_patterns    = new int[nlegend];
        box_scales      = new double[nlegend];
        box_line_widths = new int[nlegend];
        symbol_numbers  = new int[nlegend];
        symbol_colors   = new int[nlegend];
        symbol_scales   = new double[nlegend];
        text            = new String[nlegend];
        symbols         = new String[nlegend];
        // In Java symbols array needs setting to null strings first, since
        // even unused values are copied over in the bindings.
        for ( k = 0; k < nlegend; k++ )
            symbols[k] = "";

        // Only specify legend data that are required according to the
        // value of opt_array for that entry.
        position = PLStream.PL_POSITION_LEFT | PLStream.PL_POSITION_TOP;
        opt_base = PLStream.PL_LEGEND_BACKGROUND | PLStream.PL_LEGEND_BOUNDING_BOX | PLStream.PL_LEGEND_TEXT_LEFT;

        // Set up None, Box, Line, Symbol, and Line & Symbol legend entries.
        opt_array[0]   = PLStream.PL_LEGEND_NONE;
        text[0]        = "None";
        text_colors[0] = 1;

        opt_array[1]       = PLStream.PL_LEGEND_COLOR_BOX;
        text[1]            = "Box";
        text_colors[1]     = 2;
        box_colors[1]      = 2;
        box_patterns[1]    = 0;
        box_scales[1]      = 0.8;
        box_line_widths[1] = 1;

        opt_array[2]   = PLStream.PL_LEGEND_LINE;
        text[2]        = "Line";
        text_colors[2] = 3;
        line_colors[2] = 3;
        line_styles[2] = 1;
        line_widths[2] = 1;

        opt_array[3]      = PLStream.PL_LEGEND_SYMBOL;
        text[3]           = "Symbol";
        text_colors[3]    = 4;
        symbol_colors[3]  = 4;
        symbol_scales[3]  = text_scale;
        symbol_numbers[3] = 4;
        symbols[3]        = special_symbols[2];

        opt_array[4]      = PLStream.PL_LEGEND_SYMBOL | PLStream.PL_LEGEND_LINE;
        text[4]           = "L & S";
        text_colors[4]    = 5;
        line_colors[4]    = 5;
        line_styles[4]    = 1;
        line_widths[4]    = 1;
        symbol_colors[4]  = 5;
        symbol_scales[4]  = text_scale;
        symbol_numbers[4] = 4;
        symbols[4]        = special_symbols[2];

        opt = opt_base;
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0., text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            line_colors, line_styles, line_widths,
            symbol_colors, symbol_scales, symbol_numbers, symbols );
        max_height = Math.max( max_height, legend_height[0] );

        // Set up symbol legend entries with various symbols.
        for ( i = 0; i < nlegend; i++ )
        {
            opt_array[i]      = PLStream.PL_LEGEND_SYMBOL;
            text[i]           = "Symbol " + special_symbols[i];
            text_colors[i]    = i + 1;
            symbol_colors[i]  = i + 1;
            symbol_scales[i]  = text_scale;
            symbol_numbers[i] = 4;
            symbols[i]        = special_symbols[i];
        }

        opt = opt_base;
        x   = x + legend_width[0];
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0., text_colors, text,
            null, null, null, null,
            null, null, null,
            symbol_colors, symbol_scales, symbol_numbers, symbols );
        max_height = Math.max( max_height, legend_height[0] );

        // Set up symbol legend entries with various numbers of symbols.
        for ( i = 0; i < nlegend; i++ )
        {
            opt_array[i]      = PLStream.PL_LEGEND_SYMBOL;
            text[i]           = "Symbol Number " + ( i + 2 );
            text_colors[i]    = i + 1;
            symbol_colors[i]  = i + 1;
            symbol_scales[i]  = text_scale;
            symbol_numbers[i] = i + 2;
            symbols[i]        = special_symbols[2];
        }

        opt = opt_base;
        x   = x + legend_width[0];
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0., text_colors, text,
            null, null, null, null,
            null, null, null,
            symbol_colors, symbol_scales, symbol_numbers, symbols );
        max_height = Math.max( max_height, legend_height[0] );

        // Set up box legend entries with various colours.
        for ( i = 0; i < nlegend; i++ )
        {
            opt_array[i]       = PLStream.PL_LEGEND_COLOR_BOX;
            text[i]            = "Box Color " + ( i + 1 );
            text_colors[i]     = i + 1;
            box_colors[i]      = i + 1;
            box_patterns[i]    = 0;
            box_scales[i]      = 0.8;
            box_line_widths[i] = 1;
        }

        opt = opt_base;
        // Use new origin
        x          = xstart;
        y          = y + max_height;
        max_height = 0.;
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0., text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            null, null, null,
            null, null, null, null );
        max_height = Math.max( max_height, legend_height[0] );

        // Set up box legend entries with various patterns.
        for ( i = 0; i < nlegend; i++ )
        {
            opt_array[i]       = PLStream.PL_LEGEND_COLOR_BOX;
            text[i]            = "Box Pattern " + i;
            text_colors[i]     = 2;
            box_colors[i]      = 2;
            box_patterns[i]    = i;
            box_scales[i]      = 0.8;
            box_line_widths[i] = 1;
        }

        opt = opt_base;
        x   = x + legend_width[0];
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0., text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            null, null, null,
            null, null, null, null );
        max_height = Math.max( max_height, legend_height[0] );

        // Set up box legend entries with various box pattern line widths.
        for ( i = 0; i < nlegend; i++ )
        {
            opt_array[i]       = PLStream.PL_LEGEND_COLOR_BOX;
            text[i]            = "Box Line Width " + ( i + 1 );
            text_colors[i]     = 2;
            box_colors[i]      = 2;
            box_patterns[i]    = 3;
            box_scales[i]      = 0.8;
            box_line_widths[i] = i + 1;
        }

        opt = opt_base;
        x   = x + legend_width[0];
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0., text_colors, text,
            box_colors, box_patterns, box_scales, box_line_widths,
            null, null, null,
            null, null, null, null );
        max_height = Math.max( max_height, legend_height[0] );

        // Set up line legend entries with various colours.
        for ( i = 0; i < nlegend; i++ )
        {
            opt_array[i]   = PLStream.PL_LEGEND_LINE;
            text[i]        = "Line Color " + ( i + 1 );
            text_colors[i] = i + 1;
            line_colors[i] = i + 1;
            line_styles[i] = 1;
            line_widths[i] = 1;
        }

        opt = opt_base;
        // Use new origin
        x          = xstart;
        y          = y + max_height;
        max_height = 0.;
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            null, null, null, null );
        max_height = Math.max( max_height, legend_height[0] );

        // Set up line legend entries with various styles.
        for ( i = 0; i < nlegend; i++ )
        {
            opt_array[i]   = PLStream.PL_LEGEND_LINE;
            text[i]        = "Line Style " + ( i + 1 );
            text_colors[i] = 2;
            line_colors[i] = 2;
            line_styles[i] = i + 1;
            line_widths[i] = 1;
        }

        opt = opt_base;
        x   = x + legend_width[0];
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            null, null, null, null );
        max_height = Math.max( max_height, legend_height[0] );

        // Set up line legend entries with various widths.
        for ( i = 0; i < nlegend; i++ )
        {
            opt_array[i]   = PLStream.PL_LEGEND_LINE;
            text[i]        = "Line Width " + ( i + 1 );
            text_colors[i] = 2;
            line_colors[i] = 2;
            line_styles[i] = 1;
            line_widths[i] = i + 1;
        }

        opt = opt_base;
        x   = x + legend_width[0];
        pls.scol0a( 15, 32, 32, 32, 0.70 );

        pls.legend( legend_width, legend_height, position, opt, x, y,
            0.1, 15, 1, 1, 0, 0,
            opt_array, 1.0, text_scale, 2.0,
            0., text_colors, text,
            null, null, null, null,
            line_colors, line_styles, line_widths,
            null, null, null, null );
        max_height = Math.max( max_height, legend_height[0] );

        // Color bar examples
        double[] values_small  = { 0.0, 1.0 };
        double[] values_uneven = { 0.0, 2.0, 2.6, 3.4, 6.0, 7.0, 8.0, 9.0, 10.0 };
        double[] values_even   = { 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0 };
        plcolorbar_example_1( PLStream.PL_COLORBAR_IMAGE, 0.0, 0, 2, values_small, "Image Color Bars" );
        plcolorbar_example_2( PLStream.PL_COLORBAR_IMAGE, 0.0, 0, 2, values_small, "Image Color Bars" );
        plcolorbar_example_1( PLStream.PL_COLORBAR_SHADE | PLStream.PL_COLORBAR_SHADE_LABEL, 0.0, 0, 9, values_uneven,
            "Shade Color Bars - Uneven Steps" );
        plcolorbar_example_2( PLStream.PL_COLORBAR_SHADE, 3.0, 3, 9, values_even,
            "Shade Color Bars - Even Steps" );
        plcolorbar_example_1( PLStream.PL_COLORBAR_GRADIENT, 0.5, 5, 2, values_small,
            "Gradient Color Bars" );
        plcolorbar_example_2( PLStream.PL_COLORBAR_GRADIENT, 0.5, 5, 2, values_small,
            "Gradient Color Bars" );

        pls.end();
    }
}
