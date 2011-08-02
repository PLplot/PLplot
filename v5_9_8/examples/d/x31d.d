// $Id$
//
// Copyright (C) 2009 Werner Smekal
//
// set/get tester
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published
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
//

import std.string;
import std.stdio;
import std.math;
import std.cstream;

import plplot;

int main( char[][] args )
{
    // Parse and process command line arguments
    int status = 0;
    plparseopts( args, PL_PARSE_FULL );

    // Test setting / getting familying parameters before plinit
    // Save values set by plparseopts to be restored later.
    PLINT fam0, num0, bmax0;
    plgfam( &fam0, &num0, &bmax0 );
    PLINT fam1  = 0;
    PLINT num1  = 10;
    PLINT bmax1 = 1000;
    plsfam( fam1, num1, bmax1 );

    // Retrieve the same values?
    PLINT fam2, num2, bmax2;
    plgfam( &fam2, &num2, &bmax2 );
    writefln( "family parameters: fam, num, bmax = %d %d %d", fam2, num2, bmax2 );
    if ( fam2 != fam1 || num2 != num1 || bmax2 != bmax1 )
    {
        derr.writefln( "plgfam test failed" );
        status = 1;
    }
    // Restore values set initially by plparseopts.
    plsfam( fam0, num0, bmax0 );

    // Test setting / getting page parameters before plinit
    // Save values set by plparseopts to be restored later.
    PLFLT xp0, yp0;
    PLINT xleng0, yleng0, xoff0, yoff0;
    plgpage( &xp0, &yp0, &xleng0, &yleng0, &xoff0, &yoff0 );
    PLFLT xp1    = 200.0;
    PLFLT yp1    = 200.0;
    PLINT xleng1 = 400;
    PLINT yleng1 = 200;
    PLINT xoff1  = 10;
    PLINT yoff1  = 20;
    plspage( xp1, yp1, xleng1, yleng1, xoff1, yoff1 );

    // Retrieve the same values?
    PLFLT xp2, yp2;
    PLINT xleng2, yleng2, xoff2, yoff2;
    plgpage( &xp2, &yp2, &xleng2, &yleng2, &xoff2, &yoff2 );
    writefln( "page parameters: xp, yp, xleng, yleng, xoff, yoff = %f %f %d %d %d %d", xp2, yp2, xleng2, yleng2, xoff2, yoff2 );
    if ( xp2 != xp1 || yp2 != yp1 || xleng2 != xleng1 || yleng2 != yleng1 ||
         xoff2 != xoff1 || yoff2 != yoff1 )
    {
        derr.writefln( "plgpage test failed" );
        status = 1;
    }
    // Restore values set initially by plparseopts.
    plspage( xp0, yp0, xleng0, yleng0, xoff0, yoff0 );

    // Test setting / getting compression parameter across plinit.
    PLINT compression1 = 95;
    plscompression( compression1 );

    // Initialize plplot
    plinit();

    // Test if device initialization screwed around with the preset
    // compression parameter.
    PLINT compression2;
    plgcompression( &compression2 );
    writefln( "Output various PLplot parameters" );
    writefln( "compression parameter = %d", compression2 );
    if ( compression2 != compression1 )
    {
        derr.writefln( "plgcompression test failed" );
        status = 1;
    }


    // Exercise plscolor, plscol0, plscmap1, and plscmap1a to make sure
    // they work without any obvious error messages.
    PLINT[] r1 = [ 0, 255 ];
    PLINT[] g1 = [ 255, 0 ];
    PLINT[] b1 = [ 0, 0 ];
    PLFLT[] a1 = [ 1.0, 1.0 ];
    plscolor( 1 );
    plscol0( 1, 255, 0, 0 );
    plscmap1( r1, g1, b1 );
    plscmap1a( r1, g1, b1, a1 );

    PLINT level2;
    plglevel( &level2 );
    writefln( "level parameter = %d", level2 );
    if ( level2 != 1 )
    {
        derr.writefln( "plglevel test failed." );
        status = 1;
    }

    pladv( 0 );
    plvpor( 0.01, 0.99, 0.02, 0.49 );
    PLFLT xmin, xmax, ymin, ymax;
    plgvpd( &xmin, &xmax, &ymin, &ymax );
    writefln( "plvpor: xmin, xmax, ymin, ymax = %f %f %f %f", xmin, xmax, ymin, ymax );
    if ( xmin != 0.01 || xmax != 0.99 || ymin != 0.02 || ymax != 0.49 )
    {
        derr.writefln( "plgvpd test failed" );
        status = 1;
    }
    PLFLT xmid = 0.5 * ( xmin + xmax );
    PLFLT ymid = 0.5 * ( ymin + ymax );

    plwind( 0.2, 0.3, 0.4, 0.5 );
    plgvpw( &xmin, &xmax, &ymin, &ymax );
    writefln( "plwind: xmin, xmax, ymin, ymax = %f %f %f %f", xmin, xmax, ymin, ymax );
    if ( xmin != 0.2 || xmax != 0.3 || ymin != 0.4 || ymax != 0.5 )
    {
        derr.writefln( "plgvpw test failed" );
        status = 1;
    }

    // Get world coordinates for middle of viewport
    PLFLT wx, wy;
    PLINT win;
    plcalc_world( xmid, ymid, &wx, &wy, &win );
    writefln( "world parameters: wx, wy, win = %f %f %d", wx, wy, win );
    if ( fabs( wx - 0.5 * ( xmin + xmax ) ) > 1.0E-5 || fabs( wy - 0.5 * ( ymin + ymax ) ) > 1.0E-5 )
    {
        derr.writefln( "plcalc_world test failed" );
        status = 1;
    }

    // Retrieve and print the name of the output file (if any).
    // This goes to stderr not stdout since it will vary between tests and
    // we want stdout to be identical for compare test.
    string fnam;
    plgfnam( fnam );
    if ( fnam.length == 0 )
        writefln( "No output file name is set" );
    else
        writefln( "Output file name read" );
    derr.writefln( "Output file name is %s", fnam );

    // Set and get the number of digits used to display axis labels
    // Note digits is currently ignored in pls[xyz]ax and
    // therefore it does not make sense to test the returned
    // value
    plsxax( 3, 0 );
    PLINT digmax, digits;
    plgxax( &digmax, &digits );
    writefln( "x axis parameters: digmax, digits = %d %d", digmax, digits );
    if ( digmax != 3 )
    {
        derr.writefln( "plgxax test failed" );
        status = 1;
    }

    plsyax( 4, 0 );
    plgyax( &digmax, &digits );
    writefln( "y axis parameters: digmax, digits = %d %d", digmax, digits );
    if ( digmax != 4 )
    {
        derr.writefln( "plgyax test failed" );
        status = 1;
    }

    plszax( 5, 0 );
    plgzax( &digmax, &digits );
    writefln( "z axis parameters: digmax, digits = %d %d", digmax, digits );
    if ( digmax != 5 )
    {
        derr.writefln( "plgzax test failed" );
        status = 1;
    }

    plsdidev( 0.05, PL_NOTSET, 0.1, 0.2 );
    PLFLT mar, aspect, jx, jy;
    plgdidev( &mar, &aspect, &jx, &jy );
    writefln( "device-space window parameters: mar, aspect, jx, jy = %f %f %f %f", mar, aspect, jx, jy );
    if ( mar != 0.05 || jx != 0.1 || jy != 0.2 )
    {
        derr.writefln( "plgdidev test failed" );
        status = 1;
    }

    plsdiori( 1.0 );
    PLFLT ori;
    plgdiori( &ori );
    writefln( "ori parameter = %f", ori );
    if ( ori != 1.0 )
    {
        derr.writefln( "plgdiori test failed" );
        status = 1;
    }

    plsdiplt( 0.1, 0.2, 0.9, 0.8 );
    plgdiplt( &xmin, &ymin, &xmax, &ymax );
    writefln( "plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f", xmin, ymin, xmax, ymax );
    if ( xmin != 0.1 || xmax != 0.9 || ymin != 0.2 || ymax != 0.8 )
    {
        derr.writefln( "plgdiplt test failed" );
        status = 1;
    }

    plsdiplz( 0.1, 0.1, 0.9, 0.9 );
    PLFLT zxmin, zymin, zxmax, zymax;
    plgdiplt( &zxmin, &zymin, &zxmax, &zymax );
    writefln( "zoomed plot-space window parameters: xmin, ymin, xmax, ymax = %f %f %f %f", zxmin, zymin, zxmax, zymax );
    if ( fabs( zxmin - ( xmin + ( xmax - xmin ) * 0.1 ) ) > 1.0E-5 ||
         fabs( zxmax - ( xmin + ( xmax - xmin ) * 0.9 ) ) > 1.0E-5 ||
         fabs( zymin - ( ymin + ( ymax - ymin ) * 0.1 ) ) > 1.0E-5 ||
         fabs( zymax - ( ymin + ( ymax - ymin ) * 0.9 ) ) > 1.0E-5 )
    {
        derr.writefln( "plsdiplz test failed" );
        status = 1;
    }

    plscolbg( 10, 20, 30 );
    PLINT r, g, b;
    plgcolbg( &r, &g, &b );
    writefln( "background colour parameters: r, g, b = %d %d %d", r, g, b );
    if ( r != 10 || g != 20 || b != 30 )
    {
        derr.writefln( "plgcolbg test failed" );
        status = 1;
    }

    plscolbga( 20, 30, 40, 0.5 );
    PLFLT a;
    plgcolbga( &r, &g, &b, &a );
    writefln( "background/transparency colour parameters: r, g, b, a = %d %d %d %f", r, g, b, a );
    if ( r != 20 || g != 30 || b != 40 || a != 0.5 )
    {
        derr.writefln( "plgcolbga test failed" );
        status = 1;
    }

    plend();

    return status;
}
