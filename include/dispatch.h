/* $Id$
   $Log$
   Revision 1.6.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.6  1992/11/07 08:02:36  mjl
   Added dispatch table entries for Null device driver and Motif driver.

 * Revision 1.5  1992/10/22  17:05:18  mjl
 * Fixed warnings, errors generated when compling with HP C++.
 *
 * Revision 1.4  1992/10/20  20:14:09  mjl
 * Added prototypes, definitions for new routine plfamadv(), for advancing
 * to next family member file.
 *
 * Revision 1.3  1992/09/30  18:25:31  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:29  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:33:38  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	dispatch.h

	Contains all information needed by the drivers & dispatch routines.
	NOTE: <stdio.h> must be included prior to this file.  The number
	of files needing access to this header file should be small.
*/

/*----------------------------------------------------------------------*\
*  Define the PLDev data structure.
*
*  These are all quantities that must be saved on a per-device basis.
\*----------------------------------------------------------------------*/

typedef struct {
    PLFLT pxlx, pxly;
    PLINT xold, yold;
    PLINT xmin, xmax, xlen;
    PLINT ymin, ymax, ylen;
    PLINT sclxmin, sclxmax;
    PLINT sclymin, sclymax;
    PLFLT rsclx, rscly;
} PLDev;

/*----------------------------------------------------------------------*\
* Here we define the PLStream data structure.
* It contains a copy of every variable that is stream dependent, which
* tends to be those which are settable by the user and persist for longer
* than one plot.
*
* Only those quantities listed in the PLStream struct will be preserved
* for an individual stream.  Note that the lack of every plplot constant
* means that one should only switch streams at a fairly high level, i.e.
* on the scale of an entire plot.  Otherwise intermediate quantities
* will be confused between the streams, possibly resulting in garbage plots.
* This structure may be expanded in time to improve intra-stream independence,
* but it is doubtful streams will ever be fully independent (perhaps
* neither necessary nor desirable? -- time will tell).
*
* The quantities in each stream are as follows:
*
* ipls		Stream number
* ipld		Device number
* level		Initialization level
* device	Graphics device id number
* termin	Zero for noninteractive device, non-zero for terminal
* graphx	Zero if currently in text mode, non-zero for graphics mode
* nopause	Zero if we are pausing between frames
* family	Non-zero if familying is enabled
* member	Number of current family member file open
* bytemax	Number of bytes maximum per member file
* famadv        Non-zero to advance to the next family member
* nsub...	Number of subpages on physical device
* cursub	Current subpage
* color		Current default color
* width		Current pen width
* scale		Scaling factor for chr, sym, maj, min.
* chr...	Character default height and current (scaled) height
* sym...	Symbol    default height and current (scaled) height
* maj...	Major tick default height and current (scaled) height
* min...	Minor tick default height and current (scaled) height
* xdigmax..	Allowed #digits in axes labels
* xdigits..	Actual field widths (returned)
*
* pscale 	Physical coordinate scaling flag
* aspect	Global aspect ratio
* aspectset	Signals that the user modified aspect
* lpbp..	Local plot boundaries in physical coordinates
* vpp..		Viewport boundaries in physical coordinates
* spp..		Subpage  boundaries in physical coordinates
* clp..		Clip     boundaries in physical coordinates
* phy...	Physical device limits in physical coordinates
* um.		Number of micrometres in a pixel
*
* currx		Physical x-coordinate of current point
* curry		Physical y-coordinate of current point
* mark		Array of mark lengths in micrometres for broken lines
* space		Array of space lengths in micrometres for broken lines
* nms		Number of elements for current broken line style
* timecnt	Timer for broken lines
* alarm		Alarm indicating change of broken line status
* pendn		Flag indicating if pen is up or down
* curel		Current element within broken line
* inclin	Array of inclinations in tenths of degree for fill lines
* delta		Array of spacings in micrometers between fill lines
* nps		Number of distinct line styles for fills
*
* xdpi..	Device DPI settings in x and y (if supported)
* xlength..	Device output lengths in x and y (if supported)
* xoffset..	Device offsets from upper left hand corner (if supported)
*
* spd...	Subpage  boundaries in normalized device coordinates
* vpd...	Viewport boundaries in normalized device coordinates
* vpw...	Viewport boundaries in world coordinates
* pmm		Number of pixels to a millimetre
* wp....	Transformation variables for world  to physical conversion
* dp....	Transformation variables for device to physical conversion
* mp....	Transformation variables for millimetres from bottom left
*		hand corner to physical coordinates
* wm....	Transformation variables for world coordinates to mm
* base3.	World coordinate size of base for 3-d plot
* basec.	Position of centre of base for 3-d plot
* dom...	Minimum and maximum values for domain
* zzscl		Vertical (z) scale for 3-d plot
* ran..		Minimum and maximum z values for 3-d plot
* c..		Coordinate transformation from 3-d to 2-d
*
\*----------------------------------------------------------------------*/

typedef struct {
    PLINT ipls, ipld;
    FILE *OutFile;
    PLINT orientset, fileset, pageset;
    char FamilyName[80], FileName[90];

    int		plbuf_enable;
    int		plbuf_read;
    int		plbuf_write;
    FILE	*plbufFile;
    char	plbufFnam[90];

    PLINT bytecnt;
    PLINT page;
    PLINT linepos;
    PLINT level;
    PLINT device, termin, graphx;
    PLINT nopause;
    PLINT family, member, bytemax, famadv;
    PLFLT sclx, scly;
    PLINT nsubx, nsuby, cursub;
    PLINT color, width, style;
    PLINT orient;
    PLFLT xdpi, ydpi;
    PLINT xlength, ylength;
    PLINT xoffset, yoffset;
    PLINT xdigmax, ydigmax, zdigmax;
    PLINT xdigits, ydigits, zdigits;
    PLFLT scale;
    PLFLT chrdef, chrht;
    PLFLT symdef, symht;
    PLFLT majdef, majht;
    PLFLT mindef, minht;
    PLINT pscale;
    PLFLT aspect;
    PLINT aspectset;
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLINT sppxmi, sppxma, sppymi, sppyma;
    PLINT clpxmi, clpxma, clpymi, clpyma;
    PLINT phyxmi, phyxma, phyymi, phyyma;
    PLINT lpbpxmi, lpbpxma, lpbpymi, lpbpyma;
    PLINT umx, umy;
    PLINT currx, curry;
    PLINT mark[10], space[10], nms;
    PLINT timecnt, alarm, pendn, curel;
    PLINT inclin[2], delta[2], nps;

    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLFLT vpdxmi, vpdxma, vpdymi, vpdyma;
    PLFLT vpwxmi, vpwxma, vpwymi, vpwyma;
    PLFLT xpmm, ypmm;
    PLFLT wpxscl, wpxoff, wpyscl, wpyoff;
    PLFLT dpxscl, dpxoff, dpyscl, dpyoff;
    PLFLT mpxscl, mpxoff, mpyscl, mpyoff;
    PLFLT wmxscl, wmxoff, wmyscl, wmyoff;
    PLFLT base3x, base3y, basecx, basecy;
    PLFLT domxmi, domxma, domymi, domyma;
    PLFLT zzscl, ranmi, ranma;
    PLFLT cxx, cxy, cyx, cyy, cyz;
} PLStream;

/*----------------------------------------------------------------------*\
* Declare structure containing pointers to device dependent functions.
*
* pl_MenuStr	Pointer to string that is printed in device menu. 
*
* pl_DevName	A short device "name" for device selection by name. 
*
* pl_setup	Use this routine to set orientation, x and y  resolution
*		(dots/mm)  and x and y page widths. Some device drivers
*		may  choose to ignore any or all of these. A call to
*		this routine is optional. If a particular driver requires
*		any of these parameters and they are not set by a call to
*		pl_setup() then they should be prompted for in pl_init().
*		The user may call this routine only once and it is called
*		before plstar() or plbeg(). 
*
* pl_orient	Sets orientation of output to landscape or portrait.
*
* pl_init	Initialize device.  This routine may also prompt the user
*		for certain device parameters or open a graphics file (see
*		note). Called only once to set things up. 
*
* pl_line	Draws a line between two points. 
*
* pl_clear	Clears screen or ejects page or closes file (see note). 
*
* pl_page	Set up for plotting on a new page. May also open a new
*		a new graphics file (see note). 
*
* pl_adv	Advances to new page.  Same as a pl_clear followed by a
*		pl_page on most devices.
*
* pl_tidy	Tidy up. May close graphics file (see note). 
*
* pl_color	Change pen color. 
*
* pl_text	Switch device to text mode. 
*
* pl_graph	Switch device to graphics mode. 
*
* pl_width	Set graphics pen width. 
*
* pl_esc	Escape function for driver-specific commands.
*
* NOTE: Most devices allow multi-page plots to be stored in a single 
*	graphics file, in which case the graphics file should be opened 
*	in the pl_init() routine, closed in pl_tidy(), and page advances
*	done by pl_adv(). If multi-page plots need to be stored in
*	different files then pl_page() should open the file and pl_clear()
*	should close it. Do NOT open files in both pl_init() and pl_page()
*	or close files in both pl_clear() and pl_tidy().
\*----------------------------------------------------------------------*/

struct dispatch_table {
   char *pl_MenuStr;
   char *pl_DevName;
   void (*pl_init)	(PLStream *);
   void (*pl_line)	(PLStream *, PLINT, PLINT, PLINT, PLINT);
   void (*pl_clear)	(PLStream *);
   void (*pl_page)	(PLStream *);
   void (*pl_adv)	(PLStream *);
   void (*pl_tidy)	(PLStream *);
   void (*pl_color)	(PLStream *);
   void (*pl_text)	(PLStream *);
   void (*pl_graph)	(PLStream *);
   void (*pl_width)	(PLStream *);
   void (*pl_esc)	(PLStream *, PLINT, char *);
};

typedef struct dispatch_table  DISPATCH_TABLE;

/*----------------------------------------------------------------------*\
* Prototypes for all driver functions.
\*----------------------------------------------------------------------*/

void xteinit		(PLStream *);
void xteline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void xteclear		(PLStream *);
void xtepage		(PLStream *);
void xteadv		(PLStream *);
void xtetidy		(PLStream *);
void xtecolor		(PLStream *);
void xtetext		(PLStream *);
void xtegraph		(PLStream *);
void xtewidth		(PLStream *);
void xteesc		(PLStream *, PLINT, char *);

void xm_init		(PLStream *);
void xm_line		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void xm_clear		(PLStream *);
void xm_page		(PLStream *);
void xm_adv		(PLStream *);
void xm_tidy		(PLStream *);
void xm_color		(PLStream *);
void xm_text		(PLStream *);
void xm_graph		(PLStream *);
void xm_width		(PLStream *);
void xm_esc		(PLStream *, PLINT, char *);

void xw_init		(PLStream *);
void xw_line		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void xw_clear		(PLStream *);
void xw_page		(PLStream *);
void xw_adv		(PLStream *);
void xw_tidy		(PLStream *);
void xw_color		(PLStream *);
void xw_text		(PLStream *);
void xw_graph		(PLStream *);
void xw_width		(PLStream *);
void xw_esc		(PLStream *, PLINT, char *);

void tektinit		(PLStream *);
void tekfinit		(PLStream *);
void tekline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void tekclear		(PLStream *);
void tekpage		(PLStream *);
void tekadv		(PLStream *);
void tektidy		(PLStream *);
void tekcolor		(PLStream *);
void tektext		(PLStream *);
void tekgraph		(PLStream *);
void tekwidth		(PLStream *);
void tekesc		(PLStream *, PLINT, char *);

void dginit		(PLStream *);
void dgline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void dgclear		(PLStream *);
void dgpage		(PLStream *);
void dgadv		(PLStream *);
void dgtidy		(PLStream *);
void dgcolor		(PLStream *);
void dgtext		(PLStream *);
void dggraph		(PLStream *);
void dgwidth		(PLStream *);
void dgesc		(PLStream *, PLINT, char *);

void hp7470init		(PLStream *);
void hp7470line		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void hp7470clear	(PLStream *);
void hp7470page		(PLStream *);
void hp7470adv		(PLStream *);
void hp7470tidy		(PLStream *);
void hp7470color	(PLStream *);
void hp7470text		(PLStream *);
void hp7470graph	(PLStream *);
void hp7470width	(PLStream *);
void hp7470esc		(PLStream *, PLINT, char *);

void hp7580init		(PLStream *);
void hp7580line		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void hp7580clear	(PLStream *);
void hp7580page		(PLStream *);
void hp7580adv		(PLStream *);
void hp7580tidy		(PLStream *);
void hp7580color	(PLStream *);
void hp7580text		(PLStream *);
void hp7580graph	(PLStream *);
void hp7580width	(PLStream *);
void hp7580esc		(PLStream *, PLINT, char *);

void impinit		(PLStream *);
void impline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void impclear		(PLStream *);
void imppage		(PLStream *);
void impadv		(PLStream *);
void imptidy		(PLStream *);
void impcolor		(PLStream *);
void imptext		(PLStream *);
void impgraph		(PLStream *);
void impwidth		(PLStream *);
void impesc		(PLStream *, PLINT, char *);

void xfiginit		(PLStream *);
void xfigline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void xfigclear		(PLStream *);
void xfigpage		(PLStream *);
void xfigadv		(PLStream *);
void xfigtidy		(PLStream *);
void xfigcolor		(PLStream *);
void xfigtext		(PLStream *);
void xfiggraph		(PLStream *);
void xfigwidth		(PLStream *);
void xfigesc		(PLStream *, PLINT, char *);

void jetinit		(PLStream *);
void jetline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void jetclear		(PLStream *);
void jetpage		(PLStream *);
void jetadv		(PLStream *);
void jettidy		(PLStream *);
void jetcolor		(PLStream *);
void jettext		(PLStream *);
void jetgraph		(PLStream *);
void jetwidth		(PLStream *);
void jetesc		(PLStream *, PLINT, char *);

void psinit		(PLStream *);
void psline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void psclear		(PLStream *);
void pspage		(PLStream *);
void psadv		(PLStream *);
void pstidy		(PLStream *);
void pscolor		(PLStream *);
void pstext		(PLStream *);
void psgraph		(PLStream *);
void pswidth		(PLStream *);
void psesc		(PLStream *, PLINT, char *);

void nxinit             (PLStream *);
void nxline             (PLStream *, PLINT, PLINT, PLINT, PLINT);
void nxclear            (PLStream *);
void nxpage             (PLStream *);
void nxadv              (PLStream *);
void nxtidy             (PLStream *);
void nxcolor            (PLStream *);
void nxtext             (PLStream *);
void nxgraph            (PLStream *);
void nxwidth            (PLStream *);
void nxesc              (PLStream *, PLINT, char *);

void plmfile		(PLStream *);
void plmfnam		(PLStream *);
void plminit		(PLStream *);
void plmline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void plmclear		(PLStream *);
void plmpage		(PLStream *);
void plmadv		(PLStream *);
void plmtidy		(PLStream *);
void plmcolor		(PLStream *);
void plmtext		(PLStream *);
void plmgraph		(PLStream *);
void plmwidth		(PLStream *);
void plmesc		(PLStream *, PLINT, char *);

void vgainit		(PLStream *);
void vgaline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void vgaclear		(PLStream *);
void vgapage		(PLStream *);
void vgaadv		(PLStream *);
void vgatidy		(PLStream *);
void vgacolor		(PLStream *);
void vgatext		(PLStream *);
void vgagraph		(PLStream *);
void vgawidth		(PLStream *);
void vgaesc		(PLStream *, PLINT, char *);

void svgainit		(PLStream *);
void svgaline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void svgaclear		(PLStream *);
void svgapage		(PLStream *);
void svgaadv		(PLStream *);
void svgatidy		(PLStream *);
void svgacolor		(PLStream *);
void svgatext		(PLStream *);
void svgagraph		(PLStream *);
void svgawidth		(PLStream *);
void svgaesc		(PLStream *, PLINT, char *);

void os2init		(PLStream *);				 
void os2line		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void os2clear		(PLStream *);				 
void os2page		(PLStream *);				 
void os2adv		(PLStream *);				 
void os2tidy		(PLStream *);				 
void os2color		(PLStream *);				 
void os2text		(PLStream *);				 
void os2graph		(PLStream *);				 
void os2width		(PLStream *);				 
void os2esc		(PLStream *, PLINT, char *);		 

void amiwninit		(PLStream *);
void amiwnline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void amiwnclear		(PLStream *);
void amiwnpage		(PLStream *);
void amiwnadv		(PLStream *);
void amiwntidy		(PLStream *);
void amiwncolor		(PLStream *);
void amiwntext		(PLStream *);
void amiwngraph		(PLStream *);
void amiwnwidth		(PLStream *);
void amiwnesc		(PLStream *, PLINT, char *);

void amiprinit		(PLStream *);
void amiprline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void amiprclear		(PLStream *);
void amiprpage		(PLStream *);
void amipradv		(PLStream *);
void amiprtidy		(PLStream *);
void amiprcolor		(PLStream *);
void amiprtext		(PLStream *);
void amiprgraph		(PLStream *);
void amiprwidth		(PLStream *);
void amipresc		(PLStream *, PLINT, char *);

void iffinit		(PLStream *);
void iffline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void iffclear		(PLStream *);
void iffpage		(PLStream *);
void iffadv		(PLStream *);
void ifftidy		(PLStream *);
void iffcolor		(PLStream *);
void ifftext		(PLStream *);
void iffgraph		(PLStream *);
void iffwidth		(PLStream *);
void iffesc		(PLStream *, PLINT, char *);

void aegisinit		(PLStream *);
void aegisline		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void aegisclear		(PLStream *);
void aegispage		(PLStream *);
void aegisadv		(PLStream *);
void aegistidy		(PLStream *);
void aegiscolor		(PLStream *);
void aegistext		(PLStream *);
void aegisgraph		(PLStream *);
void aegiswidth		(PLStream *);
void aegisesc		(PLStream *, PLINT, char *);

void null_init		(PLStream *);
void null_line		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void null_clear		(PLStream *);
void null_page		(PLStream *);
void null_adv		(PLStream *);
void null_tidy		(PLStream *);
void null_color		(PLStream *);
void null_text		(PLStream *);
void null_graph		(PLStream *);
void null_width		(PLStream *);
void null_esc		(PLStream *, PLINT, char *);

/*----------------------------------------------------------------------*\
* Prototypes for plot buffer calls.
\*----------------------------------------------------------------------*/

void plbuf_init		(PLStream *);
void plbuf_line		(PLStream *, PLINT, PLINT, PLINT, PLINT);
void plbuf_clear	(PLStream *);
void plbuf_page		(PLStream *);
void plbuf_adv		(PLStream *);
void plbuf_tidy		(PLStream *);
void plbuf_color	(PLStream *);
void plbuf_text		(PLStream *);
void plbuf_graph	(PLStream *);
void plbuf_width	(PLStream *);
void plbuf_esc		(PLStream *, PLINT, char *);

void plRemakePlot	(PLStream *);

/*----------------------------------------------------------------------*\
* Prototypes for driver utility functions.
\*----------------------------------------------------------------------*/

void  plOpenFile	(PLStream *);
void  plFamInit		(PLStream *);
PLINT plGetInt		(char *);
PLFLT plGetFlt		(char *);
void  plGetFam		(PLStream *);
void  plSclPhy		(PLStream *, PLDev *, 
				int *, int *, int *, int *);
void  plRotPhy		(PLStream *, PLDev *, 
				int *, int *, int *, int *);

