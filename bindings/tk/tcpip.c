/* $Id$
 * $Log$
 * Revision 1.1  1994/01/15 17:50:09  mjl
 * Added to hold primarily socket related code.  The latter were taken from
 * the Tcl-DP distribution and modified to use binary i/o to/from the memory
 * buffer pointed to by pdfs->buffer (pdfs a PDFstrm *).
 *
*/

/* 
 * tcpip.c
 * Maurice LeBrun
 * 6-Jan-94
 *
 * Functions to handle a variety of TPC-IP related chores, in particular
 * socket i/o for data transfer to the Tcl-DP driver.  For the latter, the
 * Tcl-DP routines were modified to use binary records; copyright follows:
 *
 * Copyright 1992 Telecom Finland
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that this copyright
 * notice appears in all copies.  Telecom Finland
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 *
 * Copyright (c) 1993 The Regents of the University of California.
 * All rights reserved.
 * 
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

#ifdef TCL_DP

/* This file is meant to be compiled with non-ANSI compilers ("cc").
 * The reason for doing it this way is to ensure that the full C
 * environment of the target machine is visible (at the time of writing
 * this, parts of this code are not covered by any international
 * standard).  ANSI compilers are required to omit these extra symbols,
 * and at the moment there is no way to get them back except for by
 * vendor-specific defines (HP: _HPUX_SOURCE, AIX: _ALL_SOURCE, DGUX:
 * _DGUX_SOURCE).  This is an omission in the POSIX standard more than
 * anything else, and will probably be rectified at some point.  So for
 * now, instead of relying on a hodgepodge of vendor specific symbols I
 * forego the ANSI compiler here and go with good (bad) old "cc".
 */

#ifdef __STDC__				/* Just in case.. */
#define PLARGS(a)	a
#else
#define PLARGS(a)	()
#endif

#include <stdio.h>
#include "pdf.h"
#include <tcl.h>
#include <tk.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include <dp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/uio.h>

extern int errno;

/*
 * This is a "magic number" prepended to the beginning of the packet
 * It's used to help resync the packet machanism in the event of errors.
 */
#define PACKET_MAGIC	0x6feeddcc

/*
 * For TCP, it's possible to get a line in pieces.  In case everything we
 * want isn't there (e.g., in dp_packetReceive), we need a place to store
 * partial results when we're in non-blocking mode or peeking at the data.
 * The partial buffers below are created dynamically to store incomplete
 * data in these cases.
 */
typedef struct PartialRead {
    char *buffer;		/* Buffer of characters */
    int bufSize;		/* Size of buffer */
    int offset;			/* Offset of current character within buffer */
    struct PartialRead *next;	/* Next buffer in chain */
} PartialRead;

#define MAX_OPEN_FILES	128

static PartialRead *partial[MAX_OPEN_FILES];

static void pl_FreeReadBuffer	PLARGS((int fd));
static void pl_Unread		PLARGS((int fd, char *buffer,
					int numBytes, int copy));
static int  pl_Read		PLARGS((int fd, char *buffer,
					int numReq, int flags));

/*
 *--------------------------------------------------------------
 *
 * pl_PacketReceive --
 *
 *      This procedure is the C interface to the "dp_packetReceive"
 *      command. See the user documentation for a description.
 *
 * Results:
 *	Packet contents stored in pdfs->buffer.
 *
 * Side effects:
 *	The file descriptor passed in is read.
 *
 *--------------------------------------------------------------
 */
int
pl_PacketReceive(interp, fileHandle, peek, pdfs)
    Tcl_Interp *interp;		/* Tcl interpreter */
    char *fileHandle;
    int peek;
    PDFstrm *pdfs;
{
    FILE *filePtr;
    int numRead;
    int packetLen;
    int fd;
    int headerSize;
    int header[2];
    char *errMsg;
    int flags;

    if (Tcl_GetOpenFile(interp, fileHandle, 0, 1, &filePtr) != TCL_OK) {
      return TCL_ERROR;
    }

    fd = fileno(filePtr);

    /* 
     * Make sure this is a non-server TCP socket
     * Deleted since I don't have access to the optFlags array.
     */

    if (peek) {
	flags = MSG_PEEK;
    } else {
	flags = 0;
    }

    /*
     * Read in the header (2*sizeof(int))
     */
    headerSize = 2*sizeof(int);
    numRead = pl_Read (fd, (char *)header, headerSize, flags);

    if (numRead <= 0) {
	goto readError;
    }

    /*
     * Check for incomplete read.  If so, put it back (only if we consumed it!)
     * and return.
     */
    if (numRead < headerSize) {
	if (!peek) {
	    pl_Unread (fd, (char *)header, headerSize, 1);
	}
	Tcl_ResetResult(interp);
	return TCL_OK;
    }

    header[0] = ntohl(header[0]);
    header[1] = ntohl(header[1]);

    /*
     * Format of each packet:
     *
     *		First 4 bytes are PACKET_MAGIC.
     *		Next 4 bytes are packetLen.
     *		Next packetLen-headerSize is zero terminated string
     */
    if (header[0] != PACKET_MAGIC) {
        Tcl_AppendResult(interp, "Error reading ", fileHandle,
			 ": badly formatted packet", (char *) NULL);
	return TCL_ERROR;
    }
    packetLen = header[1] - headerSize;

    /*
     * Expand the size of the buffer, as needed.
     */

    if (header[1] > pdfs->bufmax) {
	free((void *) pdfs->buffer);
	pdfs->bufmax = header[1] + 32;
	pdfs->buffer = (unsigned char *) malloc(pdfs->bufmax);
    }

    /*
     * Read in the packet.  If it's only partially there, unread it and
     * return.  If we're peeking, we need to be careful since the header
     * is still in the queue.
     */
    if (peek) {
	numRead = pl_Read (fd, (char *) pdfs->buffer, header[1], flags);
	if (numRead <= 0) {
	    goto readError;
	}
	return TCL_OK;
    }

    /*
     * We're not peeking, so we've consumed the header (this is normal mode).
     * Read in the packet, and if it's not all there, put it back.
     */
    numRead = pl_Read (fd, (char *) pdfs->buffer, packetLen, flags);
    if (numRead <= 0) {
	goto readError;
    }

    if (numRead != packetLen) {
	pl_Unread (fd, (char *) pdfs->buffer, numRead, 1);
	pl_Unread (fd, (char *) header, headerSize, 1);
	return TCL_OK;
    }

    pdfs->bp = numRead;
    return TCL_OK;

readError:
    /*
     *
     * If we're in non-blocking mode, and this would block, return.
     * If the connection is closed (numRead == 0), don't return an
     * error message.  Otherwise, return one.
     *
     * In either case, we close the file, delete the file handler, and
     * return a null string.
     */

    if (errno == EWOULDBLOCK || errno == EAGAIN) {
	return TCL_OK;
    }

    /* Record the error before closing the file */
    if (numRead != 0) {
	errMsg = Tcl_PosixError (interp);
    } else {
	errMsg = NULL;	/* Suppresses spurious compiler warning */
    }

    /* 
     * Remove the file handler and close the file.  We want to go through
     * tcl in case the user has overridden the close procedure
     */
    Tdp_CleanupFile(interp, fileHandle, fd);
    pl_FreeReadBuffer(fd);

    Tcl_ResetResult(interp);
    if (numRead == 0) {
	return TCL_OK;
    } else {
	Tcl_AppendResult (interp, "Tdp_PacketReceive -- error reading ",
		  fileHandle, ": ", errMsg, (char *) NULL);
	return TCL_ERROR;
    }
}

/*
 *--------------------------------------------------------------
 *
 * pl_PacketSend --
 *
 *      This procedure is the C interface to the "dp_packetSend" command.
 *
 * Results:
 *	A standard tcl result.
 *
 * Side effects:
 *	The specified buffer is written to the file descriptor passed
 *	in.
 *
 *--------------------------------------------------------------
 */

int
pl_PacketSend(interp, fileHandle, pdfs)
    Tcl_Interp *interp;		/* Tcl interpreter */
    char *fileHandle;
    PDFstrm *pdfs;
{
    FILE *filePtr;
    int fd;
    int packetLen;
    int numSent;
    int header[2];
    struct iovec iov[2];
    char tmp[256];

    if (Tcl_GetOpenFile(interp, fileHandle, 1, 1, &filePtr) != TCL_OK) {
	return TCL_ERROR;
    }

    fd = fileno (filePtr);

    /* 
     * Make sure this is a non-server TCP socket
     * Deleted since I don't have access to the optFlags array.
     */

    /*
     * Format up the packet:
     *	  First 4 bytes are PACKET_MAGIC.
     *	  Next 4 bytes are packetLen.
     *	  Next packetLen-(sizeof(int)) bytes are buffer contents.
     */

    packetLen = pdfs->bp + 2 * sizeof (int);

    header[0] = htonl (PACKET_MAGIC);
    header[1] = htonl (packetLen);

    /* Set up scatter/gather vector */
    iov[0].iov_len = 2*sizeof(int);
    iov[0].iov_base = (char *)header;
    iov[1].iov_len = pdfs->bp;
    iov[1].iov_base = (char *) pdfs->buffer;

    /* Send it off, with error checking */
    numSent = writev (fd, iov, 2);
    if (numSent != packetLen) {

	if ((errno == 0) || (errno == EWOULDBLOCK || errno == EAGAIN)) {
	    /*
	     * Non-blocking I/O: return number of bytes actually sent.
	     */
	    Tcl_ResetResult(interp);
	    sprintf (tmp, "%d", numSent - 2*sizeof(int));
	    Tcl_SetResult(interp, tmp, TCL_VOLATILE);
	    return TCL_OK;
	} else if (errno == EPIPE) {
	    /*
	     * Got a broken pipe signal, which means the far end closed the
	     * connection.  Close the file, delete the file handler, and
	     * return 0 bytes sent.
	     */
	    Tdp_CleanupFile(interp, fileHandle, fd);
	    sprintf (tmp, "0");
	    Tcl_SetResult(interp, tmp, TCL_VOLATILE);
	    return TCL_OK;
	} else {
	    Tcl_AppendResult (interp, "Tdp_PacketSend -- error writing ",
			      fileHandle, ": ",
			      Tcl_PosixError (interp), (char *) NULL);
	}

	return TCL_ERROR;
    }

    /*
     * Return the number of bytes sent (minus the header).
     */
    sprintf (tmp, "%d", numSent - 2*sizeof(int));
    Tcl_SetResult(interp, tmp, TCL_VOLATILE);
    return TCL_OK;
}

/*
 *--------------------------------------------------------------
 *
 * pl_FreeReadBuffer --
 *
 *	This function is called to free up all the memory associated
 *	with a file once the file is closed.
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Any data buffered locally will be lost.
 *
 *--------------------------------------------------------------
 */

static void
pl_FreeReadBuffer(fd)
    int fd;
{
    PartialRead *readList;

    while (partial[fd] != NULL) {
	readList = partial[fd];
	partial[fd] = readList->next;
	free (readList->buffer);
	free (readList);
    }
}

/*
 *--------------------------------------------------------------
 *
 * pl_Unread --
 *
 *	This function puts data back into the read chain on a
 *	file descriptor.  It's basically an extended "ungetc".
 *
 * Results:
 *	None.
 *
 * Side effects:
 *	Subsequent calls to pl_Read on the fd will get this data.
 *
 *--------------------------------------------------------------
 */

static void
pl_Unread (fd, buffer, numBytes, copy)
    int fd;                     /* File descriptor */
    char *buffer;               /* Data to unget */
    int numBytes;               /* Number of bytes to unget */
    int copy;			/* Should we copy the data, or use this */
				/* buffer? */
{
    PartialRead *new;

    new = (PartialRead *)malloc (sizeof(PartialRead));
    if (copy) {
	new->buffer = malloc (numBytes);
	memcpy (new->buffer, buffer, numBytes);
    } else {
	new->buffer = buffer;
    }
    new->bufSize = numBytes;
    new->offset = 0;
    new->next = partial[fd];
    partial[fd] = new;
}

/*
 *--------------------------------------------------------------
 *
 * pl_Read --
 *
 *	This function impplements a "recv"-like command, but
 *	buffers partial reads.  The semantics are the same as
 *	with recv.
 *
 * Results:
 *	Number of bytes read, or -1 on error (with errno set).
 *
 * Side effects:
 *	All available data is read from the file descriptor.
 *
 *--------------------------------------------------------------
 */

static int
pl_Read (fd, buffer, numReq, flags)
    int fd;			/* File descriptor to read from */
    char *buffer;		/* Place to put the data */
    int numReq;			/* Number of bytes to get */
    int flags;			/* Flags for receive */
{
    int peek;
    PartialRead *readList;
    PartialRead *tmp;
    int numRead;
    int numToCopy;

    readList = partial[fd];

    /*
     * If there's no data left over from a previous read, then just do a recv
     * This is the common case.
     */
    if (readList == NULL) {
	numRead = recv(fd, buffer, numReq, flags);
	return numRead;
    }

    /*
     * There's data left over from a previous read.  Yank it in and
     * only call recv() if we didn't get enough data (this keeps the fd
     * readable if they only request as much data as is in the buffers).
     */
    peek = flags & MSG_PEEK;
    numRead = 0;
    while ((readList != NULL) && (numRead < numReq)) {
	numToCopy = readList->bufSize - readList->offset;
	if (numToCopy + numRead > numReq) {
	    numToCopy = numReq - numRead;
	}
	memcpy (buffer+numRead, readList->buffer+readList->offset, numToCopy);

	/*
	 * Consume the data if we're not peeking at it
	 */
	tmp = readList;
	readList = readList->next;
	if (!peek) {
	    tmp->offset += numToCopy;
	    if (tmp->offset == tmp->bufSize) {
		free (tmp->buffer);
		free (tmp);
		partial[fd] = readList;
	    }
	}
	numRead += numToCopy;
    }

    /*
     * Only call recv if we reached the end of previously read data and they
     * didn't get enough and the fd has data to be consumed.
     */
    if ((numRead < numReq) && (Tdp_FDIsReady(fd) & TCL_FILE_READABLE)) {
	numToCopy = numReq - numRead;
	numRead += recv(fd, buffer+numRead, numToCopy, flags);
    }

    return numRead;
}

/*----------------------------------------------------------------------*\
* Host_ID
*
* Tcl command -- return the IP address for the current host.  
*
* Derived from source code in "UNIX Network Programming" by W. Richard
* Stevens, Prentice Hall, 1990.
\*----------------------------------------------------------------------*/

static char *
get_inet(listptr, length)
    char **listptr;
    int length;
{
    struct in_addr	*ptr;

    while ( (ptr = (struct in_addr *) *listptr++) == NULL)
	continue;

    return inet_ntoa(*ptr);
}

int
Host_ID(clientData, interp, argc, argv)
    ClientData clientData;
    Tcl_Interp *interp;
    int argc;
    char **argv;
{
    register char		*ptr;
    register struct hostent	*hostptr;
    char			hostname[100];

    if (gethostname(hostname, 100)) {
	Tcl_AppendResult(interp, "Error -- cannot get host name", 
			 (char *) NULL);
	return TCL_ERROR;
    }

    if ( (hostptr = gethostbyname(hostname)) == NULL) {
	Tcl_AppendResult(interp, "Error -- cannot get host info for node ",
			 hostname, (char *) NULL);
	return TCL_ERROR;
    }

    Tcl_SetResult(interp,
		  get_inet(hostptr->h_addr_list, hostptr->h_length),
		  TCL_VOLATILE);

    return TCL_OK;
}

#else
int
pldummy_socket()
{
    return 0;
}

#endif	/* TCL_DP */
