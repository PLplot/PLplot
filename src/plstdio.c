/* $Id$

   Standardized I/O handler for PLplot.

   Copyright (C) 2006  Jim Dishaw
   Copyright (C) 2006  Hazen Babcock

   This file is part of PLplot.

   PLplot is free software; you can redistribute it and/or modify
   it under the terms of the GNU Library General Public License as published
   by the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   PLplot is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with PLplot; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

*/

#define NEED_PLDEBUG
#include "plplotP.h"

/* 
 * plio_write()
 *
 * Writes the contents of buf to stream.  Handles any I/O error conditions
 * so that the caller can "fire and forget."
 */
 
void 
plio_fwrite(void *buf, size_t size, size_t nmemb, FILE *stream)
{
  size_t bytes;

  dbug_enter("plio_fwrite");

  /* Exit if there is nothing to write */
  if(size == 0 || nmemb == 0) return;

  /* Clear the error flag for this steam */
  clearerr(stream);

  bytes = fwrite(buf, size, nmemb, stream);
  
  if(ferror(stream)) {
    /* Perhaps we can add a flag (global or per output stream)
       in order to decide if we should abort or warn.  I think
       I/O errors should generate an abort  */
    plabort("Error writing to file");
  }
}

/*
 * plio_read()
 *
 * Read from stream into buf.  Like plio_write(), this function will 
 * handle any I/O error conditions.
 */

void
plio_fread(void *buf, size_t size, size_t nmemb, FILE *stream)
{
  size_t bytes;

  dbug_enter("plio_fread");

  /* If the buffer has a size of zero, we should complain */
  if(size == 0 || nmemb == 0) {
    plwarn("Zero length buffer size in plio_read, returning");    
    return;
  }

  /* Clear the error flag for this steam */
  clearerr(stream);

  bytes = fread(buf, size, nmemb, stream);

  if(ferror(stream)) {
    /* The read resulted in an error */
    plabort("Error reading from file");
  }
}

/*
 * plio_fgets()
 *
 * Read from stream into buf.  This version of fgets is designed for the occasions
 * where the caller wants to ignore the return value.  
 *
 * NOTE: If one is reading from a file until an EOF condition, fgets() is better suited 
 * than this function, i.e.
 *
 *     while(fgets(buf, size, fp) != NULL) { ... do some stuff ... }
 *
 * rather than
 *
 *     while(!feof(fp)) { plio_fgets(buf, size, fp);  ... do some stuff ... }
 *
 * which would require checking for an empty buffer.
 */

void
plio_fgets(char *buf, int size, FILE *stream)
{
  size_t bytes;
  char *s;

  dbug_enter("plio_fgets");

  /* If the buffer has a size of zero, we should complain */
  if(size == 0) {
    plwarn("Zero length buffer size in plio_fgets, returning");    
    return;
  }

  /* Clear the error flag for this steam */
  clearerr(stream);

  s = fgets(buf, size, stream);

  if(s == NULL && ferror(stream)) {
    /* The read resulted in an error */
    plabort("Error reading from file");
  }
}
