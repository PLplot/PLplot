/* $Id$
   $Log$
   Revision 1.1  1995/04/11 09:39:13  mjl
   Mac port, contributed by Mark Franz.

 *
*/

/*
	Rect	PlotRect = {40, 10, 540, 510};

	MaxApplZone();
	
	for (i = 0; i < 12; i++)
		MoreMasters();

	mac_plot_area(&PlotRect);

*/

/*	mac.c

	Adapted from bgivga.c
 	 MArk Franz
  	 24-Mar-95

  A driver for Macintosh
  Compile this with MetroWerks C/C++ or ThinkC

*/
#include "plplotP.h"
#include <stdio.h>
#include <stdlib.h>
#include "drivers.h"
#include "mac.h"

#define MAC_XMAX 400
#define MAC_YMAX 400
#define MAC_XOFFSET 20
#define NIL 	NULL
#define Max_Mac_Windows	8


// Plot menu 
#define plotM			1000
#define PlotCopy 		1
#define PlotSave		2
#define PlotContinue	4



Str255	MAC_WINDOW_TITLE = "\pPlot ";
Str255	tempWTitle;


RgnHandle	fillRegion;
static WindowPtr	plotWindow[Max_Mac_Windows];
static PicHandle 	plotPicture[Max_Mac_Windows];
static Handle 		saveMBar, myMBar;
RGBColor			rgbColor;
long 				PICTCount;
short 				globalRef; 
short				plotActive = 0;
PicHandle 			newPICTHand;
short				plotWidth = MAC_YMAX;
short				plotHeight = MAC_XMAX;


static Rect	boundsRect = {40, 10, MAC_YMAX+40, MAC_YMAX+10};

/*----------------------------------------------------------------------*\
* mac_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/
 
void
plD_init_mac(PLStream *pls)
{	
int		i;

	for(i = 0; i < Max_Mac_Windows; i++)
	{
		plotWindow[i] = NULL;
		plotPicture[i] = NULL;
	}
	plotActive = -1;

	fprintf(stderr, "Macintosh Window\n");   // This is here just to
															// make sure Menus drawn
	
//	plotWindow = NewCWindow(NULL, &boundsRect,MAC_WINDOW_TITLE, FALSE, 
//							noGrowDocProc, (void *)-1L, FALSE, 0L);
    
//	SetPort(plotWindow);
//    GetPort(&savePort);	
//	SetPort(plotWindow);
	SetUpMenu();
//	SetPort(savePort);

	 pls->termin = 1;            /* is an interactive terminal */
	 pls->icol0 = 1;
	 pls->icol1 = 1;
	 pls->width = 1;
	 pls->bytecnt = 0;
	 pls->page = 0;
	 pls->dev_fill0 = 0;
	 pls->dev_fill1 = 0;
	 
	 pls->graphx = GRAPHICS_MODE;

	 if (!pls->colorset)
 		pls->color = 1;
 		

/* Set up device parameters */

	 plP_setpxl(2.8346, 2.8346);           /* Pixels/mm. */

	 plP_setphy((PLINT) 0, (PLINT) (boundsRect.right-boundsRect.left), 
	 			(PLINT) 0, (PLINT) (boundsRect.bottom-boundsRect.top));
	 
	 
}

/*----------------------------------------------------------------------*\
* mac_line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*----------------------------------------------------------------------*/

void
plD_line_mac(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
int x1, y1, x2, y2 ;

	y1 = (boundsRect.bottom-boundsRect.top) - y1a;
	y2 = (boundsRect.bottom-boundsRect.top) - y2a;

	 MoveTo(x1a, y1);
	 LineTo(x2a, y2);
}

/*----------------------------------------------------------------------*\
* mac_polyline()
*
* Draw a polyline in the current color.
\*----------------------------------------------------------------------*/

void
plD_polyline_mac(PLStream *pls, short *xa, short *ya, PLINT npts)
{
	 PLINT i;

	 for (i = 0; i < npts - 1; i++)
	plD_line_mac(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);
}

/*----------------------------------------------------------------------*\
* mac_eop()
*
* End of page.
\*----------------------------------------------------------------------*/

void
plD_eop_mac(PLStream *pls)
{
Rect 	myRect;
	
	ClosePicture();
	if(pls->termin == 1)
	{
		if(plotWindow !=NULL)
		{
			GetWTitle(plotWindow[plotActive], tempWTitle);
			SetWTitle(plotWindow[plotActive], "\pHit any key to continue.");
			InvalRect(&plotWindow[plotActive]->portRect);
			AdjustMenu();
	  		while(!MAC_event());
			SetWTitle(plotWindow[plotActive], tempWTitle);
//			RestoreMenu();
		}
	}
	else
	{
		if(plotPicture != NULL)
		{	
			AdjustMenu();
			myRect=plotWindow[plotActive]->portRect;
			DrawPicture(plotPicture[plotActive],&myRect);
//			RestoreMenu();
		}
	}
}

/*----------------------------------------------------------------------*\
* mac_bop()
*
* Set up for the next page.
\*----------------------------------------------------------------------*/

void
plD_bop_mac(PLStream *pls)
{
RGBColor	backGround;
Str255		numberAsString;
Str255		currentTitle;
int			i;

	pls->page++;

	plotActive++;
	if(plotActive == Max_Mac_Windows) 
		plotActive =0;
		
	if (plotWindow[plotActive] == NULL)					//  Get a window handle
	{
		boundsRect.top 	= 40+plotActive*20;				//  Set the inital window location
		boundsRect.left = (plotActive+1)*10;
		boundsRect.right = plotWidth+(plotActive+1)*10;
		boundsRect.bottom = plotHeight+40+plotActive*20;
		
		plotWindow[plotActive] = NewCWindow(NULL, &boundsRect,"\p ", FALSE, 
											noGrowDocProc, (void *)-1L, FALSE, 0L);										
	}
	currentTitle[0]=0;
	NumToString(pls->page, numberAsString);
	CopyPStrings(currentTitle, MAC_WINDOW_TITLE);
	CatenatePStrings(currentTitle, numberAsString);
	SetWTitle(plotWindow[plotActive], currentTitle);
	
	boundsRect = plotWindow[plotActive]->portRect;		//  Get current window location

	SetPort(plotWindow[plotActive]);
	SetWRefCon (plotWindow[plotActive], (long)( plotActive ) );  //  Save the window number
 	EraseRect(&plotWindow[plotActive]->portRect);
    ShowWindow (plotWindow[plotActive]);
    BringToFront (plotWindow[plotActive]);
    HiliteWindow (plotWindow[plotActive],TRUE);
	if(plotPicture[plotActive] != NULL)			//  Make sure the associated Picture is free
	{
		KillPicture(plotPicture[plotActive]);
		plotPicture[plotActive]=NULL;
	}
	plotPicture[plotActive]=OpenPicture (&(((GrafPtr) plotWindow[plotActive])->portRect));
	TextFont(monaco);
	TextSize(9);
	TextFace(normal);
	PenNormal();

//  Paint the Background  - use plscolbg(r,g,b) function 

	backGround.red = 257*plsc->cmap0[0].r;
	backGround.green = 257*plsc->cmap0[0].g;
	backGround.blue = 257*plsc->cmap0[0].b;
	RGBForeColor( &backGround );
	PaintRect(&plotWindow[plotActive]->portRect);
	
//  Make sure the colors are set correctly for the beginning of the plot 

	rgbColor.red = 257*plsc->curcolor.r;
	rgbColor.green = 257*plsc->curcolor.g;
	rgbColor.blue = 257*plsc->curcolor.b;
	RGBForeColor( &rgbColor );
	
}

/*----------------------------------------------------------------------*\
* mac_tidy()
*
* Close graphics file or otherwise clean up.
\*----------------------------------------------------------------------*/

void
plD_tidy_mac(PLStream *pls)
{
short	i;

	AdjustMenu();
	while(!MAC_event());			//  Wait for one last time
	RestoreMenu();

	for(i = 0; i < Max_Mac_Windows; i++)
	{
		if(plotWindow[i] != NULL )
			CloseWindow(plotWindow[i]);
	}
}
/*----------------------------------------------------------------------*\
* plD_state_mac()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_mac(PLStream *pls, PLINT op)
{

    switch (op) {

   		case PLSTATE_WIDTH:
			if (pls->width < 1 || pls->width > 10)
	    		pls->width=9;
	    
			PenSize((short) pls->width, (short) pls->width);

		break;
		
    	case PLSTATE_CMAP0:
    	case PLSTATE_CMAP1:
				;			//  Not sure what to do here since clors are used differently
		break;

    	case PLSTATE_COLOR1:
    	case PLSTATE_COLOR0:
			rgbColor.red = 257*plsc->curcolor.r;
			rgbColor.green = 257*plsc->curcolor.g;
			rgbColor.blue = 257*plsc->curcolor.b;
			RGBForeColor( &rgbColor );
		break;
		
    	case PLSTATE_FILL:
				;			//  TBD:  Need to set fill at some point in future
		break;


    }
}

/*----------------------------------------------------------------------*\
* mac_text()
*
* Switch to text mode.
\*----------------------------------------------------------------------*/

void
mac_text(PLStream *pls)
{
}

/*----------------------------------------------------------------------*\
* mac_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/

void
plD_esc_mac(PLStream *pls, PLINT op, void *ptr)
{
int		i;

	switch (op) 
	{
		case PLESC_FILL:
			OpenRgn();								//  Start a Region for a fill
	 		for (i = 0; i < pls->dev_npts - 1; i++)
	 		{
				plD_line_mac(pls, pls->dev_x[i], pls->dev_y[i], 
							 pls->dev_x[i + 1], pls->dev_y[i + 1]);
			}
						 
			CloseRgn(fillRegion);	//  Close the region
			
			PaintRgn(fillRegion);   // Now Paint it
		break;	
	}
}
/*----------------------------------------------------------------------*\
* mac_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/


//private functions

static Boolean MAC_event()
{
	EventRecord	theEvent;
	WindowPtr	whichWindow;
	Rect 		dragRect;
	int 		item,menu, i;
	Str255		daName;
	short		daRefNum;

	SystemTask ();

	if (GetNextEvent (everyEvent, &theEvent))
		switch (theEvent.what)
	  	{
		case mouseDown:
			switch(FindWindow(theEvent.where,&whichWindow))
			{
				case inSysWindow:
					SystemClick(&theEvent,whichWindow);
				break;
				
				case inContent:
					return TRUE;
				break;

				case inDrag:
					SetRect(&dragRect,4,24,qd.screenBits.bounds.right-4,qd.screenBits.bounds.bottom-4);
					DragWindow (whichWindow,theEvent.where, &dragRect);
				break;

				case inMenuBar:
					AdjustMenu();
					HiliteMenu(0);
					return doMenuCommand(MenuSelect(theEvent.where));
				break;
			};
			break;
			
		case keyDown:
		case autoKey:
		    if ((theEvent.modifiers & cmdKey) != 0)
		    {
				AdjustMenu();
				HiliteMenu(0);
		      	return doMenuCommand(MenuKey(theEvent.message & charCodeMask));
			}
			else
				HiliteMenu(0);
				return TRUE;
			break;
			
		case updateEvt:   //  Update all windows
			for(i = 0; i < Max_Mac_Windows; i++)
			{
				if(plotWindow[i] != NULL )
				{
					BeginUpdate(plotWindow[i]);
					SetPort(plotWindow[i]);
					DrawPicture(plotPicture[i],&plotWindow[i]->portRect);
					EndUpdate(plotWindow[i]);
				}
			}
		break;
		    
		case activateEvt:
			InvalRect(&plotWindow[plotActive]->portRect);
			break;
	    }
	    return FALSE;
}

Boolean  doMenuCommand(long mCmd)
{
	int 		item,menu;
	Str255		daName;
	short		daRefNum;

	item=LoWord(mCmd);
	menu=HiWord(mCmd);
	switch(menu)
	{
		case plotM:
			switch(item)
			{
				case PlotCopy:
					doCopy();
					return FALSE;
				break;
				case PlotSave:
					PutPictToFile();
					return FALSE;
				break;
				case PlotContinue:
					return TRUE;
				break;
			}
			break;
	}
	return FALSE;
	HiliteMenu(0);
	AdjustMenu();
}
/**************************** doCopy *************************/

void doCopy()
{
short		frontWindow;
WindowPtr	tempWindow;

	tempWindow = FrontWindow();
	frontWindow = (short) GetWRefCon(tempWindow);  //  Get the window number

	ZeroScrap();
	HLock((Handle) plotPicture[frontWindow]);
	PutScrap(GetHandleSize((Handle)plotPicture[frontWindow]) ,'PICT', *plotPicture[frontWindow]);
	HUnlock((Handle) plotPicture[frontWindow]);
}
/**************************** SetUpMenu *************************/

void SetUpMenu(void)
{
	MenuHandle mh;
	
	saveMBar=GetMenuBar();
//	ClearMenuBar();
//	mh=NewMenu(appleM,"\p\024");
//	AppendMenu(mh, "\pAbout\311");
//	InsertMenu(mh,0);
//	AddResMenu(GetMHandle(appleM), 'DRVR');	/* add DA names to Apple menu */
	mh=NewMenu(plotM,"\pPLplot");
	AppendMenu(mh, "\pCopy To Clipboard/C;Save To File/S;(-;Continue/G");
	InsertMenu(mh,0);
	DrawMenuBar();
	myMBar=GetMenuBar();
}

 void AdjustMenu(void)
 {
	SetMenuBar(myMBar);
	DrawMenuBar();
}

 void RestoreMenu(void)
 {
	SetMenuBar(saveMBar);
	DrawMenuBar();
}

// routines to create PICT file.

/*------ PutPictToFile ----------------------------------------------------------------------------*/


//void PutPictToFile(PicHandle thePicture)
void PutPictToFile(void)
{
SFReply		tr;
short		rc, fRefNum, count;
long 		inOutCount;
Point		where;
unsigned char header[512];
OSErr 		myError;
short		frontWindow;
//WindowPtr	tempWindow;
Str255		tempTitle;

//	tempWindow = FrontWindow();
	frontWindow = (short) GetWRefCon(FrontWindow());  //  Get the window number
	if(frontWindow != plotActive)
		GetWTitle(plotWindow[frontWindow], tempTitle);
	else
		BlockMove(tempWTitle, tempTitle, (long) tempWTitle[0]+1);

	
	for (count = 0; count < 512; count++)
		header[count] = 0x00;
	
	where.h=100; where.v=50;     				/* where the  Standard File dialog window goes */
	
	SFPutFile( where, "\pSave PICT as:", tempTitle, NULL, &tr );
	
	if ( tr.good ) {
		myError = Create(tr.fName, tr.vRefNum, '????', 'PICT');
		if (myError == dupFNErr) 
		{
			myError = FSDelete(tr.fName,tr.vRefNum);
			myError = Create(tr.fName, tr.vRefNum, '????', 'PICT');
		}										/* this is quick 'n' dirty or there'd 
														be more robust handling here */
		
		myError = FSOpen( tr.fName, tr.vRefNum, &fRefNum );
		if ( myError == noErr ) 
		{ 
			inOutCount = 512;
			myError = FSWrite(fRefNum, &inOutCount, header);		/* write the header */
			HLock((Handle)plotPicture[frontWindow]);
			if (myError == noErr) 
			{									/* don't write if error the first time */
				inOutCount = GetHandleSize((Handle)plotPicture[frontWindow]);
				myError == FSWrite(fRefNum,&inOutCount,*plotPicture[frontWindow]);
			}
			FSClose( fRefNum );					/* close it */
			HUnlock((Handle)plotPicture[frontWindow]);
		}
	}
}
/**************************** DoAbout *************************/
void
DoAbout( Str255 s )
{
#define BASE_RES_ID			400
#define	NIL_POINTER			0L

	ParamText( s, "\p", "\p", "\p" );
	NoteAlert( BASE_RES_ID, NIL_POINTER );
}
/**************************** DoAbout *************************/
//  Routine to place the plot area on the screen --- 
//  Defualt: boundsRect = {50, 10, 650, 610}
//
//  This utility will not allow plot to be smaller than 200 x 200
//  or larger than 600 x 600

void
mac_plot_area(Rect *plotArea)
{
//   Limits set by Screenbits --  Comemnted out

//	plotArea->left = (plotArea->left > 10) ? plotArea->left : 10;
//	plotArea->top = (plotArea->top > 40) ? plotArea->top : 40;
//	plotArea->right= (plotArea->right > qd.screenBits.bounds.right) ? 
//					qd.screenBits.bounds.right-10 : plotArea->right;
//	plotArea->bottom= (plotArea->bottom > qd.screenBits.bounds.bottom) ? 
//					qd.screenBits.bounds.bottom-10 : plotArea->bottom;					

//  XMAX and YMAX limits
//
	plotArea->left = (plotArea->left > 10) ? plotArea->left : 10;
	plotArea->top = (plotArea->top > 40) ? plotArea->top : 40;
	plotArea->right= (plotArea->right > plotArea->left+MAC_XMAX) ? 
					plotArea->left+MAC_XMAX : plotArea->right;
	plotArea->bottom= (plotArea->bottom > plotArea->top+MAC_YMAX) ? 
					plotArea->top+MAC_YMAX : plotArea->bottom;
					
//  Make sure it's at least 200 x 200 as minimum

	if(plotArea->right-plotArea->left < 200) 
		plotArea->right = plotArea->left+200;
	
	if(plotArea->bottom-plotArea->top < 200) 
		plotArea->bottom = plotArea->top+200;
		
	SetRect(&boundsRect, plotArea->left, plotArea->top, plotArea->right, plotArea->bottom);
	plotWidth = plotArea->right - plotArea->left;
	plotHeight = plotArea->bottom - plotArea->top;
}

/*******************************************************************************

 	CatenatePStrings

	Catenate two Pascal strings by attaching the second string on the end of
	the first string. If you are running under MPW 3.2 or later, you can
	simply use the PLCatenate library routine instead.

*******************************************************************************/
void	
CatenatePStrings(Str255 targetStr, Str255 appendStr)
{
	short appendLen;

	/* Figure out number of bytes to copy, up to 255 */

	appendLen = MIN(appendStr[0], 255 - targetStr[0]);
	if (appendLen > 0) {
		BlockMove(appendStr+1, targetStr+targetStr[0]+1, (long) appendLen);
		targetStr[0] += appendLen;
	}
}
/*******************************************************************************

 	CopyPStrings

	Copies two Pascal strings by attaching the second string on the end of
	the first string. If you are running under MPW 3.2 or later, you can
	simply use the PLCatenate library routine instead.

*******************************************************************************/
void	
CopyPStrings(Str255 targetStr, Str255 appendStr)
{
	short appendLen;

	/* Figure out number of bytes to copy, up to 255 */

		BlockMove(appendStr, targetStr, (long) appendStr[0]+1);
//		targetStr[0];
}


