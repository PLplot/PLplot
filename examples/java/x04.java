//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 4 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x04 {

    PLStream pls;
    final float PI = 3.1415927f;

    public static void main( String[] args ) 
    {
        x04 x = new x04( args );
    }

    public x04( String[] args )
    {
        pls = new PLStream();

        System.out.println( "x04 not implemented yet." );

    // Parse and process command line arguments.

        pls.ParseOpts( args, pls.PL_PARSE_FULL );

    // Initialize plplot.

        pls.init();
        pls.font(2);

    // Make log plots using two different styles.

        plot1(0);
        plot1(1);

        pls.end();
    }

// Log-linear plot.

    void plot1( int type )
    {
        int i;
        float[] freql = new float[101];
        float[] ampl  = new float[101];
        float[] phase = new float[101];
        float f0, freq;

        pls.adv(0);

    // Set up data for log plot.

        f0 = 1.0f;
        for (i = 0; i <= 100; i++) {
            freql[i] = -2.0f + i / 20.0f;
            freq = (float) Math.pow(10.0, freql[i]);
        // Unbelievably, Java has no log10() that I can find...
            ampl[i] = (float) (20.0 * Math.log(1.0 / Math.sqrt(1.0 + Math.pow((freq / f0), 2.))) / Math.log(10));
            phase[i] = (float) (-(180.0 / PI) * Math.atan(freq / f0));
        }

        pls.vpor(0.15, 0.85, 0.1, 0.9);
        pls.wind(-2.0, 3.0, -80.0, 0.0);

    // Try different axis and labelling styles.

        pls.col0(1);
        switch (type) {
        case 0:
            pls.box("bclnst", 0.0, 0, "bnstv", 0.0, 0);
            break;
        case 1:
            pls.box("bcfghlnst", 0.0, 0, "bcghnstv", 0.0, 0);
            break;
        }

    // Plot ampl vs freq.

        pls.col0(2);
        pls.line(101, freql, ampl);
        pls.col0(1);
        pls.ptex(1.6, -30.0, 1.0, -20.0, 0.5, "-20 dB/decade");

    // Put labels on.

        pls.col0(1);
        pls.mtex("b", 3.2, 0.5, 0.5, "Frequency");
        pls.mtex("t", 2.0, 0.5, 0.5, "Single Pole Low-Pass Filter");
        pls.col0(2);
        pls.mtex("l", 5.0, 0.5, 0.5, "Amplitude (dB)");

    // For the gridless case, put phase vs freq on same plot.

        if (type == 0) {
            pls.col0(1);
            pls.wind(-2.0, 3.0, -100.0, 0.0);
            pls.box("", 0.0, 0, "cmstv", 30.0, 3);
            pls.col0(3);
            pls.line(101, freql, phase);
            pls.col0(3);
            pls.mtex("r", 5.0, 0.5, 0.5, "Phase shift (degrees)");
        }
    }
}

//---------------------------------------------------------------------------//
//                              End of x04.java
//---------------------------------------------------------------------------//
