/* $Id$

	PLplot Null device driver.
*/
#include "plplot/plDevs.h"

#ifdef PLD_ntk

#include "plplot/plplotP.h"
#include "plplot/drivers.h"

#include <tk.h>

void plD_dispatch_init_ntk	( PLDispatchTable *pdt );

void plD_init_ntk		(PLStream *);
void plD_line_ntk		(PLStream *, short, short, short, short);
void plD_polyline_ntk		(PLStream *, short *, short *, PLINT);
void plD_eop_ntk		(PLStream *);
void plD_bop_ntk		(PLStream *);
void plD_tidy_ntk		(PLStream *);
void plD_state_ntk		(PLStream *, PLINT);
void plD_esc_ntk		(PLStream *, PLINT, void *);

void plD_dispatch_init_ntk( PLDispatchTable *pdt )
{
    pdt->pl_MenuStr  = "New Tk device";
    pdt->pl_DevName  = "ntk";
    pdt->pl_type     = plDevType_Interactive;
    pdt->pl_seq      = 43;
    pdt->pl_init     = (plD_init_fp)     plD_init_ntk;
    pdt->pl_line     = (plD_line_fp)     plD_line_ntk;
    pdt->pl_polyline = (plD_polyline_fp) plD_polyline_ntk;
    pdt->pl_eop      = (plD_eop_fp)      plD_eop_ntk;
    pdt->pl_bop      = (plD_bop_fp)      plD_bop_ntk;
    pdt->pl_tidy     = (plD_tidy_fp)     plD_tidy_ntk;
    pdt->pl_state    = (plD_state_fp)    plD_state_ntk;
    pdt->pl_esc      = (plD_esc_fp)      plD_esc_ntk;
}

#define XPIXELS 800
#define YPIXELS 600

static PLFLT scale = 10.0; /* Tk canvas units are pixels, giving corse curves */
static PLFLT ppm; /* pixels per mm */

static Tcl_Interp *interp = NULL;
static Tk_Window mainw;
static char *name = NULL;
static char curcolor[80];
static char cmd[10000];
static int ccanv = 0;
static char base[80];
static char dash[80];

static int local = 1;
static char rem_interp[80];

static PLINT xmin = 0;
static PLINT xmax = XPIXELS;
static PLINT ymin = 0;
static PLINT ymax = YPIXELS;

void
tclevent()
{
  while(Tcl_DoOneEvent(TCL_ALL_EVENTS | TCL_DONT_WAIT));
}

void
tk_cmd(char *cmd)
{
  static char ocmd[10000];

  if (local)
    Tcl_Eval(interp, cmd);
  else {
    sprintf(ocmd, "send -async %s {%s}", rem_interp, cmd);
    Tcl_Eval(interp, ocmd);
  }
}

void
create_canvas(PLStream *pls)
{
  ccanv++;

  sprintf(cmd, "canvas $plf.f2.c%d -width %d -height %d -background #%02x%02x%02x -xscrollcommand \"$plf.f2.hscroll set\" -yscrollcommand \"$plf.f2.vscroll set\" -scrollregion {0 0 %d %d}", ccanv, XPIXELS, YPIXELS, pls->cmap0[0].r, pls->cmap0[0].g , pls->cmap0[0].b, XPIXELS+20, YPIXELS+20);
  tk_cmd(cmd);

  sprintf(cmd, "$plf.f1.mb.menu add command -label \"Page %d\" -command {$plf.f2.hscroll configure -command \"$plf.f2.c%d xview\"; $plf.f2.vscroll configure -command \"$plf.f2.c%d yview\"; set dname \"Page %d\"; pack forget $ocanvas; set ocanvas $plf.f2.c%d; pack $ocanvas -fill both -expand 1}", ccanv, ccanv, ccanv, ccanv, ccanv);
  tk_cmd(cmd);
  
  sprintf(cmd, "set item%d 0", ccanv);
  tk_cmd(cmd);

  /* FIXME -- the zoom needs to set scrollbars x/yview */
  /* Shift B1, zooms in */
  sprintf(cmd, "bind $plf.f2.c%d <Shift-Button-1> {pack $plf.f2.hscroll -side bottom -fill x; pack $plf.f2.vscroll -side right -fill y;incr item%d; set zx%d($item%d) %%x; set zy%d($item%d) %%y; %%W scale all %%x %%y 1.6 1.6; set t [lindex [$plf.f2.c%d configure -scrollregion] 4]; set tx [expr [lindex $t 0] - $zx%d($item%d)]; set ty [expr [lindex $t 1] - $zy%d($item%d)]; $plf.f2.c%d configure -scrollregion \"$tx $ty [expr ([lindex $t 2] + [lindex $t 0])* 1.6 - $tx] [expr ([lindex $t 3] + [lindex $t 1]) * 1.6 - $ty] \"}", ccanv, ccanv, ccanv, ccanv,ccanv, ccanv, ccanv, ccanv, ccanv, ccanv, ccanv, ccanv);
  tk_cmd(cmd);
  
  /* Shift B3, zooms out */
  sprintf(cmd,"bind $plf.f2.c%d <Shift-Button-3> {if {$item%d != 0} {%%W scale all $zx%d($item%d) $zy%d($item%d) 0.625 0.625; set item%d [expr $item%d - 1]} {pack forget $plf.f2.hscroll $plf.f2.vscroll}}", ccanv, ccanv, ccanv, ccanv, ccanv, ccanv, ccanv, ccanv);
  tk_cmd(cmd);

  /* Shift B2, resets */
  sprintf(cmd,"bind $plf.f2.c%d <Shift-Button-2> {while {$item%d != 0} {%%W scale all $zx%d($item%d) $zy%d($item%d) 0.625 0.625; set item%d [expr $item%d - 1]}; pack forget $plf.f2.hscroll $plf.f2.vscroll}", ccanv, ccanv, ccanv, ccanv, ccanv, ccanv, ccanv, ccanv);
  tk_cmd(cmd);

  /* B2-Motion, pan FIXME--not working */
  if(0) {
  sprintf(cmd, "bind $plf.f2.c%d <B2> \"$plf.f2.c%d scan mark %%x %%y\"", ccanv, ccanv);
  tk_cmd(cmd);

  sprintf(cmd, "bind $plf.f2.c%d <B2-Motion> \"$plf.f2.c%d scan dragto %%x %%y\"", ccanv, ccanv);
  tk_cmd(cmd);
  }

  /* Control B1, identify and edit object */
  sprintf(cmd, "bind  $plf.f2.c%d <Control-Button-1> {set w $plf.f2.c%d; set xx [ expr [winfo pointerx .] - [winfo rootx $w]];set yy [ expr [winfo pointery .] - [winfo rooty $w]]; set item [$w find closest $xx $yy]; $w move $item 20 20; puts \"item=$item [$w itemconfigure $item]\"; after 500 \"$w move $item -20 -20\"}", ccanv, ccanv);
  tk_cmd(cmd);

  sprintf(cmd, "$plf.f1.mb.menu invoke %d", ccanv-1);
  tk_cmd(cmd);
}

/*--------------------------------------------------------------------------*\
 * plD_init_ntk()
 *
 * Initialize device (terminal).
\*--------------------------------------------------------------------------*/

void
plD_init_ntk(PLStream *pls)
{
    pls->dev_fill0 = 1;	    /* Handle solid fills */
    pls->dev_fill1 = 1;     /* Dont handle pattern fills */
    pls->color = 1;         /* Is a color device */
    pls->dev_dash = 1;	    /* Handle dashed lines */
    pls->plbuf_write = 1;   /* Use plot buffer */

    strcpy(curcolor, "black");
    strcpy(base,".plf");

    if (pls->server_name != NULL) {
      local = 0;
      strcpy(rem_interp, pls->server_name);
    }

    interp = Tcl_CreateInterp();
  
    if (Tcl_Init(interp) != TCL_OK) 
      plexit("Unable to initialize Tcl.");

    if (Tk_Init( interp ))
      plexit("Unable to initialize Tk.");
  
    mainw = Tk_MainWindow(interp);
    Tcl_Eval(interp, "rename exec {}");

    Tcl_Eval(interp, "tk appname PLplot_ntk");

    if (!local) {
      Tcl_Eval(interp, "wm withdraw .");

      sprintf(cmd, "send %s \"set client [tk appname]; wm deiconify .\"", rem_interp);
      if (Tcl_Eval(interp, cmd) != TCL_OK) {
	fprintf(stderr,"%s\n", interp->result);
	plexit("No such tk server.");
      }
    }

    sprintf(cmd, "set ocanvas .; set plf %s", base);
    tk_cmd(cmd);
      
    tk_cmd("frame $plf; pack $plf -fill both -expand 1");

    tk_cmd("frame $plf.f1; frame $plf.f2; pack $plf.f1 -fill x; pack $plf.f2 -fill both -expand 1");

    tk_cmd("scrollbar $plf.f2.hscroll -orient horiz; scrollbar $plf.f2.vscroll");

    // tk_cmd("pack $plf.f2.hscroll -side bottom -fill x; pack $plf.f2.vscroll -side right -fill y");

    tk_cmd("menubutton $plf.f1.mb -text \"Page 1\" -textvariable dname -relief raised -indicatoron 1 -menu $plf.f1.mb.menu; menu $plf.f1.mb.menu -tearoff 0; pack $plf.f1.mb -side left");

    if (local)
      tk_cmd("button $plf.f1.quit -text Quit -command exit; pack $plf.f1.quit -side right");
    else
      tk_cmd("button $plf.f1.quit -text Quit -command {send -async $client exit; destroy $plf; wm withdraw .}; pack $plf.f1.quit -side right");


    /* FIXME: I just discovered that Tcl_Eval is slower than Tcl_EvalObj. Fix it global-wide, `man Tcl_Eval' */

    /* Set up device parameters */

    Tcl_Eval(interp, "tk scaling"); /* pixels per mm */
    ppm = (PLFLT) atof(interp->result)/(25.4/72.);
    plP_setpxl(ppm, ppm);
    plP_setphy(xmin, xmax*scale, ymin, ymax*scale);
}

void
plD_line_ntk(PLStream *pls, short x1a, short y1a, short x2a, short y2a)
{
  sprintf(cmd, "$plf.f2.c%d create line %.1f %.1f %.1f %.1f -fill %s",
	  ccanv, x1a/scale, ymax-y1a/scale, x2a/scale, ymax-y2a/scale, curcolor);
  tk_cmd(cmd);
}

void
plD_polyline_ntk(PLStream *pls, short *xa, short *ya, PLINT npts)
{
  PLINT i, j;

  j = sprintf(cmd,"$plf.f2.c%d create line ", ccanv);
  for (i = 0; i < npts; i++)
    j += sprintf(&cmd[j], "%.1f %.1f ", xa[i]/scale, ymax-ya[i]/scale);
  j += sprintf(&cmd[j]," -fill %s", curcolor);
  if (dash[0] == '-')
    j += sprintf(&cmd[j]," %s", dash);
  
  tk_cmd(cmd);
}

void
plD_eop_ntk(PLStream *pls)
{
  tk_cmd("update");
}

void
plD_bop_ntk(PLStream *pls)
{
  create_canvas(pls);
}

void
plD_tidy_ntk(PLStream *pls)
{
  while(Tcl_DoOneEvent(TCL_ALL_EVENTS));
  //tk_cmd("update");
}

void 
plD_state_ntk(PLStream *pls, PLINT op)
{
    switch (op) {
    case PLSTATE_COLOR0:
    case PLSTATE_COLOR1:
      sprintf(curcolor, "#%02x%02x%02x",
	      pls->curcolor.r, pls->curcolor.g , pls->curcolor.b);
      break;
    }
}

void
plD_esc_ntk(PLStream *pls, PLINT op, void *ptr)
{
  PLINT i,j;
  short *xa, *ya;
  Pixmap bitmap;
  static unsigned char bit_pat[] = {
   0x24, 0x01, 0x92, 0x00, 0x49, 0x00, 0x24, 0x00, 0x12, 0x00, 0x09, 0x00,
   0x04, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff};
  
  switch (op) {

  case PLESC_DASH:
    xa = (short *) malloc(sizeof(short) * pls->dev_npts);
    ya = (short *) malloc(sizeof(short) * pls->dev_npts);
    for (i = 0; i < pls->dev_npts; i++) {
      xa[i] = pls->dev_x[i];
      ya[i] = pls->dev_y[i];
    }

    j = sprintf(dash, "-dash {");
    for (i = 0; i < pls->nms; i++)
      j += sprintf(&dash[j]," %d %d",
		   (int) ceil(pls->mark[i]/1e3 * ppm),
		   (int) ceil(pls->space[i]/1e3 * ppm));
    sprintf(&dash[j], "}");
    plD_polyline_ntk(pls, xa, ya, pls->dev_npts);
    free(xa); free(ya);
    dash[0] = 0;
  break;

  case PLESC_FLUSH:
    tk_cmd("update");
    break;

  case PLESC_FILL:
    if (pls->patt != 0) {
      /* this is a hack! The real solution is in the if(0) bellow */
      pls->xpmm *= scale;
      pls->ypmm *= scale;
      plfill_soft( pls->dev_x, pls->dev_y, pls->dev_npts);
      pls->xpmm /= scale;
      pls->ypmm /= scale;
    } else {
      j = sprintf(cmd, "$plf.f2.c%d create polygon ", ccanv);
      for (i = 0; i < pls->dev_npts; i++)
	j += sprintf(&cmd[j], "%.1f %.1f ", pls->dev_x[i]/scale,
		     ymax-pls->dev_y[i]/scale);
      j += sprintf(&cmd[j]," -fill %s", curcolor);
      tk_cmd(cmd);
    }
    
    if (0) {
      if (pls->patt != 0) {
	Tk_DefineBitmap(interp, Tk_GetUid("foo"), bit_pat, 16, 16);
	bitmap = Tk_GetBitmap(interp, mainw, Tk_GetUid("patt"));
      }
      j = sprintf(cmd, "$plf.f2.c%d create polygon ", ccanv);
      for (i = 0; i < pls->dev_npts; i++)
	j += sprintf(&cmd[j], "%.1f %.1f ", pls->dev_x[i]/scale,
		     ymax-pls->dev_y[i]/scale);
      j += sprintf(&cmd[j]," -fill %s", curcolor);
      if (pls->patt != 0)
	sprintf(&cmd[j], " -stipple patt -outline black");

      tk_cmd(cmd);
      //Tk_FreeBitmap(display, bitmap)
    }
    break;
  }
}

#else
int 
pldummy_ntk()
{
    return 0;
}

#endif				/* PLD_ntkdev */
