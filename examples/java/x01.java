//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 1 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x01 {

    float[] x, y, xs, ys;
    float xscale, yscale, xoff, yoff;
    PLStream pls;

    public static void main( String[] args ) 
    {
        x01 x = new x01();
        x.run();
    }

    x01()
    {
    // Java lacks static arrays.
        x = new float[101];
        y = new float[101];

        xs = new float[6];
        ys = new float[6];
    }

    public void run()
    {
        pls = new PLStream();

    // Initialize PLplot.
        pls.init();

    // Select the multi-stroke font.
        pls.fontld( 1 );

    // Set up the data
    // Original case

        xscale = 6.f;
        yscale = 1.f;
        xoff = 0.f;
        yoff = 0.f;

    // Do a plot
        plot1();

    // Don't forget to call plend() to finish off!
        pls.end();
    }

    void plot1()
    {
        int i;
        float xmin, xmax, ymin, ymax;

        for( i=0; i < 60; i++ )
        {
            x[i] = xoff + xscale * (i + 1) / 60.0f;
            y[i] = yoff + yscale * (float) Math.pow(x[i], 2.f);
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

        pls.col0(1);
        pls.env( xmin, xmax, ymin, ymax, 0, 0 );
        pls.col0(2);
        pls.lab( "(x)", "(y)", "#frPLplot Example 1 - y=x#u2" );

    // Plot the data points.

        pls.col0(4);
        pls.poin( 6, xs, ys, 9 );

    // Draw the line through the data.

        pls.col0(3);
        pls.line(60, x, y);
    }
}

//---------------------------------------------------------------------------//
//                              End of x01.java
//---------------------------------------------------------------------------//

