/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:58  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:28  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plform.c

	Controls format of floating point value in axes labels.
*/

#include "plplot.h"
#include <stdio.h>
#ifdef PLSTDC
#include <string.h>
#else
extern char *strcpy();
extern char *strcat();
#endif
#include <math.h>

static PLINT setpre, precis;

/*----------------------------------------------------------------------*\
* void plprec()
*
* Set the number of points written after the decimal point in labels.
\*----------------------------------------------------------------------*/

void
c_plprec( PLINT setp, PLINT prec )
{
    setpre = setp;
    precis = prec;
}

/*----------------------------------------------------------------------*\
* void plform()
*
* Formats a floating point value in one of the following formats 
* (i)	If scale == 0, use floating point format with "prec" places
*	after the decimal point. 
* (ii)	If scale == 1, use scientific notation with one place before 
*	the decimal point and "prec" places after.  In this case, the
*	value must be divided by 10^scale.
\*----------------------------------------------------------------------*/

void
plform(PLFLT value, PLINT scale, PLINT prec, char *result)
{
    char form[10];
    char temp[30];

    if(setpre)
	prec = precis;

    if(scale)
	value /= pow(10.,(double)scale);

    sprintf(form, "%%.%df", prec);
    sprintf(temp, form, value);
    strcpy(result, temp);
}

