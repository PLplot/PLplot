/*
  pltk.c
  Geoffrey Furnish
  18 April 1994

  This file implements the Tcl/Tk extension initialization command,
  for making extended wishes incorporating support for embedded
  plframes.

  Just stick this in your tkAppInit.c file, in section for
  initializing Tk extensions.

    if (Pltk_Init(interp) == TCL_ERROR) {
        return TCL_ERROR;
    }

  $Id$

  $Log$
  Revision 1.1.2.1  2001/04/19 12:31:45  rlaboiss
  First merge against MAIN

  Revision 1.1  1994/04/18 19:23:21  furnish
  Added new function Pltk_Init to allow users to easily construct PLPLOT
  enhanced application specific extended wishen.  Works great, needs advert.


  */

#define TK
#include "plplot.h"
#undef TK

int Pltk_Init( Tcl_Interp *interp )
{
    Tk_Window main;

    main = Tk_MainWindow(interp);

    Tcl_CreateCommand(interp, "plframe", plFrameCmd,
                      (ClientData) main, (void (*)(ClientData)) NULL);

    return TCL_OK;
}
