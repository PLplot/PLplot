//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 4 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

import java.lang.Math;

class x04 {

   PLStreamc plsdummy = new PLStreamc();
   plplotjavac pls = new plplotjavac();

    public static void main( String[] args ) 
    {
        x04 x = new x04( args );
    }

    public x04( String[] args )
    {
    // Parse and process command line arguments.

        pls.plParseOpts( args, pls.PL_PARSE_FULL | pls.PL_PARSE_NOPROGRAM );

    // Initialize plplot.

        pls.plinit();
        pls.plfont(2);

    // Make log plots using two different styles.

        plot1(0);
        plot1(1);

        pls.plend();
    }

// Log-linear plot.

    void plot1( int type )
    {
        int i;
        double[] freql = new double[101];
        double[] ampl  = new double[101];
        double[] phase = new double[101];
        double f0, freq;

        pls.pladv(0);

    // Set up data for log plot.

        f0 = 1.0;
        for (i = 0; i <= 100; i++) {
            freql[i] = -2.0 + i / 20.0;
            freq = Math.pow(10.0, freql[i]);
        // Unbelievably, Java has no log10() that I can find...
            ampl[i] = 20.0 * Math.log(1.0 / Math.sqrt(1.0 + Math.pow((freq / f0), 2.))) / Math.log(10.);
            phase[i] = -(180.0 / Math.PI) * Math.atan(freq / f0);
        }

        pls.plvpor(0.15, 0.85, 0.1, 0.9);
        pls.plwind(-2.0, 3.0, -80.0, 0.0);

    // Try different axis and labelling styles.

        pls.plcol0(1);
        switch (type) {
        case 0:
            pls.plbox("bclnst", 0.0, 0, "bnstv", 0.0, 0);
            break;
        case 1:
            pls.plbox("bcfghlnst", 0.0, 0, "bcghnstv", 0.0, 0);
            break;
        }

    // Plot ampl vs freq.

        pls.plcol0(2);
        pls.plline(freql, ampl);
        pls.plcol0(1);
        pls.plptex(1.6, -30.0, 1.0, -20.0, 0.5, "-20 dB/decade");

    // Put labels on.

        pls.plcol0(1);
        pls.plmtex("b", 3.2, 0.5, 0.5, "Frequency");
        pls.plmtex("t", 2.0, 0.5, 0.5, "Single Pole Low-Pass Filter");
        pls.plcol0(2);
        pls.plmtex("l", 5.0, 0.5, 0.5, "Amplitude (dB)");

    // For the gridless case, put phase vs freq on same plot.

        if (type == 0) {
            pls.plcol0(1);
            pls.plwind(-2.0, 3.0, -100.0, 0.0);
            pls.plbox("", 0.0, 0, "cmstv", 30.0, 3);
            pls.plcol0(3);
            pls.plline(freql, phase);
            pls.plcol0(3);
            pls.plmtex("r", 5.0, 0.5, 0.5, "Phase shift (degrees)");
        }
    }
}

//---------------------------------------------------------------------------//
//                              End of x04.java
//---------------------------------------------------------------------------//
