// pllegend()
//
// Copyright (C) 2010  Hezekiah M. Carty
// Copyright (C) 2010  Alan W. Irwin
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

//! @file
//!
//! Legend routines.
//!
#include "plplotP.h"

// xorigin is the X coordinate of the viewport reference point.
// yorigin is the Y coordinate of the viewport reference point.
// xlegend is the X coordinate of the top-left of the legend box
// relative to the legend box reference point.
// ylegend is the y coordinate of the top-left of the legend box
// relative to the legend box reference point.
// N.B. xorigin, yorigin, xlegend, and ylegend are all expressed
// in normalized external viewport coordinates to be consistent
// with the coordinate system used for x and y.

static void legend_position( PLINT opt, PLFLT legend_width, PLFLT legend_height,
                             PLFLT *x_legend_position, PLFLT *y_legend_position )
{
    PLFLT xorigin, yorigin, xlegend, ylegend;
    if ( opt & PL_LEGEND_RIGHT )
    {
        xorigin = 1.;
        if ( opt & PL_LEGEND_UPPER )
        {
            yorigin = 1.;
            if ( opt & PL_LEGEND_INSIDE )
            {
                xlegend = -legend_width;
                ylegend = 0.;
            }
            else if ( opt & PL_LEGEND_OUTSIDE )
            {
                xlegend = 0.;
                ylegend = legend_height;
            }
            else
            {
                plexit( "legend_position: internal logic error 1" );
            }
        }
        else if ( !( opt & PL_LEGEND_UPPER ) && !( opt & PL_LEGEND_LOWER ) )
        {
            yorigin = 0.5;
            ylegend = 0.5 * legend_height;
            if ( opt & PL_LEGEND_INSIDE )
            {
                xlegend = -legend_width;
            }
            else if ( opt & PL_LEGEND_OUTSIDE )
            {
                xlegend = 0.;
            }
            else
            {
                plexit( "legend_position: internal logic error 2" );
            }
        }
        else if ( opt & PL_LEGEND_LOWER )
        {
            yorigin = 0.;
            if ( opt & PL_LEGEND_INSIDE )
            {
                xlegend = -legend_width;
                ylegend = legend_height;
            }
            else if ( opt & PL_LEGEND_OUTSIDE )
            {
                xlegend = 0.;
                ylegend = 0.;
            }
            else
            {
                plexit( "legend_position: internal logic error 3" );
            }
        }
        else
        {
            plexit( "legend_position: internal logic error 4" );
        }
    }
    else if ( !( opt & PL_LEGEND_RIGHT ) && !( opt & PL_LEGEND_LEFT ) )
    {
        xorigin = 0.5;
        xlegend = -0.5 * legend_width;
        if ( opt & PL_LEGEND_UPPER )
        {
            yorigin = 1.;
            if ( opt & PL_LEGEND_INSIDE )
            {
                ylegend = 0.;
            }
            else if ( opt & PL_LEGEND_OUTSIDE )
            {
                ylegend = legend_height;
            }
            else
            {
                plexit( "legend_position: internal logic error 5" );
            }
        }
        else if ( opt & PL_LEGEND_LOWER )
        {
            yorigin = 0.;
            if ( opt & PL_LEGEND_INSIDE )
            {
                ylegend = legend_height;
            }
            else if ( opt & PL_LEGEND_OUTSIDE )
            {
                ylegend = 0.;
            }
            else
            {
                plexit( "legend_position: internal logic error 6" );
            }
        }
        else
        {
            plexit( "legend_position: internal logic error 7" );
        }
    }
    else if ( opt & PL_LEGEND_LEFT )
    {
        xorigin = 0.;
        if ( opt & PL_LEGEND_UPPER )
        {
            yorigin = 1.;
            if ( opt & PL_LEGEND_INSIDE )
            {
                xlegend = 0.;
                ylegend = 0.;
            }
            else if ( opt & PL_LEGEND_OUTSIDE )
            {
                xlegend = -legend_width;
                ylegend = legend_height;
            }
            else
            {
                plexit( "legend_position: internal logic error 8" );
            }
        }
        else if ( !( opt & PL_LEGEND_UPPER ) && !( opt & PL_LEGEND_LOWER ) )
        {
            yorigin = 0.5;
            ylegend = 0.5 * legend_height;
            if ( opt & PL_LEGEND_INSIDE )
            {
                xlegend = 0.;
            }
            else if ( opt & PL_LEGEND_OUTSIDE )
            {
                xlegend = -legend_width;
            }
            else
            {
                plexit( "legend_position: internal logic error 9" );
            }
        }
        else if ( opt & PL_LEGEND_LOWER )
        {
            yorigin = 0.;
            if ( opt & PL_LEGEND_INSIDE )
            {
                ylegend = legend_height;
                xlegend = 0.;
            }
            else if ( opt & PL_LEGEND_OUTSIDE )
            {
                xlegend = -legend_width;
                ylegend = 0.;
            }
            else
            {
                plexit( "legend_position: internal logic error 10" );
            }
        }
        else
        {
            plexit( "legend_position: internal logic error 11" );
        }
    }
    else
    {
        plexit( "legend_position: internal logic error 12" );
    }
    *x_legend_position = xorigin + xlegend;
    *y_legend_position = yorigin + ylegend;
}

//--------------------------------------------------------------------------
//! Obtain ratio of normalized subpage to mm coordinates in both x and y.
//!
//! @param x_subpage_per_mm : pointer to PLFLT containing x ratio after call
//! @param y_subpage_per_mm : pointer to PLFLT containing y ratio after call
//!

static void get_subpage_per_mm( PLFLT *x_subpage_per_mm, PLFLT *y_subpage_per_mm )
{
    // Normalized viewport limits
    PLFLT vxmin, vxmax, vymin, vymax;
    // Size of subpage in mm
    PLFLT mxmin, mxmax, mymin, mymax;
    // Viewport limits in world coordinates
    PLFLT wxmin, wxmax, wymin, wymax;
    plgvpd( &vxmin, &vxmax, &vymin, &vymax );
    plgspa( &mxmin, &mxmax, &mymin, &mymax );
    plgvpw( &wxmin, &wxmax, &wymin, &wymax );
    *x_subpage_per_mm = ( wxmax - wxmin ) / ( ( vxmax - vxmin ) * ( mxmax - mxmin ) );
    *y_subpage_per_mm = ( wymax - wymin ) / ( ( vymax - vymin ) * ( mymax - mymin ) );
}

//--------------------------------------------------------------------------
//! Obtain character or symbol height in (y) subpage coordinates.
//!
//! @param ifcharacter : TRUE obtain character height, FALSE obtain symbol
//! height
//! @return character or symbol height
//!

static PLFLT get_character_or_symbol_height( PLBOOL ifcharacter )
{
    // Character height in mm
    PLFLT default_mm, char_height_mm;
    PLFLT x_subpage_per_mm, y_subpage_per_mm;

    if ( ifcharacter )
    {
        plgchr( &default_mm, &char_height_mm );
    }
    else
    {
        default_mm     = plsc->symdef;
        char_height_mm = plsc->symht;
    }
    get_subpage_per_mm( &x_subpage_per_mm, &y_subpage_per_mm );
    return ( char_height_mm * y_subpage_per_mm );
}

//--------------------------------------------------------------------------
//! Convert from external normalized viewport X coordinate to normalized
//! subpage X coordinate.
//!
//! @param nx : external normalized viewport X coordinate
//!

#define viewport_to_subpage_x( nx )    ( ( xdmin_save ) + ( nx ) * ( ( xdmax_save ) - ( xdmin_save ) ) )

//--------------------------------------------------------------------------
//! Convert from normalized subpage X coordinate to external normalized
//! viewport X coordinate.
//!
//! @param nx : normalized subpage X coordinate
//!

#define subpage_to_viewport_x( nx )    ( ( nx - xdmin_save ) / ( ( xdmax_save ) - ( xdmin_save ) ) )

//--------------------------------------------------------------------------
//! Convert from external normalized viewport Y coordinate to normalized
//! subpage Y coordinate.
//!
//! @param ny : external normalized viewport Y coordinate
//!

#define viewport_to_subpage_y( ny )    ( ( ydmin_save ) + ( ny ) * ( ( ydmax_save ) - ( ydmin_save ) ) )

//--------------------------------------------------------------------------
//! Convert from normalized subpage Y coordinate to external normalized
//! viewport Y coordinate.
//!
//! @param ny : normalized subpage Y coordinate
//!

#define subpage_to_viewport_y( ny )    ( ( ny - ydmin_save ) / ( ( ydmax_save ) - ( ydmin_save ) ) )

//--------------------------------------------------------------------------
//! Plot discrete legend using lines, symbols, cmap0 colors, and/or
//! cmap1 colors.
//!
//! @param opt : options controlling overall legend.  If the
//! PL_LEGEND_BACKGROUND bit is set, plot a (semi-transparent)
//! background for the legend.  If the PL_LEGEND_TEXT_LEFT bit is set,
//! put the text area on the left of the legend and the plotted area
//! on the right.  Otherwise, put the text area on the right of the
//! legend and the plotted area on the left
//! @param x : normalized viewport X position of the upper-left corner of the
//! legend
//! @param y : normalized viewport Y position of the upper-left corner of the
//! legend
//! @param plot_width : width in normalized viewport units of the plot
//! area (where lines, symbols, and/or colored boxes are drawn in the
//! legend)
//! @param bg_color : cmap0 index of the background color for the legend
//! (PL_LEGEND_BACKGROUND)
//! @param nlegend : number of legend entries
//! @param opt_array : array of nlegend values of options to control
//! each individual plotted area corresponding to a legend entry.  If
//! the PL_LEGEND_NONE bit is set, then nothing is plotted in the
//! plotted area.  If the PL_LEGEND_COLOR_BOX, PL_LEGEND_LINE, and/or
//! PL_LEGEND_SYMBOL bits are set, the plotted area corresponding to a
//! legend entry is specified with a colored box; a line; and/or a
//! line of symbols
//! @param text_offset : offset of the text area from the plot area in
//! units of character width
//! @param text_scale : character height scale for text annotations
//! @param text_spacing : vertical spacing in units of the character
//! height from one legend entry to the next
//! @param text_justification : justification parameter used for text
//! justification.  The most common values of text_justification are
//! 0., 0.5, or 1. corresponding to a text that is left justified,
//! centred, or right justified within the text area, but other values
//! are allowed as well.
//! @param text_colors : array of nlegend text colors (cmap0 indices).
//! @param text : array of nlegend text annotations
//! @param box_colors : array of nlegend colors (cmap0 indices) for
//! the discrete colored boxes (PL_LEGEND_COLOR_BOX)
//! @param box_patterns : array of nlegend patterns (plpsty indices)
//! for the discrete colored boxes (PL_LEGEND_COLOR_BOX)
//! @param box_scales : array of nlegend scales (units of fraction of
//! character height) for the height of the discrete colored boxes
//! (PL_LEGEND_COLOR_BOX)
//! @param line_colors : array of nlegend line colors (cmap0 indices)
//! (PL_LEGEND_LINE)
//! @param line_styles : array of nlegend line styles (plsty indices)
//! (PL_LEGEND_LINE)
//! @param line_widths : array of nlegend line widths (PL_LEGEND_LINE)
//! @param symbol_colors : array of nlegend symbol colors (cmap0
//! indices) (PL_LEGEND_SYMBOL)
//! @param symbol_scales : array of nlegend scale values for the
//! symbol height (PL_LEGEND_SYMBOL)
//! @param symbol_numbers : array of nlegend numbers of symbols to be
//! drawn across the width of the plotted area (PL_LEGEND_SYMBOL)
//! @param symbols : array of nlegend symbols (plpoin indices)
//! (PL_LEGEND_SYMBOL)
//!
//! N.B. the total width of the legend is made up of plplot_width +
//! text_offset (converted to normalized subpage coordinates) + width
//! of the longest text string.  The latter quantity is calculated
//! internally and converted to normalized subpage coordinates.  The
//! total height of the legend is nlegend * text_spacing * character
//! height, where the latter quantity is calculated internally and
//! converted to normalized subpage coordinates.  The legend is clipped
//! at the edges of the current subpage.
//!

void
c_pllegend( PLINT opt, PLFLT x, PLFLT y, PLFLT plot_width,
            PLINT bg_color, PLINT bb_color, PLINT bb_style,
            PLINT nrow, PLINT ncolumn,
            PLINT nlegend, const PLINT *opt_array,
            PLFLT text_offset, PLFLT text_scale, PLFLT text_spacing,
            PLFLT text_justification,
            const PLINT *text_colors, const char **text,
            const PLINT *box_colors, const PLINT *box_patterns,
            const PLFLT *box_scales,
            const PLINT *line_colors, const PLINT *line_styles,
            const PLINT *line_widths,
            const PLINT *symbol_colors, const PLFLT *symbol_scales,
            const PLINT *symbol_numbers, const PLINT *symbols )

{
    // Legend position
    PLFLT plot_x, plot_x_end, plot_x_subpage, plot_x_end_subpage;
    PLFLT plot_y, plot_y_subpage;
    PLFLT text_x, text_y, text_x_subpage, text_y_subpage;
    // Character height (normalized subpage coordinates)
    PLFLT character_height, character_width, symbol_width;
    // x, y-position of the current legend entry
    PLFLT ty, xshift, drow, dcolumn;
    // Positions of the legend entries
    PLFLT dxs, *xs, *ys, xl[2], yl[2], xbox[4], ybox[4];
    PLINT i, j;
    // Active attributes to be saved and restored afterward.
    PLINT col0_save         = plsc->icol0,
          line_style_save   = plsc->line_style,
          line_width_save   = plsc->width,
          pattern_save      = plsc->patt;
    PLFLT text_scale_save   = plsc->chrht / plsc->chrdef,
          symbol_scale_save = plsc->symht / plsc->symdef;
    // Saved external world coordinates of viewport.
    PLFLT xwmin_save, xwmax_save, ywmin_save, ywmax_save;
    // Saved external normalized coordinates of viewport.
    PLFLT xdmin_save, xdmax_save, ydmin_save, ydmax_save;

    PLFLT x_subpage_per_mm, y_subpage_per_mm, text_width0 = 0., text_width;
    PLFLT width_border, column_separation,
          total_width, total_height, total_width_vc, total_height_vc;
    PLFLT x_legend_position, y_legend_position;

    PLINT some_boxes         = 0, some_lines = 0, some_symbols = 0;
    PLINT max_symbol_numbers = 0;
    PLINT irow = 0, icolumn = 0;

    // Default nrow, ncolumn.
    nrow    = MAX( nrow, 1 );
    ncolumn = MAX( ncolumn, 1 );
    if ( nrow * ncolumn < nlegend )
    {
        // Make smaller one large enough to accomodate nlegend.
        if ( ncolumn < nrow )
            ncolumn = ( nlegend % nrow ) ? ( nlegend / nrow ) + 1 : nlegend / nrow;
        else
            nrow = ( nlegend % ncolumn ) ? ( nlegend / ncolumn ) + 1 : nlegend / ncolumn;
    }
    // fprintf(stdout, "nrow, ncolumn = %d, %d\n", nrow, ncolumn);

    // Default position flags and sanity checks for position flags.
    if ( !( opt & PL_LEGEND_RIGHT ) && !( opt & PL_LEGEND_LEFT ) && !( opt & PL_LEGEND_UPPER ) && !( opt & PL_LEGEND_LOWER ) )
    {
        opt = opt | PL_LEGEND_RIGHT | PL_LEGEND_UPPER;
    }
    else if ( ( opt & PL_LEGEND_RIGHT ) && ( opt & PL_LEGEND_LEFT ) )
    {
        plabort( "pllegend: PL_LEGEND_RIGHT and PL_LEGEND_LEFT cannot be simultaneously set." );
        return;
    }

    else if ( ( opt & PL_LEGEND_UPPER ) && ( opt & PL_LEGEND_LOWER ) )
    {
        plabort( "pllegend: PL_LEGEND_UPPER and PL_LEGEND_LOWER cannot be simultaneously set." );
        return;
    }

    if ( !( opt & PL_LEGEND_INSIDE ) && !( opt & PL_LEGEND_OUTSIDE ) )
    {
        opt = opt | PL_LEGEND_INSIDE;
    }
    else if ( ( opt & PL_LEGEND_INSIDE ) && ( opt & PL_LEGEND_OUTSIDE ) )
    {
        plabort( "pllegend: PL_LEGEND_INSIDE and PL_LEGEND_OUTSIDE cannot be simultaneously set." );
        return;
    }

    // xdmin_save, etc., are the external relative viewport
    // coordinates within the current sub-page.
    plgvpd( &xdmin_save, &xdmax_save, &ydmin_save, &ydmax_save );

    // xwmin_save, etc., are the external world coordinates corresponding
    // to the external viewport boundaries.
    plgvpw( &xwmin_save, &xwmax_save, &ywmin_save, &ywmax_save );

    // Internal viewport corresponds to sub-page so that all legends will
    // be clipped at sub-page boundaries.
    plvpor( 0., 1., 0., 1. );

    // Internal window coordinates are the same as normalized internal
    // viewport coordinates which are the same as normalized subpage coordinates.
    plwind( 0., 1., 0., 1. );

    plschr( 0., text_scale );

    for ( i = 0; i < nlegend; i++ )
    {
        if ( opt_array[i] & PL_LEGEND_COLOR_BOX )
            some_boxes = 1;
        if ( opt_array[i] & PL_LEGEND_LINE )
            some_lines = 1;
        if ( opt_array[i] & PL_LEGEND_SYMBOL )
        {
            max_symbol_numbers = MAX( max_symbol_numbers, symbol_numbers[i] );
            some_symbols       = 1;
        }
    }

    // Get character height and width in normalized subpage coordinates.
    character_height = get_character_or_symbol_height( TRUE );
    character_width  = character_height;

    // Calculate maximum width of text area.
    for ( i = 0; i < nlegend; i++ )
    {
        // units are mm.
        text_width0 = MAX( text_width0, plstrl( text[i] ) );
    }
    get_subpage_per_mm( &x_subpage_per_mm, &y_subpage_per_mm );

    // units are normalized subpage coordinates.
    text_width0 = x_subpage_per_mm * text_width0;

    // Allow gap on end closest to legend plot.
    text_width = text_width0 + text_offset * character_width;

    // Allow small border area where only the background is plotted
    // for left and right of legend.  0.4 seems to be a reasonable factor
    // that gives a good-looking result.
    width_border = 0.4 * character_width;
    // Separate columns (if any) by 2.0 * character_width.
    column_separation = 2.0 * character_width;

    // Total width and height of legend area in normalized subpage coordinates.
    total_width = 2. * width_border + ( ncolumn - 1 ) * column_separation +
                  ncolumn * ( text_width +
                              viewport_to_subpage_x( plot_width ) - viewport_to_subpage_x( 0. ) );
    total_height = nrow * text_spacing * character_height;

    // Total width and height of legend area in normalized external viewport
    // coordinates.

    total_width_vc  = subpage_to_viewport_x( total_width ) - subpage_to_viewport_x( 0. );
    total_height_vc = subpage_to_viewport_y( total_height ) - subpage_to_viewport_y( 0. );

    // dcolumn is the spacing from one column to the next and
    // drow is the spacing from one row to the next.
    dcolumn = column_separation + text_width +
              viewport_to_subpage_x( plot_width ) - viewport_to_subpage_x( 0. );
    drow = text_spacing * character_height;

    legend_position( opt, total_width_vc, total_height_vc, &x_legend_position, &y_legend_position );
    plot_x     = x + x_legend_position;
    plot_y     = y + y_legend_position;
    plot_x_end = plot_x + plot_width;
    // Normalized subpage coordinates for legend plots
    plot_x_subpage     = viewport_to_subpage_x( plot_x );
    plot_y_subpage     = viewport_to_subpage_y( plot_y );
    plot_x_end_subpage = viewport_to_subpage_x( plot_x_end );

    // Get normalized subpage positions of the start of the legend text
    text_x         = plot_x_end;
    text_y         = plot_y;
    text_x_subpage = viewport_to_subpage_x( text_x ) +
                     text_offset * character_width;
    text_y_subpage = viewport_to_subpage_y( text_y );

    if ( opt & PL_LEGEND_BACKGROUND )
    {
        PLFLT xbg[4] = {
            plot_x_subpage,
            plot_x_subpage,
            plot_x_subpage + total_width,
            plot_x_subpage + total_width,
        };
        PLFLT ybg[4] = {
            plot_y_subpage,
            plot_y_subpage - total_height,
            plot_y_subpage - total_height,
            plot_y_subpage,
        };
        plpsty( 0 );
        plcol0( bg_color );
        plfill( 4, xbg, ybg );
        plcol0( col0_save );
    }

    if ( opt & PL_LEGEND_BOUNDING_BOX )
    {
        PLFLT xbb[5] = {
            plot_x_subpage,
            plot_x_subpage,
            plot_x_subpage + total_width,
            plot_x_subpage + total_width,
            plot_x_subpage,
        };
        PLFLT ybb[5] = {
            plot_y_subpage,
            plot_y_subpage - total_height,
            plot_y_subpage - total_height,
            plot_y_subpage,
            plot_y_subpage,
        };
        pllsty( bb_style );
        plcol0( bb_color );
        plline( 5, xbb, ybb );
        plcol0( col0_save );
    }

    if ( opt & PL_LEGEND_TEXT_LEFT )
    {
        // text area on left, plot area on right.
        text_x_subpage      = plot_x_subpage;
        plot_x_subpage     += text_width;
        plot_x_end_subpage += text_width;
    }
    // adjust border after background is drawn.
    plot_x_subpage     += width_border;
    plot_x_end_subpage += width_border;
    text_x_subpage     += width_border;

    if ( some_symbols )
    {
        max_symbol_numbers = MAX( 2, max_symbol_numbers );
        if ( ( ( xs = (PLFLT *) malloc( max_symbol_numbers * sizeof ( PLFLT ) ) ) == NULL ) ||
             ( ( ys = (PLFLT *) malloc( max_symbol_numbers * sizeof ( PLFLT ) ) ) == NULL ) )
        {
            plexit( "pllegend: Insufficient memory" );
        }

        // Get symbol width in normalized subpage coordinates if symbols are plotted to
        // adjust ends of line of symbols.
        // AWI, no idea why must use 0.5 factor to get ends of symbol lines
        // to line up approximately correctly with plotted legend lines.
        // Factor should be unity.
        symbol_width = 0.5 * get_character_or_symbol_height( FALSE );
    }

    // Draw each legend entry
    for ( i = 0; i < nlegend; i++ )
    {
        // y position of text, lines, symbols, and/or centre of cmap0 box.
        ty     = text_y_subpage - ( (double) irow + 0.5 ) * drow;
        xshift = (double) icolumn * dcolumn;
        // Label/name for the legend
        plcol0( text_colors[i] );
        plptex( text_x_subpage + xshift + text_justification * text_width0, ty, 0.1, 0.0, text_justification, text[i] );

        if ( !( opt_array[i] & PL_LEGEND_NONE ) )
        {
            if ( opt_array[i] & PL_LEGEND_COLOR_BOX )
            {
                plcol0( box_colors[i] );
                plpsty( box_patterns[i] );
                xbox[0] = plot_x_subpage + xshift;
                xbox[1] = xbox[0];
                xbox[2] = plot_x_end_subpage + xshift;
                xbox[3] = xbox[2];
                ybox[0] = ty + 0.5 * drow * box_scales[i];
                ybox[1] = ty - 0.5 * drow * box_scales[i];
                ybox[2] = ty - 0.5 * drow * box_scales[i];
                ybox[3] = ty + 0.5 * drow * box_scales[i];
                plfill( 4, xbox, ybox );
            }
            if ( opt_array[i] & PL_LEGEND_LINE )
            {
                plcol0( line_colors[i] );
                pllsty( line_styles[i] );
                plwid( line_widths[i] );
                xl[0] = plot_x_subpage + xshift;
                xl[1] = plot_x_end_subpage + xshift;
                yl[0] = ty;
                yl[1] = ty;
                plline( 2, xl, yl );
                pllsty( line_style_save );
                plwid( line_width_save );
            }

            if ( opt_array[i] & PL_LEGEND_SYMBOL )
            {
                plcol0( symbol_colors[i] );
                plssym( 0., symbol_scales[i] );
                dxs = ( plot_x_end_subpage - plot_x_subpage - symbol_width ) / (double) ( MAX( symbol_numbers[i], 2 ) - 1 );
                for ( j = 0; j < symbol_numbers[i]; j++ )
                {
                    xs[j] = plot_x_subpage + xshift +
                            0.5 * symbol_width + dxs * (double) j;
                    ys[j] = ty;
                }
                plpoin( symbol_numbers[i], xs, ys, symbols[i] );
            }
        }

        // Set irow, icolumn for next i value.
        if ( opt & PL_LEGEND_ROW_MAJOR )
        {
            icolumn++;
            if ( icolumn >= ncolumn )
            {
                icolumn = 0;
                irow++;
            }
        }
        else
        {
            irow++;
            if ( irow >= nrow )
            {
                irow = 0;
                icolumn++;
            }
        }
    }
    if ( some_symbols )
    {
        free( xs );
        free( ys );
    }

    // Restore
    plcol0( col0_save );
    plschr( 0., text_scale_save );
    plssym( 0., symbol_scale_save );
    plpsty( pattern_save );
    plvpor( xdmin_save, xdmax_save, ydmin_save, ydmax_save );
    plwind( xwmin_save, xwmax_save, ywmin_save, ywmax_save );

    return;
}

