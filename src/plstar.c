/* $Id$
   $Log$
   Revision 1.3  1992/09/30 18:25:54  furnish
   Massive cleanup to irradicate garbage code.  Almost everything is now
   prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.

 * Revision 1.2  1992/09/29  04:46:18  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:34:48  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plstar.c

	Routines for initializing plplot.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "plplot.h"
#include "dispatch.h"

/*----------------------------------------------------------------------*\
* void plstar(nx, ny)
*
* Asks for number of plotting device, and call plbeg to divide the page 
* into nx by ny subpages.  When querying for device, quits after 10 tries
* in case the user has run program in the background without input.
\*----------------------------------------------------------------------*/

void 
c_plstar( PLINT nx, PLINT ny )
{
    PLINT dev, i, level, count, length, ipls;
    char response[10];
    extern PLINT npldrivers;
    extern DISPATCH_TABLE plDispatchTable[];

    glev(&level);
    if (level != 0)
	plend1();

    dev = 0;
    count = 0;
    plgstrm(&ipls);

    if (npldrivers == 1)
	dev = 1;

    while (dev < 1 || dev > npldrivers) {
	printf("\nPlotting Options:\n");
	for (i = 0; i < npldrivers; i++)
#ifdef MSDOS
	    printf(" <%2ld> (%s)\t %s\n", i + 1, plDispatchTable[i].pl_DevName,
		   plDispatchTable[i].pl_MenuStr);
#else
	    printf(" <%2d> (%s)\t %s\n", i + 1, plDispatchTable[i].pl_DevName,
		   plDispatchTable[i].pl_MenuStr);
#endif
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
	    if (!strncmp(response, plDispatchTable[i].pl_DevName, length))
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
    plbeg(dev, nx, ny);
}

/*----------------------------------------------------------------------*\
* void plstart(devname, nx, ny)
*
* An alternate startup routine, where user supplies the device name (not
* number).  Superior to 'plbeg' since it is relatively independent of the
* particular plplot installation.  If the requested device is not
* available, or if *devname is '?' or devname is NULL, the normal
* (prompted) startup is used.
\*----------------------------------------------------------------------*/

void 
c_plstart( char *devname, PLINT nx, PLINT ny )
{
    PLINT dev, i, level;
    extern PLINT npldrivers;
    extern DISPATCH_TABLE plDispatchTable[];

    glev(&level);
    if (level != 0)
	plend1();

    if (devname == NULL) {
	plstar(nx, ny);
	return;
    }
    if (*devname == '?') {
	plstar(nx, ny);
	return;
    }
    dev = 0;
    for (i = 0; i < npldrivers; i++) {
	if (!strcmp(devname, plDispatchTable[i].pl_DevName))
	    break;
    }
    if (i < npldrivers) {
	dev = i + 1;
	plbeg(dev, nx, ny);
    }
    else {
	printf("Requested device %s not available\n", devname);
	plstar(nx, ny);
    }
}
