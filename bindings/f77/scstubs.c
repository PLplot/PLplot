/* $Id$

	C stub routines.

   Copyright (C) 2004  Alan W. Irwin

   This file is part of PLplot.

   PLplot is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Library Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   PLplot is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with PLplot; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA


	The stubs contained here are the ones that are relatively simple,
	i.e. involving only a call convention change or integer-to-string
	conversion.  Exceptions are plparseopts and  plstripc  which have
        a few more complications in them.
*/

#include "plstubs.h"

static void (*plmapform)(PLINT *, PLFLT *, PLFLT *) ; /* Note: slightly different prototype than
                                                         (*mapform)! */

PLDLLEXPORT void
PL_SETCONTLABELFORMAT(PLINT *lexp, PLINT *sigdig)
{
    c_pl_setcontlabelformat(*lexp, *sigdig);
}

PLDLLEXPORT void
PL_SETCONTLABELFORMATa(PLINT *lexp, PLINT *sigdig)
{
    c_pl_setcontlabelformat(*lexp, *sigdig);
}

PLDLLEXPORT void
PL_SETCONTLABELPARAM(PLFLT *offset, PLFLT *size, PLFLT *spacing, PLINT *active)
{
    c_pl_setcontlabelparam(*offset, *size, *spacing, *active);
}

PLDLLEXPORT void
PL_SETCONTLABELPARAMa(PLFLT *offset, PLFLT *size, PLFLT *spacing, PLINT *active)
{
    c_pl_setcontlabelparam(*offset, *size, *spacing, *active);
}

PLDLLEXPORT void
PLADV(PLINT *sub)
{
    c_pladv(*sub);
}

PLDLLEXPORT void
PLAXES7(PLFLT *x0, PLFLT *y0, char *xopt, PLFLT *xtick,
	PLINT *nxsub, char *yopt, PLFLT *ytick, PLINT *nysub)
{
    c_plaxes(*x0, *y0, xopt, *xtick, *nxsub, yopt, *ytick, *nysub);
}

PLDLLEXPORT void
PLBIN(PLINT *nbin, PLFLT *x, PLFLT *y, PLINT *center)
{
    c_plbin(*nbin, x, y, *center);
}

PLDLLEXPORT void
PLBOP(void)
{
    c_plbop();
}

PLDLLEXPORT void
PLBOX7(char *xopt, PLFLT *xtick, PLINT *nxsub,
       char *yopt, PLFLT *ytick, PLINT *nysub)
{
    c_plbox(xopt, *xtick, *nxsub, yopt, *ytick, *nysub);
}

PLDLLEXPORT void
PLBOX37(char *xopt, char *xlabel, PLFLT *xtick, PLINT *nxsub,
	char *yopt, char *ylabel, PLFLT *ytick, PLINT *nysub,
	char *zopt, char *zlabel, PLFLT *ztick, PLINT *nzsub)
{
    c_plbox3(xopt, xlabel, *xtick, *nxsub,
	     yopt, ylabel, *ytick, *nysub,
	     zopt, zlabel, *ztick, *nzsub);
}

PLDLLEXPORT void
PLCALC_WORLD(PLFLT *rx, PLFLT *ry, PLFLT *wx, PLFLT *wy, PLINT *window)
{
    c_plcalc_world(*rx, *ry, wx, wy, window);
}

PLDLLEXPORT void
PLCALC_WORLDa(PLFLT *rx, PLFLT *ry, PLFLT *wx, PLFLT *wy, PLINT *window)
{
    c_plcalc_world(*rx, *ry, wx, wy, window);
}

PLDLLEXPORT void
PLCLEAR(void)
{
    c_plclear();
}

PLDLLEXPORT void
PLCOL0(PLINT *icol)
{
    c_plcol0(*icol);
}

PLDLLEXPORT void
PLCOL1(PLFLT *col)
{
    c_plcol1(*col);
}

/* The old way, same as plcol0 */

PLDLLEXPORT void
PLCOL(PLINT *icol)
{
    c_plcol0(*icol);
}

PLDLLEXPORT void
PLCPSTRM(PLINT *iplsr, PLBOOL *flags)
{
    c_plcpstrm(*iplsr, *flags);
}

PLDLLEXPORT void
PLEND(void)
{
    c_plend();
}

PLDLLEXPORT void
PLEND1(void)
{
    c_plend1();
}

PLDLLEXPORT void
PLENV(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
      PLINT *just, PLINT *axis)
{
    c_plenv(*xmin, *xmax, *ymin, *ymax, *just, *axis);
}

PLDLLEXPORT void
PLEOP(void)
{
    c_pleop();
}

PLDLLEXPORT void
PLERRX(PLINT *n, PLFLT *xmin, PLFLT *xmax, PLFLT *y)
{
    c_plerrx(*n, xmin, xmax, y);
}

PLDLLEXPORT void
PLERRY(PLINT *n, PLFLT *x, PLFLT *ymin, PLFLT *ymax)
{
    c_plerry(*n, x, ymin, ymax);
}

PLDLLEXPORT void
PLFAMADV(void)
{
    c_plfamadv();
}

PLDLLEXPORT void
PLFILL(PLINT *n, PLFLT *x, PLFLT *y)
{
    c_plfill(*n, x, y);
}

PLDLLEXPORT void
PLFILL3(PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z)
{
    c_plfill3(*n, x, y, z);
}

PLDLLEXPORT void
PLFLUSH(void)
{
    c_plflush();
}

PLDLLEXPORT void
PLFONT(PLINT *font)
{
    c_plfont(*font);
}

PLDLLEXPORT void
PLFONTLD(PLINT *charset)
{
    c_plfontld(*charset);
}

PLDLLEXPORT void
PLGCHR(PLFLT *chrdef, PLFLT *chrht)
{
    c_plgchr(chrdef, chrht);
}

PLDLLEXPORT void
PLGCOL0(PLINT *icol0, PLINT *r, PLINT *g, PLINT *b)
{
    c_plgcol0(*icol0, r, g, b);
}

PLDLLEXPORT void
PLGCOLBG(PLINT *r, PLINT *g, PLINT *b)
{
    c_plgcolbg(r, g, b);
}

PLDLLEXPORT void
PLGCOMPRESSION(PLINT *compression)
{
    c_plgcompression(compression);
}

PLDLLEXPORT void
PLGDEV7(char *dev, int length)
{
    c_plgdev(dev);
}

PLDLLEXPORT void
PLGDIDEV(PLFLT *p_mar, PLFLT *p_aspect, PLFLT *p_jx, PLFLT *p_jy)
{
    c_plgdidev(p_mar, p_aspect, p_jx, p_jy);
}

PLDLLEXPORT void
PLGDIORI(PLFLT *p_rot)
{
    c_plgdiori(p_rot);
}

PLDLLEXPORT void
PLGDIPLT(PLFLT *p_xmin, PLFLT *p_ymin, PLFLT *p_xmax, PLFLT *p_ymax)
{
    c_plgdiplt(p_xmin, p_ymin, p_xmax, p_ymax);
}

PLDLLEXPORT void
PLGFAM(PLINT *fam, PLINT *num, PLINT *bmax)
{
    c_plgfam(fam, num, bmax);
}

PLDLLEXPORT void
PLGFNAM7(char *fnam, int length)
{
    c_plgfnam(fnam);
}

PLDLLEXPORT void
PLGLEVEL(PLINT *level)
{
    c_plglevel(level);
}

PLDLLEXPORT void
PLGPAGE(PLFLT *xpmm, PLFLT *ypmm, PLINT *xwid, PLINT *ywid,
	PLINT *xoff, PLINT *yoff)
{
    c_plgpage(xpmm, ypmm, xwid, ywid, xoff, yoff);
}

PLDLLEXPORT void
PLGRA(void)
{
    c_plgra();
}

PLDLLEXPORT void
PLGRIDDATA(PLFLT *x, PLFLT *y, PLFLT *z, PLINT *npts,
	   PLFLT *xg, PLINT *nptsx, PLFLT *yg,  PLINT *nptsy,
	   PLFLT **zg, PLINT *type, PLFLT *data)
{
    c_plgriddata(x, y, z, *npts,
		 xg, *nptsx, yg, *nptsy,
		 zg, *type, *data);
}

PLDLLEXPORT void
PLGSPA(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    c_plgspa(xmin, xmax, ymin, ymax);
}

PLDLLEXPORT void
PLGSTRM(PLINT *strm)
{
    c_plgstrm(strm);
}

PLDLLEXPORT void
PLGVER7(char *ver)
{
    c_plgver(ver);
}

PLDLLEXPORT void
PLGVPD(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    c_plgvpd(p_xmin, p_xmax, p_ymin, p_ymax);
}

PLDLLEXPORT void
PLGVPW(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    c_plgvpw(p_xmin, p_xmax, p_ymin, p_ymax);
}

PLDLLEXPORT void
PLGXAX(PLINT *digmax, PLINT *digits)
{
    c_plgxax(digmax, digits);
}

PLDLLEXPORT void
PLGYAX(PLINT *digmax, PLINT *digits)
{
    c_plgyax(digmax, digits);
}

PLDLLEXPORT void
PLGZAX(PLINT *digmax, PLINT *digits)
{
    c_plgzax(digmax, digits);
}

PLDLLEXPORT void
PLHIST(PLINT *n, PLFLT *data, PLFLT *datmin, PLFLT *datmax,
       PLINT *nbin, PLINT *oldwin)
{
    c_plhist(*n, data, *datmin, *datmax, *nbin, *oldwin);
}

PLDLLEXPORT void
PLHLS(PLFLT *hue, PLFLT *light, PLFLT *sat)
{
    c_plhls(*hue, *light, *sat);
}

PLDLLEXPORT void
PLHLSRGB(PLFLT *h, PLFLT *l, PLFLT *s, PLFLT *r, PLFLT *g, PLFLT *b)
{
    c_plhlsrgb(*h, *l, *s, r, g, b);
}

PLDLLEXPORT void
PLINIT(void)
{
    c_plinit();
}

PLDLLEXPORT void
PLJOIN(PLFLT *x1, PLFLT *y1, PLFLT *x2, PLFLT *y2)
{
    c_pljoin(*x1, *y1, *x2, *y2);
}

PLDLLEXPORT void
PLLAB7(char *xlab, char *ylab, char *title)
{
    c_pllab(xlab, ylab, title);
}

PLDLLEXPORT void
PLLIGHTSOURCE(PLFLT *x, PLFLT *y, PLFLT *z)
{
    c_pllightsource(*x, *y, *z);
}

PLDLLEXPORT void
PLLINE(PLINT *n, PLFLT *x, PLFLT *y)
{
    c_plline(*n, x, y);
}

PLDLLEXPORT void
PLLINE3(PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z)
{
    c_plline3(*n, x, y, z);
}

PLDLLEXPORT void
PLLSTY(PLINT *lin)
{
    c_pllsty(*lin);
}

static void
plmapf2c( PLINT n, PLFLT *x, PLFLT *y )
{
    (*plmapform)( &n, x, y );
}

PLDLLEXPORT void
PLMAPC( char *type,
        PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat )

{
    c_plmap(plmapf2c, type, *minlong, *maxlong, *minlat, *maxlat);
}


PLDLLEXPORT void
PLMERIDIANSC( PLFLT *dlong, PLFLT *dlat,
              PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat )
{
    c_plmeridians(plmapf2c, *dlong, *dlat, *minlong, *maxlong, *minlat, *maxlat);
}

PLDLLEXPORT void
PLMKSTRM(PLINT *p_strm)
{
    c_plmkstrm(p_strm);
}

PLDLLEXPORT void
PLMTEX7(char *side, PLFLT *disp, PLFLT *pos, PLFLT *just, char *text)
{
    c_plmtex(side, *disp, *pos, *just, text);
}

PLDLLEXPORT void
PLPARSEOPTS7(int *numargs, char *iargs, PLINT *mode, PLINT *maxindex)
{
/* Same as in plparseopts fortran subroutine that calls this one. */
#define MAXARGS 20
   if(*numargs <= MAXARGS) {
      char *argv[MAXARGS];
      int i;
      for(i = 0; i < *numargs; i++) {
	 argv[i] = iargs + (i* *maxindex);
/*	 fprintf(stderr, "%d - %s\n", i, argv[i]); */
      }
      c_plparseopts(numargs, argv, *mode);
   }  else
     fprintf(stderr,"plparseopts7: numargs too large\n");
}

PLDLLEXPORT void
PLPAT(PLINT *nlin, PLINT *inc, PLINT *del)
{
    c_plpat(*nlin, inc, del);
}

PLDLLEXPORT void
PLPOIN(PLINT *n, PLFLT *x, PLFLT *y, PLINT *code)
{
    c_plpoin(*n, x, y, *code);
}

PLDLLEXPORT void
PLPOIN3(PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT *code)
{
    c_plpoin3(*n, x, y, z, *code);
}

PLDLLEXPORT void
PLPOLY3(PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, PLBOOL *draw, PLBOOL *ifcc)
{
    c_plpoly3(*n, x, y, z, draw, *ifcc);
}

PLDLLEXPORT void
PLPREC(PLINT *setp, PLINT *prec)
{
    c_plprec(*setp, *prec);
}

PLDLLEXPORT void
PLPSTY(PLINT *patt)
{
    c_plpsty(*patt);
}

PLDLLEXPORT void
PLPTEX7(PLFLT *x, PLFLT *y, PLFLT *dx, PLFLT *dy, PLFLT *just, char *text)
{
    c_plptex(*x, *y, *dx, *dy, *just, text);
}

PLDLLEXPORT void
PLREPLOT(void)
{
    c_plreplot();
}

PLDLLEXPORT void
PLRGB(PLFLT *red, PLFLT *green, PLFLT *blue)
{
    c_plrgb(*red, *green, *blue);
}

PLDLLEXPORT void
PLRGB1(PLINT *r, PLINT *g, PLINT *b)
{
    c_plrgb1(*r, *g, *b);
}

PLDLLEXPORT void
PLRGBHLS(PLFLT *r, PLFLT *g, PLFLT *b, PLFLT *h, PLFLT *l, PLFLT *s)
{
    c_plrgbhls(*r, *g, *b, h, l, s);
}

PLDLLEXPORT void
PLSCHR(PLFLT *def, PLFLT *scale)
{
    c_plschr(*def, *scale);
}

PLDLLEXPORT void
PLSCMAP0(PLINT *r, PLINT *g, PLINT *b, PLINT *ncol0)
{
    c_plscmap0(r, g, b, *ncol0);
}

PLDLLEXPORT void
PLSCMAP0N(PLINT *n)
{
    c_plscmap0n(*n);
}

PLDLLEXPORT void
PLSCMAP1(PLINT *r, PLINT *g, PLINT *b, PLINT *ncol1)
{
    c_plscmap1(r, g, b, *ncol1);
}

PLDLLEXPORT void
PLSCMAP1L(PLBOOL *itype, PLINT *npts, PLFLT *intensity,
	  PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLBOOL *rev)
{
    c_plscmap1l(*itype, *npts, intensity, coord1, coord2, coord3, rev);
}

PLDLLEXPORT void
PLSCMAP1N(PLINT *n)
{
    c_plscmap1n(*n);
}

PLDLLEXPORT void
PLSCOL0(PLINT *icol0, PLINT *r, PLINT *g, PLINT *b)
{
    c_plscol0(*icol0, *r, *g, *b);
}

PLDLLEXPORT void
PLSCOLBG(PLINT *r, PLINT *g, PLINT *b)
{
    c_plscolbg(*r, *g, *b);
}

PLDLLEXPORT void
PLSCOLOR(PLINT *color)
{
    c_plscolor(*color);
}

PLDLLEXPORT void
PLSCOMPRESSION(PLINT *compression)
{
    c_plscompression(*compression);
}

PLDLLEXPORT void
PLSDEV7(char *dev)
{
    c_plsdev(dev);
}

PLDLLEXPORT void
PLSDIDEV(PLFLT *mar, PLFLT *aspect, PLFLT *jx, PLFLT *jy)
{
    c_plsdidev(*mar, *aspect, *jx, *jy);
}

PLDLLEXPORT void
PLSDIMAP(PLINT *dimxmin, PLINT *dimxmax, PLINT *dimymin, PLINT *dimymax,
	 PLFLT *dimxpmm, PLFLT *dimypmm)
{
    c_plsdimap(*dimxmin, *dimxmax, *dimymin, *dimymax,
	       *dimxpmm, *dimypmm);
}

PLDLLEXPORT void
PLSDIORI(PLFLT *rot)
{
    c_plsdiori(*rot);
}

PLDLLEXPORT void
PLSDIPLT(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax)
{
    c_plsdiplt(*xmin, *ymin, *xmax, *ymax);
}

PLDLLEXPORT void
PLSDIPLZ(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax)
{
    c_plsdiplz(*xmin, *ymin, *xmax, *ymax);
}

PLDLLEXPORT void
PLSESC(PLINT *esc)
{
    c_plsesc((char) *esc);
}

PLDLLEXPORT void
PLSETMAPFORMC( void (*mapform)(PLINT *, PLFLT *, PLFLT *) )
{
    plmapform = mapform ;
}

PLDLLEXPORT void
PLSETOPT7(char *opt, char *optarg)
{
    c_plsetopt(opt, optarg);
}

PLDLLEXPORT void
PLSFAM(PLINT *fam, PLINT *num, PLINT *bmax)
{
    c_plsfam(*fam, *num, *bmax);
}

PLDLLEXPORT void
PLSFNAM7(char *fnam)
{
    c_plsfnam(fnam);
}

PLDLLEXPORT void
PLSMAJ(PLFLT *def, PLFLT *scale)
{
    c_plsmaj(*def, *scale);
}

PLDLLEXPORT void
PLSMEM(PLINT *maxx, PLINT *maxy, void *plotmem)
{
    c_plsmem(*maxx, *maxy, plotmem);
}

PLDLLEXPORT void
PLSMIN(PLFLT *def, PLFLT *scale)
{
    c_plsmin(*def, *scale);
}

PLDLLEXPORT void
PLSORI(PLINT *ori)
{
    c_plsori(*ori);
}

PLDLLEXPORT void
PLSPAGE(PLFLT *xpmm, PLFLT *ypmm,
	PLINT *xwid, PLINT *ywid, PLINT *xoff, PLINT *yoff)
{
    c_plspage(*xpmm, *ypmm, *xwid, *ywid, *xoff, *yoff);
}

PLDLLEXPORT void
PLSPAUSE(PLBOOL *pause)
{
    c_plspause(*pause);
}

PLDLLEXPORT void
PLSSTRM(PLINT *strm)
{
    c_plsstrm(*strm);
}

PLDLLEXPORT void
PLSSUB(PLINT *nx, PLINT *ny)
{
    c_plssub(*nx, *ny);
}

PLDLLEXPORT void
PLSSYM(PLFLT *def, PLFLT *scale)
{
    c_plssym(*def, *scale);
}

PLDLLEXPORT void
PLSTAR(PLINT *nx, PLINT *ny)
{
    c_plstar(*nx, *ny);
}

PLDLLEXPORT void
PLSTART7(char *devname, PLINT *nx, PLINT *ny)
{
    c_plstart(devname, *nx, *ny);
}

PLDLLEXPORT void
PLSTRIPA(PLINT *id, PLINT *pen, PLFLT *x, PLFLT *y)
{
    c_plstripa(*id, *pen, *x, *y);
}

PLDLLEXPORT void
PLSTRIPC7(PLINT *id, char *xspec, char *yspec,
	  PLFLT *xmin, PLFLT *xmax, PLFLT *xjump, PLFLT *ymin, PLFLT *ymax,
	  PLFLT *xlpos, PLFLT *ylpos,
	  PLBOOL *y_ascl, PLBOOL *acc,
	  PLINT *colbox, PLINT *collab,
	  PLINT *colline, PLINT *styline,
	  char *legline0, char *legline1, char *legline2, char *legline3,
	  char *labx, char *laby, char *labtop)
{
   char* legline[4];
   legline[0] = legline0;
   legline[1] = legline1;
   legline[2] = legline2;
   legline[3] = legline3;

   c_plstripc(id, xspec, yspec,
	      *xmin, *xmax, *xjump, *ymin, *ymax,
	      *xlpos, *ylpos,
	      *y_ascl, *acc,
	      *colbox, *collab,
	      colline, styline, legline,
	      labx, laby, labtop);
}

PLDLLEXPORT void
PLSTRIPD(PLINT *id)
{
    c_plstripd(*id);
}

PLDLLEXPORT void
PLSTYL(PLINT *n, PLINT *mark, PLINT *space)
{
    c_plstyl(*n, mark, space);
}

PLDLLEXPORT void
PLSVECT(PLFLT *arrowx, PLFLT *arrowy, PLINT *npts, PLBOOL *fill)
{
    c_plsvect(arrowx, arrowy, *npts, *fill);
}

PLDLLEXPORT void
PLSVPA(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    c_plsvpa(*xmin, *xmax, *ymin, *ymax);
}

PLDLLEXPORT void
PLSXAX(PLINT *digmax, PLINT *digits)
{
    c_plsxax(*digmax, *digits);
}

PLDLLEXPORT void
PLSYAX(PLINT *digmax, PLINT *digits)
{
    c_plsyax(*digmax, *digits);
}

PLDLLEXPORT void
PLSYM(PLINT *n, PLFLT *x, PLFLT *y, PLINT *code)
{
    c_plsym(*n, x, y, *code);
}

PLDLLEXPORT void
PLSZAX(PLINT *digmax, PLINT *digits)
{
    c_plszax(*digmax, *digits);
}

PLDLLEXPORT void
PLTEXT(void)
{
    c_pltext();
}

PLDLLEXPORT void
PLVASP(PLFLT *aspect)
{
    c_plvasp(*aspect);
}

PLDLLEXPORT void
PLVPAS(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLFLT *aspect)
{
    c_plvpas(*xmin, *xmax, *ymin, *ymax, *aspect);
}

PLDLLEXPORT void
PLVPOR(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    c_plvpor(*xmin, *xmax, *ymin, *ymax);
}

PLDLLEXPORT void
PLVSTA(void)
{
    c_plvsta();
}

PLDLLEXPORT void
PLW3D(PLFLT *basex, PLFLT *basey, PLFLT *height,
      PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
      PLFLT *zmin, PLFLT *zmax,
      PLFLT *alt, PLFLT *az)
{
    c_plw3d(*basex, *basey, *height,
	    *xmin, *xmax, *ymin, *ymax, *zmin, *zmax, *alt, *az);
}

PLDLLEXPORT void
PLWID(PLINT *width)
{
    c_plwid(*width);
}

PLDLLEXPORT void
PLWIND(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax)
{
    c_plwind(*xmin, *xmax, *ymin, *ymax);
}

PLDLLEXPORT void
PLXORMOD(PLBOOL *mode, PLBOOL *status)
{
    c_plxormod(*mode, status);
}
