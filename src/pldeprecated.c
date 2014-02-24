//  Copyright (C) 2005-2014 Alan W. Irwin
//
//  This file is part of PLplot.
//
//  PLplot is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Library General Public License as published
//  by the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  PLplot is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public License
//  along with PLplot; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//--------------------------------------------------------------------------
//
//  This file contains deprecated routines to provide backwards compatibility
//  for a while.  For each routine the new routine you should be using instead
//  is explicitly commented.
//

#include "plplotP.h"

#ifdef PL_DEPRECATED

// The following functions have been removed from plplot ahead of the 5.9.8
// release. They have long been advertised as deprecated.
//   plParseOpts
//   plHLS_RGB
//   plRGB_HLS
//   plarrows


// The following functions have been marked as obsolete for some time,
// but were formally deprecated as of version 5.9.8
//   plrgb
//   plrgb1
//   plhls

// The following function was formally deprecated in 5.9.10.
//   plwid

//--------------------------------------------------------------------------
// plrgb()
//
// Set line color by red, green, blue from  0. to 1.
// Do NOT use this.  Only retained for backward compatibility
//--------------------------------------------------------------------------

void
c_plrgb( PLFLT r, PLFLT g, PLFLT b )
{
    plwarn( "plrgb: function deprecated. Use plscol instead" );

    if ( plsc->level < 1 )
    {
        plabort( "plrgb: Please call plinit first" );
        return;
    }

    plsc->icol0      = PL_RGB_COLOR;
    plsc->curcolor.r = MAX( 0, MIN( 255, (int) ( 256. * r ) ) );
    plsc->curcolor.g = MAX( 0, MIN( 255, (int) ( 256. * g ) ) );
    plsc->curcolor.b = MAX( 0, MIN( 255, (int) ( 256. * b ) ) );

    plsc->curcmap = 0;
    plP_state( PLSTATE_COLOR0 );
}

//--------------------------------------------------------------------------
// plrgb1()
//
// Set line color by 8 bit RGB values.
// Do NOT use this.  Only retained for backward compatibility
//--------------------------------------------------------------------------

void
c_plrgb1( PLINT r, PLINT g, PLINT b )
{
    plwarn( "plrgb1: function deprecated. Use plscol instead" );

    if ( plsc->level < 1 )
    {
        plabort( "plrgb1: Please call plinit first" );
        return;
    }
    if ( ( r < 0 || r > 255 ) || ( g < 0 || g > 255 ) || ( b < 0 || b > 255 ) )
    {
        plabort( "plrgb1: Invalid color" );
        return;
    }

    plsc->icol0      = PL_RGB_COLOR;
    plsc->curcolor.r = r;
    plsc->curcolor.g = g;
    plsc->curcolor.b = b;

    plsc->curcmap = 0;
    plP_state( PLSTATE_COLOR0 );
}

//--------------------------------------------------------------------------
// void plhls()
//
// Set current color by hue, lightness, and saturation.
// Convert hls color coordinates to rgb, then call plrgb.
// Do NOT use this.  Only retained for backward compatibility
//--------------------------------------------------------------------------

void
c_plhls( PLFLT h, PLFLT l, PLFLT s )
{
    PLFLT r, g, b;

    plwarn( "plhls: function deprecated. Use plhlsrgb / plscol instead" );

    c_plhlsrgb( h, l, s, &r, &g, &b );
    plrgb( r, g, b );
}

//--------------------------------------------------------------------------
// void plwid()
//
// Set pen width using a deprecated integer width value rather than
// the recommended plwidth call with floating-point width value.
//--------------------------------------------------------------------------

void
c_plwid( PLINT width )
{
    plwarn( "plwid: function deprecated. Use plwidth instead" );
    plwidth( (PLFLT) width );
}

#endif // PL_DEPRECATED
