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
	double wx[] = new double[1], wy[] = new double[1];
	double mar[] = new double[1], aspect[] = new double[1];
	double jx[] = new double[1], jy[] = new double[1];
	double ori[] = new double[1];  
	int win[] = new int[1], level[] = new int[1];
	int digmax[] = new int[1], digits[] = new int[1];
	int compression[] = new int[1];
	double xp[] = new double[1], yp[] = new double[1];
	double xp2, yp2;
	int xleng[] = new int[1], yleng[] = new int[1];
	int xoff[] = new int[1], yoff[] = new int[1];
	int xleng2, yleng2;
	int fam1[] = new int[1], num1[] = new int[1], bmax1[] = new int[1];
	int fam[] = new int[1], num[] = new int[1], bmax[] = new int[1];
	int r[] = new int[1], g[] = new int[1], b[] = new int[1];
	double a[] = new double[1];
	int r1[] = {0, 255};
	int g1[] = {255, 0};
	int b1[] = {0, 0};
	double a1[] = {1.0, 1.0};
	StringBuffer fnam = new StringBuffer(80);

	// Parse and process command line arguments

	pls.parseopts(args, PLStream.PL_PARSE_FULL | PLStream.PL_PARSE_NOPROGRAM);

	// Test setting / getting page size 
	pls.gpage(xp, yp, xleng, yleng, xoff, yoff);
	xp2 = xp[0]*0.9;
	yp2 = yp[0]*0.9;
	xleng2 = (int)(xleng[0]*0.9);
	yleng2 = (int)(yleng[0]*0.9);
	pls.spage(xp2, yp2, xleng2, yleng2, xoff[0], yoff[0]);
	pls.gpage(xp, yp, xleng, yleng, xoff, yoff);
	if (xp[0] != xp2 || yp[0] != yp2 || xleng[0] != xleng2 || yleng[0] != yleng2) {
	    System.err.println("plgpage test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.scompression(1);
	pls.gcompression(compression);
	if (compression[0] != 1) {
	    System.err.println("plgcompression test failed");
	    pls.end();
	    System.exit(1);
	}

	// Test family functions - need to restore defaults to ensure 
	// that test scripts work ok.
	pls.gfam(fam, num, bmax);
	pls.sfam(1,1,100000);
	pls.gfam(fam1, num1, bmax1);
	if (fam1[0] != 1 || num1[0] != 1 || bmax1[0] != 100000) {
	    System.err.println("plgfam test failed");
	    pls.end();
	    System.exit(1);
	}
	pls.sfam(fam[0],num[0],bmax[0]);

	// Initialize plplot

	pls.init();

	pls.scolor(1);

	pls.scol0(1, 255, 0, 0);

	pls.scmap1(r1,g1,b1);
	pls.scmap1a(r1,g1,b1,a1);
  
	pls.glevel(level);
	if (level[0] != 1) {
	    System.err.println("plglevel test failed. Level is "+level+", but 1 expected.");
	    pls.end();
	    System.exit(1);
	}

	pls.adv(0);
	pls.vpor(0.0, 1.0, 0.0, 1.0);

	pls.wind(0.2, 0.3, 0.4, 0.5);
	pls.gvpw(xmin, xmax, ymin, ymax);
	if (xmin[0] != 0.2 || xmax[0] != 0.3 || ymin[0] != 0.4 || ymax[0] != 0.5) {
	    System.err.println("plgvpw test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.gvpd(xmin, xmax, ymin, ymax);
	if (xmin[0] != 0.0 || xmax[0] != 1.0 || ymin[0] != 0.0 || ymax[0] != 1.0) {
	    System.err.println("plgvpd test failed");
	    pls.end();
	    System.exit(1);
	}

	// Get world coordinates for 0.5,0.5 which is in the middle of 
	// the window
	pls.calc_world(0.5,0.5,wx,wy,win);
	if (Math.abs(wx[0]-0.25)>1.0E-5 || Math.abs(wy[0]-0.45)>1.0E-5) {
	    System.err.println("plcalc_world test failed");
	    pls.end();
	    System.exit(1);    
	}

	// Retrieve and print the name of the output file (if any)
	pls.gfnam(fnam);
	System.out.println("Output file name is "+fnam);

	// Set and get the number of digits used to display axis labels
	// Note digits is currently ignored in pls[xyz]ax and 
	// therefore it does not make sense to test the returned 
	// value
	pls.sxax(3,0);
	pls.gxax(digmax,digits);
	if (digmax[0] != 3) {
	    System.err.println("plgxax test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.syax(3,0);
	pls.gyax(digmax,digits);
	if (digmax[0] != 3) {
	    System.err.println("plgyax test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.szax(3,0);
	pls.gzax(digmax,digits);
	if (digmax[0] != 3) {
	    System.err.println("plgzax test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.sdidev(0.05, PLStream.PL_NOTSET, 0.0, 0.0);
	pls.gdidev(mar, aspect, jx, jy);
	if (mar[0] != 0.05 || jx[0] != 0.0 || jy[0] != 0.0) {
	    System.err.println("plgdidev test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.sdiori(1.0);
	pls.gdiori(ori);
	if (ori[0] != 1.0) {
	    System.err.println("plgdiori test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.sdiplt(0.1, 0.1, 0.9, 0.9);
	pls.gdiplt(xmin, ymin, xmax, ymax);
	if (xmin[0] != 0.1 || xmax[0] != 0.9 || ymin[0] != 0.1 || ymax[0] != 0.9) {
	    System.err.println("plgdiplt test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.sdiplz(0.1, 0.1, 0.9, 0.9);
	pls.gdiplt(xmin, ymin, xmax, ymax);
	if (xmin[0] != 0.1+0.8*0.1 || xmax[0] != 0.1+0.8*0.9 || ymin[0] != 0.1+0.8*0.1 || ymax[0] != 0.1+0.8*0.9) {
	    System.err.println("plsdiplz test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.scolbg(0,0,0);
	pls.gcolbg(r, g, b);
	if (r[0] != 0 || g[0] != 0 || b[0] != 0) {
	    System.err.println("plgcolbg test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.scolbga(0,0,0,1.0);
	pls.gcolbga(r, g, b, a);
	if (r[0] != 0 || g[0] != 0 || b[0] != 0 || a[0] != 1.0) {
	    System.err.println("plgcolbga test failed");
	    pls.end();
	    System.exit(1);
	}

	pls.end();
	System.exit(0);
    }

    public static void main( String[] args )
    {
	new x31( args );
    }

};
