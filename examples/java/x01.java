//---------------------------------------------------------------------------//
// $Id$
//
// Implementation of PLplot example 1 in Java.
//---------------------------------------------------------------------------//

package plplot.examples;

import plplot.core.*;

class x01 {

    public static void main( String[] args ) 
    {
        System.out.println( "Instantiating a PLStream." );
        PLStream pls = new PLStream();

        pls.xyz();

        System.out.println( "Yo!" );
    }
}

//---------------------------------------------------------------------------//
//                              End of x01.java
//---------------------------------------------------------------------------//

