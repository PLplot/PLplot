/*
win3.cpp

  Driver for Win32
  
	Paul Casteels	11-Sep-1996	(casteels@uia.ua.ac.be)
	
	  Color Handling by
	  
		Carla Carmelo Rosa  (l36658@alfa.ist.utl.pt)
		
		  
  Modified 20/12/01 by Olof Svensson and Alexandre Gobbo
			
*/

#include "plplot/plDevs.h"
#ifdef PLD_win3

#include <stdio.h>
#include <string.h>
#include "plplot/plplotP.h"
#include "plplot/drivers.h"
#include "plplot/resource.h"
#include <windows.h>
#include <assert.h>

static int       color   = 1;
static unsigned int      hwnd = 0;
static MSG       msg;
static DrvOpt    win3_options[] = {
	{"color", DRV_INT, &color, "Use color (color=0|1)"},
	{"hwnd", DRV_INT, &hwnd, "Windows HWND handle (not supposed to be given as a command line argument)"},
	{NULL, DRV_INT, NULL, NULL}
};

LRESULT CALLBACK _export PlPlotWndProc (HWND hwnd,UINT message,UINT wParam,LONG lParam);

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
static void FillPolygonCmd (PLStream *pls);
/* BOOL CALLBACK AbortProc( HDC hDC, int Error ); */

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
	int 	rePaintBsy;       /* if we are repainting block the rest */
	int     externalWindow;   /* if true the window is provided externally */
	
	int 	newCursor, button, state;
	float 	cursorX,cursorY;
	
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
* Initialize device.
\*--------------------------------------------------------------------------*/


void plD_init_win3(PLStream *pls)
{
	HWND      hwndMain;
	WNDCLASS  wndclass;
	HINSTANCE hInstance;
	WinDev    *dev;
	int       greyvalue;
	long      backGroundColor;
	
	/* Initial window position */
	int xPos    = 100;
	int yPos    = 100;
	
	/* Initial window size */
	int nWidth  = 720;
	int nHeight = 540;
	
	int xmin = 0;
	int xmax = PIXELS_X-1;
	int ymin = 0;
	int ymax = PIXELS_Y-1;
	
	color = 1;
	hwnd = 0;
	pls->color = 1;		/* Is a color device */
	plParseDrvOpts(win3_options);
	if (!color) pls->color = 0; /* But user does not want color */
	
	/* Set up device parameters */
	pls->termin      = 1; /* is an interactive terminal */
	pls->icol0       = 1; /* current color */
	pls->width       = 1; /* current pen width */
	pls->bytecnt     = 0;
	pls->page        = 0;
	pls->plbuf_write = 1; /* buffer the output */
	pls->dev_flush   = 1; /* flush as we like */
	pls->dev_fill0   = 1;	
	pls->dev_xor     = 1; /* device support xor mode */
	if (pls->dev != NULL) delete pls->dev;
	pls->dev = new WinDev;
	assert(pls->dev != NULL);
	
	dev = (WinDev *) pls->dev;
	dev->nextPlot = 0;
	dev->hPen     = CreatePen(PS_SOLID,0,
		RGB(pls->cmap0[0].r,pls->cmap0[0].g,pls->cmap0[0].b));
	dev->hPenOld = (HPEN)SelectObject(dev->hdc,dev->hPen);
	dev->hbr      = CreateSolidBrush(RGB(pls->cmap0[0].r,pls->cmap0[0].g,pls->cmap0[0].b));
	dev->hbrOld   = (HBRUSH)SelectObject(dev->hdc,dev->hbr);
	dev->hMenu    = NULL;
	
	if (!hwnd) {
		/* Window created by the driver */
		dev->externalWindow = 0;
		hInstance = GetModuleHandle(NULL);
		
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_SAVEBITS;
		wndclass.lpfnWndProc = ::PlPlotWndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = sizeof(pls);
		wndclass.hInstance = hInstance;
		wndclass.hIcon = LoadIcon(hInstance,"PLICON");
		wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
		if (pls->color) {
			backGroundColor = RGB(pls->cmap0[0].r,pls->cmap0[0].g,pls->cmap0[0].b);
		} else {
			greyvalue = (pls->cmap0[0].r+pls->cmap0[0].g+pls->cmap0[0].b)/3;
			backGroundColor = RGB(greyvalue,greyvalue,greyvalue);
		}
        wndclass.hbrBackground = (struct HBRUSH__ *)CreateSolidBrush(backGroundColor);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szPlPlotClass;
		RegisterClass (&wndclass);
		
		
		dev->hwnd = CreateWindow(szPlPlotClass,szPlPlotWName,
			WS_OVERLAPPEDWINDOW,
			xPos,yPos,nWidth,nHeight,
			NULL,dev->hMenu,
			hInstance,NULL);
		
		SetWindowLong(dev->hwnd,GWL_USERDATA,(long)pls);
		
		ShowWindow(dev->hwnd,SW_SHOWDEFAULT);
		
		SetForegroundWindow(dev->hwnd);
		
	} else {
		/* Window provided externally */		
		dev->hwnd = (HWND)hwnd;
		dev->externalWindow = 1;
	}
	dev->hdc = GetDC(dev->hwnd);
	SetPolyFillMode(dev->hdc,WINDING);
	
	plP_setpxl(xmax/150.0/nWidth*nHeight,ymax/150.0);
	plP_setphy(xmin,xmax,ymin,ymax);
}


/*--------------------------------------------------------------------------*\
* setColor()
*
* Change the color of the pen and the brush
\*--------------------------------------------------------------------------*/
void setColor(PLStream *pls, int r, int g, int b)
{
	long color = RGB(r,g,b);
	WinDev *dev = (WinDev *) pls->dev;
	SelectObject(dev->hdc, dev->hPenOld);
	SelectObject(dev->hdc, dev->hbrOld);
	DeleteObject(dev->hPen);
	DeleteObject(dev->hbr);
	dev->lp.lopnColor=color;
	dev->lb.lbColor=color;
	dev->hPen = CreatePen(PS_SOLID,0,color);
	dev->hbr= CreateSolidBrush(color);
	dev->hPenOld = (HPEN)SelectObject(dev->hdc,dev->hPen);
	dev->hbrOld   = (HBRUSH)SelectObject(dev->hdc,dev->hbr);
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
		if (pls->color) 
			setColor(pls,pls->curcolor.r,pls->curcolor.g,pls->curcolor.b);
		else {
			greyvalue = (pls->curcolor.r+pls->curcolor.g+pls->curcolor.b)/3;
			setColor(pls,greyvalue,greyvalue,greyvalue);
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
	MoveToEx(dev->hdc,x1a * dev->xScale,(PIXELS_Y - y1a) * dev->yScale,NULL);
	LineTo(dev->hdc,x2a * dev->xScale,(PIXELS_Y - y2a) * dev->yScale);
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
		pt[i].x = xa[i] * dev->xScale;
		pt[i].y = (PIXELS_Y - ya[i]) * dev->yScale;
	}
	Polyline(dev->hdc,pt,npts);
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
	
	ReleaseDC(dev->hwnd,dev->hdc);
	if (!dev->externalWindow) {
	       /* EnableMenuItem(dev->hMenu,CM_PRINTPLOT,MF_ENABLED); */
	       /* EnableMenuItem(dev->hMenu,CM_NEXTPLOT,MF_ENABLED);  */
	       hCursor = LoadCursor(NULL,IDC_ARROW);
		   SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)hCursor);
		   SetCursor(hCursor);
		   while (!dev->nextPlot) {
			   GetMessage(&msg,NULL,0,0);
			   TranslateMessage(&msg);
			   DispatchMessage(&msg);
		   }
	}
	InvalidateRect(dev->hwnd,NULL,TRUE);
	UpdateWindow(dev->hwnd);
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
	
	hCursor = LoadCursor(NULL,IDC_WAIT);
	SetClassLong(GetActiveWindow(),GCL_HCURSOR,(long)hCursor);
	SetCursor(hCursor);
	
	dev->hdc = GetDC(dev->hwnd);
	GetClientRect(dev->hwnd,&rect);
	dev->xPhMax = rect.right;
	dev->yPhMax = rect.bottom;
	dev->xScale = rect.right / ((float)PIXELS_X);
	dev->yScale = rect.bottom / ((float)PIXELS_Y);
	
	dev->rePaint = 0;
	dev->rePaintBsy = 0;
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
	
	pls->page = 0;
	pls->OutFile = NULL;
	if (!dev->externalWindow) DestroyWindow(dev->hwnd);
}



/*--------------------------------------------------------------------------*\
* plD_esc_win3()
*
* Escape function.
\*--------------------------------------------------------------------------*/
void plD_esc_win3(PLStream *pls, PLINT op , void *ptr)
{
	WinDev *dev = (WinDev *)pls->dev;
	HCURSOR holdcursor,hnewcursor;
	
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
			InvalidateRect(dev->hwnd,NULL,TRUE);
		}
		break;
		
	case PLESC_EXPOSE:
		plreplot ();
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
		pt[i].x = pls->dev_x[i] * dev->xScale;
		pt[i].y = (PIXELS_Y - pls->dev_y[i]) * dev->yScale;
	}
	Polygon(dev->hdc,pt,pls->dev_npts);
}

LRESULT CALLBACK _export PlPlotWndProc (HWND hwnd,UINT message,	UINT wParam,LONG lParam)
{
	RECT rect;
	PAINTSTRUCT ps;
	PLStream *pls = (PLStream *)GetWindowLong(hwnd,GWL_USERDATA);
	WinDev *dev = NULL;
	HCURSOR hcurSave;
	HMETAFILE hmf;
	GLOBALHANDLE hGMem;
	LPMETAFILEPICT lpMFP;
	
	if (pls)
		dev = (WinDev *)pls->dev;
	
	switch (message) {
	case WM_LBUTTONDOWN :
		dev->newCursor = 1;
		dev->cursorX = LOWORD(lParam);
		dev->cursorY = HIWORD(lParam);
		dev->button = 1;
		dev->state = 0x000;
		return 0;
	case WM_RBUTTONDOWN :
		dev->newCursor = 1;
		dev->cursorX = LOWORD(lParam);
		dev->cursorY = HIWORD(lParam);
		dev->button = 3;
		if (dev) dev->nextPlot = 1;
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
				dev->xScale = rect.right / ((float)PIXELS_X);
				dev->yScale = rect.bottom / ((float)PIXELS_Y);
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
		/*
		case WM_COMMAND :
		switch (wParam) {
		case  CM_NEXTPLOT :
		if (dev)
		dev->nextPlot = 1;
		return 0;
		case CM_PRINTPLOT :
		dev->rePaintBsy = 1;
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
		dev->rePaintBsy = 0;
		dev->rePaint = 1;
		return 0;
		case CM_EDITCOPY :
		dev->rePaintBsy = 1;
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
				
				  dev->rePaintBsy = 0;
				  dev->rePaint = 1;
				  return 0;
				  case CM_ABOUT :
				  //			 MessageBox(hwnd,aboutText,"About",MB_OK);
				  return 0;
				  
					}
		*/
  }
  return DefWindowProc(hwnd,message,wParam,lParam);
}

/*
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
*/
#else
pldummy_win3() {
	return 0;
}
#endif   //WIN3




