/* $Id$
 * $Log$
 * Revision 1.1  1995/06/01 21:30:26  mjl
 * Header file for IPC function prototypes.
 *
 */

/* 
 * tcpip.h
 * Maurice LeBrun
 * 31-May-95
 *
 * Declarations for IP utility routines.
 * Put here so as not to clutter up user codes.
 */

#ifndef __TCPIP_H__
#define __TCPIP_H__

#include "pdf.h"
#include <tcl.h>

/* Modified version of the "Tdp_PacketSend" command. */

int
pl_PacketSend		PLARGS((Tcl_Interp *interp, PLiodev *iodev,
				PDFstrm *pdfs));

/* Modified version of the "Tdp_PacketReceive" command. */

int
pl_PacketReceive	PLARGS((Tcl_Interp *interp, PLiodev *iodev,
				PDFstrm *pdfs));

#endif	/* __TCPIP_H__ */
