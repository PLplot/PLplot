/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:33:38  furnish
   Initial checkin of the whole PLPLOT project.

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
* nsub...	Number of subpages on physical device
* cursub	Current subpage
* color		Current default color
* width		Current pen width
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
    int   ipls, ipld;
    FILE *OutFile;
    PLINT orientset, fileset, pageset;
    char FamilyName[80], FileName[90];
    PLINT bytecnt;
    PLINT page;
    PLINT linepos;
    PLINT level;
    PLINT device, termin, graphx;
    PLINT nopause;
    PLINT family, member, bytemax;
    PLFLT sclx, scly;
    PLINT nsubx, nsuby, cursub;
    PLINT color, width, style;
    PLINT orient;
    PLFLT xdpi, ydpi;
    PLINT xlength, ylength;
    PLINT xoffset, yoffset;
    PLINT xdigmax, ydigmax, zdigmax;
    PLINT xdigits, ydigits, zdigits;
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
   void (*pl_init)	PLARGS((PLStream *));
   void (*pl_line)	PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
   void (*pl_clear)	PLARGS((PLStream *));
   void (*pl_page)	PLARGS((PLStream *));
   void (*pl_adv)	PLARGS((PLStream *));
   void (*pl_tidy)	PLARGS((PLStream *));
   void (*pl_color)	PLARGS((PLStream *));
   void (*pl_text)	PLARGS((PLStream *));
   void (*pl_graph)	PLARGS((PLStream *));
   void (*pl_width)	PLARGS((PLStream *));
   void (*pl_esc)	PLARGS((PLStream *, PLINT, char *));
};

typedef struct dispatch_table  DISPATCH_TABLE;

/*----------------------------------------------------------------------*\
* Prototypes for all driver functions.
\*----------------------------------------------------------------------*/

void xteinit		PLARGS((PLStream *));
void xteline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void xteclear		PLARGS((PLStream *));
void xtepage		PLARGS((PLStream *));
void xteadv		PLARGS((PLStream *));
void xtetidy		PLARGS((PLStream *));
void xtecolor		PLARGS((PLStream *));
void xtetext		PLARGS((PLStream *));
void xtegraph		PLARGS((PLStream *));
void xtewidth		PLARGS((PLStream *));
void xteesc		PLARGS((PLStream *, PLINT, char *));

void xw_init		PLARGS((PLStream *));
void xw_line		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void xw_clear		PLARGS((PLStream *));
void xw_page		PLARGS((PLStream *));
void xw_adv		PLARGS((PLStream *));
void xw_tidy		PLARGS((PLStream *));
void xw_color		PLARGS((PLStream *));
void xw_text		PLARGS((PLStream *));
void xw_graph		PLARGS((PLStream *));
void xw_width		PLARGS((PLStream *));
void xw_esc		PLARGS((PLStream *, PLINT, char *));

void tektinit		PLARGS((PLStream *));
void tekfinit		PLARGS((PLStream *));
void tekline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void tekclear		PLARGS((PLStream *));
void tekpage		PLARGS((PLStream *));
void tekadv		PLARGS((PLStream *));
void tektidy		PLARGS((PLStream *));
void tekcolor		PLARGS((PLStream *));
void tektext		PLARGS((PLStream *));
void tekgraph		PLARGS((PLStream *));
void tekwidth		PLARGS((PLStream *));
void tekesc		PLARGS((PLStream *, PLINT, char *));

void dginit		PLARGS((PLStream *));
void dgline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void dgclear		PLARGS((PLStream *));
void dgpage		PLARGS((PLStream *));
void dgadv		PLARGS((PLStream *));
void dgtidy		PLARGS((PLStream *));
void dgcolor		PLARGS((PLStream *));
void dgtext		PLARGS((PLStream *));
void dggraph		PLARGS((PLStream *));
void dgwidth		PLARGS((PLStream *));
void dgesc		PLARGS((PLStream *, PLINT, char *));

void hp7470init		PLARGS((PLStream *));
void hp7470line		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void hp7470clear	PLARGS((PLStream *));
void hp7470page		PLARGS((PLStream *));
void hp7470adv		PLARGS((PLStream *));
void hp7470tidy		PLARGS((PLStream *));
void hp7470color	PLARGS((PLStream *));
void hp7470text		PLARGS((PLStream *));
void hp7470graph	PLARGS((PLStream *));
void hp7470width	PLARGS((PLStream *));
void hp7470esc		PLARGS((PLStream *, PLINT, char *));

void hp7580init		PLARGS((PLStream *));
void hp7580line		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void hp7580clear	PLARGS((PLStream *));
void hp7580page		PLARGS((PLStream *));
void hp7580adv		PLARGS((PLStream *));
void hp7580tidy		PLARGS((PLStream *));
void hp7580color	PLARGS((PLStream *));
void hp7580text		PLARGS((PLStream *));
void hp7580graph	PLARGS((PLStream *));
void hp7580width	PLARGS((PLStream *));
void hp7580esc		PLARGS((PLStream *, PLINT, char *));

void impinit		PLARGS((PLStream *));
void impline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void impclear		PLARGS((PLStream *));
void imppage		PLARGS((PLStream *));
void impadv		PLARGS((PLStream *));
void imptidy		PLARGS((PLStream *));
void impcolor		PLARGS((PLStream *));
void imptext		PLARGS((PLStream *));
void impgraph		PLARGS((PLStream *));
void impwidth		PLARGS((PLStream *));
void impesc		PLARGS((PLStream *, PLINT, char *));

void xfiginit		PLARGS((PLStream *));
void xfigline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void xfigclear		PLARGS((PLStream *));
void xfigpage		PLARGS((PLStream *));
void xfigadv		PLARGS((PLStream *));
void xfigtidy		PLARGS((PLStream *));
void xfigcolor		PLARGS((PLStream *));
void xfigtext		PLARGS((PLStream *));
void xfiggraph		PLARGS((PLStream *));
void xfigwidth		PLARGS((PLStream *));
void xfigesc		PLARGS((PLStream *, PLINT, char *));

void jetinit		PLARGS((PLStream *));
void jetline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void jetclear		PLARGS((PLStream *));
void jetpage		PLARGS((PLStream *));
void jetadv		PLARGS((PLStream *));
void jettidy		PLARGS((PLStream *));
void jetcolor		PLARGS((PLStream *));
void jettext		PLARGS((PLStream *));
void jetgraph		PLARGS((PLStream *));
void jetwidth		PLARGS((PLStream *));
void jetesc		PLARGS((PLStream *, PLINT, char *));

void psinit		PLARGS((PLStream *));
void psline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void psclear		PLARGS((PLStream *));
void pspage		PLARGS((PLStream *));
void psadv		PLARGS((PLStream *));
void pstidy		PLARGS((PLStream *));
void pscolor		PLARGS((PLStream *));
void pstext		PLARGS((PLStream *));
void psgraph		PLARGS((PLStream *));
void pswidth		PLARGS((PLStream *));
void psesc		PLARGS((PLStream *, PLINT, char *));

void nxinit             PLARGS((PLStream *));
void nxline             PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void nxclear            PLARGS((PLStream *));
void nxpage             PLARGS((PLStream *));
void nxadv              PLARGS((PLStream *));
void nxtidy             PLARGS((PLStream *));
void nxcolor            PLARGS((PLStream *));
void nxtext             PLARGS((PLStream *));
void nxgraph            PLARGS((PLStream *));
void nxwidth            PLARGS((PLStream *));
void nxesc              PLARGS((PLStream *, PLINT, char *));

void plmfile		PLARGS((PLStream *));
void plmfnam		PLARGS((PLStream *));
void plminit		PLARGS((PLStream *));
void plmline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void plmclear		PLARGS((PLStream *));
void plmpage		PLARGS((PLStream *));
void plmadv		PLARGS((PLStream *));
void plmtidy		PLARGS((PLStream *));
void plmcolor		PLARGS((PLStream *));
void plmtext		PLARGS((PLStream *));
void plmgraph		PLARGS((PLStream *));
void plmwidth		PLARGS((PLStream *));
void plmesc		PLARGS((PLStream *, PLINT, char *));

void vgainit		PLARGS((PLStream *));
void vgaline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void vgaclear		PLARGS((PLStream *));
void vgapage		PLARGS((PLStream *));
void vgaadv		PLARGS((PLStream *));
void vgatidy		PLARGS((PLStream *));
void vgacolor		PLARGS((PLStream *));
void vgatext		PLARGS((PLStream *));
void vgagraph		PLARGS((PLStream *));
void vgawidth		PLARGS((PLStream *));
void vgaesc		PLARGS((PLStream *, PLINT, char *));

void svgainit		PLARGS((PLStream *));
void svgaline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void svgaclear		PLARGS((PLStream *));
void svgapage		PLARGS((PLStream *));
void svgaadv		PLARGS((PLStream *));
void svgatidy		PLARGS((PLStream *));
void svgacolor		PLARGS((PLStream *));
void svgatext		PLARGS((PLStream *));
void svgagraph		PLARGS((PLStream *));
void svgawidth		PLARGS((PLStream *));
void svgaesc		PLARGS((PLStream *, PLINT, char *));

void os2init		PLARGS((PLStream *));				 
void os2line		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void os2clear		PLARGS((PLStream *));				 
void os2page		PLARGS((PLStream *));				 
void os2adv		PLARGS((PLStream *));				 
void os2tidy		PLARGS((PLStream *));				 
void os2color		PLARGS((PLStream *));				 
void os2text		PLARGS((PLStream *));				 
void os2graph		PLARGS((PLStream *));				 
void os2width		PLARGS((PLStream *));				 
void os2esc		PLARGS((PLStream *, PLINT, char *));		 

void amiwninit		PLARGS((PLStream *));
void amiwnline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void amiwnclear		PLARGS((PLStream *));
void amiwnpage		PLARGS((PLStream *));
void amiwnadv		PLARGS((PLStream *));
void amiwntidy		PLARGS((PLStream *));
void amiwncolor		PLARGS((PLStream *));
void amiwntext		PLARGS((PLStream *));
void amiwngraph		PLARGS((PLStream *));
void amiwnwidth		PLARGS((PLStream *));
void amiwnesc		PLARGS((PLStream *, PLINT, char *));

void amiprinit		PLARGS((PLStream *));
void amiprline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void amiprclear		PLARGS((PLStream *));
void amiprpage		PLARGS((PLStream *));
void amipradv		PLARGS((PLStream *));
void amiprtidy		PLARGS((PLStream *));
void amiprcolor		PLARGS((PLStream *));
void amiprtext		PLARGS((PLStream *));
void amiprgraph		PLARGS((PLStream *));
void amiprwidth		PLARGS((PLStream *));
void amipresc		PLARGS((PLStream *, PLINT, char *));

void iffinit		PLARGS((PLStream *));
void iffline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void iffclear		PLARGS((PLStream *));
void iffpage		PLARGS((PLStream *));
void iffadv		PLARGS((PLStream *));
void ifftidy		PLARGS((PLStream *));
void iffcolor		PLARGS((PLStream *));
void ifftext		PLARGS((PLStream *));
void iffgraph		PLARGS((PLStream *));
void iffwidth		PLARGS((PLStream *));
void iffesc		PLARGS((PLStream *, PLINT, char *));

void aegisinit		PLARGS((PLStream *));
void aegisline		PLARGS((PLStream *, PLINT, PLINT, PLINT, PLINT));
void aegisclear		PLARGS((PLStream *));
void aegispage		PLARGS((PLStream *));
void aegisadv		PLARGS((PLStream *));
void aegistidy		PLARGS((PLStream *));
void aegiscolor		PLARGS((PLStream *));
void aegistext		PLARGS((PLStream *));
void aegisgraph		PLARGS((PLStream *));
void aegiswidth		PLARGS((PLStream *));
void aegisesc		PLARGS((PLStream *, PLINT, char *));

/*----------------------------------------------------------------------*\
* Prototypes for driver utility functions.
\*----------------------------------------------------------------------*/

void  plOpenFile	PLARGS((PLStream *));
void  plFamInit		PLARGS((PLStream *));
PLINT plGetInt		PLARGS((char *));
PLFLT plGetFlt		PLARGS((char *));
void  plGetFam		PLARGS((PLStream *));
void  plSclPhy		PLARGS((PLStream *, PLDev *, \
				int *, int *, int *, int *));
void  plRotPhy		PLARGS((PLStream *, PLDev *, \
				int *, int *, int *, int *));
