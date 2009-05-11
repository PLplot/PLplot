/* Converted to D from plplot_d.h by htod */
module plplot;

private import std.string;


// improved D interface

/* Prints out "text" at specified position relative to viewport */
void plmtex(string side, PLFLT disp, PLFLT pos, PLFLT just, string text)
{
  c_plmtex(toStringz(side), disp, pos, just, toStringz(text));
}

/* Prints out "text" at world cooordinate (x,y). */
void plptex(PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, string text)
{
  c_plptex(x, y, dx, dy, just, toStringz(text));
}

/* Process options list using current options info. */
int plparseopts(char[][] args, PLINT mode)
{
  char*[] c_args = new char*[args.length];
  foreach( size_t i, char[] arg; args )
    c_args[i] = toStringz(arg);
  int argc = c_args.length;
  return c_plparseopts( &argc, cast(char**)c_args, mode );
}

/* simple arrow plotter. */
//void  c_plvect(PLFLT **u, PLFLT **v, PLINT nx, PLINT ny, PLFLT scale, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);
//void  c_plsvect(PLFLT *arrowx, PLFLT *arrowy, PLINT npts, PLBOOL fill);

/* This functions similarly to plbox() except that the origin of the axes */
/* is placed at the user-specified point (x0, y0). */
//void  c_plaxes(PLFLT x0, PLFLT y0, char *xopt, PLFLT xtick, PLINT nxsub, char *yopt, PLFLT ytick, PLINT nysub);

/* Plot a histogram using x to store data values and y to store frequencies */
void plbin(PLFLT[] x, PLFLT[] y, PLINT opt)
{
  PLINT nbin = x.length;
  assert(nbin==y.length, "Arrays must be of same length!");
  c_plbin(nbin, cast(PLFLT*)x, cast(PLFLT*)y, opt); 
}

/* This draws a box around the current viewport. */
//void  c_plbox(char *xopt, PLFLT xtick, PLINT nxsub, char *yopt, PLFLT ytick, PLINT nysub);

/* This is the 3-d analogue of plbox(). */
//void  c_plbox3(char *xopt, char *xlabel, PLFLT xtick, PLINT nsubx, char *yopt, char *ylabel, PLFLT ytick, PLINT nsuby, char *zopt, char *zlabel, PLFLT ztick, PLINT nsubz);

/* Draws a contour plot from data in f(nx,ny).  Is just a front-end to
 * plfcont, with a particular choice for f2eval and f2eval_data.
 */
//void  c_plcont(PLFLT **f, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

/* Draws a contour plot using the function evaluator f2eval and data stored
 * by way of the f2eval_data pointer.  This allows arbitrary organizations
 * of 2d array data to be used.
 */
//void  plfcont(PLFLT  function(PLINT , PLINT , PLPointer )f2eval, PLPointer f2eval_data, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

/* Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)) */
//void  c_plerrx(PLINT n, PLFLT *xmin, PLFLT *xmax, PLFLT *y);

/* Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)) */
//void  c_plerry(PLINT n, PLFLT *x, PLFLT *ymin, PLFLT *ymax);

/* Pattern fills the polygon bounded by the input points. */
//void  c_plfill(PLINT n, PLFLT *x, PLFLT *y);

/* Pattern fills the 3d polygon bounded by the input points. */
//void  c_plfill3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z);

/* Get the current device (keyword) name */
//void  c_plgdev(char *p_dev);

/* Get the (current) output file name.  Must be preallocated to >80 bytes */
//void  c_plgfnam(char *fnam);

/* grid irregularly sampled data */
//void  c_plgriddata(PLFLT *x, PLFLT *y, PLFLT *z, PLINT npts, PLFLT *xg, PLINT nptsx, PLFLT *yg, PLINT nptsy, PLFLT **zg, PLINT type, PLFLT data);

/* Get the current library version number */
//void  c_plgver(char *p_ver);

/* Draws a histogram of n values of a variable in array data[0..n-1] */
//void  c_plhist(PLINT n, PLFLT *data, PLFLT datmin, PLFLT datmax, PLINT nbin, PLINT opt);

/* Simple routine for labelling graphs. */
//void  c_pllab(char *xlabel, char *ylabel, char *tlabel);

/* Draws line segments connecting a series of points. */
void plline(PLFLT[] x, PLFLT[] y)
{
  PLINT n = x.length;
  assert(n==y.length, "Arrays must be of same length!");
  c_plline(n, cast(PLFLT*)x, cast(PLFLT*)y); 
}

/* Draws a line in 3 space.  */
//void  c_plline3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z);

/* plot continental outline in world coordinates */
//void  c_plmap(void  function(PLINT , PLFLT *, PLFLT *)mapform, char *type, PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);

/* Plot the latitudes and longitudes on the background. */
//void  c_plmeridians(void  function(PLINT , PLFLT *, PLFLT *)mapform, PLFLT dlong, PLFLT dlat, PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);

/* Plots a mesh representation of the function z[x][y]. */
//void  c_plmesh(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt);

/* Plots a mesh representation of the function z[x][y] with contour */
//void  c_plmeshc(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel);

/* Prints out "text" at specified position relative to viewport (3D)*/
//void  c_plmtex3(char *side, PLFLT disp, PLFLT pos, PLFLT just, char *text);
	 
/* Plots a 3-d representation of the function z[x][y]. */
//void  c_plot3d(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLBOOL side);

/* Plots a 3-d representation of the function z[x][y] with contour. */
//void  c_plot3dc(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel);

/* Plots a 3-d representation of the function z[x][y] with contour and
 * y index limits. */
//void  c_plot3dcl(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel, PLINT ixstart, PLINT ixn, PLINT *indexymin, PLINT *indexymax);

/* Set fill pattern directly. */
//void  c_plpat(PLINT nlin, PLINT *inc, PLINT *del);

/* Plots array y against x for n points using ASCII code "code".*/
//void  c_plpoin(PLINT n, PLFLT *x, PLFLT *y, PLINT code);

/* Draws a series of points in 3 space. */
//void  c_plpoin3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT code);

/* Draws a polygon in 3 space.  */
//void  c_plpoly3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLBOOL *draw, PLBOOL ifcc);

/* Prints out "text" at world cooordinate (x,y,z). */
//void  c_plptex3(PLFLT wx, PLFLT wy, PLFLT wz, PLFLT dx, PLFLT dy, PLFLT dz, PLFLT sx, PLFLT sy, PLFLT sz, PLFLT just, char *text);

/* Set color map 0 colors by 8 bit RGB values */
//void  c_plscmap0(PLINT *r, PLINT *g, PLINT *b, PLINT ncol0);

/* Set color map 0 colors by 8 bit RGB values and alpha values */
//void  c_plscmap0a(PLINT *r, PLINT *g, PLINT *b, PLFLT *a, PLINT ncol0);

/* Set color map 1 colors by 8 bit RGB values */
//void  c_plscmap1(PLINT *r, PLINT *g, PLINT *b, PLINT ncol1);

/* Set color map 1 colors by 8 bit RGB and alpha values */
//void  c_plscmap1a(PLINT *r, PLINT *g, PLINT *b, PLFLT *a, PLINT ncol1);

/* Set color map 1 colors using a piece-wise linear relationship between */
/* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space. */
//void  c_plscmap1l(PLBOOL itype, PLINT npts, PLFLT *intensity, PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLBOOL *rev);

/* Set color map 1 colors using a piece-wise linear relationship between */
/* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space. */
/* Will also linear interpolate alpha values. */
//void  c_plscmap1la(PLBOOL itype, PLINT npts, PLFLT *intensity, PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLFLT *a, PLBOOL *rev);

/* Set the device (keyword) name */
void plsdev(string devname)
{
  c_plsdev(toStringz(devname));
}

/* Set the output file name. */
void plsfnam(string fnam)
{
  c_plsfnam(toStringz(fnam));
}

/* Shade region. */
//void  c_plshade(PLFLT **a, PLINT nx, PLINT ny, PLINT  function(PLFLT , PLFLT )defined, PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void  function(PLINT , PLFLT *, PLFLT *)fill, PLBOOL rectangular, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);
//void  c_plshade1(PLFLT *a, PLINT nx, PLINT ny, PLINT  function(PLFLT , PLFLT )defined, PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void  function(PLINT , PLFLT *, PLFLT *)fill, PLBOOL rectangular, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);
//void  c_plshades(PLFLT **a, PLINT nx, PLINT ny, PLINT  function(PLFLT , PLFLT )defined, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT *clevel, PLINT nlevel, PLINT fill_width, PLINT cont_color, PLINT cont_width, void  function(PLINT , PLFLT *, PLFLT *)fill, PLBOOL rectangular, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);
//void  plfshade(PLFLT  function(PLINT , PLINT , PLPointer )f2eval, PLPointer f2eval_data, PLFLT  function(PLINT , PLINT , PLPointer )c2eval, PLPointer c2eval_data, PLINT nx, PLINT ny, PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void  function(PLINT , PLFLT *, PLFLT *)fill, PLBOOL rectangular, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

/* Initialize PLplot, passing the device name and windows/page settings. */
//void  c_plstart(char *devname, PLINT nx, PLINT ny);

/* Create 1d stripchart */
//void  c_plstripc(PLINT *id, char *xspec, char *yspec, PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax, PLFLT xlpos, PLFLT ylpos, PLBOOL y_ascl, PLBOOL acc, PLINT colbox, PLINT collab, PLINT *colline, PLINT *styline, char **legline, char *labx, char *laby, char *labtop);

/* plots a 2d image (or a matrix too large for plshade() ) */
//void  c_plimagefr(PLFLT **idata, PLINT nx, PLINT ny, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax, PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax, PLFLT valuemin, PLFLT valuemax);

/* plots a 2d image (or a matrix too large for plshade() ) - colors
   automatically scaled */
//void  c_plimage(PLFLT **idata, PLINT nx, PLINT ny, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax, PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax);

/* Set up a new line style */
//void  c_plstyl(PLINT nms, PLINT *mark, PLINT *space);

/* Plots the 3d surface representation of the function z[x][y]. */
//void  c_plsurf3d(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel);

/* Plots the 3d surface representation of the function z[x][y] with y
 * index limits. */
//void  c_plsurf3dl(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel, PLINT ixstart, PLINT ixn, PLINT *indexymin, PLINT *indexymax);

/* Plots array y against x for n points using Hershey symbol "code" */
//void  c_plsym(PLINT n, PLFLT *x, PLFLT *y, PLINT code);

/* Set the format for date / time labels */
//void  c_pltimefmt(char *fmt);

/*--------------------------------------------------------------------------*
 *		Functions for use from C or C++ only
\*--------------------------------------------------------------------------*/

/* Returns a list of file-oriented device names and their menu strings */
//void  plgFileDevs(char ***p_menustr, char ***p_devname, int *p_ndev);

/* Returns a list of all device names and their menu strings */
//void  plgDevs(char ***p_menustr, char ***p_devname, int *p_ndev);

/* Set the function pointer for the keyboard event handler */
//void  plsKeyEH(void  function(PLGraphicsIn *, void *, int *)KeyEH, void *KeyEH_data);

/* Set the function pointer for the (mouse) button event handler */
//void  plsButtonEH(void  function(PLGraphicsIn *, void *, int *)ButtonEH, void *ButtonEH_data);

/* Sets an optional user bop handler */
//void  plsbopH(void  function(void *, int *)handler, void *handler_data);

/* Sets an optional user eop handler */
//void  plseopH(void  function(void *, int *)handler, void *handler_data);

/* Set the variables to be used for storing error info */
//void  plsError(PLINT *errcode, char *errmsg);

/* Sets an optional user exit handler. */
//void  plsexit(int  function(char *)handler);

/* Sets an optional user abort handler. */
//void  plsabort(void  function(char *)handler);

/* Transformation routines */

/* Identity transformation. */
//void  pltr0(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Does linear interpolation from singly dimensioned coord arrays. */
//void  pltr1(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (column dominant, as per normal C 2d arrays). */
//void  pltr2(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Just like pltr2() but uses pointer arithmetic to get coordinates from */
/* 2d grid tables.  */
//void  pltr2p(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Identity transformation for plots from Fortran. */
//void  pltr0f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (row dominant, i.e. Fortran ordering). */
//void  pltr2f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

/* Function evaluators */

/* Does a lookup from a 2d function array.  Array is of type (PLFLT **), */
/* and is column dominant (normal C ordering). */
//PLFLT  plf2eval2(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is column dominant (normal C ordering). */
//PLFLT  plf2eval(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is row dominant (Fortran ordering). */
//PLFLT  plf2evalr(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Command line parsing utilities */

/* Merge user option table into internal info structure. */
//int  plMergeOpts(PLOptionTable *options, char *name, char **notes);

/* Set the strings used in usage and syntax messages. */
//void  plSetUsage(char *program_string, char *usage_string);

/* Process input strings, treating them as an option and argument pair. */
/* The first is for the external API, the second the work routine declared
   here for backward compatibilty. */
//int  c_plsetopt(char *opt, char *optarg);

/* Miscellaneous */

/* Get the escape character for text strings. */
//void  plgesc(char *p_esc);

/* Front-end to driver escape function. */
//void  pl_cmd(PLINT op, void *ptr);

/* Return full pathname for given file if executable */
//int  plFindName(char *p);

/* Looks for the specified executable file according to usual search path. */
//char * plFindCommand(char *fn);

/* Gets search name for file by concatenating the dir, subdir, and file */
/* name, allocating memory as needed.  */
//void  plGetName(char *dir, char *subdir, char *filename, char **filespec);

/* Prompts human to input an integer in response to given message. */
//PLINT  plGetInt(char *s);

/* Prompts human to input a float in response to given message. */
//PLFLT  plGetFlt(char *s);

/* Find the maximum and minimum of a 2d matrix allocated with plAllc2dGrid(). */
//void  plMinMax2dGrid(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmax, PLFLT *fmin);

/* Wait for graphics input event and translate to world coordinates */
//int  plGetCursor(PLGraphicsIn *gin);

/* Translates relative device coordinates to world coordinates.  */
//int  plTranslateCursor(PLGraphicsIn *gin);



extern (C):

alias double PLFLT;

/* This is apparently portable if stdint.h exists. */
/* A reasonable back-up in case stdint.h does not exist on the platform. */
alias uint PLUNICODE;
alias int PLINT;

/* For identifying logical (boolean) arguments */
alias PLINT PLBOOL;

/* For passing user data, as with X's XtPointer */
alias void* PLPointer;

/*--------------------------------------------------------------------------*
 * Complex data types and other good stuff
\*--------------------------------------------------------------------------*/

/* Switches for escape function call. */
/* Some of these are obsolete but are retained in order to process
   old metafiles. */

const PLESC_SET_RGB = 1;
const PLESC_ALLOC_NCOL = 2;
const PLESC_SET_LPB = 3;
const PLESC_EXPOSE = 4;
const PLESC_RESIZE = 5;
const PLESC_REDRAW = 6;
const PLESC_TEXT = 7;
const PLESC_GRAPH = 8;
const PLESC_FILL = 9;
const PLESC_DI = 10;
const PLESC_FLUSH = 11;
const PLESC_EH = 12;
const PLESC_GETC = 13;
const PLESC_SWIN = 14;
const PLESC_DOUBLEBUFFERING = 15;
const PLESC_XORMOD = 16;
const PLESC_SET_COMPRESSION = 17;
const PLESC_CLEAR = 18;
const PLESC_DASH = 19;
const PLESC_HAS_TEXT = 20;
const PLESC_IMAGE = 21;
const PLESC_IMAGEOPS = 22;
const PLESC_PL2DEVCOL = 23;
const PLESC_DEV2PLCOL = 24;
const PLESC_SETBGFG = 25;
const PLESC_DEVINIT = 26;

/* image operations */
const ZEROW2B = 1;
const ZEROW2D = 2;
const ONEW2B = 3;
const ONEW2D = 4;

/* Window parameter tags */
const PLSWIN_DEVICE = 1;
const PLSWIN_WORLD = 2;

/* PLplot Option table & support constants */

/* Option-specific settings */
const PL_OPT_ENABLED = 0x0001;
const PL_OPT_ARG = 0x0002;
const PL_OPT_NODELETE = 0x0004;
const PL_OPT_INVISIBLE = 0x0008;
const PL_OPT_DISABLED = 0x0010;

/* Option-processing settings -- mutually exclusive */
const PL_OPT_FUNC = 0x0100;
const PL_OPT_BOOL = 0x0200;
const PL_OPT_INT = 0x0400;
const PL_OPT_FLOAT = 0x0800;
const PL_OPT_STRING = 0x1000;

/* Global mode settings */
/* These override per-option settings */
const PL_PARSE_PARTIAL = 0x0000;
const PL_PARSE_FULL = 0x0001;
const PL_PARSE_QUIET = 0x0002;

/* processing */
const PL_PARSE_NODELETE = 0x0004;
const PL_PARSE_SHOWALL = 0x0008;
const PL_PARSE_OVERRIDE = 0x0010;
const PL_PARSE_NOPROGRAM = 0x0020;
const PL_PARSE_NODASH = 0x0040;
const PL_PARSE_SKIP = 0x0080;

/* FCI (font characterization integer) related constants. */
const PL_FCI_MARK = 0x80000000;
const PL_FCI_IMPOSSIBLE = 0x00000000;
const PL_FCI_HEXDIGIT_MASK = 0xf;
const PL_FCI_HEXPOWER_MASK = 0x7;

/* These define hexpower values corresponding to each font attribute. */
const PL_FCI_HEXPOWER_IMPOSSIBLE = 0xf;
const PL_FCI_FAMILY = 0x0;
const PL_FCI_STYLE = 0x1;

/* These are legal values for font family attribute */
const PL_FCI_WEIGHT = 0x2;
const PL_FCI_SANS = 0x0;
const PL_FCI_SERIF = 0x1;
const PL_FCI_MONO = 0x2;
const PL_FCI_SCRIPT = 0x3;

/* These are legal values for font style attribute */
const PL_FCI_SYMBOL = 0x4;
const PL_FCI_UPRIGHT = 0x0;
const PL_FCI_ITALIC = 0x1;

/* These are legal values for font weight attribute */
const PL_FCI_MEDIUM = 0x0;
const PL_FCI_BOLD = 0x1;
const PL_FCI_OBLIQUE = 0x2;

/* Obsolete names */

/* Option table definition */

struct _N1
{
    char *opt;
    int  function(char *, char *, void *)handler;
    void *client_data;
    void *var;
    int mode;
    char *syntax;
    char *desc;
}
alias _N1 PLOptionTable;

/* PLplot Graphics Input structure */


const PL_MAXKEY = 16;
struct _N2
{
    int type;
    uint state;
    uint keysym;
    uint button;
    PLINT subwindow;
    char [16]string;
    int pX;
    int pY;
    PLFLT dX;
    PLFLT dY;
    PLFLT wX;
    PLFLT wY;
}
alias _N2 PLGraphicsIn;

/* Structure for describing the plot window */


const PL_MAXWINDOWS = 64;
struct _N3
{
    PLFLT dxmi;
    PLFLT dxma;
    PLFLT dymi;
    PLFLT dyma;
    PLFLT wxmi;
    PLFLT wxma;
    PLFLT wymi;
    PLFLT wyma;
}
alias _N3 PLWindow;

/* Structure for doing display-oriented operations via escape commands */
/* May add other attributes in time */

struct _N4
{
    uint x;
    uint y;
    uint width;
    uint height;
}
alias _N4 PLDisplay;

/* Macro used (in some cases) to ignore value of argument */
/* I don't plan on changing the value so you can hard-code it */


/* See plcont.c for examples of the following */

/*
 * PLfGrid is for passing (as a pointer to the first element) an arbitrarily
 * dimensioned array.  The grid dimensions MUST be stored, with a maximum of 3
 * dimensions assumed for now.
 */

struct _N5
{
    PLFLT *f;
    PLINT nx;
    PLINT ny;
    PLINT nz;
}
alias _N5 PLfGrid;

/*
 * PLfGrid2 is for passing (as an array of pointers) a 2d function array.  The
 * grid dimensions are passed for possible bounds checking.
 */

struct _N6
{
    PLFLT **f;
    PLINT nx;
    PLINT ny;
}
alias _N6 PLfGrid2;

/*
 * NOTE: a PLfGrid3 is a good idea here but there is no way to exploit it yet
 * so I'll leave it out for now.
 */

/*
 * PLcGrid is for passing (as a pointer to the first element) arbitrarily
 * dimensioned coordinate transformation arrays.  The grid dimensions MUST be
 * stored, with a maximum of 3 dimensions assumed for now.
 */

struct _N7
{
    PLFLT *xg;
    PLFLT *yg;
    PLFLT *zg;
    PLINT nx;
    PLINT ny;
    PLINT nz;
}
alias _N7 PLcGrid;

/*
 * PLcGrid2 is for passing (as arrays of pointers) 2d coordinate
 * transformation arrays.  The grid dimensions are passed for possible bounds
 * checking.
 */

struct _N8
{
    PLFLT **xg;
    PLFLT **yg;
    PLFLT **zg;
    PLINT nx;
    PLINT ny;
}
alias _N8 PLcGrid2;

/*
 * NOTE: a PLcGrid3 is a good idea here but there is no way to exploit it yet
 * so I'll leave it out for now.
 */

/* PLColor is the usual way to pass an rgb color value. */

struct _N9
{
    ubyte r;
    ubyte g;
    ubyte b;
    PLFLT a;
    char *name;
}
alias _N9 PLColor;

/* PLControlPt is how cmap1 control points are represented. */

struct _N10
{
    PLFLT h;
    PLFLT l;
    PLFLT s;
    PLFLT p;
    PLFLT a;
    int rev;
}
alias _N10 PLControlPt;

/* A PLBufferingCB is a control block for interacting with devices
   that support double buffering. */

struct _N11
{
    PLINT cmd;
    PLINT result;
}
alias _N11 PLBufferingCB;

const PLESC_DOUBLEBUFFERING_ENABLE = 1;
const PLESC_DOUBLEBUFFERING_DISABLE = 2;

const PLESC_DOUBLEBUFFERING_QUERY = 3;

/*--------------------------------------------------------------------------* *		BRAINDEAD-ness
 *
 * Some systems allow the Fortran & C namespaces to clobber each other.
 * For PLplot to work from Fortran on these systems, we must name the the
 * externally callable C functions something other than their Fortran entry
 * names.  In order to make this as easy as possible for the casual user,
 * yet reversible to those who abhor my solution, I have done the
 * following:
 *
 *	The C-language bindings are actually different from those
 *	described in the manual.  Macros are used to convert the
 *	documented names to the names used in this package.  The
 *	user MUST include plplot.h in order to get the name
 *	redefinition correct.
 *
 * Sorry to have to resort to such an ugly kludge, but it is really the
 * best way to handle the situation at present.  If all available
 * compilers offer a way to correct this stupidity, then perhaps we can
 * eventually reverse it.
 *
 * If you feel like screaming at someone (I sure do), please
 * direct it at your nearest system vendor who has a braindead shared
 * C/Fortran namespace.  Some vendors do offer compiler switches that
 * change the object names, but then everybody who wants to use the
 * package must throw these same switches, leading to no end of trouble.
 *
 * Note that this definition should not cause any noticable effects except
 * when debugging PLplot calls, in which case you will need to remember
 * the real function names (same as before but with a 'c_' prepended).
 *
 * Also, to avoid macro conflicts, the BRAINDEAD part must not be expanded
 * in the stub routines.
 *
 * Aside: the reason why a shared Fortran/C namespace is deserving of the
 * BRAINDEAD characterization is that it completely precludes the the kind
 * of universal API that is attempted (more or less) with PLplot, without
 * Herculean efforts (e.g. remapping all of the C bindings by macros as
 * done here).  The vendors of such a scheme, in order to allow a SINGLE
 * type of argument to be passed transparently between C and Fortran,
 * namely, a pointer to a conformable data type, have slammed the door on
 * insertion of stub routines to handle the conversions needed for other
 * data types.  Intelligent linkers could solve this problem, but these are
 * not anywhere close to becoming universal.  So meanwhile, one must live
 * with either stub routines for the inevitable data conversions, or a
 * different API.  The former is what is used here, but is made far more
 * difficult in a braindead shared Fortran/C namespace.
\*--------------------------------------------------------------------------*/




alias c_pl_setcontlabelformat pl_setcontlabelformat;
alias c_pl_setcontlabelparam pl_setcontlabelparam;
alias c_pladv pladv;
alias c_plaxes plaxes;
//alias c_plbin plbin;
alias c_plbop plbop;
alias c_plbox plbox;
alias c_plbox3 plbox3;
alias c_plcalc_world plcalc_world;
alias c_plclear plclear;
alias c_plcol0 plcol0;
alias c_plcol1 plcol1;
alias c_plcont plcont;
alias c_plcpstrm plcpstrm;
alias c_plctime plctime;
alias c_plend plend;
alias c_plend1 plend1;
alias c_plenv plenv;
alias c_plenv0 plenv0;
alias c_pleop pleop;
alias c_plerrx plerrx;
alias c_plerry plerry;
alias c_plfamadv plfamadv;
alias c_plfill plfill;
alias c_plfill3 plfill3;
alias c_plflush plflush;
alias c_plfont plfont;
alias c_plfontld plfontld;
alias c_plgchr plgchr;
alias c_plgcol0 plgcol0;
alias c_plgcolbg plgcolbg;
alias c_plgcol0a plgcol0a;
alias c_plgcolbga plgcolbga;
alias c_plgcompression plgcompression;
alias c_plgdev plgdev;
alias c_plgdidev plgdidev;
alias c_plgdiori plgdiori;
alias c_plgdiplt plgdiplt;
alias c_plgfam plgfam;
alias c_plgfci plgfci;
alias c_plgfnam plgfnam;
alias c_plgfont plgfont;
alias c_plglevel plglevel;
alias c_plgpage plgpage;
alias c_plgra plgra;
alias c_plgriddata plgriddata;
alias c_plgspa plgspa;
alias c_plgstrm plgstrm;
alias c_plgver plgver;
alias c_plgvpd plgvpd;
alias c_plgvpw plgvpw;
alias c_plgxax plgxax;
alias c_plgyax plgyax;
alias c_plgzax plgzax;
alias c_plhist plhist;
alias c_plhls plhls;
alias c_plhlsrgb plhlsrgb;
alias c_plimage plimage;
alias c_plimagefr plimagefr;
alias c_plinit plinit;
alias c_pljoin pljoin;
alias c_pllab pllab;
alias c_pllightsource pllightsource;
//alias c_plline plline;
alias c_plline3 plline3;
alias c_pllsty pllsty;
alias c_plmap plmap;
alias c_plmeridians plmeridians;
alias c_plmesh plmesh;
alias c_plmeshc plmeshc;
alias c_plmkstrm plmkstrm;
// alias c_plmtex plmtex;
alias c_plmtex3 plmtex3;
alias c_plot3d plot3d;
alias c_plot3dc plot3dc;
alias c_plot3dcl plot3dcl;
// alias c_plparseopts plparseopts;
alias c_plpat plpat;
alias c_plpoin plpoin;
alias c_plpoin3 plpoin3;
alias c_plpoly3 plpoly3;
alias c_plprec plprec;
alias c_plpsty plpsty;
// alias c_plptex plptex;
alias c_plptex3 plptex3;
alias c_plreplot plreplot;
alias c_plrgb plrgb;
alias c_plrgb1 plrgb1;
alias c_plrgbhls plrgbhls;
alias c_plschr plschr;
alias c_plscmap0 plscmap0;
alias c_plscmap0a plscmap0a;
alias c_plscmap0n plscmap0n;
alias c_plscmap1 plscmap1;
alias c_plscmap1a plscmap1a;
alias c_plscmap1l plscmap1l;
alias c_plscmap1la plscmap1la;
alias c_plscmap1n plscmap1n;
alias c_plscol0 plscol0;
alias c_plscol0a plscol0a;
alias c_plscolbg plscolbg;
alias c_plscolbga plscolbga;
alias c_plscolor plscolor;
alias c_plscompression plscompression;
// alias c_plsdev plsdev;
alias c_plsdidev plsdidev;
alias c_plsdimap plsdimap;
alias c_plsdiori plsdiori;
alias c_plsdiplt plsdiplt;
alias c_plsdiplz plsdiplz;
alias c_plsesc plsesc;
alias c_plsetopt plsetopt;
alias c_plsfam plsfam;
alias c_plsfci plsfci;
// alias c_plsfnam plsfnam;
alias c_plsfont plsfont;
alias c_plshade plshade;
alias c_plshade1 plshade1;
alias c_plshades plshades;
alias c_plsmaj plsmaj;
alias c_plsmem plsmem;
alias c_plsmin plsmin;
alias c_plsori plsori;
alias c_plspage plspage;
alias c_plspause plspause;
alias c_plsstrm plsstrm;
alias c_plssub plssub;
alias c_plssym plssym;
alias c_plstar plstar;
alias c_plstart plstart;
alias c_plstripa plstripa;
alias c_plstripc plstripc;
alias c_plstripd plstripd;
alias c_plstyl plstyl;
alias c_plsurf3d plsurf3d;
alias c_plsurf3dl plsurf3dl;
alias c_plsvect plsvect;
alias c_plsvpa plsvpa;
alias c_plsxax plsxax;
alias c_plsyax plsyax;
alias c_plsym plsym;
alias c_plszax plszax;
alias c_pltext pltext;
alias c_pltimefmt pltimefmt;
alias c_plvasp plvasp;
alias c_plvect plvect;
alias c_plvpas plvpas;
alias c_plvpor plvpor;
alias c_plvsta plvsta;
alias c_plw3d plw3d;
alias c_plwid plwid;
alias c_plwind plwind;

alias c_plxormod plxormod;


/* Redefine some old function names for backward compatibility */


alias pleop plclr;
alias plbop plpage;
alias plcol0 plcol;
alias plfcont plcontf;
alias plAlloc2dGrid Alloc2dGrid;
alias plFree2dGrid Free2dGrid;
alias plMinMax2dGrid MinMax2dGrid;
alias plgvpd plP_gvpd;
alias plgvpw plP_gvpw;


/*--------------------------------------------------------------------------* *		Function Prototypes
\*--------------------------------------------------------------------------*/


	/* All void types */

	/* C routines callable from stub routines come first */

/* set the format of the contour labels */

void  c_pl_setcontlabelformat(PLINT lexp, PLINT sigdig);

/* set offset and spacing of contour labels */

void  c_pl_setcontlabelparam(PLFLT offset, PLFLT size, PLFLT spacing, PLINT active);

/* Advance to subpage "page", or to the next one if "page" = 0. */

void  c_pladv(PLINT page);

/* simple arrow plotter. */

void  c_plvect(PLFLT **u, PLFLT **v, PLINT nx, PLINT ny, PLFLT scale, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

void  c_plsvect(PLFLT *arrowx, PLFLT *arrowy, PLINT npts, PLBOOL fill);

/* This functions similarly to plbox() except that the origin of the axes */
/* is placed at the user-specified point (x0, y0). */

void  c_plaxes(PLFLT x0, PLFLT y0, char *xopt, PLFLT xtick, PLINT nxsub, char *yopt, PLFLT ytick, PLINT nysub);

/* Flags for plbin() - opt argument */
const PL_BIN_DEFAULT = 0;
const PL_BIN_CENTRED = 1;
const PL_BIN_NOEXPAND = 2;
const PL_BIN_NOEMPTY = 4;

/* Plot a histogram using x to store data values and y to store frequencies */
void c_plbin(PLINT nbin, PLFLT *x, PLFLT *y, PLINT opt);

/* Start new page.  Should only be used with pleop(). */

void  c_plbop();

/* This draws a box around the current viewport. */

void  c_plbox(char *xopt, PLFLT xtick, PLINT nxsub, char *yopt, PLFLT ytick, PLINT nysub);

/* This is the 3-d analogue of plbox(). */

void  c_plbox3(char *xopt, char *xlabel, PLFLT xtick, PLINT nsubx, char *yopt, char *ylabel, PLFLT ytick, PLINT nsuby, char *zopt, char *zlabel, PLFLT ztick, PLINT nsubz);

/* Calculate world coordinates and subpage from relative device coordinates. */

void  c_plcalc_world(PLFLT rx, PLFLT ry, PLFLT *wx, PLFLT *wy, PLINT *window);

/* Clear current subpage. */

void  c_plclear();

/* Set color, map 0.  Argument is integer between 0 and 15. */

void  c_plcol0(PLINT icol0);

/* Set color, map 1.  Argument is a float between 0. and 1. */

void  c_plcol1(PLFLT col1);

/* Draws a contour plot from data in f(nx,ny).  Is just a front-end to
 * plfcont, with a particular choice for f2eval and f2eval_data.
 */

void  c_plcont(PLFLT **f, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

/* Draws a contour plot using the function evaluator f2eval and data stored
 * by way of the f2eval_data pointer.  This allows arbitrary organizations
 * of 2d array data to be used.
 */

void  plfcont(PLFLT  function(PLINT , PLINT , PLPointer )f2eval, PLPointer f2eval_data, PLINT nx, PLINT ny, PLINT kx, PLINT lx, PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

/* Copies state parameters from the reference stream to the current stream. */
void c_plcpstrm(PLINT iplsr, PLBOOL flags);

/* Calculate continuous time from broken-down time for current stream. */
void c_plctime(PLINT year, PLINT month, PLINT day, PLINT hour, PLINT min, PLFLT sec, PLFLT *ctime);
  
/* Converts input values from relative device coordinates to relative plot */
/* coordinates. */

void  pldid2pc(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax);

/* Converts input values from relative plot coordinates to relative */
/* device coordinates. */

void  pldip2dc(PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax);

/* End a plotting session for all open streams. */

void  c_plend();

/* End a plotting session for the current stream only. */

void  c_plend1();

/* Simple interface for defining viewport and window. */

void  c_plenv(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLINT just, PLINT axis);


/* similar to plenv() above, but in multiplot mode does not advance the subpage,
 instead the current subpage is cleared */

void  c_plenv0(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLINT just, PLINT axis);

/* End current page.  Should only be used with plbop(). */

void  c_pleop();

/* Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)) */

void  c_plerrx(PLINT n, PLFLT *xmin, PLFLT *xmax, PLFLT *y);

/* Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)) */

void  c_plerry(PLINT n, PLFLT *x, PLFLT *ymin, PLFLT *ymax);

/* Advance to the next family file on the next new page */

void  c_plfamadv();

/* Pattern fills the polygon bounded by the input points. */

void  c_plfill(PLINT n, PLFLT *x, PLFLT *y);

/* Pattern fills the 3d polygon bounded by the input points. */

void  c_plfill3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z);

/* Flushes the output stream.  Use sparingly, if at all. */

void  c_plflush();

/* Sets the global font flag to 'ifont'. */

void  c_plfont(PLINT ifont);

/* Load specified font set. */

void  c_plfontld(PLINT fnt);

/* Get character default height and current (scaled) height */

void  c_plgchr(PLFLT *p_def, PLFLT *p_ht);

/* Returns 8 bit RGB values for given color from color map 0 */

void  c_plgcol0(PLINT icol0, PLINT *r, PLINT *g, PLINT *b);

/* Returns 8 bit RGB values for given color from color map 0 and alpha value */

void  c_plgcol0a(PLINT icol0, PLINT *r, PLINT *g, PLINT *b, PLFLT *a);

/* Returns the background color by 8 bit RGB value */

void  c_plgcolbg(PLINT *r, PLINT *g, PLINT *b);

/* Returns the background color by 8 bit RGB value and alpha value */

void  c_plgcolbga(PLINT *r, PLINT *g, PLINT *b, PLFLT *a);

/* Returns the current compression setting */

void  c_plgcompression(PLINT *compression);

/* Get the current device (keyword) name */

void  c_plgdev(char *p_dev);

/* Retrieve current window into device space */

void  c_plgdidev(PLFLT *p_mar, PLFLT *p_aspect, PLFLT *p_jx, PLFLT *p_jy);

/* Get plot orientation */

void  c_plgdiori(PLFLT *p_rot);

/* Retrieve current window into plot space */

void  c_plgdiplt(PLFLT *p_xmin, PLFLT *p_ymin, PLFLT *p_xmax, PLFLT *p_ymax);

/* Get FCI (font characterization integer) */

void  c_plgfci(PLUNICODE *pfci);

/* Get family file parameters */

void  c_plgfam(PLINT *p_fam, PLINT *p_num, PLINT *p_bmax);

/* Get the (current) output file name.  Must be preallocated to >80 bytes */

void  c_plgfnam(char *fnam);

/* Get the current font family, style and weight */

void  c_plgfont(PLINT *p_family, PLINT *p_style, PLINT *p_weight);

/* Get the (current) run level.  */

void  c_plglevel(PLINT *p_level);

/* Get output device parameters. */

void  c_plgpage(PLFLT *p_xp, PLFLT *p_yp, PLINT *p_xleng, PLINT *p_yleng, PLINT *p_xoff, PLINT *p_yoff);

/* Switches to graphics screen. */

void  c_plgra();

  /* grid irregularly sampled data */

void  c_plgriddata(PLFLT *x, PLFLT *y, PLFLT *z, PLINT npts, PLFLT *xg, PLINT nptsx, PLFLT *yg, PLINT nptsy, PLFLT **zg, PLINT type, PLFLT data);

  /* type of gridding algorithm for plgriddata() */

const GRID_CSA = 1;
const GRID_DTLI = 2;
const GRID_NNI = 3;
const GRID_NNIDW = 4;
const GRID_NNLI = 5;

const GRID_NNAIDW = 6;
/* Get subpage boundaries in absolute coordinates */

void  c_plgspa(PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax);

/* Get current stream number. */

void  c_plgstrm(PLINT *p_strm);

/* Get the current library version number */

void  c_plgver(char *p_ver);

/* Get viewport boundaries in normalized device coordinates */

void  c_plgvpd(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get viewport boundaries in world coordinates */

void  c_plgvpw(PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax);

/* Get x axis labeling parameters */

void  c_plgxax(PLINT *p_digmax, PLINT *p_digits);

/* Get y axis labeling parameters */

void  c_plgyax(PLINT *p_digmax, PLINT *p_digits);

/* Get z axis labeling parameters */

void  c_plgzax(PLINT *p_digmax, PLINT *p_digits);

/* Draws a histogram of n values of a variable in array data[0..n-1] */

/* Flags for plhist() - opt argument; note: some flags are passed to
   plbin() for the actual plotting */
const PL_HIST_DEFAULT = 0;
const PL_HIST_NOSCALING = 1;
const PL_HIST_IGNORE_OUTLIERS = 2;
const PL_HIST_NOEXPAND = 8;

const PL_HIST_NOEMPTY = 16;
void  c_plhist(PLINT n, PLFLT *data, PLFLT datmin, PLFLT datmax, PLINT nbin, PLINT opt);

/* Set current color (map 0) by hue, lightness, and saturation. */

void  c_plhls(PLFLT h, PLFLT l, PLFLT s);

/* Functions for converting between HLS and RGB color space */

void  c_plhlsrgb(PLFLT h, PLFLT l, PLFLT s, PLFLT *p_r, PLFLT *p_g, PLFLT *p_b);

/* Initializes PLplot, using preset or default options */

void  c_plinit();

/* Draws a line segment from (x1, y1) to (x2, y2). */

void  c_pljoin(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2);

/* Simple routine for labelling graphs. */

void  c_pllab(char *xlabel, char *ylabel, char *tlabel);

/* Sets position of the light source */
void  c_pllightsource(PLFLT x, PLFLT y, PLFLT z);

/* Draws line segments connecting a series of points. */
void c_plline(PLINT n, PLFLT *x, PLFLT *y);

/* Draws a line in 3 space.  */

void  c_plline3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z);

/* Set line style. */

void  c_pllsty(PLINT lin);

/* plot continental outline in world coordinates */

void  c_plmap(void  function(PLINT , PLFLT *, PLFLT *)mapform, char *type, PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);

/* Plot the latitudes and longitudes on the background. */

void  c_plmeridians(void  function(PLINT , PLFLT *, PLFLT *)mapform, PLFLT dlong, PLFLT dlat, PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat);

/* Plots a mesh representation of the function z[x][y]. */

void  c_plmesh(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt);

/* Plots a mesh representation of the function z[x][y] with contour */

void  c_plmeshc(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel);

/* Creates a new stream and makes it the default.  */

void  c_plmkstrm(PLINT *p_strm);

/* Prints out "text" at specified position relative to viewport */
void c_plmtex(char *side, PLFLT disp, PLFLT pos, PLFLT just, char *text);

/* Prints out "text" at specified position relative to viewport (3D)*/

void  c_plmtex3(char *side, PLFLT disp, PLFLT pos, PLFLT just, char *text);
	 
/* Plots a 3-d representation of the function z[x][y]. */

void  c_plot3d(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLBOOL side);

/* Plots a 3-d representation of the function z[x][y] with contour. */

void  c_plot3dc(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel);

/* Plots a 3-d representation of the function z[x][y] with contour and
 * y index limits. */

void  c_plot3dcl(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel, PLINT ixstart, PLINT ixn, PLINT *indexymin, PLINT *indexymax);

/*
 * definitions for the opt argument in plot3dc() and plsurf3d()
 *
 * DRAW_LINEX *must* be 1 and DRAW_LINEY *must* be 2, because of legacy code!
 */
const DRAW_LINEX=1<<0; /* draw lines parallel to the X axis */
const DRAW_LINEY=1<<1; /* draw lines parallel to the Y axis */
const DRAW_LINEXY=DRAW_LINEX|DRAW_LINEY; /* draw lines parallel to both the X and Y axis */
const MAG_COLOR=1<<2; /* draw the mesh with a color dependent of the magnitude */
const BASE_CONT=1<<3; /* draw contour plot at bottom xy plane */
const TOP_CONT=1<<4; /* draw contour plot at top xy plane */
const SURF_CONT=1<<5; /* draw contour plot at surface */
const DRAW_SIDES=1<<6; /* draw sides */
const FACETED=1<<7; /* draw outline for each square that makes up the surface */
const MESH=1<<8; /* draw mesh */

  /*
   *  valid options for plot3dc():
   *
   *  DRAW_SIDES, BASE_CONT, TOP_CONT (not yet),
   *  MAG_COLOR, DRAW_LINEX, DRAW_LINEY, DRAW_LINEXY.
   *
   *  valid options for plsurf3d():
   *
   *  MAG_COLOR, BASE_CONT, SURF_CONT, FACETED, DRAW_SIDES.
   */

/* Set fill pattern directly. */

void  c_plpat(PLINT nlin, PLINT *inc, PLINT *del);

/* Plots array y against x for n points using ASCII code "code".*/

void  c_plpoin(PLINT n, PLFLT *x, PLFLT *y, PLINT code);

/* Draws a series of points in 3 space. */

void  c_plpoin3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT code);

/* Draws a polygon in 3 space.  */

void  c_plpoly3(PLINT n, PLFLT *x, PLFLT *y, PLFLT *z, PLBOOL *draw, PLBOOL ifcc);

/* Set the floating point precision (in number of places) in numeric labels. */

void  c_plprec(PLINT setp, PLINT prec);

/* Set fill pattern, using one of the predefined patterns.*/

void  c_plpsty(PLINT patt);

/* Prints out "text" at world cooordinate (x,y). */
void c_plptex(PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, char *text);

/* Prints out "text" at world cooordinate (x,y,z). */

void  c_plptex3(PLFLT wx, PLFLT wy, PLFLT wz, PLFLT dx, PLFLT dy, PLFLT dz, PLFLT sx, PLFLT sy, PLFLT sz, PLFLT just, char *text);

/* Replays contents of plot buffer to current device/file. */

void  c_plreplot();

/* Set line color by red, green, blue from  0. to 1. */

void  c_plrgb(PLFLT r, PLFLT g, PLFLT b);

/* Set line color by 8 bit RGB values. */

void  c_plrgb1(PLINT r, PLINT g, PLINT b);

/* Functions for converting between HLS and RGB color space */

void  c_plrgbhls(PLFLT r, PLFLT g, PLFLT b, PLFLT *p_h, PLFLT *p_l, PLFLT *p_s);

/* Set character height. */

void  c_plschr(PLFLT def, PLFLT scale);

/* Set color map 0 colors by 8 bit RGB values */

void  c_plscmap0(PLINT *r, PLINT *g, PLINT *b, PLINT ncol0);

/* Set color map 0 colors by 8 bit RGB values and alpha values */

void  c_plscmap0a(PLINT *r, PLINT *g, PLINT *b, PLFLT *a, PLINT ncol0);

/* Set number of colors in cmap 0 */

void  c_plscmap0n(PLINT ncol0);

/* Set color map 1 colors by 8 bit RGB values */

void  c_plscmap1(PLINT *r, PLINT *g, PLINT *b, PLINT ncol1);

/* Set color map 1 colors by 8 bit RGB and alpha values */

void  c_plscmap1a(PLINT *r, PLINT *g, PLINT *b, PLFLT *a, PLINT ncol1);

/* Set color map 1 colors using a piece-wise linear relationship between */
/* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space. */

void  c_plscmap1l(PLBOOL itype, PLINT npts, PLFLT *intensity, PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLBOOL *rev);

/* Set color map 1 colors using a piece-wise linear relationship between */
/* intensity [0,1] (cmap 1 index) and position in HLS or RGB color space. */
/* Will also linear interpolate alpha values. */

void  c_plscmap1la(PLBOOL itype, PLINT npts, PLFLT *intensity, PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLFLT *a, PLBOOL *rev);

/* Set number of colors in cmap 1 */

void  c_plscmap1n(PLINT ncol1);

/* Set a given color from color map 0 by 8 bit RGB value */

void  c_plscol0(PLINT icol0, PLINT r, PLINT g, PLINT b);

/* Set a given color from color map 0 by 8 bit RGB value */

void  c_plscol0a(PLINT icol0, PLINT r, PLINT g, PLINT b, PLFLT a);

/* Set the background color by 8 bit RGB value */

void  c_plscolbg(PLINT r, PLINT g, PLINT b);

/* Set the background color by 8 bit RGB value and alpha value*/

void  c_plscolbga(PLINT r, PLINT g, PLINT b, PLFLT a);

/* Used to globally turn color output on/off */

void  c_plscolor(PLINT color);

/* Set the compression level */

void  c_plscompression(PLINT compression);

/* Set the device (keyword) name */
void c_plsdev(char *devname);

/* Set window into device space using margin, aspect ratio, and */
/* justification */

void  c_plsdidev(PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy);

/* Set up transformation from metafile coordinates. */

void  c_plsdimap(PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax, PLFLT dimxpmm, PLFLT dimypmm);

/* Set plot orientation, specifying rotation in units of pi/2. */

void  c_plsdiori(PLFLT rot);

/* Set window into plot space */

void  c_plsdiplt(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

/* Set window into plot space incrementally (zoom) */

void  c_plsdiplz(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

/* Set the escape character for text strings. */

void  c_plsesc(char esc);

/* Set family file parameters */

void  c_plsfam(PLINT fam, PLINT num, PLINT bmax);

/* Set FCI (font characterization integer) */

void  c_plsfci(PLUNICODE fci);

/* Set the output file name. */
void c_plsfnam(char *fnam);

/* Set the current font family, style and weight */

void  c_plsfont(PLINT family, PLINT style, PLINT weight);

/* Shade region. */

void  c_plshade(PLFLT **a, PLINT nx, PLINT ny, PLINT  function(PLFLT , PLFLT )defined, PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void  function(PLINT , PLFLT *, PLFLT *)fill, PLBOOL rectangular, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

void  c_plshade1(PLFLT *a, PLINT nx, PLINT ny, PLINT  function(PLFLT , PLFLT )defined, PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void  function(PLINT , PLFLT *, PLFLT *)fill, PLBOOL rectangular, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

void  c_plshades(PLFLT **a, PLINT nx, PLINT ny, PLINT  function(PLFLT , PLFLT )defined, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT *clevel, PLINT nlevel, PLINT fill_width, PLINT cont_color, PLINT cont_width, void  function(PLINT , PLFLT *, PLFLT *)fill, PLBOOL rectangular, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

void  plfshade(PLFLT  function(PLINT , PLINT , PLPointer )f2eval, PLPointer f2eval_data, PLFLT  function(PLINT , PLINT , PLPointer )c2eval, PLPointer c2eval_data, PLINT nx, PLINT ny, PLFLT left, PLFLT right, PLFLT bottom, PLFLT top, PLFLT shade_min, PLFLT shade_max, PLINT sh_cmap, PLFLT sh_color, PLINT sh_width, PLINT min_color, PLINT min_width, PLINT max_color, PLINT max_width, void  function(PLINT , PLFLT *, PLFLT *)fill, PLBOOL rectangular, void  function(PLFLT , PLFLT , PLFLT *, PLFLT *, PLPointer )pltr, PLPointer pltr_data);

/* Set up lengths of major tick marks. */

void  c_plsmaj(PLFLT def, PLFLT scale);

/* Set the memory area to be plotted (with the 'mem' driver) */

void  c_plsmem(PLINT maxx, PLINT maxy, void *plotmem);

/* Set up lengths of minor tick marks. */

void  c_plsmin(PLFLT def, PLFLT scale);

/* Set orientation.  Must be done before calling plinit. */

void  c_plsori(PLINT ori);

/* Set output device parameters.  Usually ignored by the driver. */

void  c_plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng, PLINT xoff, PLINT yoff);

/* Set the pause (on end-of-page) status */

void  c_plspause(PLBOOL pause);

/* Set stream number.  */

void  c_plsstrm(PLINT strm);

/* Set the number of subwindows in x and y */

void  c_plssub(PLINT nx, PLINT ny);

/* Set symbol height. */

void  c_plssym(PLFLT def, PLFLT scale);

/* Initialize PLplot, passing in the windows/page settings. */

void  c_plstar(PLINT nx, PLINT ny);

/* Initialize PLplot, passing the device name and windows/page settings. */

void  c_plstart(char *devname, PLINT nx, PLINT ny);

/* Add a point to a stripchart.  */

void  c_plstripa(PLINT id, PLINT pen, PLFLT x, PLFLT y);

/* Create 1d stripchart */

void  c_plstripc(PLINT *id, char *xspec, char *yspec, PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax, PLFLT xlpos, PLFLT ylpos, PLBOOL y_ascl, PLBOOL acc, PLINT colbox, PLINT collab, PLINT *colline, PLINT *styline, char **legline, char *labx, char *laby, char *labtop);

/* Deletes and releases memory used by a stripchart.  */

void  c_plstripd(PLINT id);

/* plots a 2d image (or a matrix too large for plshade() ) */

void  c_plimagefr(PLFLT **idata, PLINT nx, PLINT ny, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax, PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax, PLFLT valuemin, PLFLT valuemax);

/* plots a 2d image (or a matrix too large for plshade() ) - colors
   automatically scaled */

void  c_plimage(PLFLT **idata, PLINT nx, PLINT ny, PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax, PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax);

/* Set up a new line style */

void  c_plstyl(PLINT nms, PLINT *mark, PLINT *space);

/* Plots the 3d surface representation of the function z[x][y]. */

void  c_plsurf3d(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel);

/* Plots the 3d surface representation of the function z[x][y] with y
 * index limits. */

void  c_plsurf3dl(PLFLT *x, PLFLT *y, PLFLT **z, PLINT nx, PLINT ny, PLINT opt, PLFLT *clevel, PLINT nlevel, PLINT ixstart, PLINT ixn, PLINT *indexymin, PLINT *indexymax);

/* Sets the edges of the viewport to the specified absolute coordinates */

void  c_plsvpa(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/* Set x axis labeling parameters */

void  c_plsxax(PLINT digmax, PLINT digits);

/* Set inferior X window */

void  plsxwin(PLINT window_id);

/* Set y axis labeling parameters */

void  c_plsyax(PLINT digmax, PLINT digits);

/* Plots array y against x for n points using Hershey symbol "code" */

void  c_plsym(PLINT n, PLFLT *x, PLFLT *y, PLINT code);

/* Set z axis labeling parameters */

void  c_plszax(PLINT digmax, PLINT digits);

/* Switches to text screen. */

void  c_pltext();

/* Set the format for date / time labels */
 

void  c_pltimefmt(char *fmt);

/* Sets the edges of the viewport with the given aspect ratio, leaving */
/* room for labels. */

void  c_plvasp(PLFLT aspect);

/* Creates the largest viewport of the specified aspect ratio that fits */
/* within the specified normalized subpage coordinates. */

void  c_plvpas(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect);

/* Creates a viewport with the specified normalized subpage coordinates. */

void  c_plvpor(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/* Defines a "standard" viewport with seven character heights for */
/* the left margin and four character heights everywhere else. */

void  c_plvsta();

/* Set up a window for three-dimensional plotting. */

void  c_plw3d(PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0, PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0, PLFLT zmax0, PLFLT alt, PLFLT az);

/* Set pen width. */

void  c_plwid(PLINT width);

/* Set up world coordinates of the viewport boundaries (2d plots). */

void  c_plwind(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

/*  set xor mode; mode = 1-enter, 0-leave, status = 0 if not interactive device  */

void  c_plxormod(PLBOOL mode, PLBOOL *status);

/*--------------------------------------------------------------------------* *		Functions for use from C or C++ only
\*--------------------------------------------------------------------------*/

/* Returns a list of file-oriented device names and their menu strings */

void  plgFileDevs(char ***p_menustr, char ***p_devname, int *p_ndev);

/* Returns a list of all device names and their menu strings */

void  plgDevs(char ***p_menustr, char ***p_devname, int *p_ndev);

/* Set the function pointer for the keyboard event handler */

void  plsKeyEH(void  function(PLGraphicsIn *, void *, int *)KeyEH, void *KeyEH_data);

/* Set the function pointer for the (mouse) button event handler */

void  plsButtonEH(void  function(PLGraphicsIn *, void *, int *)ButtonEH, void *ButtonEH_data);

/* Sets an optional user bop handler */

void  plsbopH(void  function(void *, int *)handler, void *handler_data);

/* Sets an optional user eop handler */

void  plseopH(void  function(void *, int *)handler, void *handler_data);

/* Set the variables to be used for storing error info */

void  plsError(PLINT *errcode, char *errmsg);

/* Sets an optional user exit handler. */

void  plsexit(int  function(char *)handler);

/* Sets an optional user abort handler. */

void  plsabort(void  function(char *)handler);

	/* Transformation routines */

/* Identity transformation. */

void  pltr0(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Does linear interpolation from singly dimensioned coord arrays. */

void  pltr1(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (column dominant, as per normal C 2d arrays). */

void  pltr2(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Just like pltr2() but uses pointer arithmetic to get coordinates from */
/* 2d grid tables.  */

void  pltr2p(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);

/* Identity transformation for plots from Fortran. */

void  pltr0f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (row dominant, i.e. Fortran ordering). */

void  pltr2f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

	/* Function evaluators */

/* Does a lookup from a 2d function array.  Array is of type (PLFLT **), */
/* and is column dominant (normal C ordering). */

PLFLT  plf2eval2(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is column dominant (normal C ordering). */

PLFLT  plf2eval(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is row dominant (Fortran ordering). */

PLFLT  plf2evalr(PLINT ix, PLINT iy, PLPointer plf2eval_data);

	/* Command line parsing utilities */

/* Clear internal option table info structure. */

void  plClearOpts();

/* Reset internal option table info structure. */

void  plResetOpts();

/* Merge user option table into internal info structure. */

int  plMergeOpts(PLOptionTable *options, char *name, char **notes);

/* Set the strings used in usage and syntax messages. */

void  plSetUsage(char *program_string, char *usage_string);

/* Process input strings, treating them as an option and argument pair. */
/* The first is for the external API, the second the work routine declared
   here for backward compatibilty. */

int  c_plsetopt(char *opt, char *optarg);

int  plSetOpt(char *opt, char *optarg);

/* Process options list using current options info. */
int c_plparseopts(int *p_argc, char **argv, PLINT mode);

/* Print usage & syntax message. */

void  plOptUsage();

	/* Miscellaneous */

/* Get the escape character for text strings. */

void  plgesc(char *p_esc);

/* Front-end to driver escape function. */

void  pl_cmd(PLINT op, void *ptr);

/* Return full pathname for given file if executable */

int  plFindName(char *p);

/* Looks for the specified executable file according to usual search path. */

char * plFindCommand(char *fn);

/* Gets search name for file by concatenating the dir, subdir, and file */
/* name, allocating memory as needed.  */

void  plGetName(char *dir, char *subdir, char *filename, char **filespec);

/* Prompts human to input an integer in response to given message. */

PLINT  plGetInt(char *s);

/* Prompts human to input a float in response to given message. */

PLFLT  plGetFlt(char *s);

	/* Nice way to allocate space for a vectored 2d grid */

/* Allocates a block of memory for use as a 2-d grid of PLFLT's.  */

void  plAlloc2dGrid(PLFLT ***f, PLINT nx, PLINT ny);

/* Frees a block of memory allocated with plAlloc2dGrid(). */

void  plFree2dGrid(PLFLT **f, PLINT nx, PLINT ny);

/* Find the maximum and minimum of a 2d matrix allocated with plAllc2dGrid(). */

void  plMinMax2dGrid(PLFLT **f, PLINT nx, PLINT ny, PLFLT *fmax, PLFLT *fmin);

/* Wait for graphics input event and translate to world coordinates */

int  plGetCursor(PLGraphicsIn *gin);

/* Translates relative device coordinates to world coordinates.  */

int  plTranslateCursor(PLGraphicsIn *gin);

