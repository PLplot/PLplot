/* $Id$
   $Log$
   Revision 1.4  1992/10/22 17:04:55  mjl
   Fixed warnings, errors generated when compling with HP C++.

 * Revision 1.3  1992/09/30  18:24:56  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:44:46  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:32:40  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*--------------------------------------------------------------------------*\

    pdf_utils.c

    Copyright (C) 1992 by Maurice J. LeBrun

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies. 
 
    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems. 

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

    These functions do some of the low-level work required for the PDF
    (portable data format) package.  Warning: the full PDF package is not
    yet ready for release, and these routines may change without notice.

\*--------------------------------------------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "plplot.h"
#include "pdf.h"

static int foo;
static int debug=0;

static void print_ieeef( void *, void * );

/*--------------------------------------------------------------------------*\
*  void pdf_set (string, value)
*
*  Set an option.  Pretty sparse right now but you never know.
\*--------------------------------------------------------------------------*/

void 
pdf_set (char *option, int value)
{
    if (!strcmp(option, "debug"))
	debug = value;
}

/*----------------------------------------------------------------------*\
* write_header()
*
* Writes a header string to FILE *file.
* Input string must be NULL-terminated.
* The written string is terminated by a new-line, not a NULL.
* This is done so you can type e.g. "% strings <file> | head" and
* have it be easy to read.
\*----------------------------------------------------------------------*/

int
write_header( FILE *file, char *header )
{
    int i;

    for (i=0; i<79; i++) { 
	if (header[i] == '\0') break;
	if (putc (header[i], file) == EOF)
	    return(PDF_WRERR);
    }
    if (putc ('\n', file) == EOF)
	return(PDF_WRERR);

    return(0);
}

/*----------------------------------------------------------------------*\
*  int read_header (file, header)
*
*  Reads a newline-terminated header string from FILE *file, and converts
*  to a usual NULL-terminated string.  80 chars maximum assumed.
\*----------------------------------------------------------------------*/

int
read_header( FILE *file, char *header )
{
    int i;

    for (i=0; i<79; i++) { 
	if ((header[i] = getc (file)) == EOF) 
	    return(PDF_RDERR);

	if (header[i] == '\n') break;
    }
    header[i] = '\0';		/* NULL terminate */
    return(0);
}

/*----------------------------------------------------------------------*\
*  int write_1byte ()
*
*  Writes a U_CHAR as a single byte.
\*----------------------------------------------------------------------*/

int 
write_1byte (FILE *file, U_CHAR s)
{
    U_CHAR x[1];

    x[0] = s;
    if ( !fwrite(x, 1, 1, file))
	return (PDF_WRERR);

    return (0);
}

/*----------------------------------------------------------------------*\
*  int read_1byte ()
*
*  Reads a single byte, storing into a U_CHAR.
\*----------------------------------------------------------------------*/

int 
read_1byte ( FILE *file, U_CHAR *ps )
{
    U_CHAR x[1];

    if ( !fread (x, 1, 1, file) ) 
	return (PDF_RDERR);

    *ps =  ((U_CHAR) x[0]);
    return (0);
}

/*----------------------------------------------------------------------*\
* write_2bytes()
*
* Writes a U_SHORT as two single bytes, low end first.
\*----------------------------------------------------------------------*/

int
write_2bytes(FILE *file, U_SHORT s)
{
    U_SHORT lo, hi;
    U_CHAR x[2];

    hi = s >> 8;
    lo = s - (hi << 8);
    x[0] = (U_CHAR) lo;
    x[1] = (U_CHAR) hi;

    if ( !fwrite (x, 1, 2, file) ) 
	return (PDF_WRERR);

    return (0);
}

/*----------------------------------------------------------------------*\
*  read_2bytes()
*
*  Reads a U_SHORT from two single bytes, low end first.
\*----------------------------------------------------------------------*/

int
read_2bytes( FILE *file, U_SHORT *ps )
{
    U_CHAR x[2];

    if ( !fread (x, 1, 2, file) ) 
	return (PDF_RDERR);

    *ps = (U_SHORT) x[0] + ((U_SHORT) x[1] << 8);
    return(0);
}

/*----------------------------------------------------------------------*\
*  write_4bytes ()
*
*  Writes an unsigned long as four single bytes, low end first.
\*----------------------------------------------------------------------*/

int
write_4bytes ( FILE *file, U_LONG s )
{
    U_LONG lo, hi;
    U_CHAR x[4];

    hi = s >> 8;
    lo = s - (hi << 8);
    x[0] = (U_CHAR) lo;

    s = hi;
    hi = s >> 8;
    lo = s - (hi << 8);
    x[1] = (U_CHAR) lo;

    s = hi;
    hi = s >> 8;
    lo = s - (hi << 8);
    x[2] = (U_CHAR) lo;

    s = hi;
    hi = s >> 8;
    lo = s - (hi << 8);
    x[3] = (U_CHAR) lo;

    if ( !fwrite (x, 1, 4, file) ) 
	return (PDF_WRERR);

    return (0);
}

/*----------------------------------------------------------------------*\
*  read_4bytes ()
*
*  Reads an unsigned long from 4 single bytes, low end first.
\*----------------------------------------------------------------------*/

int
read_4bytes ( FILE *file, U_LONG *ps )
{
    U_CHAR x[4];

    if ( !fread (x, 1, 4, file) ) 
	return (PDF_RDERR);

    *ps =  ((U_LONG) x[0])
	 + ((U_LONG) x[1] << 8)
	 + ((U_LONG) x[2] << 16)
	 + ((U_LONG) x[3] << 24);

    return (0);
}

/*----------------------------------------------------------------------*\
Here is the IEEE floating point specification in both 32 bit and 64 bit
precisions, from page 9 of "IEEE Standard for Binary Floating-Point
Arithmetic", copyright 1985, IEEE Std 754-1985:


                            Single Format

msb means most significant bit
lsb means least significant bit

  1         8                                23
_____________________________________________________________________
|   |                |                                              |
| s |       e        |                        f                     |
|___|________________|______________________________________________|
     msb          lsb msb                                        lsb



                            Double Format

msb means most significant bit
lsb means least significant bit

  1        11                                52
_____________________________________________________________________
|   |                |                                              |
| s |       e        |                        f                     |
|___|________________|______________________________________________|
     msb          lsb msb                                        lsb


(Thanks to: Andy Mai (mai@ncar.ucar.edu))


According to "inmos: Transputer instruction set" the IEEE standard specifies
the floating format as:

       s exp frac

Where: s = sign bit  (1 bit)
       exp = exponent (8 bits for 32 bit float / 11 bits for 64 bit float)
       frac = fraction (23 bits for 32 bit float / 52 bits for 64 bit float)

value of (s exp frac) = (-1)^s * 1.frac * 2^(exp-bias) ; if exp not 0
                        (-1)^s * 0.frac * 2^(1-bias) ; if exp = 0

where bias = 127 for 32 bit float
      bias = 1023 for 64 bit float

(Thanks to: Tom Bjorkholm(TBJORKHOLM@abo.fi))

\*----------------------------------------------------------------------*/

/*----------------------------------------------------------------------*\
*  int write_ieeef ()
*
*  Writes a float in IEEE single precision (32 bit) format.
\*----------------------------------------------------------------------*/

int
write_ieeef (FILE *file, float f)
{
    double fdbl, fmant, f_new;
    float  fsgl, f_tmp;
    int    istat, exp, e_new, e_off, bias = 127;
    U_LONG value, s_ieee, e_ieee, f_ieee;

    if (f == 0.0) {
	value = 0;
	return (write_4bytes (file, value));
    }
    fsgl = fdbl = f;
    fmant = frexp (fdbl, &exp);

    if (fmant < 0)
	s_ieee = 1;
    else
	s_ieee = 0;

    fmant = fabs(fmant);
    f_new = 2 * fmant;
    e_new = exp - 1;

    if (e_new < 1-bias) {
	e_off = e_new - (1-bias);
	e_ieee = 0;
	f_tmp = f_new * pow((double) 2.0, (double) e_off);
    }
    else {
	e_ieee = e_new + bias;
	f_tmp = f_new - 1;
    }
    f_ieee = f_tmp * 8388608;		/* multiply by 2^23 */

    if (e_ieee > 255) {
	if (debug)
	    printf("Warning -- overflow in write_ieeef()\n");
	e_ieee = 255;
    }
    else if (e_ieee < 0) {
	if (debug)
	    printf("Impossible -- biased exponent < 0 in write_ieef()\n");
	e_ieee = 0;
    }

    s_ieee = s_ieee << 31;
    e_ieee = e_ieee << 23;

    value = s_ieee | e_ieee | f_ieee;

    if (istat = write_4bytes (file, value))
	return (istat);

    if (debug) {
/*
	if (foo == 0) {
	    printf("Size of float  is: %d\n", sizeof(float));
	    printf("Size of double is: %d\n", sizeof(double));
	    printf("Size of short  is: %d\n", sizeof(short));
	    printf("Size of int    is: %d\n", sizeof(int));
	    printf("Size of long   is: %d\n", sizeof(long));
	    foo = 1;
	}
*/
	printf("Float value (written):       %g\n", fsgl);
	print_ieeef(&fsgl, &value); 
    }

    return (0);
}

/*----------------------------------------------------------------------*\
*  int read_ieeef ()
*
*  Reads a float from a IEEE single precision (32 bit) format.
\*----------------------------------------------------------------------*/

int
read_ieeef( FILE *file, float *pf )
{
    double f_new, f_tmp;
    float  fsgl;
    int    istat, exp, bias = 127;
    U_LONG value, s_ieee, e_ieee, f_ieee;

    if (istat = read_4bytes (file, &value))
	return (istat);

    s_ieee = (value & 0x80000000) >> 31;
    e_ieee = (value & 0x7F800000) >> 23;
    f_ieee = (value & 0x007FFFFF);

    f_tmp = (double) f_ieee / 8388608.0;	/* divide by 2^23 */

    if (e_ieee == 0) {
	exp = 1 - bias;
	f_new = f_tmp;
    }
    else {
	exp = (int) e_ieee - bias;
	f_new = 1.0 + f_tmp;
    }

    fsgl = f_new * pow(2.0, (double) exp);
    if (s_ieee == 1)
	fsgl = -fsgl;

    *pf = fsgl;

    if (debug) {
	printf("Float value (read):          %g\n", fsgl);
	print_ieeef(&fsgl, &value); 
    }

    return(0);
}

/*----------------------------------------------------------------------*\
*  print_ieeef ()
*
*  Prints binary representation for numbers pointed to by arguments.
*  The first argument is the original float, the second is the
*  IEEE representation.  They should be the same on any machine that
*  uses IEEE floats.
\*----------------------------------------------------------------------*/

static void 
print_ieeef ( void *vx, void *vy)
{
    int i;
    U_LONG f, *x = (U_LONG *) vx, *y = (U_LONG *) vy;
    char bitrep[33];

    bitrep[32] = '\0';

    f = *x;
    for (i=0; i<32; i++) {
	if (f & 1)
	    bitrep[32-i-1] = '1';
	else
	    bitrep[32-i-1] = '0';
	f = f >> 1;
    }
    printf("Binary representation:      ");
    printf("%s\n", bitrep);

    f = *y;
    for (i=0; i<32; i++) {
	if (f & 1)
	    bitrep[32-i-1] = '1';
	else
	    bitrep[32-i-1] = '0';
	f = f >> 1;
    }
    printf("Converted representation:   ");
    printf("%s\n\n", bitrep);

    return;
}
