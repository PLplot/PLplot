//---------------------------------------------------------------------------//
// $Id$
//
// This file implements the core Java side of the Java->PLplot interface, by
// providing a Java class PLStreamc, which is intended to mirror the C
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

public class PLStreamc {

// Methods needed for the implementation of PLStreamc, but not suitable for
// the public interface.
//    native int mkstrm();

// Static code block to get the PLplot java wrapper dynamic library loaded in.
   static {
      try {
	 System.load( plplot.core.config.libname );
      } catch (UnsatisfiedLinkError e) {
	 System.err.println("Native code library failed to load. See the chapter on Dynamic Linking Problems in the SWIG Java documentation for help.\n" + e);
	 System.exit(1);
      }
   }

// Class data.
    int stream_id;

// Now comes stuff we need done in Java.
    public PLStreamc() 
    {
//        stream_id = mkstrm();
//        stream_id = plmkstrm();
    }

//    public int get_stream_id() { return stream_id; }
}

//---------------------------------------------------------------------------//
//                              End of PLStreamc.java
//---------------------------------------------------------------------------//

