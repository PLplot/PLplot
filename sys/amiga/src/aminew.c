/* $Id$
   $Log$
   Revision 1.1  1993/01/23 06:12:40  mjl
   Preliminary work on new graphical interface (2.04-specific) for the Amiga.

*/

/*	aminew.c

	New Amiga code, most of which only runs under Amiga OS 2.0.
*/

#include "plplot.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plamiga.h"

#include <libraries/asl.h>
#include <clib/asl_protos.h>

struct Library *AslBase = NULL;

extern struct Window *PLWindow;

char filename[80];

char *
plfilereq(void)
{
    struct FileRequester *fr;
    
    if (AslBase = OpenLibrary("asl.library", 37L)) {

	if (fr = AllocAslRequestTags(ASL_FileRequest,
				     ASL_Hail,		(ULONG)"Save to file:",
				     ASL_Window,	PLWindow,
				     ASL_FuncFlags,	FILF_SAVE,
				     TAG_DONE))
	{
	    (void) AslRequest ( fr, NULL );
	    strcpy(filename, fr->rf_File);
	    FreeAslRequest (fr);
	    return (filename);
	}
    }
    return (NULL);
}
