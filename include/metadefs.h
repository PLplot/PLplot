/* $Id$
   $Log$
   Revision 1.5  1993/03/06 05:03:49  mjl
   Bumped the metafile format ID, to allow a hack on old metafiles with bad
   linewidth commands (WIDTH commands in the body of the metafile are now
   ignored for metafiles version 1993a and older).  WIDTH commands prior to
   the INIT on old metafiles are still honored.

 * Revision 1.4  1993/02/27  04:47:26  mjl
 * Changed def'n of plm_wr macro to avoid a circular call chain.
 *
 * Revision 1.3  1993/02/23  04:51:08  mjl
 * Changed PLPLOT_HEADER and PLPLOT_VERSION to PLMETA_HEADER and PLMETA_VERSION
 * so that plplot could have its own distinct version number.
 *
 * Revision 1.2  1993/01/23  05:29:18  mjl
 * Added support for polylines and new color model.
 *
 * Revision 1.1  1992/05/20  21:33:39  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*
	metadefs.h
	Geoffrey Furnish
	5 May 1991
	
	This file contains definitions of constants and structures which
	are need by the PLPLOT metafile writer and renderer.

	PLMETA_HEADER holds the magic string at head of metafile.
	PLMETA_VERSION holds the version number (year & letter).
	All strings written into the file header are limited to a maximum
	of 80 characters.
*/

#define PLMETA_HEADER	"PLPLOT"
#define PLMETA_VERSION	"1993b"

/* Symbolic constants for old metafile versions (prior to 1992a).
   Now these are stored in the metafile header. */

#define PLMETA_X_OLD	10000
#define PLMETA_Y_OLD	10000

/* These numbers are supposed to be in dots/mm.  Since I am using a very
   large virtual display space, these need to be pretty big. */

#define PIXEL_RES_X_OLD		42
#define PIXEL_RES_Y_OLD		56

/* Macros to make it easier to abort on nonzero return code */
/* Can't call plexit on a write failure since that would be circular */

#define plm_wr(code) \
    if (code) { fprintf(stderr, "Unable to write to MetaFile\n"); exit(1); }

#define plm_rd(code) \
    if (code) plexit( "Unable to read from MetaFile" )

/*
   The available commands are as follows.

   *** NOTICE !!! ***
   If you change ANY of the following, you will wreck backward
   backward compatibility with old metafiles.  You may add, but do
   NOT delete !!!
*/

#define INITIALIZE	1
#define CLOSE		2
#define SWITCH_TO_TEXT	3
#define SWITCH_TO_GRAPH	4
#define CLEAR		5
#define PAGE		6
#define NEW_COLOR	7
#define NEW_WIDTH	8
#define LINE		9
#define LINETO		10
#define ESCAPE		11
#define ADVANCE		12
#define POLYLINE	13
#define NEW_COLOR0 	NEW_COLOR
#define NEW_COLOR1	14

#define END_OF_FIELD 255
