/* $Id$
 * $Log$
 * Revision 1.6  1994/08/10 01:15:11  mjl
 * Changed/improved page prompt, enabled both forward and backward relative
 * seeking, eliminated system dependent input code.
 *
 * Revision 1.5  1994/06/30  18:55:52  mjl
 * Minor changes to eliminate gcc -Wall warnings.
*/

/*
 *  pltek.c
 *  Review a Tektronix vector file.
 *  from 'scan', by Clair Nielsen, LANL.
 *  Modifications by Maurice LeBrun, IFS.
 *
 *  This version works well with xterm and at least one vt100/tek emulator
 *  I've tried.
 */

#include <stdio.h>
#include <math.h>
#include <ctype.h>

static long start[1000];
void describe();

/* Define graphics control characters. */

#define FF   12
#define CAN  24
#define ESC  27
#define GS   29
#define US   31
#define ETX  003

int
main(int argc, char *argv[])
{
    FILE *fd;
    char infile[128];
    int i, j, nb, npage, ipage, nextpage, ifirst, oldpage;
    int istop;
    char xtra;
    char buf[1024], lastchar;
    char ibuf[128];
    char c;

    if (argc < 2) {
	describe();
	exit(1);
    }
    else
	strcpy(infile, argv[1]);

    if ((fd = fopen(infile, "r")) == NULL) {
	printf("Cannot open %s\n", argv[1]);
	exit(1);
    }

/* Find out how many pages there are in file. */

    ipage = 0;
    start[0] = 0;
    for (i = 0; i < 10000; i++) {
	nb = fread(buf, 1, 1024, fd);
	if (nb <= 0)
	    break;
	ifirst = 0;
	for (j = 0; j < nb; j++) {
	    if ((lastchar = buf[j]) == '\f') {
		ifirst = j - 1;
		ipage++;
		start[ipage] = 1024 * i + ifirst + 2;
	    }
	}
    }

/* don't count a FF at the end of the file as a separate page */

    if (lastchar == '\f')
	ipage--;

    npage = ipage + 1;
    printf("found %d pages\n", npage);

/* Loop until the user quits */

    ipage = 0;
    while (1) {
	oldpage = ipage;
	printf("Page %d> ", ipage);

	gets(ibuf);
	c = ibuf[0];

/* User input a page number or a return */
/* A carriage return in response to the prompt proceeds to the next page. */

	if (c == '\0')
	    ipage++;

	else if (c == '+') 
	    ipage += atoi(ibuf+1);

	else if (c == '-') 
	    ipage -= atoi(ibuf+1);

	else if (isdigit(c)) 
	    ipage = atoi(ibuf);

/* User input a command or garbage */

	else {
	    if (c == 'h' || c == '?') {
		describe();
		continue;
	    }
	    else if (c == 'q')
		break;
	    else {
		describe();
		continue;
	    }
	}

/* Bounds checking */

	if (ipage > npage) {
	    printf("  Last page = %d.\n", npage);
	    ipage = npage;
	    if (ipage == oldpage)
		continue;
	}
	else if (ipage < 0) {
	    ipage = 1;
	}

/* Time to actually plot something. */

	istop = fseek(fd, start[ipage - 1], 0);
	xtra = '\0';
	istop = 0;
	printf("%c[?38h", ESC);		/* open graphics window */
	printf("%c", GS);		/* set to vector mode */
	printf("%c%c", ESC, FF);	/* clear screen */

	for (i = 0; i < 10000; i++) {
	    if (xtra != '\0') {
		fwrite(&xtra, 1, 1, stdout);
		xtra = '\0';
	    }
	    nb = fread(buf, 1, 1024, fd);
	    if (nb <= 0)
		break;
	    ifirst = 0;
	    for (j = 0; j < nb; j++) {
		if (buf[j] == '\f') {
		    fwrite(&buf[ifirst], 1, j - ifirst, stdout);
		    fflush(stdout);
		    istop = 1;
		    break;
		}
	    }
	    if (istop != 0)
		break;
	    if (buf[nb] == ESC) {
		j = j - 1;
		xtra = ESC;
	    }
	    fwrite(&buf[ifirst], 1, j - ifirst, stdout);
	}
	printf("%c%c", US, CAN);
	printf("%c%c", ESC, ETX);
	fflush(stdout);
    }
    fclose(fd);
    exit(0);
}

/*----------------------------------------------------------------------*\
 *  describe()
 *
 *  Print help message.
 *  Note: if this message starts to exceed 512 bytes, may need to split
 *  since some compilers can't handle strings that long.
\*----------------------------------------------------------------------*/

void 
describe (void)
{
    fputs("\
\n\
Usage: pltek filename \n\
At the prompt, the following replies are recognized:\n\
\n\
    h	  Gives this help message.\n\
    ?     As above.\n\
    q	  Quits program.\n\
   <n>	  Goes to the specified page number (surprise!).\n\
   -<n>   Goes back <n> pages.\n\
   +<n>   Goes forward <n> pages.\n\
 <Return> Goes to the next page (first page, if initial reply).\n\
\n\
", stdout);
}
