/* $Id$
 * $Log$
 * Revision 1.11  1995/09/22 16:03:26  mjl
 * Name changes to members of the PLiodev structure to reduce the chance
 * of collisions with possible new C++ keywords.
 *
 * Revision 1.10  1995/09/18  20:13:34  furnish
 * Crazy hack needed to allow inclusion of this file in C++ programs.
 * Really the structure member name should be changed, and all clients of
 * this struct updated.
 *
 * Revision 1.9  1994/04/08  12:10:08  mjl
 * Moved unsigned type defines to this header file.
 *
 * Revision 1.8  1994/03/23  06:58:11  mjl
 * Minor change to file wrapper to make consistent with rest of package.
 *
 * Revision 1.7  1994/02/07  23:02:38  mjl
 * PLiodev struct defined.
 *
 * Revision 1.6  1994/01/15  17:33:16  mjl
 * Added typedef of PDFstrm and wrapped function prototypes in a macro so
 * the header can be read without problems by K&R compilers (cc).
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

#ifndef __PDF_H__
#define __PDF_H__

/* Some unsigned types */

#ifndef U_CHAR
#define U_CHAR unsigned char
#endif

#ifndef U_SHORT
#define U_SHORT unsigned short
#endif

#ifndef U_INT
#define U_INT unsigned int
#endif

#ifndef U_LONG
#define U_LONG unsigned long
#endif

/* PDFstrm definition */
/* The low level PDF i/o routines use the transfer method appropriate for */
/* the first non-null type below */

typedef struct {
    FILE *file;				/* Filesystem */
    unsigned char *buffer;		/* Memory buffer */
    long bp, bufmax;			/* Buffer pointer and max size */
} PDFstrm;

/* Info for the i/o device.  Only used by Tcl/TK/dp drivers for now */

typedef struct {
    int   fd;				/* I/O device file descriptor */
    FILE  *file;			/* File handle */
    char  *fileName;			/* Fifo or socket name (if needed) */
    char  *fileHandle;			/* Handle for use from interpreter */
    int   type;				/* Communication channel type */
    char  *typeName;			/* As above, but in string form */
} PLiodev;

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
/* Use a wrapper for the prototypes for use from K&R C */

PDFstrm *pdf_fopen	PLARGS((char *fileName, char *mode));
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

#endif	/* __PDF_H__ */
