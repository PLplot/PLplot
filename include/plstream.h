/* $Id$
 * $Log$
 * Revision 1.36  1995/01/10 09:37:39  mjl
 * Fixed some braindamage incurred last update.
 *
 * Revision 1.34  1995/01/06  20:47:29  mjl
 * Added "freeaspect" stream variable: if set, aspect ratio is not preserved
 * on orientation swaps.
 *
 * Revision 1.33  1995/01/06  07:48:29  mjl
 * Made window coordinate structure a stream variable as it should be.
 *
 * Revision 1.32  1994/08/25  04:03:43  mjl
 * Added include of pdf.h, since the PLStream data structure requires it
 * in any case.
 *
 * Revision 1.31  1994/07/23  04:46:55  mjl
 * Added pls->server_nokill, which can be set to prevent plserver from dying
 * when a ^C is hit.
 *
 * Revision 1.30  1994/07/19  22:34:46  mjl
 * Eliminated some unnecessary defines.
 *
 * Revision 1.29  1994/06/30  18:37:03  mjl
 * Added status flag to keep track of current page status.
 *
 * Revision 1.28  1994/05/07  03:22:08  mjl
 * Eliminated bgcolor, fgcolor, and bgcolorset.  Now fgcolor is a figment of
 * the X driver's imagination :-).  bgcolor is identically the same as the
 * 0th entry of cmap0.  About time I introduced some sanity to these.
 *
 * Revision 1.27  1994/04/25  19:05:33  mjl
 * Additional support for cmap1 palette.
 *
 * Revision 1.26  1994/04/08  12:20:49  mjl
 * Now includes <plplot.h> in case it hasn't been included yet.  File offset
 * for metafile removed from state structure (now local to driver).  Added:
 * * MouseEH	  void*	Mouse event handler
 * * MouseEH_data void*	Pointer to client data to pass
 * * tidy         void*   pointer to cleanup routine
 * * tidy_data    void*   pointer to client data to pass
 * ("tidy" is a user customization tidy routine, to be called before
 * closing a stream to do any program specific cleanup.)
 *
 * Revision 1.25  1994/03/23  07:10:51  mjl
 * Added documentation for all variables not described yet.  Changed some
 * variables used in color map selection.  Added variables:
 *
 *  dev_minor	Minor device id (for variations on one type)
 *  dev_fill0	Set if driver can do solid area fills
 *  dev_fill1	Set if driver can do pattern area fills
 *  finc	Number to increment between member files
 *  fflen	Minimum field length to use in member file number
 *
 * Revision 1.24  1994/02/01  22:46:55  mjl
 * Added pls->user (char *).
 *
 * Revision 1.23  1994/01/25  06:22:17  mjl
 * Added pls->db variable.
 *
 * Revision 1.22  1994/01/15  17:35:09  mjl
 * Added new variables for use in Tcl/TK/DP driver: server_name, server_host,
 * server_port.
 *
 * Revision 1.21  1993/12/21  10:35:46  mjl
 * Added flag to be set by driver to indicate whether TK or DP style
 * communication is being used.
*/

/*	plstream.h

	Contains declarations for PLStream and PLDev structs.
	Also prototypes for stream & device utility functions.
*/

#include "pdf.h"

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
 ***********************************************************************
 *
 * Misc control variables
 *
 * ipls		PLINT	Stream number
 * level	PLINT	Initialization level
 * program	char*	Program name
 *
 ***********************************************************************
 *
 * Palettes (two of them)
 *
 * Color map 0 is intended for static objects, such as boxes, lines, points,
 * labels, etc.  These should be allocated before calling plinit (else you
 * get all 16 by default, which can be undesirable on some platforms).
 * These are then explicitly selected by number (in order of allocation).
 * The lowest number is 0, but this is used for the background color,
 * so all color drivers start with 1 as the default color.  
 *
 * Color map 1 is for continuous-tone plots, where color is used to
 * represent function value or intensity.  These are set in a relative way
 * only, for increased portability and flexibility.  The actual number of
 * colors used to represent intensity levels is determined by the driver.
 * Note that it is only necessary to specify intensity (on a scale from 0
 * to 1) to get an appropriate color.  Drivers incapable of fine shading
 * will do the best job they can.
 *
 * A palette selection tool for both palettes is available for the Tk
 * driver.  Direct writing of RGB values (i.e.  banging on the hardware) is
 * supported but highly discouraged (colors so written will be affected
 * unpredictably by the palette tools).
 *
 * icol0	PLINT	Color map 0 entry, current color (0 <= icol0 <= 15)
 * ncol0	PLINT	Number of colors allocated in color map 0.
 * icol1	PLINT	Color map 1 entry, current color (0 <= icol1 <= 255)
 * ncol1	PLINT	Number of colors allocated in color map 1.
 * ncol1cp	PLINT	Number of control points in cmap1 allocation (max 32)
 * lcol1cp	PLFLT	Locations of control points in cmap1 [0,1]
 * curcmap	PLINT	Current color map
 * curcolor	RGB[]	Current color
 * cmap0 	RGB[]	Color map 0: maximum of 16 RGB 8-bit values
 * cmap1 	RGB[]	Color map 1: maximum of 256 RGB 8-bit values
 *
 * cmap0setcol	int[]	Set for initialized cmap0 colors.
 * cmap1set	int	Set if cmap 1 has been initialized
 *
 ***********************************************************************
 *
 * Variables governing pen width 
 *
 * width	Current pen width
 * widthset	Set if pen width was specified
 * widthlock	Set if pen width is locked
 *
 ***********************************************************************
 *
 * Variables used to pass information between the core and the driver 
 *
 * It would be nice to use the "dev_" prefix uniformly but changing
 * all that old code would be quite a lot of work..
 *
 * device	PLINT	Graphics device id number
 * dev_minor	PLINT	Minor device id (for variations on one type)
 * color	PLINT	Set if color is available
 * colorset	PLINT	Set if "color" was set prior to calling plinit
 * plbuf_write	PLINT	Set if driver needs to use the plot buffer
 * dev_fill0	PLINT	Set if driver can do solid area fills
 * dev_fill1	PLINT	Set if driver can do pattern area fills
 * dev_di	PLINT	Set if driver wants to handle DI commands
 * dev_flush	PLINT	Set if driver wants to handle flushes itself
 * termin	PLINT	Set for interactive devices
 * graphx	PLINT	Set if currently in graphics mode
 * nopause	PLINT	Set if we are skipping the pause between frames
 * family	PLINT	Set if familying is enabled
 * member	PLINT	Number of current family member file open
 * finc		PLINT	Number to increment between member files
 * fflen	PLINT	Minimum field length to use in member file number
 * bytemax	PLINT	Number of bytes maximum per member file
 * famadv	PLINT	Set to advance to the next family member
 * DevName	char*	Device name
 * OutFile	FILE	Output file pointer
 * BaseName	char*	Output base name (i.e. family)
 * FileName	char*	Output file name
 * output_type	int	0 for file, 1 for stream
 * bytecnt	PLINT	Byte count for output stream
 * page		PLINT	Page count for output stream
 * linepos	PLINT	Line count for output stream
 * pdfs		PDFstrm* PDF stream pointer
 *
 * These are used by the escape function (for area fill, etc).
 *
 * dev_npts	PLINT	Number of points we are plotting
 * dev_x	short*	Pointer to array of x values 
 * dev_y	short*	Pointer to array of x values 
 *
 * The following pointer is for drivers that require device-specific 
 * data.  At initialization the driver should malloc the necessary
 * space and set pls->dev to point to this area.  This way there can
 * be multiple streams using the same driver without conflict.
 *
 * dev		void*	pointer to device-specific data (malloc'ed)
 *
 * User-supplied event handlers for use by interactive drivers (e.g. X).
 * Can be used to take various actions depending on input.  Currently
 * only a keyboard event handler is supported.
 *
 * KeyEH	void*	Keyboard event handler
 * KeyEH_data	void*	Pointer to client data to pass
 *
 * MouseEH	void*	Mouse event handler
 * MouseEH_data	void*	Pointer to client data to pass
 *
 * Variables used for direct specification of device characteristics
 * Not supported by all drivers (or even very many)
 *
 * xdpi..	PLFLT	Device DPI settings in x and y
 * xlength..	PLINT	Device output lengths in x and y
 * xoffset..	PLINT	Device offsets from upper left hand corner
 * pageset	PLINT	Set if page dimensions were specified
 * hack		PLINT	Enables driver-specific hack(s) if set
 *
 ***********************************************************************
 *
 * User customization tidy routine.  This is called before closing a stream
 * to do any program specific cleanup.
 *
 * tidy		void*   pointer to cleanup routine
 * tidy_data	void*   pointer to client data to pass
 *
 ************************************************************************
 *
 * Stuff used by Xlib driver
 *
 * geometry	char*	Window geometry (malloc'ed)
 * window_id	long	X-window window ID
 * nopixmap	int	Set if you want to forbid allocation of pixmaps
 * db		int	Set if you want to double buffer output
 *			(only pixmap is drawn to directly; it is blitted
 *			to output window on EOP or an Expose)
 ***********************************************************************
 *
 * These are for support of the TK driver.
 *
 * server_name	char*	Main window name of server
 * server_host	char*	Name of host to run server on
 * server_port	char*	Port to talk to server on
 * user		char*	Your user name on remote host (for remsh command)
 * plserver	char*	Name of server
 * plwindow	char*	Name of reference server window (malloc'ed)
 * tcl_cmd	char*	TCL command(s) to eval on startup
 * auto_path	char*	Additional directories to autoload
 * bufmax	int	Number of bytes sent before output buffer is flushed
 * dp		int	Use Tcl-DP for communication, if set
 * server_nokill int	Don't kill plserver on a ^C if set
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
 * aspdev	PLFLT	Original device aspect ratio
 * aspect	PLFLT	Page aspect ratio
 * aspori	PLFLT	Rotation-induced aspect ratio
 * freeaspect	PLINT	Do not preserve aspect ratio on orientation swaps
 * mar		PLFLT	Page margin (minimum)
 * jx		PLFLT	Page justification in x
 * jy		PLFLT	Page justification in y
 *
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
 * dimxmin	PLFLT	
 * dimymin	PLFLT	Target coordinate system parameters.
 * dimxmax	PLFLT
 * dimymax	PLFLT	
 * dimxpmm	PLFLT
 * dimypmm	PLFLT
 * dimxax 	PLFLT	Map meta to physical coordinates:
 * dimxb 	PLFLT	  x' = dimxax * x + dimxb
 * dimyay 	PLFLT
 * dimyb 	PLFLT	  y' = dimyay * y + dimyb
 *
 * page_status	PLINT	Flag to indicate current action
 *
 ***********************************************************************
 *
 * Fill pattern state information. 
 * patt < 0: Hardware fill, if available (not implemented yet)
 * patt ==0: Hardware fill, if available, solid
 * patt > 0: Software fill
 *
 * patt		Fill pattern number
 * inclin	Array of inclinations in tenths of degree for fill lines
 * delta	Array of spacings in micrometers between fill lines
 * nps		Number of distinct line styles for fills
 *
 ***********************************************************************
 *
 * Variables used in line drawing
 *
 * currx	Physical x-coordinate of current point
 * curry	Physical y-coordinate of current point
 * mark		Array of mark lengths in micrometers for broken lines
 * space	Array of space lengths in micrometers for broken lines
 * nms		Number of elements for current broken line style
 * timecnt	Timer for broken lines
 * alarm	Alarm indicating change of broken line status
 * pendn	Flag indicating if pen is up or down
 * curel	Current element within broken line
 *
 ***********************************************************************
 *
 * Variables governing character strings
 *
 * esc		Text string escape character
 *
 ***********************************************************************
 *
 * Scale factors for characters, symbols, and tick marks.
 *
 * scale	Scaling factor for chr, sym, maj, min.
 * chr...	Character default height and current (scaled) height
 * sym...	Symbol    default height and current (scaled) height
 * maj...	Major tick default height and current (scaled) height
 * min...	Minor tick default height and current (scaled) height
 *
 ***********************************************************************
 *
 * Variables governing numeric axis label appearance
 *
 * setpre	Non-zero to set precision using "prec"
 * precis	User-specified precision
 * xdigmax..	Allowed #digits in axes labels
 * xdigits..	Actual field widths (returned)
 *
 ***********************************************************************
 *
 * Variables governing physical coordinate system
 *
 * vpp..	Viewport boundaries in physical coordinates
 * spp..	Subpage  boundaries in physical coordinates
 * clp..	Clip     boundaries in physical coordinates
 * phy...	Physical device limits in physical coordinates
 * um.		Number of micrometers in a pixel
 * pmm		Number of pixels to a millimeter
 *
 ***********************************************************************
 *
 * State variables for 3d plots
 *
 * base3.	World coordinate size of base for 3-d plot
 * basec.	Position of centre of base for 3-d plot
 * dom...	Minimum and maximum values for domain
 * zzscl	Vertical (z) scale for 3-d plot
 * ran..	Minimum and maximum z values for 3-d plot
 * c..		Coordinate transformation from 3-d to 2-d
 *
 ***********************************************************************
 *
 * Variables for keeping track of world coordinate windows on a page.
 *
 * nCWindows	Number of coordinate windows on current page
 * windows	Array of plCWindow's for current page
 *
 ***********************************************************************
 *
 * Variables governing subpages and viewports.
 *
 * nsub...	Number of subpages on physical device
 * cursub	Current subpage
 * spd...	Subpage  boundaries in normalized device coordinates
 * vpd...	Viewport boundaries in normalized device coordinates
 * vpw...	Viewport boundaries in world coordinates
 *
 ***********************************************************************
 *
 * Transformation variables
 *
 * wp....	Transformation variables for world  to physical conversion
 * dp....	Transformation variables for device to physical conversion
 * mp....	Transformation variables for millimeters from bottom left
 *		hand corner to physical coordinates
 * wm....	Transformation variables for world coordinates to mm
 *
\*----------------------------------------------------------------------*/

typedef struct {

/* Misc control information */

    PLINT ipls;
    PLINT level;
    char *program;

/* Colormaps */

    PLINT icol0, ncol0, icol1, ncol1, ncp1, curcmap;
    int   cmap0setcol[16], cmap1set;

    PLColor curcolor;
    PLColor cmap0[16];
    PLColor cmap1[256];

    PLControlPt cmap1cp[32];

/* Variables governing pen width */

    PLINT width;
    PLINT widthset, widthlock;

/* Variables used for interacting with or by device driver */

    PLINT plbuf_write;
    PLINT device, dev_minor, termin, graphx, nopause;
    PLINT color, colorset;
    PLINT family, member, finc, fflen, bytemax, famadv;
    PLINT dev_fill0, dev_fill1, dev_di, dev_flush;

    char DevName[80];
    FILE *OutFile;
    char *BaseName, *FileName;
    int  output_type;
    PLINT bytecnt, page, linepos;
    PDFstrm *pdfs;

    PLINT dev_npts;
    short *dev_x, *dev_y;

    void *dev;

    void (*KeyEH)	(PLKey *, void *, int *);
    void *KeyEH_data;

    void (*MouseEH)	(PLMouse *, void *, int *);
    void *MouseEH_data;

    PLFLT xdpi, ydpi;
    PLINT xlength, ylength;
    PLINT xoffset, yoffset;
    PLINT pageset, hack;

/* Per stream tidy function. */

    void (*tidy)    (void *);
    void *tidy_data;

/* Stuff used by Xlib driver */

    char *geometry;
    long window_id;
    int  nopixmap, db;

/* Stuff used by TK, DP drivers */

    char *server_name, *server_host, *server_port, *user;
    char *plserver, *plwindow;
    char *tcl_cmd, *auto_path;
    int  bufmax, dp, server_nokill;

/* Plot buffer settings */

    FILE *plbufFile;
    int  plbufOwner;

/* Driver interface (DI) */

    PLINT difilt, diclpxmi, diclpxma, diclpymi, diclpyma;
    PLFLT dipxmin, dipymin, dipxmax, dipymax;
    PLFLT dipxax, dipxb, dipyay, dipyb;
    PLFLT aspdev, aspect, aspori, mar, jx, jy;
    PLFLT didxax, didxb, didyay, didyb;
    PLFLT diorot;
    PLFLT dioxax, dioxay, dioxb, dioyax, dioyay, dioyb;
    PLFLT dimxax, dimxb, dimyay, dimyb;
    PLFLT dimxmin, dimymin, dimxmax, dimymax, dimxpmm, dimypmm;
    PLINT page_status, freeaspect;

/* Fill pattern info */

    PLINT patt, inclin[2], delta[2], nps;

/* Variables used in line drawing */

    PLINT currx, curry;
    PLINT mark[10], space[10], nms;
    PLINT timecnt, alarm, pendn, curel;

/* Variables governing character strings */

    char  esc;

/* Scale factors for characters, symbols, and tick marks. */

    PLFLT scale;
    PLFLT chrdef, chrht;
    PLFLT symdef, symht;
    PLFLT majdef, majht;
    PLFLT mindef, minht;

/* Variables governing numeric axis label appearance */

    PLINT setpre, precis;
    PLINT xdigmax, ydigmax, zdigmax;
    PLINT xdigits, ydigits, zdigits;

/* Variables governing physical coordinate system */

    PLINT vppxmi, vppxma, vppymi, vppyma;
    PLINT sppxmi, sppxma, sppymi, sppyma;
    PLINT clpxmi, clpxma, clpymi, clpyma;
    PLINT phyxmi, phyxma, phyymi, phyyma;
    PLINT umx, umy;
    PLFLT xpmm, ypmm;

/* State variables for 3d plots */

    PLFLT base3x, base3y, basecx, basecy;
    PLFLT domxmi, domxma, domymi, domyma;
    PLFLT zzscl, ranmi, ranma;
    PLFLT cxx, cxy, cyx, cyy, cyz;

/* Variables for keeping track of world coordinate windows on a page. */

    int nCWindows;
    plCWindow windows[PL_MAXWINDOWS];

/* Variables governing subpages and viewports. */

    PLINT nsubx, nsuby, cursub;
    PLFLT spdxmi, spdxma, spdymi, spdyma;
    PLFLT vpdxmi, vpdxma, vpdymi, vpdyma;
    PLFLT vpwxmi, vpwxma, vpwymi, vpwyma;

/* Transformation variables */

    PLFLT wpxscl, wpxoff, wpyscl, wpyoff;
    PLFLT dpxscl, dpxoff, dpyscl, dpyoff;
    PLFLT mpxscl, mpxoff, mpyscl, mpyoff;
    PLFLT wmxscl, wmxoff, wmyscl, wmyoff;

} PLStream;

/*----------------------------------------------------------------------*\
 * Prototypes for stream & device utility functions.
\*----------------------------------------------------------------------*/

/* Get the current stream pointer */

void
plgpls(PLStream **p_pls);

/* Initializes device cmap 1 entry by interpolation from pls->cmap1 entries */

void
plcol_interp(PLStream *pls, PLColor *newcolor, int i, int ncol);

/* Opens file for output, prompting if not set. */

void
plOpenFile(PLStream *pls);

/* Sets up next file member name (in pls->FileName), but does not open it. */

void
plP_getmember(PLStream *pls);

/* Sets up file name & family stem name. */

void
plP_sfnam(PLStream *pls, const char *fnam);

/* Initializes family file parameters. */

void
plFamInit(PLStream *pls);

/* Starts new member file of family file set if necessary. */

void
plGetFam(PLStream *pls);

/* Rotates physical coordinates if necessary for given orientation. */

void
plRotPhy(PLINT orient, PLINT xmin, PLINT ymin, PLINT xmax, PLINT ymax,
	 int *px, int *py);

/* Allocates a standard PLDev structure for device-specific data */

PLDev *
plAllocDev(PLStream *pls);

#endif	/* __PLSTREAM_H__ */
