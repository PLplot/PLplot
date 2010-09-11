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

PLFLT get_character_or_symbol_height(PLINT ifcharacter) {
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

    if(ifcharacter) {
      plgchr(&default_mm, &char_height_mm);
    } else {
      default_mm = plsc->symdef;
      char_height_mm = plsc->symht;
    }
    plgvpd(&vxmin, &vxmax, &vymin, &vymax);
    vy = vymax - vymin;

    plgspa(&mxmin, &mxmax, &mymin, &mymax);
    mm_y = mymax - mymin;

    world_height_mm = mm_y * vy;

    // Character height (mm) / World height (mm) = Normalized char height
    char_height_norm = char_height_mm / world_height_mm;

    // Normalized character height * World height (world) =
    // Character height (world)
    plgvpw(&wxmin, &wxmax, &wymin, &wymax);
    world_y = wymax - wymin;

    return(char_height_norm * world_y);
}

#define normalized_to_world_x(nx) ((xmin) + (nx) * ((xmax) - (xmin)))
#define normalized_to_world_y(ny) ((ymin) + (ny) * ((ymax) - (ymin)))

// pllegend - Draw a legend using lines (nsymbols <=1 or symbols == NULL) or 
// points/symbols.
// line_length: extent of lines or extent of nsymbols points/symbols
// x, y: Normalized position of the legend in the plot window
// n: Number of legend entries
// text_colors: Color map 0 indices of the colors to use for label text
// names: Name/label for each legend entry
// colors: Line/point color for each legend entry
// nsymbols: number of points/symbols to be drawn for each line_length
// symbols: Symbol to draw for each legend entry.
void c_pllegend(PLFLT line_length, PLFLT x, PLFLT y, PLINT n, PLINT *text_colors, char **names, PLINT *colors, PLINT nsymbols, PLINT *symbols) {
    // Active indexed drawing color
    PLINT old_col0;
    // Viewport world-coordinate limits
    PLFLT xmin, xmax, ymin, ymax;
    // Legend position
    PLFLT line_x, line_x_end, line_x_world, line_x_end_world;
    PLFLT line_y, line_y_world;
    PLFLT text_x, text_y, text_x_world, text_y_world;
    // Character height (world coordinates)
    PLFLT character_height, character_width, symbol_width;
    // y-position of the current legend entry
    PLFLT ty;
    // Positions of the legend entries
    PLFLT dxs, *xs, *ys;
    PLINT i, j;
    // ifline is true if lines are to be drawn, false if points/symbols are
    // to be drawn.
    int ifline = nsymbols <= 1 || symbols == NULL;
    if(symbols == NULL) nsymbols = 2;
    nsymbols = MAX(2, nsymbols);

    old_col0 = plsc->icol0;

    plgvpw(&xmin, &xmax, &ymin, &ymax);

    // World coordinates for legend lines
    line_x = x;
    line_y = y;
    line_x_end = line_x + line_length;
    line_x_world = normalized_to_world_x(line_x);
    line_y_world = normalized_to_world_y(line_y);
    line_x_end_world = normalized_to_world_x(line_x_end);

    // Get character height and width in world coordinates
    character_height = get_character_or_symbol_height(1);
    character_width = character_height*fabs((xmax-xmin)/(ymax-ymin));
    // Get symbol width in world coordinates if symbols are plotted to
    // adjust ends of line of symbols.
    if( ifline) {
      symbol_width = 0.;
    } else {
      // AWI, no idea why must use 0.5 factor to get ends of symbol lines
      // to line up approximately correctly with plotted legend lines.
      // Factor should be unity.
      symbol_width = 0.5*get_character_or_symbol_height(0)*
      fabs((xmax-xmin)/(ymax-ymin));
    }
    // Get world-coordinate positions of the start of the legend text
    text_x = line_x_end;
    text_y = line_y;
    text_x_world = normalized_to_world_x(text_x) + character_width;
    text_y_world = normalized_to_world_y(text_y);

    // Starting y position for legend entries
    ty = text_y_world - character_height;

    if ( ( ( xs = (PLFLT *) malloc( nsymbols * sizeof ( PLFLT ) ) ) == NULL ) ||
	 ( ( ys = (PLFLT *) malloc( nsymbols * sizeof ( PLFLT ) ) ) == NULL ) )
      {
	plexit( "pllegend: Insufficient memory" );
      }

    dxs = (line_x_end_world - line_x_world - symbol_width)/(double) (nsymbols-1);
    for (j=0; j<nsymbols; j++) {
      xs[j] = line_x_world + 0.5*symbol_width + dxs*(double) j;
      ys[j] = ty;
    }

    // Draw each legend entry
    for (i = 0; i < n; i++) {
        // Line for the legend
        plcol0(colors[i]);
        if (ifline) {
            // Draw lines
            plline(nsymbols, xs, ys);
        }
        else {
            // Draw symbols
            plpoin(nsymbols, xs, ys, symbols[i]);
        }

        // Label/name for the legend
        plcol0(text_colors[i]);
        plptex(text_x_world, ty, 0.0, 0.0, 0.0, names[i]);
        // Move to the next position
        ty = ty - (1.5 * character_height);
	for (j=0; j<nsymbols; j++) {
	  ys[j] = ty;
	}
    }
    free( xs );
    free( ys );

    // Restore the previously active drawing color
    plcol0(old_col0);

    return;
}

