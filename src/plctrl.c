/* $Id$
   $Log$
   Revision 1.9  1993/11/07 09:08:14  mjl
   Added user-settable exit handler (call plsexit to set).

 * Revision 1.8  1993/10/21  19:28:25  mjl
 * Minor bug fix.
 *
 * Revision 1.7  1993/10/18  19:44:50  mjl
 * Added functions to return fully qualified pathnames and/or executable
 * names.
 *
 * Revision 1.6  1993/09/27  20:40:12  mjl
 * Added #include <string.h> to pick up prototype for strcat.
 *
 * Revision 1.5  1993/09/08  02:37:48  mjl
 * Added plGetName for building up file name specs out of path components.
 * Also moved some utility functions from plstream.c here.
 *
 * Revision 1.4  1993/07/31  08:18:10  mjl
 * Changes to text/graph screen switching commands to reflect reorganization.
 *
 * Revision 1.3  1993/07/01  22:20:19  mjl
 * Changed all plplot source files to include plplotP.h (private) rather than
 * plplot.h.  Rationalized namespace -- all externally-visible internal
 * plplot functions now start with "plP_". Moved functions plend() and plend1()
 * to plcore.c.  Added pl_cmd() as a front-end to the driver escape function
 * to allow virtually any command & data be sent to the driver by the calling
 * program.
*/

/*	plctrl.c

	Misc. control routines, like begin, end, exit, change graphics/text
	mode, change color.  If you don't know where it should go, put
	it here.
*/

#include "plplotP.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __unix
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#endif

static void	strcat_delim	(char *);
static void	(*exit_handler) (void);

/*----------------------------------------------------------------------*\
* void plexit()
*
* In case of an abort this routine is called.  It just prints out an
* error message and tries to clean up as much as possible.
\*----------------------------------------------------------------------*/

void
plexit(char *errormsg)
{
    plend();
    if (*errormsg != '\0') {
	fprintf(stderr, "\n*** PLPLOT ERROR ***\n");
	fprintf(stderr, "%s\n", errormsg);
    }
    fprintf(stderr, "Program aborted\n");
    pl_exit();
}

/*----------------------------------------------------------------------*\
* void pl_exit()
*
* Just a front-end to exit().  If cleanup needs to be done in the main
* program, the user should write his/her own exit handler and pass it in
* via plsexit().
\*----------------------------------------------------------------------*/

void
pl_exit(void)
{
    if (exit_handler != NULL)
	(*exit_handler)();
    else
	exit(1);
}

/*----------------------------------------------------------------------*\
* void plsexit()
*
* Sets the exit handler to use instead of exit().
\*----------------------------------------------------------------------*/

void
plsexit(void (*handler) (void))
{
    exit_handler = handler;
}

/*----------------------------------------------------------------------*\
* void plgra()
*
* Switches to graphics screen.
\*----------------------------------------------------------------------*/

void
c_plgra()
{
    PLINT level;
    plP_glev(&level);
    if (level < 1)
	plexit("plgra: Please call plinit first.");

    plP_esc(PLESC_GRAPH, NULL);
}

/*----------------------------------------------------------------------*\
* void pltext()
*
* Switches to text screen.
\*----------------------------------------------------------------------*/

void
c_pltext()
{
    PLINT level;

    plP_glev(&level);
    if (level < 1)
	plexit("pltext: Please call plinit first.");

    plP_esc(PLESC_TEXT, NULL);
}

/*----------------------------------------------------------------------*\
* void plhls()
*
* Set current color by hue, lightness, and saturation.
* Convert hls color coordinates to rgb, then call plrgb.
\*----------------------------------------------------------------------*/

void
c_plhls(PLFLT h, PLFLT l, PLFLT s)
{
    PLFLT r, g, b;

    plHLS_RGB(h, l, s, &r, &g, &b);
    plrgb(r, g, b);
}

/*----------------------------------------------------------------------*\
* void value()
*
* Auxiliary function used by plHLS_RGB().
\*----------------------------------------------------------------------*/

static float
value(double n1, double n2, double hue)
{
    float val;

    while (hue >= 360.)
	hue -= 360.;
    while (hue < 0.)
	hue += 360.;

    if (hue < 60.)
	val = n1 + (n2 - n1) * hue / 60.;
    else if (hue < 180.)
	val = n2;
    else if (hue < 240.)
	val = n1 + (n2 - n1) * (240. - hue) / 60.;
    else
	val = n1;

    return (val);
}

/*----------------------------------------------------------------------*\
* void plHLS_RGB()
*
* Convert HLS color to RGB color.
* Bounds on each coordinate:
*	hue		[0., 360.]	degrees
*	lightness	[0., 1.]	magnitude
*	saturation	[0., 1.]	magnitude
*
*  Hue is always mapped onto the interval [0., 360.] regardless of input.
\*----------------------------------------------------------------------*/

void
plHLS_RGB(PLFLT h, PLFLT l, PLFLT s, PLFLT *p_r, PLFLT *p_g, PLFLT *p_b)
{
    float m1, m2;

    if (l <= .5)
	m2 = l * (s + 1.);
    else
	m2 = l + s - l * s;

    m1 = 2 * l - m2;

    *p_r = value(m1, m2, h + 120.);
    *p_g = value(m1, m2, h);
    *p_b = value(m1, m2, h - 120.);
}

/*----------------------------------------------------------------------*\
* void pl_cmd()
*
* Front-end to driver escape function.
* In principle this can be used to pass just about anything directly
* to the driver.
\*----------------------------------------------------------------------*/

void pl_cmd(PLINT op, void *ptr)
{
    plP_esc(op, ptr);
}

/*----------------------------------------------------------------------*\
* char *plFindCommand
*
* Looks for the specified executable file.  Search path:
*	current directory
*	$(PLPLOT_DIR)
*	INSTALL_DIR
*
* The caller must free the returned pointer (points to malloc'ed memory)
* when finished with it.
\*----------------------------------------------------------------------*/

char *
plFindCommand(char *fn)
{
    char *fs = NULL, *dn;

/* Current directory */

    plGetName(".", "", fn, &fs);
    if ( ! plFindName(fs))
	return fs;

/* $(PLPLOT_DIR) */

    if ((dn = getenv("PLPLOT_DIR")) != NULL) {
	plGetName(dn, "", fn, &fs);
	if ( ! plFindName(fs))
	    return fs;
    }

/* INSTALL_DIR */

#ifdef INSTALL_DIR
    plGetName(INSTALL_DIR, "", fn, &fs);
    if ( ! plFindName(fs))
	return fs;
#endif

/* Crapped out */

    free_mem(fs);
    fprintf(stderr, "plFindCommand: cannot locate command: %s\n", fn);
    return NULL;
}

/*----------------------------------------------------------------------*\
* int plFindName
*
* Authors: Paul Dubois (LLNL), others?
* This function is in the public domain.
*
* Given a pathname, determine if it is a symbolic link.  If so, continue
* searching to the ultimate terminus - there may be more than one link.
* Use the error value to determine when the terminus is reached, and to
* determine if the pathname really exists.  Then stat it to determine
* whether it's executable.  Return 0 for an executable, errno otherwise.
* Note that 'p' _must_ have at least one '/' character - it does by
* construction in this program.  The contents of the array pointed to by
* 'p' are changed to the actual pathname if findname is successful.
*
* This function is only defined under Unix for now.
\*----------------------------------------------------------------------*/

#ifdef __unix
int 
plFindName(char *p)
{
    int n;
    char buf[1024], *cp;
    extern int errno;
    struct stat sbuf;

    while ((n = readlink(p, buf, 1024)) > 0) {
#ifdef DEBUG
	fprintf(stderr, "Readlink read %d chars at: %s\n", n, p);
#endif
	if (buf[0] == '/') {	/* Link is an absolute path */
	    strncpy(p, buf, n);
	    p[n] = '\0';
#ifdef DEBUG
	    fprintf(stderr, "Link is absolute: %s\n", p);
#endif
	}
	else {			/* Link is relative to its directory; make it
				   absolute */
	    cp = 1 + strrchr(p, '/');
	    strncpy(cp, buf, n);
	    cp[n] = '\0';
#ifdef DEBUG
	    fprintf(stderr, "Link is relative: %s\n\tTotal path: %s\n", cp, p);
#endif
	}
    }

/* SGI machines return ENXIO instead of EINVAL Dubois 11/92 */

    if (errno == EINVAL || errno == ENXIO) {
#ifdef DEBUG
	fprintf(stderr, "%s may be the one ...", p);
#endif
#ifdef SX
#define S_ISREG(mode)   (mode & S_IFREG)
#endif
	if ((stat(p, &sbuf) == 0) && S_ISREG(sbuf.st_mode)) {
#ifdef DEBUG
	    fprintf(stderr, "regular file\n");
#endif
	    return (access(p, X_OK));
	}
    }
#ifdef DEBUG
    fprintf(stderr, "not executable\n");
#endif
    return (errno ? errno : -1);
}

#else
int 
plFindName(char *p)
{
    return 1;
}
#endif

/*----------------------------------------------------------------------*\
* void plGetName()
*
* Gets search name for file by concatenating the dir, subdir, and file
* name, allocating memory as needed.  The appropriate delimiter is added
* after the dir specification as necessary.  The caller is responsible
* for freeing the malloc'ed memory.
\*----------------------------------------------------------------------*/

void
plGetName(char *dir, char *subdir, char *filename, char **filespec)
{
    int lfilespec;

/* Malloc space for filespec */

    free_mem(*filespec);
    lfilespec = 10;
    lfilespec = strlen(dir) + strlen(subdir) + strlen(filename) + 10;
    *filespec = (char *) malloc(lfilespec);

    strcpy(*filespec, dir);

    if (*subdir != '\0') {
	strcat_delim(*filespec);
	strcat(*filespec, subdir);
    }
    if (*filename != '\0') {
	strcat_delim(*filespec);
	strcat(*filespec, filename);
    }
}

/*----------------------------------------------------------------------*\
* void strcat_delim()
*
* Append path name deliminator if necessary (does not add one if one's
* there already, or if dealing with a colon-terminated device name as
* used on the Amiga).
\*----------------------------------------------------------------------*/

static void
strcat_delim(char *dirspec)
{
    int ldirspec = strlen(dirspec);
#ifdef MSDOS
    if (dirspec[ldirspec-1] != '\\')
	strcat(dirspec, "\\");
#endif
#ifdef __unix
    if (dirspec[ldirspec-1] != '/')
	strcat(dirspec, "/");
#endif
#ifdef AMIGA
    if (dirspec[ldirspec-1] != '/' && dirspec[ldirspec-1] != ':')
	strcat(dirspec, "/");
#endif
}

/*----------------------------------------------------------------------*\
* plGetInt()
*
* Prompts human to input an integer in response to given message.
\*----------------------------------------------------------------------*/

PLINT
plGetInt(char *s)
{
    PLINT m;
    int i = 0;
    char line[256];

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
    return (0);
}

/*----------------------------------------------------------------------*\
* plGetFlt()
*
* Prompts human to input a float in response to given message.
\*----------------------------------------------------------------------*/

PLFLT
plGetFlt(char *s)
{
    PLFLT m;
    int i = 0;
    char line[256];

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
    return (0.);
}
