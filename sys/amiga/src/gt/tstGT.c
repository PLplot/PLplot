/* $Id$
   $Log$
   Revision 1.1  1993/01/23 06:13:34  mjl
   GadTools (interface-builder) files and skeleton test code created.

*/

/*	tstGT.c

	Skeleton program for testing GadTools-built interface.
*/

#include <exec/types.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <intuition/imageclass.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <graphics/displayinfo.h>
#include <graphics/gfxbase.h>
#include <clib/exec_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>

#include "plplotGT.h"
#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
	if (! SetupScreen()) {
		if (! OpenplplotGTWindow()) {
			for (;;) {
			    Wait(1L << plplotGTWnd->UserPort->mp_SigBit);
				if (! HandleplplotGTIDCMP()) 
					break;
			};
			CloseplplotGTWindow();
		}
		CloseDownScreen();
		exit(0);
	}
	else
		exit(1);
}

/* Routines to handle the calls by the IDCMP handler. */

int plplotGTItem0( void )
{
	/* routine when (sub)item "Open" is selected. */

	printf("Selected menu option: Open\n");
	return(1);
}

int plplotGTItem1( void )
{
	/* routine when (sub)item "Save" is selected. */

	printf("Selected menu option: Save\n");
	return(1);
}

int plplotGTItem2( void )
{
	/* routine when (sub)item "ILBM (IFF)" is selected. */

	printf("Selected menu option: ILBM (IFF)\n");
	return(1);
}

int plplotGTItem3( void )
{
	/* routine when (sub)item "Bitmap" is selected. */

	printf("Selected menu option: Bitmap\n");
	return(1);
}

int plplotGTItem4( void )
{
	/* routine when (sub)item "Full page (landscape)" is selected. */

	printf("Selected menu option: Full page (landscape)\n");
	return(1);
}

int plplotGTItem5( void )
{
	/* routine when (sub)item "Full page (portrait)" is selected. */

	printf("Selected menu option: Full page (portrait)\n");
	return(1);
}

int plplotGTItem6( void )
{
	/* routine when (sub)item "About..." is selected. */

	printf("Selected menu option: About...\n");
	return(1);
}

int plplotGTItem7( void )
{
	/* routine when (sub)item "Quit" is selected. */

	printf("Selected menu option: Quit\n");
	return(0);
}

int plplotGTItem8( void )
{
	/* routine when (sub)item "Reset" is selected. */

	printf("Selected menu option: Reset\n");
	return(1);
}

int plplotGTItem9( void )
{
	/* routine when (sub)item "Workbench" is selected. */

	printf("Selected menu option: Workbench\n");
	return(1);
}

int plplotGTItem10( void )
{
	/* routine when (sub)item "Custom" is selected. */

	printf("Selected menu option: Custom\n");
	return(1);
}

int plplotGTItem11( void )
{
	/* routine when (sub)item "Screen parameters" is selected. */

	printf("Selected menu option: Screen parameters\n");
	return(1);
}

int plplotGTVanillaKey( void )
{
	/* routine for "IDCMP_VANILLAKEY". */

	printf("Keypress detected, character code = %d\n", plplotGTMsg.Code);
	return(1);
}
