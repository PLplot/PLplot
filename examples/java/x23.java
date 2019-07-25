//--------------------------------------------------------------------------
//    Displays Greek letters and mathematically interesting Unicode ranges
//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
// Copyright (C) 2005-2014  Alan Irwin
// Copyright (C) 2005  Andrew Ross
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
// Implementation of PLplot example 23 in Java.
//--------------------------------------------------------------------------

package plplot.examples;

import plplot.core.*;
import static plplot.core.plplotjavacConstants.*;

//
// Displays Greek letters and mathematically interesting Unicode ranges
//

class x23 {
    PLStream pls = new PLStream();

    static String[] Greek = {
        "#gA", "#gB", "#gG", "#gD", "#gE", "#gZ", "#gY", "#gH", "#gI", "#gK", "#gL", "#gM",
        "#gN", "#gC", "#gO", "#gP", "#gR", "#gS", "#gT", "#gU", "#gF", "#gX", "#gQ", "#gW",
        "#ga", "#gb", "#gg", "#gd", "#ge", "#gz", "#gy", "#gh", "#gi", "#gk", "#gl", "#gm",
        "#gn", "#gc", "#go", "#gp", "#gr", "#gs", "#gt", "#gu", "#gf", "#gx", "#gq", "#gw"
    };

    static String[] Type1 = {
        "0x0020", "0x0021", "0x0023", "0x0025", "0x0026",
        "0x0028", "0x0029", "0x002b", "0x002c", "0x002e",
        "0x002f", "0x0030", "0x0031", "0x0032", "0x0033",
        "0x0034", "0x0035", "0x0036", "0x0037", "0x0038",
        "0x0039", "0x003a", "0x003b", "0x003c", "0x003d",
        "0x003e", "0x003f", "0x005b", "0x005d", "0x005f",
        "0x007b", "0x007c", "0x007d", "0x00a9", "0x00ac",
        "0x00ae", "0x00b0", "0x00b1", "0x00d7", "0x00f7",
        "0x0192", "0x0391", "0x0392", "0x0393", "0x0394",
        "0x0395", "0x0396", "0x0397", "0x0398", "0x0399",
        "0x039a", "0x039b", "0x039c", "0x039d", "0x039e",
        "0x039f", "0x03a0", "0x03a1", "0x03a3", "0x03a4",
        "0x03a5", "0x03a6", "0x03a7", "0x03a8", "0x03a9",
        "0x03b1", "0x03b2", "0x03b3", "0x03b4", "0x03b5",
        "0x03b6", "0x03b7", "0x03b8", "0x03b9", "0x03ba",
        "0x03bb", "0x03bc", "0x03bd", "0x03be", "0x03bf",
        "0x03c0", "0x03c1", "0x03c2", "0x03c3", "0x03c4",
        "0x03c5", "0x03c6", "0x03c7", "0x03c8", "0x03c9",
        "0x03d1", "0x03d2", "0x03d5", "0x03d6", "0x2022",
        "0x2026", "0x2032", "0x2033", "0x203e", "0x2044",
        "0x2111", "0x2118", "0x211c", "0x2122", "0x2126",
        "0x2135", "0x2190", "0x2191", "0x2192", "0x2193",
        "0x2194", "0x21b5", "0x21d0", "0x21d1", "0x21d2",
        "0x21d3", "0x21d4", "0x2200", "0x2202", "0x2203",
        "0x2205", "0x2206", "0x2207", "0x2208", "0x2209",
        "0x220b", "0x220f", "0x2211", "0x2212", "0x2215",
        "0x2217", "0x221a", "0x221d", "0x221e", "0x2220",
        "0x2227", "0x2228", "0x2229", "0x222a", "0x222b",
        "0x2234", "0x223c", "0x2245", "0x2248", "0x2260",
        "0x2261", "0x2264", "0x2265", "0x2282", "0x2283",
        "0x2284", "0x2286", "0x2287", "0x2295", "0x2297",
        "0x22a5", "0x22c5", "0x2320", "0x2321", "0x2329",
        "0x232a", "0x25ca", "0x2660", "0x2663", "0x2665",
        "0x2666"
    };

    static String[] title = {
        "#<0x10>PLplot Example 23 - Greek Letters",
        "#<0x10>PLplot Example 23 - Type 1 Symbol Font Glyphs by Unicode (a)",
        "#<0x10>PLplot Example 23 - Type 1 Symbol Font Glyphs by Unicode (b)",
        "#<0x10>PLplot Example 23 - Type 1 Symbol Font Glyphs by Unicode (c)",
        "#<0x10>PLplot Example 23 - Number Forms Unicode Block",
        "#<0x10>PLplot Example 23 - Arrows Unicode Block (a)",
        "#<0x10>PLplot Example 23 - Arrows Unicode Block (b)",
        "#<0x10>PLplot Example 23 - Mathematical Operators Unicode Block (a)",
        "#<0x10>PLplot Example 23 - Mathematical Operators Unicode Block (b)",
        "#<0x10>PLplot Example 23 - Mathematical Operators Unicode Block (c)",
        "#<0x10>PLplot Example 23 - Mathematical Operators Unicode Block (d)",
    };

    static int[] lo = {
        0x0,
        0x0,
        0x40,
        0x80,
        0x2153,
        0x2190,
        0x21d0,
        0x2200,
        0x2240,
        0x2280,
        0x22c0,
    };

    static int[] hi = {
        0x30,
        0x40,
        0x80,
        0xA6,
        0x2184,
        0x21d0,
        0x2200,
        0x2240,
        0x2280,
        0x22c0,
        0x2300,
    };

    static int[] nxcells = {
        12,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
    };

    static int[] nycells = {
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
        8,
    };

    // non-zero values Must be consistent with nxcells and nycells.
    static int[] offset = {
        0,
        0,
        64,
        128,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
    };

// 30 possible FCI values.

    static int FCI_COMBINATIONS = 30;

    static long fci[] = {
        0x80000000L,
        0x80000001L,
        0x80000002L,
        0x80000003L,
        0x80000004L,
        0x80000010L,
        0x80000011L,
        0x80000012L,
        0x80000013L,
        0x80000014L,
        0x80000020L,
        0x80000021L,
        0x80000022L,
        0x80000023L,
        0x80000024L,
        0x80000100L,
        0x80000101L,
        0x80000102L,
        0x80000103L,
        0x80000104L,
        0x80000110L,
        0x80000111L,
        0x80000112L,
        0x80000113L,
        0x80000114L,
        0x80000120L,
        0x80000121L,
        0x80000122L,
        0x80000123L,
        0x80000124L,
    };

    static String family[] = {
        "sans-serif",
        "serif",
        "monospace",
        "script",
        "symbol",
    };
    static String style[] = {
        "upright",
        "italic",
        "oblique",
    };
    static String weight[] = {
        "medium",
        "bold",
    };

    public x23( String[] args )
    {
        double xmin[], xmax[], ymin[], ymax[], ycharacter_scale, yoffset;
        double chardef[], charht[], deltax, deltay, x, y;
        int    i, j, page, length, slice;
        String cmdString = null;

        pls.parseopts( args, PL_PARSE_FULL | PL_PARSE_NOPROGRAM );

        pls.init();

        for ( page = 0; page < 11; page++ )
        {
            pls.adv( 0 );

            // Set up viewport and window

            pls.vpor( 0.02, 0.98, 0.02, 0.90 );
            pls.wind( 0.0, 1.0, 0.0, 1.0 );
            xmin = new double[1];
            xmax = new double[1];
            ymin = new double[1];
            ymax = new double[1];
            pls.gspa( xmin, xmax, ymin, ymax );
            pls.schr( 0., 0.8 );
            ycharacter_scale = ( 1.0 - 0.0 ) / ( ymax[0] - ymin[0] );

            // Factor should be 0.5, but heuristically it turns out to be larger.
            chardef = new double[1];
            charht  = new double[1];
            pls.gchr( chardef, charht );
            yoffset = 1.0 * charht[0] * ycharacter_scale;

            // Draw the grid using PLStream.box

            pls.col0( 2 );
            deltax = 1.0 / ( nxcells[page] );
            deltay = 1.0 / ( nycells[page] );
            pls.box( "bcg", deltax, 0, "bcg", deltay, 0 );
            pls.col0( 15 );
            length = hi[page] - lo[page];
            slice  = 0;
            for ( j = nycells[page] - 1; j >= -1; j-- )
            {
                y = ( 0.5 + j ) * deltay;
                for ( i = 0; i < nxcells[page]; i++ )
                {
                    x = ( 0.5 + i ) * deltax;
                    if ( slice < length )
                    {
                        if ( page == 0 )
                        {
                            cmdString = Greek[slice];
                        }
                        else if ( ( page >= 1 ) && ( page <= 3 ) )
                        {
                            cmdString = "#[" + Type1[offset[page] + slice] + "]";
                        }
                        else if ( page >= 4 )
                        {
                            cmdString = "#[0x" + Integer.toHexString( lo[page] + slice ) + "]";
                        }
                        pls.ptex( x, y + yoffset, 1., 0., 0.5, cmdString );
                        pls.ptex( x, y - yoffset, 1., 0., 0.5, "#" + cmdString );
                    }
                    slice += 1;
                }
            }

            pls.schr( 0., 1.0 );
            // Page title
            pls.mtex( "t", 1.5, 0.5, 0.5, title[page] );
        }

        // Demonstrate methods of getting the current fonts
        long[] fci_old = new long[1];
        int[]  ifamily = new int[1];
        int[]  istyle  = new int[1];
        int[]  iweight = new int[1];
        pls.gfci( fci_old );
        pls.gfont( ifamily, istyle, iweight );
        System.out.println( "For example 23 prior to page 12 the FCI is 0x" + Long.toHexString( fci_old[0] ) );
        System.out.println( "For example 23 prior to page 12 the font family, style and weight are " + family[ifamily[0]] + " " + style[istyle[0]] + " " + weight[iweight[0]] );;

        for ( page = 11; page < 16; page++ )
        {
            double dy = 0.030;
            int    family_index, style_index, weight_index;
            // Must be big enough to contain the prefix strings, the
            // font-changing commands, and the "The quick brown..." string.
            String string = "";

            pls.adv( 0 );
            pls.vpor( 0.02, 0.98, 0.02, 0.90 );
            pls.wind( 0.0, 1.0, 0.0, 1.0 );
            pls.sfci( 0 );
            if ( page == 11 )
            {
                pls.mtex( "t", 1.5, 0.5, 0.5,
                    "#<0x10>PLplot Example 23 - " +
                    "Set Font with plsfci" );
            }
            else if ( page == 12 )
            {
                pls.mtex( "t", 1.5, 0.5, 0.5,
                    "#<0x10>PLplot Example 23 - " +
                    "Set Font with plsfont" );
            }
            else if ( page == 13 )
            {
                pls.mtex( "t", 1.5, 0.5, 0.5,
                    "#<0x10>PLplot Example 23 - " +
                    "Set Font with ##<0x8nnnnnnn> construct" );
            }
            else if ( page == 14 )
            {
                pls.mtex( "t", 1.5, 0.5, 0.5,
                    "#<0x10>PLplot Example 23 - " +
                    "Set Font with ##<0xmn> constructs" );
            }
            else if ( page == 15 )
            {
                pls.mtex( "t", 1.5, 0.5, 0.5,
                    "#<0x10>PLplot Example 23 - " +
                    "Set Font with ##<FCI COMMAND STRING/> constructs" );
            }
            pls.schr( 0., 0.75 );
            for ( i = 0; i < FCI_COMBINATIONS; i++ )
            {
                family_index = i % 5;
                style_index  = ( i / 5 ) % 3;
                weight_index = ( ( i / 5 ) / 3 ) % 2;
                if ( page == 11 )
                {
                    pls.sfci( fci[i] );
                    string = "Page 12, " + family[family_index] + ", " +
                             style[style_index] + ", " + weight[weight_index] +
                             ":  The quick brown fox jumps over the lazy dog";
                }
                else if ( page == 12 )
                {
                    pls.sfont( family_index, style_index, weight_index );
                    string = "Page 13, " + family[family_index] + ", " +
                             style[style_index] + ", " + weight[weight_index] +
                             ":  The quick brown fox jumps over the lazy dog";
                }
                else if ( page == 13 )
                {
                    string = "Page 14, " + family[family_index] + ", " +
                             style[style_index] + ", " + weight[weight_index] +
                             ":  #<0x" + Long.toHexString( fci[i] ) + ">" +
                             "The quick brown fox jumps over the lazy dog";
                }
                else if ( page == 14 )
                {
                    string = "Page 15, " + family[family_index] + ", " +
                             style[style_index] + ", " + weight[weight_index] + ":  " +
                             "#<0x" + Integer.toHexString( family_index ) + "0>" +
                             "#<0x" + Integer.toHexString( style_index ) + "1>" +
                             "#<0x" + Integer.toHexString( weight_index ) + "2>" +
                             "The quick brown fox jumps over the lazy dog";
                }
                else if ( page == 15 )
                {
                    string = "Page 16, " + family[family_index] + ", " +
                             style[style_index] + ", " + weight[weight_index] + ":  " +
                             "#<" + family[family_index] + "/>" +
                             "#<" + style[style_index] + "/>" +
                             "#<" + weight[weight_index] + "/>" +
                             "The quick brown fox jumps over the lazy dog";
                }
                pls.ptex( 0., 1. - ( i + 0.5 ) * dy, 1., 0., 0., string );
            }

            pls.schr( 0., 1.0 );
        }

        // Restore defaults
        pls.col0( 1 );

        pls.end();
    }

    public static void main( String[] args )
    {
        new x23( args );
    }
}

//--------------------------------------------------------------------------
//                              End of x23.java
//--------------------------------------------------------------------------
