/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:35:08  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	scstubs.c

	C stub routines.

	The stubs contained here are the ones that are relatively simple,
	i.e. involving only a call convention change or integer-to-string
	conversion.
*/

#include "plstubs.h"

void 
PLADV(sub)
PLINT *sub;
{
    c_pladv(*sub);
}

void 
PLANCOL_(icolor, name)
PLINT *icolor;
long *name;
{
    long2str(name, cstring1);
    c_plancol(*icolor, cstring1);
}

void 
PLAXES_(x0, y0, xopt, xtick, nxsub, yopt, ytick, nysub)
long *xopt, *yopt;
PLFLT *x0, *y0, *xtick, *ytick;
PLINT *nxsub, *nysub;
{
    long2str(xopt, cstring1);
    long2str(yopt, cstring2);
    c_plaxes(*x0, *y0, cstring1, *xtick, *nxsub, cstring2, *ytick, *nysub);
}

void 
PLBEG(dev, nx, ny)
PLINT *dev, *nx, *ny;
{
    c_plbeg(*dev, *nx, *ny);
}

void 
PLBIN(nbin, x, y, center)
PLINT *nbin, *center;
PLFLT *x, *y;
{
    c_plbin(*nbin, x, y, *center);
}

void 
PLBOX_(xopt, xtick, nxsub, yopt, ytick, nysub)
long *xopt, *yopt;
PLFLT *xtick, *ytick;
PLINT *nxsub, *nysub;
{
    long2str(xopt, cstring1);
    long2str(yopt, cstring2);
    c_plbox(cstring1, *xtick, *nxsub, cstring2, *ytick, *nysub);
}

void 
PLBOX3_(xopt, xlabel, xtick, nxsub, yopt, ylabel, ytick, nysub,
	zopt, zlabel, ztick, nzsub)
long *xopt, *xlabel, *yopt, *ylabel, *zopt, *zlabel;
PLFLT *xtick, *ytick, *ztick;
PLINT *nxsub, *nysub, *nzsub;
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
PLCOL(color)
PLINT *color;
{
    c_plcol(*color);
}

void 
PLEND()
{
    c_plend();
}

void 
PLEND1()
{
    c_plend1();
}

void 
PLENV(xmin, xmax, ymin, ymax, just, axis)
PLFLT *xmin, *xmax, *ymin, *ymax;
PLINT *just, *axis;
{
    c_plenv(*xmin, *xmax, *ymin, *ymax, *just, *axis);
}

void 
PLERRX(n, xmin, xmax, y)
PLINT *n;
PLFLT *xmin, *xmax, *y;
{
    c_plerrx(*n, xmin, xmax, y);
}

void 
PLERRY(n, x, ymin, ymax)
PLINT *n;
PLFLT *x, *ymin, *ymax;
{
    c_plerry(*n, x, ymin, ymax);
}

void 
PLFILL(n, x, y)
PLINT *n;
PLFLT *x, *y;
{
    c_plfill(*n, x, y);
}

void 
PLFONT(font)
PLINT *font;
{
    c_plfont(*font);
}

void 
PLFONTLD(charset)
PLINT *charset;
{
    c_plfontld(*charset);
}

void 
PLGRA()
{
    c_plgra();
}

void 
PLGFAM(fam, num, bmax)
PLINT *fam, *num, *bmax;
{
    c_plgfam(fam, num, bmax);
}

void 
PLGFNAM_(fnam)
long *fnam;
{
    c_plgfnam(cstring1);
    str2long(cstring1, fnam);
}

void 
PLGPAGE(xpmm, ypmm, xwid, ywid, xoff, yoff)
PLINT *xwid, *ywid, *xoff, *yoff;
PLFLT *xpmm, *ypmm;
{
    c_plgpage(xpmm, ypmm, xwid, ywid, xoff, yoff);
}

void 
PLGSPA(xmin, xmax, ymin, ymax)
PLFLT *xmin, *xmax, *ymin, *ymax;
{
    c_plgspa(xmin, xmax, ymin, ymax);
}

void 
PLGSTRM(strm)
PLINT *strm;
{
    c_plgstrm(strm);
}

void 
PLGXAX(digmax, digits)
PLINT *digmax, *digits;
{
    c_plgxax(digmax, digits);
}

void 
PLGYAX(digmax, digits)
PLINT *digmax, *digits;
{
    c_plgyax(digmax, digits);
}

void 
PLGZAX(digmax, digits)
PLINT *digmax, *digits;
{
    c_plgzax(digmax, digits);
}

void 
PLHIST(n, data, datmin, datmax, nbin, oldwin)
PLINT *n, *nbin, *oldwin;
PLFLT *data, *datmin, *datmax;
{
    c_plhist(*n, data, *datmin, *datmax, *nbin, *oldwin);
}

void 
PLHLS(hue, light, sat)
PLFLT *hue, *light, *sat;
{
    c_plhls(*hue, *light, *sat);
}

void 
PLJOIN(x1, y1, x2, y2)
PLFLT *x1, *y1, *x2, *y2;
{
    c_pljoin(*x1, *y1, *x2, *y2);
}

void 
PLLAB_(xlab, ylab, title)
long *xlab, *ylab, *title;
{
    long2str(xlab, cstring1);
    long2str(ylab, cstring2);
    long2str(title, cstring3);
    c_pllab(cstring1, cstring2, cstring3);
}

void 
PLLINE(n, x, y)
PLINT *n;
PLFLT *x, *y;
{
    c_plline(*n, x, y);
}

void 
PLLSTY(lin)
PLINT *lin;
{
    c_pllsty(*lin);
}

void 
PLMTEX_(side, disp, pos, just, text)
long *side, *text;
PLFLT *disp, *pos, *just;
{
    long2str(side, cstring1);
    long2str(text, cstring2);
    c_plmtex(cstring1, *disp, *pos, *just, cstring2);
}

void 
PLPAT(nlin, inc, del)
PLINT *nlin, *inc, *del;
{
    c_plpat(*nlin, inc, del);
}

void 
PLPOIN(n, x, y, code)
PLINT *n, *code;
PLFLT *x, *y;
{
    c_plpoin(*n, x, y, *code);
}

void 
PLPREC(setp, prec)
PLINT *setp, *prec;
{
    c_plprec(*setp, *prec);
}

void 
PLPSTY(patt)
PLINT *patt;
{
    c_plpsty(*patt);
}

void 
PLPTEX_(x, y, dx, dy, just, text)
PLFLT *x, *y, *dx, *dy, *just;
long *text;
{
    long2str(text, cstring1);
    c_plptex(*x, *y, *dx, *dy, *just, cstring1);
}

void 
PLRGB(red, green, blue)
PLFLT *red, *green, *blue;
{
    c_plrgb(*red, *green, *blue);
}

void 
PLSCHR(def, scale)
PLFLT *def, *scale;
{
    c_plschr(*def, *scale);
}

void 
PLSFAM(fam, num, bmax)
PLINT *fam, *num, *bmax;
{
    c_plsfam(*fam, *num, *bmax);
}

void 
PLSFNAM_(fnam)
long *fnam;
{
    long2str(fnam, cstring1);
    c_plsfnam(cstring1);
}

void 
PLSASP(aspect)
PLFLT *aspect;
{
    c_plsasp(*aspect);
}

PLSLPB(xmi, xma, ymi, yma)
PLINT *xmi, *xma, *ymi, *yma;
{
    c_plslpb(*xmi, *xma, *ymi, *yma);
}

void 
PLSMAJ(def, scale)
PLFLT *def, *scale;
{
    c_plsmaj(*def, *scale);
}

void 
PLSMIN(def, scale)
PLFLT *def, *scale;
{
    c_plsmin(*def, *scale);
}

void 
PLSORI(ori)
PLINT *ori;
{
    c_plsori(*ori);
}

void 
PLSPAGE(xpmm, ypmm, xwid, ywid, xoff, yoff)
PLINT *xwid, *ywid, *xoff, *yoff;
PLFLT *xpmm, *ypmm;
{
    c_plspage(*xpmm, *ypmm, *xwid, *ywid, *xoff, *yoff);
}

void 
PLSPAUSE(pause)
PLINT *pause;
{
    c_plspause(*pause);
}

void 
PLSSTRM(strm)
PLINT *strm;
{
    c_plsstrm(*strm);
}

void 
PLSSYM(def, scale)
PLFLT *def, *scale;
{
    c_plssym(*def, *scale);
}

void 
PLSTAR(nx, ny)
PLINT *nx, *ny;
{
    c_plstar(*nx, *ny);
}

void 
PLSTART_(devname, nx, ny)
long *devname;
PLINT *nx, *ny;
{
    long2str(devname, cstring1);
    c_plstart(cstring1, *nx, *ny);
}

void 
PLSTYL(n, mark, space)
PLINT *n, *mark, *space;
{
    c_plstyl(*n, mark, space);
}

void 
PLSVPA(xmin, xmax, ymin, ymax)
PLFLT *xmin, *xmax, *ymin, *ymax;
{
    c_plsvpa(*xmin, *xmax, *ymin, *ymax);
}

void 
PLSXAX(digmax, digits)
PLINT *digmax, *digits;
{
    c_plsxax(*digmax, *digits);
}

void 
PLSYAX(digmax, digits)
PLINT *digmax, *digits;
{
    c_plsyax(*digmax, *digits);
}

void 
PLSYM(n, x, y, code)
PLINT *n, *code;
PLFLT *x, *y;
{
    c_plsym(*n, x, y, *code);
}

void 
PLSZAX(digmax, digits)
PLINT *digmax, *digits;
{
    c_plszax(*digmax, *digits);
}

void 
PLTEXT()
{
    c_pltext();
}

void 
PLVASP(aspect)
PLFLT *aspect;
{
    c_plvasp(*aspect);
}

void 
PLVPAS(xmin, xmax, ymin, ymax, aspect)
PLFLT *xmin, *xmax, *ymin, *ymax, *aspect;
{
    c_plvpas(*xmin, *xmax, *ymin, *ymax, *aspect);
}

void 
PLVPOR(xmin, xmax, ymin, ymax)
PLFLT *xmin, *xmax, *ymin, *ymax;
{
    c_plvpor(*xmin, *xmax, *ymin, *ymax);
}

void 
PLVSTA()
{
    c_plvsta();
}

void 
PLW3D(basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az)
PLFLT *basex, *basey, *height, *xmin, *xmax, *ymin, *ymax, *zmin, *zmax,
*alt, *az;
{
    c_plw3d(*basex, *basey, *height, *xmin, *xmax, *ymin, *ymax, *zmin, *zmax, *alt, *az);
}

void 
PLWID(width)
PLINT *width;
{
    c_plwid(*width);
}

void 
PLWIND(xmin, xmax, ymin, ymax)
PLFLT *xmin, *xmax, *ymin, *ymax;
{
    c_plwind(*xmin, *xmax, *ymin, *ymax);
}
