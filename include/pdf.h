/* $Id$
   $Log$
   Revision 1.2  1992/09/29 04:45:30  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:33:40  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*--------------------------------------------------------------------------*\

    pdf.h

    Copyright (C) 1992 by Maurice J. LeBrun

    Macros and prototypes for the PDF package.

    This software may be freely copied, modified and redistributed without
    fee provided that this copyright notice is preserved intact on all
    copies and modified copies. 
 
    There is no warranty or other guarantee of fitness of this software.
    It is provided solely "as is". The author(s) disclaim(s) all
    responsibility and liability with respect to this software's usage or
    its effect upon hardware or computer systems. 

\*--------------------------------------------------------------------------*/

#ifndef INCLUDED_PDF
#define INCLUDED_PDF

#include "chdr.h"

/* Error numbers */

#define PDF_ERROR		1	/* Unknown error	*/
#define PDF_FNOPEN		2	/* File not open	*/
#define PDF_FAOPEN		3	/* File already open	*/
#define PDF_BADUN		4	/* Bad unit number	*/
#define PDF_BADNBITS		5	/* Invalid # of bits	*/
#define PDF_RDERR		6	/* Read error		*/
#define PDF_WRERR		7	/* Write error		*/
#define PDF_NOTPDF		8	/* Not a valid PDF file */

/* Prototypes */

int  pdf_open 		(INT ius, char *filename);
int  pdf_creat 		(INT ius, char *filename);
int  pdf_close 		(INT ius);
int  pdf_wrflt 		(INT ius, FLOAT *f, INT len, INT nbits);
int  pdf_rdflt 		(INT ius, FLOAT *f, INT len, INT *pnbits);
int  pdf_wrint 		(INT ius, FLOAT *f, INT len, INT nbits);
int  pdf_rdint 		(INT ius, FLOAT *f, INT len, INT *pnbits);
void pdf_set		(char *option, int value);

int  read_header	(FILE *file, char *header);
int  write_header	(FILE *file, char *header);
int  write_1byte	(FILE *file, U_CHAR s);
int  read_1byte		(FILE *file, U_CHAR *ps);
int  write_2bytes	(FILE *file, U_SHORT s);
int  read_2bytes 	(FILE *file, U_SHORT *ps);
int  write_4bytes	(FILE *file, U_LONG s);
int  read_4bytes 	(FILE *file, U_LONG *ps);
int  write_ieeef	(FILE *file, float f);
int  read_ieeef		(FILE *file, float *pf);

#endif	/* INCLUDED_PDF */
