/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:35:03  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*	string.c

	String utility routines.
*/

#include "plplot.h"
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#ifdef PLSTDC
#include <string.h>
#else
extern int strlen();
extern char *strchr();
#endif

#define PLMAXSTR	300
#define STLEN		250

static char font[] = "nris";
static char greek[] = "ABGDEZYHIKLMNCOPRSTUFXQWabgdezyhiklmncoprstufxqw";
static short symbol[PLMAXSTR];
static SCHAR xygrid[STLEN];

extern SCHAR *fntbffr;
extern short int *fntindx;
extern short int indxleng;
extern short int *fntlkup;
extern short int numberfonts, numberchars;

/*----------------------------------------------------------------------*\
* void plstr() 
* 
* Prints out a "string" at reference position with physical coordinates
* (refx,refy). The coordinates of the vectors defining the string are
* passed through the linear mapping defined by the 2 x 2 matrix xform()
* before being plotted.  The reference position is at the left-hand edge of
* the string. If base = 1, it is aligned with the baseline of the string.
* If base = 0, it is aligned with the centre of the character box. 
* 
* Note, all calculations are done in terms of millimetres. These are scaled
* as necessary before plotting the string on the page. 
\*----------------------------------------------------------------------*/

void 
plstr(base, xform, refx, refy, string)
PLINT base, refx, refy;
char *string;
PLFLT *xform;
{
    short int *symbol;
    SCHAR *xygrid;
    PLINT ch, cx, cy, i, k, length, level, penup;
    PLINT xbase, ybase, ydisp, lx, ly, style;
    PLINT oline, uline;
    PLFLT width, xorg, yorg, x, y, def, ht, dscale, scale;
    PLFLT xscl, xoff, yscl, yoff;

    width = 0.0;
    oline = 0;
    uline = 0;

    gchr(&def, &ht);
    dscale = 0.05 * ht;
    scale = dscale;
    gmp(&xscl, &xoff, &yscl, &yoff);

    /* Line style must be continuous */

    gnms(&style);
    snms(0);

    pldeco(&symbol, &length, string);
    xorg = 0.0;
    yorg = 0.0;
    level = 0;

    for (i = 0; i < length; i++) {
	ch = symbol[i];
	if (ch == -1) {
	    level = level + 1;
	    yorg = yorg + 16.0 * scale;
	    scale = dscale * pow(0.75, (double) ABS(level));
	}
	else if (ch == -2) {
	    level = level - 1;
	    scale = dscale * pow(0.75, (double) ABS(level));
	    yorg = yorg - 16.0 * scale;
	}
	else if (ch == -3)
	    xorg = xorg - width * scale;
	else if (ch == -4)
	    oline = !oline;
	else if (ch == -5)
	    uline = !uline;
	else {
	    if (plcvec(ch, &xygrid)) {
		xbase = xygrid[2];
		width = xygrid[3] - xbase;
		if (base == 0) {
		    ybase = 0;
		    ydisp = xygrid[0];
		}
		else {
		    ybase = xygrid[0];
		    ydisp = 0;
		}
		k = 4;
		penup = 1;
		while (1) {
		    cx = xygrid[k++];
		    cy = xygrid[k++];
		    if (cx == 64 && cy == 64)
			break;
		    if (cx == 64 && cy == 0)
			penup = 1;
		    else {
			x = xorg + (cx - xbase) * scale;
			y = yorg + (cy - ybase) * scale;
			lx = refx + ROUND(xscl * (xform[0] * x + xform[1] * y));
			ly = refy + ROUND(yscl * (xform[2] * x + xform[3] * y));
			if (penup != 0) {
			    movphy(lx, ly);
			    penup = 0;
			}
			else
			    draphy(lx, ly);
		    }
		}

		if (oline) {
		    x = xorg;
		    y = yorg + (30 + ydisp) * scale;
		    lx = refx + ROUND(xscl * (xform[0] * x + xform[1] * y));
		    ly = refy + ROUND(yscl * (xform[2] * x + xform[3] * y));
		    movphy(lx, ly);
		    x = xorg + width * scale;
		    lx = refx + ROUND(xscl * (xform[0] * x + xform[1] * y));
		    ly = refy + ROUND(yscl * (xform[2] * x + xform[3] * y));
		    draphy(lx, ly);
		}
		if (uline) {
		    x = xorg;
		    y = yorg + (-5 + ydisp) * scale;
		    lx = refx + ROUND(xscl * (xform[0] * x + xform[1] * y));
		    ly = refy + ROUND(yscl * (xform[2] * x + xform[3] * y));
		    movphy(lx, ly);
		    x = xorg + width * scale;
		    lx = refx + ROUND(xscl * (xform[0] * x + xform[1] * y));
		    ly = refy + ROUND(yscl * (xform[2] * x + xform[3] * y));
		    draphy(lx, ly);
		}
		xorg = xorg + width * scale;
	    }
	}
    }
    snms(style);
}

/*----------------------------------------------------------------------*\
* PLFLT plstrl() 
* 
* Computes the length of a string in mm, including escape sequences.
\*----------------------------------------------------------------------*/

PLFLT 
plstrl(string)
char *string;
{
    short int *symbol;
    SCHAR *xygrid;
    PLINT ch, i, length, level;
    PLFLT width, xorg, dscale, scale, def, ht;
    PLFLT xscl, xoff, yscl, yoff;

    width = 0.0;
    gchr(&def, &ht);
    dscale = 0.05 * ht;
    scale = dscale;
    gmp(&xscl, &xoff, &yscl, &yoff);

    pldeco(&symbol, &length, string);
    xorg = 0.0;
    level = 0;

    for (i = 0; i < length; i++) {
	ch = symbol[i];
	if (ch == -1) {
	    level = level + 1;
	    scale = dscale * pow(0.75, (double) ABS(level));
	}
	else if (ch == -2) {
	    level = level - 1;
	    scale = dscale * pow(0.75, (double) ABS(level));
	}
	else if (ch == -3)
	    xorg = xorg - width * scale;
	else if (ch == -4 || ch == -5);
	else {
	    if (plcvec(ch, &xygrid)) {
		width = xygrid[3] - xygrid[2];
		xorg = xorg + width * scale;
	    }
	}
    }
    return ((PLFLT) xorg);
}

/*----------------------------------------------------------------------*\
* PLINT plcvec()
*
* Gets the character digitisation of Hershey table entry "char".
* Returns 1 if there is a valid entry.
\*----------------------------------------------------------------------*/

PLINT 
plcvec(ch, xygr)
PLINT ch;
SCHAR **xygr;
{
    PLINT k, ib;
    SCHAR x, y;

    ch--;
    if (ch < 0 || ch >= indxleng)
	return ((PLINT) 0);
    ib = fntindx[ch] - 2;
    if (ib == -2)
	return ((PLINT) 0);

    k = 0;
    do {
	ib++;
	x = fntbffr[2 * ib];
	y = fntbffr[2 * ib + 1];
	xygrid[k++] = x;
	xygrid[k++] = y;
    } while ((x != 64 || y != 64) && k <= (STLEN - 2));

    *xygr = xygrid;
    return ((PLINT) 1);
}

/*----------------------------------------------------------------------*\
* void pldeco() 
* 
* Decode a character string, and return an array of float integer symbol
* numbers. This routine is responsible for interpreting all escape
* sequences. At present the following escape sequences are defined
* (the letter following the # may be either upper or lower case):
*
* #u       :      up one level (returns -1)
* #d       :      down one level (returns -2)
* #b       :      backspace (returns -3)
* #+       :      toggles overline mode (returns -4)
* #-       :      toggles underline mode (returns -5)
* ##       :      #
* #gx      :      greek letter corresponding to roman letter x
* #fn      :      switch to Normal font
* #fr      :      switch to Roman font
* #fi      :      switch to Italic font
* #fs      :      switch to Script font
* #(nnn)   :      Hershey symbol number nnn (any number of digits)
\*----------------------------------------------------------------------*/

void 
pldeco(sym, length, text)
PLINT *length;
short int **sym;
char *text;
{
    PLINT ch, icol, ifont, ig, j, lentxt;
    char test;

    /* Initialize parameters. */
    lentxt = strlen(text);
    *length = 0;
    *sym = symbol;
    gatt(&ifont, &icol);
    if (ifont > numberfonts)
	ifont = 1;

    /* Get next character; treat non-printing characters as spaces. */
    j = 0;
    while (j < lentxt) {
	if (*length >= PLMAXSTR)
	    return;
	test = text[j++];
	ch = test;
	if (ch < 0 || ch > 175)
	    ch = 32;

	/* Test for escape sequence (#) */
	if (ch == '#' && (lentxt - j) >= 1) {
	    test = text[j++];
	    if (test == '#')
		symbol[(*length)++] = *(fntlkup + (ifont - 1) * numberchars + ch);

	    else if (test == 'u' || test == 'U')
		symbol[(*length)++] = -1;

	    else if (test == 'd' || test == 'D')
		symbol[(*length)++] = -2;

	    else if (test == 'b' || test == 'B')
		symbol[(*length)++] = -3;

	    else if (test == '+')
		symbol[(*length)++] = -4;

	    else if (test == '-')
		symbol[(*length)++] = -5;

	    else if (test == '(') {
		symbol[*length] = 0;
		while ('0' <= text[j] && text[j] <= '9') {
		    symbol[*length] = symbol[*length] * 10 + text[j] - '0';
		    j++;
		}
		(*length)++;
		if (text[j] == ')')
		    j++;
	    }
	    else if (test == 'f' || test == 'F') {
		test = text[j++];
		ifont = strpos(font, isupper(test)?tolower(test):test) + 1;
		if (ifont == 0 || ifont > numberfonts)
		    ifont = 1;
	    }
	    else if (test == 'g' || test == 'G') {
		test = text[j++];
		ig = strpos(greek, test) + 1;
		symbol[(*length)++] = *(fntlkup + (ifont - 1) * numberchars + 127 + ig);
	    } else {
		;
	    }
	}
	else
	    /* Decode character. */
	    symbol[(*length)++] = *(fntlkup + (ifont - 1) * numberchars + ch);
    }
}

/*----------------------------------------------------------------------*\
* PLINT strpos()
*
* Searches string str for first occurence of character chr.  If found 
* the position of the character in the string is returned (the first  
* character has position 0).  If the character is not found a -1 is 
* returned. 
\*----------------------------------------------------------------------*/

PLINT 
strpos(str, chr)
char *str;
int chr;
{
    char *temp;

    if (temp = strchr(str, chr))
	return ((PLINT) (temp - str));
    else
	return ((PLINT) - 1);
}

/*----------------------------------------------------------------------*\
* PLINT stindex()
*
* Similar to strpos, but searches for occurence of string str2.
\*----------------------------------------------------------------------*/

PLINT 
stindex(str1, str2)
char *str1, *str2;
{
    PLINT base;
    PLINT str1ind;
    PLINT str2ind;

    for (base = 0; *(str1 + base) != '\0'; base++) {
	for (str1ind = base, str2ind = 0; *(str2 + str2ind) != '\0' &&
	     *(str2 + str2ind) == *(str1 + str1ind); str1ind++, str2ind++);	/* no body */

	if (*(str2 + str2ind) == '\0')
	    return ((PLINT) base);
    }
    return ((PLINT) - 1);	/* search failed */
}

/*----------------------------------------------------------------------*\
* PLINT stsearch()
*
* Searches string str for character chr (case insensitive).
\*----------------------------------------------------------------------*/

PLINT 
stsearch(str, chr)
char *str;
int chr;
{
    if (strchr(str, chr))
	return ((PLINT) 1);
    else if (strchr(str, toupper(chr)))
	return ((PLINT) 1);
    else
	return ((PLINT) 0);
}
