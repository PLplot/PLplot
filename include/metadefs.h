/* $Id$
   $Log$
   Revision 1.1  1992/05/20 21:33:39  furnish
   Initial checkin of the whole PLPLOT project.

*/

/*
	metadefs.h
	Geoffrey Furnish
	5 May 1991
	
	This file contains definitions of constants and structures which
	are need by the PLPLOT metafile writer and renderer.

	PLPLOT_HEADER holds the magic string at head of metafile.
	PLPLOT_VERSION holds the version number (year & letter).
	All strings written into the file header are limited to a maximum
	of 80 characters.
*/

#define PLPLOT_HEADER	"PLPLOT"
#define PLPLOT_VERSION	"1992a"

/* Symbolic constants for old metafile versions (prior to 1992a).
   Now these are stored in the metafile header. */

#define PLMETA_X_OLD	10000
#define PLMETA_Y_OLD	10000

/* These numbers are supposed to be in dots/mm.  Since I am using a very
   large virtual display space, these need to be pretty big. */

#define PIXEL_RES_X_OLD		42
#define PIXEL_RES_Y_OLD		56

/* Macros to make it easier to abort on nonzero return code */

#define plm_wr(code) \
	if (code) plexit( "Unable to write to MetaFile" )

#define plm_rd(code) \
	if (code) plexit( "Unable to read from MetaFile" )

/*
   The available commands are as follows.

   *** NOTICE !!! ***
   If you change ANY of the following, you will wreck backward
   backward compatibility with old metafiles.  You may add, but do
   NOT delete !!!
*/

#define INITIALIZE 1
#define CLOSE 2
#define SWITCH_TO_TEXT 3
#define SWITCH_TO_GRAPH 4
#define CLEAR 5
#define PAGE 6
#define NEW_COLOR 7
#define NEW_WIDTH 8
#define LINE 9
#define LINETO 10
#define ESCAPE 11
#define ADVANCE 12

#define END_OF_FIELD 255
