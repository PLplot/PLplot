/* $Id$
   $Log$
   Revision 1.2  1992/10/12 17:11:26  mjl
   Amiga-specific mods, including ANSI-fication.

 * Revision 1.1  1992/05/20  21:35:27  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

#include "plplot.h"
#include <stdio.h>
#include "plamiga.h"

struct PLPrefs PLCurPrefs;

static struct PLPrefs PLDefPrefs =
{
    PLCUST | PLBUFF,
    PLLACE | PLHIRES,
    20, 20,
    300, 200,
    0, 0,			/* Let ScreenChange set width and height for custom screen */
    0, 0,
    2,
    0xCCC, 0x000, 0x00D, 0x080, 0xF00, 0xEE0, 0xE90, 0xF0F,
    0x0BE, 0x090, 0x999, 0xBBB, 0xF99, 0x909, 0x0DB, 0xDDD
};

void 
GetPLDefs(void)
{
    FILE *PrefsFile;

    /* Read user preferences from s:PlPlot/PlPlot.def if it exists,
       otherwise use default preferences */

    if ((PrefsFile = fopen("s:PlPlot.def", "r")) != NULL) {
	(void) fread((char *) &PLDefPrefs, sizeof(struct PLPrefs), 1, PrefsFile);
	(void) fclose(PrefsFile);
    }
    PLCurPrefs = PLDefPrefs;
}

void 
SetPLDefs(void)
{
    FILE *PrefsFile;

    if ((PrefsFile = fopen("s:PlPlot.def", "w")) != NULL) {
	(void) fwrite((char *) &PLCurPrefs, sizeof(struct PLPrefs), 1, PrefsFile);
	(void) fclose(PrefsFile);
    }
    PLDefPrefs = PLCurPrefs;
}

void 
RestorePrefs(void)
{
    PLCurPrefs = PLDefPrefs;
}
