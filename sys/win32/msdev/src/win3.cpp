/*
 win3.cpp

  Driver for Win32

  Paul Casteels	11-Sep-1996	(casteels@uia.ua.ac.be)

  Color Handling by

  Carla Carmelo Rosa  (l36658@alfa.ist.utl.pt)


*/

/* Set constants for dealing with colormap.  In brief:
 * ccmap		When set, turns on custom color map
 * WIN_COLORS		Number of low "pixel" values to copy.  
 * CMAP0_COLORS		Color map 0 entries.  
 * CMAP1_COLORS		Color map 1 entries.  
 * MAX_COLORS		Maximum colors period.
 * See Init_CustomCmap() and  Init_DefaultCmap() for more info.
 * Set ccmap at your own risk -- still under development.
*/

#include "plplot/plDevs.h"
#ifdef PLD_win3

#include <stdio.h>
#include <string.h>
#include "plplot/plplotP.h"
#include "plplot/drivers.h"
#include "plplot/resource.h"
#include <windows.h>
#include <commdlg.h>
#include <assert.h>

#define TEXT_MODE 0
#define GRAPHICS_MODE 1

#define CLEAN 0
#define DIRTY 1

static const float xRes = 8192.0,yRes = 8192.0;
static const int niceCnt = 20;
static const char szPlPlotClass[] = "PlplotClass";
static const char szPlPlotWName[] = "PlPlot Window";
static const char aboutText[] = "        Plplot    "PLPLOT_VERSION
								"\nComments/Questions to\n"
								"plplot-list@dino.ph.utexas.edu\n";

LRESULT CALLBACK _export PlPlotWndProc (HWND hwnd,UINT message,UINT wParam,LONG lParam);
void *h_pldll;
//new...
static int plplot_ccmap =0;

#define WIN_COLORS 255
#define MAX_COLORS 256

/* Function prototypes */
/* Initialization */

static void    InitColors     (PLStream *pls);
static void    Init           (PLStream *pls);
static void    AllocCustomMap (PLStream *pls);
static void    AllocCmap1     (PLStream *pls);
//static void  AllocCmap0     (PLStream *pls);
//static void  AllocBGFG      (PLStream *pls);

/* Miscellaneous */
//static void  StoreCmap0		(PLStream *pls);
//static void  StoreCmap1		(PLStream *pls);

static void FillPolygonCmd (PLStream *pls);
BOOL CALLBACK AbortProc( HDC hDC, int Error );

//ADDED 
typedef struct {
  HWND	hwnd;
  HMENU 	hMenu;
  LOGPEN 	lp;
  LOGBRUSH lb;
  HPEN	hPen;
  HDC 	hdc;
  HPALETTE 	hpal;		
  HBRUSH	hbr;

  float 	xScale,yScale;
  int xPhMax,yPhMax;
  int 	nextPlot; 		// set to 1 by Nextplot menu
  int 	nice;             // be nice for windows multitasking
  int 	rePaint;          // if the background is cleared we need a repaint
  int 	rePaintBsy;       // if we are repainting block the rest
					// plRemakePlot is not reentrant (in Windows)?
  int 	newCursor;
  float 	cursorX,cursorY;

  int 	color;		//ADDED 1 para ColorDevice
  int       ncol0;		//ADDED Number of cmap 0 colors allocated 
  int       ncol1;   		//ADDED Number of cmap 1 colors allocated 
  COLORREF	cmap1[MAX_COLORS];//ADDED Color entries for cmap 1 */
  COLORREF	curcolor;

} WinDev;

static MSG msg;

static long colors[16]={
  RGB(0,0,0),           // 0 = black
  RGB(0,0,255), // 1 = blue
  RGB(0,255,0), // 2 = green
  RGB(0,255,255),       // 3 = cyan
  RGB(255,0,0), // 4 = red
  RGB(255,0,255),       // 5 = magenta
  RGB(255,255,0),       // 6 = yellow
  RGB(255,255,255),     // 7 = white
  RGB(0,0,0),
  RGB(0,0,0),
  RGB(0,0,0),
  RGB(0,0,0),
  RGB(0,0,0),
  RGB(0,0,0),
  RGB(0,0,0),
  RGB(0,0,0)
};

// Transfer control to windows
static void checkMessage() {
  if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
	 TranslateMessage(&msg);
	 DispatchMessage(&msg);
  }
}

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
 

/*--------------------------------------------------------------------------*\
* Initialize device.
\*--------------------------------------------------------------------------*/


void plD_init_win3(PLStream *pls)
{
	 HWND   hwndMain;
	 WNDCLASS wndclass;
#ifdef WIN32
	 HINSTANCE hInstance;
#else
	 HANDLE hInstance;
#endif

	 WinDev *dev;
	 int xPos,yPos;
	 int nWidth,nHeight;

	 pls->termin = 1;                       // is an interactive terminal
	 pls->icol0 = 1;                        // current color
	 pls->width = 1;                        // current pen width
	 pls->bytecnt = 0;
	 pls->page = 0;
	 pls->plbuf_write = 1;  // buffer the output
	 pls->dev_flush = 1;            // flush as we like
	 //PC
	 pls->dev_fill0 = 1;

/* Set up device parameters */
	 if (pls->dev != NULL)
		delete pls->dev;
	 pls->dev = new WinDev;
	 assert(pls->dev != NULL);
	 dev = (WinDev *) pls->dev;
	 dev->nextPlot = 0;
	 dev->nice = 0;
	 dev->hPen = CreatePen(PS_SOLID,0,colors[0]);
	 dev->hbr = CreateSolidBrush(colors[0]);

// Get/save this from plplot.ini ??
	 xPos = 100;
	 yPos = 100;
	 nWidth = 600;
	 nHeight = 400;

#ifdef WIN32
	hInstance = GetModuleHandle(NULL);
	dev->hMenu = LoadMenu(hInstance,MAKEINTRESOURCE(PLCOMMANDS));
	if (dev->hMenu == NULL) {
		hInstance = (HINSTANCE)h_pldll;
	    dev->hMenu = LoadMenu(hInstance,MAKEINTRESOURCE(PLCOMMANDS));
	}
#else
	 hwndMain = GetActiveWindow();
	 hInstance = GetWindowWord(hwndMain,GWW_HINSTANCE);
	 dev->hMenu = LoadMenu(hInstance,"COMMANDS");
#endif
	 wndclass.style = CS_HREDRAW | CS_VREDRAW;
	 wndclass.lpfnWndProc = ::PlPlotWndProc;
	 wndclass.cbClsExtra = 0;
	 wndclass.cbWndExtra = sizeof(pls);
	 wndclass.hInstance = hInstance;
	 wndclass.hIcon = LoadIcon(hInstance,"PLICON");
	 wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	 wndclass.hbrBackground = (struct HBRUSH__ *)GetStockObject(WHITE_BRUSH);
	 wndclass.lpszMenuName = NULL;
	 wndclass.lpszClassName = szPlPlotClass;
	 RegisterClass (&wndclass);

	 dev->hwnd = CreateWindow(szPlPlotClass,szPlPlotWName,
		WS_OVERLAPPEDWINDOW,
		xPos,yPos,nWidth,nHeight,
		NULL,dev->hMenu,
		hInstance,NULL);

	 SetWindowLong(dev->hwnd,0,(long)pls);

	 ShowWindow(dev->hwnd,SW_SHOW);

	 plP_setpxl(2.5,2.5);           /* Pixels/mm. */
	 plP_setphy(0,xRes,0,yRes);

	 InitColors(pls);
}

/*--------------------------------------------------------------------------*\
 * InitColors()
 *
 * Does all color initialization.
\*--------------------------------------------------------------------------*/
static void InitColors(PLStream *pls)
{
  	if (!plplot_ccmap) AllocCustomMap(pls);
}

/*--------------------------------------------------------------------------*\
 * AllocCustomMap()
\*--------------------------------------------------------------------------*/
LOGPALETTE *plgpl = (LOGPALETTE*) LocalAlloc(LPTR,
				     sizeof(LOGPALETTE)+WIN_COLORS * sizeof(PALETTEENTRY));


static void AllocCustomMap(PLStream *pls)
{
 WinDev *dev=(WinDev *) pls->dev;
 int i, red, green, blue;

	plgpl->palNumEntries = WIN_COLORS;
	plgpl->palVersion = 0x300;
  
	for (i=0,green=0; i<WIN_COLORS; i++) { 
			red = 255 - (int)(255/WIN_COLORS*i);
			blue= 0 + (int)(255/WIN_COLORS*i);
			plgpl->palPalEntry[i].peRed 	= LOBYTE(red);
			plgpl->palPalEntry[i].peGreen 	= LOBYTE(green);
			plgpl->palPalEntry[i].peBlue	= LOBYTE(blue);
			plgpl->palPalEntry[i].peFlags 	= PC_NOCOLLAPSE;
	}
	dev->hpal = CreatePalette(plgpl);
	LocalFree((HLOCAL) plgpl);
	//AllocCmap1(pls);
	//PC delete[] plgpl.palPalEntry;
	plscmap1n(WIN_COLORS);

		dev->lp.lopnStyle=PS_SOLID;
		dev->lp.lopnWidth.x=1;
		dev->lp.lopnWidth.y=0;
		dev->lp.lopnColor=colors[(int)pls->icol0];

		dev->lb.lbStyle=PS_SOLID;
		dev->lb.lbColor=colors[(int)pls->icol0];
		dev->lb.lbHatch=HS_CROSS;

	dev->hPen = CreatePenIndirect(&dev->lp);
	dev->hbr = CreateBrushIndirect(&dev->lb);

}

/*--------------------------------------------------------------------------*\
 * plD_state_win3()
 *
 * Handle change in PLStream state (color, pen width, fill attribute, etc).
\*--------------------------------------------------------------------------*/
int stat=0;

void plD_state_win3(PLStream *pls, PLINT op)
{
 WinDev *dev = (WinDev *) pls->dev;
 int cores;

	DeleteObject(dev->hPen);
	switch(op) {
      	case PLSTATE_WIDTH:
			break;

	      case PLSTATE_COLOR0:
			dev->lp.lopnColor=colors[(int)pls->icol0];
			dev->lb.lbColor=colors[(int)pls->icol0];
			dev->hPen = CreatePen(PS_SOLID,0,colors[(int)pls->icol0]);
			dev->hbr= CreateSolidBrush(colors[(int)pls->icol0]);
			stat=0;	
			break;
	    
		//NEW_____________
	    	case PLSTATE_COLOR1:
      	  int icol1;
			if(stat==0) {
				SelectPalette(dev->hdc,dev->hpal, 0);
				dev->ncol1=	WIN_COLORS;
				cores=RealizePalette(dev->hdc);	
				printf("%i cores,,win_colors=%i\n",cores, dev->ncol1);
				stat=1;
			}
			dev->lp.lopnColor=PALETTEINDEX(icol1);
			dev->lb.lbColor=PALETTEINDEX(icol1);

			icol1 = pls->icol1;
			if((dev->hPen = CreatePen(PS_SOLID,0,PALETTEINDEX(icol1)))==NULL)
				printf("\n No create");
			if((dev->hbr = CreateSolidBrush(PALETTEINDEX(icol1)))==NULL)
				printf("\n No create");
			break;
	}
	SelectObject(dev->hdc,dev->hPen);
	SelectObject(dev->hdc,dev->hbr);
}
/*--------------------------------------------------------------------------*\
* line()
*
* Draw a line in the current color from (x1,y1) to (x2,y2).
\*--------------------------------------------------------------------------*/
void plD_line_win3(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
  WinDev *dev = (WinDev *)pls->dev;
	  SelectObject(dev->hdc,dev->hPen);
#ifdef WIN32
	  MoveToEx(dev->hdc,x1a * dev->xScale,(yRes - y1a) * dev->yScale,NULL);
#else
	  MoveTo(dev->hdc,x1a * dev->xScale,(yRes - y1a) * dev->yScale);
#endif
	  LineTo(dev->hdc,x2a * dev->xScale,(yRes - y2a) * dev->yScale);
	  if (!dev->rePaintBsy)
		if (dev->nice++ > niceCnt) {
			dev->nice = 0;
			checkMessage();
		}
	  DeleteObject(dev->hPen);
}

/*--------------------------------------------------------------------------*\
* bgi_polyline()
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
		pt[i].x = xa[i] * dev->xScale;
		pt[i].y = (yRes - ya[i]) * dev->yScale;
	}
	SelectObject(dev->hdc,dev->hPen);
	Polyline(dev->hdc,pt,npts);
	if (!dev->rePaintBsy) {
		dev->nice = 0;
		checkMessage();
	}
	DeleteObject(dev->hPen);
}
/*--------------------------------------------------------------------------*\
* bgi_eop()
*
* End of page.
\*--------------------------------------------------------------------------*/
void plD_eop_win3(PLStream *pls)
{
  WinDev *dev = (WinDev *)pls->dev;
  HCURSOR hCursor;

  ReleaseDC(dev->hwnd,dev->hdc);
  EnableMenuItem(dev->hMenu,CM_PRINTPLOT,MF_ENABLED);
  EnableMenuItem(dev->hMenu,CM_NEXTPLOT,MF_ENABLED);
  hCursor = LoadCursor(NULL,IDC_ARROW);
#ifdef WIN32
  SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)hCursor);
#else
  SetClassWord(GetActiveWindow(),GCW_HCURSOR,hCursor);
#endif
  SetCursor(hCursor);
  while (!dev->nextPlot) {
	 GetMessage(&msg,NULL,0,0);
	 TranslateMessage(&msg);
	 DispatchMessage(&msg);
  }
  InvalidateRect(dev->hwnd,NULL,TRUE);
  UpdateWindow(dev->hwnd);
  dev->nextPlot = 0;
}

/*--------------------------------------------------------------------------*\
* bop()
*
* Set up for the next page.
* Advance to next family file if necessary (file output).
\*--------------------------------------------------------------------------*/
void plD_bop_win3(PLStream *pls)
{
  WinDev *dev = (WinDev *)pls->dev;
  RECT rect;
  HCURSOR hCursor;

  EnableMenuItem(dev->hMenu,CM_PRINTPLOT,MF_GRAYED);
  EnableMenuItem(dev->hMenu,CM_NEXTPLOT,MF_GRAYED);

  hCursor = LoadCursor(NULL,IDC_WAIT);
#ifdef WIN32
  SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)hCursor);
#else
  SetClassWord(GetActiveWindow(),GCW_HCURSOR,hCursor);
#endif
  SetCursor(hCursor);

  dev->hdc = GetDC(dev->hwnd);
  GetClientRect(dev->hwnd,&rect);
  dev->xPhMax = rect.right;
  dev->yPhMax = rect.bottom;
  dev->xScale = rect.right / (xRes + 1);
  dev->yScale = rect.bottom / (yRes + 1);

  dev->rePaint = 0;
  dev->rePaintBsy = 0;
  dev->nice = 0;
  pls->page++;
//PC
  plD_state_win3(pls, PLSTATE_COLOR0);            // Set drawing color 
}

/*--------------------------------------------------------------------------*\
* bgi_tidy()
*
* Close graphics file or otherwise clean up.
\*--------------------------------------------------------------------------*/

void
plD_tidy_win3(PLStream *pls)
{
  WinDev *dev = (WinDev *)pls->dev;

  pls->page = 0;
  pls->OutFile = NULL;
  DestroyWindow(dev->hwnd);
}



/*--------------------------------------------------------------------------*\
* bgi_esc()
*
* Escape function.
\*--------------------------------------------------------------------------*/

void
plD_esc_win3(PLStream *pls, PLINT op , void *ptr)
{
  WinDev *dev = (WinDev *)pls->dev;
  HCURSOR holdcursor,hnewcursor;

  switch (op) {
  case PLESC_GETC:
	 hnewcursor = LoadCursor(NULL,IDC_CROSS);
#ifdef WIN32
	 holdcursor = (HCURSOR)GetClassLong(GetActiveWindow(),GCL_HCURSOR);
	 SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)hnewcursor);
#else
	 holdcursor = GetClassWord(GetActiveWindow(),GCW_HCURSOR);
	 SetClassWord(GetActiveWindow(),GCW_HCURSOR,hnewcursor);
#endif
	 SetCursor(hnewcursor);
	 dev->newCursor = 0;
	 while (!dev->newCursor)
		checkMessage();
	 ((PLGraphicsIn *)ptr)->dX = dev->cursorX; //PC / pls->xlength;
	 ((PLGraphicsIn *)ptr)->dY = dev->cursorY; //PC / pls->ylength;;
#ifdef WIN32
	 SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)holdcursor);
#else
	 SetClassWord(GetActiveWindow(),GCW_HCURSOR,holdcursor);
#endif
	 break;
  case PLESC_FILL:
	  FillPolygonCmd(pls);
	  break;
  }
}
static void win3_dispatch_init_helper( PLDispatchTable *pdt,
                                     char *menustr, char *devnam,
                                     int type, int seq, plD_init_fp init )
{
    pdt->pl_MenuStr = menustr;
    pdt->pl_DevName = devnam;
    pdt->pl_type = type;
    pdt->pl_seq = seq;
    pdt->pl_init     = init;
    pdt->pl_line     = (plD_line_fp)     plD_line_win3;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_win3;
    pdt->pl_eop      = (plD_eop_fp)      plD_eop_win3;
    pdt->pl_bop      = (plD_bop_fp)      plD_bop_win3;
    pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_win3;
    pdt->pl_state    = (plD_state_fp)    plD_state_win3;
    pdt->pl_esc      = (plD_esc_fp)      plD_esc_win3;
}

void plD_dispatch_init_win3	( PLDispatchTable *pdt )
{
    win3_dispatch_init_helper( pdt,
                             "PlPlot Win32 Window", "win3",
                             plDevType_FileOriented, 29,
                             (plD_init_fp) plD_init_win3 );
}
/*-------------------------------------------------------------*\
* FillPolygonCmd()
\*-------------------------------------------------------------*/
static void FillPolygonCmd(PLStream *pls) {
	POINT pt[PL_MAXPOLY];
	int i;
	WinDev *dev = (WinDev *)pls->dev;
	SelectObject(dev->hdc,dev->hPen);
	SelectObject(dev->hdc,dev->hbr);

	if (pls->dev_npts > PL_MAXPOLY)
		plexit("FillPolygonCmd : Too many points in polygon\n");
	for (i=0; i < pls->dev_npts;i++) {
		pt[i].x = pls->dev_x[i] * dev->xScale;
		pt[i].y = (yRes - pls->dev_y[i]) * dev->yScale;
	}
	SetPolyFillMode(dev->hdc,WINDING);
	Polygon(dev->hdc,pt,pls->dev_npts);

}

LRESULT CALLBACK _export PlPlotWndProc (HWND hwnd,UINT message,
  UINT wParam,LONG lParam)
{
  RECT rect;
  PAINTSTRUCT ps;
  PLStream *pls = (PLStream *)GetWindowLong(hwnd,0);
  WinDev *dev = NULL;
  HCURSOR hcurSave;
  HMETAFILE hmf;
  GLOBALHANDLE hGMem;
  LPMETAFILEPICT lpMFP;

  if (pls)
	 dev = (WinDev *)pls->dev;

  switch (message) {
	 case WM_LBUTTONDOWN :
		if (dev)
		  dev->nextPlot = 1;
		return 0;
	 case WM_RBUTTONDOWN :
		dev->newCursor = 1;
		dev->cursorX = float(LOWORD(lParam))/float(dev->xPhMax);
		dev->cursorY = float(dev->yPhMax - HIWORD(lParam)) / float(dev->yPhMax);
		return 0;
	 case WM_ERASEBKGND :
		if (!dev->rePaintBsy)
		  dev->rePaint = 1;
		break;
	 case WM_PAINT :
		if (dev) {
		  if (dev->rePaint) {
			 dev->rePaint = 0;
			 dev->rePaintBsy = 1;
			 hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT));
			 dev->hdc = GetDC(dev->hwnd);
			 GetClientRect(dev->hwnd,&rect);
			 dev->xPhMax = rect.right;
			 dev->yPhMax = rect.bottom;
			 dev->xScale = rect.right / (xRes + 1);
			 dev->yScale = rect.bottom / (yRes + 1);
			 plRemakePlot(pls);
			 dev->rePaintBsy = 0;
			 SetCursor(hcurSave);
			 ReleaseDC(dev->hwnd,dev->hdc);
		  }
		  BeginPaint(hwnd,&ps);
		  EndPaint(hwnd,&ps);
		  return 0;
		}
		break;
	 case WM_DESTROY :
//              PostQuitMessage(0);
		return 0;
	 case WM_COMMAND :
		switch (wParam) {
		  case  CM_NEXTPLOT :
			 if (dev)
				dev->nextPlot = 1;
			 return 0;
		  case CM_PRINTPLOT :
			 dev->rePaintBsy = 1;
			 if (dev->hdc = GetPrinterDC()) {
				dev->xScale = GetDeviceCaps(dev->hdc,HORZRES) / (xRes + 1);
				dev->yScale = GetDeviceCaps(dev->hdc,VERTRES) / (yRes + 1);
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
			 dev->rePaintBsy = 0;
			 dev->rePaint = 1;
			 return 0;
		  case CM_EDITCOPY :
			 dev->rePaintBsy = 1;
			 dev->hdc = CreateMetaFile(NULL);
#ifdef WIN32
			 SetWindowExtEx(dev->hdc,xRes,yRes,NULL);
			 SetWindowOrgEx(dev->hdc,0,0,NULL);
#else
			 SetWindowExt(dev->hdc,xRes,yRes);
			 SetWindowOrg(dev->hdc,0,0);
#endif
			 dev->xScale = 1.0;
			 dev->yScale = 1.0;
			 hcurSave = SetCursor(LoadCursor(NULL,IDC_WAIT));
			 plRemakePlot(pls);
			 SetCursor(hcurSave);

			 hmf = CloseMetaFile(dev->hdc);
			 hGMem = GlobalAlloc(GHND,(DWORD)sizeof(METAFILEPICT));
			 lpMFP = (LPMETAFILEPICT) GlobalLock(hGMem);

			 lpMFP->mm = MM_ISOTROPIC;
			 lpMFP->xExt = xRes;
			 lpMFP->yExt = yRes;
			 lpMFP->hMF = hmf;

			 GlobalUnlock(hGMem);

			 OpenClipboard(dev->hwnd);
			 EmptyClipboard();
			 SetClipboardData(CF_METAFILEPICT,hGMem);
			 CloseClipboard();

			 dev->rePaintBsy = 0;
			 dev->rePaint = 1;
			 return 0;
		  case CM_ABOUT :
			 MessageBox(hwnd,aboutText,"About",MB_OK);
			 return 0;
		}
  }
  return DefWindowProc(hwnd,message,wParam,lParam);
}
BOOL CALLBACK AbortProc( HDC hDC, int Error )
{
    MSG   msg;
    while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    return TRUE;
}
#else
pldummy_win3() {
   return 0;
}
#endif   //WIN3




