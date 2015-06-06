//      PLplot WIN32 GDI driver.
//
// Copyright (C) 2004  Andrew Roach
//
// This file is part of PLplot.
//
// PLplot is free software; you can redistribute it and/or modify
// it under the terms of the GNU Library General Public License as published
// by the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// PLplot is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public License
// along with PLplot; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
//
//
#include "plDevs.h"

#ifdef PLD_wingdi

#include <string.h>
#include <windows.h>
#if !defined ( __CYGWIN__ )
#include <tchar.h>
#else
#include <winnt.h>
#define _T( a )    __TEXT( a )
#endif

#define NEED_PLDEBUG
#define DEBUG
#include "plplotP.h"
#include "drivers.h"
#include "plevent.h"

#define INITIAL_HEAP_SIZE 16384     // Initial size of heap in bytes

// Enumerated type for the device states
enum _dev_state 
{
   DEV_WAITING = 0,   // Device is idle
   DEV_ACTIVE,        // Device is ready for next plot
   DEV_SIZEMOVE,	  // Device might be sizing or moving the window
   DEV_RESIZE,        // Device is resizing the window
   DEV_DRAWING        // Device is actively drawing
};

// Enumerated type used to indicate the device type
// for updating page metrics
enum _page_device
{
	PAGE_WINDOW,      // Setup page metrics for a window
	PAGE_PRINTER,     // Setup page metrics for a printer
};

// Device-specific info per stream
struct wingdi_Dev
{
	//
	// Members that are common to interactive GUI devices
	//
	PLFLT        xscale;	 // Virtual x pixels to device pixel scaling
	PLFLT		 yscale;	 // Virtual y pixels to device pixel scaling
    PLINT        width;      // Window Width (which can change)
    PLINT        height;     // Window Height

	enum _dev_state state;      // Current state of the device
	enum _dev_state prev_state; // Previous state of the device
							      // Used to restore after redraw
								  
    //
    // WIN32 API variables
    //
    HWND              hwnd;       // Handle for the main window.
	HDC               hdc;        // Plot window device context
	HPEN              pen;        // Current pen used for drawing
	COLORREF		  color;      // Current color
	HDC               hdc_bmp;    // Bitmap device context
    HBITMAP           bitmap;     // Bitmap of current display
};

// Device info
PLDLLIMPEXP_DRIVER const char* plD_DEVICE_INFO_wingdi = "wingdi:Windows GDI:1:wingdi:9:wingdi\n";

// Counter used to indicate the number of streams in use
static int wingdi_streams = 0; 

// Window class for a plot window
static TCHAR* plot_window_class_name = _T( "PlplotWin" );
static WNDCLASSEX plot_window_class;

// Popup menu used by a plot window
static HMENU plot_popup_menu;

// Private heap used to allocate memory for the driver
static HANDLE wingdi_heap;

PLDLLIMPEXP_DRIVER void plD_dispatch_init_wingdi( PLDispatchTable *pdt );
void plD_init_wingdi( PLStream * );

//--------------------------------------------------------------------------
// Graphics primitive implementation functions
//--------------------------------------------------------------------------
static void plD_line_wingdi( PLStream *, short, short, short, short );
static void plD_polyline_wingdi( PLStream *, short *, short *, PLINT );
static void plD_fill_polygon_wingdi( PLStream *pls );
static void plD_eop_wingdi( PLStream * );
static void plD_bop_wingdi( PLStream * );
static void plD_tidy_wingdi( PLStream * );
static void plD_state_wingdi( PLStream *, PLINT );
static void plD_esc_wingdi( PLStream *, PLINT, void * );

#define PopupPrint       0x08A1
#define PopupNextPage    0x08A2
#define PopupQuit        0x08A3

void plD_dispatch_init_wingdi( PLDispatchTable *pdt )
{
#ifndef ENABLE_DYNDRIVERS
    pdt->pl_MenuStr = "Win32/64 GDI device";
    pdt->pl_DevName = "wingdi";
#endif
    pdt->pl_type     = plDevType_Interactive;
    pdt->pl_seq      = 9;
    pdt->pl_init     = (plD_init_fp) plD_init_wingdi;
    pdt->pl_line     = (plD_line_fp) plD_line_wingdi;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_wingdi;
    pdt->pl_eop      = (plD_eop_fp) plD_eop_wingdi;
    pdt->pl_bop      = (plD_bop_fp) plD_bop_wingdi;
    pdt->pl_tidy     = (plD_tidy_fp) plD_tidy_wingdi;
    pdt->pl_state    = (plD_state_fp) plD_state_wingdi;
    pdt->pl_esc      = (plD_esc_fp) plD_esc_wingdi;
}

static void
CrossHairCursor( struct wingdi_Dev * dev )
{
	HCURSOR cursor;
	
    cursor = LoadCursor( NULL, IDC_CROSS );
    SetClassLong( dev->hwnd, GCL_HCURSOR, (long) cursor );
    SetCursor( cursor ); 
}

static void
NormalCursor( struct wingdi_Dev * dev)
{
	HCURSOR cursor;
	
	cursor = LoadCursor( NULL, IDC_ARROW );
    SetClassLongPtr( dev->hwnd, GCL_HCURSOR, (LONG_PTR) cursor );
	SetCursor( cursor ); 
}

static void
BusyCursor( struct wingdi_Dev * dev )
{
	HCURSOR cursor;
	
	cursor = LoadCursor( NULL, IDC_WAIT );
    SetClassLongPtr( dev->hwnd, GCL_HCURSOR, (LONG_PTR) cursor );
    SetCursor( cursor ); 
}

//--------------------------------------------------------------------------
//  static void UpdatePageMetrics ( PLStream *pls, char flag )
//
//      UpdatePageMetrics is a simple function which simply gets new values 
//      for a changed DC, be it swapping from printer to screen or vice-versa.
//      The flag variable is used to tell the function if it is updating
//      from the printer (1) or screen (0).
//--------------------------------------------------------------------------
static void UpdatePageMetrics( PLStream *pls, enum _page_device dev_type )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;

    if ( dev_type == PAGE_PRINTER )
    {
		// Get the page size from the printer
        dev->width  = GetDeviceCaps( dev->hdc, HORZRES ); 
        dev->height = GetDeviceCaps( dev->hdc, VERTRES );
    }
    else
    {
		RECT rect;
		
        GetClientRect( dev->hwnd, &rect );
        dev->width  = rect.right;
        dev->height = rect.bottom;
		
		pldebug( "wingdi", "Page size [%d %d] [%d %d]\n", 
				 rect.left, rect.top, 
				 rect.right, rect.bottom );
    }

	dev->xscale = (PLFLT) PIXELS_X / dev->width;
	dev->yscale = (PLFLT) PIXELS_Y / dev->height;

    pldebug( "wingdi", "Scale = (%f %f) (FLT)\n",
			 dev->xscale, dev->yscale );

	// Need to get the DPI information from Windows
	// HORZRES/VERTRES = Width/Height in pixels
	// HORZSIZE/VERTSIZE = Width/Height in millimeters
	pldebug( "wingdi", "Original xdpi = %f ydpi = %f\n",
			 pls->xdpi, pls->ydpi );
	pls->xdpi = GetDeviceCaps( dev->hdc, HORZRES ) 
				/ GetDeviceCaps( dev->hdc, HORZSIZE ) * 25.4;
	pls->ydpi = GetDeviceCaps( dev->hdc, VERTRES ) 
				/ GetDeviceCaps( dev->hdc, VERTSIZE ) * 25.4;
	pldebug( "wingdi", "New xdpi = %f ydpi = %f\n",
			 pls->xdpi, pls->ydpi );
	
	// Set the mapping from pixels to mm
    plP_setpxl( dev->xscale * pls->xdpi / 25.4, 
				dev->yscale * pls->ydpi / 25.4 );

	// Set the physical limits for this device
    plP_setphy( 0, PIXELS_X, 
				0, PIXELS_Y );
}

//--------------------------------------------------------------------------
//  static void PrintPage ( PLStream *pls )
//
//     Function brings up a standard printer dialog and, after the user
//     has selected a printer, replots the current page to the windows
//     printer.
//--------------------------------------------------------------------------
static void PrintPage( PLStream *pls )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;
    PRINTDLG   Printer;
    DOCINFO    docinfo;
    struct wingdi_Dev *push;      // A copy of the entire structure

    //    Reset the docinfo structure to 0 and set it's fields up
    //    This structure is used to supply a name to the print queue
    ZeroMemory( &docinfo, sizeof ( docinfo ) );
    docinfo.cbSize      = sizeof ( docinfo );
    docinfo.lpszDocName = _T( "Plplot Page" );

    //   Reset out printer structure to zero and initialise it
    ZeroMemory( &Printer, sizeof ( PRINTDLG ) );
    Printer.lStructSize = sizeof ( PRINTDLG );
    Printer.hwndOwner   = dev->hwnd;
    Printer.Flags       = PD_NOPAGENUMS | PD_NOSELECTION | PD_RETURNDC;
    Printer.nCopies     = 1;

    //   Call the printer dialog function.
    //   If the user has clicked on "Print", then we will continue
    //   processing and print out the page.
    if ( PrintDlg( &Printer ) != 0 )
    {
        //  Before doing anything, we will take some backup copies
        //  of the existing values for page size and the like, because
        //  all we are going to do is a quick and dirty modification
        //  of plplot's internals to match the new page size and hope
        //  it all works out ok. After that, we will manip the values,
        //  and when all is done, restore them.
		push = HeapAlloc( wingdi_heap, 0, sizeof ( struct wingdi_Dev ) );
        if ( push != NULL )
        {
            BusyCursor( dev );
            memcpy( push, dev, sizeof ( struct wingdi_Dev ) );

			// Change the device context to the printer
            dev->hdc = Printer.hDC;
            UpdatePageMetrics( pls, PAGE_PRINTER );

            //
            //   Now the stuff that actually does the printing !!
            //
            StartDoc( dev->hdc, &docinfo );
            plRemakePlot( pls );
            EndDoc( dev->hdc );

            //
            //  Now to undo everything back to what it was for the screen
            //
            memcpy( dev, push, sizeof ( struct wingdi_Dev ) );
			UpdatePageMetrics( pls, PAGE_WINDOW );

            HeapFree( wingdi_heap, 0, push );
            NormalCursor( dev );
            RedrawWindow( dev->hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ERASENOW );
        }
    }
}

static void
wait_for_user_input( PLStream *pls )
{
	struct wingdi_Dev * dev = (struct wingdi_Dev *) pls->dev;
	MSG msg;
	
    dev->state = DEV_WAITING;
	pldebug( "wingdi", "Waiting for user input\n" );
	
	// Process messages in the queue or until we are no longer waiting
    while ( GetMessage( &msg, NULL, 0, 0 ) && dev->state != DEV_ACTIVE )
    {
        TranslateMessage( &msg );
        switch ( (int) msg.message )
        {
        case WM_CONTEXTMENU:
        case WM_RBUTTONDOWN:
			TrackPopupMenu( plot_popup_menu, 
						    TPM_CENTERALIGN | TPM_RIGHTBUTTON, 
							LOWORD( msg.lParam ),
							HIWORD( msg.lParam ), 
							0, 
							dev->hwnd, 
							NULL );
            break;

        case WM_CHAR:
            if ( ( (TCHAR) ( msg.wParam ) == 32 ) ||
                 ( (TCHAR) ( msg.wParam ) == 13 ) )
            {
                dev->state = DEV_ACTIVE;
            }
            else if ( ( (TCHAR) ( msg.wParam ) == 27 ) ||
                      ( (TCHAR) ( msg.wParam ) == 'q' ) ||
                      ( (TCHAR) ( msg.wParam ) == 'Q' ) )
            {
                dev->state = DEV_ACTIVE;
                PostQuitMessage( 0 );
            }
            break;

        case WM_LBUTTONDBLCLK:
            pldebug( "wingdi", "WM_LBUTTONDBLCLK\n" );
            dev->state = DEV_ACTIVE;
            break;

        case WM_COMMAND:
            switch ( LOWORD( msg.wParam ) )
            {
            case PopupPrint:
                pldebug( "wingdi", "PopupPrint\n" );
                PrintPage( pls );
                break;
            case PopupNextPage:
                pldebug( "wingdi", "PopupNextPage\n" );
                dev->state = DEV_ACTIVE;
                break;
            case PopupQuit:
                pldebug( "wingdi", "PopupQuit\n" );
                dev->state = DEV_ACTIVE;
                PostQuitMessage( 0 );
                break;
            }
            break;

        default:
            DispatchMessage( &msg );
            break;
        }
    }
	
	pldebug( "wingdi", "Done waiting\n" );
}
			
//--------------------------------------------------------------------------
// GetCursorCmd()
//
// Handle events connected to selecting points (modelled after xwin)
//--------------------------------------------------------------------------

static void
GetCursorCmd( PLStream *pls, PLGraphicsIn *gin )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;
	MSG msg;
    HCURSOR    crosshair;
    HCURSOR    previous;

    plGinInit( gin );

    crosshair = LoadCursor( GetModuleHandle( NULL ), IDC_CROSS );
    previous  = SetCursor( crosshair );

    while ( gin->pX < 0 )
    {
        GetMessage( &msg, NULL, 0, 0 );
        TranslateMessage( &msg );
        switch ( (int) msg.message )
        {
        case WM_LBUTTONDOWN:
            if ( msg.wParam & MK_LBUTTON )
            {
                gin->pX = msg.pt.x;
                gin->pY = msg.pt.y;
                gin->dX = (PLFLT) gin->pX / ( dev->width - 1 );
                gin->dY = 1.0 - (PLFLT) gin->pY / ( dev->height - 1 );

                gin->button = 1; // AM: there is no macro to indicate the pressed button!
                gin->state  = 0; // AM: is there an equivalent under Windows?
                gin->keysym = 0x20;
            }
            break;
        case WM_CHAR:
            gin->pX = msg.pt.x;
            gin->pY = msg.pt.y;
            gin->dX = (PLFLT) gin->pX / ( dev->width - 1 );
            gin->dY = 1.0 - (PLFLT) gin->pY / ( dev->height - 1 );

            gin->button = 0;
            gin->state  = 0;
            gin->keysym = msg.wParam;

            break;
        }
    }

    // Restore the previous cursor
    SetCursor( previous );
}

//--------------------------------------------------------------------------
//    static void CopySCRtoBMP(PLStream *pls)
//       Function copies the screen contents into a bitmap which is
//       later used for fast redraws of the screen (when it gets corrupted)
//       rather than remaking the plot from the plot buffer.
//--------------------------------------------------------------------------
static void CopySCRtoBMP( PLStream *pls )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;
	RECT rect;
	HGDIOBJ previous;
	
	if ( dev->hdc_bmp != NULL )
		DeleteDC( dev->hdc_bmp );
    if ( dev->bitmap != NULL )
        DeleteObject( dev->bitmap );

    dev->hdc_bmp = CreateCompatibleDC( dev->hdc );
    GetClientRect( dev->hwnd, &rect );
    dev->bitmap    = CreateCompatibleBitmap( dev->hdc, 
										     rect.right, rect.bottom );
    previous = SelectObject( dev->hdc_bmp, dev->bitmap );
    BitBlt( dev->hdc_bmp, 0, 0, rect.right, rect.bottom, dev->hdc, 0, 0, SRCCOPY );
    SelectObject( dev->hdc_bmp, previous );
}

//--------------------------------------------------------------------------
// static void Resize( PLStream *pls )
//
// This function calculates how to resize a window after a message has been
// received from windows telling us the window has been changed.
// It tries to recalculate the scale of the window so everything works out
// just right.  This function assumes that is being called from a WM_PAINT
// message, thus it does not do anything to force a redraw.
//--------------------------------------------------------------------------
static void Resize( PLStream *pls )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;
	RECT rect;
	enum _dev_state current;
	
    pldebug( "wingdi", "Resizing\n" );

	// Only resize the window IF plplot has finished with it
    if ( dev->state == DEV_WAITING )
    {
        GetClientRect( dev->hwnd, &rect );
        pldebug( "wingdi", "[%d %d] [%d %d]\n", 
			rect.left, rect.top,
			rect.right, rect.bottom );
		
		// Check to make sure it isn't just minimised (i.e. zero size)
        if ( ( rect.right > 0 ) && ( rect.bottom > 0 ) )
        {
			UpdatePageMetrics( pls, PAGE_WINDOW );
			
			// Save the current state because remaking the plot
			// will change it when the BOP is executed.
			current = dev->state;
			plRemakePlot( pls );
			dev->state = current;
			
			// Regenerate the bitmap so that it is available for the
			// WM_PAINT message
            CopySCRtoBMP( pls );
        }
    }
}

//--------------------------------------------------------------------------
// This is the window function for the plot window. Whenever a message is
// dispatched using DispatchMessage (or sent with SendMessage) this function
// gets called with the contents of the message.
//--------------------------------------------------------------------------

LRESULT CALLBACK PlplotWndProc( HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam )
{
    PLStream   *pls = NULL;
    struct wingdi_Dev *dev = NULL;

    // During WM_CREATE message, the PLStream pointer is not set, thus
	// we need to handle this message without attempting to getting the user data
    if ( nMsg == WM_CREATE )
    {
		// Signal that the message was handled
        return ( 0 );
    }

	// Try to get the address to pls for this window
#ifdef _WIN64
	pls = (PLStream *) GetWindowLongPtr( hwnd, GWLP_USERDATA );
#else
	pls = (PLStream *) GetWindowLongPtr( hwnd, GWL_USERDATA ); 
#endif

	// If we did not get a valid pointer, pass the message to the
	// default message handler
    if ( pls == NULL )                  
    {
		return DefWindowProc( hwnd, nMsg, wParam, lParam );
	}

    dev = (struct wingdi_Dev *) pls->dev;

    //
    // Process the windows messages
    //
    // Everything except WM_CREATE is done here and it is generally hoped that
    // pls and dev are defined already by this stage.
    // That will be true MOST of the time. Some times WM_PAINT will be called
    // before we get to initialise the user data area of the window with the
    // pointer to the windows plplot stream
    //

    switch ( nMsg )
    {
    case WM_DESTROY:
        pldebug( "wingdi", "WM_DESTROY\n" );
        PostQuitMessage( 0 );
        return ( 0 );
        break;

    case WM_PAINT:
		// A WM_PAINT message gets sent on an expose, resize, and move
		// events.  On expose and move events, a blit of a bitmap will
		// be performed.  On a resize, the plot needs to be regenerated.
		// Because a WM_PAINT is sent after the triggering event 
		// (e.g. a move), the triggering event is responsible for ensuring
		// a valid bitmap exists.  Thus, this message handler only needs
		// to blit the bitmap to the window.
        pldebug( "wingdi", "WM_PAINT\n" );

		// Check to see if there is an area that needs to be redrawn.
		// Per the MSDN document, calling GetUpdateRect with a NULL RECT
		// value will determine if there is an update region.  BeginPaint()
		// will provide the update region in rcPaint.
		if ( GetUpdateRect( dev->hwnd, NULL, FALSE ) )
		{
			// Yes there is, start the redraw
			PAINTSTRUCT  ps;
			HGDIOBJ      previous;
				
			BusyCursor( dev );
			BeginPaint( dev->hwnd, &ps );
			
			pldebug( "wingdi", "  Need to redraw area (%d,%d) (%d,%d)\n",
					 ps.rcPaint.left, ps.rcPaint.top,
					 ps.rcPaint.right, ps.rcPaint.bottom );
			pldebug( "wingdi", "  Erase status = %d\n", ps.fErase );
			pldebug( "wingdi", "  Device state = %d\n", dev->state );
			
			// If we have a valid bitmap and are not currently drawing
			// a new plot, then we can blit the bitmap to handle the 
			// redraw.  On a resize, this will result in the bitmap being 
			// clipped during the resize.  A StretchBlt could be used to 
			// rescale the bitmap; however, not all devices support
			// stretch blits.
			if( dev->bitmap != NULL && dev->state != DEV_DRAWING ) 
			{
				// A bitmap exists, thus only a blit is required
				previous = SelectObject( dev->hdc_bmp, dev->bitmap );
				BitBlt( dev->hdc, 
						ps.rcPaint.left, ps.rcPaint.top,
						ps.rcPaint.right, ps.rcPaint.bottom,
						dev->hdc_bmp, 
						ps.rcPaint.left, ps.rcPaint.top, 
						SRCCOPY );
				SelectObject( dev->hdc_bmp, previous );			
			}

			EndPaint( dev->hwnd, &ps );
			NormalCursor( dev );
        }
		
		// Signal that the message was processed
		return ( 0 );
        break;

	case WM_SIZE:
		pldebug( "wingdi", "WM_SIZE wParam = %d\n", wParam );
		// The size might have changed.  We only care about
		// Maximized events, a drag resize, or a restore.
		if( wParam == SIZE_MAXIMIZED ) {
			// The window was maximized, which is a resize
			Resize( pls );
		}
		else if( dev->state == DEV_SIZEMOVE ) 
		{
			// This is a drag resize.  Must check before the SIZE_RESTORED
			// because a drag resize is also a SIZE_RESTORED.  
			
			// Change the state to indicate that the window has changed
			// size and not just moved.
			pldebug( "wingdi", "  Save state %d\n", dev->state );	
			dev->state = DEV_RESIZE;
		} 
		else if( wParam == SIZE_RESTORED ) 
		{
			// This could be a restore from a maximized or minimized state.
			// Unless code is added to detect the difference (i.e. look for
			// an earlier SIZE_MINIMIZED), just treat it as a resize
			Resize( pls );
		}
		return( 0 );
		break;
		
    case WM_ENTERSIZEMOVE:
        pldebug( "wingdi", "WM_ENTERSIZEMOVE\n" );
		pldebug( "wingdi", "  Save state %d\n", dev->state );
		dev->prev_state = dev->state;
		// Indicate that we might be sizing or moving the window
		dev->state = DEV_SIZEMOVE;
        return ( 0 );
        break;

    case WM_EXITSIZEMOVE:
        pldebug( "wingdi", "WM_EXITSIZEMOVE\n" );
		// If the window has been resized, regenerate the plot
		// for the new window dimensions
		if( dev->state == DEV_RESIZE || dev->state == DEV_SIZEMOVE )
		{
			pldebug( "wingdi", "  Restore state %d\n", dev->prev_state );
			// Restore the previous state before handling the resize
			// The resize routine needs the original state to preserve
			// the state when the buffer is replayed.
			dev->state = dev->prev_state;
			Resize( pls );
		}
        return ( 0 );
        break;

    case WM_ERASEBKGND:
        // Determine if the window needs to be erased based
		// on the current state.
		// DEV_WAITING = No erase necessary, the next WM_PAINT
		//    will repaint the affected area
		// DEV_ACTIVE = Erase the client area because a new
		//    a plot will need to be generated
		if( dev->state != DEV_WAITING )
		{
			COLORREF previous_color;
			RECT rect;
			
            pldebug( "wingdi", "WM_ERASEBKGND\n" );

            //
            //    This is a new "High Speed" way of filling in the background.
            //    supposedly this executes faster than creating a brush and
            //    filling a rectangle - go figure ?
            //
			// NOTE:  Should GetUpdateRect be used instead?
			GetClientRect( dev->hwnd, &rect );
            previous_color = SetBkColor( dev->hdc, 
			                             RGB( pls->cmap0[0].r, pls->cmap0[0].g, pls->cmap0[0].b ) );
            ExtTextOut( dev->hdc, 0, 0, ETO_OPAQUE, &rect, _T( "" ), 0, 0 );
            SetBkColor( dev->hdc, previous_color );

            return ( 1 );
        }
        return ( 0 );
        break;

    case WM_COMMAND:
        pldebug( "wingdi", "WM_COMMAND\n" );
        return ( 0 );
        break;
    }

    // If we don't handle a message completely we hand it to the system
    // provided default window function.
    return DefWindowProc( hwnd, nMsg, wParam, lParam );
}

//--------------------------------------------------------------------------
// wingdi_module_initialize()
//
// Handles the initialization of window classes and module global
// variables.
//--------------------------------------------------------------------------
static void
wingdi_module_initialize( void )
{	
    // Return if the module has been initialized
	if(wingdi_streams > 0) return;
	wingdi_streams++;
	
	pldebug( "wingdi", "module init\n" );
	
	// Initialize the entire structure to zero.
    memset( &plot_window_class, 0, sizeof ( WNDCLASSEX ) );

	// Set the name of the plot window class
    plot_window_class.lpszClassName = plot_window_class_name;

    // Set the size of the window class structure, to include
	// any extra data that might be passed
    plot_window_class.cbSize = sizeof ( WNDCLASSEX );

    // All windows of this class redraw when resized.
    plot_window_class.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS 
                              | CS_OWNDC | CS_PARENTDC;

    // Set the callback function.
    plot_window_class.lpfnWndProc = PlplotWndProc;

    // This class is used with the current program instance.
    plot_window_class.hInstance = GetModuleHandle( NULL );

    // Use standard application icon and arrow cursor provided by the OS
    plot_window_class.hIcon   = LoadIcon( NULL, IDI_APPLICATION );
    plot_window_class.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
    plot_window_class.hCursor = LoadCursor( NULL, IDC_ARROW );

    // Handle the erase background in the callback function
    plot_window_class.hbrBackground = NULL;

	// Allocate extra space for a window instance to store the
	// pointer to the plot stream (PLStream)
    plot_window_class.cbWndExtra = sizeof ( PLStream * );

    //
    // Now register the window class for use.
    //
    RegisterClassEx( &plot_window_class );

    //
    // Create the popup menu used by the plot window
    //
    plot_popup_menu = CreatePopupMenu();
    AppendMenu( plot_popup_menu, MF_STRING, PopupPrint, _T( "Print" ) );
    AppendMenu( plot_popup_menu, MF_STRING, PopupNextPage, _T( "Next Page" ) );
    AppendMenu( plot_popup_menu, MF_STRING, PopupQuit, _T( "Quit" ) );	

	//
	// Create a private heap to use for memory allocation.
	// This will keep memory separate from the overall program
	// and does not have the overhead of GlobalAlloc().  Requires
	// a minimum of Windows XP or Windows Server 2003
	//
	wingdi_heap = HeapCreate(0, INITIAL_HEAP_SIZE, 0);
	if(wingdi_heap == NULL)
	{
		//plexit("wingdi: Unable to allocate heap of size %d bytes",
		//	   INITIAL_HEAP_SIZE);
		plexit("wingdi: Unable to allocate heap");
	}
}

static void wingdi_module_cleanup( void )
{
	wingdi_streams--;
	if(wingdi_streams > 0) return;
	
    DeleteMenu( plot_popup_menu, PopupPrint, 0 );
    DeleteMenu( plot_popup_menu, PopupNextPage, 0 );
    DeleteMenu( plot_popup_menu, PopupQuit, 0 );
    DestroyMenu( plot_popup_menu );
	
	if( HeapDestroy(wingdi_heap) == 0 ) {
		plexit("wingdi: Failed to destroy heap");
	}
	
	if( ! UnregisterClass(plot_window_class_name, plot_window_class.hInstance) ) {
		plexit("wingdi: Failed to unregister window class");
	}
}

//--------------------------------------------------------------------------
// plD_init_wingdi()
//
// Initialize device (terminal).
//--------------------------------------------------------------------------

void
plD_init_wingdi( PLStream *pls )
{
    struct wingdi_Dev *dev;
    DrvOpt wingdi_options[] = {
        { NULL,     DRV_INT, NULL,         NULL}
    };
    TCHAR *program;
#ifdef UNICODE
    int   programlength;
#endif

	pls->debug = 1;
	pldebug( "wingdi", "Device Init\n" );
	
	// Initialize the module
	wingdi_module_initialize();
	
    // Allocate and initialize device-specific data
    if ( pls->dev != NULL )
        free( (void *) pls->dev );

    pls->dev = calloc( 1, (size_t) sizeof ( struct wingdi_Dev ) );
    if ( pls->dev == NULL )
        plexit( "plD_init_wingdi_Dev: Out of memory." );

	// Shortcut to the device specific data structure
    dev = (struct wingdi_Dev *) pls->dev;
	
    pls->icol0 = 1;                   // Set a fall back pen color in case user doesn't

    pls->termin      = 1;             // interactive device
    pls->graphx      = GRAPHICS_MODE; // No text mode for this driver (at least for now, might add a console window if I ever figure it out and have the inclination)
    pls->dev_fill0   = 1;             // driver can do solid area fills
    pls->dev_xor     = 1;             // driver supports xor mode
    pls->dev_clear   = 0;             // driver does not support clear - what is the proper API?
    pls->dev_dash    = 0;             // driver can not do dashed lines (yet)
    pls->plbuf_write = 1;             // driver uses the buffer for redraws

    if ( !pls->colorset )
        pls->color = 1;

    // Check for and set up driver options
    plParseDrvOpts( wingdi_options );

    // Set up the initial device parameters.  This will be updated
	// after the plot window is initialized.
    if ( pls->xlength <= 0 || pls->ylength <= 0 )
    {
        // use default width, height of 800x600 if not specified by 
		// -geometry option or plspage
        plspage( 0., 0., 800, 600, 0, 0 );
    }

    dev->width  = pls->xlength - 1;     // should I use -1 or not???
    dev->height = pls->ylength - 1;

#ifdef UNICODE
    //convert the program name to wide char
    programlength = strlen( pls->program ) + 1;
    program       = malloc( programlength * sizeof ( TCHAR ) );
    MultiByteToWideChar( CP_UTF8, 0, pls->program, programlength, program, programlength );
#else
    program = pls->program;
#endif
  
    // Create our main window using the plot window class
    dev->hwnd = CreateWindowEx( 
		WS_EX_WINDOWEDGE + WS_EX_LEFT,
        plot_window_class_name,        // Class name
        program,                       // Caption
        WS_OVERLAPPEDWINDOW,           // Style
        pls->xoffset,                  // Initial x (use default)
        pls->yoffset,                  // Initial y (use default)
        pls->xlength,                  // Initial x size (use default)
        pls->ylength,                  // Initial y size (use default)
        NULL,                          // No parent window
        NULL,                          // No menu
        plot_window_class.hInstance,   // This program instance
        NULL                           // Creation parameters
        );

#ifdef UNICODE
    free( program );
#endif

    // Attach a pointer to the stream to the window's user area
    // this pointer will be used by the windows call back for
    // process this window
#ifdef _WIN64
    SetWindowLongPtr( dev->hwnd, GWLP_USERDATA, (LONG_PTR) pls );
#else
	SetWindowLongPtr( dev->hwnd, GWL_USERDATA, (LONG) pls );
#endif

	// Get the device context of the window that was created
	dev->hdc = GetDC( dev->hwnd );

	// Set the initial color for the drawing pen
    plD_state_wingdi( pls, PLSTATE_COLOR0 );

    // Display the window which we just created (using the nShow
    // passed by the OS, which allows for start minimized and that
    // sort of thing).
    ShowWindow( dev->hwnd, SW_SHOWDEFAULT );
    SetForegroundWindow( dev->hwnd );

    // Set up the DPI etc...
    if ( pls->xdpi <= 0 ) // Get DPI from windows
    {
        plspage( GetDeviceCaps( dev->hdc, HORZRES ) 
				 / GetDeviceCaps( dev->hdc, HORZSIZE ) * 25.4,
                 GetDeviceCaps( dev->hdc, VERTRES ) 
				 / GetDeviceCaps( dev->hdc, VERTSIZE ) * 25.4, 
				 0, 0, 0, 0 );
    }
    else
    {
        pls->ydpi = pls->xdpi;        // Set X and Y dpi's to the same value
    }

    //
    //  Now we have to find out, from windows, just how big our drawing area is
    //  when we specified the page size earlier on, that includes the borders,
    //  title bar etc... so now that windows has done all its initialisations,
    //  we will ask how big the drawing area is, and tell plplot
    //
	UpdatePageMetrics( pls, PAGE_WINDOW );
	plspage( pls->xdpi, pls->ydpi, 0, 0, 0, 0 );

    // Set fill rule.
    if ( pls->dev_eofill )
        SetPolyFillMode( dev->hdc, ALTERNATE );
    else
        SetPolyFillMode( dev->hdc, WINDING );

	// Indicate that the plot window is active
	dev->state = DEV_ACTIVE;
}

//--------------------------------------------------------------------------
// plD_line_wingdi()
//
// Draw a line in the current color from (x1,y1) to (x2,y2).
//--------------------------------------------------------------------------

void
plD_line_wingdi( PLStream *pls, short x1a, short y1a, short x2a, short y2a )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;
	HGDIOBJ previous_obj;
    POINT      points[2];

    points[0].x = (LONG) ( x1a / dev->xscale );
    points[1].x = (LONG) ( x2a / dev->xscale );
    points[0].y = (LONG) ( dev->height - ( y1a / dev->yscale ) );
    points[1].y = (LONG) ( dev->height - ( y2a / dev->yscale ) );

    previous_obj = SelectObject( dev->hdc, dev->pen );

    if ( points[0].x != points[1].x || points[0].y != points[1].y )
    {
        Polyline( dev->hdc, points, 2 );
    }
    else
    {
        SetPixel( dev->hdc, points[0].x, points[0].y, dev->color );
    }
    SelectObject( dev->hdc, previous_obj );
}

//--------------------------------------------------------------------------
// plD_polyline_wingdi()
//
// Draw a polyline in the current color.
//--------------------------------------------------------------------------

void
plD_polyline_wingdi( PLStream *pls, short *xa, short *ya, PLINT npts )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;
    int        i;
    POINT      *points = NULL;
	HGDIOBJ    previous_obj;
	
    if ( npts > 0 )
    {
        points = HeapAlloc( wingdi_heap, HEAP_ZERO_MEMORY,
					        (size_t) npts * sizeof ( POINT ) );
        if ( points != NULL )
        {
            for ( i = 0; i < npts; i++ )
            {
                points[i].x = (LONG) ( xa[i] / dev->xscale );
                points[i].y = (LONG) ( dev->height - ( ya[i] / dev->yscale ) );
            }
			
            previous_obj = SelectObject( dev->hdc, dev->pen );
            Polyline( dev->hdc, points, npts );
            SelectObject( dev->hdc, previous_obj );
            HeapFree( wingdi_heap, 0, points );    
        }
        else
        {
            plexit( "Could not allocate memory to \"plD_polyline_wingdi\"\n" );
        }
    }
}

//--------------------------------------------------------------------------
// plD_fill_polygon_wingdi()
//
// Fill polygon described in points pls->dev_x[] and pls->dev_y[].
//--------------------------------------------------------------------------

static void
plD_fill_polygon_wingdi( PLStream *pls )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;
    int        i;
    POINT      *points = NULL;
	HGDIOBJ    previous_brush, previous_pen;
    HPEN       hpen;
    HBRUSH     fillbrush;  // brush used for fills

    if ( pls->dev_npts > 0 )
    {
        points = HeapAlloc( wingdi_heap, HEAP_ZERO_MEMORY, 
						    (size_t) pls->dev_npts * sizeof ( POINT ) );

        if ( points == NULL )
            plexit( "Could not allocate memory to \"plD_fill_polygon_wingdi\"\n" );

        for ( i = 0; i < pls->dev_npts; i++ )
        {
            points[i].x = (PLINT) ( pls->dev_x[i] / dev->xscale );
            points[i].y = (PLINT) ( dev->height - ( pls->dev_y[i] / dev->yscale ) );
        }

		// Create a brush for the fill and a pen for the border
        fillbrush = CreateSolidBrush( dev->color );
        hpen      = CreatePen( PS_SOLID, 1, dev->color );
        previous_brush = SelectObject( dev->hdc, fillbrush );
        previous_pen   = SelectObject( dev->hdc, hpen );
		
		// Draw the filled polygon
        Polygon( dev->hdc, points, pls->dev_npts );
		
		// Restore the previous objects and delete
        SelectObject( dev->hdc, previous_brush );
        DeleteObject( fillbrush );
        SelectObject( dev->hdc, previous_pen );
        DeleteObject( hpen );
		
        HeapFree( wingdi_heap, 0, points );
    }
}

void
plD_eop_wingdi( PLStream *pls )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;

    pldebug( "wingdi", "End of the page\n" );
	
	// Save a bitmap of the current screen to help with redraws
    CopySCRtoBMP( pls );

	// Set the cursor to normal to indicate that the window is not busy
    NormalCursor( dev );

    if ( !pls->nopause )
    {
		// Wait for the user to indicate the next action
		wait_for_user_input( pls );
    }
}

//--------------------------------------------------------------------------
//  Beginning of the new page
//--------------------------------------------------------------------------
void
plD_bop_wingdi( PLStream *pls )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;
    pldebug( "wingdi", "Start of Page\n" );

	// Indicate that the device is actively drawing
	dev->state = DEV_DRAWING;

    //
    //  Turn the cursor to a busy sign, clear the page by "invalidating" it
    //  then reset the colors and pen width
    //
	
	// Change the cursor to indicate the program is busy
    BusyCursor( dev );
	
    RedrawWindow( dev->hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ERASENOW );

    plD_state_wingdi( pls, PLSTATE_COLOR0 );
}

void
plD_tidy_wingdi( PLStream *pls )
{
    struct wingdi_Dev *dev = NULL;

    pldebug( "wingdi", "plD_tidy_wingdi\n" );

    if ( pls->dev != NULL )
    {
        dev = (struct wingdi_Dev *) pls->dev;

        if ( dev->hdc != NULL )
            ReleaseDC( dev->hwnd, dev->hdc );
		
        if ( dev->bitmap != NULL )
            DeleteObject( dev->bitmap );

		DestroyWindow( dev->hwnd );
		
        free_mem( pls->dev );
		
		wingdi_module_cleanup();
    }
}

//--------------------------------------------------------------------------
// plD_state_wingdi()
//
// Handle change in PLStream state (color, pen width, fill attribute, etc).
//--------------------------------------------------------------------------

void
plD_state_wingdi( PLStream *pls, PLINT op )
{
    struct wingdi_Dev *dev = (struct wingdi_Dev *) pls->dev;

    switch ( op )
    {
    case PLSTATE_COLOR0:
    case PLSTATE_COLOR1:
        dev->color = RGB( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b );
        break;

    case PLSTATE_CMAP0:
    case PLSTATE_CMAP1:
        dev->color = RGB( pls->curcolor.r, pls->curcolor.g, pls->curcolor.b );
        break;
    }

    if ( dev->pen != NULL )
        DeleteObject( dev->pen );
    dev->pen = CreatePen( PS_SOLID, (int) pls->width, dev->color );
}

//--------------------------------------------------------------------------
// plD_esc_wingdi()
//
// Handle PLplot escapes
//--------------------------------------------------------------------------

void
plD_esc_wingdi( PLStream *pls, PLINT op, void *ptr )
{
    struct wingdi_Dev   *dev = (struct wingdi_Dev *) pls->dev;

    switch ( op )
    {
    case PLESC_GETC:
        GetCursorCmd( pls, (PLGraphicsIn *) ptr );
        break;

    case PLESC_FILL:
        plD_fill_polygon_wingdi( pls );
        break;

    case PLESC_DOUBLEBUFFERING:
        break;

    case PLESC_XORMOD:
        if ( *(PLINT *) ( ptr ) == 0 )
            SetROP2( dev->hdc, R2_COPYPEN );
        else
            SetROP2( dev->hdc, R2_XORPEN );
        break;
    }
}

#else
int
pldummy_wingdi()
{
    return ( 0 );
}

#endif                          // PLD_wingdidev
