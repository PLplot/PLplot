//---------------------------------------------------------------------------//
// $Id$
//---------------------------------------------------------------------------//

//---------------------------------------------------------------------------//
// Copyright (C) 2001  Geoffrey Furnish
// Copyright (C) 2001, 2002  Alan W. Irwin
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
// Implementation of PLplot example 13 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;
import java.text.*;

class x13 {

    static String[] text = {"Maurice", "Geoffrey", "Alan", "Rafael", "Vince"};
    static int per[] = {10, 32, 12, 30, 16};

    public static void main( String[] args ) 
    {
        x13 x = new x13( args );
    }

    public x13( String[] args )
    {
       PLStreamc plsdummy = new PLStreamc();
       plplotjavac pls = new plplotjavac();

	int i, j, dthet, theta0, theta1, theta;
	double just, dx, dy;
        double[] x = new double[ 500 ];
	double[] y = new double[ 500 ];
	
        pls.plParseOpts( args, pls.PL_PARSE_FULL | pls.PL_PARSE_NOPROGRAM );

        pls.plinit();
	
	pls.plenv(0., 10., 0., 10., 1, -2);
	pls.plcol0(2);
    // n.b. all theta quantities scaled by 2*pi/500 to be integers to avoid
    // floating point logic problems.
	theta0 = 0;
	dthet = 1;
	for (i = 0; i <= 4; i++) {
            j = 0;
            x[j] = 5.;
            y[j++] = 5.;
        // n.b. the theta quantities multiplied by 2*pi/500 afterward so
        // in fact per is interpreted as a percentage.
            theta1 = theta0 + 5 * per[i];
            if (i == 4)
                theta1 = 500;
            for (theta = theta0; theta <= theta1; theta += dthet) {
                x[j] = 5 + 3 * Math.cos((2.*Math.PI/500.)*theta);
                y[j++] = 5 + 3 * Math.sin((2.*Math.PI/500.)*theta);
            }
            pls.plcol0(i + 1);
            pls.plpsty((i + 3) % 8 + 1);
	    // Make array of correct size to copy to.
	    double [] xsized = new double[j];
	    double [] ysized = new double[j];
	    System.arraycopy(x, 0, xsized, 0, j);
	    System.arraycopy(y, 0, ysized, 0, j);
            pls.plfill(xsized, ysized);
            pls.plcol0(1);
            pls.plline(xsized, ysized);
            just = (2.*Math.PI/500.)*(theta0 + theta1)/2.;
            dx = .25 * Math.cos(just);
            dy = .25 * Math.sin(just);
            if ((theta0 + theta1)  < 250 || (theta0 + theta1) > 750) 
                just = 0.;
            else 
                just = 1.;

            pls.plptex((x[j / 2] + dx), (y[j / 2] + dy), 1.0, 0.0, just, text[i]);
            theta0 = theta - dthet;
	}
	pls.plfont(2);
	pls.plschr(0., 1.3);
	pls.plptex(5.0, 9.0, 1.0, 0.0, 0.5, "Percentage of Sales");
        pls.plend();
    }
}

//---------------------------------------------------------------------------//
//                              End of x13.java
//---------------------------------------------------------------------------//
