/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:46:48  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:36:00  furnish
 * Initial checkin of the whole PLPLOT project.
 *
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

static long start[1000];
void describe();
#define UNSELECTED -999

/* Define graphics control characters. */

#define FF   12
#define CAN  24
#define ESC  27
#define GS   29
#define US   31
#define ETX  003

int 
main (int argc, char *argv[])
{
	FILE *fd,*fd1;
	char infile[128];
	int i,j,k,nb,nframe,iframe,ifirst,oldframe;
	int istop;
	char xtra;
	char buf[1024], lastchar;
	int dummy;
	char *ninp;
	char ibuf[128];
	char c;

	if (argc < 2) {
	    describe();
	    exit(1);
	}
	else
	    strcpy(infile,argv[1]);

	if ((fd = fopen(infile,"r")) == NULL) {
	    printf("Cannot open %s\n",argv[1]);
	    exit(1);
	}
	fd1 = fopen("temp.out","w");

/* Find out how many frames there are in file. */

	iframe = 0;
	start[0] = 0;
	for (i = 0; i < 10000; i++) {
	    nb = fread(buf,1,1024,fd);
	    if (nb <= 0) break;
	    ifirst = 0;
	    for (j = 0; j < nb; j++) {
		if ((lastchar = buf[j]) == '\f') {
		    ifirst = j-1;
		    iframe++;
		    start[iframe] = 1024*i+ifirst+2;
		}
	    }
	}

/* don't count a FF at the end of the file as a separate frame */

	if (lastchar == '\f') 
	    iframe--;

	nframe = iframe+1;
	printf("found %d frames\n",nframe);

/* Loop until the user quits */

	iframe = 0;
	while (1) {
	    oldframe = iframe;
	    iframe++;
	    if (iframe >= 1 && iframe <= nframe)
		printf("Frame Number? [%d] ", iframe);
	    else {
		iframe = UNSELECTED;
		printf("Command? ");
	    }

	    gets(ibuf);
	    c = ibuf[0];

/* User input a frame number or a return */
/* A carriage return in response to the prompt proceeds to the next page. */

	    if (isdigit(c) || c == '\0') {
#ifdef MSDOS
		igrame = atoi( ibuf );
#else
		sscanf(ibuf," %d",&iframe);
#endif
		if (iframe == UNSELECTED) continue;
		if (iframe > nframe) {
		    printf("  Last frame = %d.\n", nframe);
		    continue;
		}
		if (iframe < 0) {
		    iframe += oldframe;
		    if (iframe < 0) {
			printf("  Illegal frame number.\n");
			continue;
		    }
		}
	    }

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

	    istop = fseek(fd,start[iframe-1],0);
	    xtra = '\0';
	    istop = 0;
	    printf("%c[?38h",ESC); /* open graphics window */
	    printf("%c",GS);       /* set to vector mode */
	    printf("%c%c",ESC,FF); /* clear screen */

	    for (i = 0; i < 10000; i++) {
		if (xtra != '\0') {
		    fwrite(&xtra,1,1,stdout);
		    xtra = '\0';
		}
		nb = fread(buf,1,1024,fd);
		if (nb <= 0) break;
		ifirst = 0;
		for (j = 0; j < nb; j++) {
		    if (buf[j] == '\f') {
			fwrite(&buf[ifirst],1,j-ifirst,stdout);
			fflush(stdout);
			istop = 1;
			break;
		    }
		}
		if (istop != 0) break;
		if (buf[nb] == ESC) {
		    j = j-1;
		    xtra = ESC;
		}
		fwrite(&buf[ifirst],1,j-ifirst,stdout);
	    }
	    printf("%c%c",US,CAN);
	    printf("%c%c",ESC,ETX);
	    fflush(stdout);
	}
	fclose(fd);
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
   <n>	  Goes to the specified frame number (surprise!).\n\
	  If negative, will go back the specified number of frames.\n\
 <Return> Goes to the next frame (first frame, if initial reply).\n\
\n\
", stdout);
}
