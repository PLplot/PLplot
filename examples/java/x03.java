//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 3 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;
import java.text.*;

class x03 {

    PLStream pls;

    public static void main( String[] args ) 
    {
        x03 x = new x03( args );
    }

    public x03( String[] args )
    {
        pls = new PLStream();
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

    // Parse and process command line arguments.

        pls.ParseOpts( args, pls.PL_PARSE_FULL );

    // Initialize plplot.

        pls.init();

    // Set up viewport and window, but do not draw box.

        pls.env(-1.3, 1.3, -1.3, 1.3, 1, -2);
        for (i = 1; i <= 10; i++) {
            for (j = 0; j <= 360; j++) {
                x[j] = 0.1 * i * x0[j];
                y[j] = 0.1 * i * y0[j];
            }

        // Draw circles for polar grid.

            pls.line(361, x, y);
        }

        pls.col0(2);
        for (i = 0; i <= 11; i++) {
            theta = 30.0 * i;
            dx = Math.cos(dtr * theta);
            dy = Math.sin(dtr * theta);

        // Draw radial spokes for polar grid.

            pls.join(0.0, 0.0, dx, dy);
            String text = nf.format(theta);

        // Write labels for angle.

            if (dx >= 0)
                pls.ptex(dx, dy, dx, dy, -0.15, text);
            else
                pls.ptex(dx, dy, -dx, -dy, 1.15, text);
        }

    // Draw the graph.

        for (i = 0; i <= 360; i++) {
            r = Math.sin(dtr * (5 * i));
            x[i] = x0[i] * r;
            y[i] = y0[i] * r;
        }
        pls.col0(3);
        pls.line(361, x, y);

        pls.col0(4);
        pls.mtex("t", 2.0, 0.5, 0.5, "#frPLplot Example 3 - r(#gh)=sin 5#gh");

    // Close the plot at end.

        pls.end();
    }
}

//---------------------------------------------------------------------------//
//                              End of x03.java
//---------------------------------------------------------------------------//
