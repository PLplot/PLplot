/* plplotcapi.i contains the common API (API used by the many computer
 * language interfaces for the PLplot library) of PLplot in a form that
 * is readily understood by swig.
 */
/*

Copyright (C) 2002  Gary Bishop
Copyright (C) 2002, 2004  Alan W. Irwin
Copyright (C) 2004  Rafael Laboissiere
Copyright (C) 2004  Andrew Ross

This file is part of PLplot.

PLplot is free software; you can redistribute it and/or modify
it under the terms of the GNU Library General Public License as published by
the Free Software Foundation; version 2 of the License.

PLplot is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Library General Public License for more details.

You should have received a copy of the GNU Library General Public License
along with the file PLplot; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
*/

/* For passing user data, as with X's XtPointer */

typedef void* PLPointer;

/*--------------------------------------------------------------------------*\
 * Complex data types and other good stuff
\*--------------------------------------------------------------------------*/

/* Switches for escape function call. */
/* Some of these are obsolete but are retained in order to process
   old metafiles */

#define PLESC_SET_RGB		1	/* obsolete */
#define PLESC_ALLOC_NCOL	2	/* obsolete */
#define PLESC_SET_LPB		3	/* obsolete */
#define PLESC_EXPOSE		4	/* handle window expose */
#define PLESC_RESIZE		5	/* handle window resize */
#define PLESC_REDRAW		6	/* handle window redraw */
#define PLESC_TEXT		7	/* switch to text screen */
#define PLESC_GRAPH		8	/* switch to graphics screen */
#define PLESC_FILL		9	/* fill polygon */
#define PLESC_DI		10	/* handle DI command */
#define PLESC_FLUSH		11	/* flush output */
#define PLESC_EH		12      /* handle Window events */
#define PLESC_GETC		13	/* get cursor position */
#define PLESC_SWIN		14	/* set window parameters */
#define PLESC_PLFLTBUFFERING	15	/* configure PLFLT buffering */
#define PLESC_XORMOD		16	/* set xor mode */
#define PLESC_SET_COMPRESSION	17	/* AFR: set compression */
#define PLESC_CLEAR		18      /* RL: clear graphics region */
#define PLESC_DASH		19	/* RL: draw dashed line */
#define PLESC_HAS_TEXT		20	/* driver draws text */
#define PLESC_IMAGE		21	/* handle image */
#define PLESC_IMAGEOPS          22      /* plimage related operations */

/* image operations */
#if 0
#define ZEROW2B   1
#define ZEROW2D   2
#define ONEW2B    3
#define ONEW2D    4
#endif

/* definitions for the opt argument in plsurf3d() */

#define DRAW_LINEX 0x01 /* draw lines parallel to the X axis */
#define DRAW_LINEY 0x02 /* draw lines parallel to the Y axis */
#define DRAW_LINEXY 0x03 /* draw lines parallel to both the X and Y axes */
#define MAG_COLOR 0x04 /* draw the mesh with a color dependent of the magnitude */
#define BASE_CONT 0x08 /* draw contour plot at bottom xy plane */
#define TOP_CONT 0x10 /* draw contour plot at top xy plane */
#define SURF_CONT 0x20 /* draw contour plot at surface */
#define DRAW_SIDES 0x40 /* draw sides */
#define FACETED   0x80 /* draw outline for each square that makes up the surface */
#define MESH 0x100 /* draw mesh */

/* Flags for plbin() - opt argument */
#define PL_BIN_DEFAULT          0
#define PL_BIN_CENTRED          1
#define PL_BIN_NOEXPAND         2
#define PL_BIN_NOEMPTY          4

/* Flags for plhist() - opt argument; note: some flags are passed to
   plbin() for the actual plotting */
#define PL_HIST_DEFAULT         0
#define PL_HIST_NOSCALING       1
#define PL_HIST_IGNORE_OUTLIERS 2
#define PL_HIST_NOEXPAND        8
#define PL_HIST_NOEMPTY        16

/* Window parameter tags */

#define PLSWIN_DEVICE		1	/* device coordinates */
#define PLSWIN_WORLD		2	/* world coordinates */

/* PLplot Option table & support constants */

/* Option-specific settings */

#define PL_OPT_ENABLED		0x0001	/* Obsolete */
#define PL_OPT_ARG		0x0002	/* Option has an argment */
#define PL_OPT_NODELETE		0x0004	/* Don't delete after processing */
#define PL_OPT_INVISIBLE	0x0008	/* Make invisible */
#define PL_OPT_DISABLED		0x0010	/* Processing is disabled */

/* Option-processing settings -- mutually exclusive */

#define PL_OPT_FUNC		0x0100	/* Call handler function */
#define PL_OPT_BOOL		0x0200	/* Set *var = 1 */
#define PL_OPT_INT		0x0400	/* Set *var = atoi(optarg) */
#define PL_OPT_FLOAT		0x0800	/* Set *var = atof(optarg) */
#define PL_OPT_STRING		0x1000	/* Set var = optarg */

/* Global mode settings */
/* These override per-option settings */

#define PL_PARSE_PARTIAL	0x0000	/* For backward compatibility */
#define PL_PARSE_FULL		0x0001	/* Process fully & exit if error */
#define PL_PARSE_QUIET		0x0002	/* Don't issue messages */
#define PL_PARSE_NODELETE	0x0004	/* Don't delete options after */
					/* processing */
#define PL_PARSE_SHOWALL	0x0008	/* Show invisible options */
#define PL_PARSE_OVERRIDE	0x0010	/* Obsolete */
#define PL_PARSE_NOPROGRAM	0x0020	/* Program name NOT in *argv[0].. */
#define PL_PARSE_NODASH		0x0040	/* Set if leading dash NOT required */
#define PL_PARSE_SKIP		0x0080	/* Skip over unrecognized args */

#define PL_MAXKEY 16

typedef struct {
    int type;                   /* of event (CURRENTLY UNUSED) */
    unsigned int state;         /* key or button mask */
    unsigned int keysym;        /* key selected */
    unsigned int button;        /* mouse button selected */
    PLINT subwindow;            /* subwindow (alias subpage, alias subplot) number */
    char string[PL_MAXKEY];     /* translated string */
    int pX, pY;                 /* absolute device coordinates of pointer */
    PLFLT dX, dY;               /* relative device coordinates of pointer */
    PLFLT wX, wY;               /* world coordinates of pointer */
} PLGraphicsIn;


/* Structure for describing the plot window */

#define PL_MAXWINDOWS	64	/* Max number of windows/page tracked */

/* Macro used (in some cases) to ignore value of argument */
/* I don't plan on changing the value so you can hard-code it */

#define PL_NOTSET (-42)


#define PLESPLFLTBUFFERING_ENABLE     1
#define PLESPLFLTBUFFERING_DISABLE    2
#define PLESPLFLTBUFFERING_QUERY      3

#define GRID_CSA    1 /* Bivariate Cubic Spline approximation */
#define GRID_DTLI   2 /* Delaunay Triangulation Linear Interpolation */
#define GRID_NNI    3 /* Natural Neighbors Interpolation */
#define GRID_NNIDW  4 /* Nearest Neighbors Inverse Distance Weighted */
#define GRID_NNLI   5 /* Nearest Neighbors Linear Interpolation */
#define GRID_NNAIDW 6 /* Nearest Neighbors Around Inverse Distance Weighted  */


#ifdef SWIG_PYTHON
#define SWIG_OBJECT_DATA PYOBJECT_DATA
#else
#define SWIG_OBJECT_DATA OBJECT_DATA
#endif

#ifdef SWIG_PYTHON

/* Non-common API that are included here because they traditionally
 * were part of plmodule.c. */

#if 0

/* Deprecated function that we no longer want to propagate to the
 * python API. */

%feature("autodoc", "Plot an arrow.") plarrows;
void
plarrows(PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT n,
         PLFLT scale, PLFLT dx, PLFLT dy) ;

#endif

%feature("autodoc", "Set inferior X window.") plsxwin;
void
plsxwin(PLINT window_id);

#endif /* SWIG_PYTHON */

/* Complete list of common API (has "c_" suffix version defined in plplot.h) */

%feature("autodoc", "Set the format of the contour labels.")
pl_setcontlabelformat;
void
pl_setcontlabelformat(PLINT lexp, PLINT sigdig);

%feature("autodoc", "Set offset and spacing of contour labels.") pl_setcontlabelparam;
void
pl_setcontlabelparam(PLFLT offset, PLFLT size, PLFLT spacing, PLINT active);

%feature("autodoc", "Advance to subpage \"page\", or to the next one if \"page\" = 0.") pladv;
void
pladv(PLINT page);

%feature("autodoc", "This functions similarly to plbox() except that the origin of the axes is placed at the user-specified point (x0, y0).") plaxes;
void
plaxes(PLFLT x0, PLFLT y0, const char *xopt, PLFLT xtick, PLINT nxsub,
	 const char *yopt, PLFLT ytick, PLINT nysub);

%feature("autodoc", "Plot a histogram using x to store data values and y to store frequencies.") plbin;
void
plbin(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLINT center);

%feature("autodoc", "Start new page.  Should only be used with pleop().") plbop;
void
plbop(void);

%feature("autodoc", "Draw a box around the current viewport.") plbox;
void
plbox(const char *xopt, PLFLT xtick, PLINT nxsub,
	const char *yopt, PLFLT ytick, PLINT nysub);

%feature("autodoc", "This is the 3-d analogue of plbox().") plbox3;
void
plbox3(const char *xopt, const char *xlabel, PLFLT xtick, PLINT nsubx,
	 const char *yopt, const char *ylabel, PLFLT ytick, PLINT nsuby,
	 const char *zopt, const char *zlabel, PLFLT ztick, PLINT nsubz);

%feature("autodoc", "Calculate world coordinates and subpage from relative device coordinates.") plcalc_world;
void
plcalc_world(PLFLT rx, PLFLT ry, PLFLT *OUTPUT, PLFLT *OUTPUT, PLINT *OUTPUT);

%feature("autodoc", "Clear current subpage.") plclear;
void
plclear(void);

%feature("autodoc", "Set color, map 0.  Argument is integer between 0 and 15.") plcol0;
void
plcol0(PLINT icol0);

%feature("autodoc", "Set color, map 1.  Argument is a float between 0. and 1.") plcol1;
void
plcol1(PLFLT col1);

%feature("autodoc", "Draw a contour plot.") plcont;
void
plcont(PLFLT **Matrix, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	 PLINT ky, PLINT ly, PLFLT *Array, PLINT n,
	 pltr_func pltr,
	 PLPointer SWIG_OBJECT_DATA);


%feature("autodoc", "Copy state parameters from the reference stream to the current stream.") plcpstrm;
void
plcpstrm(PLINT iplsr, PLBOOL flags);

%feature("autodoc", "End a plotting session for all open streams.") plend;
void
plend(void);

%feature("autodoc", "End a plotting session for the current stream only.") plend1;
void
plend1(void);

%feature("autodoc", "Simple interface for defining viewport and window.") plenv;
void
plenv(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLINT just, PLINT axis);

%feature("autodoc", "Simple interface for defining viewport and window.") plenv0;
void
plenv0(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	PLINT just, PLINT axis);

%feature("autodoc", "End current page.  Should only be used with plbop().") pleop;
void
pleop(void);

%feature("autodoc", "Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i)).") plerrx;
void
plerrx(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk);

%feature("autodoc", "Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i)).") plerry;
void
plerry(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk);

%feature("autodoc", "Advance to the next family file on the next new page.") plfamadv;
void
plfamadv(void);

%feature("autodoc", "Pattern fills the polygon bounded by the input points.") plfill;
void
plfill(PLINT n, PLFLT *Array, PLFLT *ArrayCk);

%feature("autodoc", "Pattern fills the 3d polygon bounded by the input points.") plfill3;
void
plfill3(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk);

%feature("autodoc", "Flush the output stream.  Use sparingly, if at all.") plflush;
void
plflush(void);

%feature("autodoc", "Set the global font flag to 'ifont'.") plfont;
void
plfont(PLINT ifont);

%feature("autodoc", "Load specified font set.") plfontld;
void
plfontld(PLINT fnt);

%feature("autodoc", "Get character default height and current (scaled) height.") plgchr;
void
plgchr(PLFLT *OUTPUT, PLFLT *OUTPUT);

%feature("autodoc", "Get 8 bit RGB values for given color from color map 0.") plgcol0;
void
plgcol0(PLINT icol0, PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT);

%feature("autodoc", "Get 8-bit RGB background color.") plgcolbg;
void
plgcolbg(PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT);

%feature("autodoc", "Get the current compression setting.") plgcompression;
void
plgcompression(PLINT *OUTPUT);

%feature("autodoc", "Get the current device (keyword) name.") plgdev;
void
plgdev(char *OUTPUT);

%feature("autodoc", "Retrieve current window into device space.") plgdidev;
void
plgdidev(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

%feature("autodoc", "Get plot orientation.") plgdiori;
void
plgdiori(PLFLT *OUTPUT);

%feature("autodoc", "Retrieve current window into plot space.") plgdiplt;
void
plgdiplt(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

%feature("autodoc", "Get family file parameters.") plgfam;
void
plgfam(PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT);

%feature("autodoc", "Get FCI (font characterization integer).") plgfci;
void
plgfci(PLUNICODE *OUTPUT);

%feature("autodoc", "Get the (current) output file name.") plgfnam;
void
plgfnam(char *OUTPUT);

%feature("autodoc", "Get the (current) run level.") plglevel;
void
plglevel(PLINT *OUTPUT);

%feature("autodoc", "Get output device parameters.") plgpage;
void
plgpage(PLFLT *OUTPUT, PLFLT *OUTPUT,
	  PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT);

%feature("autodoc", "Switch to graphics screen.") plgra;
void
plgra(void);

%feature("autodoc", "Grid irregular sample data.") plgriddata;
void
plgriddata(PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT n,
           PLFLT *ArrayX, PLINT nx, PLFLT *ArrayY,  PLINT ny,
	   PLFLT **OutMatrixCk, PLINT type, PLFLT data);


%feature("autodoc", "Get subpage boundaries in absolute coordinates.") plgspa;
void
plgspa(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

%feature("autodoc", "Get current stream number.") plgstrm;
void
plgstrm(PLINT *OUTPUT);

%feature("autodoc", "Get current library version number.") plgver;
void
plgver(char *OUTPUT);

%feature("autodoc", "Get viewport boundaries in normalized device coordinates.") plgvpd;
void
plgvpd(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

%feature("autodoc", "Get viewport boundaries in world coordinates.") plgvpw;
void
plgvpw(PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

%feature("autodoc", "Get x axis labeling parameters.") plgxax;
void
plgxax(PLINT *OUTPUT, PLINT *OUTPUT);

%feature("autodoc", "Get y axis labeling parameters.") plgyax;
void
plgyax(PLINT *OUTPUT, PLINT *OUTPUT);

%feature("autodoc", "Get z axis labeling parameters.") plgzax;
void
plgzax(PLINT *OUTPUT, PLINT *OUTPUT);

%feature("autodoc", "Draw histogram.") plhist;
void
plhist(PLINT n, PLFLT *Array, PLFLT datmin, PLFLT datmax,
	 PLINT nbin, PLINT oldwin);

%feature("autodoc", "Set current color (map 0) by hue, lightness, and saturation.") plhls;
void
plhls(PLFLT h, PLFLT l, PLFLT s);

%feature("autodoc", "Transform from HLS to RGB form of colours.") plhlsrgb;
void
plhlsrgb(PLFLT h, PLFLT l, PLFLT s, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

%feature("autodoc", "Initialize PLplot, using preset or default options.") plinit;
void
plinit(void);

%feature("autodoc", "Draw a line segment from (x1, y1) to (x2, y2).") pljoin;
void
pljoin(PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2);

%feature("autodoc", "Label graphs.") pllab;
void
pllab(const char *xlabel, const char *ylabel, const char *tlabel);

%feature("autodoc", "Set position of the light source.") pllightsource;
void
pllightsource(PLFLT x, PLFLT y, PLFLT z);

%feature("autodoc", "Draw line segments connecting a series of points.") plline;
void
plline(PLINT n, PLFLT *Array, PLFLT *ArrayCk);

%feature("autodoc", "Draw a line in 3 space.") plline3;
void
plline3(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk);

%feature("autodoc", "Set line style.") pllsty;
void
pllsty(PLINT lin);

%feature("autodoc", "Plot a 3-d mesh representation of z[x][y].") plmesh;
void
plmesh(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
       PLINT nx, PLINT ny, PLINT opt);

%feature("autodoc", "Plot a 3-d contoured mesh representation of the function z[x][y].") plmeshc;
void
plmeshc(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
	 PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n);

%feature("autodoc", "Create a new stream and makes it the default.") plmkstrm;
void
plmkstrm(PLINT *OUTPUT);

%feature("autodoc", "Print \"text\" at specified position relative to viewport.") plmtex;
void
plmtex(const char *side, PLFLT disp, PLFLT pos, PLFLT just,
	 const char *text);

%feature("autodoc", "Print \"text\" at specified position relative to 3D viewport.") plmtex3;
void
plmtex3(const char *side, PLFLT disp, PLFLT pos, PLFLT just,
	 const char *text);

%feature("autodoc", "Plot a 3-d representation of the function z[x][y].") plot3d;
void
plot3d(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
	 PLINT nx, PLINT ny, PLINT opt, PLBOOL side);

%feature("autodoc", "Plot a 3-d contoured representation of the function z[x][y].") plot3dc;
void
plot3dc(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
	 PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n);

%feature("autodoc", "Plot a 3-d contoured representation of the function z[x][y] with limits on the y index.") plot3dcl;
void
plot3dcl(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
	 PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n,
	 PLINT ixstart, PLINT n, PLINT *Array, PLINT *ArrayCk);

%feature("autodoc", "Plot a 3-d contoured surface representation of the function z[x][y].") plsurf3d;
void
plsurf3d(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
	 PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n);

%feature("autodoc", "Plot a 3-d contoured surface representation of the function z[x][y] with limits on the y index.") plsurf3dl;
void
plsurf3dl(PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
	 PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n, 
	 PLINT ixstart, PLINT n, PLINT *Array, PLINT *ArrayCk);

%feature("autodoc", "Process options list using current options info.") plparseopts;
PLINT
plparseopts(int *p_argc, char **argv, PLINT mode);

%feature("autodoc", "Set fill pattern directly.") plpat;
void
plpat(PLINT n, PLINT *Array, PLINT *ArrayCk);

%feature("autodoc", "Plot array y against x for n points using ASCII code \"code\".") plpoin;
void
plpoin(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLINT code);

%feature("autodoc", "Draw a series of points in 3 space.") plpoin3;
void
plpoin3(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT code);

%feature("autodoc", "Draw a polygon in 3 space.") plpoly3;
void
plpoly3(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLBOOL *ArrayCkMinus1,
	    PLBOOL flag);

%feature("autodoc", "Set the floating point precision (in number of places) in numeric labels.") plprec;
void
plprec(PLINT setp, PLINT prec);

%feature("autodoc", "Set fill pattern, using one of the predefined patterns.") plpsty;
void
plpsty(PLINT patt);

%feature("autodoc", "Print \"text\" at world cooordinates (x,y).") plptex;
void
plptex(PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, const char *text);

%feature("autodoc", "Print \"text\" at world cooordinates (x,y,z).") plptex3;
void
plptex3(PLFLT x, PLFLT y, PLFLT z, PLFLT dx, PLFLT dy, PLFLT dz, PLFLT sx, PLFLT sy, PLFLT sz, PLFLT just, const char *text);

%feature("autodoc", "Replay contents of plot buffer to current device/file.") plreplot;
void
plreplot(void);

%feature("autodoc", "Transform from RGB to HLS form of colours.") plrgbhls;
void
plrgbhls(PLFLT r, PLFLT g, PLFLT b, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT);

%feature("autodoc", "Set character height.") plschr;
void
plschr(PLFLT def, PLFLT scale);

%feature("autodoc", "Set color map 0 colors by 8 bit RGB values.") plscmap0;
void
plscmap0(PLINT *Array, PLINT *ArrayCk, PLINT *ArrayCk, PLINT n);

%feature("autodoc", "Set number of colors in cmap 0.") plscmap0n;
void
plscmap0n(PLINT ncol0);

%feature("autodoc", "Set color map 1 colors by 8 bit RGB values.") plscmap1;
void
plscmap1(PLINT *Array, PLINT *ArrayCk, PLINT *ArrayCk, PLINT n);

%feature("autodoc", "Set color map 1 colors using a piece-wise linear relationship between intensity [0,1] (cmap 1 index) and position in HLS or
RGB color space.") plscmap1l;
void
plscmap1l(PLBOOL itype, PLINT n, PLFLT *Array,
	    PLFLT *ArrayCk, PLFLT *ArrayCk, PLFLT *ArrayCk,
	    PLBOOL *ArrayCkMinus1Null);

%feature("autodoc", "Set number of colors in cmap 1.") plscmap1n;
void
plscmap1n(PLINT ncol1);

%feature("autodoc", "Set 8-bit RGB value in cmap 0.") plscol0;
void
plscol0(PLINT icol0, PLINT r, PLINT g, PLINT b);

%feature("autodoc", "Set the background color using 8-bit RGB value.") plscolbg;
void
plscolbg(PLINT r, PLINT g, PLINT b);

%feature("autodoc", "Globally turn color output on/off.") plscolor;
void
plscolor(PLINT color);

%feature("autodoc", "Set the compression level.") plscompression;
void
plscompression(PLINT compression);

%feature("autodoc", "Set the device (keyword) name.") plsdev;
void
plsdev(const char *devname);

%feature("autodoc", "Set window into device space using margin, aspect ratio, and justification.") plsdidev;
void
plsdidev(PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy);

%feature("autodoc", "Set up transformation from metafile coordinates.") plsdimap;
void
plsdimap(PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax,
	   PLFLT dimxpmm, PLFLT dimypmm);

%feature("autodoc", "Set plot orientation, specifying rotation in units of pi/2.") plsdiori;
void
plsdiori(PLFLT rot);

%feature("autodoc", "Set window into plot space.") plsdiplt;
void
plsdiplt(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

%feature("autodoc", "Set window into plot space incrementally (zoom).") plsdiplz;
void
plsdiplz(PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax);

%feature("autodoc", "Set the escape character for text strings.") plsesc;
void
plsesc(char esc);

%feature("autodoc", "Process input strings, treating them as an option and argument pair. The first is for the external API, the second the work routine declared here for backward compatibilty.") plsetopt;
PLINT
plsetopt(char *opt, char *optarg);

%feature("autodoc", "Set family file parameters.") plsfam;
void
plsfam(PLINT fam, PLINT num, PLINT bmax);

%feature("autodoc", "Set FCI (font characterization integer).") plsfci;
void
plsfci(PLUNICODE fci);

%feature("autodoc", "Set the output file name.") plsfnam;
void
plsfnam(const char *fnam);

%feature("autodoc", "Shade regions with continuous range of colours.") plshades;
void
plshades( PLFLT **Matrix, PLINT nx, PLINT ny, defined_func df,
	  PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
	  PLFLT *Array, PLINT n, PLINT fill_width,
	  PLINT cont_color, PLINT cont_width,
	  fill_func ff, PLBOOL rectangular,
	  pltr_func pltr,
	  PLPointer SWIG_OBJECT_DATA);

%feature("autodoc", "Shade region with discrete colour, pattern fill.") plshade;
void
plshade(PLFLT **Matrix, PLINT nx, PLINT ny, defined_func df,
	  PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	  PLFLT shade_min, PLFLT shade_max,
	  PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	  PLINT min_color, PLINT min_width,
	  PLINT max_color, PLINT max_width,
	  fill_func ff, PLBOOL rectangular,
	  pltr_func pltr,
	  PLPointer SWIG_OBJECT_DATA);

%feature("autodoc", "Set up lengths of major tick marks.") plsmaj;
void
plsmaj(PLFLT def, PLFLT scale);

%feature("autodoc", "Set up lengths of minor tick marks.") plsmin;
void
plsmin(PLFLT def, PLFLT scale);

%feature("autodoc", "Set orientation.  Must be done before calling plinit.") plsori;
void
plsori(PLINT ori);

%feature("autodoc", "Set output device parameters.  Usually ignored by the driver.") plspage;
void
plspage(PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng,
	  PLINT xoff, PLINT yoff);

%feature("autodoc", "Set the pause (on end-of-page) status.") plspause;
void
plspause(PLBOOL pause);

%feature("autodoc", "Set stream number.") plsstrm;
void
plsstrm(PLINT strm);

%feature("autodoc", "Set the number of subwindows in x and y.") plssub;
void
plssub(PLINT nx, PLINT ny);

%feature("autodoc", "Set symbol height.") plssym;
void
plssym(PLFLT def, PLFLT scale);

%feature("autodoc", "Initialize PLplot, passing in the windows/page settings.") plstar;
void
plstar(PLINT nx, PLINT ny);

%feature("autodoc", "Initialize PLplot, passing the device name and windows/page settings.") plstart;
void
plstart(const char *devname, PLINT nx, PLINT ny);

%feature("autodoc", "Add a point to a stripchart.") plstripa;
void
plstripa(PLINT id, PLINT pen, PLFLT x, PLFLT y);

%feature("autodoc", "Create 1d stripchart.") plstripc;
void
plstripc(PLINT *OUTPUT, char *xspec, char *yspec,
	PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax,
	PLFLT xlpos, PLFLT ylpos,
	PLBOOL y_ascl, PLBOOL acc,
	PLINT colbox, PLINT collab,
	PLINT *Array, PLINT *ArrayCk, char *legline[4],
	char *labx, char *laby, char *labtop);

%feature("autodoc", "Deletes and releases memory used by a stripchart.") plstripd;
void
plstripd(PLINT id);

%feature("autodoc", "Set up a new line style.") plstyl;
void
plstyl(PLINT n, PLINT *Array, PLINT *ArrayCk);

%feature("autodoc", "Set vector arrow style.") plsvect;
void
plsvect(PLFLT *Array, PLFLT *ArrayCk, PLINT n, PLBOOL fill);

%feature("autodoc", "Set the edges of the viewport to the specified absolute coordinates.") plsvpa;
void
plsvpa(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

%feature("autodoc", "Set x axis labeling parameters.") plsxax;
void
plsxax(PLINT digmax, PLINT digits);

%feature("autodoc", "Set y axis labeling parameters.") plsyax;
void
plsyax(PLINT digmax, PLINT digits);

%feature("autodoc", "Plot array y against x for n points using Hershey symbol \"code\".") plsym;
void
plsym(PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLINT code);

%feature("autodoc", "Set z axis labeling parameters.") plszax;
void
plszax(PLINT digmax, PLINT digits);

%feature("autodoc", "Switch to text screen.") pltext;
void
pltext(void);

%feature("autodoc", "Sets the edges of the viewport with the given aspect ratio, leaving room for labels.") plvasp;
void
plvasp(PLFLT aspect);

%feature("autodoc", "Plot vectors.") plvect;
void
plvect(PLFLT **Matrix, PLFLT **MatrixCk, PLINT nx, PLINT ny, PLFLT scale,
	pltr_func pltr,
	PLPointer SWIG_OBJECT_DATA);

%feature("autodoc", "Create the largest viewport of the specified aspect ratio that fits within the specified normalized subpage coordinates.") plvpas;
void
plvpas(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect);

%feature("autodoc", "Create a viewport with the specified normalized subpage coordinates.") plvpor;
void
plvpor(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

%feature("autodoc", "Define a \"standard\" viewport with seven character heights for the left margin and four character heights everywhere else.") plvsta;
void
plvsta(void);

%feature("autodoc", "Set up a window for three-dimensional plotting.") plw3d;
void
plw3d(PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0,
	PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0,
	PLFLT zmax0, PLFLT alt, PLFLT az);

%feature("autodoc", "Set pen width.") plwid;
void
plwid(PLINT width);

%feature("autodoc", "Set up world coordinates of the viewport boundaries (2d plots).") plwind;
void
plwind(PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax);

%feature("autodoc", "Set xor mode; mode = 1-enter, 0-leave, status = 0 if not interactive device.") plxormod;
void
plxormod(PLBOOL mode, PLBOOL *OUTPUT);

#if 0

/* Deprecated functions that are in common API, but we don't want to
 * propagate them to the python API. */

%feature("autodoc", "Set line color by red, green, blue values in range from 0. to 1.") plrgb;
void
plrgb(PLFLT r, PLFLT g, PLFLT b);

%feature("autodoc", "Set line color by 8 bit RGB values.") plrgb1;
void
plrgb1(PLINT r, PLINT g, PLINT b);

void
plshade1(PLFLT *Matrix, PLINT nx, PLINT ny, defined_func df,
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 fill_func ff, PLBOOL rectangular,
	 pltr_func pltr,
	 PLPointer SWIG_OBJECT_DATA);

#endif

/*--------------------------------------------------------------------------*\
 *		Functions for use from C or C++ only
 *  N.B. If you want these in python, they should be officially put in
 *  the common API for all front-ends to the PLplot library with "c_" suffix,
 *  DocBook xml documentation in the api.xml chapter, etc.
\*--------------------------------------------------------------------------*/

#if 0

/* Draws a contour plot using the function evaluator f2eval and data stored
 * by way of the f2eval_data pointer.  This allows arbitrary organizations
 * of 2d array data to be used.
 */
void
plfcont(f2eval_func f2eval,
	PLPointer SWIG_OBJECT_DATA,
	PLINT nx, PLINT ny, PLINT kx, PLINT lx,
	PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
	pltr_func pltr,
	PLPointer SWIG_OBJECT_DATA);
/* plot continental outline in world coordinates */

void
plmap( void (*mapform)(PLINT, PLFLT *, PLFLT *), char *type,
         PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat );

/* Plot the latitudes and longitudes on the background. */

void
plmeridians( void (*mapform)(PLINT, PLFLT *, PLFLT *),
               PLFLT dlong, PLFLT dlat,
               PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat );

void
plfshade(f2eval_func,
	 PLPointer SWIG_OBJECT_DATA,
	 c2eval_func,
	 PLPointer c2eval_data,
	 PLINT nx, PLINT ny,
	 PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
	 PLFLT shade_min, PLFLT shade_max,
	 PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
	 PLINT min_color, PLINT min_width,
	 PLINT max_color, PLINT max_width,
	 fill_func, PLBOOL rectangular,
	 pltr_func,
	 PLPointer SWIG_OBJECT_DATA);

/* Converts input values from relative device coordinates to relative plot */
/* coordinates. */

void
pldid2pc(PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT);

/* Converts input values from relative plot coordinates to relative */
/* device coordinates. */

void
pldip2dc(PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT);
#endif

/* plots a 2d image (or a matrix too large for plshade() ). */

void
plimage( PLFLT **Matrix, PLINT nx, PLINT ny,
	 PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
	 PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax);

#ifdef 0
/* Returns a list of file-oriented device names and their menu strings */
void
plgFileDevs(char ***p_menustr, char ***p_devname, PLINT *p_ndev);

/* Returns a list of all device names and their menu strings */

void
plgDevs(char ***p_menustr, char ***p_devname, PLINT *p_ndev);

/* Set the function pointer for the keyboard event handler */

void
plsKeyEH(void (*KeyEH) (PLGraphicsIn *, void *, PLINT *), void *KeyEH_data);

/* Set the function pointer for the (mouse) button event handler */

void
plsButtonEH(void (*ButtonEH) (PLGraphicsIn *, void *, PLINT *),
	    void *ButtonEH_data);
#endif
/* Set the variables to be used for storing error info */

#if 0
/* Cannot get this to work since plsError is not simply an output
 * of an internal integer and character string. */
%feature("autodoc", "Set the variables to be used for storing error info.") plsError;
void
plsError(PLINT *OUTPUT, char *OUTPUT);
#endif

/* Sets an optional user exit handler. */
#if 0
void
plsexit(PLINT (*handler) (char *));
	/* Transformation routines */
#endif

#if 0
/* Just like pltr2() but uses pointer arithmetic to get coordinates from */
/* 2d grid tables.  */

void
pltr2p(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data);
/* Identity transformation for plots from Fortran. */

void
pltr0f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

/* Does linear interpolation from doubly dimensioned coord arrays */
/* (row dominant, i.e. Fortran ordering). */

void
pltr2f(PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data);

/* Example linear transformation function for contour plotter. */

void
xform(PLFLT x, PLFLT y, PLFLT *OUTPUT, PLFLT *OUTPUT);
	/* Function evaluators */
/* Does a lookup from a 2d function array.  Array is of type (PLFLT **), */
/* and is column dominant (normal C ordering). */

PLFLT
plf2eval2(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is column dominant (normal C ordering). */

PLFLT
plf2eval(PLINT ix, PLINT iy, PLPointer plf2eval_data);

/* Does a lookup from a 2d function array.  Array is of type (PLFLT *), */
/* and is row dominant (Fortran ordering). */

PLFLT
plf2evalr(PLINT ix, PLINT iy, PLPointer plf2eval_data);
#endif
	/* Command line parsing utilities */

/* Clear internal option table info structure. */

void
plClearOpts(void);

/* Reset internal option table info structure. */

void
plResetOpts(void);

/* Merge user option table into internal info structure. */
#if 0
PLINT
plMergeOpts(PLOptionTable *options, char *name, char **notes);
#endif
/* Set the strings used in usage and syntax messages. */

void
plSetUsage(char *program_string, char *usage_string);

#if 0
/* This is wrapped by common API plsetopt so ignore. */
PLINT
plSetOpt(char *opt, char *optarg);
#endif

/* Print usage & syntax message. */

void
plOptUsage(void);

	/* Miscellaneous */
#if 0
/* Set the output file pointer */

void
plgfile(FILE **p_file);

/* Get the output file pointer */

void
plsfile(FILE *file);

/* Get the escape character for text strings. */

void
plgesc(char *p_esc);

/* Front-end to driver escape function. */

void
pl_cmd(PLINT op, void *ptr);

/* Return full pathname for given file if executable */

PLINT
plFindName(char *p);

/* Looks for the specified executable file according to usual search path. */

char *
plFindCommand(char *fn);

/* Gets search name for file by concatenating the dir, subdir, and file */
/* name, allocating memory as needed.  */

void
plGetName(char *dir, char *subdir, char *filename, char **filespec);

/* Prompts human to input an integer in response to given message. */

PLINT
plGetInt(char *s);

/* Prompts human to input a float in response to given message. */

PLFLT
plGetFlt(char *s);

	/* Nice way to allocate space for a vectored 2d grid */

/* Allocates a block of memory for use as a 2-d grid of PLFLT's.  */

void
plAlloc2dGrid(PLFLT ***f, PLINT nx, PLINT ny);

/* Frees a block of memory allocated with plAlloc2dGrid(). */

void
plFree2dGrid(PLFLT **f, PLINT nx, PLINT ny);

/* Find the maximum and minimum of a 2d matrix allocated with plAllc2dGrid(). */

#endif

void
plMinMax2dGrid(PLFLT **Matrix, PLINT nx, PLINT ny, PLFLT *OUTPUT, PLFLT *OUTPUT);

/* Wait for graphics input event and translate to world coordinates */

%feature("autodoc", "Wait for graphics input event and translate to world coordinates.") plGetCursor;
PLINT
plGetCursor(PLGraphicsIn *gin);

/* Translates relative device coordinates to world coordinates.  */

#if 0
/* Use plcalc_world instead of plTranslateCursor. */
int
plTranslateCursor(PLGraphicsIn *gin);
#endif
