/* pllegend()
 *
 * Author: Hezekiah Carty 2010
 *
 * Copyright (C) 2010  Hezekiah M. Carty
 *
 * This file is part of PLplot.
 *
 * PLplot is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Library Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * PLplot is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with PLplot; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "plplotP.h"

static PLFLT get_character_or_symbol_height( PLINT ifcharacter )
{
    // Character height in mm
    PLFLT default_mm, char_height_mm;
    // Normalized viewport limits
    PLFLT vxmin, vxmax, vymin, vymax;
    PLFLT vy;
    // Size of subpage in mm
    PLFLT mxmin, mxmax, mymin, mymax;
    PLFLT mm_y;
    // World height in mm
    PLFLT world_height_mm;
    // Normalized character height
    PLFLT char_height_norm;
    // Window dimensions
    PLFLT wxmin, wxmax, wymin, wymax;
    PLFLT world_y;

    if ( ifcharacter )
    {
        plgchr( &default_mm, &char_height_mm );
    }
    else
    {
        default_mm     = plsc->symdef;
        char_height_mm = plsc->symht;
    }
    plgvpd( &vxmin, &vxmax, &vymin, &vymax );
    vy = vymax - vymin;

    plgspa( &mxmin, &mxmax, &mymin, &mymax );
    mm_y = mymax - mymin;

    world_height_mm = mm_y * vy;

    // Character height (mm) / World height (mm) = Normalized char height
    char_height_norm = char_height_mm / world_height_mm;

    // Normalized character height * World height (world) =
    // Character height (world)
    plgvpw( &wxmin, &wxmax, &wymin, &wymax );
    world_y = wymax - wymin;

    return ( char_height_norm * world_y );
}

#define normalized_to_world_x( nx )    ( ( xmin ) + ( nx ) * ( ( xmax ) - ( xmin ) ) )
#define normalized_to_world_y( ny )    ( ( ymin ) + ( ny ) * ( ( ymax ) - ( ymin ) ) )

// pllegend - Draw a legend using lines (nsymbols <=1 or symbols == NULL) or
// points/symbols.
// plot_width: width of plotted areas (lines, symbols, or colour boxes) in legend
// x, y: Normalized position of the legend in the plot window
// nlegend: Number of legend entries
// text_colors: Color map 0 indices of the colors to use for label text
// text: text string for each legend entry
// cmap0_colors: cmap0 color index for each legend entry
// nsymbols: number of points/symbols to be drawn for each plot_width
// symbols: Symbol to draw for each legend entry.

void
c_pllegend( PLINT opt, PLFLT plot_width, PLFLT x, PLFLT y, PLINT nlegend,
            PLINT *text_colors, char **text, PLINT *cmap0_colors,
            PLINT *line_style, PLINT *line_width,
            PLINT nsymbols, PLINT *symbols )

{
    // Viewport world-coordinate limits
    PLFLT xmin, xmax, ymin, ymax;
    // Legend position
    PLFLT plot_x, plot_x_end, plot_x_world, plot_x_end_world;
    PLFLT plot_y, plot_y_world;
    PLFLT text_x, text_y, text_x_world, text_y_world;
    // Character height (world coordinates)
    PLFLT character_height, character_width, symbol_width;
    // y-position of the current legend entry
    PLFLT ty;
    // Positions of the legend entries
    PLFLT dxs, *xs, *ys, xl[2], yl[2];
    PLINT i, j;
    // opt_plot is the kind of plot made for the legend.
    PLINT opt_plot = opt & ( PL_LEGEND_LINE | PL_LEGEND_SYMBOL |
                             PL_LEGEND_CMAP0 | PL_LEGEND_CMAP1 );
    // Active attributes to be saved and restored afterward.
    PLINT old_col0 = plsc->icol0, old_line_style = plsc->line_style,
        old_line_width = plsc->width;
    // Sanity checks.
    // Check opt_plot for a valid combination of kind of plots.
    if ( !( ( opt_plot & ( PL_LEGEND_LINE | PL_LEGEND_SYMBOL ) ) ||
            opt_plot == PL_LEGEND_CMAP0 || opt_plot == PL_LEGEND_CMAP1 ) )
    {
        plabort( "pllegend: invalid opt" );
        return;
    }

    if ( ( symbols == NULL ) && ( opt & PL_LEGEND_SYMBOL ) )
    {
        plabort( "pllegend: invalid combination of opt requesting a symbols style of legend while symbols are not properly defined." );
        return;
    }
    nsymbols = MAX( 2, nsymbols );

    plgvpw( &xmin, &xmax, &ymin, &ymax );

    // World coordinates for legend plots
    plot_x           = x;
    plot_y           = y;
    plot_x_end       = plot_x + plot_width;
    plot_x_world     = normalized_to_world_x( plot_x );
    plot_y_world     = normalized_to_world_y( plot_y );
    plot_x_end_world = normalized_to_world_x( plot_x_end );

    // Get character height and width in world coordinates
    character_height = get_character_or_symbol_height( 1 );
    character_width  = character_height * fabs( ( xmax - xmin ) / ( ymax - ymin ) );
    // Get world-coordinate positions of the start of the legend text
    text_x       = plot_x_end;
    text_y       = plot_y;
    text_x_world = normalized_to_world_x( text_x ) + character_width;
    text_y_world = normalized_to_world_y( text_y );

    //    if (opt & PL_LEGEND_TEXT_LEFT)
    {
      //
    }

    // Starting y position for legend entries
    ty = text_y_world - character_height;

    if ( opt & PL_LEGEND_LINE )
    {
        xl[0] = plot_x_world;
        xl[1] = plot_x_end_world;
        yl[0] = ty;
        yl[1] = ty;
    }

    if ( opt & PL_LEGEND_SYMBOL )
    {
        if ( ( ( xs = (PLFLT *) malloc( nsymbols * sizeof ( PLFLT ) ) ) == NULL ) ||
             ( ( ys = (PLFLT *) malloc( nsymbols * sizeof ( PLFLT ) ) ) == NULL ) )
        {
            plexit( "pllegend: Insufficient memory" );
        }

        // Get symbol width in world coordinates if symbols are plotted to
        // adjust ends of line of symbols.
        // AWI, no idea why must use 0.5 factor to get ends of symbol lines
        // to line up approximately correctly with plotted legend lines.
        // Factor should be unity.
        symbol_width = 0.5 * get_character_or_symbol_height( 0 ) *
                       fabs( ( xmax - xmin ) / ( ymax - ymin ) );
        dxs = ( plot_x_end_world - plot_x_world - symbol_width ) / (double) ( nsymbols - 1 );
        for ( j = 0; j < nsymbols; j++ )
        {
            xs[j] = plot_x_world + 0.5 * symbol_width + dxs * (double) j;
            ys[j] = ty;
        }
    }

    // Draw each legend entry
    for ( i = 0; i < nlegend; i++ )
    {
        // Label/name for the legend
        plcol0( text_colors[i] );
        plptex( text_x_world, ty, 0.0, 0.0, 0.0, text[i] );

        // prepare for the next position
        ty = ty - ( 1.5 * character_height );
        plcol0( cmap0_colors[i] );
        if ( opt & PL_LEGEND_LINE )
        {
            pllsty( line_style[i]) ;
            plwid( line_width[i] );
            plline( 2, xl, yl );
            // prepare for the next position
            yl[0] = ty;
            yl[1] = ty;
            pllsty( old_line_style );
            plwid( old_line_width );
        }
        if ( opt & PL_LEGEND_SYMBOL )
        {
            plpoin( nsymbols, xs, ys, symbols[i] );
            // prepare for the next position
            for ( j = 0; j < nsymbols; j++ )
            {
                ys[j] = ty;
            }
        }
    }
    if ( opt & PL_LEGEND_SYMBOL )
    {
        free( xs );
        free( ys );
    }

    // Restore the previously active drawing color
    plcol0( old_col0 );

    return;
}

