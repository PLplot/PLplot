/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:32:31  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	dispatch.c

	Initialize the device list.
	Also driver support functions are here.

	Each device is selected by the appropriate define, passed in from 
	the makefile.  When installing plplot you may wish to exclude devices
	not present on your system in order to reduce screen clutter.  

	The dispatch table entries are as follows:

	- Device description name.  Appears in the device menu list. 
	- Device keyword name.  Appears in the device menu list. Generic 
	  names like "printer" are OK as long as they only appear once in 
	  the device list. 
	- Initialize device. 
	- Draw line between two points 
	- Clear screen (or eject page). 
	- Set up new page. 
	- Advance to new page (same as clear & setup on most devices).
	- Tidy up device (flush buffers, close file, etc.) 
	- Change pen color. 
	- Switch to text mode (or screen). 
	- Switch to graphics mode (or screen). 
	- Set pen width. 
	- Escape function, for driver-specific commands. 
*/

#include <stdio.h>
#include "plplot.h"
#include "dispatch.h"

/* Functions only in this file */

static void plSclSetup PLARGS((PLStream *, PLDev *));

/* Dispatch table initialization */
/* Note: if you hit a <CR> in response to plstar() prompt, you get the
   FIRST one active below, so arrange them accordingly for your system. 
   (i.e. all the system-specific ones should go first, since they won't
   appear on most systems.) */

DISPATCH_TABLE plDispatchTable[] = {
    /* Terminal types */
#ifdef NEXT
    {
        "NeXT Display",
        "next",
        nxinit,
        nxline,
        nxclear,
        nxpage,
        nxadv,
        nxtidy,
        nxcolor,
        nxtext,
        nxgraph,
        nxwidth,
        nxesc
    },
#endif
#ifdef AMIGA
   {
	"Amiga Window",
	"amiwn",
	amiwninit,
	amiwnline,
	amiwnclear,
	amiwnpage,
	amiwnadv,
	amiwntidy,
	amiwncolor,
	amiwntext,
	amiwngraph,
	amiwnwidth,
	amiwnesc
   },
#endif
#ifdef OS2PM
    {
	"OS/2 PM Screen",
	"os2",
	os2init,
	os2line,
	os2clear,
	os2page,
	os2adv,
	os2tidy,
	os2color,
	os2text,
	os2graph,
	os2width,
	os2esc
    },
#endif
#ifdef XWIN
    {
	"X-Window Screen",
	"xwin",
	xw_init,
	xw_line,
	xw_clear,
	xw_page,
	xw_adv,
	xw_tidy,
	xw_color,
	xw_text,
	xw_graph,
	xw_width,
	xw_esc
    },
#endif
#ifdef VGA
    {
	"DOS VGA Screen",
	"vga",
	vgainit,
	vgaline,
	vgaclear,
	vgapage,
	vgaadv,
	vgatidy,
	vgacolor,
	vgatext,
	vgagraph,
	vgawidth,
	vgaesc
    },
#endif
#ifdef GNUSVGA
    {
	"DOS SVGA Screen",
	"svga",
	svgainit,
	svgaline,
	svgaclear,
	svgapage,
	svgaadv,
	svgatidy,
	svgacolor,
	svgatext,
	svgagraph,
	svgawidth,
	svgaesc
    },
#endif
#ifdef XTERM
    {
	"Xterm Window",
	"xterm",
	xteinit,
	xteline,
	xteclear,
	xtepage,
	xteadv,
	xtetidy,
	xtecolor,
	xtetext,
	xtegraph,
	xtewidth,
	xteesc
    },
#endif
#ifdef TEK
    {
	"Tektronix Terminal",
	"tekt",
	tektinit,
	tekline,
	tekclear,
	tekpage,
	tekadv,
	tektidy,
	tekcolor,
	tektext,
	tekgraph,
	tekwidth,
	tekesc
    },
#endif
#ifdef DG300
    {
	"DG300 Terminal",
	"dg300",
	dginit,
	dgline,
	dgclear,
	dgpage,
	dgadv,
	dgtidy,
	dgcolor,
	dgtext,
	dggraph,
	dgwidth,
	dgesc
    },
#endif
    /* File types */
#ifdef PLMETA
    {
	"PLPLOT Native Meta-File",
	"plmeta",
	plminit,
	plmline,
	plmclear,
	plmpage,
	plmadv,
	plmtidy,
	plmcolor,
	plmtext,
	plmgraph,
	plmwidth,
	plmesc
    },
#endif
#ifdef TEK
    {
	"Tektronix File",
	"tekf",
	tekfinit,
	tekline,
	tekclear,
	tekpage,
	tekadv,
	tektidy,
	tekcolor,
	tektext,
	tekgraph,
	tekwidth,
	tekesc
    },
#endif
#ifdef PS
    {
	"PostScript File",
	"ps",
	psinit,
	psline,
	psclear,
	pspage,
	psadv,
	pstidy,
	pscolor,
	pstext,
	psgraph,
	pswidth,
	psesc
    },
#endif
#ifdef XFIG
    {
	"Xfig file",
	"xfig",
	xfiginit,
	xfigline,
	xfigclear,
	xfigpage,
	xfigadv,
	xfigtidy,
	xfigcolor,
	xfigtext,
	xfiggraph,
	xfigwidth,
	xfigesc
    },
#endif
#ifdef LJII
    {
	"LaserJet II Bitmap File (150 dpi)",
	"ljii",
	jetinit,
	jetline,
	jetclear,
	jetpage,
	jetadv,
	jettidy,
	jetcolor,
	jettext,
	jetgraph,
	jetwidth,
	jetesc
    },
#endif
#ifdef AMIGA
    {
	"Amiga Printer (prefs settings)",
	"amipr",
	amiprinit,
	amiprline,
	amiprclear,
	amiprpage,
	amipradv,
	amiprtidy,
	amiprcolor,
	amiprtext,
	amiprgraph,
	amiprwidth,
	amipresc
    },
#endif
#ifdef IFF	/* probably only of interest on the Amiga */
   {
	"IFF Graphics File",
	"iff",
	iffinit,
	iffline,
	iffclear,
	iffpage,
	iffadv,
	ifftidy,
	iffcolor,
	ifftext,
	iffgraph,
	iffwidth,
	iffesc
   },
#endif
#ifdef AEGIS	/* probably only of interest on the Amiga */
   {
	"Aegis Draw File",
	"aegis",
	aegisinit,
	aegisline,
	aegisclear,
	aegispage,
	aegisadv,
	aegistidy,
	aegiscolor,
	aegistext,
	aegisgraph,
	aegiswidth,
	aegisesc
   },
#endif
#ifdef HP7470
    {
	"HP 7470 Plotter File (HPGL Cartridge, Small Plotter)",
	"hp7470",
	hp7470init,
	hp7470line,
	hp7470clear,
	hp7470page,
	hp7470adv,
	hp7470tidy,
	hp7470color,
	hp7470text,
	hp7470graph,
	hp7470width,
	hp7470esc
    },
#endif
#ifdef HP7580
    {
	"HP 7580 Plotter File (Large Plotter)",
	"hp7580",
	hp7580init,
	hp7580line,
	hp7580clear,
	hp7580page,
	hp7580adv,
	hp7580tidy,
	hp7580color,
	hp7580text,
	hp7580graph,
	hp7580width,
	hp7580esc
    },
#endif
#ifdef IMP
    {
	"Impress File",
	"imp",
	impinit,
	impline,
	impclear,
	imppage,
	impadv,
	imptidy,
	impcolor,
	imptext,
	impgraph,
	impwidth,
	impesc
    }
#endif
};

PLINT npldrivers = (sizeof(plDispatchTable)/sizeof(struct dispatch_table));

static char line[80];

/*----------------------------------------------------------------------*\
*  plOpenFile()
*
*  Opens file for output, prompting if not set.
*  Prints extra newline at end to make output look better in batch runs.
\*----------------------------------------------------------------------*/

void
plOpenFile(pls)
PLStream *pls;
{
    int i=0;

    while (!pls->OutFile) {
	if (!pls->fileset) {
	    printf("Enter desired name for graphics output file: ");
	    fgets(line, sizeof(line), stdin);
	    line[strlen(line) - 1] = '\0';

	    strncpy(pls->FileName, line, sizeof(pls->FileName) - 1);
	    pls->FileName[sizeof(pls->FileName) - 1] = '\0';
	    pls->fileset = 1;
	    strcpy(pls->FamilyName, pls->FileName);
	}
	if (pls->family) {
	    (void) sprintf(pls->FileName, "%s.%i", pls->FamilyName, pls->member);
	}

	if ((pls->OutFile = fopen(pls->FileName, BINARY_WRITE)) == NULL) {
	    printf("Can't open %s.\n", pls->FileName);
	    pls->fileset = 0;
	}
	else
	    printf("\n");

	if (i++ > 10)
	    plexit("Too many tries.");
    }
}

/*----------------------------------------------------------------------*\
*  plFamInit()
*
*  Initializes family file parameters.
\*----------------------------------------------------------------------*/

void
plFamInit(pls)
PLStream *pls;
{
    if (pls->family) {
	pls->bytecnt = 0;
	if (!pls->member)
	    pls->member = 1;
	if (!pls->bytemax)
	    pls->bytemax = PL_FILESIZE_KB * 1000;
    }
}

/*----------------------------------------------------------------------*\
*  plGetFam()
*
*  Starts new member file of family file set if necessary.
*
*  Note each member file is a complete graphics file (can be printed 
*  individually), although 'plrender' will treat a family as a single 
*  logical file if given the family name instead of the member name.
\*----------------------------------------------------------------------*/

void
plGetFam(pls)
PLStream *pls;
{
    if (pls->family) {
	if (pls->bytecnt > pls->bytemax) {
	    grtidy();
	    pls->fileset = 1;
	    pls->member++;
	    grinit();
	    return;
	}
    }
}

/*----------------------------------------------------------------------*\
*  plGetInt()
*
*  Prompts human to input an integer in response to given message.
\*----------------------------------------------------------------------*/

PLINT 
plGetInt(s)
char *s;
{
    PLINT m;
    int i=0;

    while (i++ < 10) {
	printf(s);
	fgets(line, sizeof(line), stdin);
#ifdef MSDOS
	m = atoi(line);
	return (m);
#else
	if (sscanf(line, "%d", &m) == 1)
	    return (m);
	printf("No value or value out of range; please try again\n");
#endif
    }
    plexit("Too many tries.");
}

/*----------------------------------------------------------------------*\
*  plGetFlt()
*
*  Prompts human to input a float in response to given message.
\*----------------------------------------------------------------------*/

PLFLT 
plGetFlt(s)
char *s;
{
    PLFLT m;
    int i=0;

    while (i++ < 10) {
	printf(s);
	fgets(line, sizeof(line), stdin);
#ifdef MSDOS
	m = atof(line);
	return (m);
#else
	if (sscanf(line, "%f", &m) == 1)
	    return (m);
	printf("No value or value out of range; please try again\n");
#endif
    }
    plexit("Too many tries.");
}

/*----------------------------------------------------------------------*\
*  plRotPhy()
*
*  Rotates physical coordinates if necessary for given orientation.
\*----------------------------------------------------------------------*/

void
plRotPhy(pls, dev, px1, py1, px2, py2)
PLStream *pls;
PLDev *dev;
int *px1, *py1, *px2, *py2;
{
    int x1o, y1o, x2o, y2o;

    x1o = *px1; y1o = *py1;
    x2o = *px2; y2o = *py2;

    if (!pls->orient) {
	*px1 = x1o;
	*py1 = y1o;
	*px2 = x2o;
	*py2 = y2o;
    }
    else {
	*px1 = dev->xmax - (y1o - dev->ymin);
	*py1 = dev->ymin + (x1o - dev->xmin);
	*px2 = dev->xmax - (y2o - dev->ymin);
	*py2 = dev->ymin + (x2o - dev->xmin);
    }
}

/*----------------------------------------------------------------------*\
*  plSclPhy()
*
*  Scales physical coordinates to get a specified aspect ratio on plot.
\*----------------------------------------------------------------------*/

void
plSclPhy(pls, dev, px1, py1, px2, py2)
PLStream *pls;
PLDev *dev;
int *px1, *py1, *px2, *py2;
{
    int x1o, y1o, x2o, y2o;

    if (pls->aspect <= 0.)	/* Failsafe (should never happen) */
	return;

    x1o = *px1; y1o = *py1;
    x2o = *px2; y2o = *py2;

    if (pls->aspectset) {
	plSclSetup(pls, dev);
	pls->aspectset = 0;
    }
    *px1 = dev->sclxmin + (x1o - dev->xmin)*dev->rsclx;
    *px2 = dev->sclxmin + (x2o - dev->xmin)*dev->rsclx;
    *py1 = dev->sclymin + (y1o - dev->ymin)*dev->rscly;
    *py2 = dev->sclymin + (y2o - dev->ymin)*dev->rscly;
}

/*----------------------------------------------------------------------*\
*  plSclSetup()
*
*  Gets physical scaling factors in response to given aspect ratio request.
*  Largest box that satisfies given aspect ratio that fits on the page
*  or subpage will be used.
\*----------------------------------------------------------------------*/

static void 
plSclSetup(pls, dev)
PLStream *pls;
PLDev *dev;
{
    PLINT sclxlen, sclylen, devxlen, devylen;

/* Get global aspect scaling factors */

    devxlen = dev->xmax - dev->xmin;
    devylen = dev->ymax - dev->ymin;

    sclylen = devylen;
    sclxlen = sclylen/pls->aspect;
    if (sclxlen > devxlen) {
	sclxlen = devxlen;
	sclylen = sclxlen*pls->aspect;
    }

    dev->sclxmin = dev->xmin + (dev->xmax - sclxlen)/2.0;
    dev->sclymin = dev->ymin + (dev->ymax - sclylen)/2.0;

    dev->rsclx = (double) sclxlen / (double) devxlen;
    dev->rscly = (double) sclylen / (double) devylen;
}
