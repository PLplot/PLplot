/* $Id$
   $Log$
   Revision 1.3  2000/12/18 21:01:50  airwin
   Change to new style plplot/*.h header file locations.

   Revision 1.2  2000/05/12 18:05:32  furnish
   Imported latest Mac port work by Rob Managan.

 * Revision 1.1  1995/04/11  09:39:13  mjl
 * Mac port, contributed by Mark Franz.
 *
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
  Compile this with MetroWerks C/C++ 

*/
#include "plplot/plplotP.h"
#include <stdio.h>
#include <stdlib.h>
#include "plplot/drivers.h"
#include "plplot/mac.h"

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
static int			Max_Mac_Windows_Used;
static WindowPtr	plotWindow[Max_Mac_Windows];
static PicHandle 	plotPicture[Max_Mac_Windows];
static Handle 		saveMBar, myMBar;
static MenuHandle	myMenu;
RGBColor			rgbColor;
long 				PICTCount;
short 				globalRef; 
short				plotActive = 0;
PicHandle 			newPICTHand;
short				plotWidth = MAC_XMAX;
short				plotHeight = MAC_YMAX;
static	GrafPtr	savedPort;


static Rect	boundsRect = {40, 10, MAC_YMAX+40, MAC_XMAX+10};

/*----------------------------------------------------------------------*\
* mac_init()
*
* Initialize device.
\*----------------------------------------------------------------------*/
 
void
plD_init_mac1(PLStream *pls)
{	
	Max_Mac_Windows_Used = 1;
	init_mac(pls);
}
void
plD_init_mac8(PLStream *pls)
{	
	Max_Mac_Windows_Used = 8;
	init_mac(pls);
}
void
init_mac(PLStream *pls)
{	
	int		i;

	for(i = 0; i < Max_Mac_Windows_Used; i++)
	{
		plotWindow[i] = NULL;
		plotPicture[i] = NULL;
	}
	plotActive = -1;

	fprintf(stderr, "Macintosh Window\n");  // This is here just to
											// make sure Menus drawn
	
	SetUpMenu();

	 pls->termin = 1;            /* is an interactive terminal */
	 pls->icol0 = 1;
	 pls->icol1 = 1;
	 pls->width = 1;
	 pls->bytecnt = 0;
	 pls->page = 0;
	 pls->dev_fill0 = 1;		/* supports hardware solid fills */
	 pls->dev_fill1 = 0;
//    pls->plbuf_write = 1;	/* Activate plot buffer */
	 
	 pls->graphx = GRAPHICS_MODE;

	 if (!pls->colorset)
 		pls->color = 1;
 		

/* Set up device parameters */

	 plP_setpxl(2.83465, 2.83465);           /* Pixels/mm. */

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
	int y1, y2 ;

	GetPort(&savedPort);
	SetPort(plotWindow[plotActive]);

	y1 = (boundsRect.bottom-boundsRect.top) - y1a;
	y2 = (boundsRect.bottom-boundsRect.top) - y2a;

	 MoveTo(x1a, y1);
	 LineTo(x2a, y2);

	 SetPort(savedPort);
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

	GetPort(&savedPort);
	SetPort(plotWindow[plotActive]);

	 for (i = 0; i < npts - 1; i++)
	plD_line_mac(pls, xa[i], ya[i], xa[i + 1], ya[i + 1]);

	 SetPort(savedPort);
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
	
	GetPort(&savedPort);
	SetPort(plotWindow[plotActive]);

	ClosePicture();
	DrawPicture(plotPicture[plotActive],&plotWindow[plotActive]->portRect);

	if(pls->termin == 1)
	{
		if(plotWindow[plotActive] !=NULL)  // added subscript for current window RAM 10/14/96
		{
			GetWTitle(plotWindow[plotActive], tempWTitle);
			SetWTitle(plotWindow[plotActive], "\pHit any key to continue.");
			InvalRect(&plotWindow[plotActive]->portRect);
			AdjustMenu();
			if (Max_Mac_Windows_Used != 1)
			{
		  		while(!MAC_event(pls));
				SetWTitle(plotWindow[plotActive], tempWTitle);
			}
//			RestoreMenu();
		}
	}
	else
	{
		if(plotPicture[plotActive] != NULL)
		{	
			AdjustMenu();
			myRect=plotWindow[plotActive]->portRect;
			DrawPicture(plotPicture[plotActive],&myRect);
//			RestoreMenu();
		}
	}

	 SetPort(savedPort);
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

	pls->page++;

	plotActive++;
	
	if(plotActive == Max_Mac_Windows_Used) 
		plotActive =0;
		
	if (plotWindow[plotActive] == NULL)					//  Get a window handle
	{
		boundsRect.top 	= 40+plotActive*20;				//  Set the inital window location
		boundsRect.left = (plotActive+1)*10;
		boundsRect.right = plotWidth+(plotActive+1)*10;
		boundsRect.bottom = plotHeight+40+plotActive*20;
		
		plotWindow[plotActive] = NewCWindow(NULL, &boundsRect,"\p ", FALSE, 
											documentProc, (void *)-1L, FALSE, 0L);										
	}
	currentTitle[0]=0;
	NumToString(pls->page, numberAsString);
	CopyPStrings(currentTitle, MAC_WINDOW_TITLE);
	CatenatePStrings(currentTitle, numberAsString);
	SetWTitle(plotWindow[plotActive], currentTitle);
	
	boundsRect = plotWindow[plotActive]->portRect;		//  Get current window location

	GetPort(&savedPort);
	SetPort(plotWindow[plotActive]);
	SetWRefCon(plotWindow[plotActive], (long)( plotActive ) );  //  Save the window number
 	EraseRect(&plotWindow[plotActive]->portRect);
    ShowWindow(plotWindow[plotActive]);
	SelectWindow(plotWindow[plotActive]);	// RAM 10/9/96
	if(plotPicture[plotActive] != NULL)			//  Make sure the associated Picture is free
	{
		SetWindowPic(plotWindow[plotActive], NULL);
		KillPicture(plotPicture[plotActive]);
		plotPicture[plotActive]=NULL;
	}
	plotPicture[plotActive]=OpenPicture(&(((GrafPtr) plotWindow[plotActive])->portRect));
	TextFont(kFontIDMonaco);
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
	

	 SetPort(savedPort);
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

	GetPort(&savedPort);
	SetPort(plotWindow[plotActive]);

//	AdjustMenu();
//	while(!MAC_event(pls));			//  Wait for one last time
//	RestoreMenu();

	for(i = 0; i < Max_Mac_Windows_Used; i++)
	{
		if(plotWindow[i] != NULL )
			DisposeWindow(plotWindow[i]);
	}

	 SetPort(savedPort);
}
/*----------------------------------------------------------------------*\
* plD_state_mac()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*----------------------------------------------------------------------*/

void 
plD_state_mac(PLStream *pls, PLINT op)
{

	GetPort(&savedPort);
	SetPort(plotWindow[plotActive]);

    switch (op) {

   		case PLSTATE_WIDTH:
			if (pls->width < 1 || pls->width > 10)
	    		pls->width=9;
	    
			PenSize((short) pls->width, (short) pls->width);

		break;
		
    	case PLSTATE_CMAP0:
    	case PLSTATE_CMAP1:
				;			//  Not sure what to do here since colors are used differently
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

	 SetPort(savedPort);
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
 *
 * Functions:
 *
 *	PLESC_EH	Handle pending events
 *	PLESC_EXPOSE	Force an expose
 *	PLESC_FILL	Fill polygon
 *	PLESC_FLUSH	Flush X event buffer
 *	PLESC_GETC	Get coordinates upon mouse click
 *	PLESC_REDRAW	Force a redraw
 *	PLESC_RESIZE	Force a resize
\*----------------------------------------------------------------------*/

void
plD_esc_mac(PLStream *pls, PLINT op, void *ptr)
{
int		i;

	GetPort(&savedPort);
	SetPort(plotWindow[plotActive]);

	switch (op) 
	{
	  case PLESC_EXPOSE:	/* handle window expose */
	    break;
	  case PLESC_RESIZE:	/* handle window resize */
	    break;
	  case PLESC_REDRAW:	/* handle window redraw */
	    break;
	  case PLESC_TEXT:		/* switch to text screen */
	    break;
	  case PLESC_GRAPH:		/* switch to graphics screen */
	    break;
	  case PLESC_FILL:		/* fill polygon */
			OpenRgn();								//  Start a Region for a fill
	 		for (i = 0; i < pls->dev_npts - 1; i++)
	 		{
				plD_line_mac(pls, pls->dev_x[i], pls->dev_y[i], 
							 pls->dev_x[i + 1], pls->dev_y[i + 1]);
			}
						 
			CloseRgn(fillRegion);	//  Close the region
			
			PaintRgn(fillRegion);   // Now Paint it
		break;
	  case PLESC_DI:		/* handle DI command */
	    break;
	  case PLESC_FLUSH:		/* flush output */
	    break;
	  case PLESC_EH:        /* handle Window events */
	    break;
	  case PLESC_GETC:		/* get cursor position */
	    break;
	  case PLESC_SWIN:		/* set window parameters */
	    break;
	}

	 SetPort(savedPort);
}
/*----------------------------------------------------------------------*\
* mac_esc()
*
* Escape function.
\*----------------------------------------------------------------------*/


//private functions

static unsigned char MAC_event(PLStream *pls)
{
	EventRecord	theEvent;
	WindowPtr	whichWindow;
	Rect 		dragRect, growRect;
	int 		i, growResult;
	short		newWidth, newHeight;

	SystemTask();

	if (GetNextEvent(everyEvent, &theEvent))
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
					DragWindow(whichWindow,theEvent.where, &dragRect);
					break;
				case inGrow:
					SetRect(&growRect,128,128,32767,32767);
					growResult = GrowWindow(whichWindow,theEvent.where, &growRect);
					if (growResult != 0)
					{
						newWidth  = LoWord(growResult);
						newHeight = HiWord(growResult);
						ResizeWin(pls, whichWindow,newWidth,newHeight);
					}
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
			
		case updateEvt:   //  Update a window
			whichWindow = (WindowPtr)theEvent.message;

			i = GetWRefCon(whichWindow);
			if (i >= 0 && i < Max_Mac_Windows_Used)
			{
				BeginUpdate(plotWindow[i]);
				GetPort(&savedPort);
				SetPort(plotWindow[i]);
				DrawPicture(plotPicture[i],&plotWindow[i]->portRect);
				SetPort(savedPort);
				EndUpdate(plotWindow[i]);
			}
		break;
		    
		case activateEvt:
//			InvalRect(&plotWindow[plotActive]->portRect);
			break;
	    }
	    return FALSE;
}

void	ResizeWin(	PLStream *pls,
					WindowPtr whichWindow,
					short newWidth,
					short newHeight)
{
	int			i, myWindow = Max_Mac_Windows_Used;

	i = GetWRefCon(whichWindow);
	if (i >= 0 && i < Max_Mac_Windows_Used)
	{
		myWindow = i;
	}
	if (Max_Mac_Windows_Used == myWindow)
		return;
	SizeWindow(whichWindow, newWidth, newHeight, true);
	InvalRect(&whichWindow->portRect); // make sure whole window is redrawn in the update

	if (myWindow == plotActive)
	{
		plotWidth = newWidth;
		plotHeight = newHeight;
		plP_setphy((PLINT) 0, (PLINT) (newWidth), (PLINT) 0, (PLINT) (newHeight));
	}
}

unsigned char  doMenuCommand(long mCmd)
{
	unsigned char	result=FALSE;
	int 		item,menu;
//	Str255		daName;
//	short		daRefNum;

	item=LoWord(mCmd);
	menu=HiWord(mCmd);
	switch(menu)
	{
		case plotM:
			switch(item)
			{
				case PlotCopy:
					doCopy();
					result = FALSE;
				break;
				case PlotSave:
					PutPictToFile();
					result = FALSE;
				break;
				case PlotContinue:
					result = TRUE;
				break;
			}
			break;
	}
	HiliteMenu(0);
	AdjustMenu();
	return result;
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
	
	saveMBar=GetMenuBar();
//	ClearMenuBar();
//	myMenu=NewMenu(appleM,"\p\024");
//	AppendMenu(myMenu, "\pAbout\311");
//	InsertMenu(myMenu,0);
//	AddResMenu(GetMHandle(appleM), 'DRVR');	/* add DA names to Apple menu */
	myMenu=NewMenu(plotM,"\pPLplot");
	AppendMenu(myMenu, "\pCopy To Clipboard/C;Save To File/S;(-;Continue/G");
	InsertMenu(myMenu,0);
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
	DisposeMenu(myMenu);
	DisposeHandle(myMBar);
	DisposeHandle(saveMBar);
}

// routines to create PICT file.

/*------ PutPictToFile ----------------------------------------------------------------------------*/


//void PutPictToFile(PicHandle thePicture)
void PutPictToFile(void)
{
	SFReply		tr;
	short		fRefNum, count;
	long 		inOutCount;
	Point		where;
	unsigned char header[512];
	OSErr 		myError;
	short		frontWindow;
	Str255		tempTitle;

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
		myError = Create(tr.fName, tr.vRefNum, 'CDrw', 'PICT');
		if (myError == dupFNErr) 
		{
			myError = FSDelete(tr.fName,tr.vRefNum);
			myError = Create(tr.fName, tr.vRefNum, 'CDrw', 'PICT');
		}							/* this is quick 'n' dirty or there'd 
										be more robust handling here */
		
		myError = FSOpen( tr.fName, tr.vRefNum, &fRefNum );
		if ( myError == noErr ) 
		{ 
			inOutCount = 512;
			myError = FSWrite(fRefNum, &inOutCount, header);		/* write the header */
			HLock((Handle)plotPicture[frontWindow]);
			if (myError == noErr) 
			{						/* don't write if error the first time */
				inOutCount = GetHandleSize((Handle)plotPicture[frontWindow]);
				myError = FSWrite(fRefNum,&inOutCount,*plotPicture[frontWindow]);
			}
			FSClose( fRefNum );		/* close it */
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
//	short appendLen;

	/* Figure out number of bytes to copy, up to 255 */

		BlockMove(appendStr, targetStr, (long) appendStr[0]+1);
//		targetStr[0];
}


/*--------------------------------------------------------------------------*\
 * FILE *plMacLibOpen(fn)
 *
 * Return file pointer to lib file.
 * Locations checked:
 *	In the system Preferences folder we check for
 *  the lib folder or a plplot_lib folder. The latter may be an alias
\*--------------------------------------------------------------------------*/
#include <Folders.h>
#include <Files.h>
//#include <MoreFilesExtras.h>
//#include <FSpCompat.h>
#include <FSp_fopen.h>

FILE *plMacLibOpen(char *fn)
{
	FILE	*file;
	char	*fs = NULL;
	OSErr	anErr;
	short	prefVRefNum;
	long	prefDirID;
	FSSpec	FSlibFileDir, FSlibFile;
	Boolean	resolveAliasChains=TRUE, targetIsFolder, wasAliased;
	
	/* find the preferences folder */
	anErr = FindFolder( kOnSystemDisk, 'pref', kDontCreateFolder , &prefVRefNum, &prefDirID);
	// first see if the plplot_lib folder is here; alias or not
	anErr = FSMakeFSSpec(prefVRefNum, prefDirID, "\pplplot_lib", &FSlibFileDir);
	if ( (anErr == noErr) && (FSlibFileDir.parID == 0) )
		FSlibFileDir.parID = fsRtParID;
	// if it is an alias get what it points to!
	anErr = ResolveAliasFile(&FSlibFileDir, resolveAliasChains, &targetIsFolder, &wasAliased);
	// make FSSpec for file within the directory ; 1st construct the relative filename
	// then make the FSSpec; then open it!
	plGetName("", p2cstr(FSlibFileDir.name), fn, &fs);
	anErr = FSMakeFSSpec(FSlibFileDir.vRefNum, FSlibFileDir.parID, c2pstr(fs), &FSlibFile);
	if ( (anErr == noErr) && (FSlibFile.parID == 0) )
		FSlibFile.parID = fsRtParID;
	file = FSp_fopen(&FSlibFile, "rb");
/*    if (file != NULL)
    {
	    free_mem(fs);
		return (file);
	}
*/	
    free_mem(fs);
    return (file);
}

