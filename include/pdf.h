/* $Id$
   $Log$
   Revision 1.6  1994/01/15 17:33:16  mjl
   Added typedef of PDFstrm and wrapped function prototypes in a macro so
   the header can be read without problems by K&R compilers (cc).

 * Revision 1.5  1993/07/02  07:21:41  mjl
 * All PDF routines now start with "pdf_".
 *
 * Revision 1.4  1993/01/23  05:31:46  mjl
 * Added prototypes for functions that support polyline read/writes.
 *
 * Revision 1.3  1992/09/30  18:25:33  furnish
 * Massive cleanup to irradicate garbage code.  Almost everything is now
 * prototyped correctly.  Builds on HPUX, SUNOS (gcc), AIX, and UNICOS.
 *
 * Revision 1.2  1992/09/29  04:45:30  furnish
 * Massive clean up effort to remove support for garbage compilers (K&R).
 *
 * Revision 1.1  1992/05/20  21:33:40  furnish
 * Initial checkin of the whole PLPLOT project.
*/

/*
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
*/

#ifndef _PDF_INCLUDED
#define _PDF_INCLUDED

/* PDFstrm definition */
/* The low level routines use the transfer method appropriate for the */
/* first non-null type below */

typedef struct {
    FILE *file;				/* Filesystem */
    unsigned char *buffer;		/* Memory buffer */
    long bp, bufmax;			/* Buffer pointer and max size */
} PDFstrm;

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
/* Use a wrapper for the prototypes for use from cc */

PDFstrm *pdf_fopen	PLARGS((char *filename, char *mode));
PDFstrm *pdf_bopen	PLARGS((U_CHAR *buffer, long bufmax));
PDFstrm *pdf_finit	PLARGS((FILE *file));
int  pdf_close		PLARGS((PDFstrm *pdfs));

int  pdf_putc		PLARGS((int c, PDFstrm *pdfs));
int  pdf_getc		PLARGS((PDFstrm *pdfs));
int  pdf_ungetc		PLARGS((int c, PDFstrm *pdfs));

int  pdf_rd_header	PLARGS((PDFstrm *pdfs, char *header));
int  pdf_wr_header	PLARGS((PDFstrm *pdfs, char *header));
int  pdf_wr_1byte	PLARGS((PDFstrm *pdfs, U_CHAR s));
int  pdf_rd_1byte	PLARGS((PDFstrm *pdfs, U_CHAR *ps));
int  pdf_wr_2bytes	PLARGS((PDFstrm *pdfs, U_SHORT s));
int  pdf_rd_2bytes 	PLARGS((PDFstrm *pdfs, U_SHORT *ps));
int  pdf_wr_2nbytes	PLARGS((PDFstrm *pdfs, U_SHORT *s, PLINT n));
int  pdf_rd_2nbytes 	PLARGS((PDFstrm *pdfs, U_SHORT *s, PLINT n));
int  pdf_wr_4bytes	PLARGS((PDFstrm *pdfs, U_LONG s));
int  pdf_rd_4bytes 	PLARGS((PDFstrm *pdfs, U_LONG *ps));
int  pdf_wr_ieeef	PLARGS((PDFstrm *pdfs, float f));
int  pdf_rd_ieeef	PLARGS((PDFstrm *pdfs, float *pf));

#endif	/* _PDF_INCLUDED */
