/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:45  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:46:33  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:35:05  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/* long2str.c
 *
 * long <-> char converters (4/word), for system-independent
 * Fortran -> C linkage.
 *
 * Requires that a long be the same size as an integer*4.
 *
 * Might as well put all the needed temporary strings here also.
 */

char cstring1[300], cstring2[300], cstring3[300];
char cstring4[300], cstring5[300], cstring6[300];

/* Under AIX double precision (-qAUTODBL=DBLPAD), the integers seem to
   be longword-aligned.  Hence we need to include this offset. */

#ifdef DOUBLE_PREC
#ifdef AIX
#define OFFSET 1
#endif
#endif

#ifndef OFFSET
#define OFFSET 0
#endif

/*--------------------------------------------------------------------------*\
*  void long2str(istring, cstring)
*
*  long -> char converter (4/word), for system-independent
*  Fortran -> C linkage (need to circumvent endian problem).
*
*  Requires that a long be the same size as an integer.
\*--------------------------------------------------------------------------*/

void 
long2str (long *istring, char *cstring)
{
    int i, j, k, ii, lstring, mstring, nstring;
    long max_slen = 1000; 	/* eventually will pass this in */

#ifdef MSDOS
    char *c = (char *)istring;
    i=0;
    while (*c) cstring[i++] = *c++;
    cstring[i] = '\0';
#else
    j = 0;
    for (i=0;; i++) {
	lstring = istring[i];
	for (ii=0; ii<4; ii++) {
	    mstring = lstring >> 8;
	    nstring =  lstring - (mstring << 8);
	    if (j == max_slen || nstring == 0) {
		cstring[j] = '\0';
		return;
	    }
	    cstring[j] = nstring;
	    lstring = mstring;
	    j++;
	}
	i += OFFSET;
    }
#endif
}

/*--------------------------------------------------------------------------*\
*  void str2long(cstring, istring)
*
*  char -> long converter (4/word), for system-independent
*  Fortran -> C linkage (need to circumvent endian problem).
*
*  Requires that a long be the same size as an integer.
\*--------------------------------------------------------------------------*/

void 
str2long (char *cstring, long *istring)
{
    int i, j, k, ii;
    long max_ilen = 1000;	/* eventually will pass this in */

    j = 0;
    for (i=0; i<max_ilen; i++) {
	istring[i] = 0;
	for (ii=0; ii<4; ii++) {
	    if (cstring[j] == '\0') {
		istring[i+1] = 0;
		return;
	    }
	    istring[i] = istring[i] + (cstring[j] << (ii*8));
	    j++;
	}
	i += OFFSET;
    }
}
