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

// Arg parsing support.
    public final int PL_PARSE_PARTIAL      = 0x0000;
    public final int PL_PARSE_FULL         = 0x0001;
    public final int PL_PARSE_QUIET        = 0x0002;
    public final int PL_PARSE_NODELETE     = 0x0004;

    public final int PL_PARSE_SHOWALL      = 0x0008;
    public final int PL_PARSE_OVERRIDE     = 0x0010;
    public final int PL_PARSE_NOPROGRAM    = 0x0020;
    public final int PL_PARSE_NODASH       = 0x0040;
    public final int PL_PARSE_SKIP         = 0x0080;

    public native void ClearOpts();
    public native void ResetOpts();
// public native int MergeOpts(...);
    public native void SetUsage( String program_string, String usage_string );
// setopt()/SetOpt() ???
    public native int ParseOpts( String[] argv, int mode );
    public native void OptUsage();

// The PLplot core API function calls.
    public native void adv( int page );
    public native void box( String xopt, float xtick, int nxsub,
                            String yopt, float ytick, int nysub );
    public native void box( String xopt, double xtick, int nxsub,
                            String yopt, double ytick, int nysub );
    public native void box3(
        String xopt, String xlabel, float xtick, int nsubx,
        String yopt, String ylabel, float ytick, int nsuby,
        String zopt, String zlabel, float ztick, int nsubz );
    public native void col0( int icol );
    public native void end();
    public native void env( float xmin, float xmax, float ymin, float ymax,
                            int just, int axis );
    public native void flush();
    public native void font( int fnt );
    public native void fontld( int fnt );
    public native int gstrm();
    public native void init();

    public native void join( float x1, float y1, float x2, float y2 );
    public native void join( double x1, double y1, double x2, double y2 );

    public native void lab( String xlabel, String ylabel, String tlabel );
    public native void line( int n, float[] x, float[] y );
    public native void poin( int n, float[] x, float[] y, int code );
    public native void poin( float x, float y, int code ); // API addition
    public native void poin( double x, double y, int code ); // API addition
    public native void ptex( float x, float y, float dx, float dy,
                             float just, String text );
    public native void ptex( double x, double y, double dx, double dy,
                             double just, String text );
    public native void schr( float def, float scale );
    public native void schr( double def, double scale );
    public native void ssub( int nx, int ny );
    public native void styl( int nms, int mark, int space );
    public native void syax( int digmax, int digits );
    public native void vpor( float xmin, float xmax, float ymin, float ymax );
    public native void vpor( double xmin, double xmax, double ymin, double ymax );
    public native void vsta();
    public native void wid( int width );
    public native void wind( float xmin, float xmax, float ymin, float ymax );
    public native void wind( double xmin, double xmax, double ymin, double ymax );

// Methods needed for the implementation of PLStream, but not suitable for
// the public interface.
    native int mkstrm();

// Static code block to get the PLplot dynamic library loaded in.
    static {
        System.loadLibrary( "plplot" );
    }

// Class data.
    int stream_id;

// Now comes stuff we need done in Java.
    public PLStream() 
    {
        stream_id = mkstrm();
    }

    public int get_stream_id() { return stream_id; }
}

//---------------------------------------------------------------------------//
//                              End of PLStream.java
//---------------------------------------------------------------------------//

