/*
win3.cpp

  Driver for Win32

	Paul Casteels	11-Sep-1996	(casteels@uia.ua.ac.be)
	
	  Color Handling by
	
		Carla Carmelo Rosa  (l36658@alfa.ist.utl.pt)
		
		
  Modified 20/12/01 by Olof Svensson and Alexandre Gobbo

  Modified 26/12/05 by Arjen Markus: copied anti-aliasing font
  handling from wingcc driver
			
*/

#include "plDevs.h"
#ifdef PLD_win3

#include <stdio.h>
#include <string.h>
#include "plplotP.h"
#include "drivers.h"
#include "resource.h"
#include <windows.h>
#include <assert.h>

static int			color = 1;
static unsigned int hwnd = 0;
static unsigned int buffered = 1;
/* Initial window size */
static int nWidth  = 720;
static int nHeight = 540;
/* Initial window position */
static int xPos    = 100;
static int yPos    = 100;	
	
#ifdef HAVE_FREETYPE
/*
 *  Original text:
 *  Freetype support has been added to the wingcc driver using the
 *  plfreetype.c module, and implemented as a driver-specific optional extra
 *  invoked via the -drvopt command line toggle. It uses the
 *  "PLESC_HAS_TEXT" command for rendering within the driver.
 *
 *  Freetype support is turned on/off at compile time by defining
 *  "HAVE_FREETYPE".
 *
 *  To give the user some level of control over the fonts that are used,
 *  environmental variables can be set to over-ride the definitions used by
 *  the five default plplot fonts.
 *
 *  Freetype rendering is used with the command line "-drvopt text".
 *  Anti-aliased fonts can be used by issuing "-drvopt text,smooth"
 */

#include "plfreetype.h"

#ifndef max_number_of_grey_levels_used_in_text_smoothing
#define max_number_of_grey_levels_used_in_text_smoothing 64
#endif

static int freetype=0;
static int smooth_text=0;
static int save_reg=0;
#endif

static DrvOpt    win3_options[] = {
	{"color", DRV_INT, &color, "Use color (color=0|1)"},
	{"hwnd", DRV_INT, &hwnd, "Windows HWND handle (not supposed to be given as a command line argument)"},
	{"buffered", DRV_INT, &buffered, "Sets buffered operation"},
#ifdef HAVE_FREETYPE
	{"text", DRV_INT, &freetype, "Use driver text (FreeType)"},
	{"smooth", DRV_INT, &smooth_text, "Turn text smoothing on (1) or off (0)"},
	{"save", DRV_INT, &save_reg, "Save defaults to registary"},
#endif
	{NULL, DRV_INT, NULL, NULL}
};

LRESULT CALLBACK __declspec(dllexport) PlPlotWndProc (HWND hwnd,UINT message,UINT wParam,LONG lParam);

static const char szPlPlotClass[] = "PlplotClass";
static const char szPlPlotWName[] = "Plplot Window";

/* Function prototypes */

void plD_init_win3(PLStream *pls);
void plD_line_win3(PLStream *pls, short x1a, short y1a, short x2a, short y2a);
void plD_polyline_win3(PLStream *pls, short *xa, short *ya, PLINT npts);
void plD_eop_win3(PLStream *pls);
void plD_bop_win3(PLStream *pls);
void plD_tidy_win3(PLStream *pls);
void plD_state_win3(PLStream *pls, PLINT op);
void plD_esc_win3(PLStream *pls, PLINT op , void *ptr);
void FillPolygonCmd (PLStream *pls);
void plD_DrawImage_win3(PLStream *pls);
static void imageops(PLStream *pls, PLINT *ptr);
/* BOOL CALLBACK AbortProc( HDC hDC, int Error ); */

#ifdef HAVE_FREETYPE

static void plD_pixel_win3 (PLStream *pls, short x, short y);
static void plD_pixelV_win3 (PLStream *pls, short x, short y);
static void init_freetype_lv1 (PLStream *pls);
static void init_freetype_lv2 (PLStream *pls);

extern "C" {
extern void plD_FreeType_init(PLStream *pls);
extern void plD_render_freetype_text (PLStream *pls, EscText *args);
extern void plD_FreeType_Destroy(PLStream *pls);
extern void pl_set_extended_cmap0(PLStream *pls, int ncol0_width, int ncol0_org);
}

static int GetRegValue(char *key_name, char *key_word, char *buffer, int size);
static int SetRegValue(char *key_name, char *key_word, char *buffer,int dwType, int size);


#define SetRegStringValue(a,b,c) SetRegValue(a, b, c, REG_SZ, strlen(c)+1 )
#define SetRegBinaryValue(a,b,c,d) SetRegValue(a, b, (char *)c, REG_BINARY, d )
#define SetRegIntValue(a,b,c) SetRegValue(a, b, (char *)c, REG_DWORD, 4 )
#define GetRegStringValue(a,b,c,d) GetRegValue(a, b, c, d)
#define GetRegIntValue(a,b,c) GetRegValue(a, b, (char *)c, 4)
#define GetRegBinaryValue(a,b,c,d) GetRegValue(a, b, (char *)c, d )

#endif

void plD_dispatch_init_win3	( PLDispatchTable *pdt )
{
	pdt->pl_MenuStr  = "PLplot Win32 Window";
	pdt->pl_DevName  = "win3";
	pdt->pl_type     = plDevType_Interactive;
	pdt->pl_seq      = 10;
	pdt->pl_init     = (plD_init_fp)     plD_init_win3;
	pdt->pl_line     = (plD_line_fp)     plD_line_win3;
	pdt->pl_polyline = (plD_polyline_fp) plD_polyline_win3;
	pdt->pl_eop      = (plD_eop_fp)      plD_eop_win3;
	pdt->pl_bop      = (plD_bop_fp)      plD_bop_win3;
	pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_win3;
	pdt->pl_state    = (plD_state_fp)    plD_state_win3;
	pdt->pl_esc      = (plD_esc_fp)      plD_esc_win3;
}

typedef struct {
	HWND	hwnd;
	HMENU 	hMenu;
	HANDLE  hThread;

	LOGPEN 	lp;
	LOGBRUSH lb;
	HPEN	hPen, hPenOld;
	HDC 	hdc;
	HPALETTE 	hpal;		
	HBRUSH	hbr, hbrOld;

	float 	xScale,yScale;
	int     xPhMax,yPhMax;
	int 	nextPlot; 	
	int 	rePaint;          /* if the background is cleared we need a repaint */
	int 	rePaintBusy;      /* if we are repainting block the rest */
	int     externalWindow;   /* if true the window is provided externally */
	
	int		write_to_window;	/* Set if plotting direct to window */
    int		write_to_pixmap;	/* Set if plotting to pixmap */

	int 	newCursor, button, state;
	float 	cursorX,cursorY;

	HBITMAP db_bmp;		
	HDC		db_hdc;		
	long	PenColor;
	int		PenWidth;
	long    backGroundColor;

    bool    isDead;	
	bool    isIdle;			// Flag indicating that the window is idle (i.e. before bop)
} WinDev;

/*
static HDC GetPrinterDC(void)
{
PRINTDLG pdlg;
BOOLEAN ok;

  // Initialize the PRINTDLG structure
  memset( &pdlg, 0, sizeof( PRINTDLG ) );
  pdlg.lStructSize = sizeof( PRINTDLG );
  // Set the flag to return printer DC
  pdlg.Flags = PD_RETURNDC;

    // Invoke the printer dialog box
    ok = PrintDlg( &pdlg );
    // hDC member of the PRINTDLG structure contains
    // the printer DC
    return pdlg.hDC;
	}
*/

/*--------------------------------------------------------------------------*\
* Provide a solution to the problem of temporary files (AM)
\*--------------------------------------------------------------------------*/

extern "C" {

FILE *tmpfile( void )
{
	FILE *outfile ;
	char *string  ;
	char *ptemp   ;
	char  buffer[100] ;
	char  buffer2[100] ;

	ptemp = getenv( "TEMP" ) ;
	if ( ptemp == NULL ) {
		ptemp = getenv( "TMP" ) ;
	}
	if ( ptemp == NULL ) {
		ptemp = "C:" ;
	}
	string  = tmpnam( buffer ) ;
	strcpy( buffer2, ptemp ) ;
	strcat( buffer2, string ) ;
	outfile = fopen( buffer2, "wb+" ) ;

	return outfile ;
}
}

/* Create a plot window
 */
void create_plot_window(PLStream *pls)
{
	HINSTANCE hInstance;
	WNDCLASS  wndclass;
	WinDev    *dev = (WinDev *)pls->dev;
	char      *ptitle;
		
	hInstance = GetModuleHandle(NULL);
		
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_SAVEBITS;
	wndclass.lpfnWndProc = ::PlPlotWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = sizeof(pls);
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance,"PLICON");
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground = (struct HBRUSH__ *)CreateSolidBrush(dev->backGroundColor);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szPlPlotClass;
	RegisterClass (&wndclass);
	
	ptitle = (char *) &szPlPlotWName[0] ;
	if ( pls->plwindow ) ptitle = pls->plwindow ;
		
	dev->hwnd = CreateWindow(
		szPlPlotClass, ptitle,
		WS_OVERLAPPEDWINDOW,
		xPos, yPos, nWidth, nHeight,
		NULL, dev->hMenu,
		hInstance, NULL);
	

	SetWindowLong(dev->hwnd, GWL_USERDATA, (long)pls);
		
	ShowWindow(dev->hwnd,SW_SHOWDEFAULT);
		
	SetForegroundWindow(dev->hwnd);
}

#ifdef _MT_NOT_WORKING_YET
/* Multi-threaded support is enable.  This permits a much better handling of the plot windows,
 * particularly if nopause is true.
 */

/* This function is passed to the CreateThread call in order to create a plot window and
 * process messages for that window.  This will allow the message queue to be processed all
 * the time instead of only when eop() is called.  Without the thread, the message queue is
 * blocked and that can cause problems (e.g. things get plotted in the wrong window if a window
 * is moved and there are multiple plots)
 */
DWORD WINAPI message_handler( LPVOID lpParam )
{
	PLStream *pls = (PLStream *)lpParam;
	WinDev *dev = (WinDev *)pls->dev;
	MSG msg;
    BOOL bRet;

	// Create the plot window
	create_plot_window(pls);

	do {
		printf("Thread(%lx) Waiting for message on %lx\n",
			dev->hThread, dev->hwnd);
		bRet = GetMessage(&msg, dev->hwnd, 0, 0);
		
		if(bRet == -1) {
			/* Error getting message */
		} else if(bRet != 0) {
			/* Successfully got a message */
			printf("Got message for thread %lx\n", dev->hThread);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while(1);

    return(0);
}

/* Creates a window in a seperate thread so that messages can be processed without
 * blocking the main execution thread.  If the thread creation fails, we degrade
 * gracefully by going to a non-threaded implementation.
 */
void create_window_thread(PLStream *pls)
{
	DWORD dwThreadId;
	WinDev *dev = (WinDev *)pls->dev;

    dev->hThread = CreateThread(
        NULL,                        // default security attributes
        0,                           // use default stack size
        message_handler,             // thread function
        pls,                         // argument to thread function
        0,                           // use default creation flags
        &dwThreadId);                // returns the thread identifier

   // Check the return value for success.
   if (dev->hThread == NULL) {
      // Thread creation failed
   }
   printf("Created thread %lx\n", dev->hThread);
}
#endif

/*--------------------------------------------------------------------------*\
* Initialize device.
\*--------------------------------------------------------------------------*/


void plD_init_win3(PLStream *pls)
{
	HWND      hwndMain;
	RECT      rect;
	WinDev    *dev;
	int       greyvalue;
	
	int xmin = 0;
	int xmax = PIXELS_X-1;
	int ymin = 0;
	int ymax = PIXELS_Y-1;

#ifdef HAVE_FREETYPE
    FT_Data *FT;

	/*
	 *  Variables used for reading the registary keys
	 *  might eventually add a user defined pallette here, but for now it just does freetype
	 */
    char key_name[] = "Software\\PLplot\\win32";
    char Keyword_text[] = "freetype";
    char Keyword_smooth[] = "smooth";

	/*
	 *  Read registry to see if the user has set up default values
	 *  for text and smoothing. These will be overriden by anything that
	 *  might be given on the command line, so we will load the
	 *  values right into the same memory slots we pass to plParseDrvOpts
	 */
    GetRegIntValue(key_name, Keyword_text, &freetype);
    GetRegIntValue(key_name, Keyword_smooth, &smooth_text);
#endif

	// Set default values for the driver options
	hwnd = 0;
	color = 1;
	pls->color = 1;		// Is a color device

	// Parse the driver options
	plParseDrvOpts(win3_options);

#ifdef HAVE_FREETYPE
	/*
	 *  We will now save the settings to the registry if the user wants
	 */
    pls->FT = NULL ;

    if (save_reg==1) {
        SetRegIntValue(key_name, Keyword_text, &freetype);
        SetRegIntValue(key_name, Keyword_smooth, &smooth_text);
    }
#endif

	/* Set up device parameters */
	pls->termin      = 1; /* is an interactive terminal */
	pls->icol0       = 1; /* current color */
	pls->width       = 1; /* current pen width */
	pls->bytecnt     = 0;
	pls->page        = 0;
	if (!color) pls->color = 0; /* But user does not want color */	
	if (buffered)
		pls->plbuf_write = 1; /* buffer the output */
	else
		pls->plbuf_write = 0;
	pls->dev_flush   = 1; /* flush as we like */
	pls->dev_fill0   = 1;	
	pls->dev_fastimg = 1; /* is a fast image device */
	pls->dev_xor     = 1; /* device support xor mode */

	if (pls->dev != NULL) delete pls->dev;
	pls->dev = new WinDev;
	assert(pls->dev != NULL);
	
	dev = (WinDev *) pls->dev;
	dev->nextPlot = 0;
	dev->write_to_window = 1;
	dev->write_to_pixmap = 0;
	dev->PenColor=RGB(pls->cmap0[0].r,pls->cmap0[0].g,pls->cmap0[0].b);
	dev->PenWidth=0;
	
	dev->hPen     = CreatePen(PS_SOLID,dev->PenWidth,dev->PenColor);
	dev->hPenOld = (HPEN)SelectObject(dev->hdc,dev->hPen);
	dev->hbr      = CreateSolidBrush(RGB(pls->cmap0[0].r,pls->cmap0[0].g,pls->cmap0[0].b));
	dev->hbrOld   = (HBRUSH)SelectObject(dev->hdc,dev->hbr);
	dev->hMenu    = NULL;

    dev->isDead   = FALSE;
	dev->isIdle   = TRUE;

	if (pls->color) {
		dev->backGroundColor = RGB(pls->cmap0[0].r,pls->cmap0[0].g,pls->cmap0[0].b);
	} else {
		greyvalue = (pls->cmap0[0].r+pls->cmap0[0].g+pls->cmap0[0].b)/3;
		dev->backGroundColor = RGB(greyvalue,greyvalue,greyvalue);
	}

	if (!hwnd) {
		/* Window created by the driver */
		dev->externalWindow = 0;
		dev->hwnd = NULL;
		dev->hThread = NULL;

#ifdef _MT_NOT_WORKING_YET
		create_window_thread(pls);

		// Wait until the window is created
		while(dev->hwnd == NULL) ;
#else
		create_plot_window(pls);
#endif		
	} else {
		/* Window provided externally */		
		dev->hwnd = (HWND)hwnd;
		dev->hThread = NULL;

		dev->externalWindow = 1;
	}

	// Get the display device context for the clien area of the window
	dev->hdc = GetDC(dev->hwnd);

	SetPolyFillMode(dev->hdc, WINDING);
	
	plP_setpxl(xmax/150.0/nWidth*nHeight, ymax/150.0);
	plP_setphy(xmin,xmax,ymin,ymax);

	if (pls->db) {
     	// create a compatible device context
     	dev->db_hdc = CreateCompatibleDC(dev->hdc);
     	dev->db_bmp = CreateCompatibleBitmap(dev->hdc, nWidth, nHeight);
		SelectObject(dev->db_hdc, dev->db_bmp);
		dev->hdc = dev->db_hdc;
	}

	GetClientRect(dev->hwnd, &rect);
	dev->xPhMax = rect.right;
	dev->yPhMax = rect.bottom;
	dev->xScale = rect.right / ((float)PIXELS_X);
	dev->yScale = rect.bottom / ((float)PIXELS_Y);

#ifdef HAVE_FREETYPE

	if (freetype) {
		pls->dev_text = 1;     /* want to draw text */
		pls->dev_unicode = 1;  /* want unicode */
		init_freetype_lv1(pls);
		FT=(FT_Data *)pls->FT;
		FT->want_smooth_text=smooth_text;
	}

	if (pls->dev_text) {
		init_freetype_lv2(pls);
	}
#endif
}


void setPen(PLStream *pls)
{
	WinDev *dev = (WinDev *) pls->dev;

	SelectObject(dev->hdc, dev->hPenOld);
	SelectObject(dev->hdc, dev->hbrOld);
	DeleteObject(dev->hPen);
	DeleteObject(dev->hbr);
	dev->lp.lopnColor=dev->PenColor;
	dev->lb.lbColor=dev->PenColor;
	dev->hPen = CreatePen(PS_SOLID,dev->PenWidth,dev->PenColor);
	dev->hbr= CreateSolidBrush(dev->PenColor);
	dev->hPenOld = (HPEN)SelectObject(dev->hdc,dev->hPen);
	dev->hbrOld   = (HBRUSH)SelectObject(dev->hdc,dev->hbr);
}

/*--------------------------------------------------------------------------*\
* setColor()
*
* Change the color of the pen and the brush
\*--------------------------------------------------------------------------*/


void setColor(PLStream *pls, int r, int g, int b)
{
	WinDev *dev = (WinDev *) pls->dev;

	dev->PenColor=RGB(r,g,b);
	setPen(pls);
}

/*--------------------------------------------------------------------------*\
* plD_state_win3()
*
* Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/


void plD_state_win3(PLStream *pls, PLINT op)
{
	int cores, greyvalue;
	HPEN oldPen;
	
	switch(op) {

	case PLSTATE_WIDTH:	
		{
			WinDev *dev = (WinDev *) pls->dev;

			dev->PenWidth=pls->width;
			setPen(pls);
		}
		break;

	case PLSTATE_COLOR0:
		if ( ! pls->color ) {
			if ((pls->cmap0[0].r+pls->cmap0[0].g+pls->cmap0[0].b)/3 > 128) {
				if (pls->icol0) setColor(pls,0,0,0);
				else            setColor(pls,255,255,255);
			} else {
				if (pls->icol0) setColor(pls,255,255,255);
				else            setColor(pls,0,0,0);
			}
			break;
		}
		/* else fallthrough */
	case PLSTATE_COLOR1:
	        if (pls->color) {
			setColor(pls,pls->curcolor.r,pls->curcolor.g,pls->curcolor.b);
	        } else {
			greyvalue = (pls->curcolor.r+pls->curcolor.g+pls->curcolor.b)/3;
			setColor(pls,greyvalue,greyvalue,greyvalue);
		}
		break;

    case PLSTATE_CMAP0:
		{
			WinDev *dev = (WinDev *) pls->dev;
			if (pls->color) {
				dev->backGroundColor = RGB(pls->cmap0[0].r,pls->cmap0[0].g,pls->cmap0[0].b);
			} else {
				int greyvalue = (pls->cmap0[0].r+pls->cmap0[0].g+pls->cmap0[0].b)/3;
				dev->backGroundColor = RGB(greyvalue,greyvalue,greyvalue);
			}
		}
		break;

	}
}
/*--------------------------------------------------------------------------*\
* plD_line_win3()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/
void plD_line_win3(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
	WinDev *dev = (WinDev *)pls->dev;
	int    xpixb ;
	int    ypixb ;
	int    xpixe ;
	int    ypixe ;

	xpixb = (int)((float)x1a * dev->xScale);
	ypixb = (int)((float)(PIXELS_Y - y1a) * dev->yScale);
	xpixe = (int)((float)x2a * dev->xScale);
	ypixe = (int)((float)(PIXELS_Y - y2a) * dev->yScale);

	// If the beginning and end are the same
	if ( xpixb == xpixe && ypixb == ypixe ) {
		// Draw a pixel
		SetPixel(dev->hdc,xpixb,ypixb,dev->PenColor);
	} else {
		// Otherwise draw a line
		MoveToEx(dev->hdc,xpixb,ypixb,NULL);
		LineTo(dev->hdc,xpixe,ypixe);
	}
}

/*--------------------------------------------------------------------------*\
* plD_polyline_win3()
*
* Draw a polyline in the current color.
\*--------------------------------------------------------------------------*/
void plD_polyline_win3(PLStream *pls, short *xa, short *ya, PLINT npts)
{
	WinDev *dev = (WinDev *)pls->dev;
	POINT pt[PL_MAXPOLY];
	int i;
	
	if (npts > PL_MAXPOLY)
		plexit("FillPolygonCmd : Too many points in polygon\n");
	for (i=0; i < npts;i++) {
		pt[i].x = (int)((float)xa[i] * dev->xScale);
		pt[i].y = (int)((float)(PIXELS_Y - ya[i]) * dev->yScale);
	}
	Polyline(dev->hdc, pt, npts);
}
/*--------------------------------------------------------------------------*\
* plD_eop_win3()
*
* End of page.
\*--------------------------------------------------------------------------*/
void plD_eop_win3(PLStream *pls)
{
	WinDev *dev = (WinDev *)pls->dev;
	HCURSOR hCursor;
	
	dev->isIdle = TRUE;  // After an eop, the window is idle
	
	if (!pls->db) ReleaseDC(dev->hwnd,dev->hdc);

	// If the window has not been created outside the Plplot library...
	if (!dev->externalWindow) {
		// EnableMenuItem(dev->hMenu,CM_PRINTPLOT,MF_ENABLED);
	    // EnableMenuItem(dev->hMenu,CM_NEXTPLOT,MF_ENABLED);

		// Load and set the cursor
		hCursor = LoadCursor(NULL,IDC_ARROW);
		SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)hCursor);
		SetCursor(hCursor);

		// Check to see if we need to handle the message queue
		if(dev->hThread == NULL) {
			// No message thread, so we need to handle all the messages now
			MSG msg;
			BOOL bRet;

			// Handle messages in the queue
			do {
				if(pls->nopause) {
					// Do a non-blocking message retrieval
					bRet = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
				} else {
					// Since we are supposed to pause, do a blocking message retrieval
					bRet = GetMessage(&msg, NULL, 0, 0);
				}
				if(bRet == -1) {
					/* Error getting message */
				} else if(bRet != 0) {
					/* Successfully got a message */
					if(dev->hwnd != msg.hwnd)
						printf(
							"***** pls(%lx) dev(%lx) Got message %x for hwnd(%lx) and I am hwnd(%lx)\n",
							pls, dev, msg.message, msg.hwnd, dev->hwnd);
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			} while(!dev->nextPlot && !dev->isDead && bRet != 0);
		} else {
			// There is a message thread, so we only need to check to see
			// if we need to pause between pages
		
			if(! pls->nopause) {
				// This plot is supposed to pause

				// Clear the nextplot flag and wait for it to change
				for(dev->nextPlot = 0; !dev->nextPlot && !dev->isDead; ) ;
			}
		}
	}

	if (!pls->db) {
		InvalidateRect(dev->hwnd,NULL,TRUE);
		//UpdateWindow(dev->hwnd);
		PAINTSTRUCT ps;
		HDC winDC;

		winDC = BeginPaint(dev->hwnd, &ps);
		EndPaint(dev->hwnd,  &ps);
	} else {
		RECT rect;
		
		GetClientRect(dev->hwnd,&rect);
		HBRUSH hbr = CreateSolidBrush(dev->backGroundColor);
		FillRect(dev->hdc, &rect,hbr);
	}

	dev->nextPlot = 0;
}

/*--------------------------------------------------------------------------*\
* plD_bop_win3()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*--------------------------------------------------------------------------*/
void plD_bop_win3(PLStream *pls)
{
	WinDev *dev = (WinDev *)pls->dev;
	RECT rect;
	HCURSOR hCursor;
	
	/*	EnableMenuItem(dev->hMenu,CM_PRINTPLOT,MF_GRAYED); */
	/*	EnableMenuItem(dev->hMenu,CM_NEXTPLOT,MF_GRAYED);  */
	if (!dev->externalWindow) {
		hCursor = LoadCursor(NULL,IDC_WAIT);
		SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)hCursor);
		SetCursor(hCursor);
	}

	if (pls->db)
		dev->hdc = dev->db_hdc;
	else
		dev->hdc = GetDC(dev->hwnd);

#ifdef HAVE_FREETYPE
	FT_Data *FT=(FT_Data *)pls->FT;
#endif
	
	GetClientRect(dev->hwnd,&rect);
	dev->xPhMax = rect.right;
	dev->yPhMax = rect.bottom;
	dev->xScale = rect.right / ((float)PIXELS_X);
	dev->yScale = rect.bottom / ((float)PIXELS_Y);
	
	dev->rePaint = 0;
	dev->rePaintBusy = 0;
	dev->isIdle = FALSE;
	pls->page++;
	
	plD_state_win3(pls, PLSTATE_COLOR0); /* Set drawing color */
}

/*--------------------------------------------------------------------------*\
* plD_tidy_win3()
*
* Close graphics file or otherwise clean up.
\*--------------------------------------------------------------------------*/
void plD_tidy_win3(PLStream *pls)
{
	WinDev *dev = (WinDev *)pls->dev;

#ifdef HAVE_FREETYPE
	if (pls->dev_text) {
		FT_Data *FT=(FT_Data *)pls->FT;

		plscmap0n(FT->ncol0_org);
		plD_FreeType_Destroy(pls);
    }
#endif

	pls->page = 0;
	pls->OutFile = NULL;
	if (!dev->externalWindow) DestroyWindow(dev->hwnd);
	if (!dev->hThread) CloseHandle(dev->hThread);
}

/*--------------------------------------------------------------------------*\
* plD_esc_win3()
*
* Escape function.
\*--------------------------------------------------------------------------*/

#define DPMM 4.0

void plD_esc_win3(PLStream *pls, PLINT op , void *ptr)
{
	WinDev *dev = (WinDev *)pls->dev;
	HCURSOR holdcursor,hnewcursor;
	MSG msg;

	switch (op) {

	case PLESC_GETC:
		hnewcursor = LoadCursor(NULL,IDC_CROSS);
		holdcursor = (HCURSOR)GetClassLong(GetActiveWindow(),GCL_HCURSOR);
		SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)hnewcursor);
		SetCursor(hnewcursor);
		dev->newCursor = 0;
		dev->button = 0;
		dev->state = 0;
		while (!dev->newCursor) {
			GetMessage(&msg,NULL,0,0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		((PLGraphicsIn *)ptr)->dX = float(dev->cursorX)/float(PIXELS_X*dev->xScale);
		((PLGraphicsIn *)ptr)->dY = float(PIXELS_Y*dev->yScale - dev->cursorY) / float(PIXELS_Y*dev->yScale);
		((PLGraphicsIn *)ptr)->button = dev->button;
		((PLGraphicsIn *)ptr)->state = dev->state;
		SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)holdcursor);
		break;

	case PLESC_FILL:
		FillPolygonCmd(pls);
		break;

	case PLESC_XORMOD:
		if (*(PLINT*)(ptr) == 0)
			SetROP2(dev->hdc,R2_COPYPEN);
		else
			SetROP2(dev->hdc,R2_XORPEN);
		break;
		
	case PLESC_REDRAW:
		break;
		
	case PLESC_RESIZE:
		{
			RECT rect;
			dev->hdc = GetDC(dev->hwnd);
			GetClientRect(dev->hwnd,&rect);
			dev->xPhMax = rect.right;
			dev->yPhMax = rect.bottom;
			dev->xScale = rect.right / ((float)PIXELS_X);
			dev->yScale = rect.bottom / ((float)PIXELS_Y);
			

			{
			PLFLT pxlx = DPMM/dev->xScale;
			PLFLT pxly = DPMM/dev->yScale;
			plP_setpxl(pxlx, pxly);
			}

#ifdef HAVE_FREETYPE
			{
			  FT_Data *FT=(FT_Data *)pls->FT;
			  if (FT)
			    {
/* TODO !!!!
			      FT->scale=dev->scale;
			      FT->ymax=dev->height;
*/
			    }
			}
#endif

			if (pls->db)
			{
				//SetBitmapDimensionEx(dev->db_bmp,rect.right,rect.bottom,NULL);
				DeleteObject(dev->db_bmp);
		     	dev->db_bmp = CreateCompatibleBitmap(dev->hdc, rect.right,rect.bottom);
				SelectObject(dev->db_hdc, dev->db_bmp);
				dev->hdc = dev->db_hdc;
				HBRUSH hbr = CreateSolidBrush( dev->backGroundColor);
				FillRect(dev->hdc, &rect,hbr);
				plreplot ();
			}
			else
				InvalidateRect(dev->hwnd,NULL,TRUE);
			
		}
		break;

#ifdef HAVE_FREETYPE
     case PLESC_HAS_TEXT:
        plD_render_freetype_text(pls, (EscText *)ptr);
        break;

/*     case PLESC_LIKES_UNICODE:
        plD_render_freetype_sym(pls, (EscText *)ptr);
        break;*/

#endif
		
	case PLESC_EXPOSE:
		if (pls->db)
		{
			PAINTSTRUCT ps;
			HDC winDC;
			RECT r;

			// Set up a display context to begin painting
			winDC = BeginPaint(dev->hwnd, &ps);
			GetClientRect(dev->hwnd, &r);

     		// copy the buffer to the screen
     		BitBlt(GetDC(dev->hwnd), 0, 0, r.right, r.bottom, dev->db_hdc, 0, 0, SRCCOPY);

			// Tell Windows you are done painting
			EndPaint(dev->hwnd,  &ps);
		}
		else
			plreplot ();
		break;
		
        case PLESC_IMAGE:
                plD_DrawImage_win3(pls);
                break;

        case PLESC_IMAGEOPS:
                imageops(pls, (PLINT *) ptr);
                break;
	}
}

/*-------------------------------------------------------------*\
* FillPolygonCmd()
\*-------------------------------------------------------------*/
static void FillPolygonCmd(PLStream *pls) {
	POINT pt[PL_MAXPOLY];
	int i;
	WinDev *dev = (WinDev *)pls->dev;
	if (pls->dev_npts > PL_MAXPOLY)
		plexit("FillPolygonCmd : Too many points in polygon\n");
	for (i=0; i < pls->dev_npts;i++) {
		pt[i].x = (int)((float)pls->dev_x[i] * dev->xScale);
		pt[i].y = (int)((float)(PIXELS_Y - pls->dev_y[i]) * dev->yScale);
	}
	Polygon(dev->hdc,pt,pls->dev_npts);
}

/*--------------------------------------------------------------------------*\
 * plD_DrawImage_win3()
 *
 * Experimental! Currently only works on 32-bit displays
\*--------------------------------------------------------------------------*/


void plD_DrawImage_win3(PLStream *pls)
{
	WinDev *dev = (WinDev *) pls->dev;
	HDC hdcMemory;
	HBITMAP bitmap, bitmapOld;
	BYTE    *byteArray;
	
	int     byteArrayXsize, byteArrayYsize;
	int     imageX0, imageY0, imageWX, imageWY;
	int     image_kx_start, image_ky_start, image_kx_end, image_ky_end, image_wkx, image_wky;
	int     imageXmin, imageXmax, imageYmin, imageYmax;

	int i, npts, nx, ny, ix, iy, corners[5], Ppts_x[5], Ppts_y[5];
	int clpxmi, clpxma, clpymi, clpyma, icol1;

	int level;
	float wcxmi, wcxma, wcymi, wcyma;
	long ptr1, ptr2;

	clpxmi = plsc->imclxmin;
	clpxma = plsc->imclxmax;
	clpymi = plsc->imclymin;
	clpyma = plsc->imclymax;

	//  printf("clpxmi %d %d %d %d\n", clpxmi, clpxma, clpymi, clpyma);
	wcxmi = (float)((clpxmi - pls->wpxoff)/pls->wpxscl);
	wcxma = (float)((clpxma - pls->wpxoff)/pls->wpxscl);
	wcymi = (float)((clpymi - pls->wpyoff)/pls->wpyscl);
	wcyma = (float)((clpyma - pls->wpyoff)/pls->wpyscl);

	npts = plsc->dev_nptsX*plsc->dev_nptsY;

	nx = pls->dev_nptsX;
	ny = pls->dev_nptsY;

	imageXmin = pls->dev_ix[0];
	imageYmin = pls->dev_iy[0];
	imageXmax = pls->dev_ix[nx*ny-1];
	imageYmax = pls->dev_iy[nx*ny-1];

	//  printf("imageXmin %d %d %d %d\n", imageXmin, imageXmax, imageYmin, imageYmax);

	if (clpxmi > imageXmin) {
		imageX0 = (int)(dev->xScale * (float)clpxmi) + 1;
		image_kx_start = (int)wcxmi;
	} else {
		imageX0 = (int)(dev->xScale * (float)imageXmin) + 1;
		image_kx_start = 0;
	}

	if (clpxma < imageXmax) {
		imageWX = (int)(dev->xScale * (float)clpxma) - imageX0;
		image_kx_end = (int)wcxma;
		image_wkx =  image_kx_end-image_kx_start+1;
	} else {
		imageWX = (int)(dev->xScale * (float)imageXmax) - imageX0;
		image_kx_end = image_kx_start+nx;
		image_wkx = nx;
	}

	if (clpymi > imageYmin) {
		imageY0 = (int)(dev->yScale * (float)(PIXELS_Y-clpyma)) + 1;
		image_ky_start = ny - 1 - (int)wcyma;
	} else {
		imageY0 = (int)(dev->yScale * (float)(PIXELS_Y - imageYmax)) + 1;
		image_ky_start = 0;
	}

	if (clpyma < imageYmax) {
		imageWY = (int)(dev->yScale * (float)(PIXELS_Y-clpymi)) - imageY0;
		image_ky_end = ny -1 - (int)(wcymi);
		image_wky = image_ky_end-image_ky_start+1;
	} else {
		imageWY = (int)(dev->yScale * (float)(PIXELS_Y - imageYmin)) - imageY0;
		image_ky_end = ny - 1 - image_ky_start;
		image_wky = ny;
	}

	//  printf("imageX0 %d %d %d %d\n", imageX0, imageY0, imageWX, imageWY);
	//  printf("kx %d %d %d %d\n", image_kx_start, image_kx_end, image_ky_start, image_ky_end);
	//  printf("Before malloc... %d %d \n", nx, ny);
	byteArray = (BYTE*)malloc(nx*ny*sizeof(BYTE)*4);
	//  printf("After malloc...\n");

	for (ix=0; ix<nx; ix++) {
		if ((ix >= image_kx_start) || (ix <= image_kx_end)) {
			ptr1 = 4*ix;
			for (iy=0; iy<ny; iy++) {
				if ((iy >= image_ky_start) || (iy <= image_ky_end)) {
					icol1 = (int)(pls->dev_z[ix*(ny-1)+iy] / 65535.0) * pls->ncol1;
					icol1 = MIN(icol1, pls->ncol1-1);
					ptr2 = ptr1+4*(ny-iy-1)*nx;
					//	    printf("%d  ", ptr2);
					*(BYTE*)(byteArray+sizeof(BYTE)*ptr2++) = pls->cmap1[icol1].b;
					*(BYTE*)(byteArray+sizeof(BYTE)*ptr2++) = pls->cmap1[icol1].g;
					*(BYTE*)(byteArray+sizeof(BYTE)*ptr2++) = pls->cmap1[icol1].r;
					*(BYTE*)(byteArray+sizeof(BYTE)*ptr2) = 255;
				}
			}
		}
	}
		
	//  printf("Before CreateCompatibleBitmap...\n");
	bitmap = CreateCompatibleBitmap(dev->hdc, nx, ny);
	SetBitmapBits(bitmap, 4*npts, (const void*)byteArray);

	//  printf("Before CreateCompatibleDC...\n");
	hdcMemory = CreateCompatibleDC(dev->hdc);
	bitmapOld = (HBITMAP)SelectObject(hdcMemory, bitmap);
	SetStretchBltMode(dev->hdc, HALFTONE);
	//  printf("%d %d %d %d %d %d %d %d\n",imageX0, imageY0, imageWX, imageWY, image_kx_start, image_ky_start, image_wkx, image_wky);
	StretchBlt(dev->hdc, imageX0, imageY0, imageWX, imageWY, hdcMemory, image_kx_start, image_ky_start, image_wkx, image_wky, SRCCOPY);
	
	SelectObject(hdcMemory, bitmapOld);
	DeleteObject(bitmap);
	ReleaseDC(dev->hwnd,hdcMemory);				

	free(byteArray);
}

static void imageops(PLStream *pls, PLINT *ptr)
{

  WinDev *dev = (WinDev *) pls->dev;

  /* TODO: store/revert to/from previous state */

  switch (*ptr) {
  case ZEROW2D:
    dev->write_to_window = 0;
    break;

  case ONEW2D:
    dev->write_to_window = 1;
    break;

  case ZEROW2B:
    dev->write_to_pixmap = 0;
    break;

  case ONEW2B:
    //    XFlush(xwd->display);
    dev->write_to_pixmap = 1;
    break;
  }
}

/* Message dispatcher for the win3 driver
 */
LRESULT CALLBACK PlPlotWndProc (HWND hwnd, UINT message,
	UINT wParam, LONG lParam)
{
	PLStream *pls = (PLStream *)GetWindowLong(hwnd, GWL_USERDATA);
	WinDev *dev = NULL;
	RECT paint_rect;
	HMETAFILE hmf;
	GLOBALHANDLE hGMem;
	LPMETAFILEPICT lpMFP;
#ifdef HAVE_FREETYPE
    FT_Data *FT=(FT_Data *)pls->FT;
#endif
	
	// Make sure that we have a valid plot stream
	if (pls) {
		dev = (WinDev *)pls->dev;
	
		switch (message) {
		case WM_CHAR :
			// A key was pressed
			if ( wParam == VK_RETURN ) {
				// The return key moves us to the next plot
				dev->nextPlot = 1 ;
			}
			return 0;
		case WM_LBUTTONDOWN :
			// Left mouse button press
			dev->newCursor = 1;
			dev->cursorX = LOWORD(lParam);
			dev->cursorY = HIWORD(lParam);
			dev->button = 1;
			dev->state = 0x000;
			return 0;
		case WM_RBUTTONDOWN :
			// Right mouse button press
			dev->newCursor = 1;
			dev->cursorX = LOWORD(lParam);
			dev->cursorY = HIWORD(lParam);
			dev->button = 3;

			// Advance to next plot
			dev->nextPlot = 1;
			dev->state = 0x000;
			return 0;
		case WM_MOUSEMOVE :
			if ((wParam & MK_LBUTTON) || (wParam & MK_RBUTTON)) {
				dev->newCursor = 1;
				dev->cursorX = LOWORD(lParam);
				dev->cursorY = HIWORD(lParam);
				dev->state = 0x100;
				return 0;
			}
			break;
		case WM_ERASEBKGND :
			if (!dev->rePaintBusy)
				dev->rePaint = 1;
			break;
		case WM_PAINT :
			printf("  Processing message %x for pls(%lx), dev(%lx), dev->hwnd(%lx) hwnd(%lx)\n",
				message, pls, dev, dev->hwnd, hwnd);

			/* Determine if there is a region that needs to be updated.
			 * No need to erase the updated region because we redraw the
			 * entire plot.
			 */			
			if (GetUpdateRect(dev->hwnd, &paint_rect, FALSE)) {
				printf("    Need to update %d, %d\n", dev->isIdle, dev->rePaintBusy);
				/* If we are not currently repainting, we can process a WM_PAINT message
				 */
				if(!dev->rePaintBusy) {
					RECT client_rect;
					PAINTSTRUCT ps;
					HDC win_hdc;
					HCURSOR hcurSave;
				
					printf("    Not busy, repainting\n");

					// Set the flags to indicate we are repainting
					dev->rePaint = 0;
					dev->rePaintBusy = 1;

					// Display a busy cursor
					hcurSave = SetCursor(LoadCursor(NULL, IDC_WAIT));

					// Free the current device context
					ReleaseDC(dev->hwnd, dev->hdc);

					/* Start the painting process.  NOTE:  The HDC returned by
					 * BeginPaint is clipped to the region that needs to be updated
					 */
					dev->hdc = BeginPaint(dev->hwnd, &ps);

					// Get a device context for the entire window
					win_hdc = GetDC(dev->hwnd);

					// Get the coordinates of the drawing area
					GetClientRect(dev->hwnd, &client_rect);
					dev->xPhMax = client_rect.right;
					dev->yPhMax = client_rect.bottom;
					dev->xScale = client_rect.right / ((float)PIXELS_X);
					dev->yScale = client_rect.bottom / ((float)PIXELS_Y);
				
#ifdef HAVE_FREETYPE
					/* if ( FT != NULL ) { */
					if ( freetype ) {
						FT = (FT_Data *) pls->FT ;

						if ( dev->xPhMax < dev->yPhMax ) {
							FT->scale=1.0/dev->xScale;
						} else {
							FT->scale=1.0/dev->yScale;
						}
						FT->ymax=dev->yPhMax;
						FT->invert_y=1;
					}
#endif
					// Redraw the window by playing back the buffer
					plRemakePlot(pls);

					// Redraw complete, cleanup
					dev->rePaintBusy = 0;
					SetCursor(hcurSave);

					// Release the clipped device context
					ReleaseDC(dev->hwnd, dev->hdc);

					// Indicate that the redraw is complete
					EndPaint(dev->hwnd, &ps);

					// and restore the device context for the entire window
 					dev->hdc = win_hdc;
				
					plD_state_win3(pls, PLSTATE_COLOR0); /* Set drawing color */
				} else {
					/* There is a region that needs to be redrawn, but we are not ready
					 * to do it.  The BeginPaint() call needs to happen in order to
					 * notify the system that we have handled the request.
					 */
					HDC dummy;
					PAINTSTRUCT ps;
							
					printf("    Not repaintable\n");
					dummy = BeginPaint(dev->hwnd, &ps);
					ReleaseDC(dev->hwnd, dummy);
					EndPaint(dev->hwnd, &ps);
				}/* if repaintable */
			} /* GetUpdateRect() */
			return 0;
		case WM_DESTROY :
			// PostQuitMessage(0);
			dev->isDead = TRUE;
			return 0;
		/*
		case WM_COMMAND :
			switch (wParam) {
			case  CM_NEXTPLOT :
				dev->nextPlot = 1;
				return 0;
			case CM_PRINTPLOT :
				dev->rePaintBusy = 1;
				if (dev->hdc = GetPrinterDC()) {
					dev->xScale = GetDeviceCaps(dev->hdc,HORZRES) / ((float)PIXELS_X);
					dev->yScale = GetDeviceCaps(dev->hdc,VERTRES) / ((float)PIXELS_Y);
#ifdef WIN32
					DOCINFO di;
					di.cbSize = sizeof(DOCINFO);
					di.lpszDocName = "Plplot - Print";
					di.lpszOutput = NULL;
					di.lpszDatatype = NULL;
					di.fwType = NULL;
					if( SetAbortProc( dev->hdc, (int(__stdcall *)(struct HDC__ *,int))AbortProc ) == SP_ERROR ) {
						MessageBox( NULL, "Error setting up AbortProc",
							"Error", MB_APPLMODAL | MB_OK);
						break;
					}
					StartDoc(dev->hdc,&di);
					StartPage(dev->hdc);
					hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT));
					plRemakePlot(pls);
					EndPage(dev->hdc);
					EndDoc(dev->hdc);
#else
					Escape(dev->hdc,STARTDOC,0,NULL,NULL);
					hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT));
					plRemakePlot(pls);
					Escape(dev->hdc,NEWFRAME,0,NULL,NULL);
					Escape(dev->hdc,ENDDOC,0,NULL,NULL);
#endif
					SetCursor(hcurSave);
					DeleteDC(dev->hdc);
				}
				dev->rePaintBusy = 0;
				dev->rePaint = 1;
		
				return 0;
			case CM_EDITCOPY :
				dev->rePaintBusy = 1;
				dev->hdc = CreateMetaFile(NULL);
				SetWindowExtEx(dev->hdc,PIXELS_X,PIXELS_Y,NULL);
				SetWindowOrgEx(dev->hdc,0,0,NULL);
				dev->xScale = 1.0;
				dev->yScale = 1.0;
				hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT));
				plRemakePlot(pls);
				SetCursor(hcurSave);
		
				hmf = CloseMetaFile(dev->hdc);
				hGMem = GlobalAlloc(GHND,(DWORD)sizeof(METAFILEPICT));
				lpMFP = (LPMETAFILEPICT) GlobalLock(hGMem);
		
				lpMFP->mm = MM_ISOTROPIC;
				lpMFP->xExt = PIXELS_X;
				lpMFP->yExt = PIXELS_Y;
				lpMFP->hMF = hmf;
			
				GlobalUnlock(hGMem);
			
				OpenClipboard(dev->hwnd);
				EmptyClipboard();
				SetClipboardData(CF_METAFILEPICT,hGMem);
				CloseClipboard();
				
				dev->rePaintBusy = 0;
				dev->rePaint = 1;
				return 0;

			case CM_ABOUT :
				// MessageBox(hwnd,aboutText,"About",MB_OK);
				return 0;
			}
			*/
		}
	}

	// Pass the message to the default handler and then return
	return DefWindowProc(hwnd,message,wParam,lParam);
}

/*
BOOL CALLBACK AbortProc( HDC hDC, int Error )
{
	MSG   msg;
	
	while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
	return TRUE;
}
*/

#ifdef HAVE_FREETYPE

/*--------------------------------------------------------------------------*\
 * int SetRegValue(char *key_name, char *key_word, char *buffer,int dwType, int size)
 *
 *  Function set the registry; if registry entry does not exist, it is
 *  created. Actually, the key is created before it is set just to make sure
 *  that is is there !
\*--------------------------------------------------------------------------*/

static int SetRegValue(char *key_name, char *key_word, char *buffer,int dwType, int size)
{
  int j=0;

  DWORD lpdwDisposition;
  HKEY hKey;

  j=RegCreateKeyEx(
                    HKEY_CURRENT_USER,
                    key_name,
                    0,                          /* reserved */
                    NULL,                       /* address of class string */
                    REG_OPTION_NON_VOLATILE,       /* special options flag */
                    KEY_WRITE,                  /* desired security access */
                    NULL,                       /* address of key security structure */
                    &hKey,                      /* address of buffer for opened handle */
                    &lpdwDisposition            /* address of disposition value buffer */
                  );

  if (j==ERROR_SUCCESS)
    {
      RegSetValueEx(hKey, key_word, 0, dwType, (unsigned char *)buffer, size);
      RegCloseKey(hKey);
    }
  return(j);
}

/*--------------------------------------------------------------------------*\
 * int GetRegValue(char *key_name, char *key_word, char *buffer, int size)
 *
 *  Function reads the registry and gets a string value from it
 *  buffer must be allocated by the caller, and the size is given in the size
 *  paramater.
 *  Return code is 1 for success, and 0 for failure.
\*--------------------------------------------------------------------------*/

static int GetRegValue(char *key_name, char *key_word, char *buffer, int size)
{
  int ret=0;
  HKEY hKey;
  int dwType;
  int dwSize=size;

    if (RegOpenKeyEx(HKEY_CURRENT_USER, key_name, 0, KEY_READ, &hKey)==ERROR_SUCCESS) {
      if (RegQueryValueEx(hKey, key_word, 0, (LPDWORD)&dwType,(unsigned char *)buffer,(LPDWORD)&dwSize)==ERROR_SUCCESS) {
          ret=1;
        }
      RegCloseKey(hKey);
      }
  return(ret);
}

/*----------------------------------------------------------------------*\
 *  void plD_pixel_wingcc (PLStream *pls, short x, short y)
 *
 *  callback function, of type "plD_pixel_fp", which specifies how a single
 *  pixel is set in the current colour.
\*----------------------------------------------------------------------*/

void plD_pixel_win3 (PLStream *pls, short x, short y)
{
  WinDev *dev=(WinDev *)pls->dev;

   SetPixel(dev->hdc, x, y,dev->PenColor);

}

void plD_pixelV_win3 (PLStream *pls, short x, short y)
{
  WinDev *dev=(WinDev *)pls->dev;

   SetPixelV(dev->hdc, x, y,dev->PenColor);

}


/*----------------------------------------------------------------------*\
 *  void init_freetype_lv1 (PLStream *pls)
 *
 *  "level 1" initialisation of the freetype library.
 *  "Level 1" initialisation calls plD_FreeType_init(pls) which allocates
 *  memory to the pls->FT structure, then sets up the pixel callback
 *  function.
\*----------------------------------------------------------------------*/

static void init_freetype_lv1 (PLStream *pls)
{
	FT_Data *FT;
	int x;
	WinDev *dev=(WinDev *)pls->dev;

	plD_FreeType_init(pls);

	FT=(FT_Data *)pls->FT;


	/*
 	 *  Work out if our device support "fast" pixel setting
	 *  and if so, use that instead of "slow" pixel setting
	 */

	x=GetDeviceCaps(dev->hdc,RASTERCAPS);

	if (x & RC_BITBLT)
		FT->pixel= (plD_pixel_fp)plD_pixelV_win3;
	else
		FT->pixel= (plD_pixel_fp)plD_pixel_win3;
}

/*----------------------------------------------------------------------*\
 *  void init_freetype_lv2 (PLStream *pls)
 *
 *  "Level 2" initialisation of the freetype library.
 *  "Level 2" fills in a few setting that aren't public until after the
 *  graphics sub-syetm has been initialised.
 *  The "level 2" initialisation fills in a few things that are defined
 *  later in the initialisation process for the GD driver.
 *
 *  FT->scale is a scaling factor to convert co-ordinates. This is used by
 *  the GD and other drivers to scale back a larger virtual page and this
 *  eliminate the "hidden line removal bug". Set it to 1 if your device
 *  doesn't have scaling.
 *
 *  Some coordinate systems have zero on the bottom, others have zero on
 *  the top. Freetype does it one way, and most everything else does it the
 *  other. To make sure everything is working ok, we have to "flip" the
 *  coordinates, and to do this we need to know how big in the Y dimension
 *  the page is, and whether we have to invert the page or leave it alone.
 *
 *  FT->ymax specifies the size of the page FT->invert_y=1 tells us to
 *  invert the y-coordinates, FT->invert_y=0 will not invert the
 *  coordinates.
\*----------------------------------------------------------------------*/

static void init_freetype_lv2 (PLStream *pls)
{
	WinDev *dev=(WinDev *)pls->dev;
	FT_Data *FT=(FT_Data *)pls->FT;

	/*
	FT->scale=dev->scale;
	FT->ymax=dev->height;
	*/
	if ( dev->xPhMax > dev->yPhMax ) {
		FT->scale=1.0/dev->xScale;
	} else {
		FT->scale=1.0/dev->yScale;
	}
	/* AM: Workaround for double painting problem
		These settings will make sure the freetype text does
		not appear on the first time round, only after the
		initial WM_PAINT.
	*/
	FT->scale = dev->xScale ;
	FT->ymax=2*dev->yPhMax;
	FT->invert_y=0;

	if (FT->want_smooth_text==1) {    /* do we want to at least *try* for smoothing ? */
		FT->ncol0_org=pls->ncol0;                                   /* save a copy of the original size of ncol0 */
		FT->ncol0_xtra=16777216-(pls->ncol1+pls->ncol0);            /* work out how many free slots we have */
		FT->ncol0_width=max_number_of_grey_levels_used_in_text_smoothing;              /* find out how many different shades of anti-aliasing we can do */
		FT->ncol0_width=max_number_of_grey_levels_used_in_text_smoothing;                 /* set a maximum number of shades */
		plscmap0n(FT->ncol0_org+(FT->ncol0_width*pls->ncol0));      /* redefine the size of cmap0 */
		/* the level manipulations are to turn off the plP_state(PLSTATE_CMAP0)
		 * call in plscmap0 which (a) leads to segfaults since the GD image is
		 * not defined at this point and (b) would be inefficient in any case since
		 * setcmap is always called later (see plD_bop_png) to update the driver
		 * color palette to be consistent with cmap0. */
		{
			PLINT level_save;

			level_save = pls->level;
			pls->level = 0;
			pl_set_extended_cmap0(pls, FT->ncol0_width, FT->ncol0_org); /* call the function to add the extra cmap0 entries and calculate stuff */
			pls->level = level_save;
        }
		FT->smooth_text=1;      /* Yippee ! We had success setting up the extended cmap0 */
	}
}
#endif

#else
pldummy_win3() {
	return 0;
}
#endif   //WIN3
