//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Copyright (C) 2001  Geoffrey Furnish
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
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Implementation of PLplot example 19 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class Mapform19 implements PLCallback {

    public void mapform(double[] x, double[] y) {
	int i;
	double xp, yp, radius;
	for (i=0;i< x.length;i++) {
	    radius = 90.0-y[i];
	    xp = radius * Math.cos(x[i] * Math.PI / 180.0);
	    yp = radius * Math.sin(x[i] * Math.PI / 180.0);
	    x[i] = xp;
	    y[i] = yp;
	}
        
	
    }

}

class x19 {

    PLStream pls = new PLStream();

    public static void main( String[] args ) 
    {
        new x19(args);
    }

    public x19 (String[] args) 
    {
	double minx, maxx, miny,maxy;
        PLCallback nullCallback = null;

	// Parse and process command line arguments.
	pls.parseopts( args, PLStream.PL_PARSE_FULL | PLStream.PL_PARSE_NOPROGRAM );

	// Longitude (x) and latitude (y)
	
	miny = -70;
	maxy = 80;
	
	// Initialize PLplot.
	pls.init();
	// Cartesian plots
	// Most of world
	
	minx = 190;
	maxx = 190+360;
	
	pls.col0(1);
	pls.env(minx, maxx, miny, maxy, 1, -1);
	pls.map(nullCallback, "usaglobe", minx, maxx, miny, maxy);

	// The Americas

	minx = 190;
	maxx = 340;

	pls.col0(1);
	pls.env(minx, maxx, miny, maxy, 1, -1);
	pls.map(nullCallback, "usaglobe", minx, maxx, miny, maxy);

	// Polar, Northern hemisphere
        
        // Create callback object containing mapform function
	Mapform19 mapform19 = new Mapform19();

	minx = 0;
	maxx = 360;

	pls.env(-75., 75., -75., 75., 1, -1);
	pls.map(mapform19,"globe", minx, maxx, miny, maxy);
	
	pls.lsty(2);
	pls.meridians(mapform19,10.0, 10.0, 0.0, 360.0, -10.0, 80.0);

        pls.end();

    }
}

//---------------------------------------------------------------------------//
//                              End of x19.java
//---------------------------------------------------------------------------//
