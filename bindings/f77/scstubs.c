/* $Id$
 * $Log$
 * Revision 1.20  1995/01/14 06:04:05  mjl
 * Fixed plscmap1l documentation and changed call syntax to pass "rev" array.
 *
 * Revision 1.19  1994/07/20  06:08:03  mjl
 * Added Fortran interface routines to handle the new 3d functions plline3(),
 * plpoin3(), and plpoly3().
 *
 * Revision 1.18  1994/03/23  08:39:10  mjl
 * Fixed some stubs to the cmap 1 handlers.
 *
 * Revision 1.17  1993/12/06  07:47:34  mjl
 * Changed routine names used in new color model.
 *
 * Revision 1.16  1993/11/15  08:40:57  mjl
 * Added stub for PLSCOLBG.
 *
 * Revision 1.15  1993/10/21  19:27:07  mjl
 * Changed all names that ended with an underscore (as part of the C/Fortran
 * linkage) to end with a '7' instead, to avoid problems with f2c.
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
PLAXES7(PLFLT *x0, PLFLT *y0, char *xopt, PLFLT *xtick,
	PLINT *nxsub, char *yopt, PLFLT *ytick, PLINT *nysub)
{
    c_plaxes(*x0, *y0, xopt, *xtick, *nxsub, yopt, *ytick, *nysub);
}

void
PLBIN(PLINT *nbin, PLFLT *x, PLFLT *y, PLINT *center)
{
    c_plbin(*nbin, x, y, *center);
}

void
PLBOP()
{
    c_plbop();
}

void
PLBOX7(char *xopt, PLFLT *xtick, PLINT *nxsub,
       char *yopt, PLFLT *ytick, PLINT *nysub)
{
    c_plbox(xopt, *xtick, *nxsub, yopt, *ytick, *nysub);
}

void
PLBOX37(char *xopt, char *xlabel, PLFLT *xtick, PLINT *nxsub,
	char *yopt, char *ylabel, PLFLT *ytick, PLINT *nysub,
	char *zopt, char *zlabel, PLFLT *ztick, PLINT *nzsub)
{
    c_plbox3(xopt, xlabel, *xtick, *nxsub,
	     yopt, ylabel, *ytick, *nysub,
	     zopt, zlabel, *ztick, *nzsub);
}

void
PLCOL0(PLINT *icol)
{
    c_plcol0(*icol);
}

void
PLCOL1(PLFLT *col)
{
    c_plcol1(*col);
}

/* The old way, same as plcol0 */

void
PLCOL(PLINT *icol)
{
    c_plcol0(*icol);
}

void
PLEND()
{
    c_plend();
}

void
PLEND1(void)
{
    c_plend1();
}

void
PLENV(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
      PLINT *just, PLINT *axis)
{
    c_plenv(*xmin, *xmax, *ymin, *ymax, *just, *axis);
}

void
PLEOP()
{
    c_pleop();
}

void
PLERRX(PLINT *n, PLFLT *xmin, PLFLT *xmax, PLFLT *y)
{
    c_plerrx(*n, xmin, xmax, y);
}

void
PLERRY(PLINT *n, PLFLT *x, PLFLT *ymin, PLFLT *ymax)
{
    c_plerry(*n, x, ymin, ymax);
}

void
PLFAMADV(void)
{
    c_plfamadv();
}

void
PLFILL(PLINT *n, PLFLT *x, PLFLT *y)
{
    c_plfill(*n, x, y);
}

void
PLFLUSH(void)
{
    c_plflush();
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
PLGCHR(PLFLT *chrdef, PLFLT *chrht)
{
    c_plgchr(chrdef, chrht);
}

void
PLGFAM(PLINT *fam, PLINT *num, PLINT *bmax)
{
    c_plgfam(fam, num, bmax);
}

void
PLGFNAM7(char *fnam)
{
    c_plgfnam(fnam);
}

void
PLGPAGE(PLFLT *xpmm, PLFLT *ypmm, PLINT *xwid, PLINT *ywid,
	PLINT *xoff, PLINT *yoff)
{
    c_plgpage(xpmm, ypmm, xwid, ywid, xoff, yoff);
}

void
PLGRA()
{
    c_plgra();
}

void
PLGSPA(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    c_plgspa(xmin, xmax, ymin, ymax);
}

void
PLGSTRM(PLINT *strm)
{
    c_plgstrm(strm);
}

void
PLGVER7(char *ver)
{
    c_plgver(ver);
}

void
PLGXAX(PLINT *digmax, PLINT *digits)
{
    c_plgxax(digmax, digits);
}

void
PLGYAX(PLINT *digmax, PLINT *digits)
{
    c_plgyax(digmax, digits);
}

void
PLGZAX(PLINT *digmax, PLINT *digits)
{
    c_plgzax(digmax, digits);
}

void
PLHIST(PLINT *n, PLFLT *data, PLFLT *datmin, PLFLT *datmax,
       PLINT *nbin, PLINT *oldwin)
{
    c_plhist(*n, data, *datmin, *datmax, *nbin, *oldwin);
}

void
PLHLS(PLFLT *hue, PLFLT *light, PLFLT *sat)
{
    c_plhls(*hue, *light, *sat);
}

void
PLINIT(void)
{
    c_plinit();
}

void
PLJOIN(PLFLT *x1, PLFLT *y1, PLFLT *x2, PLFLT *y2)
{
    c_pljoin(*x1, *y1, *x2, *y2);
}

void
PLLAB7(char *xlab, char *ylab, char *title)
{
    c_pllab(xlab, ylab, title);
}

void
PLLINE(PLINT *n, PLFLT *x, PLFLT *y)
{
    c_plline(*n, x, y);
}

void
PLLINE3(PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z)
{
    c_plline3(*n, x, y, z);
}

void
PLLSTY(PLINT *lin)
{
    c_pllsty(*lin);
}

void
PLMTEX7(char *side, PLFLT *disp, PLFLT *pos, PLFLT *just, char *text)
{
    c_plmtex(side, *disp, *pos, *just, text);
}

void
PLPAT(PLINT *nlin, PLINT *inc, PLINT *del)
{
    c_plpat(*nlin, inc, del);
}

void
PLPOIN(PLINT *n, PLFLT *x, PLFLT *y, PLINT *code)
{
    c_plpoin(*n, x, y, *code);
}

void
PLPOIN3(PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT *code)
{
    c_plpoin3(*n, x, y, z, *code);
}

void
PLPOLY3(PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT *draw)
{
    c_plpoly3(*n, x, y, z, draw);
}

void
PLPREC(PLINT *setp, PLINT *prec)
{
    c_plprec(*setp, *prec);
}

void
PLPSTY(PLINT *patt)
{
    c_plpsty(*patt);
}

void
PLPTEX7(PLFLT *x, PLFLT *y, PLFLT *dx, PLFLT *dy, PLFLT *just, char *text)
{
    c_plptex(*x, *y, *dx, *dy, *just, text);
}

void
PLRGB(PLFLT *red, PLFLT *green, PLFLT *blue)
{
    c_plrgb(*red, *green, *blue);
}

void
PLRGB1(PLINT *r, PLINT *g, PLINT *b)
{
    c_plrgb1(*r, *g, *b);
}

void
PLSASP(PLFLT *aspect)
{
    c_plsasp(*aspect);
}

void
PLSCHR(PLFLT *def, PLFLT *scale)
{
    c_plschr(*def, *scale);
}

void
PLSCMAP0(PLINT *r, PLINT *g, PLINT *b, PLINT *ncol0)
{
    c_plscmap0(r, g, b, *ncol0);
}

void
PLSCMAP1(PLINT *r, PLINT *g, PLINT *b, PLINT *ncol1)
{
    c_plscmap1(r, g, b, *ncol1);
}

void
PLSCMAP0N(PLINT *n)
{
    c_plscmap0n(*n);
}

void
PLSCMAP1L(PLINT *itype, PLINT *npts, PLFLT *intensity,
	  PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLINT *rev)
{
    c_plscmap1l(*itype, *npts, intensity, coord1, coord2, coord3, rev);
}

void
PLSCOL0(PLINT *icol0, PLINT *r, PLINT *g, PLINT *b)
{
    c_plscol0(*icol0, *r, *g, *b);
}

void
PLSCOLBG(PLINT *r, PLINT *g, PLINT *b)
{
    c_plscolbg(*r, *g, *b);
}

void
PLSCOLOR(PLINT *color)
{
    c_plscolor(*color);
}

void
PLSDEV7(char *dev)
{
    c_plsdev(dev);
}

void
PLSESC7(PLINT *esc)
{
    c_plsesc((char) *esc);
}

void
PLSFAM(PLINT *fam, PLINT *num, PLINT *bmax)
{
    c_plsfam(*fam, *num, *bmax);
}

void
PLSFNAM7(char *fnam)
{
    c_plsfnam(fnam);
}

void
PLSLPB(PLINT *xmi, PLINT *xma, PLINT *ymi, PLINT *yma)
{
    c_plslpb(*xmi, *xma, *ymi, *yma);
}

void
PLSMAJ(PLFLT *def, PLFLT *scale)
{
    c_plsmaj(*def, *scale);
}

void
PLSMIN(PLFLT *def, PLFLT *scale)
{
    c_plsmin(*def, *scale);
}

void
PLSORI(PLINT *ori)
{
    c_plsori(*ori);
}

void
PLSPAGE(PLFLT *xpmm, PLFLT *ypmm,
	PLINT *xwid, PLINT *ywid, PLINT *xoff, PLINT *yoff)
{
    c_plspage(*xpmm, *ypmm, *xwid, *ywid, *xoff, *yoff);
}

void
PLSPAUSE(PLINT *pause)
{
    c_plspause(*pause);
}

void
PLSSTRM(PLINT *strm)
{
    c_plsstrm(*strm);
}

void
PLSSUB(PLINT *nx, PLINT *ny)
{
    c_plssub(*nx, *ny);
}

void
PLSSYM(PLFLT *def, PLFLT *scale)
{
    c_plssym(*def, *scale);
}

void
PLSTAR(PLINT *nx, PLINT *ny)
{
    c_plstar(*nx, *ny);
}

void
PLSTART7(char *devname, PLINT *nx, PLINT *ny)
{
    c_plstart(devname, *nx, *ny);
}

void
PLSTYL(PLINT *n, PLINT *mark, PLINT *space)
{
    c_plstyl(*n, mark, space);
}

void
PLSVPA(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    c_plsvpa(*xmin, *xmax, *ymin, *ymax);
}

void
PLSXAX(PLINT *digmax, PLINT *digits)
{
    c_plsxax(*digmax, *digits);
}

void
PLSYAX(PLINT *digmax, PLINT *digits)
{
    c_plsyax(*digmax, *digits);
}

void
PLSYM(PLINT *n, PLFLT *x, PLFLT *y, PLINT *code)
{
    c_plsym(*n, x, y, *code);
}

void
PLSZAX(PLINT *digmax, PLINT *digits)
{
    c_plszax(*digmax, *digits);
}

void
PLTEXT()
{
    c_pltext();
}

void
PLVASP(PLFLT *aspect)
{
    c_plvasp(*aspect);
}

void
PLVPAS(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLFLT *aspect)
{
    c_plvpas(*xmin, *xmax, *ymin, *ymax, *aspect);
}

void
PLVPOR(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    c_plvpor(*xmin, *xmax, *ymin, *ymax);
}

void
PLVSTA()
{
    c_plvsta();
}

void
PLW3D(PLFLT *basex, PLFLT *basey, PLFLT *height,
      PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
      PLFLT *zmin, PLFLT *zmax,
      PLFLT *alt, PLFLT *az)
{
    c_plw3d(*basex, *basey, *height,
	    *xmin, *xmax, *ymin, *ymax, *zmin, *zmax, *alt, *az);
}

void
PLWID(PLINT *width)
{
    c_plwid(*width);
}

void
PLWIND(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    c_plwind(*xmin, *xmax, *ymin, *ymax);
}
