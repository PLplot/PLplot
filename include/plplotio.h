/* $Id$
   $Log$
   Revision 1.1  1993/02/23 04:55:47  mjl
   Added to handle any data structure definitions or function prototypes that
   need access to stdio.h, so that plplot.h could remain free of this.

*/

/*
    plplotio.h

    Header file for all stdio-dependent data structures and prototypes.
    Should be included AFTER plplot.h and stdio.h.
*/

#ifndef __PLPLOTIO_H__
#define __PLPLOTIO_H__

#ifdef __cplusplus
extern "C" {
#endif

void  plgfile	(FILE *);

void  plsfile	(FILE *);

#ifdef __cplusplus
}
#endif

#endif	/* __PLPLOTIO_H__ */
