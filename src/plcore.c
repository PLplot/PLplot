/* $Id$
   $Log$
   Revision 1.4  1993/03/02 19:01:01  mjl
   Added plgver() for retrieving plplot library version.

 * Revision 1.3  1993/02/25  18:31:36  mjl
 * Changed the order of driver calls on a grclr().  The plot buffer clear
 * must come first.
 *
 * Revision 1.2  1993/02/23  05:11:44  mjl
 * Eliminated gradv driver function.  Completely rewrote initialization
 * routines.  Now plstar and plstart are merely front-ends to plinit, which
 * does all the initialization.  Added plsdev for setting the device name, and
 * plssub for setting the subpages.  Added plgpls for getting the current pls
 * pointer, for offloading stream-dependent code into other files.  Added
 * plsesc/plgesc for setting/getting the escape character for text strings (can
 * be any of [!#$%&*@^~], brackets not included).  Put in some checks for
 * routines that set family file parameters.  Eliminated some unused routines.
 *
 * Revision 1.1  1993/01/23  05:49:27  mjl
 * Holds "core" routines -- includes routines from what was base.c, plstar.c,
 * some others.  The stream data and dispatch table are now local to this
 * file only.  Also added support for new color model, polyline support.
 *

 * Previous history (as base.c):
 *
 * Revision 1.10  1992/11/07  08:04:27  mjl
 * Fixed a problem encountered when a user tried to change the default
 * character/symbol scale heights.
 *
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
*/

/*	plcore.c

	Central dispatch facility for plplot.
	Also contains the plplot main data structures, external access
	routines, and initialization calls.

	This stuff used to be in "dispatch.h", "dispatch.c", and "base.c".
*/

#include "plcore.h"

static void	GetDev(void);

/*----------------------------------------------------------------------*\
* Driver calls
*
* Each call is followed by the appropriate plot buffer call.  If the target
* device has enabled the plot buffer, this will result in a record of the
* current page being stored in the plot buffer (which can be "replayed" to
* handle screen refresh, resize, dumps, etc).

* Each call that actually plots something (versus just changing state)
* is preceded by a test to see if a newpage command has been deferred.
* This is the only way to prevent extraneous newpage commands while
* retaining a user-friendly page advance mechanism.
*
\*----------------------------------------------------------------------*/

/* Initialize device. */

void
grinit(void)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_init) (&pls[ipls]);
    plbuf_init(&pls[ipls]);
}

/* Draw line between two points */

void
grline(PLSHORT x1, PLSHORT y1, PLSHORT x2, PLSHORT y2)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_line) (&pls[ipls], x1, y1, x2, y2);
    plbuf_line(&pls[ipls], x1, y1, x2, y2);
}

/* Draw polyline */

void
grpolyline(PLSHORT *x, PLSHORT *y, PLINT npts)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_polyline) (&pls[ipls], x, y, npts);
    plbuf_polyline(&pls[ipls], x, y, npts);
}

/* Clear screen (or eject page). */
/* Here the plot buffer call must be made first */

void
grclr(void)
{
    plbuf_clear(&pls[ipls]);
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_clear) (&pls[ipls]);
}

/* Set up new page. */

void
grpage(void)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_page) (&pls[ipls]);
    plbuf_page(&pls[ipls]);
}

/* Tidy up device (flush buffers, close file, etc.) */

void
grtidy(void)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_tidy) (&pls[ipls]);
    plbuf_tidy(&pls[ipls]);
}

/* Change pen color. */

void
grcol(void)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_color) (&pls[ipls]);
    plbuf_color(&pls[ipls]);
}

/* Switch to text mode (or screen). */

void
grtext(void)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_text) (&pls[ipls]);
    plbuf_text(&pls[ipls]);
}

/* Switch to graphics mode (or screen). */

void
grgra(void)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_graph) (&pls[ipls]);
    plbuf_graph(&pls[ipls]);
}

/* Set pen width. */

void
grwid(void)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_width) (&pls[ipls]);
    plbuf_width(&pls[ipls]);
}

/* Escape function, for driver-specific commands. */

void
gresc(PLINT op, char *ptr)
{
    offset = pls[ipls].device - 1;
    (*dispatch_table[offset].pl_esc) (&pls[ipls], op, ptr);
    plbuf_esc(&pls[ipls], op, ptr);
}

/*----------------------------------------------------------------------*\
* Startup routines.  
\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
* void plstar(nx, ny)
*
* Here we are passing the windows/page in x and y.
\*----------------------------------------------------------------------*/

void
c_plstar(PLINT nx, PLINT ny)
{
    if (pls[ipls].level != 0)
	plend1();

    plssub(nx, ny);

    c_plinit();
}

/*----------------------------------------------------------------------*\
* void plstart(devname, nx, ny)
*
* Here we are passing the device name, and windows/page in x and y.
\*----------------------------------------------------------------------*/

void
c_plstart(char *devname, PLINT nx, PLINT ny)
{
    if (pls[ipls].level != 0)
	plend1();

    plssub(nx, ny);
    plsdev(devname);

    c_plinit();
}

/*----------------------------------------------------------------------*\
* void plinit()
*
* Checks certain startup parameters for validity, then proceeds with
* initialization.  Accepts no arguments.
\*----------------------------------------------------------------------*/

void
c_plinit()
{
    PLFLT gscale, hscale;
    PLFLT size_chr, size_sym, size_maj, size_min;
    PLINT mk = 0, sp = 0, inc = 0, del = 2000;

    if (pls[ipls].level != 0)
	plend1();

/* Set device number */

    GetDev();

/* Subpage checks */

    if (pls[ipls].nsubx <= 0)
	pls[ipls].nsubx = 1;
    if (pls[ipls].nsuby <= 0)
	pls[ipls].nsuby = 1;
    pls[ipls].cursub = 0;

/* Stream number */

    pls[ipls].ipls = ipls;

/* Initialize color maps */

    plCmaps_init(&pls[ipls]);

/* We're rolling now.. */

    pls[ipls].level = 1;

/* Load fonts */

    font = 1;
    if (fontset) {
	plfntld(initfont);
	fontset = 0;
    }
    else
	plfntld(0);

/* Initialize device */

    grinit();
    grpage();

/*
* Set default sizes 
* Global scaling:  
*	Normalize to the page length for more uniform results.  
* 	A virtual page length of 200 mm is assumed.
* Subpage scaling: 
*	Reduce sizes with plot area (non-proportional, so that character
*	size doesn't get too small).
*/
    gscale = 0.5 * 
	((pls[ipls].phyxma - pls[ipls].phyxmi) / pls[ipls].xpmm +
	 (pls[ipls].phyyma - pls[ipls].phyymi) / pls[ipls].ypmm) / 200.0;

    hscale = gscale / sqrt((double) pls[ipls].nsuby);

    size_chr = 4.0;
    size_sym = 4.0;		/* All these in virtual plot units */
    size_maj = 3.0;
    size_min = 1.5;

    schr((PLFLT) (size_chr * hscale), (PLFLT) (size_chr * hscale));
    ssym((PLFLT) (size_sym * hscale), (PLFLT) (size_sym * hscale));
    smaj((PLFLT) (size_maj * hscale), (PLFLT) (size_maj * hscale));
    smin((PLFLT) (size_min * hscale), (PLFLT) (size_min * hscale));

/* Switch to graphics mode and set color */

    grgra();
    plcol(1);

    plstyl(0, &mk, &sp);
    plpat(1, &inc, &del);

/* Set clip limits. */

    pls[ipls].clpxmi = pls[ipls].phyxmi;
    pls[ipls].clpxma = pls[ipls].phyxma;
    pls[ipls].clpymi = pls[ipls].phyymi;
    pls[ipls].clpyma = pls[ipls].phyyma;
}

/*----------------------------------------------------------------------*\
* void GetDev()
*
* If the the user has not already specified the output device, or the
* one specified is either: (a) not available, (b) "?", or (c) NULL, the
* user is prompted for it.  A file name of "-" indicates output to stdout.
* 
* Prompting quits after 10 unsuccessful tries in case the user has
* run the program in the background with insufficient input.
\*----------------------------------------------------------------------*/

static void
GetDev()
{
    PLINT dev = 0, i, count, length;
    char response[80];

/* Device name already specified.  See if it is valid. */

    if (*pls[ipls].DevName != '\0' && *pls[ipls].DevName != '?') {
	for (i = 0; i < npldrivers; i++) {
	    if (!strcmp(pls[ipls].DevName, dispatch_table[i].pl_DevName))
		break;
	}
	if (i < npldrivers) {
	    pls[ipls].device = i + 1;
	    return;
	}
	else {
	    printf("Requested device %s not available\n", pls[ipls].DevName);
	}
    }

    dev = 0;
    count = 0;

    if (npldrivers == 1)
	dev = 1;

/* User hasn't specified it correctly yet, so we prompt */

    while (dev < 1 || dev > npldrivers) {
	printf("\nPlotting Options:\n");
	for (i = 0; i < npldrivers; i++) {
	    printf(" <%2ld> (%s)\t %s\n", i + 1, dispatch_table[i].pl_DevName,
		   dispatch_table[i].pl_MenuStr);
	}
	if (ipls == 0)
	    printf("\nEnter device number or keyword: ");
	else
	    printf("\nEnter device number or keyword (stream %d): ", ipls);

	fgets(response, sizeof(response), stdin);

/* First check to see if device keyword was entered. */
/* Final "\n" in response messes things up, so ignore it.  */

	length = strlen(response);
	if (*(response - 1 + length) == '\n')
	    length--;

	for (i = 0; i < npldrivers; i++) {
	    if (!strncmp(response, dispatch_table[i].pl_DevName,
			 (unsigned int) length))
		break;
	}
	if (i < npldrivers) {
	    dev = i + 1;
	}
	else {
	    if ((dev = atoi(response)) < 1) {
		printf("Invalid device.");
		dev = 0;
	    }
	}
	if (count++ > 10)
	    plexit("Too many tries.");
    }
    pls[ipls].device = dev;
}

/*----------------------------------------------------------------------*\
* void plwarn()
*
* A handy way to issue warnings, if need be.
\*----------------------------------------------------------------------*/

void
plwarn(char *errormsg)
{
    int was_gfx = 0;

    if (pls[ipls].level > 0) {
	if (pls[ipls].graphx == 1) {
	    was_gfx = 1;
	    grtext();
	}
    }

    fprintf(stderr, "\n*** PLPLOT WARNING ***\n");
    if (*errormsg != '\0')
	fprintf(stderr, "%s\n", errormsg);

    if (was_gfx == 1) {
	grgra();
    }
}

/*----------------------------------------------------------------------*\
*  Load font.
\*----------------------------------------------------------------------*/

void
c_plfontld(PLINT fnt)
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
c_plgpage(PLFLT *p_xp, PLFLT *p_yp,
	  PLINT *p_xleng, PLINT *p_yleng, PLINT *p_xoff, PLINT *p_yoff)
{
    *p_xp = pls[ipls].xdpi;
    *p_yp = pls[ipls].ydpi;
    *p_xleng = pls[ipls].xlength;
    *p_yleng = pls[ipls].ylength;
    *p_xoff = pls[ipls].xoffset;
    *p_yoff = pls[ipls].yoffset;
}

void
c_plssub(PLINT nx, PLINT ny)
{
    if (pls[ipls].level > 0) {
	plwarn("plssub: Must be called before plinit.");
	return;
    }
    if (nx > 0)
	pls[ipls].nsubx = nx;
    if (ny > 0)
	pls[ipls].nsuby = ny;
}

void
c_plsdev(char *devname)
{
    if (pls[ipls].level > 0) {
	plwarn("plsdev: Must be called before plinit.");
	return;
    }
    strncpy(pls[ipls].DevName, devname, sizeof(pls[ipls].DevName) - 1);
    pls[ipls].DevName[sizeof(pls[ipls].DevName) - 1] = '\0';
}

void
c_plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng, PLINT xoff, PLINT yoff)
{
    if (xp)
	pls[ipls].xdpi = xp;
    if (yp)
	pls[ipls].ydpi = yp;

    if (xleng)
	pls[ipls].xlength = xleng;
    if (yleng)
	pls[ipls].ylength = yleng;

    if (xoff)
	pls[ipls].xoffset = xoff;
    if (yoff)
	pls[ipls].yoffset = yoff;

    pls[ipls].pageset = 1;
}

void
plgpls(PLStream *pls)
{
    pls = &pls[ipls];
}

void
c_plgstrm(PLINT *p_strm)
{
    *p_strm = ipls;
}

void
c_plsstrm(PLINT strm)
{
    if (strm < 0 || strm >= PL_NSTREAMS)
	printf("plsstrm: Illegal stream number (%d) -- must be in [0, %d]\n",
	       strm, PL_NSTREAMS);
    else
	ipls = strm;
}

void
plsKeyEH(void (*KeyEH)(PLKey *, void *, int *), void *KeyEH_data)
{
    pls[ipls].KeyEH = KeyEH;
    pls[ipls].KeyEH_data = KeyEH_data;
}

/* Set orientation.  Must be done before calling plinit. */

void
c_plsori(PLINT ori)
{
    pls[ipls].orient = ori;
    pls[ipls].orientset = 1;
}

/* Set pen width.  Can be done any time, but before calling plinit is best
   since otherwise it may be volatile (i.e. reset on next page advance). */

void
c_plwid(PLINT width)
{
    pls[ipls].width = width;

    if (pls[ipls].level > 0)
	grwid();
    else
	pls[ipls].widthset = 1;
}

/* Set global aspect ratio.  Must be done before calling plinit. */

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
c_plslpb(PLFLT lpbxmi, PLFLT lpbxma, PLFLT lpbymi, PLFLT lpbyma)
{
    pls[ipls].lpbpxmi = dcpcx(lpbxmi);
    pls[ipls].lpbpxma = dcpcx(lpbxma);
    pls[ipls].lpbpymi = dcpcy(lpbymi);
    pls[ipls].lpbpyma = dcpcy(lpbyma);

    gresc(PL_SET_LPB, (char *) NULL);
}

/* Note these two are only useable from C.. */

void
plgfile(FILE *file)
{
    file = pls[ipls].OutFile;
}

void
plsfile(FILE *file)
{
    pls[ipls].OutFile = file;
    pls[ipls].fileset = 1;
}

/* Flush current output file.  Use sparingly, if at all. */

void
plflush(void)
{
    fflush(pls[ipls].OutFile);
}

/*
*  Note here we only pass back a pointer to the file name since we can't be
*  sure the user has allocated enough space for it.
*/

void
c_plgfnam(char *fnam)
{
    fnam = pls[ipls].FileName;
}

void
c_plsfnam(char *fnam)
{
    strncpy(pls[ipls].FileName, fnam, sizeof(pls[ipls].FileName) - 1);
    pls[ipls].FileName[sizeof(pls[ipls].FileName) - 1] = '\0';
    pls[ipls].OutFile = NULL;
    pls[ipls].fileset = 1;
    strcpy(pls[ipls].FamilyName, pls[ipls].FileName);
}

void
c_plspause(PLINT pause)
{
    pls[ipls].nopause = !pause;
}

/* Set/get the number of points written after the decimal point in labels. */

void
c_plprec(PLINT setp, PLINT prec)
{
    pls[ipls].setpre = setp;
    pls[ipls].precis = prec;
}

void
gprec(PLINT *p_setp, PLINT *p_prec)
{
    *p_setp = pls[ipls].setpre;
    *p_prec = pls[ipls].precis;
}

/*
* Set/get the escape character for text strings.
* From C you can pass as a character, from Fortran it needs to be the decimal
* ASCII value.  Only selected characters are allowed to prevent the user from
* shooting himself in the foot (a '\' isn't allowed since it conflicts with
* C's use of backslash as a character escape).
*/

void
c_plsesc(char esc)
{
    switch (esc) {
    case '!':			/* ASCII 33 */
    case '#':			/* ASCII 35 */
    case '$':			/* ASCII 36 */
    case '%':			/* ASCII 37 */
    case '&':			/* ASCII 38 */
    case '*':			/* ASCII 42 */
    case '@':			/* ASCII 64 */
    case '^':			/* ASCII 94 */
    case '~':			/* ASCII 126 */
	pls[ipls].esc = esc;
	break;

    default:
	plwarn("Invalid escape character, ignoring.");
    }
}

void
plgesc(char *p_esc)
{
    if (pls[ipls].esc == '\0')
	pls[ipls].esc = '#';

    *p_esc = pls[ipls].esc;
}

/* Note: you MUST have allocated space for this (80 characters is safe) */

void
c_plgver(char *p_ver)
{
    strcpy(p_ver, PLPLOT_VERSION);
}

/*----------------------------------------------------------------------*\
*  Routines that deal with colors & color maps.
\*----------------------------------------------------------------------*/

/* Set color, map 0 */

void
c_plcol(PLINT icol0)
{
    if (pls[ipls].level < 1)
	plexit("plcol: Please call plinit first.");

    if (icol0 < 0 || icol0 > 15) {
	plwarn("plcol: Invalid color.");
	return;
    }

    if (pls[ipls].cmap0setcol[icol0] == 0) {
	plwarn("plcol: Requested color not allocated.");
	return;
    }

    pls[ipls].icol0 = icol0;
    pls[ipls].curcolor.r = pls[ipls].cmap0[icol0].r;
    pls[ipls].curcolor.g = pls[ipls].cmap0[icol0].g;
    pls[ipls].curcolor.b = pls[ipls].cmap0[icol0].b;

    grcol();
}

/* Set line color by red, green, blue from  0. to 1. */

void
c_plrgb(PLFLT r, PLFLT g, PLFLT b)
{
    if (pls[ipls].level < 1)
	plexit("plrgb: Please call plinit first.");

    if ((r < 0. || r > 1.) || (g < 0. || g > 1.) || (b < 0. || b > 1.)) {
	plwarn("plrgb: Invalid RGB color value");
	return;
    }

    pls[ipls].icol0 = PL_RGB_COLOR;
    pls[ipls].curcolor.r = MIN(255, (int) (256. * r));
    pls[ipls].curcolor.g = MIN(255, (int) (256. * g));
    pls[ipls].curcolor.b = MIN(255, (int) (256. * b));

    grcol();
}

/* Set line color by 8 bit RGB values. */

void
c_plrgb1(PLINT r, PLINT g, PLINT b)
{
    if (pls[ipls].level < 1)
	plexit("plrgb1: Please call plinit first.");

    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
	plwarn("plrgb1: Invalid color");
	return;
    }

    pls[ipls].icol0 = PL_RGB_COLOR;
    pls[ipls].curcolor.r = r;
    pls[ipls].curcolor.g = g;
    pls[ipls].curcolor.b = b;

    grcol();
}

/* Set number of colors in color map 0 */

void
c_plscm0n(PLINT ncol0)
{
    if (pls[ipls].level > 0) {
	plwarn("plscm0: Must be called before plinit.");
	return;
    }

    if (ncol0 < 2 || ncol0 > 16)
	plexit("plscm0: Number of colors out of range");

    pls[ipls].ncol0 = ncol0;
}

/* Set color map 0 colors by 8 bit RGB values */
/* WARNING -- This sets ncol0 as well. */

void
c_plscm0(PLINT *r, PLINT *g, PLINT *b, PLINT ncol0)
{
    int i;

    if (pls[ipls].level > 0) {
	plwarn("plscm0: Must be called before plinit.");
	return;
    }

    if (ncol0 > 16)
	plexit("plscm0: Maximum of 16 colors in color map 0.");

    pls[ipls].ncol0 = ncol0;

    for (i = 0; i < ncol0; i++) {
	if ((r[i] < 0 || r[i] > 255) ||
	    (g[i] < 0 || g[i] > 255) ||
	    (b[i] < 0 || b[i] > 255)) {

	    plwarn("plscm0: Invalid color");
	    continue;
	}

	pls[ipls].cmap0[i].r = r[i];
	pls[ipls].cmap0[i].g = g[i];
	pls[ipls].cmap0[i].b = b[i];
	pls[ipls].cmap0setcol[i] = 1;
    }
}

/* Set a given color from color map 0 by 8 bit RGB value */

void
c_plscol0(PLINT icol0, PLINT r, PLINT g, PLINT b)
{
    if (pls[ipls].level > 0) {
	plwarn("plscol0: Must becalled before plinit.");
	return;
    }

    if (icol0 < 0 || icol0 > 15)
	plexit("plscol0: Illegal color table value");

    if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255)) {
	plwarn("plscol0: Invalid color");
	return;
    }

    pls[ipls].cmap0[icol0].r = r;
    pls[ipls].cmap0[icol0].g = g;
    pls[ipls].cmap0[icol0].b = b;
    pls[ipls].cmap0setcol[icol0] = 1;
}

/* Set color map 1 colors by 8 bit RGB values */
/* You MUST set all 256 colors if you use this */

void
c_plscm1(PLINT *r, PLINT *g, PLINT *b)
{
    int i;

    if (pls[ipls].level > 0) {
	plwarn("plscm1: Must be called before plinit.");
	return;
    }

    for (i = 0; i < 256; i++) {
	if ((r[i] < 0 || r[i] > 255) ||
	    (g[i] < 0 || g[i] > 255) ||
	    (b[i] < 0 || b[i] > 255)) {

	    printf("plscm1: Invalid RGB color: %d, %d, %d\n", r[i], g[i], b[i]);
	    plexit("");
	}
	pls[ipls].cmap1[i].r = r[i];
	pls[ipls].cmap1[i].g = g[i];
	pls[ipls].cmap1[i].b = b[i];
    }
    pls[ipls].cmap1set = 1;
}

/* Set color map 1 colors using a linear relationship between function
*  height and position in HLS or RGB color space.
*  There are 6 parameters pointed to by "param"; these correspond to:
*
*   itype=0 (HLS)    itype=1 (RGB)
*   -------------    -------------
*	H-min		R-min
*	H-max		R-max
*	L-min		G-min
*	L-max		G-max
*	S-min		B-min
*	S-max		B-max
*
*  Bounds on RGB coordinates:
*	R,G,B		[0., 1.]	magnitude
*
*  Bounds on HLS coordinates:
*	hue		[0., 360.]	degrees
*	lightness	[0., 1.]	magnitude
*	saturation	[0., 1.]	magnitude
*/

void
c_plscm1f1(PLINT itype, PLFLT *param)
{
    int i;
    PLFLT h, l, s, r, g, b;

    if (pls[ipls].level > 0) {
	plwarn("plscm1f1: Must be called before plinit.");
	return;
    }
    for (i = 0; i < 256; i++) {
	r = param[0] + (param[1] - param[0]) * i / 255.;
	g = param[2] + (param[3] - param[2]) * i / 255.;
	b = param[4] + (param[5] - param[4]) * i / 255.;

	if (itype == 0) {
	    h = r;
	    l = g;
	    s = b;
	    plHLS_RGB(h, l, s, &r, &g, &b);
	}

	if ((r < 0. || r > 1.) || (g < 0. || g > 1.) || (b < 0. || b > 1.)) {
	    printf("plscm1f1: Invalid RGB color: %f, %f, %f\n", r, g, b);
	    plexit("");
	}
	pls[ipls].cmap1[i].r = MIN(255, (int) (256. * r));
	pls[ipls].cmap1[i].g = MIN(255, (int) (256. * g));
	pls[ipls].cmap1[i].b = MIN(255, (int) (256. * b));
    }
    pls[ipls].cmap1set = 1;
}

/* Used to globally turn color output on/off */

void
c_plscolor(PLINT color)
{
    pls[ipls].colorset = 1;
    pls[ipls].color = color;
}

/*----------------------------------------------------------------------*\
*  These set/get information for family files, and may be called prior
*  to plinit to set up the necessary parameters.  Arguments:
*
*	fam	familying flag (boolean)
*	num	member number
*	bmax	maximum member size
\*----------------------------------------------------------------------*/

void
c_plgfam(PLINT *p_fam, PLINT *p_num, PLINT *p_bmax)
{
    *p_fam = pls[ipls].family;
    *p_num = pls[ipls].member;
    *p_bmax = pls[ipls].bytemax;
}

void
c_plsfam(PLINT fam, PLINT num, PLINT bmax)
{
    if (pls[ipls].level > 0)
	plwarn("plsfam: Must be called before plinit.");

    if (fam >= 0)
	pls[ipls].family = fam;
    if (num >= 0)
	pls[ipls].member = num;
    if (bmax >= 0)
	pls[ipls].bytemax = bmax;
}

void
c_plfamadv(void)
{
    pls[ipls].famadv = 1;
}

/*----------------------------------------------------------------------*\
*  Interface routines for axis labling parameters.
*  See pldtik.c for more info.
\*----------------------------------------------------------------------*/

void
c_plgxax(PLINT *p_digmax, PLINT *p_digits)
{
    *p_digmax = pls[ipls].xdigmax;
    *p_digits = pls[ipls].xdigits;
}

void
c_plsxax(PLINT digmax, PLINT digits)
{
    pls[ipls].xdigmax = digmax;
    pls[ipls].xdigits = digits;
}

void
c_plgyax(PLINT *p_digmax, PLINT *p_digits)
{
    *p_digmax = pls[ipls].ydigmax;
    *p_digits = pls[ipls].ydigits;
}

void
c_plsyax(PLINT digmax, PLINT digits)
{
    pls[ipls].ydigmax = digmax;
    pls[ipls].ydigits = digits;
}

void
c_plgzax(PLINT *p_digmax, PLINT *p_digits)
{
    *p_digmax = pls[ipls].zdigmax;
    *p_digits = pls[ipls].zdigits;
}

void
c_plszax(PLINT digmax, PLINT digits)
{
    pls[ipls].zdigmax = digmax;
    pls[ipls].zdigits = digits;
}

/*----------------------------------------------------------------------*\
*  These should not be called by the user.
\*----------------------------------------------------------------------*/

void
glev(PLINT *p_n)
{
    *p_n = pls[ipls].level;
}

void
slev(PLINT n)
{
    pls[ipls].level = n;
}

void
gbase(PLFLT *p_x, PLFLT *p_y, PLFLT *p_xc, PLFLT *p_yc)
{
    *p_x = pls[ipls].base3x;
    *p_y = pls[ipls].base3y;
    *p_xc = pls[ipls].basecx;
    *p_yc = pls[ipls].basecy;
}

void
sbase(PLFLT x, PLFLT y, PLFLT xc, PLFLT yc)
{
    pls[ipls].base3x = x;
    pls[ipls].base3y = y;
    pls[ipls].basecx = xc;
    pls[ipls].basecy = yc;
}

void
gnms(PLINT *p_n)
{
    *p_n = pls[ipls].nms;
}

void
snms(PLINT n)
{
    pls[ipls].nms = n;
}

void
gcurr(PLINT *p_ix, PLINT *p_iy)
{
    *p_ix = pls[ipls].currx;
    *p_iy = pls[ipls].curry;
}

void
scurr(PLINT ix, PLINT iy)
{
    pls[ipls].currx = ix;
    pls[ipls].curry = iy;
}

void
gdom(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = pls[ipls].domxmi;
    *p_xmax = pls[ipls].domxma;
    *p_ymin = pls[ipls].domymi;
    *p_ymax = pls[ipls].domyma;
}

void
sdom(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].domxmi = xmin;
    pls[ipls].domxma = xmax;
    pls[ipls].domymi = ymin;
    pls[ipls].domyma = ymax;
}

void
grange(PLFLT *p_zscl, PLFLT *p_zmin, PLFLT *p_zmax)
{
    *p_zscl = pls[ipls].zzscl;
    *p_zmin = pls[ipls].ranmi;
    *p_zmax = pls[ipls].ranma;
}

void
srange(PLFLT zscl, PLFLT zmin, PLFLT zmax)
{
    pls[ipls].zzscl = zscl;
    pls[ipls].ranmi = zmin;
    pls[ipls].ranma = zmax;
}

void
gw3wc(PLFLT *p_dxx, PLFLT *p_dxy, PLFLT *p_dyx, PLFLT *p_dyy, PLFLT *p_dyz)
{
    *p_dxx = pls[ipls].cxx;
    *p_dxy = pls[ipls].cxy;
    *p_dyx = pls[ipls].cyx;
    *p_dyy = pls[ipls].cyy;
    *p_dyz = pls[ipls].cyz;
}

void
sw3wc(PLFLT dxx, PLFLT dxy, PLFLT dyx, PLFLT dyy, PLFLT dyz)
{
    pls[ipls].cxx = dxx;
    pls[ipls].cxy = dxy;
    pls[ipls].cyx = dyx;
    pls[ipls].cyy = dyy;
    pls[ipls].cyz = dyz;
}

void
gvpp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = pls[ipls].vppxmi;
    *p_ixmax = pls[ipls].vppxma;
    *p_iymin = pls[ipls].vppymi;
    *p_iymax = pls[ipls].vppyma;
}

void
svpp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    pls[ipls].vppxmi = ixmin;
    pls[ipls].vppxma = ixmax;
    pls[ipls].vppymi = iymin;
    pls[ipls].vppyma = iymax;
}

void
gspp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = pls[ipls].sppxmi;
    *p_ixmax = pls[ipls].sppxma;
    *p_iymin = pls[ipls].sppymi;
    *p_iymax = pls[ipls].sppyma;
}

void
sspp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    pls[ipls].sppxmi = ixmin;
    pls[ipls].sppxma = ixmax;
    pls[ipls].sppymi = iymin;
    pls[ipls].sppyma = iymax;
}

void
gclp(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = pls[ipls].clpxmi;
    *p_ixmax = pls[ipls].clpxma;
    *p_iymin = pls[ipls].clpymi;
    *p_iymax = pls[ipls].clpyma;
}

void
sclp(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    pls[ipls].clpxmi = ixmin;
    pls[ipls].clpxma = ixmax;
    pls[ipls].clpymi = iymin;
    pls[ipls].clpyma = iymax;
}

void
gphy(PLINT *p_ixmin, PLINT *p_ixmax, PLINT *p_iymin, PLINT *p_iymax)
{
    *p_ixmin = pls[ipls].phyxmi;
    *p_ixmax = pls[ipls].phyxma;
    *p_iymin = pls[ipls].phyymi;
    *p_iymax = pls[ipls].phyyma;
}

void
sphy(PLINT ixmin, PLINT ixmax, PLINT iymin, PLINT iymax)
{
    pls[ipls].phyxmi = ixmin;
    pls[ipls].phyxma = ixmax;
    pls[ipls].phyymi = iymin;
    pls[ipls].phyyma = iymax;
}

void
gsub(PLINT *p_nx, PLINT *p_ny, PLINT *p_cs)
{
    *p_nx = pls[ipls].nsubx;
    *p_ny = pls[ipls].nsuby;
    *p_cs = pls[ipls].cursub;
}

void
ssub(PLINT nx, PLINT ny, PLINT cs)
{
    pls[ipls].nsubx = nx;
    pls[ipls].nsuby = ny;
    pls[ipls].cursub = cs;
}

void
gumpix(PLINT *p_ix, PLINT *p_iy)
{
    *p_ix = pls[ipls].umx;
    *p_iy = pls[ipls].umy;
}

void
sumpix(PLINT ix, PLINT iy)
{
    pls[ipls].umx = ix;
    pls[ipls].umy = iy;
}

void
gatt(PLINT *p_ifnt, PLINT *p_icol0)
{
    *p_ifnt = font;
    *p_icol0 = pls[ipls].icol0;
}

void
satt(PLINT ifnt, PLINT icol0)
{
    font = ifnt;
    pls[ipls].icol0 = icol0;
}

void
gcol(PLINT *p_icol0)
{
    *p_icol0 = pls[ipls].icol0;
}

void
scol(PLINT icol0)
{
    pls[ipls].icol0 = icol0;
}

void
gwid(PLINT *p_pwid)
{
    *p_pwid = pls[ipls].width;
}

void
swid(PLINT pwid)
{
    pls[ipls].width = pwid;
}

void
gspd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = pls[ipls].spdxmi;
    *p_xmax = pls[ipls].spdxma;
    *p_ymin = pls[ipls].spdymi;
    *p_ymax = pls[ipls].spdyma;
}

void
sspd(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].spdxmi = xmin;
    pls[ipls].spdxma = xmax;
    pls[ipls].spdymi = ymin;
    pls[ipls].spdyma = ymax;
}

void
gvpd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = pls[ipls].vpdxmi;
    *p_xmax = pls[ipls].vpdxma;
    *p_ymin = pls[ipls].vpdymi;
    *p_ymax = pls[ipls].vpdyma;
}

void
svpd(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].vpdxmi = xmin;
    pls[ipls].vpdxma = xmax;
    pls[ipls].vpdymi = ymin;
    pls[ipls].vpdyma = ymax;
}

void
gvpw(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax)
{
    *p_xmin = pls[ipls].vpwxmi;
    *p_xmax = pls[ipls].vpwxma;
    *p_ymin = pls[ipls].vpwymi;
    *p_ymax = pls[ipls].vpwyma;
}

void
svpw(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax)
{
    pls[ipls].vpwxmi = xmin;
    pls[ipls].vpwxma = xmax;
    pls[ipls].vpwymi = ymin;
    pls[ipls].vpwyma = ymax;
}

void
gpixmm(PLFLT *p_x, PLFLT *p_y)
{
    *p_x = pls[ipls].xpmm;
    *p_y = pls[ipls].ypmm;
}

void
spixmm(PLFLT x, PLFLT y)
{
    pls[ipls].xpmm = x;
    pls[ipls].ypmm = y;
    pls[ipls].umx = 1000.0 / pls[ipls].xpmm;
    pls[ipls].umy = 1000.0 / pls[ipls].ypmm;
}

/* All the drivers call this to set physical pixels/mm. */

void
setpxl(PLFLT xpmm0, PLFLT ypmm0)
{
    pls[ipls].xpmm = xpmm0;
    pls[ipls].ypmm = ypmm0;
    pls[ipls].umx = 1000.0 / pls[ipls].xpmm;
    pls[ipls].umy = 1000.0 / pls[ipls].ypmm;
}

/* Sets up physical limits of plotting device and the mapping between
   normalized device coordinates and physical coordinates. */

void
setphy(PLINT xmin, PLINT xmax, PLINT ymin, PLINT ymax)
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
gwp(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = pls[ipls].wpxscl;
    *p_xoff = pls[ipls].wpxoff;
    *p_yscl = pls[ipls].wpyscl;
    *p_yoff = pls[ipls].wpyoff;
}

void
swp(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].wpxscl = xscl;
    pls[ipls].wpxoff = xoff;
    pls[ipls].wpyscl = yscl;
    pls[ipls].wpyoff = yoff;
}

void
gwm(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = pls[ipls].wmxscl;
    *p_xoff = pls[ipls].wmxoff;
    *p_yscl = pls[ipls].wmyscl;
    *p_yoff = pls[ipls].wmyoff;
}

void
swm(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].wmxscl = xscl;
    pls[ipls].wmxoff = xoff;
    pls[ipls].wmyscl = yscl;
    pls[ipls].wmyoff = yoff;
}

void
gdp(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = pls[ipls].dpxscl;
    *p_xoff = pls[ipls].dpxoff;
    *p_yscl = pls[ipls].dpyscl;
    *p_yoff = pls[ipls].dpyoff;
}

void
sdp(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].dpxscl = xscl;
    pls[ipls].dpxoff = xoff;
    pls[ipls].dpyscl = yscl;
    pls[ipls].dpyoff = yoff;
}

void
gmp(PLFLT *p_xscl, PLFLT *p_xoff, PLFLT *p_yscl, PLFLT *p_yoff)
{
    *p_xscl = pls[ipls].mpxscl;
    *p_xoff = pls[ipls].mpxoff;
    *p_yscl = pls[ipls].mpyscl;
    *p_yoff = pls[ipls].mpyoff;
}

void
smp(PLFLT xscl, PLFLT xoff, PLFLT yscl, PLFLT yoff)
{
    pls[ipls].mpxscl = xscl;
    pls[ipls].mpxoff = xoff;
    pls[ipls].mpyscl = yscl;
    pls[ipls].mpyoff = yoff;
}

void
gchr(PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = pls[ipls].chrdef;
    *p_ht = pls[ipls].chrht;
}

void
schr(PLFLT def, PLFLT ht)
{
    pls[ipls].chrdef = def;
    pls[ipls].chrht = ht;
}

void
gsym(PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = pls[ipls].symdef;
    *p_ht = pls[ipls].symht;
}

void
ssym(PLFLT def, PLFLT ht)
{
    pls[ipls].symdef = def;
    pls[ipls].symht = ht;
}

void
gmaj(PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = pls[ipls].majdef;
    *p_ht = pls[ipls].majht;
}

void
smaj(PLFLT def, PLFLT ht)
{
    pls[ipls].majdef = def;
    pls[ipls].majht = ht;
}

void
gmin(PLFLT *p_def, PLFLT *p_ht)
{
    *p_def = pls[ipls].mindef;
    *p_ht = pls[ipls].minht;
}

void
smin(PLFLT def, PLFLT ht)
{
    pls[ipls].mindef = def;
    pls[ipls].minht = ht;
}

void
gpat(PLINT *p_inc[], PLINT *p_del[], PLINT *p_nlin)
{
    *p_inc = pls[ipls].inclin;
    *p_del = pls[ipls].delta;
    *p_nlin = pls[ipls].nps;
}

void
spat(PLINT inc[], PLINT del[], PLINT nlin)
{
    PLINT i;

    pls[ipls].nps = nlin;
    for (i = 0; i < nlin; i++) {
	pls[ipls].inclin[i] = inc[i];
	pls[ipls].delta[i] = del[i];
    }
}

void
gmark(PLINT *p_mar[], PLINT *p_spa[], PLINT *p_nms)
{
    *p_mar = pls[ipls].mark;
    *p_spa = pls[ipls].space;
    *p_nms = pls[ipls].nms;
}

void
smark(PLINT mar[], PLINT spa[], PLINT nms)
{
    PLINT i;

    pls[ipls].nms = nms;
    for (i = 0; i < nms; i++) {
	pls[ipls].mark[i] = mar[i];
	pls[ipls].space[i] = spa[i];
    }
}

void
gcure(PLINT **p_cur, PLINT **p_pen, PLINT **p_tim, PLINT **p_ala)
{
    *p_cur = &(pls[ipls].curel);
    *p_pen = &(pls[ipls].pendn);
    *p_tim = &(pls[ipls].timecnt);
    *p_ala = &(pls[ipls].alarm);
}

void
scure(PLINT cur, PLINT pen, PLINT tim, PLINT ala)
{
    pls[ipls].curel = cur;
    pls[ipls].pendn = pen;
    pls[ipls].timecnt = tim;
    pls[ipls].alarm = ala;
}
