/* $Id$
 *
 *                  Support routines for freetype font engine
 *
 *  This file contains a series of support routines for drivers interested
 *  in using freetype rendered fonts instead of plplot plotter fonts.
 *  Freetype supports a gerth of font formats including TrueType, OpenType,
 *  Adobe Type1, Type42 etc... the list seems almost endless. Any bitmap
 *  driver should be able to use any of these freetype fonts from plplot if
 *  these routines are properly initialised.
 *
 *  Freetype support is not intended to be a "feature" of the common API,
 *  but is  implemented as a driver-specific optional extra invoked via the
 *  -drvopt command line toggle. It is intended to be used in the context of
 *  "PLESC_HAS_TEXT" for any bitmap drivers without native font support.
 *  Implementing freetype in this manner minimise changes to the overall
 *  API. Because of this approach, there is not a "wealth" of font options
 *  available to the programmer. You can not do anything you can't do for a
 *  normal freetype plotter font like boldface. You can do most of the
 *  things that you can do with a plotter font however, like greek
 *  characters superscripting, and selecting one of the four "pre-defined"
 *  plplot font types. At present underlining and overlining are not
 *  supported.
 *
 *  To give the user some level of control over the fonts that are used,
 *  environmental variables can be set to over-ride the definitions used by
 *  the five default plplot fonts.
 *
 *  The exact syntax for evoking freetype fonts is dependant on each
 *  driver, but for the GD and GNUSVGA drivers I have followed the syntax of
 *  the PS driver and use the command-line switch of "-drvopt text" to
 *  activate the feature, and suggest other programmers do the same for
 *  commonality.
 *
 *  Both anti-aliased and monochrome font rendering is supported by these
 *  routines. How these are evoked depends on the programmer, but with the
 *  GD and GNUSVGA driver families I have used the command-line switch
 *  "-drvopt smooth" to activate the feature; but, considering you also need
 *  to turn freetype on, it would probably really be more like "-drvopt
 *  text,smooth".
 *
 */

#include <unistd.h>

#include "plDevs.h"
#include "plplotP.h"
#include "drivers.h"
#ifdef HAVE_FREETYPE
#include "plfreetype.h"

/*              TOP LEVEL DEFINES       */

/*  Freetype lets you set the text size absolutely. It also takes into
 *  account the DPI when doing so. So does plplot. Why, then, is it that the
 *  size of the text drawn by plplot is bigger than the text drawn by
 *  freetype when given IDENTICAL parameters ? Perhaps I am missing
 *  something somewhere, but to fix this up we use TEXT_SCALING_FACTOR to
 *  set a scaling factor to try and square things up a bit.
 */

#define TEXT_SCALING_FACTOR .7

/* default size of temporary text buffer */
/* If we wanted to be fancy we could add sizing, but this should be big enough */

#define NTEXT_ALLOC 1024

/*              FUNCTION PROTOTYPES    */

/*  Public prototypes, generally available to the API  */

void plD_FreeType_init(PLStream *pls);
void plD_render_freetype_text (PLStream *pls, EscText *args);
void plD_FreeType_Destroy(PLStream *pls);
void pl_set_extended_cmap0(PLStream *pls, int ncol0_width, int ncol0_org);

/*  Private prototypes for use in this file only */

static void FT_WriteStr(PLStream *pls,const char *text, int x, int y);
static void FT_StrX_Y(PLStream *pls,const char *text,int *xx, int *yy);
static void FT_PlotChar(PLStream *pls,FT_Data *FT, FT_GlyphSlot  slot, int x, int y, short colour );
static void FT_SetFace( PLStream *pls, int fnt );
static PLFLT CalculateIncrement( int bg, int fg, int levels);

/*----------------------------------------------------------------------*\
 * FT_StrX_Y()
 *
 * Returns the dimensions of the text box. It does this by fully parsing
 * the supplied text through the rendering engine. It does everything
 * but draw the text. This seems, to me, the easiest and most accurate
 * way of determining the text's dimensions. If/when caching is added,
 * the CPU hit for this "double processing" will be minimal.
\*----------------------------------------------------------------------*/

void
FT_StrX_Y(PLStream *pls, const char *text, int *xx, int *yy)
{
    FT_Data *FT=(FT_Data *)pls->FT;
    short len=strlen(text);
    short i=0;
    FT_Vector  akerning;
    int x=0,y=0;
    char esc;

    plgesc(&esc);

/*
 * Things seems to work better with this line than without it;
 * I guess because there is no vertical kerning or advancement for most
 * non-transformed fonts, so we need to define *something* for the y height,
 * and this is the best thing I could think of.
 */

    y -= FT->face->size->metrics.height;

/* walk through the text character by character */
    for (i=0;i<len;i++) {
	if ((text[i]==esc)&&(text[i-1]!=esc)) {
	    if (text[i+1]==esc) continue;

	    switch(text[i+1]) {
	    case 'f':  /* Change Font */
	    case 'F':
                switch (text[i+2]) {
		case 'r':
		case 'R':
		    FT_SetFace( pls ,2);
		    break;
		case 'i':
		case 'I':
		    FT_SetFace( pls ,3);
		    break;
		case 's':
		case 'S':
		    FT_SetFace( pls ,4);
		    break;
		default:
		    FT_SetFace( pls ,1);
		    break;
		}
                FT_Set_Transform( FT->face, &FT->matrix, &FT->pos );
                i+=2;
                break;

	    case 'u': /* super script */
	    case 'd': /* subscript */
	    case 'U':
	    case 'D':
                i++;
                break;
	    }

        } else {

	/* see if we have kerning for the particular character pair */
	    if ((i>0)&&FT_HAS_KERNING(FT->face)) {
		FT_Get_Kerning( FT->face,
				text[i-1],
				text[i],
				ft_kerning_default,
				&akerning );
		x+= (akerning.x >> 6);        /* add (or subtract) the kerning */
	    }

     /*
      * Next we load the char. This also draws the char, transforms it, and
      * converts it to a bitmap. At present this is a bit wasteful, but
      * if/when I add cache support, then this data won't go to waste.
      * Since there is no sense in going to the trouble of doing anti-aliasing
      * calculations since we aren't REALLY plotting anything, we will render
      * this as monochrome since it is probably marginally quicker. If/when
      * cache support is added, naturally this will have to change.
      */

	    FT_Load_Char( FT->face, text[i], FT_LOAD_MONOCHROME+FT_LOAD_RENDER);

     /*
      * Add in the "advancement" needed to position the cursor for the next
      * character. Unless the text is transformed, "y" will always be zero.
      * Y is negative because freetype does things upside down
      */

	    x += (FT->face->glyph->advance.x);
	    y -= (FT->face->glyph->advance.y);
	}
    }

/*
 * Convert from unit of 1/64 of a pixel to pixels, and do it real fast with
 * a bitwise shift (mind you, any decent compiler SHOULD optimise /64 this way
 * anyway...)
 */

    *yy=y>> 6;
    *xx=x>> 6;
}

/*----------------------------------------------------------------------*\
 * FT_WriteStr()
 *
 * Writes a string of FT text at the current cursor location.
 * most of the code here is identical to "FT_StrX_Y" and I will probably
 * collapse the two into some more efficient code eventually.
\*----------------------------------------------------------------------*/

void
FT_WriteStr(PLStream *pls, const char *text, int x, int y)
{
    FT_Data *FT=(FT_Data *)pls->FT;
    short len=strlen(text);
    short i=0,last_char=-1;
    FT_Vector  akerning, adjust;
    char esc;

/* copy string to modifiable memory */
    strncpy(FT->textbuf, text, NTEXT_ALLOC-1);
    if (len >= NTEXT_ALLOC)
	plwarn("FT_StrX_Y: string too long!");

    plgesc(&esc);

/*
 *  Adjust for the descender - make sure the font is nice and centred
 *  vertically. Freetype assumes we have a base-line, but plplot thinks of
 *  centre-lines, so that's why we have to do this. Since this is one of our
 *  own adjustments, rather than a freetype one, we have to run it through
 *  the transform matrix manually.
 *
 *  For some odd reason, this works best if we triple the
 *  descender's height and then adjust the height later on...
 *  Don't ask me why, 'cause I don't know. But it does seem to work.
 *
 *  I really wish I knew *why* it worked better though...
 *
 *   y-=FT->face->descender >> 6;
 */

#ifdef DODGIE_DECENDER_HACK
    adjust.y= (FT->face->descender >> 6)*3;
#else
    adjust.y= (FT->face->descender >> 6);
#endif

    adjust.x=0;
    FT_Vector_Transform( &adjust, &FT->matrix);
    x+=adjust.x;
    y-=adjust.y;

/* walk through the text character by character */

    for (i=0; i<len; i++) {
	if ((FT->textbuf[i]==esc)&&(FT->textbuf[i-1]!=esc)) {
	    if (FT->textbuf[i+1]==esc) continue;

	    switch(FT->textbuf[i+1]) {
	    case 'f':  /* Change Font */
	    case 'F':
                switch (FT->textbuf[i+2]) {
		case 'r':
		case 'R':
		    FT_SetFace( pls,2);
		    break;
		case 'i':
		case 'I':
		    FT_SetFace( pls,3);
		    break;
		case 's':
		case 'S':
		    FT_SetFace( pls,4);
		    break;
		default:
		    FT_SetFace( pls,1);
		    break;
		}

                FT=(FT_Data *)pls->FT;
                FT_Set_Transform( FT->face, &FT->matrix, &FT->pos );
                i+=2;
                break;

	    case 'g':  /* Greek font */
	    case 'G':
                FT->greek=pls->cfont;
                FT_SetFace( pls, 5 );

                FT=(FT_Data *)pls->FT;

/*                if (FT->face->charmap==NULL)
                   if (FT_Select_Charmap(FT->face, ft_encoding_symbol ))
                       plexit("Could not access a chamap for greek font");

 To get the greek symbol to work it should just be a case of specifying that
 we want to use a symbol charmap. for some reason, this doesn't work. but
 this commented out code SHOULD work. Instead we will manually change the
 value of the greek code to get it working. I doubt this will work on all
 platforms, and all font types, but it does work for DOS/windows and
 truetype fonts.
*/

                FT_Set_Transform( FT->face, &FT->matrix, &FT->pos );
		FT->textbuf[i+2]-=29;
                i++;
                break;

/*
 *  We run the OFFSET for the super-script and sub-script through the
 *  transformation matrix so we can calculate nice and easy the required
 *  offset no matter what's happened rotation wise. Everything else, like
 *  kerning and advancing from character to character is transformed
 *  automatically by freetype, but since the superscript/subscript is a
 *  feature of plplot, and not freetype, we have to make allowances.
 */

	    case 'u': /* super script */
	    case 'U': /* super script */
                adjust.y= (FT->face->size->metrics.height >> 6)/2;
                adjust.x=0;
                FT_Vector_Transform( &adjust, &FT->matrix);
                x+=adjust.x;
                y-=adjust.y;
                i++;
                break;

	    case 'd': /* subscript */
	    case 'D': /* subscript */
                adjust.y= (FT->face->size->metrics.height >> 6)/-2;
                adjust.x=0;
                FT_Vector_Transform( &adjust, &FT->matrix);
                x+=adjust.x;
                y-=adjust.y;
                i++;
                break;
	    }

        } else {
	/* see if we have kerning for the particular character pair */
	    if ((last_char!=-1)&&(i>0)&&FT_HAS_KERNING(FT->face)) {
		FT_Get_Kerning( FT->face,
				FT->textbuf[last_char],
				FT->textbuf[i],
				ft_kerning_default, &akerning );
		x+= (akerning.x >> 6);        /* add (or subtract) the kerning */
		y-= (akerning.y >> 6);        /* Do I need this in case of rotation ? */

            }

	    if (FT->smooth_text==0)
		FT_Load_Char( FT->face, FT->textbuf[i], FT_LOAD_MONOCHROME+FT_LOAD_RENDER);
	    else
		FT_Load_Char( FT->face, FT->textbuf[i], FT_LOAD_RENDER|FT_LOAD_FORCE_AUTOHINT);

	    FT_PlotChar(pls,FT, FT->face->glyph,  x, y, 2 ); /* render the text */
	    x += (FT->face->glyph->advance.x >> 6);
	    y -= (FT->face->glyph->advance.y >> 6);

	    if (FT->greek!=0) {
		FT_SetFace( pls, FT->greek );
		FT=(FT_Data *)pls->FT;
		FT_Set_Transform( FT->face, &FT->matrix, &FT->pos );
		FT->greek=0;
            }
	    last_char=i;
	}
    } /* end for */
}

/*----------------------------------------------------------------------*\
 * FT_PlotChar()
 *
 * Plots an individual character. I know some of this stuff, like colour
 * could be parsed from plstream, but it was just quicker this way.
\*----------------------------------------------------------------------*/

void
FT_PlotChar(PLStream *pls, FT_Data *FT, FT_GlyphSlot slot,
	    int x, int y, short colour )
{
    unsigned char bittest;
    short i,k,j;
    int n=slot->bitmap.pitch;
    FT_Data *FTT=(FT_Data *)pls->FT;

    if ((slot->bitmap.pixel_mode==ft_pixel_mode_mono)||(pls->icol0==0)) {
	x+=slot->bitmap_left;
	y-=slot->bitmap_top;

	for(i=0;i<slot->bitmap.rows;i++) {
	    for (k=0;k<n;k++) {
		bittest=128;
		for (j=0;j<8;j++) {
		    if ((bittest&(unsigned char)slot->bitmap.buffer[(i*n)+k])==bittest)
			FT->pixel(pls, x+(k*8)+j, y+i);
		    bittest>>=1;
		}
            }
        }
    }

/* this is the anti-aliased stuff */
/* At present only one anti-aliased mode is supported, using a
   fixed background and palette. That might change eventually */

    else {
	x+=slot->bitmap_left;
	y-=slot->bitmap_top;

	for(i=0;i<slot->bitmap.rows;i++) {
	    for (k=0;k<slot->bitmap.width;k++) {
		FT->shade=(slot->bitmap.buffer[(i*slot->bitmap.width)+k]);
		if (FT->shade>0) {
		    FT->col_idx=FT->ncol0_width-((FT->ncol0_width*FT->shade)/255);
		    FT->last_icol0=pls->icol0;
		    plcol0(pls->icol0+(FT->col_idx*(FT->ncol0_org-1)));
		    FT->pixel(pls,x+k,y+i);
		    plcol0(FT->last_icol0);
                }
	    }
        }
    }
}

/*----------------------------------------------------------------------*\
 * plD_FreeType_init()
 *
 * Allocates memory to Freetype structure
 * Initialises the freetype library.
 * Initialises freetype structure
\*----------------------------------------------------------------------*/

void plD_FreeType_init(PLStream *pls)
{
    FT_Data *FT;
    char *a;
/* font paths and file names can be long so leave generous (1024) room */
    char font_path[1024];
    const char *env_var_names[]={"PLPLOT_NORMAL_FONT","PLPLOT_ROMAN_FONT",
				 "PLPLOT_ITALIC_FONT","PLPLOT_SCRIPT_FONT",
				 "PLPLOT_SYMBOL_FONT"};
    short i;

#ifdef MSDOS
    static char *default_font_names[]={"arial.ttf","times.ttf","timesi.ttf","arial.ttf",
				       "symbol.ttf"};
#else
    const char *default_unix_font_path="/usr/share/fonts/truetype/";
    static char *default_font_names[]={"Arial.ttf","Times_New_Roman.ttf",
				       "Times_New_Roman_Italic.ttf","Comic_Sans_MS.ttf",
				       "Arial.ttf"};
#endif

    if (pls->FT) {
	plwarn("Freetype seems already to have been initialised!");
	return;
    }

    if ((pls->FT=calloc(1, (size_t)sizeof(FT_Data)))==NULL)
	plexit("Could not allocate memory for Freetype");

    FT=(FT_Data *)pls->FT;

    if ((FT->textbuf=calloc(NTEXT_ALLOC, 1))==NULL)
	plexit("Could not allocate memory for Freetype text buffer");

    if ( FT_Init_FreeType( &FT->library ) )
	plexit("Could not initialise Freetype library");

    FT->cfont=PL_UNDEFINED;

#ifdef MSDOS

/*
 * Work out if we have Win95+ or Win3.?... sort of.
 * Actually, this just tries to find the place where the fonts live by looking
 * for arial, which should be on all windows machines.
 * At present, it only looks in two places, on one drive. I might change this
 * soon.
 */

    if (access("c:/windows/fonts/arial.ttf", F_OK)==0)
	strcpy(font_path,"c:/windows/fonts/");

    else if (access("c:/windows/system/arial.ttf", F_OK)==0)
	strcpy(font_path,"c:/windows/system/");

    else
	plwarn("Could not find font path; I sure hope you have defined fonts manually !");

#else

/*
 *  For Unix systems, we will set the font path up a little differently in
 *  that "/usr/share/fonts/truetype/" has been set as the default path, but
 *  the user can override this by setting the environmental variable
 *  "PLPLOT_FREETYPE_FONT_PATH" to something else.
 *  NOTE WELL - the trailing slash must be added for now !
 */

    if (a = getenv("PLPLOT_FREETYPE_FONT_PATH"))
        strcpy(font_path,a);
    else
        strcpy(font_path,default_unix_font_path);

#endif

/*
 * Next we go through and fill out the FT->font_name entries with
 * values for the four possible "cfont" settings. The plplot documentation
 * defines these a little differently from the PS.C driver:
 *   Font       Plplot name     ps.c name       plfreetype ms-dos/unix
 *   ----       -----------     ---------       -----------------
 *     1        normal          "Times-Roman"           arial
 *     2        roman           "Times-Roman"           times
 *     3        italic          "Times-Italic"        times-italic
 *     4        script          "Helvetica"             comic
 *     5        greek                                   symbol
 *
 *  Under plplot, the "normal" font is actually a sans-serifed font, while
 *  ps.c defines it as times-roman, a serifed font. For the freetype
 *  implementation I will define sans-serifed font, arial.
 *
 * The driver looks for 5 environmental variables where the path and name of
 * these fonts can be OPTIONALLY set, overriding the "guessed" values.
 *
 * The naming priority order is:  1) Overridden by command line;
 *                                2) Set via enviro variables
 *                                3) "guessed" by program
 */

    for (i=0; i<5; i++) {
	if (a = getenv(env_var_names[i])) {

/*
 *  Work out if we have been given an absolute path to a font name, or just
 *  a font name sans-path. To do this we will look for a directory separator
 *  character, which means some system specific junk. DJGPP is all wise, and
 *  understands both Unix and DOS conventions. DOS only knows DOS, and
 *  I assume everything else knows Unix-speak. (Why Bill, didn't you just
 *  pay the extra 15c and get a REAL separator???)
 */

#ifdef MSDOS
	    if (a[1]==':') /* check for MS-DOS absolute path */
#else
	    if ((a[0]=='/')||(a[0]=='~')) /* check for unix abs path */
#endif
		strcpy(FT->font_name[i],a);

	    else {
		strcpy(FT->font_name[i],font_path);
		strcat(FT->font_name[i],a);
	    }

	} else {
	    strcpy(FT->font_name[i],font_path);
	    strcat(FT->font_name[i],default_font_names[i]);
	}

	if (access(FT->font_name[i], F_OK)!=0) {
	    char msgbuf[1024];
	    sprintf(msgbuf,
		    "Possible error defining one of the freetype compatible fonts:\n %s",
		    FT->font_name[i]);
	    plwarn(msgbuf);
	}
    }
/*
 * Next, we check to see if -drvopt has been used on the command line to
 * over-ride any settings
 */

}


/*----------------------------------------------------------------------*\
 * FT_SetFace( PLStream *pls, int fnt )
 *
 * Sets up the font face and size
\*----------------------------------------------------------------------*/

void FT_SetFace( PLStream *pls, int fnt )
{
    FT_Data *FT=(FT_Data *)pls->FT;
    double font_size = pls->chrht * 72/25.4; /* font_size in points, chrht is in mm */

    if (FT->face!=NULL) {
	FT_Done_Face(FT->face);
	FT->face=NULL;
    }

    if (FT->face==NULL) {
	if (FT_New_Face( FT->library,FT->font_name[fnt-1],
			 0,&FT->face)) plexit("Loading a font in freetype");
	FT->cfont=fnt;
    }

    FT_Set_Char_Size(FT->face,0, font_size * 64/TEXT_SCALING_FACTOR,pls->xdpi, pls->ydpi );
}

/*----------------------------------------------------------------------*\
 * plD_render_freetype_text()
 *
 * Transforms the font
 * calculates real-world bitmap coordis from plplot ones
 * renders text using freetype
\*----------------------------------------------------------------------*/

void plD_render_freetype_text (PLStream *pls, EscText *args)
{
    FT_Data *FT=(FT_Data *)pls->FT;
    int x,y;
    int w=0,h=0;
    PLFLT *t = args->xform;
    FT_Matrix matrix;
    PLFLT angle=PI*pls->diorot/2;
    PLFLT a1, alpha;
    PLINT clxmin, clxmax, clymin, clymax;
    PLFLT Sin_A,Cos_A;
    FT_Vector adjust;


    if (FT->cfont!=pls->cfont) FT_SetFace(pls,pls->cfont);


/*  this will help work out underlining and overlining

    printf("%d %d %d %d;",FT->face->underline_position>>6,
	   FT->face->descender>>6,
	   FT->face->ascender>>6,
	   ((FT->face->underline_position*-1)+FT->face->ascender)>>6);

*/

/*
 *  Now we work out how long the text is (for justification etc...) and how
 *  high the text is. This is done on UN-TRANSFORMED text, since we will
 *  apply our own transformations on it later, so it's necessary for us
 *  to to turn all transformations off first, before calling the function
 *  that calculates the text size.
 */

    FT->matrix.xx =0x10000;
    FT->matrix.xy =0000;
    FT->matrix.yx =0;
    FT->matrix.yy =0x10000;

    FT_Vector_Transform( &FT->pos, &FT->matrix);
    FT_Set_Transform( FT->face, &FT->matrix, &FT->pos );

    FT_StrX_Y(pls,args->string,&w, &h);


/*
 *      Set up the transformation Matrix
 *
 * Fortunately this is almost identical to plplot's own transformation matrix;
 * you have NO idea how much effort that saves ! Some params are in a
 * different order, and Freetype wants integers whereas plplot likes floats,
 * but such differences are quite trivial.
 *
 * For some odd reason, this needs to be set a different way for DJGPP. Why ?
 * I wish I knew.
 */

#ifdef DJGPP
    FT->matrix.xx =(FT_Fixed)0x10000*t[0];
    FT->matrix.xy =(FT_Fixed)0x10000*t[2];
    FT->matrix.yx =(FT_Fixed)0x10000*t[1];
    FT->matrix.yy =(FT_Fixed)0x10000*t[3];
#else
    FT->matrix.xx =(FT_Fixed)0x10000*t[0];
    FT->matrix.xy =(FT_Fixed)0x10000*t[1];
    FT->matrix.yx =(FT_Fixed)0x10000*t[2];
    FT->matrix.yy =(FT_Fixed)0x10000*t[3];
#endif


/*                            Rotate the Font
 *
 *  If the page has been rotated using -ori, this is where we rotate the
 *  font to point in the right direction. To make things nice and easy, we
 *  will use freetypes matrix math stuff to do this for us.
 */

    Cos_A=cos(angle);
    Sin_A=sin(angle);

    matrix.xx =(FT_Fixed)0x10000*Cos_A;

#ifdef DJGPP
    matrix.xy =(FT_Fixed)0x10000*Sin_A*-1;
    matrix.yx =(FT_Fixed)0x10000*Sin_A;
#else
    matrix.xy =(FT_Fixed)0x10000*Sin_A;
    matrix.yx =(FT_Fixed)0x10000*Sin_A*-1;
#endif

    matrix.yy =(FT_Fixed)0x10000*Cos_A;

    FT_Matrix_Multiply(&matrix,&FT->matrix);


/*       Calculate a Vector from the matrix
 *
 * This is closely related to the "transform matrix".
 * The matrix is used for rendering the glyph, while the vector is used for
 * calculating offsets of the text box, so we need both. Why ? I dunno, but
 * we have to live with it, and it works...
 */


    FT_Vector_Transform( &FT->pos, &FT->matrix);


/*    Transform the font face
 *
 * This is where our matrix transformation is calculated for the font face.
 * This is only done once for each unique transformation since it is "sticky"
 * within the font. Font rendering is done later, using the supplied matrix,
 * but invisibly to us from here on. I don't believe the vector is used, but
 * it is asked for.
 */

    FT_Set_Transform( FT->face, &FT->matrix, &FT->pos );



/*                            Rotate the Page
 *
 *  If the page has been rotated using -ori, this is we recalculate the
 *  reference point for the text using plplot functions.
 */

  difilt(&args->x, &args->y, 1, &clxmin, &clxmax, &clymin, &clymax);


/*
 *   Convert into normal coordinates from virtual coordinates
 */

  x=args->x/FT->scale;

 if (FT->invert_y==1)
    y=FT->ymax-(args->y/FT->scale);
 else
    y=args->y/FT->scale;


 /*          Adjust for the justification and character height
  *
  *  Eeeksss... this wasn't a nice bit of code to work out, let me tell you.
  *  I could not work out an entirely satisfactory solution that made
  *  logical sense, so came up with an "illogical" one as well.
  *  The logical one works fine for text in the normal "portrait"
  *  orientation, and does so for reasons you might expect it to work; But
  *  for all other orientations, the text's base line is either a little
  *  high, or a little low. This is because of the way the base-line pos
  *  is calculated from the decender height. The "dodgie" way of calculating
  *  the position is to use the character height here, then adjust for the
  *  decender height by a three-fold factor later on. That approach seems to
  *  work a little better for rotated pages, but why it should be so, I
  *  don't understand. You can compile in or out which way you want it by
  *  defining "DODGIE_DECENDER_HACK".
  *
  *  note: the logic of the page rotation coming up next is that we pump in
  *  the justification factor and then use freetype to rotate and transform
  *  the values, which we then use to change the plotting location.
  */


#ifdef DODGIE_DECENDER_HACK
    adjust.y=h;
#else
    adjust.y=0;
#endif

    adjust.x=args->just*w*-1;
    FT_Vector_Transform( &adjust, &matrix);
    x+=adjust.x;
    y-=adjust.y;

    FT_WriteStr(pls,args->string,x,y); /* write it out */
}

/*----------------------------------------------------------------------*\
 * plD_FreeType_Destroy()
 *
 * Restores cmap0 if it had been modifed for anti-aliasing
 * closes the freetype library.
 * Deallocates memory to the Freetype structure
\*----------------------------------------------------------------------*/

void plD_FreeType_Destroy(PLStream *pls)
{
    FT_Data *FT=(FT_Data *)pls->FT;

    if (FT) {
	if (FT->smooth_text==1) plscmap0n(FT->ncol0_org);
	FT_Done_Library(FT->library);
	free(FT->textbuf);
	free(pls->FT);
	pls->FT=NULL;
    }
}

/*----------------------------------------------------------------------*\
 * PLFLT CalculateIncrement( int bg, int fg, int levels)
 *
 * Takes the value of the foreground, and the background, and when
 * given the number of desired steps, calculates how much to incriment
 * a value to transition from fg to bg.
 * This function only does it for one colour channel at a time.
\*----------------------------------------------------------------------*/

static PLFLT CalculateIncrement( int bg, int fg, int levels)
{
    PLFLT ret=0;

    if (levels>1) {
	if (fg>bg)
	    ret=((fg+1)-bg)/levels;
	else if (fg<bg)
	    ret=(((fg-1)-bg)/levels);
    }
    return(ret);
}

/*----------------------------------------------------------------------*\
 * void pl_set_extended_cmap0(PLStream *pls, int ncol0_width, int ncol0_org)
 *
 *  ncol0_width - how many greyscale levels to accolate to each CMAP0 entry
 *  ncol0_org - the originl number of CMAP0 entries.
 *
 *  This function calcualtes and sets an extended CMAP0 entry for the
 *  driver. It is assumed that the caller has checked to make sure there is
 *  room for extending CMAP0 already.
 *
 *  NOTES
 *  We don't bother calculating an entry for CMAP[0], the background.
 *  It is assumed the caller has already expanded the size of CMAP[0]
\*----------------------------------------------------------------------*/

void pl_set_extended_cmap0(PLStream *pls, int ncol0_width, int ncol0_org)
{
    int i,j,k;
    int r,g,b;
    PLFLT r_inc,g_inc,b_inc;

    for (i=1;i<ncol0_org;i++) {

	r=pls->cmap0[i].r;
	g=pls->cmap0[i].g;
	b=pls->cmap0[i].b;

	r_inc=CalculateIncrement(pls->cmap0[0].r,r,ncol0_width);
	g_inc=CalculateIncrement(pls->cmap0[0].g,g,ncol0_width);
	b_inc=CalculateIncrement(pls->cmap0[0].b,b,ncol0_width);

	for (j=0,k=ncol0_org+i-1;j<ncol0_width;j++,k+=(ncol0_org-1)) {
	    r-=r_inc;
	    g-=g_inc;
	    b-=b_inc;
	    if ((r<0)||(g<0)||(b<0))
		plscol0 (k, 0, 0, 0);
	    else
		plscol0 (k, r, g, b);
        }
    }
}

#else
int
plfreetype()
{
    return 0;
}

#endif
