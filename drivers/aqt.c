#import <Foundation/Foundation.h>
#import <aquaterm/AQTAdapter.h>

/* Debugging extras */
static inline void NOOP_(id x, ...) {;}

#ifdef LOGGING
#define LOG  NSLog
#else
#define LOG  NOOP_
#endif  /* LOGGING */


static NSAutoreleasePool *arpool;   // Objective-C autorelease pool
static id adapter;                          // Adapter object

// ----------------------------------------------------------------
// --- Start of PlPlot function aqtrm()
// ----------------------------------------------------------------
#include "plplotP.h"
#include "drivers.h"

// FIXME : Are the (numerical) values in this string important?

char* plD_DEVICE_INFO_aqt = "aqt:AquaTerm (OS-X):1:aqt:50:aqt";

/* declarations for functions local to aqt.c */

void get_cursor(PLGraphicsIn *);
void proc_str (PLStream *, EscText *);
static void esc_purge(char *, const char *);

/* Driver entry and dispatch setup */

void plD_dispatch_init_aqt      ( PLDispatchTable *pdt );

void plD_init_aqt               (PLStream *);
void plD_line_aqt               (PLStream *, short, short, short, short);
void plD_polyline_aqt   		(PLStream *, short *, short *, PLINT);
void plD_eop_aqt                (PLStream *);
void plD_bop_aqt                (PLStream *);
void plD_tidy_aqt               (PLStream *);
void plD_state_aqt              (PLStream *, PLINT);
void plD_esc_aqt                (PLStream *, PLINT, void *);

void plD_dispatch_init_aqt( PLDispatchTable *pdt )
{
   pdt->pl_MenuStr  = "AquaTerm - Mac OS X";
   pdt->pl_DevName  = "aqt";
   pdt->pl_type     = plDevType_Interactive;
   pdt->pl_seq      = 1;
   pdt->pl_init     = (plD_init_fp)     plD_init_aqt;
   pdt->pl_line     = (plD_line_fp)     plD_line_aqt;
   pdt->pl_polyline = (plD_polyline_fp) plD_polyline_aqt;
   pdt->pl_eop      = (plD_eop_fp)      plD_eop_aqt;
   pdt->pl_bop      = (plD_bop_fp)      plD_bop_aqt;
   pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_aqt;
   pdt->pl_state    = (plD_state_fp)    plD_state_aqt;
   pdt->pl_esc      = (plD_esc_fp)      plD_esc_aqt;
}
static int currentPlot = 0;
static int maxWindows = 30;
static int windowXSize = 0;
static int windowYSize = 0;

#define SCALE			0.1
#define AQT_Max_X       720
#define AQT_Max_Y       720
#define DPI             72.0

//---------------------------------------------------------------------
//   aqt_init()
//
//   Initialize device
//----------------------------------------------------------------------

void plD_init_aqt(PLStream *pls)
{
	
   if (arpool == NULL)   /* Make sure we don't leak mem by allocating every time */
   {
      arpool = [[NSAutoreleasePool alloc] init];
      adapter = [[AQTAdapter alloc] init];
   }
   [adapter setBackgroundColorRed:0.5 green:0.5 blue:0.5];

   pls->termin = 1;		// If interactive terminal, set true.
   pls->color = 1;		// aqt is color terminal
   pls->width = 1;
   pls->verbose = 1;
   pls->bytecnt = 0;
   pls->debug = 1;
   pls->dev_text = 1;
   pls->page = 0;
   pls->dev_fill0 = 1;		/* supports hardware solid fills */
   pls->dev_fill1 = 1;

   pls->graphx = GRAPHICS_MODE;

   if (!pls->colorset)
      pls->color = 1; 
   //
   // Set up device parameters
   //
   plP_setpxl(DPI/25.4/SCALE, DPI/25.4/SCALE);           /* Pixels/mm. */
   //
   // Set the bounds for plotting.  default is AQT_Max_X x AQT_Max_Y unless otherwise specified.
   //
   if (pls->xlength <= 0 || pls->ylength <= 0){
      windowXSize = AQT_Max_X;
      windowYSize = AQT_Max_Y;
      plP_setphy((PLINT) 0, (PLINT) (AQT_Max_X/SCALE), (PLINT) 0, (PLINT) (AQT_Max_Y/SCALE));
   } else {
      // FIXME : Would the user typically specify a window size in pixels?
      //   All I know is that the Perl Data Language Plplot package seems to
      windowXSize = pls->xlength;
      windowYSize = pls->ylength;
      plP_setphy((PLINT) 0, (PLINT) (pls->xlength/SCALE), (PLINT) 0, (PLINT) (pls->ylength/SCALE));
   }   
}

//----------------------------------------------------------------------
//   aqt_bop()
//
//   Set up for the next page.
//----------------------------------------------------------------------

void plD_bop_aqt(PLStream *pls)
{
   currentPlot = currentPlot>=maxWindows?0:currentPlot;
   [adapter openPlotWithIndex:currentPlot++];
   [adapter setPlotSize:NSMakeSize(windowXSize, windowYSize)];
   [adapter setLinewidth:1.0];
   [adapter setColorRed:(float)(pls->curcolor.r/255.)
   				  green:(float)(pls->curcolor.g/255.)
				   blue:(float)(pls->curcolor.b/255.)];

   pls->page++;
}

//---------------------------------------------------------------------
//   aqt_line()
//
//   Draw a line in the current color from (x1,y1) to (x2,y2).
//----------------------------------------------------------------------

void plD_line_aqt(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
   [adapter moveToPoint:NSMakePoint((float)x1a*SCALE, (float)y1a*SCALE)];
   [adapter addLineToPoint:NSMakePoint((float)x2a*SCALE, (float)y2a*SCALE)];
}

//---------------------------------------------------------------------
//  aqt_polyline()
//
// Draw a polyline in the current color.
//---------------------------------------------------------------------

void plD_polyline_aqt(PLStream *pls, short *xa, short *ya, PLINT npts)
{
   int i;

   for (i = 0; i < npts - 1; i++)
      plD_line_aqt(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

//---------------------------------------------------------------------
//   aqt_eop()
//
//   End of page
//---------------------------------------------------------------------

void plD_eop_aqt(PLStream *pls)
{
   [adapter renderPlot];
}

//---------------------------------------------------------------------
// aqt_tidy()
//
// Close graphics file or otherwise clean up.
//---------------------------------------------------------------------

void plD_tidy_aqt(PLStream *pls)
{
   [adapter closePlot];
}
//---------------------------------------------------------------------
//  plD_state_aqt()
//
//  Handle change in PLStream state (color, pen width, fill attribute, etc).
//---------------------------------------------------------------------

void plD_state_aqt(PLStream *pls, PLINT op)
{
   int i;
   float r,g,b;

   switch (op)
   {
      case PLSTATE_WIDTH:
         [adapter setLinewidth:(float)pls->width];
         break;

      case PLSTATE_COLOR0:	// this seems to work, but that isn't to say that it is done right...
         [adapter setBackgroundColorRed:(float)(plsc->cmap0[0].r/255.0) 
	                              green:(float)(plsc->cmap0[0].g/255.0)
	                               blue:(float)(plsc->cmap0[0].b/255.0)];
      case PLSTATE_COLOR1:
      case PLSTATE_FILL:
		 [adapter setColorRed:(float)(pls->curcolor.r/255.)
   		 		   	    green:(float)(pls->curcolor.g/255.)
		 		         blue:(float)(pls->curcolor.b/255.)];
         break;

      case PLSTATE_CMAP0:
         break;

      case PLSTATE_CMAP1:
         break;
   }
}

//---------------------------------------------------------------------
// aqt_esc()
//
// Escape function.
//
// Functions:
//
//      PLESC_EH        Handle pending events
//      PLESC_EXPOSE    Force an expose
//      PLESC_FILL      Fill polygon
//      PLESC_FLUSH     Flush X event buffer
//      PLESC_GETC      Get coordinates upon mouse click
//      PLESC_REDRAW    Force a redraw
//      PLESC_RESIZE    Force a resize
//---------------------------------------------------------------------
//
void plD_esc_aqt(PLStream *pls, PLINT op, void *ptr)
{
   int     i;
   switch (op)
   {
      case PLESC_EXPOSE:              // handle window expose
         break;
      case PLESC_RESIZE:              // handle window resize
         break;
      case PLESC_REDRAW:              // handle window redraw
         break;
      case PLESC_TEXT:                // switch to text screen
         break;
      case PLESC_GRAPH:               // switch to graphics screen
         break;
      case PLESC_FILL:                // fill polygon	 
         [adapter moveToVertexPoint:NSMakePoint(pls->dev_x[0]*SCALE, pls->dev_y[0]*SCALE)];
         for (i = 1; i < pls->dev_npts ; i++)
         {
            [adapter addEdgeToVertexPoint:NSMakePoint(pls->dev_x[i]*SCALE, pls->dev_y[i]*SCALE)];
         };
         break;
      case PLESC_DI:                  // handle DI command
         break;
      case PLESC_FLUSH:               // flush output
         break;
      case PLESC_EH:                  // handle Window events
         break;
      case PLESC_GETC:                // get cursor position
         [adapter renderPlot]; // needed to give the user something to click on
         get_cursor((PLGraphicsIn*)ptr);
         break;
      case PLESC_SWIN:                // set window parameters
         break;
      case PLESC_HAS_TEXT:
         proc_str(pls, (EscText *)ptr);
         break;

   }
}

//---------------------------------------------------------------------
// get_cursor()
//
// returns the location of the next mouse click
//---------------------------------------------------------------------

void get_cursor(PLGraphicsIn *gin){
	int scanned, x, y, button;
    NSString *temp;

	temp = [adapter waitNextEvent];
	scanned = sscanf([temp cString],"1:{%d, %d}:%d", &x, &y, &button);

	// multiple FIXMEs
	// 1) should the returned coordinates be adjusted by scale?
	// 2) should different coordinates be adjusted in different ways?
	//     i.e. what is the difference between the absolute, relative and world
	//     coordinates?
	// 3) why are the world coordinates (wX, wY) set to zero between when this 
	//     sub-routine is called and the values are returned to the program
	//     that made the plplot call?	
	// 4) should the structure members state and keysym be set to something?
	// 5) is the translated string structure member relevant?

	if(scanned == 3){	// check that we did actually get a reasonable event string
		gin->button = button;
		gin->pX = x;
		gin->pY = y;
		gin->dX = (PLFLT)x;
		gin->dY = (PLFLT)y;
	} else {	// just return zeroes if we did not
		gin->button = 0;
		gin->pX = 0;
		gin->pY = 0;
		gin->dX = 0.0;
		gin->dY = 0.0;	
	}
}

//---------------------------------------------------------------------
// proc_str()
//
// process strings for display
//---------------------------------------------------------------------

void proc_str (PLStream *pls, EscText *args)
{
   PLFLT   *t = args->xform;
   PLFLT   a1, alpha, ft_ht, angle;
   PLINT   clxmin, clxmax, clymin, clymax;
   char    str[128], fontn[128], updown[128], esc, *strp;
   const char *cur_str;
   char    *font, *ofont;
   float   ft_scale;
   int             i, jst, ref, symbol, length, ltmp;
   NSMutableAttributedString  *s;
   char    char_ind;
   unichar Greek[53] = {
      0X391, 0X392, 0X3A7, 0X394, 0X395, 0X3A6, 0X393, 0X397,
      0X399, 0X3D1, 0X39A, 0X39B, 0X39C, 0X39D, 0X39F, 0X3A0,
      0X398, 0X3A1, 0X3A3, 0X3A4, 0X3A5, 0X3A7, 0X3A9, 0X39E,
      0X3A8, 0X396, 0X3B1, 0X3B2, 0X3C7, 0X3B4, 0X3B5, 0X3D5,
      0X3B3, 0X3B7, 0X3B9, 0X3C6, 0X3BA, 0X3BB, 0X3BC, 0X3BD,
      0X3BF, 0X3C0, 0X3B8, 0X3C1, 0X3C3, 0X3C4, 0X3C5, 0X3DB,
      0X3C9, 0X3BE, 0X3C8, 0X3B6, 0x003F};

   //  Set the font height - the 1.2 factor was trial and error

   ft_ht = 1.2*pls->chrht * DPI/25.4; /* ft_ht in points. ht is in mm */

   //
   //  Now find the angle of the text relative to the screen...
   //
   angle = pls->diorot * 90.;
   a1 = acos(t[0]) * 180. / PI;
   if (t[2] > 0.)
      alpha = a1 - angle;
   else
      alpha = 360. - a1 - angle;
   //
   // any transformation if there is one - normally on text there isn't any
   //
   difilt(&args->x, &args->y, 1, &clxmin, &clxmax, &clymin, &clymax);

   //     check clip limits. For now, only the reference point of the string is checked;
   //     but the the whole string should be checked

   if ( args->x < clxmin || args->x > clxmax || args->y < clymin || args->y > clymax)
      return;

   //   * Text justification.  Left, center and right justification, which
   //   *  are the more common options, are supported; variable justification is
   //   *  only approximate, based on plplot computation of it's string lenght

   if (args->just < 0.33){
      jst = AQTAlignLeft;                             /* left */
   }
   else if (args->just > 0.66){
      jst = AQTAlignRight;                            /* right */
   }
   else {
      jst = AQTAlignCenter;                           /* center */
   }
   /*
    * Reference point (center baseline of string).
    *  If base = 0, it is aligned with the center of the text box
    *  If base = 1, it is aligned with the baseline of the text box
    *  If base = 2, it is aligned with the top of the text box
    *  Currently plplot only uses base=0
    */

   if (args->base == 2)
      ref = AQTAlignTop;
   else if (args->base == 1)
      ref = AQTAlignBaseline;
   else
      ref = AQTAlignMiddle;

   //
   //  Set the default font
   //
   switch (pls->cfont) {
      case 1: ofont = "Times-Roman";  fontn[0]=1; break;
      case 2: ofont = "Times-Roman";  fontn[0]=2; break;
      case 3: ofont = "Times-Italic"; fontn[0]=3; break;
      case 4: ofont = "Helvetica";    fontn[0]=4; break;
      default:  ofont = "Times-Roman";fontn[0]=1;
   }

   //  Get a purged string for testing

   plgesc(&esc);

   length=0;
   esc_purge(str, args->string);


   //  set font and super/subscriptsfor each character
   do{
      updown[length]=0;
      fontn[length]=fontn[0];
   }while(str[length++] && length < 128);
   fontn[--length]=0;

   cur_str = args->string;

   strp = str;

   do {
      symbol = 0;

      if (*cur_str == esc) {
         cur_str++;

         if (*cur_str == esc) {
            // <esc><esc>
            *strp++ = *cur_str++;
         }
         else switch (*cur_str) {

            case 'f':
               //  Change font
               cur_str++;
               switch (*cur_str) {
                  case 'n': font = "Times-Roman";
                     fontn[strp-str]=1; break;
                  case 'r': font = "Times-Roman";
                     fontn[strp-str]=2; break;
                  case 'i': font = "Times-Italic";
                     fontn[strp-str]=3; break;
                  case 's': font = "Helvetica";
                     fontn[strp-str]=4; break;
                  default:  font = "Times-Roman";
                     fontn[strp-str]=1;
               }
                  //  set new font for rest of string or until next font change
                  ltmp=strp-str;
               do{
                  fontn[ltmp++]=fontn[strp-str];
               }while(ltmp < length);

                  cur_str++;
               break;

            case 'g':
               // Greek Letters are single characters
               cur_str++;
               fontn[strp-str]=5; break;
               *strp++ = *cur_str++;
               break;

            case 'd':
               // Subscript not used
               ltmp=strp-str;
               do{
                  updown[ltmp++]--;
               }while(ltmp < length);
                  cur_str++;
               break;

            case 'u':
               // Superscript not used
               ltmp=strp-str;
               do{
                  updown[ltmp++]++;
               }while(ltmp < length);
                  cur_str++;
               break;

               /* ignore the next sequences */

            case '+':
            case '-':
            case 'b':
               // plwarn("'+', '-', and 'b' text escape sequences not processed.");
               cur_str++;
               break;

            case '(':
               // plwarn("'g(...)' text escape sequence not processed.");
               while (*cur_str++ != ')');
               *strp++ = 'x';                              //  Plplot uses #(229) for x in exponent
               break;
         }
      }
         // copy from current to next token

         while(!symbol && *cur_str && *cur_str != esc) {
            *strp++ = *cur_str++;
         }
         *strp = '\0';

   } while(*cur_str);

      //  Now we create an attributed string
      //
      s = [[NSMutableAttributedString alloc] initWithString:[NSString stringWithCString:str]];
      //
      //  Set the default font and color for the string before we do anything else
      //
      [adapter setFontname:[NSString stringWithCString:ofont]];
      [adapter setFontsize:ft_ht];
      [adapter setColorRed:(float)(pls->curcolor.r/255.)
                     green:(float)(pls->curcolor.g/255.)
                      blue:(float)(pls->curcolor.b/255.)];
      //
      //  Set the font
      for(i = 0; i < length; i++)
      {
         //
         //  Set Greek Characters
         if(fontn[i]==5){
            if((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')) {
               char_ind = (char) str[i] - 'A';
               if(char_ind >= 32)
                  char_ind -=6;

               if(char_ind < 0 || char_ind > 51 )
                  char_ind = 52;

               [s replaceCharactersInRange:NSMakeRange(i, 1)
                                withString:[NSString stringWithCharacters:Greek+char_ind length:1]];
            }
         }
         //
         //  Set Super and subscripts

         if(updown[i]!=0)
         {
            [s addAttribute:@"NSSuperScript"
                      value:[NSNumber numberWithInt:updown[i]]
                      range:NSMakeRange(i, 1)];
         }
      }
      [adapter addLabel:s atPoint:NSMakePoint((float)args->x*SCALE, (float)args->y*SCALE) angle:alpha align:(jst | ref)]; 

      [s release];

}

static void esc_purge(char *dstr, const char *sstr)
{
   char esc;

   plgesc(&esc);

   while(*sstr){
      if (*sstr != esc) {
         *dstr++ = *sstr++;
         continue;
      }

      sstr++;
      if (*sstr == esc)
         continue;
      else {
         switch(*sstr++) {
            //                              case 'g':
            case 'f':
               sstr++;
               break; /* two chars sequence */

            case '(':
               while (*sstr++ != ')'); /* multi chars s
                      equence */
               *dstr++ = 'x';                              //  Plplot uses #(229) for x in exponent
               break;

            default:
               break;  /* single char escape */
         }
      }
   }
   *dstr = '\0';
}

// ----------------------------------------------------------------
// --- End of PlPLOT function aqtrm()
// ----------------------------------------------------------------
