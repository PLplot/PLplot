/* $Id$
   $Log$
   Revision 1.10  1992/11/07 08:04:27  mjl
   Fixed a problem encountered when a user tried to change the default
   character/symbol scale heights.

 * Revision 1.9  1992/10/29  16:03:13  mjl
 * Attached an ID string here too as an experiment.
 *
 * Revision 1.8  1992/10/27  22:14:11  mjl
 * Support for plflush() function.
 *
 * Revision 1.7  1992/10/24  05:17:31  mjl
 * Fixed a stupidity I made earlier in plspage().
 *
 * Revision 1.6  1992/10/22  17:05:31  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
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
    plbuf_init(&pls[ipls]);
}

 /* Draw line between two points */
void 
grline (PLINT x1, PLINT y1, PLINT x2, PLINT y2)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_line) (&pls[ipls], x1, y1, x2, y2);
    plbuf_line(&pls[ipls], x1, y1, x2, y2);
}

 /* Clear screen (or eject page). */
void 
grclr (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_clear) (&pls[ipls]);
    plbuf_clear(&pls[ipls]);
}

 /* Set up new page. */
void 
grpage (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_page) (&pls[ipls]);
    plbuf_page(&pls[ipls]);
}

 /* Advance to new page. */
void 
gradv (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_adv) (&pls[ipls]);
    plbuf_adv(&pls[ipls]);
}

 /* Tidy up device (flush buffers, close file, etc.) */
void 
grtidy (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_tidy) (&pls[ipls]);
    plbuf_tidy(&pls[ipls]);
}

 /* Change pen color. */
void 
grcol (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_color) (&pls[ipls]);
    plbuf_color(&pls[ipls]);
}

 /* Switch to text mode (or screen). */
void 
grtext (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_text) (&pls[ipls]);
    plbuf_text(&pls[ipls]);
}

 /* Switch to graphics mode (or screen). */
void 
grgra (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_graph) (&pls[ipls]);
    plbuf_graph(&pls[ipls]);
}

 /* Set pen width. */
void 
grwid (void)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_width) (&pls[ipls]);
    plbuf_width(&pls[ipls]);
}

 /* Escape function, for driver-specific commands. */
void 
gresc (PLINT op, char *ptr)
{
    offset = pls[ipls].device - 1;
    (*plDispatchTable[offset].pl_esc) (&pls[ipls], op, ptr);
    plbuf_esc(&pls[ipls], op, ptr);
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
    sscale(scale);

    plschr(4.0, 1.0);
    plssym(4.0, 1.0);
    plsmaj(3.0, 1.0);
    plsmin(1.5, 1.0);

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
c_plgpage( PLFLT *p_xp, PLFLT *p_yp,
	  PLINT *p_xleng, PLINT *p_yleng, PLINT *p_xoff, PLINT *p_yoff )

{
    *p_xp = pls[ipls].xdpi;
    *p_yp = pls[ipls].ydpi;
    *p_xleng = pls[ipls].xlength;
    *p_yleng = pls[ipls].ylength;
    *p_xoff = pls[ipls].xoffset;
    *p_yoff = pls[ipls].yoffset;
}

void
c_plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng, PLINT xoff, PLINT yoff)
{
    if (xp) pls[ipls].xdpi = xp;
    if (yp) pls[ipls].ydpi = yp;

    if (xleng) pls[ipls].xlength = xleng;
    if (yleng) pls[ipls].ylength = yleng;

    if (xoff) pls[ipls].xoffset = xoff;
    if (yoff) pls[ipls].yoffset = yoff;

    pls[ipls].pageset = 1;
}

void 
c_plgstrm( PLINT *p_strm )
{
    *p_strm = ipls;
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

/* Flush current output file.  Use sparingly, if at all. */

void
plflush( void )
{
    fflush(pls[ipls].OutFile);
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
c_plgfam( PLINT *p_fam, PLINT *p_num, PLINT *p_bmax )
{
    *p_fam = pls[ipls].family;
    *p_num = pls[ipls].member;
    *p_bmax = pls[ipls].bytemax;
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
c_plgxax( PLINT *p_digmax, PLINT *p_digits )
{
    *p_digmax = pls[ipls].xdigmax;
    *p_digits = pls[ipls].xdigits;
}

void 
c_plsxax( PLINT digmax, PLINT digits )
{
    pls[ipls].xdigmax = digmax;
    pls[ipls].xdigits = digits;
}

void 
c_plgyax( PLINT *p_digmax, PLINT *p_digits )
{
    *p_digmax = pls[ipls].ydigmax;
    *p_digits = pls[ipls].ydigits;
}

void 
c_plsyax( PLINT digmax, PLINT digits )
{
    pls[ipls].ydigmax = digmax;
    pls[ipls].ydigits = digits;
}

void 
c_plgzax( PLINT *p_digmax, PLINT *p_digits )
{
    *p_digmax = pls[ipls].zdigmax;
    *p_digits = pls[ipls].zdigits;
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
glev (PLINT *p_n)
{
    *p_n = pls[ipls].level;
}

void 
slev (PLINT n)
{
    pls[ipls].level = n;
}

void 
gbase (PLFLT *p_x, PLFLT *p_y, PLFLT *p_xc, PLFLT *p_yc)
{
    *p_x = pls[ipls].base3x;
    *p_y = pls[ipls].base3y;
    *p_xc = pls[ipls].basecx;
    *p_yc = pls[ipls].basecy;
}

void sbase (PLFLT x, PLFLT y, PLFLT xc, PLFLT yc)
{
    pls[ipls].base3x = x;
    pls[ipls].base3y = y;
    pls[ipls].basecx = xc;
    pls[ipls].basecy = yc;
}

void 
gnms (PLINT *p_n)
{
    *p_n = pls[ipls].nms;
}

void 
snms (PLINT n)
{
    pls[ipls].nms = n;
}

void 
gdev (PLINT *p_dev, PLINT *p_term, PLINT *p_gra)
{
    *p_dev = pls[ipls].device;
    *p_term = pls[ipls].termin;
    *p_gra = pls[ipls].graphx;
}

void 
sdev (PLINT dev, PLINT term, PLINT gra)
{
    pls[ipls].device = dev;
    pls[ipls].termin = term;
    pls[ipls].graphx = gra;
}

void 
gdevice (PLINT *p_dev)
{
    *p_dev = pls[ipls].device;
}

void 
sdevice (PLINT dev)
{
    pls[ipls].device = dev;
}

void 
ggra (PLINT *p_gra)
{
    *p_gra = pls[ipls].graphx;
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
gcurr (PLINT *p_ix, PLINT *p_iy)
{
    *p_ix = pls[ipls].currx;
    *p_iy = pls[ipls].curry;
}

void 
scurr (PLINT ix, PLINT iy)
{
    pls[ipls].currx = ix;
    pls[ipls].curry = iy;
}

void 
gdom (PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = pls[ipls].domxmi;
    *p_xmax = pls[ipls].domxma;
    *p_ymin = pls[ipls].domymi;
    *p_ymax = pls[ipls].domyma;
}

void sdom (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].domxmi = xmin;
    pls[ipls].domxma = xmax;
    pls[ipls].domymi = ymin;
    pls[ipls].domyma = ymax;
}

void 
grange (PLFLT *p_zscl, PLFLT *p_zmin, PLFLT *p_zmax)
{
    *p_zscl = pls[ipls].zzscl;
    *p_zmin = pls[ipls].ranmi;
    *p_zmax = pls[ipls].ranma;
}

void srange (PLFLT zscl, PLFLT zmin, PLFLT zmax)
{
    pls[ipls].zzscl = zscl;
    pls[ipls].ranmi = zmin;
    pls[ipls].ranma = zmax;
}

void 
gw3wc (PLFLT *p_dxx, PLFLT *p_dxy, PLFLT *p_dyx, PLFLT *p_dyy, PLFLT *p_dyz)
{
    *p_dxx = pls[ipls].cxx;
    *p_dxy = pls[ipls].cxy;
    *p_dyx = pls[ipls].cyx;
    *p_dyy = pls[ipls].cyy;
    *p_dyz = pls[ipls].cyz;
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
gvpp (PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = pls[ipls].vppxmi;
    *p_ixmax = pls[ipls].vppxma;
    *p_iymin = pls[ipls].vppymi;
    *p_iymax = pls[ipls].vppyma;
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
gspp (PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = pls[ipls].sppxmi;
    *p_ixmax = pls[ipls].sppxma;
    *p_iymin = pls[ipls].sppymi;
    *p_iymax = pls[ipls].sppyma;
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
gclp (PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = pls[ipls].clpxmi;
    *p_ixmax = pls[ipls].clpxma;
    *p_iymin = pls[ipls].clpymi;
    *p_iymax = pls[ipls].clpyma;
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
gphy (PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = pls[ipls].phyxmi;
    *p_ixmax = pls[ipls].phyxma;
    *p_iymin = pls[ipls].phyymi;
    *p_iymax = pls[ipls].phyyma;
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
gsub (PLINT *p_nx, PLINT *p_ny, PLINT *p_cs)
{
    *p_nx = pls[ipls].nsubx;
    *p_ny = pls[ipls].nsuby;
    *p_cs = pls[ipls].cursub;
}

void 
ssub (PLINT nx, PLINT ny, PLINT cs)
{
    pls[ipls].nsubx = nx;
    pls[ipls].nsuby = ny;
    pls[ipls].cursub = cs;
}

void 
gumpix (PLINT *p_ix, PLINT *p_iy)
{
    *p_ix = pls[ipls].umx;
    *p_iy = pls[ipls].umy;
}

void 
sumpix (PLINT ix, PLINT iy)
{
    pls[ipls].umx = ix;
    pls[ipls].umy = iy;
}

void 
gatt (PLINT *p_ifnt, PLINT *p_icol)
{
    *p_ifnt = font;
    *p_icol = pls[ipls].color;
}

void 
satt (PLINT ifnt, PLINT icol)
{
    font = ifnt;
    pls[ipls].color = icol;
}

void 
gcol (PLINT *p_icol)
{
    *p_icol = pls[ipls].color;
}

void 
scol (PLINT icol)
{
    pls[ipls].color = icol;
}

void 
gwid (PLINT *p_pwid)
{
    *p_pwid = pls[ipls].width;
}

void 
swid (PLINT pwid)
{
    pls[ipls].width = pwid;
}

void 
gspd (PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = pls[ipls].spdxmi;
    *p_xmax = pls[ipls].spdxma;
    *p_ymin = pls[ipls].spdymi;
    *p_ymax = pls[ipls].spdyma;
}

void sspd (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].spdxmi = xmin;
    pls[ipls].spdxma = xmax;
    pls[ipls].spdymi = ymin;
    pls[ipls].spdyma = ymax;
}

void 
gvpd (PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = pls[ipls].vpdxmi;
    *p_xmax = pls[ipls].vpdxma;
    *p_ymin = pls[ipls].vpdymi;
    *p_ymax = pls[ipls].vpdyma;
}

void svpd (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].vpdxmi = xmin;
    pls[ipls].vpdxma = xmax;
    pls[ipls].vpdymi = ymin;
    pls[ipls].vpdyma = ymax;
}

void 
gvpw (PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = pls[ipls].vpwxmi;
    *p_xmax = pls[ipls].vpwxma;
    *p_ymin = pls[ipls].vpwymi;
    *p_ymax = pls[ipls].vpwyma;
}

void svpw (PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].vpwxmi = xmin;
    pls[ipls].vpwxma = xmax;
    pls[ipls].vpwymi = ymin;
    pls[ipls].vpwyma = ymax;
}

void 
gpixmm (PLFLT *p_x, PLFLT *p_y)
{
    *p_x = pls[ipls].xpmm;
    *p_y = pls[ipls].ypmm;
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
gwp (PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = pls[ipls].wpxscl;
    *p_xoff = pls[ipls].wpxoff;
    *p_yscl = pls[ipls].wpyscl;
    *p_yoff = pls[ipls].wpyoff;
}

void swp (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].wpxscl = xscl;
    pls[ipls].wpxoff = xoff;
    pls[ipls].wpyscl = yscl;
    pls[ipls].wpyoff = yoff;
}

void 
gwm (PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = pls[ipls].wmxscl;
    *p_xoff = pls[ipls].wmxoff;
    *p_yscl = pls[ipls].wmyscl;
    *p_yoff = pls[ipls].wmyoff;
}

void swm (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].wmxscl = xscl;
    pls[ipls].wmxoff = xoff;
    pls[ipls].wmyscl = yscl;
    pls[ipls].wmyoff = yoff;
}

void 
gdp (PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = pls[ipls].dpxscl;
    *p_xoff = pls[ipls].dpxoff;
    *p_yscl = pls[ipls].dpyscl;
    *p_yoff = pls[ipls].dpyoff;
}

void sdp (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].dpxscl = xscl;
    pls[ipls].dpxoff = xoff;
    pls[ipls].dpyscl = yscl;
    pls[ipls].dpyoff = yoff;
}

void 
gmp (PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = pls[ipls].mpxscl;
    *p_xoff = pls[ipls].mpxoff;
    *p_yscl = pls[ipls].mpyscl;
    *p_yoff = pls[ipls].mpyoff;
}

void smp (PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].mpxscl = xscl;
    pls[ipls].mpxoff = xoff;
    pls[ipls].mpyscl = yscl;
    pls[ipls].mpyoff = yoff;
}

void 
gchr (PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = pls[ipls].chrdef;
    *p_ht = pls[ipls].chrht;
}

void schr (PLFLT def, PLFLT ht)
{
    pls[ipls].chrdef = def;
    pls[ipls].chrht = ht;
}

void 
gscale (PLFLT *p_scale)
{
    *p_scale = pls[ipls].scale;
}

void sscale (PLFLT scale)
{
    pls[ipls].scale = scale;
}

void 
gsym (PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = pls[ipls].symdef;
    *p_ht = pls[ipls].symht;
}

void ssym (PLFLT def, PLFLT ht)
{
    pls[ipls].symdef = def;
    pls[ipls].symht = ht;
}

void 
gmaj (PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = pls[ipls].majdef;
    *p_ht = pls[ipls].majht;
}

void smaj (PLFLT def, PLFLT ht)
{
    pls[ipls].majdef = def;
    pls[ipls].majht = ht;
}

void 
gmin (PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = pls[ipls].mindef;
    *p_ht = pls[ipls].minht;
}

void smin (PLFLT def, PLFLT ht)
{
    pls[ipls].mindef = def;
    pls[ipls].minht = ht;
}

void 
gpat (PLINT *p_inc[], PLINT *p_del[], PLINT *p_nlin)
{
    *p_inc = pls[ipls].inclin;
    *p_del = pls[ipls].delta;
    *p_nlin = pls[ipls].nps;
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
gmark (PLINT *p_mar[], PLINT *p_spa[], PLINT **p_nms)
{
    *p_mar = pls[ipls].mark;
    *p_spa = pls[ipls].space;
    *p_nms = &(pls[ipls].nms);
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
gcure (PLINT **p_cur, PLINT **p_pen, PLINT **p_tim, PLINT **p_ala)
{
    *p_cur = &(pls[ipls].curel);
    *p_pen = &(pls[ipls].pendn);
    *p_tim = &(pls[ipls].timecnt);
    *p_ala = &(pls[ipls].alarm);
}

void 
scure (PLINT cur, PLINT pen, PLINT tim, PLINT ala)
{
    pls[ipls].curel = cur;
    pls[ipls].pendn = pen;
    pls[ipls].timecnt = tim;
    pls[ipls].alarm = ala;
}
