/* $Id$
   $Log$
   Revision 1.12  1993/07/31 08:13:39  mjl
   Variables added: dev_di - the driver can handle driver interface commands;
   dev_fill - the driver can handle polygon fill commands (both to be
   implemented using the escape function).  Also other support variables added.

 * Revision 1.11  1993/07/28  05:51:12  mjl
 * Added stream variables nopixmap (tell driver not to use pixmaps) and
 * dual_screen (set on devices that have dual text/graphics screens).
 *
 * Revision 1.10  1993/07/16  22:30:10  mjl
 * Added many new variables used in driver interface to PLStream definition.
 * Eliminated some obsolete variables and function prototypes.
 *
 * Revision 1.9  1993/07/02  07:25:30  mjl
 * Added variables for dealing with X driver, TK driver, driver interface.
 *
 * Revision 1.8  1993/04/26  19:57:52  mjl
 * Fixes to allow (once again) output to stdout and plrender to function as
 * a filter.  A type flag was added to handle file vs stream differences.
*/

/*	plstream.h

	Contains declarations for PLStream and PLDev structs.
	Also prototypes for stream & device utility functions.
*/

#ifndef __PLSTREAM_H__
#define __PLSTREAM_H__

/*----------------------------------------------------------------------*\
* Define the PLDev data structure.
*
* These are all quantities that must be saved on a per-device basis.
* Some drivers (xwin, tk) allocate structures defined internally.
\*----------------------------------------------------------------------*/

typedef struct {
    PLFLT pxlx, pxly;
    PLINT xold, yold;

    PLINT xmin, xmax, xlen;
    PLINT ymin, ymax, ylen;

    PLINT xmin_dev, xmax_dev, xlen_dev;
    PLINT ymin_dev, ymax_dev, ylen_dev;

    PLFLT xscale_dev, yscale_dev;
} PLDev;

/*----------------------------------------------------------------------*\
* Define the PLStream data structure.
*
* This contains a copy of every variable that is stream dependent, which
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
* There are undoubtably some inconsistencies in the declaration & use of
* the variables below.  This is a result of the continuing evolution of
* plplot and the numerous authors involved.  Hopefully in time the function
* of all variables can be fully documented and made more consistent.
*
* The quantities in each stream are as follows:
*
* ipls		Stream number
* ipld		Device number
* DevName	Device name
*
* OutFile	Output file pointer
* FamilyName	Output family name (i.e. stem)
* FileName	Output file name
* output_type	0 for file, 1 for stream
*
* fileset	Set if file name or file pointer was specified
* pageset	Set if page dimensions were specified
* widthset	Set if pen width was specified
*
* bytecnt	Byte count for output stream
* page		Page count for output stream
* linepos	Line count for output stream
* lp_offset	Byte position for previous page (metafile output)
*
***********************************************************************
* PLPLOT uses two user-alterable palettes:
*
* Color map 0 is intended for static objects, such as boxes, lines, points,
* labels, etc.  These should be allocated before calling plinit (else you
* get all 16 by default, which can be undesirable on some platforms).
* These are then explicitly selected by number (in order of allocation).
* The lowest number is 0, but since this is used on some platforms to
* store the background color, all color drivers start with 1 as the
* default color.  The user is encouraged to do the same.
*
* Color map 1 is for height-field plots, where color is used to represent
* function height (intensity).  These are set in a relative way only, 
* for increased portability and flexibility.  The actual number of colors
* used to represent height levels is determined by the driver.  Note that
* it is only necessary to specify height (on a scale from 0 to 1) to get
* an appropriate color.  Drivers capable of finer shading can interpolate
* RGB values to precisely map height to color.
*
* Eventually a palette selection tool for both palettes will be provided
* for some drivers (e.g. tk).  Direct writing of RGB values (i.e.  banging
* on the hardware) is supported but highly discouraged (colors so written
* will be affected unpredictably by the palette tools).
*
* icol0		PLINT	Color map 0 entry, current color (0 <= icol0 <= 15)
* ncol0		PLINT	Number of colors allocated in color map 0.
* htlvl		PLINT	Color map 1 current height level (0.0 <= htlvl <= 1.0)
* bgcolor	RGB	Background color, if specified
* fgcolor	RGB	Foreground color, if specified
* curcolor	RGB[]	Current color, for convenience
* cmap0 	RGB[]	Color map 0: maximum of 16 RGB 8-bit values
* cmap1 	RGB[]	Color map 1: maximum of 256 RGB 8-bit values
*
* cmap0setcol	int[]	Set for every color allocated in cmap0.
* cmap1set	PLINT	Set if cmap1 allocated.
* bgcolorset	PLINT	Set if "bgcolor" was set
*
***********************************************************************
*
* The following pointer is for drivers that require device-specific 
* data.  At initialization the driver should malloc the necessary
* space and set pls->dev to point to this area.  This way there can
* be multiple streams using the same driver without conflict.
*
* dev		void*	pointer to device-specific data (malloc'ed)
*
***********************************************************************
*
* User-supplied event handlers for use by interactive drivers (e.g. X).
* Can be used to take various actions depending on input.  Currently
* only a keyboard event handler is supported.
*
* KeyEH		void*	Keyboard event handler
* KeyEH_data	void*	pointer to client data to pass
*
***********************************************************************
*
* Stuff used by Xlib driver
*
* geometry	char*	window geometry (malloc'ed)
* window_id	long	X-window window ID
* nopixmap	int	Set if you want to forbid allocation of pixmaps
*
***********************************************************************
*
* These are for support of the TK driver.
*
* plserver	char*	name of server
* plwindow	char*	name of reference server window (malloc'ed)
* tcl_cmd	char*	TCL command(s) to eval on startup
* auto_path	char*	Additional directories to autoload
* bufmax	int	number of bytes sent before output buffer is flushed
*
***********************************************************************
*
* Variables for use by the plot buffer
*
* plbufFile	FILE	Plot buffer file pointer
* plbufOwner	int	Typically set; only zero if current stream is cloned.
*
***********************************************************************
*
* Driver interface (DI)
*
* difilt	PLINT	Driver interface filter flag
*
* dipxmin	PLFLT	
* dipymin	PLFLT	Min, max relative plot coordinates
* dipxmax	PLFLT
* dipymax	PLFLT	
* dipxax 	PLFLT	Plot window transformation:
* dipxb 	PLFLT	  x' = dipxax * x + dipxb
* dipyay 	PLFLT
* dipyb 	PLFLT	  y' = dipyay * y + dipyb
*
* didxmin	PLFLT	
* didymin	PLFLT	Min, max relative device coordinates
* didxmax	PLFLT
* didymax	PLFLT	
* didxax 	PLFLT	Device window transformation:
* didxb 	PLFLT	  x' = didxax * x + didxb
* didyay 	PLFLT
* didyb 	PLFLT	  y' = didyay * y + didyb
*
* diclpxmi	PLINT
* diclpxma	PLINT	Device clip limits
* diclpymi	PLINT
* diclpyma	PLINT
*
* diorot	PLFLT	Rotation angle (in units of pi/2)
* dioxax	PLFLT	Orientation transformation:
* dioxay	PLFLT	  x' = dioxax * x + dioxay * y + dioxb
* dioxb 	PLFLT
* dioyax	PLFLT	  y' = dioyax * x + dioyay * y + dioyb
* dioyay	PLFLT
* dioyb 	PLFLT
*
* dimxax 	PLFLT	Map meta to physical coordinates:
* dimxb 	PLFLT	  x' = dimxax * x + dimxb
* dimyay 	PLFLT
* dimyb 	PLFLT	  y' = dimyay * y + dimyb
*
***********************************************************************
*
* Stuff used by the driver.
* It would be nice to use the "dev_" prefix uniformly but changing
* all that old code would be quite a lot of work..
*
* device	PLINT	Graphics device id number
* color		PLINT	Set if color is available
* colorset	PLINT	Set if "color" was set prior to calling plinit
* plbuf_write	PLINT	Set if driver needs to use the plot buffer
* dev_fill	PLINT	Set if driver can do area fills
* dev_di	PLINT	Set if driver wants to handle DI commands
* dev_dual	PLINT	Set if device supports dual text/graphics screens
* termin	PLINT	Set for interactive devices
* graphx	PLINT	Set if currently in graphics mode
* nopause	PLINT	Set if we are skipping the pause between frames
* family	PLINT	Set if familying is enabled
* member	PLINT	Number of current family member file open
* bytemax	PLINT	Number of bytes maximum per member file
* famadv	PLINT	Set to advance to the next family member
*
* These are used by the escape function in some circumstances.
*
* dev_npts	PLINT	Number of points we are plotting
* dev_x		short*	Pointer to array of x values 
* dev_y		short*	Pointer to array of x values 
*
***********************************************************************
*
* Everything else
*
* program	char*	Program name
*
* level		Initialization level
* nsub...	Number of subpages on physical device
* cursub	Current subpage
* width		Current pen width
* esc		Text string escape character
* scale		Scaling factor for chr, sym, maj, min.
* chr...	Character default height and current (scaled) height
* sym...	Symbol    default height and current (scaled) height
* maj...	Major tick default height and current (scaled) height
* min...	Minor tick default height and current (scaled) height
* setpre	Non-zero to set precision using "prec"
* precis	User-specified precision
* xdigmax..	Allowed #digits in axes labels
* xdigits..	Actual field widths (returned)
*
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
    char DevName[80];

    FILE *OutFile;
    char *FamilyName, *FileName;
    int  output_type;

    PLINT fileset;
    PLINT pageset;
    PLINT widthset;

    PLINT bytecnt;
    PLINT page;
    PLINT linepos;
    FPOS_T lp_offset;

/* Colormaps */

    PLINT icol0, ncol0, bgcolorset;
    int   cmap0setcol[16], cmap1set;
    PLFLT htlvl;

    PLColor fgcolor;
    PLColor bgcolor;
    PLColor curcolor;
    PLColor cmap0[16];
    PLColor cmap1[256];

/* Driver event handler */

    void (*KeyEH)	(PLKey *, void *, int *);
    void *KeyEH_data;

/* Stuff used by Xlib driver */

    char *geometry;
    long window_id;
    int  nopixmap;

/* Stuff used by TK driver */

    char *plserver;
    char *plwindow;
    char *tcl_cmd;
    char *auto_path;
    int  bufmax;

/* Driver-specific data */

    void *dev;

/* Plot buffer settings */

    FILE *plbufFile;
    int  plbufOwner;

/* Driver interface (DI) */

    PLINT difilt, diclpxmi, diclpxma, diclpymi, diclpyma;
    PLFLT dipxmin, dipymin, dipxmax, dipymax;
    PLFLT dipxax, dipxb, dipyay, dipyb;
    PLFLT didxmin, didymin, didxmax, didymax;
    PLFLT didxax, didxb, didyay, didyb;
    PLFLT diorot;
    PLFLT dioxax, dioxay, dioxb, dioyax, dioyay, dioyb;
    PLFLT dimxax, dimxb, dimyay, dimyb;

/* Driver settings */

    PLINT plbuf_write;
    PLINT color, colorset;
    PLINT device, termin, graphx, nopause;
    PLINT family, member, bytemax, famadv;
    PLINT dev_fill, dev_dual, dev_di;
    PLINT dev_npts;
    short *dev_x, *dev_y;

/* Everything else */
/* Could use some better grouping */

    char *program;

    PLINT level;
    PLFLT sclx, scly;
    PLINT nsubx, nsuby, cursub;
    PLINT width, style;
    char  esc;
    PLFLT xdpi, ydpi;
    PLINT xlength, ylength;
    PLINT xoffset, yoffset;
    PLINT setpre, precis;
    PLINT xdigmax, ydigmax, zdigmax;
    PLINT xdigits, ydigits, zdigits;
    PLFLT scale;
    PLFLT chrdef, chrht;
    PLFLT symdef, symht;
    PLFLT majdef, majht;
    PLFLT mindef, minht;
    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLINT sppxmi, sppxma, sppymi, sppyma;
    PLINT clpxmi, clpxma, clpymi, clpyma;
    PLINT phyxmi, phyxma, phyymi, phyyma;
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
* Prototypes for stream & device utility functions.
\*----------------------------------------------------------------------*/

void  plgpls		(PLStream **);
void  plCmaps_init	(PLStream *);
void  plOpenFile	(PLStream *);
void  plFamInit		(PLStream *);
PLINT plGetInt		(char *);
PLFLT plGetFlt		(char *);
void  plGetFam		(PLStream *);
void  plRotPhy		(PLINT, PLDev *, 
			 int *, int *, int *, int *);
void  plP_sfnam		(PLStream *, char *);
PLDev * plAllocDev	(PLStream *pls);

#endif	/* __PLSTREAM_H__ */
