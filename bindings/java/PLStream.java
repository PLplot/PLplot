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

    public native void arrows( float[] u, float[] v, float[] x, float[] y,
                               int n, float scale, float dx, float dy );
    public native void arrows( double[] u, double[] v, double[] x, double[] y,
                               int n, double scale, double dx, double dy );

    public native void axes( float x0, float y0,
                             String xopt, float xtick, int nxsub,
                             String yopt, float ytick, int nysub );
    public native void axes( double x0, double y0,
                             String xopt, double xtick, int nxsub,
                             String yopt, double ytick, int nysub );

    public native void bin( int nbin, float[] x, float[] y, int center );
    public native void bin( int nbin, double[] x, double[] y, int center );

    public native void bop();

    public native void box( String xopt, float xtick, int nxsub,
                            String yopt, float ytick, int nysub );
    public native void box( String xopt, double xtick, int nxsub,
                            String yopt, double ytick, int nysub );

    public native void box3(
        String xopt, String xlabel, float xtick, int nsubx,
        String yopt, String ylabel, float ytick, int nsuby,
        String zopt, String zlabel, float ztick, int nsubz );
    public native void box3(
        String xopt, String xlabel, double xtick, int nsubx,
        String yopt, String ylabel, double ytick, int nsuby,
        String zopt, String zlabel, double ztick, int nsubz );

    public native void col0( int icol );

    public native void col1( float col1 );
    public native void col1( double col1 );

// cont
// pltr1 variants.
    public native void cont( float[][] z, float[] clevel, float[] xg, float[] yg, int wrap );
    public native void cont( double[][] z, double[] clevel, double[] xg, double[] yg, int wrap );

// pltr2 variants.
    public native void cont( float[][] z, float[] clevel, float[][] xg, float[][] yg, int wrap );
    public native void cont( double[][] z, double[] clevel, double[][] xg, double[][] yg, int wrap );

// fcont
// cpstrm?
// did2pc
// dip2dc

    public native void end();

// end1?

    public native void env( float xmin, float xmax, float ymin, float ymax,
                            int just, int axis );
    public native void env( double xmin, double xmax, double ymin, double ymax,
                            int just, int axis );

    public native void eop();

    public native void errx( int n, float[] xmin, float[] xmax, float[] y );
    public native void errx( int n, double[] xmin, double[] xmax, double[] y );

    public native void erry( int n, float[] x, float[] ymin, float[] ymax );
    public native void erry( int n, double[] x, double[] ymin, double[] ymax );

    public native void famadv();

    public native void fill( int n, float[] x, float[] y );
    public native void fill( int n, double[] x, double[] y );

    public native void fill3( int n, float[] x, float[] y, float[] z );
    public native void fill3( int n, double[] x, double[] y, float[] z );

    public native void flush();
    public native void font( int fnt );
    public native void fontld( int fnt );

// gchr
// gcol0
// gcolbg
// gcompression
// gdev
// gdidev
// gdiori
// gdiplt
// gfam
// gfnam
// glevel
// gpage
// gra
// gspa

    public native int gstrm();

// gver
// gxax
// gyax
// gzax

    public native void hist( int n, float[] data, float datmin, float datmax,
                             int nbin, int oldwin );
    public native void hist( int n, double[] data, double datmin, double datmax,
                             int nbin, int oldwin );

    public native void init();

    public native void join( float x1, float y1, float x2, float y2 );
    public native void join( double x1, double y1, double x2, double y2 );

    public native void lab( String xlabel, String ylabel, String tlabel );

    public native void lightsource( float x, float y, float z );
    public native void lightsource( double x, double y, double z );

    public native void line( int n, float[] x, float[] y );
    public native void line( int n, double[] x, double[] y );

    public native void line3( int n, float[] x, float[] y, float[] z );
    public native void line3( int n, double[] x, double[] y, double[] z );

    public native void lsty( int lin );

// map
// meridians

    public native void mesh( float[] x, float[] y, float[][] z, int opt );
    public native void mesh( double[] x, double[] y, double[][] z, int opt );

// mkstrm?

    public native void mtex( String side,
                             float disp, float pos, float just,
                             String text);
    public native void mtex( String side,
                             double disp, double pos, double just,
                             String text);

    public native void plot3d( float[] x, float[] y, float[][] z,
                               int opt, int side );
    public native void plot3d( double[] x, double[] y, double[][] z,
                               int opt, int side );

    public native void plotsh3d( float[] x, float[] y, float[][] z, int side );
    public native void plotsh3d( double[] x, double[] y, double[][] z, int side );

// pat

    public native void poin( int n, float[] x, float[] y, int code );
    public native void poin( int n, double[] x, double[] y, int code );
    public native void poin( float x, float y, int code ); // API addition
    public native void poin( double x, double y, int code ); // API addition

// poin3
// poly3
// plprec

    public native void psty( int patt );

    public native void ptex( float x, float y, float dx, float dy,
                             float just, String text );
    public native void ptex( double x, double y, double dx, double dy,
                             double just, String text );

// replot
// rgb
// rgb1

    public native void schr( float def, float scale );
    public native void schr( double def, double scale );

    public native void scmap0n( int ncol0 );
    public native void scmap1n( int ncol1 );
    public native void scmap0( int[] r, int[] g, int[] b, int ncol0 );
    public native void scmap1( int[] r, int[] g, int[] b, int ncol1 );
    public native void scmap1l( int itype, int npts, float[] i, float[] c1,
				float[] c2, float[] c3, int[] rev );
    public native void scmap1l( int itype, int npts, double[] i, double[] c1,
				double[] c2, double[] c3, int[] rev );
    public native void scol0( int icol0, int r, int g, int b );
    public native void scolbg( int r, int g, int b );
    public native void scolor( int color );
    public native void scompression( int compression );

// /* Set the device (keyword) name */

// void
// c_plsdev(const char *devname);

// /* Set window into device space using margin, aspect ratio, and */
// /* justification */

// void
// c_plsdidev(PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy);

// /* Set up transformation from metafile coordinates. */

// void
// c_plsdimap(PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax,
// 	   PLFLT dimxpmm, PLFLT dimypmm);

// /* Set plot orientation, specifying rotation in units of pi/2. */

// void
// c_plsdiori(PLFLT rot);

// /* Set window into plot space */

// void
// c_plsdiplt(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

// /* Set window into plot space incrementally (zoom) */

// void
// c_plsdiplz(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

// /* Set the escape character for text strings. */

// void
// c_plsesc(char esc);

    public native void setcontlabelparam( float offset, float size, float spacing, int active );
    public native void setcontlabelparam( double offset, double size, double spacing, int active );

    public native void setcontlabelformat( int lexp, int sigdig );

// /* Set family file parameters */

// void
// c_plsfam(PLINT fam, PLINT num, PLINT bmax);

// /* Set the output file name. */

// void
// c_plsfnam(const char *fnam);

    public native void shade( float[][] a,
                              float left, float right, float bottom, float top,
                              float shade_min, float shade_max,
                              int sh_cmap, float sh_color, int sh_width,
                              int min_color, int min_width,
                              int max_color, int max_width,
                              int rect );
    public native void shade( double[][] a,
                              double left, double right, double bottom, double top,
                              double shade_min, double shade_max,
                              int sh_cmap, double sh_color, int sh_width,
                              int min_color, int min_width,
                              int max_color, int max_width,
                              int rect );

// pltr1
    public native void shade( float[][] a,
                              float left, float right, float bottom, float top,
                              float shade_min, float shade_max,
                              int sh_cmap, float sh_color, int sh_width,
                              int min_color, int min_width,
                              int max_color, int max_width,
                              int rect, float[] xg, float[] yg, int wrap );
    public native void shade( double[][] a,
                              double left, double right, double bottom, double top,
                              double shade_min, double shade_max,
                              int sh_cmap, double sh_color, int sh_width,
                              int min_color, int min_width,
                              int max_color, int max_width,
                              int rect, double[] xg, double[] yg, int wrap );

// pltr2
    public native void shade( float[][] a,
                              float left, float right, float bottom, float top,
                              float shade_min, float shade_max,
                              int sh_cmap, float sh_color, int sh_width,
                              int min_color, int min_width,
                              int max_color, int max_width,
                              int rect, float[][] xg, float[][] yg, int wrap );
    public native void shade( double[][] a,
                              double left, double right, double bottom, double top,
                              double shade_min, double shade_max,
                              int sh_cmap, double sh_color, int sh_width,
                              int min_color, int min_width,
                              int max_color, int max_width,
                              int rect, double[][] xg, double[][] yg, int wrap );

// smaj
// smin
// sori

// void
// c_plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng,
// 	  PLINT xoff, PLINT yoff);

    public native void spause( int pause );

// sstrm?

    public native void ssub( int nx, int ny );

// ssym
// star
// start
// stripc
// stripa
// stripd
// image

    public native void styl( int nms, int mark, int space );

    public native void svpa( float xmin, float xmax, float ymin, float ymax );
    public native void svpa( double xmin, double xmax, double ymin, double ymax );

    public native void sxax( int digmax, int digits );

// sxwin

    public native void syax( int digmax, int digits );

    public native void sym( int n, float[] x, float[] y, int code );
    public native void sym( int n, double[] x, double[] y, int code );
    public native void sym( float x, float y, int code ); // API addition
    public native void sym( double x, double y, int code ); // API addition

    public native void szax( int digmax, int digits );
    public native void text();

    public native void vasp( float aspect );
    public native void vasp( double aspect );

    public native void vpas( float xmin, float xmax, float ymin, float ymax, float aspect );
    public native void vpas( double xmin, double xmax, double ymin, double ymax, double aspect );

    public native void vpor( float xmin, float xmax, float ymin, float ymax );
    public native void vpor( double xmin, double xmax, double ymin, double ymax );

    public native void vsta();

    public native void w3d( float basex, float basey, float height, float xmin0,
                            float xmax0, float ymin0, float ymax0, float zmin0,
                            float zmax0, float alt, float az);
    public native void w3d( double basex, double basey, double height, double xmin0,
                            double xmax0, double ymin0, double ymax0, double zmin0,
                            double zmax0, double alt, double az);

    public native void wid( int width );

    public native void wind( float xmin, float xmax, float ymin, float ymax );
    public native void wind( double xmin, double xmax, double ymin, double ymax );

// Methods needed for the implementation of PLStream, but not suitable for
// the public interface.
    native int mkstrm();

// Static code block to get the PLplot dynamic library loaded in.
    static {
        System.loadLibrary( plplot.core.config.libname );
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

