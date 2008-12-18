// $Id$
//
//   set/get tester
//
// Copyright (C) 2008 Andrew Ross
// Copyright (C) 2008 Alan Irwin
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

package plplot.examples;

import plplot.core.*;


class x31 {
    PLStream pls = new PLStream();

    x31(String[] args)
    {
	double xmin[] = new double[1], xmax[] = new double[1];
	double ymin[] = new double[1], ymax[] = new double[1];
	double zxmin[] = new double[1], zxmax[] = new double[1];
	double zymin[] = new double[1], zymax[] = new double[1];
	double xmid, ymid;
	double wx[] = new double[1], wy[] = new double[1];
	double mar[] = new double[1], aspect[] = new double[1];
	double jx[] = new double[1], jy[] = new double[1];
	double ori[] = new double[1];  
	int win[] = new int[1], level2[] = new int[1];
	int digmax[] = new int[1], digits[] = new int[1];
	int compression1[] = new int[1], compression2[] = new int[1];
	double xp1[] = new double[1], yp1[] = new double[1];
	double xp2[] = new double[1], yp2[] = new double[1];
	int xleng1[] = new int[1], yleng1[] = new int[1];
	int xoff1[] = new int[1], yoff1[] = new int[1];
	int xleng2[] = new int[1], yleng2[] = new int[1];
	int xoff2[] = new int[1], yoff2[] = new int[1];
	int fam1[] = new int[1], num1[] = new int[1], bmax1[] = new int[1];
	int fam2[] = new int[1], num2[] = new int[1], bmax2[] = new int[1];
	int r[] = new int[1], g[] = new int[1], b[] = new int[1];
	double a[] = new double[1];
	int r1[] = {0, 255};
	int g1[] = {255, 0};
	int b1[] = {0, 0};
	double a1[] = {1.0, 1.0};
	StringBuffer fnam = new StringBuffer(80);
	int status;

	status = 0;	

	// Parse and process command line arguments

	pls.parseopts(args, PLStream.PL_PARSE_FULL | PLStream.PL_PARSE_NOPROGRAM);

	// Test setting / getting compression parameter across plinit.
	compression1[0] = 100;
	pls.scompression(compression1[0]);

	// Test setting / getting familying parameters across plinit
	fam1[0] = 0;
	num1[0] = 10;
	bmax1[0] = 1000;
	pls.sfam(fam1[0], num1[0], bmax1[0]);

	// Test setting / getting page parameters across plinit
	xp1[0] = 200.;
	yp1[0] = 200.;
	xleng1[0] = 400;
	yleng1[0] = 200;
	xoff1[0] = 10;
	yoff1[0] = 20;
	pls.spage(xp1[0], yp1[0], xleng1[0], yleng1[0], xoff1[0], yoff1[0]);

	// Initialize plplot
	pls.init();

	// Test if device initialization screwed around with the preset
	// compression parameter.
	pls.gcompression(compression2);
	System.out.println("Output various PLplot parameters");
	System.out.println("compression parameter = " + compression2[0] );
	if (compression2[0] != compression1[0]) {
	    System.err.println("plgcompression test failed" );
	    status = 1;
	}

	// Test if device initialization screwed around with any of the
	// preset familying values.
	pls.gfam(fam2, num2, bmax2);
	System.out.println("family parameters: fam, num, bmax = " + 
			   fam2[0] + " " + num2[0] + " " + bmax2[0] );
	if (fam2[0] != fam1[0] || num2[0] != num1[0] || bmax2[0] != bmax1[0]) {
	    System.err.println("plgfam test failed" );
	    status = 1;
	}

	// Test if device initialization screwed around with any of the
	// preset page values.
	pls.gpage(xp2, yp2, xleng2, yleng2, xoff2, yoff2);
	System.out.println("page parameters: xp, yp, xleng, yleng, xoff, yoff = " + xp2[0] + " " + yp2[0] + " " + xleng2[0] + " " + yleng2[0] + " " + xoff2[0] + " " + yoff2[0] );
	if (xp2[0] != xp1[0] || yp2[0] != yp1[0] || 
	    xleng2[0] != xleng1[0] || yleng2[0] != yleng1[0] || 
	    xoff2[0] != xoff1[0] || yoff2[0] != yoff1[0] ) {
	    System.err.println("plgpage test failed" );
	    status = 1;
	}


	// Exercise plscolor, plscol0, plscmap1, and plscmap1a to make sure
	// they work without any obvious error messages.
	pls.scolor(1);
	pls.scol0(1, 255, 0, 0);
	pls.scmap1(r1,g1,b1);
	pls.scmap1a(r1,g1,b1,a1);
  
	pls.glevel(level2);
	System.out.println("level parameter = " + level2[0] );
	if (level2[0] != 1) {
	    System.err.println("plglevel test failed." );
	    status = 1;
	}

	pls.adv(0);
	pls.vpor(0.01, 0.99, 0.02, 0.49);
	pls.gvpd(xmin, xmax, ymin, ymax);
	System.out.println("plvpor: xmin, xmax, ymin, ymax = " + xmin[0] + " " + xmax[0] + " " + ymin[0] + " " + ymax[0] );
	if (xmin[0] != 0.01 || xmax[0] != 0.99 || ymin[0] != 0.02 || ymax[0] != 0.49) {
	    System.err.println("plgvpd test failed" );
	    status = 1;
	}
	xmid = 0.5*(xmin[0]+xmax[0]);
	ymid = 0.5*(ymin[0]+ymax[0]);

	pls.wind(0.2, 0.3, 0.4, 0.5);
	pls.gvpw(xmin, xmax, ymin, ymax);
	System.out.println("plwind: xmin, xmax, ymin, ymax = " + xmin[0] + " " + xmax[0] + " " + ymin[0] + " " + ymax[0] );
	if (xmin[0] != 0.2 || xmax[0] != 0.3 || ymin[0] != 0.4 || ymax[0] != 0.5) {
	    System.err.println("plgvpw test failed" );
	    status = 1;
	}

	// Get world coordinates for middle of viewport
	pls.calc_world(xmid,ymid,wx,wy,win);
	System.out.println("world parameters: wx, wy, win = " + wx[0] + " " + wy[0] + " " + win[0] );
	if (Math.abs(wx[0]-0.5*(xmin[0]+xmax[0]))>1.0E-5 || Math.abs(wy[0]-0.5*(ymin[0]+ymax[0]))>1.0E-5) {
	    System.err.println("plcalc_world test failed" );
	    status = 1;    
	}

	// Retrieve and print the name of the output file (if any).
	// This goes to stderr not stdout since it will vary between tests and 
	// we want stdout to be identical for compare test.
	pls.gfnam(fnam);
	if (fnam.length() == 0) {
	    System.out.println("No output file name is set" );
	}
	else {
	    System.out.println("Output file name read" );
	}
	System.err.println("Output file name is " + fnam );

	// Set and get the number of digits used to display axis labels
	// Note digits is currently ignored in pls[xyz]ax and 
	// therefore it does not make sense to test the returned 
	// value
	pls.sxax(3,0);
	pls.gxax(digmax,digits);
	System.out.println("x axis parameters: digmax, digits = " + digmax[0] + " " + digits[0] );
	if (digmax[0] != 3) {
	    System.err.println("plgxax test failed" );
	    status = 1;
	}

	pls.syax(4,0);
	pls.gyax(digmax,digits);
	System.out.println("y axis parameters: digmax, digits = " + digmax[0] + " " + digits[0] );
	if (digmax[0] != 4) {
	    System.err.println("plgyax test failed" );
	    status = 1;
	}

	pls.szax(5,0);
	pls.gzax(digmax,digits);
	System.out.println("z axis parameters: digmax, digits = " + digmax[0] + " " + digits[0] );
	if (digmax[0] != 5) {
	    System.err.println("plgzax test failed" );
	    status = 1;
	}

	pls.sdidev(0.05, PLStream.PL_NOTSET, 0.1, 0.2);
	pls.gdidev(mar, aspect, jx, jy);
	System.out.println("device-space window parameters: mar, aspect, jx, jy = " + mar[0] + " " + aspect[0] + " " + jx[0] + " " + jy[0] );
	if (mar[0] != 0.05 || jx[0] != 0.1 || jy[0] != 0.2) {
	    System.err.println("plgdidev test failed" );
	    status = 1;
	}

	pls.sdiori(1.0);
	pls.gdiori(ori);
	System.out.println("ori parameter = " + ori[0] );
	if (ori[0] != 1.0) {
	    System.err.println("plgdiori test failed" );
	    status = 1;
	}

	pls.sdiplt(0.1, 0.2, 0.9, 0.8);
	pls.gdiplt(xmin, ymin, xmax, ymax);
	System.out.println("plot-space window parameters: xmin, ymin, xmax, ymax = " +  xmin[0] + " " +  ymin[0] + " " + xmax[0] + " " + ymax[0] );
	if (xmin[0] != 0.1 || xmax[0] != 0.9 || ymin[0] != 0.2 || ymax[0] != 0.8) {
	    System.err.println("plgdiplt test failed" );
	    status = 1;
	}

	pls.sdiplz(0.1, 0.1, 0.9, 0.9);
	pls.gdiplt(zxmin, zymin, zxmax, zymax);
	System.out.println("zoomed plot-space window parameters: xmin, ymin, xmax, ymax = " + zxmin[0] + " " + zymin[0] + " " + zxmax[0] + " " + zymax[0] );
	if ( Math.abs(zxmin[0] -(xmin[0] + (xmax[0]-xmin[0])*0.1)) > 1.0E-5 || 
	     Math.abs(zxmax[0] -(xmin[0]+(xmax[0]-xmin[0])*0.9)) > 1.0E-5 || 
	     Math.abs(zymin[0] -(ymin[0]+(ymax[0]-ymin[0])*0.1)) > 1.0E-5 || 
	     Math.abs(zymax[0] -(ymin[0]+(ymax[0]-ymin[0])*0.9)) > 1.0E-5 ) {
	    System.err.println("plsdiplz test failed" );
	    status = 1;
	}

	pls.scolbg(10,20,30);
	pls.gcolbg(r, g, b);
	System.out.println("background colour parameters: r, g, b = " + r[0] + " " +  g[0] + " " + b[0] );
	if (r[0] != 10 || g[0] != 20 || b[0] != 30) {
	    System.err.println("plgcolbg test failed" );
	    status = 1;
	}

	pls.scolbga(20,30,40,0.5);
	pls.gcolbga(r, g, b, a);
	System.out.println("background/transparency colour parameters: r, g, b, a = " + r[0] + " " +  g[0] + " " + b[0] + " " + a[0] );
	if (r[0] != 20 || g[0] != 30 || b[0] != 40 || a[0] != 0.5) {
	    System.err.println("plgcolbga test failed" );
	    status = 1;
	}
	pls.end();
	System.exit(status);
    }

    public static void main( String[] args )
    {
	new x31( args );
    }

};
