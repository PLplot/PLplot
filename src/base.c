/* $Id$
   $Log$
   Revision 1.6  1992/10/22 17:05:31  mjl
   Fixed warnings, errors generated when compling with HP C++.

 * Revision 1.5  1992/10/20  20:15:44  mjl
 * Arguments to plspage() now have no effect if zero.
 *
 * Revision 1.4  1992/10/12  17:07:59  mjl
 * Added PL_NEED_SIZE_T define to those files that need to know the value
 * of (size_t) for non-POSIX systems (in this case the Amiga) that require you
 * to include <stddef.h> to get it.
 *
 * Revision 1.3  1992/09/30  18:25:38  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:36  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:06  furnish
 * Initial checkin of the whole PLPLOT project.
 *
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

#include "plplot.h"
#include <stdio.h>
#include <string.h>
#include "dispatch.h"

static PLINT ipls;
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
PLINT offset;

 /* Initialize device. */
void 
grinit (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_init) (&pls[ipls]);
}

 /* Draw line between two points */
void 
grline (PLINT x1, PLINT y1, PLINT x2, PLINT y2)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_line) (&pls[ipls], x1, y1, x2, y2);
}

 /* Clear screen (or eject page). */
void 
grclr (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_clear) (&pls[ipls]);
}

 /* Set up new page. */
void 
grpage (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_page) (&pls[ipls]);
}

 /* Advance to new page. */
void 
gradv (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_adv) (&pls[ipls]);
}

 /* Tidy up device (flush buffers, close file, etc.) */
void 
grtidy (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_tidy) (&pls[ipls]);
}

 /* Change pen color. */
void 
grcol (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_color) (&pls[ipls]);
}

 /* Switch to text mode (or screen). */
void 
grtext (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_text) (&pls[ipls]);
}

 /* Switch to graphics mode (or screen). */
void 
grgra (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_graph) (&pls[ipls]);
}

 /* Set pen width. */
void 
grwid (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_width) (&pls[ipls]);
}

 /* Escape function, for driver-specific commands. */
void 
gresc (PLINT op, char *ptr)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_esc) (&pls[ipls], op, ptr);
}

/*----------------------------------------------------------------------*\
*  Setup calls
\*----------------------------------------------------------------------*/

/* Initializes the graphics device "dev"  */

void 
grbeg (PLINT dev)
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
c_plfontld( PLINT fnt )
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
c_plgpage( PLFLT *pxp, PLFLT *pyp,
	  PLINT *pxleng, PLINT *pyleng, PLINT *pxoff, PLINT *pyoff )

{
    *pxp = pls[ipls].xdpi;
    *pyp = pls[ipls].ydpi;
    *pxleng = pls[ipls].xlength;
    *pyleng = pls[ipls].ylength;
    *pxoff = pls[ipls].xoffset;
    *pyoff = pls[ipls].yoffset;
}

void
c_plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng, PLINT xoff, PLINT yoff)
{
    if (!xp) pls[ipls].xdpi = xp;
    if (!yp) pls[ipls].ydpi = yp;

    if (!xleng) pls[ipls].xlength = xleng;
    if (!yleng) pls[ipls].ylength = yleng;

    if (!xoff) pls[ipls].xoffset = xoff;
    if (!yoff) pls[ipls].yoffset = yoff;

    pls[ipls].pageset = 1;
}

void 
c_plgstrm( PLINT *pstrm )
{
    *pstrm = ipls;
}

void 
c_plsstrm( PLINT strm )
{
    ipls = strm;
}

/* Set orientation.  Must be done before calling plstar(). */

void 
c_plsori( PLINT ori )
{
    pls[ipls].orient = ori;
    pls[ipls].orientset = 1;
}

/* Set global aspect ratio.  Must be done before calling plstar(). */

void 
c_plsasp(PLFLT asp)
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

void
c_plslpb (PLFLT lpbxmi, PLFLT lpbxma, PLFLT lpbymi, PLFLT lpbyma)
{
    pls[ipls].lpbpxmi = dcpcx(lpbxmi);
    pls[ipls].lpbpxma = dcpcx(lpbxma);
    pls[ipls].lpbpymi = dcpcy(lpbymi);
    pls[ipls].lpbpyma = dcpcy(lpbyma);

    gresc(PL_SET_LPB, (char *) NULL);
}

/* Note these two are only useable from C.. */

void 
plgfile( FILE *file )
{
    file = pls[ipls].OutFile;
}

void 
plsfile( FILE *file )
{
    pls[ipls].OutFile = file;
    pls[ipls].fileset = 1;
}

/*
*  Note here we only pass back a pointer to the file name since we can't be
*  sure the user has allocated enough space for it. 
*/

void 
c_plgfnam( char *fnam )
{
    fnam = pls[ipls].FileName;
}

void 
c_plsfnam( char *fnam )
{
    strncpy(pls[ipls].FileName, fnam, sizeof(pls[ipls].FileName) - 1);
    pls[ipls].FileName[sizeof(pls[ipls].FileName) - 1] = '\0';
    pls[ipls].OutFile = NULL;
    pls[ipls].fileset = 1;
    strcpy(pls[ipls].FamilyName, pls[ipls].FileName);
}

void 
c_plspause( PLINT pause )
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
c_plgfam( PLINT *pfam, PLINT *pnum, PLINT *pbmax )
{
    *pfam = pls[ipls].family;
    *pnum = pls[ipls].member;
    *pbmax = pls[ipls].bytemax;
}

void 
c_plsfam( PLINT fam, PLINT num, PLINT bmax )
{
    if (pls[ipls].level > 0)
        plwarn("You must call plsfam before calling plstar.");

    pls[ipls].family = fam;
    pls[ipls].member = num;
    pls[ipls].bytemax = bmax;
}

void 
c_plfamadv( void )
{
    pls[ipls].famadv = 1;
}

/*----------------------------------------------------------------------*\
*  Interface routines for axis labling parameters.
*  See pldtik.c for more info.
\*----------------------------------------------------------------------*/

void 
c_plgxax( PLINT *pdigmax, PLINT *pdigits )
{
    *pdigmax = pls[ipls].xdigmax;
    *pdigits = pls[ipls].xdigits;
}

void 
c_plsxax( PLINT digmax, PLINT digits )
{
    pls[ipls].xdigmax = digmax;
    pls[ipls].xdigits = digits;
}

void 
c_plgyax( PLINT *pdigmax, PLINT *pdigits )
{
    *pdigmax = pls[ipls].ydigmax;
    *pdigits = pls[ipls].ydigits;
}

void 
c_plsyax( PLINT digmax, PLINT digits )
{
    pls[ipls].ydigmax = digmax;
    pls[ipls].ydigits = digits;
}

void 
c_plgzax( PLINT *pdigmax, PLINT *pdigits )
{
    *pdigmax = pls[ipls].zdigmax;
    *pdigits = pls[ipls].zdigits;
}

void 
c_plszax( PLINT digmax, PLINT digits )
{
    pls[ipls].zdigmax = digmax;
    pls[ipls].zdigits = digits;
}

/*----------------------------------------------------------------------*\
*  These should not be called by the user.
\*----------------------------------------------------------------------*/

void 
glev (PLINT *pn)
{
    *pn = pls[ipls].level;
}

void 
slev (PLINT n)
{
    pls[ipls].level = n;
}

void 
gbase (PLFLT *px, PLFLT *py, PLFLT *pxc, PLFLT *pyc)
{
    *px = pls[ipls].base3x;
    *py = pls[ipls].base3y;
    *pxc = pls[ipls].basecx;
    *pyc = pls[ipls].basecy;
}

void sbase (PLFLT x, PLFLT y, PLFLT xc, PLFLT yc)
{
    pls[ipls].base3x = x;
    pls[ipls].base3y = y;
    pls[ipls].basecx = xc;
    pls[ipls].basecy = yc;
}

void 
gnms (PLINT *pn)
{
    *pn = pls[ipls].nms;
}

void 
snms (PLINT n)
{
    pls[ipls].nms = n;
}

void 
gdev (PLINT *pdev, PLINT *pterm, PLINT *pgra)
{
    *pdev = pls[ipls].device;
    *pterm = pls[ipls].termin;
    *pgra = pls[ipls].graphx;
}

void 
sdev (PLINT dev, PLINT term, PLINT gra)
{
    pls[ipls].device = dev;
    pls[ipls].termin = term;
    pls[ipls].graphx = gra;
}

void 
gdevice (PLINT *pdev)
{
    *pdev = pls[ipls].device;
}

void 
sdevice (PLINT dev)
{
    pls[ipls].device = dev;
}

void 
ggra (PLINT *pgra)
{
    *pgra = pls[ipls].graphx;
}

void 
sgra (PLINT gra)
{
    pls[ipls].graphx = gra;
}

void 
smod (PLINT term)
{
    pls[ipls].termin = term;
}

void 
gcurr (PLINT *pix, PLINT *piy)
{
    *pix = pls[ipls].currx;
    *piy = pls[ipls].curry;
}

void 
scurr (PLINT ix, PLINT iy)
{
    pls[ipls].currx = ix;
    pls[ipls].curry = iy;
}

void 
gdom (PLFLT *pxmin, PLFLT *pxmax, PLFLT *pymin, PLFLT *pymax)
{
    *pxmin = pls[ipls].domxmi;
    *pxmax = pls[ipls].domxma;
    *pymin = pls[ipls].domymi;
    *pymax = pls[ipls].domyma;
}

void sdom (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].domxmi = xmin;
    pls[ipls].domxma = xmax;
    pls[ipls].domymi = ymin;
    pls[ipls].domyma = ymax;
}

void 
grange (PLFLT *pzscl, PLFLT *pzmin, PLFLT *pzmax)
{
    *pzscl = pls[ipls].zzscl;
    *pzmin = pls[ipls].ranmi;
    *pzmax = pls[ipls].ranma;
}

void srange (PLFLT zscl, PLFLT zmin, PLFLT zmax)
{
    pls[ipls].zzscl = zscl;
    pls[ipls].ranmi = zmin;
    pls[ipls].ranma = zmax;
}

void 
gw3wc (PLFLT *pdxx, PLFLT *pdxy, PLFLT *pdyx, PLFLT *pdyy, PLFLT *pdyz)
{
    *pdxx = pls[ipls].cxx;
    *pdxy = pls[ipls].cxy;
    *pdyx = pls[ipls].cyx;
    *pdyy = pls[ipls].cyy;
    *pdyz = pls[ipls].cyz;
}

void sw3wc (PLFLT dxx, PLFLT dxy, PLFLT dyx, PLFLT dyy, PLFLT dyz)
{
    pls[ipls].cxx = dxx;
    pls[ipls].cxy = dxy;
    pls[ipls].cyx = dyx;
    pls[ipls].cyy = dyy;
    pls[ipls].cyz = dyz;
}

void 
gvpp (PLINT *pixmin, PLINT *pixmax, PLINT *piymin, PLINT *piymax)
{
    *pixmin = pls[ipls].vppxmi;
    *pixmax = pls[ipls].vppxma;
    *piymin = pls[ipls].vppymi;
    *piymax = pls[ipls].vppyma;
}

void 
svpp (PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    pls[ipls].vppxmi = ixmin;
    pls[ipls].vppxma = ixmax;
    pls[ipls].vppymi = iymin;
    pls[ipls].vppyma = iymax;
}

void 
gspp (PLINT *pixmin, PLINT *pixmax, PLINT *piymin, PLINT *piymax)
{
    *pixmin = pls[ipls].sppxmi;
    *pixmax = pls[ipls].sppxma;
    *piymin = pls[ipls].sppymi;
    *piymax = pls[ipls].sppyma;
}

void 
sspp (PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    pls[ipls].sppxmi = ixmin;
    pls[ipls].sppxma = ixmax;
    pls[ipls].sppymi = iymin;
    pls[ipls].sppyma = iymax;
}

void 
gclp (PLINT *pixmin, PLINT *pixmax, PLINT *piymin, PLINT *piymax)
{
    *pixmin = pls[ipls].clpxmi;
    *pixmax = pls[ipls].clpxma;
    *piymin = pls[ipls].clpymi;
    *piymax = pls[ipls].clpyma;
}

void 
sclp (PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    pls[ipls].clpxmi = ixmin;
    pls[ipls].clpxma = ixmax;
    pls[ipls].clpymi = iymin;
    pls[ipls].clpyma = iymax;
}

void 
gphy (PLINT *pixmin, PLINT *pixmax, PLINT *piymin, PLINT *piymax)
{
    *pixmin = pls[ipls].phyxmi;
    *pixmax = pls[ipls].phyxma;
    *piymin = pls[ipls].phyymi;
    *piymax = pls[ipls].phyyma;
}

void 
sphy (PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    pls[ipls].phyxmi = ixmin;
    pls[ipls].phyxma = ixmax;
    pls[ipls].phyymi = iymin;
    pls[ipls].phyyma = iymax;
}

void 
gsub (PLINT *pnx, PLINT *pny, PLINT *pcs)
{
    *pnx = pls[ipls].nsubx;
    *pny = pls[ipls].nsuby;
    *pcs = pls[ipls].cursub;
}

void 
ssub (PLINT nx, PLINT ny, PLINT cs)
{
    pls[ipls].nsubx = nx;
    pls[ipls].nsuby = ny;
    pls[ipls].cursub = cs;
}

void 
gumpix (PLINT *pix, PLINT *piy)
{
    *pix = pls[ipls].umx;
    *piy = pls[ipls].umy;
}

void 
sumpix (PLINT ix, PLINT iy)
{
    pls[ipls].umx = ix;
    pls[ipls].umy = iy;
}

void 
gatt (PLINT *pifnt, PLINT *picol)
{
    *pifnt = font;
    *picol = pls[ipls].color;
}

void 
satt (PLINT ifnt, PLINT icol)
{
    font = ifnt;
    pls[ipls].color = icol;
}

void 
gcol (PLINT *picol)
{
    *picol = pls[ipls].color;
}

void 
scol (PLINT icol)
{
    pls[ipls].color = icol;
}

void 
gwid (PLINT *ppwid)
{
    *ppwid = pls[ipls].width;
}

void 
swid (PLINT pwid)
{
    pls[ipls].width = pwid;
}

void 
gspd (PLFLT *pxmin, PLFLT *pxmax, PLFLT *pymin, PLFLT *pymax)
{
    *pxmin = pls[ipls].spdxmi;
    *pxmax = pls[ipls].spdxma;
    *pymin = pls[ipls].spdymi;
    *pymax = pls[ipls].spdyma;
}

void sspd (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].spdxmi = xmin;
    pls[ipls].spdxma = xmax;
    pls[ipls].spdymi = ymin;
    pls[ipls].spdyma = ymax;
}

void 
gvpd (PLFLT *pxmin, PLFLT *pxmax, PLFLT *pymin, PLFLT *pymax)
{
    *pxmin = pls[ipls].vpdxmi;
    *pxmax = pls[ipls].vpdxma;
    *pymin = pls[ipls].vpdymi;
    *pymax = pls[ipls].vpdyma;
}

void svpd (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].vpdxmi = xmin;
    pls[ipls].vpdxma = xmax;
    pls[ipls].vpdymi = ymin;
    pls[ipls].vpdyma = ymax;
}

void 
gvpw (PLFLT *pxmin, PLFLT *pxmax, PLFLT *pymin, PLFLT *pymax)
{
    *pxmin = pls[ipls].vpwxmi;
    *pxmax = pls[ipls].vpwxma;
    *pymin = pls[ipls].vpwymi;
    *pymax = pls[ipls].vpwyma;
}

void svpw (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].vpwxmi = xmin;
    pls[ipls].vpwxma = xmax;
    pls[ipls].vpwymi = ymin;
    pls[ipls].vpwyma = ymax;
}

void 
gpixmm (PLFLT *px, PLFLT *py)
{
    *px = pls[ipls].xpmm;
    *py = pls[ipls].ypmm;
}

void spixmm (PLFLT x, PLFLT y)
{
    pls[ipls].xpmm = x;
    pls[ipls].ypmm = y;
    pls[ipls].umx = 1000.0 / pls[ipls].xpmm;
    pls[ipls].umy = 1000.0 / pls[ipls].ypmm;
}

/* All the drivers call this to set physical pixels/mm. */

void setpxl (PLFLT xpmm0, PLFLT ypmm0)
{
    pls[ipls].xpmm = xpmm0;
    pls[ipls].ypmm = ypmm0;
    pls[ipls].umx = 1000.0 / pls[ipls].xpmm;
    pls[ipls].umy = 1000.0 / pls[ipls].ypmm;
}

/* Sets up physical limits of plotting device and the mapping between
   normalized device coordinates and physical coordinates. */

void 
setphy (PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax)
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
gwp (PLFLT *pxscl, PLFLT *pxoff, PLFLT *pyscl, PLFLT *pyoff)
{
    *pxscl = pls[ipls].wpxscl;
    *pxoff = pls[ipls].wpxoff;
    *pyscl = pls[ipls].wpyscl;
    *pyoff = pls[ipls].wpyoff;
}

void swp (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].wpxscl = xscl;
    pls[ipls].wpxoff = xoff;
    pls[ipls].wpyscl = yscl;
    pls[ipls].wpyoff = yoff;
}

void 
gwm (PLFLT *pxscl, PLFLT *pxoff, PLFLT *pyscl, PLFLT *pyoff)
{
    *pxscl = pls[ipls].wmxscl;
    *pxoff = pls[ipls].wmxoff;
    *pyscl = pls[ipls].wmyscl;
    *pyoff = pls[ipls].wmyoff;
}

void swm (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].wmxscl = xscl;
    pls[ipls].wmxoff = xoff;
    pls[ipls].wmyscl = yscl;
    pls[ipls].wmyoff = yoff;
}

void 
gdp (PLFLT *pxscl, PLFLT *pxoff, PLFLT *pyscl, PLFLT *pyoff)
{
    *pxscl = pls[ipls].dpxscl;
    *pxoff = pls[ipls].dpxoff;
    *pyscl = pls[ipls].dpyscl;
    *pyoff = pls[ipls].dpyoff;
}

void sdp (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].dpxscl = xscl;
    pls[ipls].dpxoff = xoff;
    pls[ipls].dpyscl = yscl;
    pls[ipls].dpyoff = yoff;
}

void 
gmp (PLFLT *pxscl, PLFLT *pxoff, PLFLT *pyscl, PLFLT *pyoff)
{
    *pxscl = pls[ipls].mpxscl;
    *pxoff = pls[ipls].mpxoff;
    *pyscl = pls[ipls].mpyscl;
    *pyoff = pls[ipls].mpyoff;
}

void smp (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].mpxscl = xscl;
    pls[ipls].mpxoff = xoff;
    pls[ipls].mpyscl = yscl;
    pls[ipls].mpyoff = yoff;
}

void 
gchr (PLFLT *pdef, PLFLT *pht)
{
    *pdef = pls[ipls].chrdef;
    *pht = pls[ipls].chrht;
}

void schr (PLFLT def, PLFLT ht)
{
    pls[ipls].chrdef = def;
    pls[ipls].chrht = ht;
}

void 
gsym (PLFLT *pdef, PLFLT *pht)
{
    *pdef = pls[ipls].symdef;
    *pht = pls[ipls].symht;
}

void ssym (PLFLT def, PLFLT ht)
{
    pls[ipls].symdef = def;
    pls[ipls].symht = ht;
}

void 
gmaj (PLFLT *pdef, PLFLT *pht)
{
    *pdef = pls[ipls].majdef;
    *pht = pls[ipls].majht;
}

void smaj (PLFLT def, PLFLT ht)
{
    pls[ipls].majdef = def;
    pls[ipls].majht = ht;
}

void 
gmin (PLFLT *pdef, PLFLT *pht)
{
    *pdef = pls[ipls].mindef;
    *pht = pls[ipls].minht;
}

void smin (PLFLT def, PLFLT ht)
{
    pls[ipls].mindef = def;
    pls[ipls].minht = ht;
}

void 
gpat (PLINT *pinc[], PLINT *pdel[], PLINT *pnlin)
{
    *pinc = pls[ipls].inclin;
    *pdel = pls[ipls].delta;
    *pnlin = pls[ipls].nps;
}

void 
spat (PLINT inc[], PLINT del[], PLINT nlin)
{
    PLINT i;

    pls[ipls].nps = nlin;
    for (i = 0; i < nlin; i++) {
	pls[ipls].inclin[i] = inc[i];
	pls[ipls].delta[i] = del[i];
    }
}

void 
gmark (PLINT *pmar[], PLINT *pspa[], PLINT **pnms)
{
    *pmar = pls[ipls].mark;
    *pspa = pls[ipls].space;
    *pnms = &(pls[ipls].nms);
}

void 
smark (PLINT mar[], PLINT spa[], PLINT nms)
{
    PLINT i;

    pls[ipls].nms = nms;
    for (i = 0; i < nms; i++) {
	pls[ipls].mark[i] = mar[i];
	pls[ipls].space[i] = spa[i];
    }
}

void 
gcure (PLINT **pcur, PLINT **ppen, PLINT **ptim, PLINT **pala)
{
    *pcur = &(pls[ipls].curel);
    *ppen = &(pls[ipls].pendn);
    *ptim = &(pls[ipls].timecnt);
    *pala = &(pls[ipls].alarm);
}

void 
scure (PLINT cur, PLINT pen, PLINT tim, PLINT ala)
{
    pls[ipls].curel = cur;
    pls[ipls].pendn = pen;
    pls[ipls].timecnt = tim;
    pls[ipls].alarm = ala;
}
