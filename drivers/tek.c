/* $Id$
 * $Log$
 * Revision 1.31  1994/07/23 04:44:27  mjl
 * Added conditional compilation of atexit() code based on STDC_HEADERS.
 *
 * Revision 1.30  1994/07/22  22:21:28  mjl
 * Eliminated a gcc -Wall warning.
 *
 * Revision 1.29  1994/07/19  22:30:29  mjl
 * All device drivers: enabling macro renamed to PLD_<driver>, where <driver>
 * is xwin, ps, etc.  See plDevs.h for more detail.
 *
 * Revision 1.28  1994/06/30  17:52:35  mjl
 * Made another pass to eliminate warnings when using gcc -Wall, especially
 * those created by changing a PLINT from a long to an int.
 *
 * Revision 1.27  1994/06/24  04:38:35  mjl
 * Greatly reworked the POSIX_TTY code that puts the terminal into cbreak
 * mode.  Now, initially both the original and modified terminal states are
 * saved.  When the terminal goes into graphics mode, it is also put into
 * cbreak mode.  This ensures that the program gets character-at-a-time
 * input, which is good for quitting <Q> PLplot or for paging in plrender.
 * When the terminal goes into text mode, the original terminal state
 * (canonical input, typically) is restored, which is good for reading user
 * input or command interpreters.  Just make sure you use plgra() and
 * pltext() for switching; if you switch the terminal locally it may get
 * confused until the next plgra() or pltext().
 *
 * Revision 1.26  1994/06/23  22:32:07  mjl
 * Now ensures that device is in graphics mode before issuing any graphics
 * instruction.
*/

/*	tek.c

	PLPLOT tektronix device & emulators driver.
*/
#include "plDevs.h"

#if defined(PLD_xterm) ||		/* xterm */ \
    defined(PLD_tek4010) ||		/* TEK 4010 */ \
    defined(PLD_tek4107) ||		/* TEK 4107 */ \
    defined(PLD_mskermit) ||		/* MS-kermit emulator */ \
    defined(PLD_versaterm) ||		/* Versaterm emulator */ \
    defined(PLD_vlt)			/* VLT emulator */

#include "plplotP.h"
#include "drivers.h"
#include "plevent.h"

#include <ctype.h>

/* Static function prototypes */

static void	WaitForPage	(PLStream *pls);
static void	EventHandler	(PLStream *pls, int input_char);
static void	tek_init	(PLStream *pls);
static void	tek_text	(PLStream *pls);
static void	tek_graph	(PLStream *pls);
static void	fill_polygon	(PLStream *pls);
static void	encode_int	(char *c, int i);
static void	encode_vector	(char *c, int x, int y);
static void	tek_vector	(PLStream *pls, int x, int y);
static void	scolor		(int icol, int r, int g, int b);
static void	setcmap		(PLStream *pls);

/* Stuff for handling tty cbreak mode */

#ifdef POSIX_TTY

#include <termios.h>
#include <unistd.h>

static struct termios	termios_cbreak, termios_reset;
static enum { RESET, CBREAK } ttystate = RESET;

static void tty_atexit	(void);
static void tty_setup	(void);
static int  tty_cbreak	(void);
static int  tty_reset	(void);
#endif

/* Pixel settings */

#define TEKX   1023
#define TEKY    779

/* Graphics control characters. */

#define ETX  3
#define ENQ  5
#define BEL  7
#define FF   12
#define CAN  24
#define SUB  26
#define ESC  27
#define GS   29
#define US   31

/* Static vars */

enum {tek4010, tek4105, tek4107, xterm, mskermit, vlt, versaterm};

static exit_eventloop = 0;
static int curcolor;

/* color for MS-DOS Kermit v2.31 (and up) tektronix emulator 
 *	0 = normal, 1 = bright 
 *	foreground color (30-37) = 30 + colors
 *		where colors are   1=red, 2=green, 4=blue 
 */
static char *kermit_color[15]= {
    "\033[0;30m","\033[0;37m",
    "\033[0;32m","\033[0;36m","\033[0;31m","\033[0;35m",
    "\033[1;34m","\033[1;33m","\033[1;31m","\033[1;37m",
    "\033[1;35m","\033[1;32m","\033[1;36m","\033[0;34m",
    "\033[0;33m"};

/*----------------------------------------------------------------------*\
 * plD_init_xterm()	xterm 
 * plD_init_tekt()	Tek 4010 terminal
 * plD_init_tekf()	Tek 4010 file
 * plD_init_tek4107t()	Tek 4105/4107 terminal
 * plD_init_tek4107f()	Tek 4105/4107 file
 * plD_init_mskermit()	MS-Kermit emulator (DOS)
 * plD_init_vlt()	VLT emulator (Amiga)
 * plD_init_versaterm()	VersaTerm emulator (Mac)
 *
 * Initialize device.  These just set attributes for the particular
 * tektronix device, then call tek_init().  The following attributes can
 * be set:
 *
 * pls->termin		if a terminal device
 * pls->color		if color
 * pls->dev_fill0	if can handle solid area fill
 * pls->dev_fill1	if can handle pattern area fill
 *
\*----------------------------------------------------------------------*/

void 
plD_init_xterm(PLStream *pls)
{
    pls->dev_minor = xterm;
    pls->termin = 1;
    tek_init(pls);
}

void
plD_init_tekt(PLStream *pls)
{
    pls->termin = 1;
    plD_init_tekf(pls);
}

void
plD_init_tekf(PLStream *pls)
{
    pls->dev_minor = tek4010;
    tek_init(pls);
}

void
plD_init_tek4107t(PLStream *pls)
{
    pls->termin = 1;
    plD_init_tek4107f(pls);
}

void
plD_init_tek4107f(PLStream *pls)
{
    pls->dev_minor = tek4107;
    pls->color = 1;
    pls->dev_fill0 = 1;
    tek_init(pls);
}

void
plD_init_mskermit(PLStream *pls)
{
    pls->dev_minor = mskermit;
    pls->termin = 1;
    pls->color = 1;
    pls->dev_fill0 = 1;
    tek_init(pls);
}

void
plD_init_vlt(PLStream *pls)
{
    pls->dev_minor = vlt;
    pls->termin = 1;
    pls->color = 1;
    pls->dev_fill0 = 1;
    tek_init(pls);
}

void
plD_init_versaterm(PLStream *pls)
{
    pls->dev_minor = versaterm;
    pls->termin = 1;
    pls->color = 1;
    pls->dev_fill0 = 1;
    tek_init(pls);
}

/*----------------------------------------------------------------------*\
 * tek_init()
 *
 * Generic tektronix device initialization.
\*----------------------------------------------------------------------*/

static void
tek_init(PLStream *pls)
{
    PLDev *dev;

    pls->icol0 = 1;
    pls->width = 1;
    pls->bytecnt = 0;
    pls->page = 0;
    pls->graphx = TEXT_MODE;
    curcolor = 1;

/* Allocate and initialize device-specific data */

    dev = plAllocDev(pls);

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;
    dev->xmin = 0;
    dev->ymin = 0;
    dev->xmax = TEKX * 16;
    dev->ymax = TEKY * 16;
    dev->pxlx = 4.771 * 16;
    dev->pxly = 4.653 * 16;

    plP_setpxl(dev->pxlx, dev->pxly);
    plP_setphy(dev->xmin, dev->xmax, dev->ymin, dev->ymax);

/* Terminal/file initialization */

    if (pls->termin) {
	pls->OutFile = stdout;
#ifdef POSIX_TTY
	tty_setup();
#endif
    }
    else {
	plFamInit(pls);
	plOpenFile(pls);
    }

    switch (pls->dev_minor) {
    case tek4107:
	pls->graphx = GRAPHICS_MODE;
	fprintf(pls->OutFile, "%c%%!0", ESC);	/* set tek mode */
	fprintf(pls->OutFile, "%cKN1", ESC);	/* clear the view */
	fprintf(pls->OutFile, "%cLZ", ESC);	/* clear dialog buffer */
	fprintf(pls->OutFile, "%cML1", ESC);	/* set default color */
	break;

/* A sneaky hack: VLT sometimes has leftover panel information, causing 
 * garbage at the beginning of a sequence of color fills.  Since 
 * there is no clear panel command, instead I set the fill color to the
 * same as background and issue an end panel command.
 */

    case vlt:{
	char fillcol[4];
	tek_graph(pls);
	encode_int(fillcol, 0);
	fprintf(pls->OutFile, "%cMP%s", ESC, fillcol);
	fprintf(pls->OutFile, "%cLE", ESC);
	break;
    }

    default:
	tek_graph(pls);
    }

/* Initialize palette */

    if (pls->color) {
	printf("%cTM111", ESC);		/* Switch to RGB colors */
	setcmap(pls);
    }

/* Finish initialization */

    fprintf(pls->OutFile, "%c", GS);		/* Enter vector mode */
    if (pls->termin)
	fprintf(pls->OutFile, "%c%c", ESC, FF);	/* erase and home */

    fflush(pls->OutFile);
}

/*----------------------------------------------------------------------*\
 * plD_line_tek()
 *
 * Draw a line from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_tek(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
    PLDev *dev = (PLDev *) pls->dev;
    int x1 = x1a >> 4, y1 = y1a >> 4, x2 = x2a >> 4, y2 = y2a >> 4;

    tek_graph(pls);

/* If continuation of previous line just send new point */

    if (x1 != dev->xold || y1 != dev->yold) {
	fprintf(pls->OutFile, "%c", GS);
	pls->bytecnt++;
	tek_vector(pls, x1, y1);
    }
    tek_vector(pls, x2, y2);

    dev->xold = x2;
    dev->yold = y2;
}

/*----------------------------------------------------------------------*\
 * plD_polyline_tek()
 *
 * Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_tek(PLStream *pls, short *xa, short *ya, PLINT npts)
{
    PLINT i;

    for (i = 0; i < npts - 1; i++)
	plD_line_tek(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
 * plD_eop_tek()
 *
 * End of page.  User must hit a <CR> to continue (terminal output).
\*----------------------------------------------------------------------*/

void
plD_eop_tek(PLStream *pls)
{
    tek_graph(pls);

    if (pls->termin) {
	if ( ! pls->nopause) 
	    WaitForPage(pls);
    }
    fprintf(pls->OutFile, "%c%c", ESC, FF);
}

/*----------------------------------------------------------------------*\
 * plD_bop_tek()
 *
 * Set up for the next page.  Advance to next family file if necessary
 * (file output).  Devices that share graphics/alpha screens need a page
 * clear.
\*----------------------------------------------------------------------*/

void
plD_bop_tek(PLStream *pls)
{
    PLDev *dev = (PLDev *) pls->dev;

    dev->xold = UNDEFINED;
    dev->yold = UNDEFINED;

    if (pls->termin) {
	switch (pls->dev_minor) {
	case mskermit:
	    fprintf(pls->OutFile, "%c%c", ESC, FF);
	    break;
	}
    }
    else
	plGetFam(pls);

    pls->page++;

/* Initialize palette */

    if (pls->color) 
	setcmap(pls);
}

/*----------------------------------------------------------------------*\
 * plD_tidy_tek()
 *
 * Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_tek(PLStream *pls)
{
    if ( ! pls->termin) {
	fclose(pls->OutFile);
    }
    else {
	tek_text(pls);
	fflush(pls->OutFile);
    }
}

/*----------------------------------------------------------------------*\
 * plD_state_tek()
 *
 * Handle change in PLStream state (color, pen width, fill attribute,
 * etc).
\*----------------------------------------------------------------------*/

void 
plD_state_tek(PLStream *pls, PLINT op)
{
    switch (op) {

    case PLSTATE_WIDTH:
	break;

    case PLSTATE_COLOR0:
	if (pls->color) {
	    int icol0 = pls->icol0;
	    tek_graph(pls);
	    if (icol0 != PL_RGB_COLOR) {
		curcolor = icol0;
		switch (pls->dev_minor) {
		case mskermit:
		    fprintf(pls->OutFile, "%s", kermit_color[icol0 % 14] );
		    break;

		default:
		    fprintf(pls->OutFile, "%cML%c", ESC, icol0 + 48);
		}
	    }
	}
	break;

    case PLSTATE_COLOR1:
	if (pls->color) {
	    int icol1, ncol1;
	    tek_graph(pls);
	    if ((ncol1 = MIN(16 - pls->ncol0, pls->ncol1)) < 1)
		break;

	    icol1 = pls->ncol0 + (pls->icol1 * (ncol1-1)) / (pls->ncol1-1);
	    curcolor = icol1;
	    fprintf(pls->OutFile, "%cML%c", ESC, icol1 + 48);
	}
	break;

    case PLSTATE_CMAP0:
    case PLSTATE_CMAP1:
	if (pls->color)
	    setcmap(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
 * plD_esc_tek()
 *
 * Escape function.
\*----------------------------------------------------------------------*/

void 
plD_esc_tek(PLStream *pls, PLINT op, void *ptr)
{
    switch (op) {

    case PLESC_TEXT:
	tek_text(pls);
	break;

    case PLESC_GRAPH:
	tek_graph(pls);
	break;

    case PLESC_FILL:
	fill_polygon(pls);
	break;
    }
}

/*----------------------------------------------------------------------*\
 * fill_polygon()
 *
 * Fill polygon described in points pls->dev_x[] and pls->dev_y[].
\*----------------------------------------------------------------------*/

static void
fill_polygon(PLStream *pls)
{
    int i;
    char fillcol[4], firstpoint[5];

    if (pls->dev_npts < 1)
	return;

    tek_graph(pls);

    encode_int(fillcol, -curcolor);
    encode_vector(firstpoint, pls->dev_x[0]>>4, pls->dev_y[0]>>4);

/* Select the fill pattern */

    fprintf(pls->OutFile, "%cMP%s", ESC, fillcol);

/* Begin panel boundary */
/* Change %s0 to %s1 to see the boundary of each fill box -- cool! */

    fprintf(pls->OutFile, "%cLP%s0", ESC, firstpoint);

/* Specify boundary (use vector mode) */

    fprintf(pls->OutFile, "%c", GS);
    pls->bytecnt++;
    for (i = 1; i < pls->dev_npts; i++) {
	tek_vector(pls, pls->dev_x[i]>>4, pls->dev_y[i]>>4);
    }

/* End panel */

    fprintf(pls->OutFile, "%cLE", ESC);
}

/*----------------------------------------------------------------------*\
 * tek_text()
 *
 * Switch to text screen (or alpha mode, for vanilla tek's).  Restore
 * terminal to its original state, to better handle user input if
 * necessary.
 *
 * Note: xterm behaves strangely in the following circumstance: switch to
 * the text screen, print a string, and switch to the graphics screen, all
 * done in quick succession.  The first character of the printed string
 * usually comes out blank -- but only apparently so, because if you force
 * a refresh of the screen in this area it will reappear.  This is a
 * reproducible bug on the HP 720 under X11R5.  If you insert a sleep(1)
 * after the switch to text screen or before the switch to graphics
 * screen, the string is printed correctly.  I've been unable to find a
 * workaround for this problem (and I've tried, you can believe eet man).
\*----------------------------------------------------------------------*/

static void 
tek_text(PLStream *pls)
{
    if (pls->termin && (pls->graphx == GRAPHICS_MODE)) {
#ifdef POSIX_TTY
	tty_reset();
#endif
	pls->graphx = TEXT_MODE;
	switch (pls->dev_minor) {
	case xterm:
	    printf("%c%c", ESC, ETX);		/* switch to vt100 screen */
	    break;

	case mskermit:
	case vlt:
	    printf("%c[?38l", ESC);		/* switch to vt100 screen */
	    break;

	case versaterm:
	    printf("%c%%!2", ESC);		/* switch to vt100 screen */
	    break;

	case tek4107:
	    printf("%cLV1", ESC);		/* set dialog visible */
	    printf("%c%%!1", ESC);		/* set ansi mode */
	    break;

	default:
	    printf("%c", US);			/* enter alpha mode */
	}
    }
}

/*----------------------------------------------------------------------*\
 * tek_graph()
 *
 * Switch to graphics screen (or vector mode, for vanilla tek's).  Also
 * switch terminal to cbreak mode, to allow single keystrokes to govern
 * actions at end of page.
\*----------------------------------------------------------------------*/

static void 
tek_graph(PLStream *pls)
{
    if (pls->termin && (pls->graphx == TEXT_MODE)) {
#ifdef POSIX_TTY
	tty_cbreak();
#endif
	pls->graphx = GRAPHICS_MODE;
	switch (pls->dev_minor) {
	case xterm:
	case mskermit:
	case vlt:
	    printf("%c[?38h", ESC);		/* switch to tek screen */
	    break;

	case versaterm:
	    printf("%c%%!0", ESC);		/* switch to tek4107 screen */
	    break;

	case tek4107:
	    printf("%c%%!0", ESC);		/* set tek mode */
	    printf("%c%c", ESC, FF);		/* clear screen */
	    printf("%cLV0", ESC);		/* set dialog invisible */
	    break;

	default:
	    printf("%c", GS);			/* enter vector mode */
	}
    }
}

/*----------------------------------------------------------------------*\
 * encode_int()
 *
 * Encodes a single int into standard tek integer format, storing into a
 * NULL-terminated character string (must be length 4 or greater).  This
 * scheme does not work for negative integers less than 15.
\*----------------------------------------------------------------------*/

static void
encode_int(char *c, int i)
{
    int negative = 0;

    if (i > 0) {
	if (i & 0x7C00)				/* are any of bits 10-14 set?*/
	    *c++ = ((i >> 10) & 0x1F) | 0x40;
	if (i & 0x03F0)				/* are any of bits 4-9 set? */
	    *c++ = ((i >> 4) & 0x3F) | 0x40;
    }
    else {
	i = -i;
	negative = 1;
    }

    if (i & 0x000F)				/* are any of bits 0-3 set? */
	*c = (i & 0x0F) | 0x20;
    else					/* if not, put in a space */
	*c = 0x20;
	
    if ( ! negative)				/* if positive, set sign bit */
	*c |= 0x10;

    c++; *c = '\0';				/* NULL-terminate */
    return;
}

/*----------------------------------------------------------------------*\
 * encode_vector()
 *
 * Encodes an xy vector (2 ints) into standard tek vector format, storing
 * into a NULL-terminated character string of length 5.  Note that the y
 * coordinate always comes first.
\*----------------------------------------------------------------------*/

static void
encode_vector(char *c, int x, int y)
{
    c[0] = y / 32 + 32;
    c[1] = y - (y / 32) * 32 + 96;
    c[2] = x / 32 + 32;
    c[3] = x - (x / 32) * 32 + 64;
    c[4] = '\0';
}

/*----------------------------------------------------------------------*\
 * tek_vector()
 *
 * Issues a vector draw command, assuming we are in vector plot mode.  XY
 * coordinates are encoded according to the standard xy encoding scheme.
\*----------------------------------------------------------------------*/

static void
tek_vector(PLStream *pls, int x, int y)
{
    int hy, ly, hx, lx;

    hy = y / 32 + 32;
    ly = y - (y / 32) * 32 + 96;
    hx = x / 32 + 32;
    lx = x - (x / 32) * 32 + 64;
    fprintf(pls->OutFile, "%c%c%c%c", hy, ly, hx, lx);
    pls->bytecnt += 4;
}

/*----------------------------------------------------------------------*\
 * scolor()
 *
 * Sets a color by tek-encoded RGB values.  Need to convert PLPLOT RGB
 * color range (0 to 255) to Tek RGB color range (0 to 100).
\*----------------------------------------------------------------------*/

static void
scolor(int icol, int r, int g, int b)
{
    char tek_col[4], tek_r[4], tek_g[4], tek_b[4];

    encode_int(tek_col, icol);
    encode_int(tek_r, 100*r/255);
    encode_int(tek_g, 100*g/255);
    encode_int(tek_b, 100*b/255);

    printf("%cTG14%s%s%s%s", ESC, tek_col, tek_r, tek_g, tek_b);
}

/*----------------------------------------------------------------------*\
 * setcmap()
 *
 * Sets up color palette.
\*----------------------------------------------------------------------*/

static void
setcmap(PLStream *pls)
{
    int i, ncol1 = MIN(16 - pls->ncol0, pls->ncol1);
    PLColor cmap1col;

    tek_graph(pls);

/* Initialize cmap 0 colors */

    for (i = 0; i < pls->ncol0; i++) 
	scolor(i, pls->cmap0[i].r, pls->cmap0[i].g, pls->cmap0[i].b);

/* Initialize any remaining slots for cmap1 */

    for (i = 0; i < ncol1; i++) {
	plcol_interp(pls, &cmap1col, i, ncol1);
	scolor(i + pls->ncol0, cmap1col.r, cmap1col.g, cmap1col.b);
    }
}

/*----------------------------------------------------------------------*\
 * WaitForPage()
 *
 * This routine waits for the user to advance the plot, while handling
 * all other events.
\*----------------------------------------------------------------------*/

static void
WaitForPage(PLStream *pls)
{
    int input_char;

    putchar(BEL);
    fflush(stdout);

    while ( ! exit_eventloop) {
	input_char = getchar();
	EventHandler(pls, input_char);
    }
    exit_eventloop = FALSE;
}

/*----------------------------------------------------------------------*\
 * EventHandler()
 *
 * Event handler routine for xterm.  Just reacts to keyboard input.
\*----------------------------------------------------------------------*/

static void
EventHandler(PLStream *pls, int input_char)
{
    PLKey key;

    key.code = 0;
    key.string[0] = '\0';

/* Translate keystroke into a PLKey */

    if ( ! isprint(input_char)) {
	switch (input_char) {
	case 0x0A:
	    key.code = PLK_Linefeed;
	    break;

	case 0x0D:
	    key.code = PLK_Return;
	    break;

	case 0x08:
	    key.code = PLK_BackSpace;
	    break;

	case 0x7F:
	    key.code = PLK_Delete;
	    break;
	}
    }
    else {
	key.string[0] = input_char;
	key.string[1] = '\0';
    }

#ifdef DEBUG
    pltext();
    fprintf(stderr, "Input char %x, Keycode %x, string: %s\n",
	    input_char, key.code, key.string);
    plgra();
#endif

/* Call user event handler */
/* Since this is called first, the user can disable all plplot internal
   event handling by setting key.code to 0 and key.string to '\0' */

    if (pls->KeyEH != NULL)
	(*pls->KeyEH) (&key, pls->KeyEH_data, &exit_eventloop);

/* Handle internal events */

/* Advance to next page (i.e. terminate event loop) on a <eol> */

    if (key.code == PLK_Linefeed)
	exit_eventloop = TRUE;

/* Terminate on a 'Q' (not 'q', since it's too easy to hit by mistake) */

    if (key.string[0] == 'Q') {
	pls->nopause = TRUE;
	plexit("");
    }

/* Some test code, don't rely on it. */

    if (key.string[0] == 't') {
	fprintf(stderr, "Entering GIN mode..\n");

	printf("%c%c", ESC, SUB);	/* Enter GIN mode */
	fflush(stdout);
    }
}

/*----------------------------------------------------------------------*\
 * tty cbreak-mode handlers
 *
 * Taken from "Advanced Programming in the UNIX(R) Environment", 
 * by W. Richard Stevens.
\*----------------------------------------------------------------------*/

#ifdef POSIX_TTY

static void
tty_setup(void)				/* setup for terminal operations */
{
    if (tcgetattr(STDIN_FILENO, &termios_reset) < 0) {
	fprintf(stderr, "Unable to set up cbreak mode.\n");
	return;
    }

    termios_cbreak = termios_reset;		/* structure copy */

    termios_cbreak.c_lflag &= ~(ICANON);	/* canonical mode off */
    termios_cbreak.c_cc[VMIN] = 1;		/* 1 byte at a time */
    termios_cbreak.c_cc[VTIME] = 0;		/* no timer */

#ifdef STDC_HEADERS
    if (atexit(tty_atexit))
	fprintf(stderr, "Unable to set up atexit handler.\n");
#endif
    return;
}

static int
tty_cbreak(void)			/* put terminal into a cbreak mode */
{
    if (ttystate != CBREAK) {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_cbreak) < 0)
	    return -1;

	ttystate = CBREAK;
    }
    return 0;
}

static int
tty_reset(void)				/* restore terminal's mode */
{
    if (ttystate != RESET) {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termios_reset) < 0)
	    return -1;

	ttystate = RESET;
    }
    return 0;
}

static void
tty_atexit(void)			/* exit handler */
{
    tty_reset();
}

#endif			/* POSIX_TTY */

#else
int pldummy_tek() {return 0;}

#endif	/*  defined(PLD_xterm) || ... */
