//----------------------------------*-C++-*----------------------------------//
// plstream.cc
// Geoffrey Furnish
// 21 September 1994
//---------------------------------------------------------------------------//
// @> Source file plstream.
//
// $Id$
//
// $Log$
// Revision 1.2  1995/06/22 18:52:59  furnish
// Bare pointer elimination, api collapsing, and removal of features
// intended for use from Fortran.  Still muuuuuch work to be done on this
// class.
//
// Revision 1.1  1995/05/30  07:34:46  mjl
// Changed to a more natural naming scheme.
//
//
// Old log entries (as stream.cc):
//
// Revision 1.5  1995/05/08  20:52:05  furnish
// Mostly formatting improvements, but also some overloading resolutions
// and some C++ style considerations.
//
// Revision 1.4  1995/03/17  07:53:17  mjl
// Added new interface methods to the parsing functions.  Eliminated old
// ones that were obsolete.
//
// Revision 1.3  1995/01/16  19:24:19  mjl
// Fixed arglists for scmap1l and plscmap1l.
//
// Revision 1.2  1994/10/18  16:12:39  furnish
// Beginnings of 2-d abstraction for contouring and shading.  Better
// constructors.  Names for colors.  Other minor stuff.  Still need to do
// major hacking on the 2-d abstraction, and also need to remove large
// numbers of unnecessary methods.
//
// Revision 1.1  1994/10/06  07:24:47  furnish
// New C++ wrapper around the PLplot API.  Needs much work.
//
//---------------------------------------------------------------------------//

#include "plplot.h"
#include "plstream.h"

#include <iostream.h>

PLFLT Contourable_Data_evaluator( PLINT i, PLINT j, PLPointer p )
{
    const Contourable_Data& d = *(Contourable_Data *) p;

    return d(i,j);
}

void Coord_Xform_evaluator( PLFLT ox, PLFLT oy,
			    PLFLT *nx, PLFLT *ny, PLPointer p )
{
    const Coord_Xformer& xf = *(Coord_Xformer *) p;

    xf.xform( ox, oy, *nx, *ny );
}

// A specific case for handling transformation defined by 2-d grid vertex
// specification matricies.

cxx_pltr2::cxx_pltr2( Coord_2d& cx, Coord_2d& cy )
    : xg(cx), yg(cy)
{}

// Next routine copied and modified for C++ from PLPLOT 4.99d.

/*--------------------------------------------------------------------------*\
 * pltr2()
 *
 * Does linear interpolation from doubly dimensioned coord arrays
 * (column dominant, as per normal C 2d arrays).
 *
 * This routine includes lots of checks for out of bounds.  This would
 * occur occasionally due to some bugs in the contour plotter (now fixed).
 * If an out of bounds coordinate is obtained, the boundary value is provided
 * along with a warning.  These checks should stay since no harm is done if
 * if everything works correctly.
\*--------------------------------------------------------------------------*/

void cxx_pltr2::xform( PLFLT x, PLFLT y, PLFLT& tx, PLFLT& ty ) const
{
    int nx, ny;
    xg.elements( nx, ny );

    int ul, ur, vl, vr;
    float du, dv;

    float xll, xlr, xrl, xrr;
    float yll, ylr, yrl, yrr;
    float xmin, xmax, ymin, ymax;

    ul = (int) x;
    ur = ul + 1;
    du = x - ul;

    vl = (int) y;
    vr = vl + 1;
    dv = y - vl;

    xmin = 0;
    xmax = nx - 1;
    ymin = 0;
    ymax = ny - 1;

    if (x < xmin || x > xmax || y < ymin || y > ymax) {
	cerr << "cxx_pltr2::xform, Invalid coordinates\n";
	
	if (x < xmin) {

	    if (y < ymin) {
		tx = xg(0,0);
		ty = yg(0,0);
	    }
	    else if (y > ymax) {
		tx = xg(0, ny-1);
		ty = yg(0, ny-1);
	    }
	    else {
		xll = xg(0, vl);
		yll = yg(0, vl);
		xlr = xg(0, vr);
		ylr = yg(0, vr);

		tx = xll * (1 - dv) + xlr * (dv);
		ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else if (x > xmax) {

	    if (y < ymin) {
		tx = xg(nx-1, 0);
		ty = yg(nx-1, 0);
	    }
	    else if (y > ymax) {
		tx = xg(nx-1, ny-1);
		ty = yg(nx-1, ny-1);
	    }
	    else {
		xll = xg(nx-1, vl);
		yll = yg(nx-1, vl);
		xlr = xg(nx-1, vr);
		ylr = yg(nx-1, vr);

		tx = xll * (1 - dv) + xlr * (dv);
		ty = yll * (1 - dv) + ylr * (dv);
	    }
	}
	else {
	    if (y < ymin) {
		xll = xg(ul, 0);
		xrl = xg(ur, 0);
		yll = yg(ul, 0);
		yrl = yg(ur, 0);

		tx = xll * (1 - du) + xrl * (du);
		ty = yll * (1 - du) + yrl * (du);
	    }
	    else if (y > ymax) {
		xlr = xg(ul, ny-1);
		xrr = xg(ur, ny-1);
		ylr = yg(ul, ny-1);
		yrr = yg(ur, ny-1);

		tx = xlr * (1 - du) + xrr * (du);
		ty = ylr * (1 - du) + yrr * (du);
	    }
	}
    }

/* Normal case.
   Look up coordinates in row-dominant array.
   Have to handle right boundary specially -- if at the edge, we'd
   better not reference the out of bounds point. */

    else {

	xll = xg(ul, vl);
	yll = yg(ul, vl);

/* ur is out of bounds */

	if (ur == nx && vr < ny) {

	    xlr = xg(ul, vr);
	    ylr = yg(ul, vr);

	    tx = xll * (1 - dv) + xlr * (dv);
	    ty = yll * (1 - dv) + ylr * (dv);
	}

/* vr is out of bounds */

	else if (ur < nx && vr == ny) {

	    xrl = xg(ur, vl);
	    yrl = yg(ur, vl);

	    tx = xll * (1 - du) + xrl * (du);
	    ty = yll * (1 - du) + yrl * (du);
	}

/* both ur and vr are out of bounds */

	else if (ur == nx && vr == ny) {

	    tx = xll;
	    ty = yll;
	}

/* everything in bounds */

	else {

	    xrl = xg(ur, vl);
	    xlr = xg(ul, vr);
	    xrr = xg(ur, vr);

	    yrl = yg(ur, vl);
	    ylr = yg(ul, vr);
	    yrr = yg(ur, vr);

	    tx = xll * (1 - du) * (1 - dv) + xlr * (1 - du) * (dv) +
		xrl * (du) * (1 - dv) + xrr * (du) * (dv);

	    ty = yll * (1 - du) * (1 - dv) + ylr * (1 - du) * (dv) +
		yrl * (du) * (1 - dv) + yrr * (du) * (dv);
	}
    }
}

int plstream::next_stream = 0;
int plstream::active_streams = 0;

plstream::plstream()
{
    ::c_plinit();
    ::c_plgstrm( &stream );
}

plstream::plstream( PLS::stream_id sid, int strm /*=0*/ )
{
    switch(sid) {
    case PLS::Next:
//	throw( "plstream ctor option not implemented." );
	break;

    case PLS::Current:
	::c_plgstrm( &stream );
	break;

    case PLS::Specific:
	stream = strm;
	break;

    default:
//	throw( "plstream ctor option not implemented." );
	break;
    }
}

plstream::plstream( int nx, int ny, const char *driver )
{
    ::c_plsstrm(next_stream++);

    if (driver)
	::c_plsdev(driver);
    ::c_plssub( nx, ny );
    ::c_plinit();

    ::c_plgstrm( &stream );

    active_streams++;
}

plstream::~plstream()
{
    ::c_plsstrm(stream);
    ::c_plend1();

    active_streams--;
    if (!active_streams)
	::c_plend();
}

#define BONZAI { throw "plstream method not implemented."; }

	/* C routines callable from stub routines come first */

// Advance to subpage "page", or to the next one if "page" = 0.

void
plstream::adv( PLINT page )
{
    set_stream();

    pladv(page);
}

// This functions similarly to plbox() except that the origin of the axes is
// placed at the user-specified point (x0, y0).

void
plstream::axes( PLFLT x0, PLFLT y0, const char *xopt, PLFLT xtick, PLINT nxsub,
		const char *yopt, PLFLT ytick, PLINT nysub )
{
    set_stream();

    plaxes( x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub );
}

// Plot a histogram using x to store data values and y to store frequencies.

void plstream::bin( PLINT nbin, PLFLT *x, PLFLT *y, PLINT center )
{
    set_stream();

    plbin(nbin, x, y, center);
}

// Start new page.  Should only be used with pleop().

void plstream::bop()
{
    set_stream();

    plbop();
}

// This draws a box around the current viewport.

void plstream::box( const char *xopt, PLFLT xtick, PLINT nxsub,
		    const char *yopt, PLFLT ytick, PLINT nysub )
{
    set_stream();

    plbox( xopt, xtick, nxsub, yopt, ytick, nysub );
}

// This is the 3-d analogue of plbox().

void
plstream::box3( const char *xopt, const char *xlabel, PLFLT xtick, PLINT nsubx,
		const char *yopt, const char *ylabel, PLFLT ytick, PLINT nsuby,
		const char *zopt, const char *zlabel, PLFLT ztick, PLINT nsubz )
{
    set_stream();

    plbox3( xopt, xlabel, xtick, nsubx,
	    yopt, ylabel, ytick, nsuby,
	    zopt, zlabel, ztick, nsubz );
}

// Set color, map 0.  Argument is integer between 0 and 15.

void plstream::col0( PLINT icol0 )
{
    set_stream();

    plcol0( icol0 );
}

// Set the color using a descriptive name.  Replaces plcol0().

void plstream::col( PLcolor c )
{
    set_stream();

    plcol0( (int) c );
}

// Set color, map 1.  Argument is a float between 0. and 1.

void plstream::col( PLFLT c )
{
    set_stream();

    plcol1( c );
}

// Draws a contour plot from data in f(nx,ny).  Is just a front-end to
// plfcont, with a particular choice for f2eval and f2eval_data.

void plstream::cont( PLFLT **f, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
		     PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
		     void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		     PLPointer pltr_data )
{
    set_stream();

    plcont( f, nx, ny, kx, lx, ky, ly, clevel, nlevel,
	    pltr, pltr_data );
}

/* Draws a contour plot using the function evaluator f2eval and data stored
 * by way of the f2eval_data pointer.  This allows arbitrary organizations
 * of 2d array data to be used. 
 */

void plstream::fcont( PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
		      PLPointer f2eval_data,
		      PLINT nx, PLINT ny, PLINT kx, PLINT lx,
		      PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
		      void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		      PLPointer pltr_data )
{
    set_stream();

    plfcont( f2eval, f2eval_data,
	     nx, ny, kx, lx, ky, ly, clevel, nlevel,
	     pltr, pltr_data );
}

// /* Copies state parameters from the reference stream to the current stream. */

// void plstream::cpstrm( PLINT iplsr, PLINT flags )
// {
//     set_stream();

//     plcpstrm(iplsr,flags);
// }

// Converts input values from relative device coordinates to relative plot
// coordinates.

void plstream::did2pc( PLFLT& xmin, PLFLT& ymin, PLFLT& xmax, PLFLT& ymax )
{
    set_stream();

    pldid2pc( &xmin, &ymin, &xmax, &ymax );
}

// Converts input values from relative plot coordinates to relative device
// coordinates.

void plstream::dip2dc( PLFLT& xmin, PLFLT& ymin, PLFLT& xmax, PLFLT& ymax )
{
    set_stream();

    pldip2dc( &xmin, &ymin, &xmax, &ymax );
}

// These shouldn't be needed, are supposed to be handled by ctor/dtor
// semantics of the plstream object.

// /* End a plotting session for all open streams. */

// void plstream::end()
// {
//     set_stream();

//     plend();
// }

// /* End a plotting session for the current stream only. */

// void plstream::end1()
// {
//     set_stream();

//     plend1();
// }

// Simple interface for defining viewport and window.

void plstream::env( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
		    PLINT just, PLINT axis )
{
    set_stream();

    plenv(xmin, xmax, ymin, ymax, just, axis);
}

// End current page.  Should only be used with plbop().

void plstream::eop()
{
    set_stream();

    pleop();
}

// Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)).

void plstream::errx( PLINT n, PLFLT *xmin, PLFLT *xmax, PLFLT *y )
{
    set_stream();

    plerrx(n, xmin, xmax, y);
}

// Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)).

void plstream::erry( PLINT n, PLFLT *x, PLFLT *ymin, PLFLT *ymax )
{
    set_stream();

    plerry(n, x, ymin, ymax);
}

// Advance to the next family file on the next new page.

void plstream::famadv()
{
    set_stream();

    plfamadv();
}

// Pattern fills the polygon bounded by the input points.

void plstream::fill( PLINT n, PLFLT *x, PLFLT *y )
{
    set_stream();

    plfill(n, x, y);
}

// Flushes the output stream.  Use sparingly, if at all.

void plstream::plflush()
{
    set_stream();

    ::c_plflush();
}

// Sets the global font flag to 'ifont'.

void plstream::font( PLINT ifont )
{
    set_stream();

    plfont(ifont);
}

// Load specified font set.

void plstream::fontld( PLINT fnt )
{
    set_stream();

    plfontld(fnt);
}

// Get character default height and current (scaled) height.

void plstream::gchr( PLFLT& p_def, PLFLT& p_ht )
{
    set_stream();

    plgchr( &p_def, &p_ht );
}

// Returns 8 bit RGB values for given color from color map 0.

void plstream::gcol0( PLINT icol0, PLINT& r, PLINT& g, PLINT& b )
{
    set_stream();

    plgcol0( icol0, &r, &g, &b );
}

// Returns the background color by 8 bit RGB value/

void plstream::gcolbg( PLINT& r, PLINT& g, PLINT& b )
{
    set_stream();

    plgcolbg( &r, &g, &b );
}

// Retrieve current window into device space.

void plstream::gdidev( PLFLT& mar, PLFLT& aspect, PLFLT& jx,
		       PLFLT& jy )
{
    set_stream();

    plgdidev( &mar, &aspect, &jx, &jy );
}

// Get plot orientation.

void plstream::gdiori( PLFLT& rot )
{
    set_stream();

    plgdiori( &rot );
}

// Retrieve current window into plot space.

void plstream::gdiplt( PLFLT& xmin, PLFLT& ymin, PLFLT& xmax, PLFLT& ymax )
{
    set_stream();

    plgdiplt( &xmin, &ymin, &xmax, &ymax );
}

// Get family file parameters.

void plstream::gfam( PLINT& fam, PLINT& num, PLINT& bmax )
{
    set_stream();

    plgfam( &fam, &num, &bmax );
}

// Get the (current) output file name.  Must be preallocated to >80 bytes.

void plstream::gfnam( char *fnam )
{
    set_stream();

    plgfnam(fnam);
}

// Get output device parameters.

void plstream::gpage(PLFLT& xp, PLFLT& yp, PLINT& xleng, PLINT& yleng,
		     PLINT& xoff, PLINT& yoff )
{
    set_stream();

    plgpage( &xp, &yp, &xleng, &yleng, &xoff, &yoff );
}

// Switches to graphics screen.

void plstream::gra()
{
    set_stream();

    plgra();
}

// Get subpage boundaries in absolute coordinates.

void plstream::gspa( PLFLT& xmin, PLFLT& xmax, PLFLT& ymin, PLFLT& ymax )
{
    set_stream();

    plgspa( &xmin, &xmax, &ymin, &ymax );
}

// This shouldn't be needed in this model.

// /* Get current stream number. */

// void plstream::gstrm( PLINT *p_strm )
// {
//     set_stream();

//     plgstrm(p_strm);
// }

// Get the current library version number.

void plstream::gver( char *p_ver )
{
    set_stream();

    plgver(p_ver);
}

// Get x axis labeling parameters.

void plstream::gxax( PLINT& digmax, PLINT& digits )
{
    set_stream();

    plgxax( &digmax, &digits );
}

// Get y axis labeling parameters.

void plstream::gyax( PLINT& digmax, PLINT& digits )
{
    set_stream();

    plgyax( &digmax, &digits );
}

/* Get z axis labeling parameters */

void plstream::gzax( PLINT *p_digmax, PLINT *p_digits )
{
    set_stream();

    plgzax(p_digmax, p_digits);
}

/* Draws a histogram of n values of a variable in array data[0..n-1] */

void plstream::hist( PLINT n, PLFLT *data, PLFLT datmin, PLFLT datmax,
		     PLINT nbin, PLINT oldwin )
{
    set_stream();

    plhist(n, data, datmin, datmax, nbin, oldwin);
}

/* Set current color (map 0) by hue, lightness, and saturation. */

void plstream::hls( PLFLT h, PLFLT l, PLFLT s )
{
    set_stream();

    plhls(h,l,s);
}

/* Initializes PLplot, using preset or default options */

void plstream::init()
{
    set_stream();

    plinit();
}

/* Draws a line segment from (x1, y1) to (x2, y2). */

void plstream::join( PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2 )
{
    set_stream();

    pljoin(x1,y1,x2,y2);
}

/* Simple routine for labelling graphs. */

void plstream::lab( const char *xlabel, const char *ylabel,
		    const char *tlabel )
{
    set_stream();

    pllab(xlabel, ylabel, tlabel);
}

/* Draws line segments connecting a series of points. */

void plstream::line( PLINT n, PLFLT *x, PLFLT *y )
{
    set_stream();

    plline(n,x,y);
}

/* Draws a line in 3 space.  */

void plstream::line3( PLINT n, PLFLT *x, PLFLT *y, PLFLT *z )
{
    set_stream();

    plline3(n,x,y,z);
}

/* Set line style. */

void plstream::lsty( PLINT lin )
{
    set_stream();

    pllsty(lin);
}

/* plot continental outline in world coordinates */

void plstream::map( void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type,
		    PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat )
{
    set_stream();

    plmap( mapform, type, minlong, maxlong, minlat, maxlat );
}

/* Plot the latitudes and longitudes on the background. */

void  plstream::meridians( void (*mapform)(PLINT, PLFLT *, PLFLT *), 
			   PLFLT dlong, PLFLT dlat,
			   PLFLT minlong, PLFLT maxlong,
			   PLFLT minlat, PLFLT maxlat)
{
    set_stream();

    plmeridians( mapform, dlong, dlat, minlong, maxlong, minlat,
		 maxlat );
}

/* Plots a mesh representation of the function z[x][y]. */

void plstream::mesh( PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny,
		     PLINT opt)
{
    set_stream();

    plmesh(x, y, z, nx, ny, opt );
}

// /* Creates a new stream and makes it the default.  */

// void plstream::mkstrm( PLINT *p_strm )
// {
//     set_stream();

//     plmkstrm(p_strm);
// }

/* Prints out "text" at specified position relative to viewport */

void plstream::mtex( const char *side, PLFLT disp, PLFLT pos, PLFLT just,
		     const char *text )
{
    set_stream();

    plmtex(side,disp,pos,just,text);
}

/* Plots a 3-d representation of the function z[x][y]. */

void plstream::plot3d( PLFLT *x, PLFLT *y, PLFLT **z,
		       PLINT nx, PLINT ny, PLINT opt, PLINT side )
{
    set_stream();

    ::plot3d(x,y,z,nx,ny,opt,side);
}

/* Set fill pattern directly. */

void plstream::pat( PLINT nlin, PLINT *inc, PLINT *del )
{
    set_stream();

    plpat(nlin,inc,del);
}

/* Plots array y against x for n points using ASCII code "code".*/

void plstream::poin( PLINT n, PLFLT *x, PLFLT *y, PLINT code )
{
    set_stream();

    plpoin(n,x,y,code);
}

/* Draws a series of points in 3 space. */

void plstream::poin3( PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT code )
{
    set_stream();

    plpoin3(n,x,y,z,code);
}

/* Draws a polygon in 3 space.  */

void plstream::poly3( PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT *draw )
{
    set_stream();

    plpoly3(n,x,y,z,draw);
}

/* Set the floating point precision (in number of places) in numeric labels. */

void plstream::prec( PLINT setp, PLINT prec )
{
    set_stream();

    plprec(setp,prec);
}

/* Set fill pattern, using one of the predefined patterns.*/

void plstream::psty( PLINT patt )
{
    set_stream();

    plpsty(patt);
}

/* Prints out "text" at world cooordinate (x,y). */

void plstream::ptex( PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just,
		     const char *text)
{
    set_stream();

    plptex(x,y,dx,dy,just,text);
}

/* Replays contents of plot buffer to current device/file. */

void plstream::replot()
{
    set_stream();

    plreplot();
}

/* Set line color by red, green, blue from  0. to 1. */

void plstream::rgb( PLFLT r, PLFLT g, PLFLT b )
{
    set_stream();

    plrgb(r,g,b);
}

/* Set line color by 8 bit RGB values. */

void plstream::rgb( PLINT r, PLINT g, PLINT b )
{
    set_stream();

    plrgb1(r,g,b);
}

/* Set character height. */

void plstream::schr( PLFLT def, PLFLT scale )
{
    set_stream();

    plschr(def,scale);
}

/* Set number of colors in cmap 0 */

void plstream::scmap0n( PLINT ncol0 )
{
    set_stream();

    plscmap0n(ncol0);
}

/* Set number of colors in cmap 1 */

void plstream::scmap1n( PLINT ncol1 )
{
    set_stream();

    plscmap1n(ncol1);
}

/* Set color map 0 colors by 8 bit RGB values */

void plstream::scmap0( PLINT *r, PLINT *g, PLINT *b, PLINT ncol0 )
{
    set_stream();

    plscmap0(r,g,b,ncol0);
}

/* Set color map 1 colors by 8 bit RGB values */

void plstream::scmap1( PLINT *r, PLINT *g, PLINT *b, PLINT ncol1 )
{
    set_stream();

    plscmap1(r,g,b,ncol1);
}

/* Set color map 1 colors using a piece-wise linear relationship between */
/* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space. */

void plstream::scmap1l( PLINT itype, PLINT npts, PLFLT *intensity,
			PLFLT *coord1, PLFLT *coord2, PLFLT *coord3,
			PLINT *rev )
{
    set_stream();

    plscmap1l(itype,npts,intensity,coord1,coord2,coord3,rev);
}

/* Set a given color from color map 0 by 8 bit RGB value */

void plstream::scol0( PLINT icol0, PLINT r, PLINT g, PLINT b )
{
    set_stream();

    plscol0(icol0,r,g,b);
}

/* Set the background color by 8 bit RGB value */

void plstream::scolbg( PLINT r, PLINT g, PLINT b )
{
    set_stream();

    plscolbg(r,g,b);
}

/* Used to globally turn color output on/off */

void plstream::scolor( PLINT color )
{
    set_stream();

    plscolor(color);
}

/* Set the device (keyword) name */

void plstream::sdev( const char *devname )
{
    set_stream();

    plsdev(devname);
}

/* Set window into device space using margin, aspect ratio, and */
/* justification */

void plstream::sdidev( PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy )
{
    set_stream();

    plsdidev(mar,aspect,jx,jy);
}

/* Set up transformation from metafile coordinates. */

void plstream::sdimap( PLINT dimxmin, PLINT dimxmax,
		       PLINT dimymin, PLINT dimymax,
		       PLFLT dimxpmm, PLFLT dimypmm)
{
    set_stream();

    plsdimap(dimxmin,dimxmax,dimymin,dimymax,dimxpmm,dimypmm);
}

/* Set plot orientation, specifying rotation in units of pi/2. */

void plstream::sdiori( PLFLT rot )
{
    set_stream();

    plsdiori(rot);
}

/* Set window into plot space */

void plstream::sdiplt( PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax )
{
    set_stream();

    plsdiplt(xmin,ymin,xmax,ymax);
}

/* Set window into plot space incrementally (zoom) */

void plstream::sdiplz( PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax )
{
    set_stream();

    plsdiplz(xmin,ymin,xmax,ymax);
}

/* Set the escape character for text strings. */

void plstream::sesc( char esc )
{
    set_stream();

    plsesc(esc);
}

/* Set family file parameters */

void plstream::sfam( PLINT fam, PLINT num, PLINT bmax )
{
    set_stream();

    plsfam(fam,num,bmax);
}

/* Set the output file name. */

void plstream::sfnam( const char *fnam )
{
    set_stream();

    plsfnam(fnam);
}

/* Shade region. */

void 
plstream::shade( PLFLT **a, PLINT nx, PLINT ny, const char **defined,
		 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
		 PLFLT shade_min, PLFLT shade_max,
		 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
		 PLINT min_color, PLINT min_width,
		 PLINT max_color, PLINT max_width,
		 void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
		 void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		 PLPointer pltr_data )
{
    set_stream();

    plshade( a,nx,ny,defined,left,right,bottom,top,
	     shade_min, shade_max,
	     sh_cmap, sh_color, sh_width,
	     min_color, min_width, max_color, max_width,
	     fill, rectangular, pltr, pltr_data );
}

void
plstream::shade( Contourable_Data& d, PLFLT xmin, PLFLT xmax,
		 PLFLT ymin, PLFLT ymax, PLFLT shade_min, PLFLT shade_max,
		 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
		 PLINT min_color, PLINT min_width,
		 PLINT max_color, PLINT max_width,
		 PLINT rectangular,
		 Coord_Xformer *pcxf )
{
    set_stream();

    int nx, ny;
    d.elements( nx, ny );

    ::plfshade( Contourable_Data_evaluator, &d,
		NULL, NULL,
		nx, ny,
		xmin, xmax, ymin, ymax, shade_min, shade_max,
		sh_cmap, sh_color, sh_width,
		min_color, min_width, max_color, max_width,
		::plfill, rectangular,
		Coord_Xform_evaluator, pcxf );
}

void 
plstream::shade1( PLFLT *a, PLINT nx, PLINT ny, const char *defined,
		  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
		  PLFLT shade_min, PLFLT shade_max,
		  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
		  PLINT min_color, PLINT min_width,
		  PLINT max_color, PLINT max_width,
		  void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
		  void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		  PLPointer pltr_data )
{
    set_stream();

    plshade1( a, nx, ny, defined,
	      left, right, bottom, top,
	      shade_min, shade_max,
	      sh_cmap, sh_color, sh_width,
	      min_color, min_width, max_color, max_width,
	      fill, rectangular, pltr, pltr_data );
}

void 
plstream::fshade( PLFLT (*f2eval) (PLINT, PLINT, PLPointer),
		  PLPointer f2eval_data,
		  PLFLT (*c2eval) (PLINT, PLINT, PLPointer),
		  PLPointer c2eval_data,
		  PLINT nx, PLINT ny, 
		  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
		  PLFLT shade_min, PLFLT shade_max,
		  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
		  PLINT min_color, PLINT min_width,
		  PLINT max_color, PLINT max_width,
		  void (*fill) (PLINT, PLFLT *, PLFLT *), PLINT rectangular,
		  void (*pltr) (PLFLT, PLFLT, PLFLT *, PLFLT *, PLPointer),
		  PLPointer pltr_data )
{
    set_stream();

    plfshade( f2eval, f2eval_data,
	      c2eval, c2eval_data,
	      nx, ny, left, right, bottom, top,
	      shade_min, shade_max,
	      sh_cmap, sh_color, sh_width,
	      min_color, min_width, max_color, max_width,
	      fill, rectangular, pltr, pltr_data );
}

/* Set up lengths of major tick marks. */

void plstream::smaj( PLFLT def, PLFLT scale )
{
    set_stream();

    plsmaj(def,scale);
}

/* Set up lengths of minor tick marks. */

void plstream::smin( PLFLT def, PLFLT scale )
{
    set_stream();

    plsmin(def, scale);
}

/* Set orientation.  Must be done before calling plinit. */

void plstream::sori( PLINT ori )
{
    set_stream();

    plsori(ori);
}

/* Set output device parameters.  Usually ignored by the driver. */

void plstream::spage( PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng,
		      PLINT xoff, PLINT yoff )
{
    set_stream();

    plspage(xp,yp,xleng,yleng,xoff,yoff);
}

/* Set the pause (on end-of-page) status */

void plstream::spause( PLINT pause )
{
    set_stream();

    plspause(pause);
}

/* Set stream number.  */

void plstream::sstrm( PLINT strm )
{
    set_stream();

    plsstrm(strm);
}

/* Set the number of subwindows in x and y */

void plstream::ssub( PLINT nx, PLINT ny )
{
    set_stream();

    plssub(nx,ny);
}

/* Set symbol height. */

void plstream::ssym( PLFLT def, PLFLT scale )
{
    set_stream();

    plssym(def,scale);
}

/* Initialize PLplot, passing in the windows/page settings. */

void plstream::star( PLINT nx, PLINT ny )
{
    set_stream();

    plstar(nx,ny);
}

/* Initialize PLplot, passing the device name and windows/page settings. */

void plstream::start( const char *devname, PLINT nx, PLINT ny )
{
    set_stream();

    plstart(devname,nx,ny);
}

/* Set up a new line style */

void plstream::styl( PLINT nms, PLINT *mark, PLINT *space )
{
    set_stream();

    plstyl(nms, mark, space);
}

/* Sets the edges of the viewport to the specified absolute coordinates */

void plstream::svpa( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax )
{
    set_stream();

    plsvpa(xmin,xmax,ymin,ymax);
}

/* Set x axis labeling parameters */

void plstream::sxax( PLINT digmax, PLINT digits )
{
    set_stream();

    plsxax(digmax,digits);
}

/* Set inferior X window */

void plstream::sxwin( PLINT window_id )
{
    set_stream();

    plsxwin(window_id);
}

/* Set y axis labeling parameters */

void plstream::syax( PLINT digmax, PLINT digits )
{
    set_stream();

    plsyax(digmax,digits);
}

/* Plots array y against x for n points using Hershey symbol "code" */

void plstream::sym( PLINT n, PLFLT *x, PLFLT *y, PLINT code )
{
    set_stream();

    plsym(n,x,y,code);
}

/* Set z axis labeling parameters */

void plstream::szax( PLINT digmax, PLINT digits )
{
    set_stream();

    plszax(digmax,digits);
}

/* Switches to text screen. */

void plstream::text()
{
    set_stream();

    pltext();
}

/* Sets the edges of the viewport with the given aspect ratio, leaving */
/* room for labels. */

void plstream::vasp( PLFLT aspect )
{
    set_stream();

    plvasp(aspect);
}

/* Creates the largest viewport of the specified aspect ratio that fits */
/* within the specified normalized subpage coordinates. */

void plstream::vpas( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
		     PLFLT aspect )
{
    set_stream();

    plvpas(xmin,xmax,ymin,ymax,aspect);
}

/* Creates a viewport with the specified normalized subpage coordinates. */

void plstream::vpor( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax )
{
    set_stream();

    plvpor(xmin,xmax,ymin,ymax);
}

/* Defines a "standard" viewport with seven character heights for */
/* the left margin and four character heights everywhere else. */

void plstream::vsta()
{
    set_stream();

    plvsta();
}

/* Set up a window for three-dimensional plotting. */

void plstream::w3d( PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0,
		    PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0,
		    PLFLT zmax0, PLFLT alt, PLFLT az )
{
    set_stream();

    plw3d(basex,basey,height,xmin0,xmax0,ymin0,ymax0,zmin0,zmax0,
	  alt,az);
}

/* Set pen width. */

void plstream::wid( PLINT width )
{
    set_stream();

    plwid(width);
}

/* Set up world coordinates of the viewport boundaries (2d plots). */

void plstream::wind( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax )
{
    set_stream();

    plwind(xmin,xmax,ymin,ymax);
}

	/* The rest for use from C only */

/* Returns a list of file-oriented device names and their menu strings */

void plstream::gFileDevs( char ***p_menustr, char ***p_devname, int *p_ndev )
{
    set_stream();

    plgFileDevs(p_menustr,p_devname,p_ndev);
}

/* Set the function pointer for the keyboard event handler */

void plstream::sKeyEH( void (*KeyEH) (PLGraphicsIn *, void *, int *),
		       void *KeyEH_data )
{
    set_stream();

    plsKeyEH(KeyEH, KeyEH_data);
}

/* Sets an optional user exit handler. */

void plstream::sexit( int (*handler) (char *) )
{
    set_stream();

    plsexit(handler);
}

	/* Transformation routines */

/* Identity transformation. */

void plstream::tr0( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty,
		    PLPointer pltr_data )
{
    set_stream();

    pltr0(x,y,tx,ty,pltr_data);
}

/* Does linear interpolation from singly dimensioned coord arrays. */

void plstream::tr1( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty,
		    PLPointer pltr_data )
{
    set_stream();

    pltr1(x,y,tx,ty,pltr_data);
}

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (column dominant, as per normal C 2d arrays). */

void plstream::tr2( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty,
		    PLPointer pltr_data )
{
    set_stream();

    pltr2(x,y,tx,ty,pltr_data);
}

/* Just like pltr2() but uses pointer arithmetic to get coordinates from */
/* 2d grid tables.  */

void plstream::tr2p( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty,
		     PLPointer pltr_data )
{
    set_stream();

    pltr2p(x,y,tx,ty,pltr_data);
}

// We obviously won't be using this object from Fortran...
// /* Identity transformation for plots from Fortran. */

// void plstream::tr0f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data )
// {
//     set_stream();

//     pltr0f(x,y,tx,ty,pltr_data);
// }

// /* Does linear interpolation from doubly dimensioned coord arrays */
// /* (row dominant, i.e. Fortran ordering). */

// void plstream::tr2f( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data )
// {
//     set_stream();

//     pltr2f(x,y,tx,ty,pltr_data);
// }

/* Example linear transformation function for contour plotter. */

void  plstream::xform( PLFLT x, PLFLT y, PLFLT * tx, PLFLT * ty )
{
    set_stream();

    xform(x,y,tx,ty);
}

	/* Function evaluators */

/* Does a lookup from a 2d function array.  Array is of type (PLFLT **), */
/* and is column dominant (normal C ordering). */

PLFLT plstream::f2eval2( PLINT ix, PLINT iy, PLPointer plf2eval_data )
{
    set_stream();

    return ::plf2eval2( ix, iy, plf2eval_data );
}

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is column dominant (normal C ordering). */

PLFLT plstream::f2eval( PLINT ix, PLINT iy, PLPointer plf2eval_data )
{
    set_stream();

    return ::plf2eval( ix, iy, plf2eval_data );
}

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is row dominant (Fortran ordering). */

PLFLT plstream::f2evalr( PLINT ix, PLINT iy, PLPointer plf2eval_data )
{
    set_stream();

    return ::plf2evalr( ix, iy, plf2eval_data );
}

	/* Command line parsing utilities */

/* Clear internal option table info structure. */

void plstream::ClearOpts()
{
    set_stream();

    ::plClearOpts();
}

/* Reset internal option table info structure. */

void plstream::ResetOpts()
{
    set_stream();

    ::plResetOpts();
}

/* Merge user option table into internal info structure. */

int plstream::MergeOpts( PLOptionTable *options, char *name, char **notes )
{
    set_stream();

    return ::plMergeOpts(options, name, notes);
}

/* Set the strings used in usage and syntax messages. */

void plstream::SetUsage( char *program_string, char *usage_string )
{
    set_stream();

    ::plSetUsage(program_string, usage_string);
}

/* Process input strings, treating them as an option and argument pair. */

int plstream::SetOpt( char *opt, char *optarg )
{
    set_stream();

    return ::plSetOpt(opt, optarg);
}

/* Process options list using current options info. */

int plstream::ParseOpts( int *p_argc, char **argv, PLINT mode )
{
    set_stream();

    return ::plParseOpts( p_argc, argv, mode );
}

/* Print usage & syntax message. */

void plstream::OptUsage()
{
    set_stream();

    ::plOptUsage();
}

	/* Miscellaneous */

/* Set the output file pointer */

void plstream::gfile(FILE **p_file )
{
    set_stream();

    ::plgfile(p_file);
}

/* Get the output file pointer */

void plstream::sfile( FILE *file )
{
    set_stream();
    
    ::plsfile(file);
}


/* Get the escape character for text strings. */

void plstream::gesc( char *p_esc )
{
    set_stream();

	::plgesc( p_esc );
}

/* Front-end to driver escape function. */

void plstream::pl_cmd( PLINT op, void *ptr )
{
    set_stream();

	::pl_cmd(op,ptr);
}

/* Return full pathname for given file if executable */

int  plstream::FindName( char *p )
{
    set_stream();

    return plFindName(p);
}

/* Looks for the specified executable file according to usual search path. */

char *plstream::FindCommand( char *fn )
{
    set_stream();

    return plFindCommand(fn);
}

/* Gets search name for file by concatenating the dir, subdir, and file */
/* name, allocating memory as needed.  */

void plstream::GetName( char *dir, char *subdir, char *filename,
			char **filespec )
{
    set_stream();

    plGetName(dir,subdir,filename,filespec);
}

/* Prompts human to input an integer in response to given message. */

PLINT plstream::GetInt( char *s )
{
    set_stream();

    return plGetInt(s);
}

/* Prompts human to input a float in response to given message. */

PLFLT plstream::GetFlt( char *s )
{
    set_stream();

    return plGetFlt(s);
}

	/* Nice way to allocate space for a vectored 2d grid */

/* Allocates a block of memory for use as a 2-d grid of PLFLT's.  */

void plstream::Alloc2dGrid( PLFLT ***f, PLINT nx, PLINT ny )
{
    set_stream();

    plAlloc2dGrid(f,nx,ny);
}

/* Frees a block of memory allocated with plAlloc2dGrid(). */

void plstream::Free2dGrid( PLFLT **f, PLINT nx, PLINT ny )
{
    set_stream();

    plFree2dGrid(f,nx,ny);
}

/* Functions for converting between HLS and RGB color space */

void plstream::HLS_RGB( PLFLT h, PLFLT l, PLFLT s, PLFLT *p_r, PLFLT *p_g,
			PLFLT *p_b )
{
    set_stream();

    plHLS_RGB(h,l,s,p_r,p_g,p_b);
}

void plstream::RGB_HLS( PLFLT r, PLFLT g, PLFLT b, PLFLT *p_h, PLFLT *p_l,
			PLFLT *p_s )
{
    set_stream();

    plRGB_HLS(r,g,b,p_h,p_l,p_s);
}

/* Wait for right button mouse event and translate to world coordinates */

int plstream::GetCursor( PLGraphicsIn *gin )
{
    set_stream();

    return plGetCursor(gin);
}

//---------------------------------------------------------------------------//
//                              end of plstream.cc
//---------------------------------------------------------------------------//
