//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Copyright (C) 2001  Geoffrey Furnish
// Copyright (C) 2002  Alan W. Irwin
// This file is part of PLplot.
//
// This file is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This file is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with the file; if not, write to the Free Software
//Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Implementation of PLplot example 18 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;
import java.text.*;
class x18 {


   final int NPTS = 1000;
   static int opt[] = { 1, 0, 1, 0 };
   static double alt[] = {20.0, 35.0, 50.0, 65.0};
   static double az[] = {30.0, 40.0, 50.0, 60.0};
   PLStreamc plsdummy = new PLStreamc();
   plplotjavac pls = new plplotjavac();

   double THETA(int a)
     {
	return 2. * Math.PI * (double) a/20.;
     }
   double PHI (int a)
     {
	return Math.PI * (double) a/20.1;
     }
   void test_poly(int k)
     {
	int i, j;
	int [][] draw = { 
	     { 1, 1, 1, 1 },
	     { 1, 0, 1, 0 },
	     { 0, 1, 0, 1 },
	     { 1, 1, 0, 0 }
	};

	double [] x = new double [5];
	double [] y = new double [5];
	double [] z = new double [5];

	pls.pladv(0);
	pls.plvpor(0.0, 1.0, 0.0, 0.9);
	pls.plwind(-1.0, 1.0, -0.9, 1.1);
	pls.plcol0(1);
	pls.plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, alt[k], az[k]);
	pls.plbox3("bnstu", "x axis", 0.0, 0,
		 "bnstu", "y axis", 0.0, 0,
		 "bcdmnstuv", "z axis", 0.0, 0);
	
	pls.plcol0(2);

	// x = r sin(phi) cos(theta)
	// y = r sin(phi) sin(theta)
	// z = r cos(phi)
	// r = 1 :=)

	for( i=0; i < 20; i++ ) {
	   for( j=0; j < 20; j++ ) {
	      x[0] = Math.sin( PHI(j) ) * Math.cos( THETA(i) );
	      y[0] = Math.sin( PHI(j) ) * Math.sin( THETA(i) );
	      z[0] = Math.cos( PHI(j) );
	      
	      x[1] = Math.sin( PHI(j+1) ) * Math.cos( THETA(i) );
	      y[1] = Math.sin( PHI(j+1) ) * Math.sin( THETA(i) );
	      z[1] = Math.cos( PHI(j+1) );
	      
	      x[2] = Math.sin( PHI(j+1) ) * Math.cos( THETA(i+1) );
	      y[2] = Math.sin( PHI(j+1) ) * Math.sin( THETA(i+1) );
	      z[2] = Math.cos( PHI(j+1) );
	    
	      x[3] = Math.sin( PHI(j) ) * Math.cos( THETA(i+1) );
	      y[3] = Math.sin( PHI(j) ) * Math.sin( THETA(i+1) );
	      z[3] = Math.cos( PHI(j) );
	      
	      x[4] = Math.sin( PHI(j) ) * Math.cos( THETA(i) );
	      y[4] = Math.sin( PHI(j) ) * Math.sin( THETA(i) );
	      z[4] = Math.cos( PHI(j) );

	      pls.plpoly3(x, y, z, draw[k], 1 );
	   }
	}

	pls.plcol0(3);
	pls.plmtex("t", 1.0, 0.5, 0.5, "unit radius sphere" );
     }
   // Does a series of 3-d plots for a given data set, with different
   // viewing options in each plot.
   public static void main( String[] args ) 
     {
	x18 x = new x18( args );
     }
   
   public x18( String[] args )
     {
	NumberFormat nf = NumberFormat.getNumberInstance();
	
	int i, j, k;
	double r;

        // Parse and process command line arguments.
	pls.plParseOpts( args, pls.PL_PARSE_FULL | pls.PL_PARSE_NOPROGRAM );

	// Initialize plplot.
	pls.plinit();

	for( k=0; k < 4; k++ )
	  test_poly(k);

	double[] x = new double[NPTS];
	double[] y = new double[NPTS];
	double[] z = new double[NPTS];

	// From the mind of a sick and twisted physicist...

	for (i = 0; i < NPTS; i++) {
	   z[i] = -1. + 2. * i / NPTS;

	   // Pick one ...

	   // r    = 1. - ( (double) i / (double) NPTS );
	   r = z[i];

	   x[i] = r * Math.cos( 2. * Math.PI * 6. * i / NPTS );
	   y[i] = r * Math.sin( 2. * Math.PI * 6. * i / NPTS );
	}

	for (k = 0; k < 4; k++) {
	   pls.pladv(0);
	   pls.plvpor(0.0, 1.0, 0.0, 0.9);
	   pls.plwind(-1.0, 1.0, -0.9, 1.1);
	   pls.plcol0(1);
	   pls.plw3d(1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, -1.0, 1.0, alt[k], az[k]);
	   pls.plbox3("bnstu", "x axis", 0.0, 0,
		    "bnstu", "y axis", 0.0, 0,
		    "bcdmnstuv", "z axis", 0.0, 0);
	   
	   pls.plcol0(2);
	   
	   if (opt[k]>0)
	     pls.plline3( x, y, z );
	   else
	     pls.plpoin3( x, y, z, 1 );
	   
	   pls.plcol0(3);
	   String title =  "#frPLplot Example 18 - Alt=" + 
	     nf.format((int) alt[k]) +
	     ", Az=" + 
	     nf.format((int) az[k]);
	   pls.plmtex("t", 1.0, 0.5, 0.5, title);
	}
	
	pls.plend();
     }
   
}

//---------------------------------------------------------------------------//
//                              End of x18.java
//---------------------------------------------------------------------------//
