/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:34:06  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	base.c

	Contains PLPLOT internal variables, external access routines,
	device driver calls, and miscellaneous initialization calls.
*/

/*----------------------------------------------------------------------*\
* PLPLOT internal variables are listed below.
*
* ipls		Stream number
* font..	Current default font and control parameters
\*----------------------------------------------------------------------*/

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

static int ipls;
static PLINT font, initfont, fontset;

/*
* PLStream data structure.
* It contains a copy of every variable that is stream dependent.
* See the definition in plplot.h for more info.
*/

static PLStream pls[PL_NSTREAMS];

/*----------------------------------------------------------------------*\
*  Driver calls
\*----------------------------------------------------------------------*/

extern DISPATCH_TABLE plDispatchTable[];
int offset;

 /* Initialize device. */
void
grinit()
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_init) (&pls[ipls]);
}

 /* Draw line between two points */
void
grline(x1, y1, x2, y2)
PLINT x1, y1, x2, y2;
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_line) (&pls[ipls], x1, y1, x2, y2);
}

 /* Clear screen (or eject page). */
void
grclr()
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_clear) (&pls[ipls]);
}

 /* Set up new page. */
void
grpage()
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_page) (&pls[ipls]);
}

 /* Advance to new page. */
void
gradv()
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_adv) (&pls[ipls]);
}

 /* Tidy up device (flush buffers, close file, etc.) */
void
grtidy()
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_tidy) (&pls[ipls]);
}

 /* Change pen color. */
void
grcol()
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_color) (&pls[ipls]);
}

 /* Switch to text mode (or screen). */
void
grtext()
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_text) (&pls[ipls]);
}

 /* Switch to graphics mode (or screen). */
void
grgra()
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_graph) (&pls[ipls]);
}

 /* Set pen width. */
void
grwid()
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_width) (&pls[ipls]);
}

 /* Escape function, for driver-specific commands. */
void
gresc(op, ptr)
PLINT op;
char *ptr;
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_esc) (&pls[ipls], op, ptr);
}

/*----------------------------------------------------------------------*\
*  Setup calls
\*----------------------------------------------------------------------*/

/* Initializes the graphics device "dev"  */

void 
grbeg(dev)
PLINT dev;
{
    PLINT mk = 0, sp = 0;
    PLINT inc = 0, del = 2000;
    PLINT phyxmi, phyxma, phyymi, phyyma;
    PLFLT xpmm, ypmm, scale;

/* Set device number, graphics mode and font */

    pls[ipls].ipls = ipls;
    pls[ipls].device = dev;
    pls[ipls].color = 1;
    font = 1;

/* Start by initializing device */

    if (fontset) {
	plfntld(initfont);
	fontset = 0;
    }
    else
	plfntld(0);

    grinit();
    grpage();

/* Set default sizes */
/* These are now normalized to the page length for more uniform results */
/* A virtual page length of 200 mm is assumed */

    gphy(&phyxmi, &phyxma, &phyymi, &phyyma);
    gpixmm(&xpmm, &ypmm);
    scale = 0.5 * ((phyxma-phyxmi)/xpmm + (phyyma-phyymi)/ypmm)/ 200.0;

    plschr((PLFLT) (4.0 * scale), 1.0);
    plssym((PLFLT) (4.0 * scale), 1.0);
    plsmaj((PLFLT) (3.0 * scale), 1.0);
    plsmin((PLFLT) (1.5 * scale), 1.0);

/* Switch to graphics mode and set color */

    grgra();
    grcol();
    grwid();

    plprec(0, 0);
    plstyl(0, &mk, &sp);
    plpat(1, &inc, &del);

/* Set clip limits. */

    pls[ipls].clpxmi = pls[ipls].phyxmi;
    pls[ipls].clpxma = pls[ipls].phyxma;
    pls[ipls].clpymi = pls[ipls].phyymi;
    pls[ipls].clpyma = pls[ipls].phyyma;
}

void 
c_plfontld(fnt)
PLINT fnt;
{
    if (fnt != 0)
	fnt = 1;

    if (pls[ipls].level > 0)
	plfntld(fnt);
    else {
	initfont = fnt;
	fontset = 1;
    }
}

/*----------------------------------------------------------------------*\
*  External access routines.
\*----------------------------------------------------------------------*/

void 
c_plgpage(pxp, pyp, pxleng, pyleng, pxoff, pyoff)
PLINT *pxleng, *pyleng, *pxoff, *pyoff;
PLFLT *pxp, *pyp;
{
    *pxp = pls[ipls].xdpi;
    *pyp = pls[ipls].ydpi;
    *pxleng = pls[ipls].xlength;
    *pyleng = pls[ipls].ylength;
    *pxoff = pls[ipls].xoffset;
    *pyoff = pls[ipls].yoffset;
}

#ifdef PLSTDC
void
c_plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng, PLINT xoff, PLINT yoff)
#else
void 
c_plspage(xp, yp, xleng, yleng, xoff, yoff)
PLINT xleng, yleng, xoff, yoff;
PLFLT xp, yp;
#endif
{
    pls[ipls].xdpi = xp;
    pls[ipls].ydpi = yp;
    pls[ipls].xlength = xleng;
    pls[ipls].ylength = yleng;
    pls[ipls].xoffset = xoff;
    pls[ipls].yoffset = yoff;
    pls[ipls].pageset = 1;
}

void 
c_plgstrm(pstrm)
PLINT *pstrm;
{
    *pstrm = ipls;
}

void 
c_plsstrm(strm)
PLINT strm;
{
    ipls = strm;
}

/* Set orientation.  Must be done before calling plstar(). */

void 
c_plsori(ori)
PLINT ori;
{
    pls[ipls].orient = ori;
    pls[ipls].orientset = 1;
}

/* Set global aspect ratio.  Must be done before calling plstar(). */

#ifdef PLSTDC
void 
c_plsasp(PLFLT asp)
#else
void 
c_plsasp(asp)
PLFLT asp;
#endif
{
    pls[ipls].aspect = asp;
    pls[ipls].aspectset = 1;

    if (pls[ipls].aspect > 0.0)
	pls[ipls].pscale = 1;
    else
	pls[ipls].pscale = 0;
}

/* Set local plot bounds.  This is used to 'mark' a window for treatment
   as a separate object by the renderer.  The inputs should be in normalized
   device coords (they are translated into physical coords before storing. */

#ifdef PLSTDC
void
c_plslpb (PLFLT lpbxmi, PLFLT lpbxma, PLFLT lpbymi, PLFLT lpbyma)
#else
void
c_plslpb (lpbxmi, lpbxma, lpbymi, lpbyma)
PLFLT lpbxmi, lpbxma, lpbymi, lpbyma;
#endif
{
    pls[ipls].lpbpxmi = dcpcx(lpbxmi);
    pls[ipls].lpbpxma = dcpcx(lpbxma);
    pls[ipls].lpbpymi = dcpcy(lpbymi);
    pls[ipls].lpbpyma = dcpcy(lpbyma);

    gresc(PL_SET_LPB, (char *) NULL);
}

/* Note these two are only useable from C.. */

void 
plgfile(file)
FILE *file;
{
    file = pls[ipls].OutFile;
}

void 
plsfile(file)
FILE *file;
{
    pls[ipls].OutFile = file;
    pls[ipls].fileset = 1;
}

/*
*  Note here we only pass back a pointer to the file name since we can't be
*  sure the user has allocated enough space for it. 
*/

void 
c_plgfnam(fnam)
char *fnam;
{
    fnam = pls[ipls].FileName;
}

void 
c_plsfnam(fnam)
char *fnam;
{
    strncpy(pls[ipls].FileName, fnam, sizeof(pls[ipls].FileName) - 1);
    pls[ipls].FileName[sizeof(pls[ipls].FileName) - 1] = '\0';
    pls[ipls].OutFile = NULL;
    pls[ipls].fileset = 1;
    strcpy(pls[ipls].FamilyName, pls[ipls].FileName);
}

void 
c_plspause(pause)
PLINT pause;
{
    pls[ipls].nopause = !pause;
}

/*----------------------------------------------------------------------*\
*  These set/get information for family files, and may be called prior
*  to 'plstar' to set up the necessary parameters.  Arguments:
*
*	fam	familying flag (boolean)
*	num	member number
*	bmax	maximum member size
\*----------------------------------------------------------------------*/

void 
c_plgfam(pfam, pnum, pbmax)
PLINT *pfam, *pnum, *pbmax;
{
    *pfam = pls[ipls].family;
    *pnum = pls[ipls].member;
    *pbmax = pls[ipls].bytemax;
}

void 
c_plsfam(fam, num, bmax)
PLINT fam, num, bmax;
{
    if (pls[ipls].level > 0)
        plwarn("You must call plsfam before calling plstar.");

    pls[ipls].family = fam;
    pls[ipls].member = num;
    pls[ipls].bytemax = bmax;
}

/*----------------------------------------------------------------------*\
*  Interface routines for axis labling parameters.
*  See pldtik.c for more info.
\*----------------------------------------------------------------------*/

void 
c_plgxax(pdigmax, pdigits)
PLINT *pdigmax, *pdigits;
{
    *pdigmax = pls[ipls].xdigmax;
    *pdigits = pls[ipls].xdigits;
}

void 
c_plsxax(digmax, digits)
PLINT digmax, digits;
{
    pls[ipls].xdigmax = digmax;
    pls[ipls].xdigits = digits;
}

void 
c_plgyax(pdigmax, pdigits)
PLINT *pdigmax, *pdigits;
{
    *pdigmax = pls[ipls].ydigmax;
    *pdigits = pls[ipls].ydigits;
}

void 
c_plsyax(digmax, digits)
PLINT digmax, digits;
{
    pls[ipls].ydigmax = digmax;
    pls[ipls].ydigits = digits;
}

void 
c_plgzax(pdigmax, pdigits)
PLINT *pdigmax, *pdigits;
{
    *pdigmax = pls[ipls].zdigmax;
    *pdigits = pls[ipls].zdigits;
}

void 
c_plszax(digmax, digits)
PLINT digmax, digits;
{
    pls[ipls].zdigmax = digmax;
    pls[ipls].zdigits = digits;
}

/*----------------------------------------------------------------------*\
*  These should not be called by the user.
\*----------------------------------------------------------------------*/

void 
glev(pn)
PLINT *pn;
{
    *pn = pls[ipls].level;
}

void 
slev(n)
PLINT n;
{
    pls[ipls].level = n;
}

void 
gbase(px, py, pxc, pyc)
PLFLT *px, *py, *pxc, *pyc;
{
    *px = pls[ipls].base3x;
    *py = pls[ipls].base3y;
    *pxc = pls[ipls].basecx;
    *pyc = pls[ipls].basecy;
}

#ifdef PLSTDC
void sbase (PLFLT x, PLFLT y, PLFLT xc, PLFLT yc)
#else
void 
sbase(x, y, xc, yc)
PLFLT x, y, xc, yc;
#endif
{
    pls[ipls].base3x = x;
    pls[ipls].base3y = y;
    pls[ipls].basecx = xc;
    pls[ipls].basecy = yc;
}

void 
gnms(pn)
PLINT *pn;
{
    *pn = pls[ipls].nms;
}

void 
snms(n)
PLINT n;
{
    pls[ipls].nms = n;
}

void 
gdev(pdev, pterm, pgra)
PLINT *pdev, *pterm, *pgra;
{
    *pdev = pls[ipls].device;
    *pterm = pls[ipls].termin;
    *pgra = pls[ipls].graphx;
}

void 
sdev(dev, term, gra)
PLINT dev, term, gra;
{
    pls[ipls].device = dev;
    pls[ipls].termin = term;
    pls[ipls].graphx = gra;
}

void 
gdevice(pdev)
PLINT *pdev;
{
    *pdev = pls[ipls].device;
}

void 
sdevice(dev)
PLINT dev;
{
    pls[ipls].device = dev;
}

void 
ggra(pgra)
PLINT *pgra;
{
    *pgra = pls[ipls].graphx;
}

void 
sgra(gra)
PLINT gra;
{
    pls[ipls].graphx = gra;
}

void 
smod(term)
PLINT term;
{
    pls[ipls].termin = term;
}

void 
gcurr(pix, piy)
PLINT *pix, *piy;
{
    *pix = pls[ipls].currx;
    *piy = pls[ipls].curry;
}

void 
scurr(ix, iy)
PLINT ix, iy;
{
    pls[ipls].currx = ix;
    pls[ipls].curry = iy;
}

void 
gdom(pxmin, pxmax, pymin, pymax)
PLFLT *pxmin, *pxmax, *pymin, *pymax;
{
    *pxmin = pls[ipls].domxmi;
    *pxmax = pls[ipls].domxma;
    *pymin = pls[ipls].domymi;
    *pymax = pls[ipls].domyma;
}

#ifdef PLSTDC
void sdom (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
#else
void 
sdom(xmin, xmax, ymin, ymax)
PLFLT xmin, xmax, ymin, ymax;
#endif
{
    pls[ipls].domxmi = xmin;
    pls[ipls].domxma = xmax;
    pls[ipls].domymi = ymin;
    pls[ipls].domyma = ymax;
}

void 
grange(pzscl, pzmin, pzmax)
PLFLT *pzscl, *pzmin, *pzmax;
{
    *pzscl = pls[ipls].zzscl;
    *pzmin = pls[ipls].ranmi;
    *pzmax = pls[ipls].ranma;
}

#ifdef PLSTDC
void srange (PLFLT zscl, PLFLT zmin, PLFLT zmax)
#else
void 
srange(zscl, zmin, zmax)
PLFLT zscl, zmin, zmax;
#endif
{
    pls[ipls].zzscl = zscl;
    pls[ipls].ranmi = zmin;
    pls[ipls].ranma = zmax;
}

void 
gw3wc(pdxx, pdxy, pdyx, pdyy, pdyz)
PLFLT *pdxx, *pdxy, *pdyx, *pdyy, *pdyz;
{
    *pdxx = pls[ipls].cxx;
    *pdxy = pls[ipls].cxy;
    *pdyx = pls[ipls].cyx;
    *pdyy = pls[ipls].cyy;
    *pdyz = pls[ipls].cyz;
}

#ifdef PLSTDC
void sw3wc (PLFLT dxx, PLFLT dxy, PLFLT dyx, PLFLT dyy, PLFLT dyz)
#else
void 
sw3wc(dxx, dxy, dyx, dyy, dyz)
PLFLT dxx, dxy, dyx, dyy, dyz;
#endif
{
    pls[ipls].cxx = dxx;
    pls[ipls].cxy = dxy;
    pls[ipls].cyx = dyx;
    pls[ipls].cyy = dyy;
    pls[ipls].cyz = dyz;
}

void 
gvpp(pixmin, pixmax, piymin, piymax)
PLINT *pixmin, *pixmax, *piymin, *piymax;
{
    *pixmin = pls[ipls].vppxmi;
    *pixmax = pls[ipls].vppxma;
    *piymin = pls[ipls].vppymi;
    *piymax = pls[ipls].vppyma;
}

void 
svpp(ixmin, ixmax, iymin, iymax)
PLINT ixmin, ixmax, iymin, iymax;
{
    pls[ipls].vppxmi = ixmin;
    pls[ipls].vppxma = ixmax;
    pls[ipls].vppymi = iymin;
    pls[ipls].vppyma = iymax;
}

void 
gspp(pixmin, pixmax, piymin, piymax)
PLINT *pixmin, *pixmax, *piymin, *piymax;
{
    *pixmin = pls[ipls].sppxmi;
    *pixmax = pls[ipls].sppxma;
    *piymin = pls[ipls].sppymi;
    *piymax = pls[ipls].sppyma;
}

void 
sspp(ixmin, ixmax, iymin, iymax)
PLINT ixmin, ixmax, iymin, iymax;
{
    pls[ipls].sppxmi = ixmin;
    pls[ipls].sppxma = ixmax;
    pls[ipls].sppymi = iymin;
    pls[ipls].sppyma = iymax;
}

void 
gclp(pixmin, pixmax, piymin, piymax)
PLINT *pixmin, *pixmax, *piymin, *piymax;
{
    *pixmin = pls[ipls].clpxmi;
    *pixmax = pls[ipls].clpxma;
    *piymin = pls[ipls].clpymi;
    *piymax = pls[ipls].clpyma;
}

void 
sclp(ixmin, ixmax, iymin, iymax)
PLINT ixmin, ixmax, iymin, iymax;
{
    pls[ipls].clpxmi = ixmin;
    pls[ipls].clpxma = ixmax;
    pls[ipls].clpymi = iymin;
    pls[ipls].clpyma = iymax;
}

void 
gphy(pixmin, pixmax, piymin, piymax)
PLINT *pixmin, *pixmax, *piymin, *piymax;
{
    *pixmin = pls[ipls].phyxmi;
    *pixmax = pls[ipls].phyxma;
    *piymin = pls[ipls].phyymi;
    *piymax = pls[ipls].phyyma;
}

void 
sphy(ixmin, ixmax, iymin, iymax)
PLINT ixmin, ixmax, iymin, iymax;
{
    pls[ipls].phyxmi = ixmin;
    pls[ipls].phyxma = ixmax;
    pls[ipls].phyymi = iymin;
    pls[ipls].phyyma = iymax;
}

void 
gsub(pnx, pny, pcs)
PLINT *pnx, *pny, *pcs;
{
    *pnx = pls[ipls].nsubx;
    *pny = pls[ipls].nsuby;
    *pcs = pls[ipls].cursub;
}

void 
ssub(nx, ny, cs)
PLINT nx, ny, cs;
{
    pls[ipls].nsubx = nx;
    pls[ipls].nsuby = ny;
    pls[ipls].cursub = cs;
}

void 
gumpix(pix, piy)
PLINT *pix, *piy;
{
    *pix = pls[ipls].umx;
    *piy = pls[ipls].umy;
}

void 
sumpix(ix, iy)
PLINT ix, iy;
{
    pls[ipls].umx = ix;
    pls[ipls].umy = iy;
}

void 
gatt(pifnt, picol)
PLINT *pifnt, *picol;
{
    *pifnt = font;
    *picol = pls[ipls].color;
}

void 
satt(ifnt, icol)
PLINT ifnt, icol;
{
    font = ifnt;
    pls[ipls].color = icol;
}

void 
gcol(picol)
PLINT *picol;
{
    *picol = pls[ipls].color;
}

void 
scol(icol)
PLINT icol;
{
    pls[ipls].color = icol;
}

void 
gwid(ppwid)
PLINT *ppwid;
{
    *ppwid = pls[ipls].width;
}

void 
swid(pwid)
PLINT pwid;
{
    pls[ipls].width = pwid;
}

void 
gspd(pxmin, pxmax, pymin, pymax)
PLFLT *pxmin, *pxmax, *pymin, *pymax;
{
    *pxmin = pls[ipls].spdxmi;
    *pxmax = pls[ipls].spdxma;
    *pymin = pls[ipls].spdymi;
    *pymax = pls[ipls].spdyma;
}

#ifdef PLSTDC
void sspd (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
#else
void 
sspd(xmin, xmax, ymin, ymax)
PLFLT xmin, xmax, ymin, ymax;
#endif
{
    pls[ipls].spdxmi = xmin;
    pls[ipls].spdxma = xmax;
    pls[ipls].spdymi = ymin;
    pls[ipls].spdyma = ymax;
}

void 
gvpd(pxmin, pxmax, pymin, pymax)
PLFLT *pxmin, *pxmax, *pymin, *pymax;
{
    *pxmin = pls[ipls].vpdxmi;
    *pxmax = pls[ipls].vpdxma;
    *pymin = pls[ipls].vpdymi;
    *pymax = pls[ipls].vpdyma;
}

#ifdef PLSTDC
void svpd (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
#else
void 
svpd(xmin, xmax, ymin, ymax)
PLFLT xmin, xmax, ymin, ymax;
#endif
{
    pls[ipls].vpdxmi = xmin;
    pls[ipls].vpdxma = xmax;
    pls[ipls].vpdymi = ymin;
    pls[ipls].vpdyma = ymax;
}

void 
gvpw(pxmin, pxmax, pymin, pymax)
PLFLT *pxmin, *pxmax, *pymin, *pymax;
{
    *pxmin = pls[ipls].vpwxmi;
    *pxmax = pls[ipls].vpwxma;
    *pymin = pls[ipls].vpwymi;
    *pymax = pls[ipls].vpwyma;
}

#ifdef PLSTDC
void svpw (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
#else
void 
svpw(xmin, xmax, ymin, ymax)
PLFLT xmin, xmax, ymin, ymax;
#endif
{
    pls[ipls].vpwxmi = xmin;
    pls[ipls].vpwxma = xmax;
    pls[ipls].vpwymi = ymin;
    pls[ipls].vpwyma = ymax;
}

void 
gpixmm(px, py)
PLFLT *px, *py;
{
    *px = pls[ipls].xpmm;
    *py = pls[ipls].ypmm;
}

#ifdef PLSTDC
void spixmm (PLFLT x, PLFLT y)
#else
void 
spixmm(x, y)
PLFLT x, y;
#endif
{
    pls[ipls].xpmm = x;
    pls[ipls].ypmm = y;
    pls[ipls].umx = 1000.0 / pls[ipls].xpmm;
    pls[ipls].umy = 1000.0 / pls[ipls].ypmm;
}

/* All the drivers call this to set physical pixels/mm. */

#ifdef PLSTDC
void setpxl (PLFLT xpmm0, PLFLT ypmm0)
#else
void 
setpxl(xpmm0, ypmm0)
PLFLT xpmm0, ypmm0;
#endif
{
    pls[ipls].xpmm = xpmm0;
    pls[ipls].ypmm = ypmm0;
    pls[ipls].umx = 1000.0 / pls[ipls].xpmm;
    pls[ipls].umy = 1000.0 / pls[ipls].ypmm;
}

/* Sets up physical limits of plotting device and the mapping between
   normalized device coordinates and physical coordinates. */

void 
setphy(xmin, xmax, ymin, ymax)
PLINT xmin, xmax, ymin, ymax;
{
    PLFLT xpmm, ypmm, mpxscl, mpyscl;

    sphy(xmin, xmax, ymin, ymax);
    sdp((PLFLT) (xmax - xmin), (PLFLT) (xmin), (PLFLT) (ymax - ymin),
	(PLFLT) (ymin));

    gpixmm(&xpmm, &ypmm);
    mpxscl = xpmm;
    if (xmax <= xmin)
	mpxscl = -xpmm;
    mpyscl = ypmm;
    if (ymax <= ymin)
	mpyscl = -ypmm;
    smp(mpxscl, (PLFLT) (xmin), mpyscl, (PLFLT) (ymin));
}

void 
gwp(pxscl, pxoff, pyscl, pyoff)
PLFLT *pxscl, *pxoff, *pyscl, *pyoff;
{
    *pxscl = pls[ipls].wpxscl;
    *pxoff = pls[ipls].wpxoff;
    *pyscl = pls[ipls].wpyscl;
    *pyoff = pls[ipls].wpyoff;
}

#ifdef PLSTDC
void swp (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
#else
void 
swp(xscl, xoff, yscl, yoff)
PLFLT xscl, xoff, yscl, yoff;
#endif
{
    pls[ipls].wpxscl = xscl;
    pls[ipls].wpxoff = xoff;
    pls[ipls].wpyscl = yscl;
    pls[ipls].wpyoff = yoff;
}

void 
gwm(pxscl, pxoff, pyscl, pyoff)
PLFLT *pxscl, *pxoff, *pyscl, *pyoff;
{
    *pxscl = pls[ipls].wmxscl;
    *pxoff = pls[ipls].wmxoff;
    *pyscl = pls[ipls].wmyscl;
    *pyoff = pls[ipls].wmyoff;
}

#ifdef PLSTDC
void swm (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
#else
void 
swm(xscl, xoff, yscl, yoff)
PLFLT xscl, xoff, yscl, yoff;
#endif
{
    pls[ipls].wmxscl = xscl;
    pls[ipls].wmxoff = xoff;
    pls[ipls].wmyscl = yscl;
    pls[ipls].wmyoff = yoff;
}

void 
gdp(pxscl, pxoff, pyscl, pyoff)
PLFLT *pxscl, *pxoff, *pyscl, *pyoff;
{
    *pxscl = pls[ipls].dpxscl;
    *pxoff = pls[ipls].dpxoff;
    *pyscl = pls[ipls].dpyscl;
    *pyoff = pls[ipls].dpyoff;
}

#ifdef PLSTDC
void sdp (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
#else
void 
sdp(xscl, xoff, yscl, yoff)
PLFLT xscl, xoff, yscl, yoff;
#endif
{
    pls[ipls].dpxscl = xscl;
    pls[ipls].dpxoff = xoff;
    pls[ipls].dpyscl = yscl;
    pls[ipls].dpyoff = yoff;
}

void 
gmp(pxscl, pxoff, pyscl, pyoff)
PLFLT *pxscl, *pxoff, *pyscl, *pyoff;
{
    *pxscl = pls[ipls].mpxscl;
    *pxoff = pls[ipls].mpxoff;
    *pyscl = pls[ipls].mpyscl;
    *pyoff = pls[ipls].mpyoff;
}

#ifdef PLSTDC
void smp (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
#else
void 
smp(xscl, xoff, yscl, yoff)
PLFLT xscl, xoff, yscl, yoff;
#endif
{
    pls[ipls].mpxscl = xscl;
    pls[ipls].mpxoff = xoff;
    pls[ipls].mpyscl = yscl;
    pls[ipls].mpyoff = yoff;
}

void 
gchr(pdef, pht)
PLFLT *pdef, *pht;
{
    *pdef = pls[ipls].chrdef;
    *pht = pls[ipls].chrht;
}

#ifdef PLSTDC
void schr (PLFLT def, PLFLT ht)
#else
void 
schr(def, ht)
PLFLT def, ht;
#endif
{
    pls[ipls].chrdef = def;
    pls[ipls].chrht = ht;
}

void 
gsym(pdef, pht)
PLFLT *pdef, *pht;
{
    *pdef = pls[ipls].symdef;
    *pht = pls[ipls].symht;
}

#ifdef PLSTDC
void ssym (PLFLT def, PLFLT ht)
#else
void 
ssym(def, ht)
PLFLT def, ht;
#endif
{
    pls[ipls].symdef = def;
    pls[ipls].symht = ht;
}

void 
gmaj(pdef, pht)
PLFLT *pdef, *pht;
{
    *pdef = pls[ipls].majdef;
    *pht = pls[ipls].majht;
}

#ifdef PLSTDC
void smaj (PLFLT def, PLFLT ht)
#else
void 
smaj(def, ht)
PLFLT def, ht;
#endif
{
    pls[ipls].majdef = def;
    pls[ipls].majht = ht;
}

void 
gmin(pdef, pht)
PLFLT *pdef, *pht;
{
    *pdef = pls[ipls].mindef;
    *pht = pls[ipls].minht;
}

#ifdef PLSTDC
void smin (PLFLT def, PLFLT ht)
#else
void 
smin(def, ht)
PLFLT def, ht;
#endif
{
    pls[ipls].mindef = def;
    pls[ipls].minht = ht;
}

void 
gpat(pinc, pdel, pnlin)
PLINT *pinc[], *pdel[], *pnlin;
{
    *pinc = pls[ipls].inclin;
    *pdel = pls[ipls].delta;
    *pnlin = pls[ipls].nps;
}

void 
spat(inc, del, nlin)
PLINT inc[], del[], nlin;
{
    PLINT i;

    pls[ipls].nps = nlin;
    for (i = 0; i < nlin; i++) {
	pls[ipls].inclin[i] = inc[i];
	pls[ipls].delta[i] = del[i];
    }
}

void 
gmark(pmar, pspa, pnms)
PLINT *pmar[], *pspa[], **pnms;
{
    *pmar = pls[ipls].mark;
    *pspa = pls[ipls].space;
    *pnms = &(pls[ipls].nms);
}

void 
smark(mar, spa, nms)
PLINT mar[], spa[], nms;
{
    PLINT i;

    pls[ipls].nms = nms;
    for (i = 0; i < nms; i++) {
	pls[ipls].mark[i] = mar[i];
	pls[ipls].space[i] = spa[i];
    }
}

void 
gcure(pcur, ppen, ptim, pala)
PLINT **pcur, **ppen, **ptim, **pala;
{
    *pcur = &(pls[ipls].curel);
    *ppen = &(pls[ipls].pendn);
    *ptim = &(pls[ipls].timecnt);
    *pala = &(pls[ipls].alarm);
}

void 
scure(cur, pen, tim, ala)
PLINT cur, pen, tim, ala;
{
    pls[ipls].curel = cur;
    pls[ipls].pendn = pen;
    pls[ipls].timecnt = tim;
    pls[ipls].alarm = ala;
}
