//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Copyright (C) 2004  Alan W. Irwin
//

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
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Implementation of PLplot example 14 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;
import java.text.*;

class x14 {

    double xscale, yscale, xoff, yoff;
    PLStreamc plsdummy = new PLStreamc();
    plplotjavac pls = new plplotjavac();

   public static void main( String[] args ) 
     {
        x14 x = new x14( args );
     }
   
   public x14( String[] args )
     {
	String geometry_master = "500x410+100+200";
	String geometry_slave = "500x410+650+200";

    // Parse and process command line arguments.

        pls.plParseOpts( args, pls.PL_PARSE_FULL|pls.PL_PARSE_NOPROGRAM );

	StringBuffer driver = new StringBuffer(80);
	
	pls.plgdev(driver);
	String sdriver = new String(driver);
	System.out.println("Demo of multiple output streams via the " + sdriver +  " driver.");
	System.out.println("Running with the second stream as slave to the first.");
	System.out.println("");

	// Set up first stream

	pls.plsetopt("geometry", geometry_master);

	pls.plsdev(sdriver);
	pls.plssub(2, 2);
	pls.plinit();
	
	// Start next stream
	
	pls.plsstrm(1);
	
	// Turn off pause to make this a slave (must follow master)
	
	pls.plsetopt("geometry", geometry_slave);
	pls.plspause(0);
	pls.plsdev(sdriver);
	pls.plinit();
	
	//Set up the data & plot
	// Original case
	
	pls.plsstrm(0);
	
        xscale = 6.;
        yscale = 1.;
        xoff = 0.;
        yoff = 0.;
        plot1();

	// Set up the data & plot
	
	xscale = 1.;
	yscale = 1.e+6;
	plot1();

	// Set up the data & plot
	
	xscale = 1.;
	yscale = 1.e-6;
	int digmax = 2;
	pls.plsyax(digmax, 0);
	plot1();
	
	// Set up the data & plot
	
	xscale = 1.;
	yscale = 0.0014;
	yoff = 0.0185;
	digmax = 5;
	pls.plsyax(digmax, 0);
	plot1();
	
	// To slave
	// The pleop() ensures the eop indicator gets lit.
	
	pls.plsstrm(1);
	plot4();
	pls.pleop();
	
	// Back to master
	
	pls.plsstrm(0);
	plot2();
	plot3();
	
	// To slave
	
	pls.plsstrm(1);
	plot5();
	pls.pleop();
	
	// Back to master to wait for user to advance
	
	pls.plsstrm(0);
	pls.pleop();
	
	// Call plend to finish off.

        pls.plend();
    }

    void plot1()
    {
        int i;
        double xmin, xmax, ymin, ymax;
        double x[] = new double[60];
        double y[] = new double[60];
        double xs[] = new double[6];
        double ys[] = new double[6];

        for( i=0; i < 60; i++ )
        {
            x[i] = xoff + xscale * (i + 1) / 60.0;
            y[i] = yoff + yscale * Math.pow(x[i], 2.);
        }

        xmin = x[0];
        xmax = x[59];
        ymin = y[0];
        ymax = y[59];

        for( i=0; i < 6; i++ )
        {
            xs[i] = x[i * 10 + 3];
            ys[i] = y[i * 10 + 3];
        }

    // Set up the viewport and window using PLENV. The range in X is 0.0 to
    // 6.0, and the range in Y is 0.0 to 30.0. The axes are scaled separately
    // (just = 0), and we just draw a labelled box (axis = 0).

        pls.plcol0(1);
        pls.plenv( xmin, xmax, ymin, ymax, 0, 0 );
        pls.plcol0(6);
        pls.pllab( "(x)", "(y)", "#frPLplot Example 1 - y=x#u2" );

    // Plot the data points.

        pls.plcol0(9);
        pls.plpoin( xs, ys, 9 );

    // Draw the line through the data.

        pls.plcol0(4);
        pls.plline(x, y);
        pls.plflush();
    }

    void plot2()
    {
        int i;
        double x[] = new double[100];
        double y[] = new double[100];

    // Set up the viewport and window using PLENV. The range in X is -2.0 to
    // 10.0, and the range in Y is -0.4 to 2.0. The axes are scaled
    // separately (just = 0), and we draw a box with axes (axis = 1).

        pls.plcol0(1);
        pls.plenv(-2.0, 10.0, -0.4, 1.2, 0, 1);
        pls.plcol0(2);
        pls.pllab("(x)", "sin(x)/x", "#frPLplot Example 1 - Sinc Function");

    // Fill up the arrays.

        for (i = 0; i < 100; i++) {
            x[i] = (i - 19.0) / 6.0;
            y[i] = 1.0;
            if (x[i] != 0.0)
                y[i] = Math.sin(x[i]) / x[i];
        }

    // Draw the line.

        pls.plcol0(3);
        pls.plline(x, y);
        pls.plflush();
    }

    void plot3()
    {
        int i;
        int space0[] = {};
        int mark0[] = {};
        int space1[] = {1500};
        int mark1[] = {1500};
        double x[] = new double[101];
        double y[] = new double[101];

    // For the final graph we wish to override the default tick intervals,
    // and so do not use plenv().

        pls.pladv(0);

    // Use standard viewport, and define X range from 0 to 360 degrees, Y
    // range from -1.2 to 1.2.

        pls.plvsta();
        pls.plwind( 0.0, 360.0, -1.2, 1.2 );

    // Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.

        pls.plcol0(1);
        pls.plbox("bcnst", 60.0, 2, "bcnstv", 0.2, 2);

    // Superimpose a dashed line grid, with 1.5 mm marks and spaces. 
    // plstyl expects a pointer!

        pls.plstyl(mark1, space1);
        pls.plcol0(2);
        pls.plbox("g", 30.0, 0, "g", 0.2, 0);
        pls.plstyl(mark0, space0);

        pls.plcol0(3);
        pls.pllab( "Angle (degrees)", "sine",
                 "#frPLplot Example 1 - Sine function" );

        for (i = 0; i < 101; i++) {
            x[i] = 3.6 * i;
            y[i] = Math.sin(x[i] * Math.PI / 180.0);
        }

        pls.plcol0(4);
        pls.plline(x, y);
        pls.plflush();
    }

    void plot4()
    {
        NumberFormat nf = NumberFormat.getNumberInstance();

        int i, j;
        double dtr, theta, dx, dy, r;

        double[] x0 = new double[361];
        double[] y0 = new double[361];
        double[] x  = new double[361];
        double[] y  = new double[361];

        dtr = Math.PI / 180.0;
        for( i = 0; i <= 360; i++ )
        {
            x0[i] = Math.cos(dtr * i);
            y0[i] = Math.sin(dtr * i);
        }


    // Set up viewport and window, but do not draw box.

        pls.plenv(-1.3, 1.3, -1.3, 1.3, 1, -2);
        for (i = 1; i <= 10; i++) {
            for (j = 0; j <= 360; j++) {
                x[j] = 0.1 * i * x0[j];
                y[j] = 0.1 * i * y0[j];
            }

        // Draw circles for polar grid.

            pls.plline(x, y);
        }

        pls.plcol0(2);
        for (i = 0; i <= 11; i++) {
            theta = 30.0 * i;
            dx = Math.cos(dtr * theta);
            dy = Math.sin(dtr * theta);

        // Draw radial spokes for polar grid.

            pls.pljoin(0.0, 0.0, dx, dy);
            String text = nf.format(theta);

        // Write labels for angle.

	//Slightly off zero to avoid floating point logic flips at 90 and 270 deg.
            if (dx >= -0.00001)
                pls.plptex(dx, dy, dx, dy, -0.15, text);
            else
                pls.plptex(dx, dy, -dx, -dy, 1.15, text);
        }

    // Draw the graph.

        for (i = 0; i <= 360; i++) {
            r = Math.sin(dtr * (5 * i));
            x[i] = x0[i] * r;
            y[i] = y0[i] * r;
        }
        pls.plcol0(3);
        pls.plline(x, y);

        pls.plcol0(4);
        pls.plmtex("t", 2.0, 0.5, 0.5, "#frPLplot Example 3 - r(#gh)=sin 5#gh");
        pls.plflush();
    }

    static final int XPTS = 35;
    static final int YPTS = 46;
    static final double XSPA =  2./(XPTS-1);
    static final double YSPA =  2./(YPTS-1);
    final double clevel[] = {-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.};
// Transformation function
    final double tr[] = {XSPA, 0.0, -1.0, 0.0, YSPA, -1.0};
   
    void plot5()
    {
        int i, j;

        double[][] xg0 = new double[XPTS][YPTS];
        double[][] yg0 = new double[XPTS][YPTS];
        double[][] z = new double[XPTS][YPTS];
        double[][] w = new double[XPTS][YPTS];
	
        double xx, yy, argx, argy, distort;
	final int[] mark = {1500};
        final int[] space = {1500};
	final int[] mark0 = {};
        final int[] space0 = {};
	
    /* Set up function arrays */
       
	for (i = 0; i < XPTS; i++) {
            xx = (double) (i - (XPTS / 2)) / (double) (XPTS / 2);
            for (j = 0; j < YPTS; j++) {
                yy = (double) (j - (YPTS / 2)) / (double) (YPTS / 2) - 1.0;
                z[i][j] = xx * xx - yy * yy;
                w[i][j] = 2 * xx * yy;
            }
	}

    /* Set up grids */

	
	for (i = 0; i < XPTS; i++) {
            for (j = 0; j < YPTS; j++) {
            // Replacement for mypltr of x09c.c
                xx = tr[0] * i + tr[1] * j + tr[2];
                yy = tr[3] * i + tr[4] * j + tr[5];
		
            // Note these are one-dimensional because of arrangement of
            // zeros in the final tr definition above.
	    // But I haven't found out yet, how with swig to overload
	    // one- and two-dimensional array arguments so for now make 
	    // xg0 --> yg1 two-dimensional.
                xg0[i][j] = xx;
                yg0[i][j] = yy;
            }
	}


    // Plot using scaled identity transform used to create xg0 and yg0

	pls.plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
	pls.plcol0(2);
        pls.plcont(z, 1, XPTS, 1, YPTS, clevel, xg0, yg0 );
	pls.plstyl(mark, space);
	pls.plcol0(3);
        pls.plcont(w, 1, XPTS, 1, YPTS, clevel, xg0, yg0 );
	pls.plstyl(mark0, space0);
	pls.plcol0(1);
	pls.pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");
        pls.plflush();
    }
}

//---------------------------------------------------------------------------//
//                              End of x14.java
//---------------------------------------------------------------------------//
