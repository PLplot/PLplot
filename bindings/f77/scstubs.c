/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:37  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:35:08  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	scstubs.c

	C stub routines.

	The stubs contained here are the ones that are relatively simple,
	i.e. involving only a call convention change or integer-to-string
	conversion.
*/

#include "plstubs.h"

void 
PLADV(PLINT *sub)
{
    c_pladv(*sub);
}

void 
PLANCOL_( PLINT *icolor, long *name )
{
    long2str(name, cstring1);
    c_plancol(*icolor, cstring1);
}

void 
PLAXES_( PLFLT *x0, PLFLT *y0, long *xopt, PLFLT *xtick,
	PLINT *nxsub, long *yopt, PLFLT *ytick, PLINT *nysub)
{
    long2str(xopt, cstring1);
    long2str(yopt, cstring2);
    c_plaxes(*x0, *y0, cstring1, *xtick, *nxsub, cstring2, *ytick, *nysub);
}

void 
PLBEG( PLINT *dev, PLINT *nx, PLINT *ny )
{
    c_plbeg(*dev, *nx, *ny);
}

void 
PLBIN( PLINT *nbin, PLFLT *x, PLFLT *y, PLINT *center)
{
    c_plbin(*nbin, x, y, *center);
}

void 
PLBOX_( long *xopt, PLFLT *xtick, PLINT *nxsub,
       long *yopt, PLFLT *ytick, PLINT *nysub )
{
    long2str(xopt, cstring1);
    long2str(yopt, cstring2);
    c_plbox(cstring1, *xtick, *nxsub, cstring2, *ytick, *nysub);
}

void 
PLBOX3_(long *xopt, long *xlabel, PLFLT *xtick, PLINT *nxsub,
	long *yopt, long *ylabel, PLFLT *ytick, PLINT *nysub,
	long *zopt, long *zlabel, PLFLT *ztick, PLINT *nzsub)
{
    long2str(xopt, cstring1);
    long2str(xlabel, cstring2);

    long2str(yopt, cstring3);
    long2str(ylabel, cstring4);

    long2str(zopt, cstring5);
    long2str(zlabel, cstring6);

    c_plbox3(cstring1, cstring2, *xtick, *nxsub,
	    cstring3, cstring4, *ytick, *nysub,
	    cstring5, cstring6, *ztick, *nzsub);
}

void 
PLCLR()
{
    c_plclr();
}

void 
PLCOL(PLINT *color)
{
    c_plcol(*color);
}

void 
PLEND()
{
    c_plend();
}

void 
PLEND1 (void)
{
    c_plend1();
}

void 
PLENV( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
      PLINT *just, PLINT *axis )
{
    c_plenv(*xmin, *xmax, *ymin, *ymax, *just, *axis);
}

void 
PLERRX(  PLINT *n, PLFLT *xmin, PLFLT *xmax, PLFLT *y)
{
    c_plerrx(*n, xmin, xmax, y);
}

void 
PLERRY( PLINT *n, PLFLT *x, PLFLT *ymin, PLFLT *ymax)
{
    c_plerry(*n, x, ymin, ymax);
}

void 
PLFILL( PLINT *n, PLFLT *x, PLFLT *y )
{
    c_plfill(*n, x, y);
}

void 
PLFONT(PLINT *font)
{
    c_plfont(*font);
}

void 
PLFONTLD(PLINT *charset)
{
    c_plfontld(*charset);
}

void 
PLGRA()
{
    c_plgra();
}

void 
PLGFAM( PLINT *fam, PLINT *num, PLINT *bmax )
{
    c_plgfam(fam, num, bmax);
}

void 
PLGFNAM_(long *fnam)
{
    c_plgfnam(cstring1);
    str2long(cstring1, fnam);
}

void 
PLGPAGE( PLFLT *xpmm, PLFLT *ypmm, PLINT *xwid, PLINT *ywid,
	PLINT *xoff, PLINT *yoff )
{
    c_plgpage(xpmm, ypmm, xwid, ywid, xoff, yoff);
}

void 
PLGSPA( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    c_plgspa(xmin, xmax, ymin, ymax);
}

void 
PLGSTRM(PLINT *strm)
{
    c_plgstrm(strm);
}

void 
PLGXAX( PLINT *digmax, PLINT *digits )
{
    c_plgxax(digmax, digits);
}

void 
PLGYAX( PLINT *digmax, PLINT *digits )
{
    c_plgyax(digmax, digits);
}

void 
PLGZAX( PLINT *digmax, PLINT *digits )
{
    c_plgzax(digmax, digits);
}

void 
PLHIST( PLINT *n, PLFLT *data, PLFLT *datmin, PLFLT *datmax,
       PLINT *nbin, PLINT *oldwin )
{
    c_plhist(*n, data, *datmin, *datmax, *nbin, *oldwin);
}

void 
PLHLS( PLFLT *hue, PLFLT *light, PLFLT *sat )
{
    c_plhls(*hue, *light, *sat);
}

void 
PLJOIN( PLFLT *x1, PLFLT *y1, PLFLT *x2, PLFLT *y2 )
{
    c_pljoin(*x1, *y1, *x2, *y2);
}

void 
PLLAB_( long *xlab, long *ylab, long *title )
{
    long2str(xlab, cstring1);
    long2str(ylab, cstring2);
    long2str(title, cstring3);
    c_pllab(cstring1, cstring2, cstring3);
}

void 
PLLINE( PLINT *n, PLFLT *x, PLFLT *y )
{
    c_plline(*n, x, y);
}

void 
PLLSTY(PLINT *lin)
{
    c_pllsty(*lin);
}

void 
PLMTEX_( long *side, PLFLT *disp, PLFLT *pos, PLFLT *just, long *text )
{
    long2str(side, cstring1);
    long2str(text, cstring2);
    c_plmtex(cstring1, *disp, *pos, *just, cstring2);
}

void 
PLPAT( PLINT *nlin, PLINT *inc, PLINT *del )
{
    c_plpat(*nlin, inc, del);
}

void 
PLPOIN( PLINT *n, PLFLT *x, PLFLT *y, PLINT *code )
{
    c_plpoin(*n, x, y, *code);
}

void 
PLPREC( PLINT *setp, PLINT *prec )
{
    c_plprec(*setp, *prec);
}

void 
PLPSTY(PLINT *patt)
{
    c_plpsty(*patt);
}

void 
PLPTEX_( PLFLT *x, PLFLT *y, PLFLT *dx, PLFLT *dy, PLFLT *just, long *text )
{
    long2str(text, cstring1);
    c_plptex(*x, *y, *dx, *dy, *just, cstring1);
}

void 
PLRGB( PLFLT *red, PLFLT *green, PLFLT *blue )
{
    c_plrgb(*red, *green, *blue);
}

void 
PLSCHR( PLFLT *def, PLFLT *scale )
{
    c_plschr(*def, *scale);
}

void 
PLSFAM( PLINT *fam, PLINT *num, PLINT *bmax )
{
    c_plsfam(*fam, *num, *bmax);
}

void 
PLSFNAM_( long *fnam )
{
    long2str(fnam, cstring1);
    c_plsfnam(cstring1);
}

void 
PLSASP( PLFLT *aspect )
{
    c_plsasp(*aspect);
}

int 
PLSLPB (PLINT *xmi, PLINT *xma, PLINT *ymi, PLINT *yma)
{
    c_plslpb(*xmi, *xma, *ymi, *yma);
}

void 
PLSMAJ( PLFLT *def, PLFLT *scale )
{
    c_plsmaj(*def, *scale);
}

void 
PLSMIN( PLFLT *def, PLFLT *scale )
{
    c_plsmin(*def, *scale);
}

void 
PLSORI( PLINT *ori )
{
    c_plsori(*ori);
}

void 
PLSPAGE( PLFLT *xpmm, PLFLT *ypmm,
	PLINT *xwid, PLINT *ywid, PLINT *xoff, PLINT *yoff )
{
    c_plspage(*xpmm, *ypmm, *xwid, *ywid, *xoff, *yoff);
}

void 
PLSPAUSE (PLINT *pause)
{
    c_plspause(*pause);
}

void 
PLSSTRM( PLINT *strm )
{
    c_plsstrm(*strm);
}

void 
PLSSYM( PLFLT *def, PLFLT *scale )
{
    c_plssym(*def, *scale);
}

void 
PLSTAR( PLINT *nx, PLINT *ny )
{
    c_plstar(*nx, *ny);
}

void 
PLSTART_( long *devname, PLINT *nx, PLINT *ny )
{
    long2str(devname, cstring1);
    c_plstart(cstring1, *nx, *ny);
}

void 
PLSTYL( PLINT *n, PLINT *mark, PLINT *space )
{
    c_plstyl(*n, mark, space);
}

void 
PLSVPA( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax )
{
    c_plsvpa(*xmin, *xmax, *ymin, *ymax);
}

void 
PLSXAX( PLINT *digmax, PLINT *digits )
{
    c_plsxax(*digmax, *digits);
}

void 
PLSYAX( PLINT *digmax, PLINT *digits )
{
    c_plsyax(*digmax, *digits);
}

void 
PLSYM( PLINT *n, PLFLT *x, PLFLT *y, PLINT *code )
{
    c_plsym(*n, x, y, *code);
}

void 
PLSZAX( PLINT *digmax, PLINT *digits )
{
    c_plszax(*digmax, *digits);
}

void 
PLTEXT()
{
    c_pltext();
}

void 
PLVASP( PLFLT *aspect )
{
    c_plvasp(*aspect);
}

void 
PLVPAS( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLFLT *aspect )
{
    c_plvpas(*xmin, *xmax, *ymin, *ymax, *aspect);
}

void 
PLVPOR( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax )
{
    c_plvpor(*xmin, *xmax, *ymin, *ymax);
}

void 
PLVSTA()
{
    c_plvsta();
}

void 
PLW3D( PLFLT *basex, PLFLT *basey, PLFLT *height,
      PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
      PLFLT *zmin, PLFLT *zmax,
      PLFLT *alt, PLFLT *az )
{
    c_plw3d(*basex, *basey, *height, *xmin, *xmax, *ymin, *ymax, *zmin, *zmax, *alt, *az);
}

void 
PLWID( PLINT *width )
{
    c_plwid(*width);
}

void 
PLWIND( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax )
{
    c_plwind(*xmin, *xmax, *ymin, *ymax);
}
