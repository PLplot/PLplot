//---------------------------------------------------------------------------//
// $Id$
//
// This file implements the core Java side of the Java->PLplot interface, by
// providing a Java class PLStream, which is intended to mirror the C
// PLStream of PLplot.  However, we go further here in Java than we were able
// to do in C, by actually making the PLplot API entry points methods of this
// stream class.  Thus, to plot to a stream in Java, you just instantiate the
// stream, and invoke PLplot API services upon it.  This is basically what we
// have tried to do in the bindings to other object oriented environments,
// such as C++ and the Tcl/Tk and Python/Tk widget contexts.  A stream
// represents sort of a "drawable" abstraction, and the PLplot API gets the
// drawing done on the identified stream.  In C you have to swtich the stream
// explicitly with the plsstrm function, but in Java you just invoke the API
// function on the approrpiate stream object.
//---------------------------------------------------------------------------//

package plplot.core;

public class PLStream {

// The PLplot core API function calls.
    public native void col0( int icol );
    public native void end();
    public native void env( float xmin, float xmax, float ymin, float ymax,
                            int just, int axis );
    public native void fontld( int fnt );
    public native void init();
    public native void lab( String xlabel, String ylabel, String tlabel );
    public native void line( int n, float[] x, float[] y );
    public native void poin( int n, float[] x, float[] y, int code );

// Static code block to get the PLplot dynamic library loaded in.
    static {
        System.loadLibrary( "plplot" );
    }

// Now comes stuff we need done in Java.
    public PLStream() {}

    public void xyz() { System.out.println( "In xyz()" ); }
}

//---------------------------------------------------------------------------//
//                              End of PLStream.java
//---------------------------------------------------------------------------//

