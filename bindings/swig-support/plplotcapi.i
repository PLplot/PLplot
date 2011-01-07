// plplotcapi.i contains the common API (API used by the many computer
// language interfaces for the PLplot library) of PLplot in a form that
// is readily understood by swig.
//
//
//
//Copyright (C) 2002  Gary Bishop
//Copyright (C) 2002-2010  Alan W. Irwin
//Copyright (C) 2004  Rafael Laboissiere
//Copyright (C) 2004  Andrew Ross
//
//This file is part of PLplot.
//
//PLplot is free software; you can redistribute it and/or modify
//it under the terms of the GNU Library General Public License as published by
//the Free Software Foundation; version 2 of the License.
//
//PLplot is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Library General Public License for more details.
//
//You should have received a copy of the GNU Library General Public License
//along with the file PLplot; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//

// For passing user data, as with X's XtPointer

typedef void*  PLPointer;

//--------------------------------------------------------------------------
// Complex data types and other good stuff
//--------------------------------------------------------------------------

// Switches for escape function call.
// Some of these are obsolete but are retained in order to process
//   old metafiles

#define PLESC_SET_RGB            1      // obsolete
#define PLESC_ALLOC_NCOL         2      // obsolete
#define PLESC_SET_LPB            3      // obsolete
#define PLESC_EXPOSE             4      // handle window expose
#define PLESC_RESIZE             5      // handle window resize
#define PLESC_REDRAW             6      // handle window redraw
#define PLESC_TEXT               7      // switch to text screen
#define PLESC_GRAPH              8      // switch to graphics screen
#define PLESC_FILL               9      // fill polygon
#define PLESC_DI                 10     // handle DI command
#define PLESC_FLUSH              11     // flush output
#define PLESC_EH                 12     // handle Window events
#define PLESC_GETC               13     // get cursor position
#define PLESC_SWIN               14     // set window parameters
#define PLESC_PLFLTBUFFERING     15     // configure PLFLT buffering
#define PLESC_XORMOD             16     // set xor mode
#define PLESC_SET_COMPRESSION    17     // AFR: set compression
#define PLESC_CLEAR              18     // RL: clear graphics region
#define PLESC_DASH               19     // RL: draw dashed line
#define PLESC_HAS_TEXT           20     // driver draws text
#define PLESC_IMAGE              21     // handle image
#define PLESC_IMAGEOPS           22     // plimage related operations

// image operations
#if 0
#define ZEROW2B    1
#define ZEROW2D    2
#define ONEW2B     3
#define ONEW2D     4
#endif

// definitions for the opt argument in plsurf3d()

#define DRAW_LINEX         0x01          // draw lines parallel to the X axis
#define DRAW_LINEY         0x02          // draw lines parallel to the Y axis
#define DRAW_LINEXY        0x03          // draw lines parallel to both the X and Y axes
#define MAG_COLOR          0x04          // draw the mesh with a color dependent of the magnitude
#define BASE_CONT          0x08          // draw contour plot at bottom xy plane
#define TOP_CONT           0x10          // draw contour plot at top xy plane
#define SURF_CONT          0x20          // draw contour plot at surface
#define DRAW_SIDES         0x40          // draw sides
#define FACETED            0x80          // draw outline for each square that makes up the surface
#define MESH               0x100         // draw mesh

// Flags for plbin() - opt argument
#define PL_BIN_DEFAULT     0
#define PL_BIN_CENTRED     1
#define PL_BIN_NOEXPAND    2
#define PL_BIN_NOEMPTY     4

// Flags for plhist() - opt argument; note: some flags are passed to
//   plbin() for the actual plotting
#define PL_HIST_DEFAULT            0
#define PL_HIST_NOSCALING          1
#define PL_HIST_IGNORE_OUTLIERS    2
#define PL_HIST_NOEXPAND           8
#define PL_HIST_NOEMPTY            16

// Flags for pllegend.
#define PL_LEGEND_NONE            1
#define PL_LEGEND_COLOR_BOX       2
#define PL_LEGEND_LINE            4
#define PL_LEGEND_SYMBOL          8
#define PL_LEGEND_TEXT_LEFT       16
#define PL_LEGEND_BACKGROUND      32
#define PL_LEGEND_BOUNDING_BOX    64
#define PL_LEGEND_ROW_MAJOR       128
#define PL_LEGEND_LEFT            256
#define PL_LEGEND_RIGHT           512
#define PL_LEGEND_UPPER           1024
#define PL_LEGEND_LOWER           2048
#define PL_LEGEND_INSIDE          4096
#define PL_LEGEND_OUTSIDE         8192

// Flags for plcolorbar
#define PL_COLORBAR_LEFT            1
#define PL_COLORBAR_RIGHT           2
#define PL_COLORBAR_UPPER           4
#define PL_COLORBAR_LOWER           8
#define PL_COLORBAR_LABEL_LEFT      16
#define PL_COLORBAR_LABEL_RIGHT     32
#define PL_COLORBAR_LABEL_UPPER     64
#define PL_COLORBAR_LABEL_LOWER     128
#define PL_COLORBAR_IMAGE           256
#define PL_COLORBAR_SHADE           512
#define PL_COLORBAR_GRADIENT        1024
#define PL_COLORBAR_CAP_LOW         2048
#define PL_COLORBAR_CAP_HIGH        4096
#define PL_COLORBAR_SHADE_LABEL     8192

// Window parameter tags

#define PLSWIN_DEVICE    1              // device coordinates
#define PLSWIN_WORLD     2              // world coordinates

// Axis label tags
#define PL_X_AXIS        1              // The x-axis
#define PL_Y_AXIS        2              // The y-axis
#define PL_Z_AXIS        3              // The z-axis

// PLplot Option table & support constants

// Option-specific settings

#define PL_OPT_ENABLED      0x0001      // Obsolete
#define PL_OPT_ARG          0x0002      // Option has an argment
#define PL_OPT_NODELETE     0x0004      // Don't delete after processing
#define PL_OPT_INVISIBLE    0x0008      // Make invisible
#define PL_OPT_DISABLED     0x0010      // Processing is disabled

// Option-processing settings -- mutually exclusive

#define PL_OPT_FUNC      0x0100         // Call handler function
#define PL_OPT_BOOL      0x0200         // Set *var = 1
#define PL_OPT_INT       0x0400         // Set *var = atoi(optarg)
#define PL_OPT_FLOAT     0x0800         // Set *var = atof(optarg)
#define PL_OPT_STRING    0x1000         // Set var = optarg

// Global mode settings
// These override per-option settings

#define PL_PARSE_PARTIAL              0x0000 // For backward compatibility
#define PL_PARSE_FULL                 0x0001 // Process fully & exit if error
#define PL_PARSE_QUIET                0x0002 // Don't issue messages
#define PL_PARSE_NODELETE             0x0004 // Don't delete options after
                                             // processing
#define PL_PARSE_SHOWALL              0x0008 // Show invisible options
#define PL_PARSE_OVERRIDE             0x0010 // Obsolete
#define PL_PARSE_NOPROGRAM            0x0020 // Program name NOT in *argv[0]..
#define PL_PARSE_NODASH               0x0040 // Set if leading dash NOT required
#define PL_PARSE_SKIP                 0x0080 // Skip over unrecognized args

// FCI (font characterization integer) related constants.
#define PL_FCI_MARK                   0x80000000
#define PL_FCI_IMPOSSIBLE             0x00000000
#define PL_FCI_HEXDIGIT_MASK          0xf
#define PL_FCI_HEXPOWER_MASK          0x7
#define PL_FCI_HEXPOWER_IMPOSSIBLE    0xf
// These define hexpower values corresponding to each font attribute.
#define PL_FCI_FAMILY                 0x0
#define PL_FCI_STYLE                  0x1
#define PL_FCI_WEIGHT                 0x2
// These are legal values for font family attribute
#define PL_FCI_SANS                   0x0
#define PL_FCI_SERIF                  0x1
#define PL_FCI_MONO                   0x2
#define PL_FCI_SCRIPT                 0x3
#define PL_FCI_SYMBOL                 0x4
// These are legal values for font style attribute
#define PL_FCI_UPRIGHT                0x0
#define PL_FCI_ITALIC                 0x1
#define PL_FCI_OBLIQUE                0x2
// These are legal values for font weight attribute
#define PL_FCI_MEDIUM                 0x0
#define PL_FCI_BOLD                   0x1

#define PL_MAXKEY                     16

typedef struct
{
    int          type;              // of event (CURRENTLY UNUSED)
    unsigned int state;             // key or button mask
    unsigned int keysym;            // key selected
    unsigned int button;            // mouse button selected
    PLINT        subwindow;         // subwindow (alias subpage, alias subplot) number
    char         string[PL_MAXKEY]; // translated string
    int          pX, pY;            // absolute device coordinates of pointer
    PLFLT        dX, dY;            // relative device coordinates of pointer
    PLFLT        wX, wY;            // world coordinates of pointer
} PLGraphicsIn;


// Structure for describing the plot window

#define PL_MAXWINDOWS    64     // Max number of windows/page tracked

// Macro used (in some cases) to ignore value of argument
// I don't plan on changing the value so you can hard-code it

#define PL_NOTSET                     ( -42 )


#define PLESPLFLTBUFFERING_ENABLE     1
#define PLESPLFLTBUFFERING_DISABLE    2
#define PLESPLFLTBUFFERING_QUERY      3

#define GRID_CSA                      1 // Bivariate Cubic Spline approximation
#define GRID_DTLI                     2 // Delaunay Triangulation Linear Interpolation
#define GRID_NNI                      3 // Natural Neighbors Interpolation
#define GRID_NNIDW                    4 // Nearest Neighbors Inverse Distance Weighted
#define GRID_NNLI                     5 // Nearest Neighbors Linear Interpolation
#define GRID_NNAIDW                   6 // Nearest Neighbors Around Inverse Distance Weighted


#ifdef SWIG_PYTHON
#define SWIG_OBJECT_DATA        PYOBJECT_DATA
#define SWIG_OBJECT_DATA_img    PYOBJECT_DATA_img
#define pltr_img                pltr
#else
#define SWIG_OBJECT_DATA        OBJECT_DATA
#define SWIG_OBJECT_DATA_img    OBJECT_DATA_img
#endif

%feature( "autodoc", "1" );

#ifdef SWIG_PYTHON

// Non-common API that are included here because they traditionally
// were part of plmodule.c.

#if 0
// Deprecated function that we no longer want to propagate to the
// python API.

%feature( "docstring", "Plot an arrow." ) plarrows;
void
plarrows( PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT n,
          PLFLT scale, PLFLT dx, PLFLT dy );

#endif

%feature( "docstring", "Set inferior X window." ) plsxwin;
void
plsxwin( PLINT window_id );

#endif // SWIG_PYTHON

// Complete list of common API (has "c_" suffix version defined in plplot.h)

%feature( "docstring", "Set the format of the contour labels." )
pl_setcontlabelformat;
void
pl_setcontlabelformat( PLINT lexp, PLINT sigdig );

%feature( "docstring", "Set offset and spacing of contour labels." ) pl_setcontlabelparam;
void
pl_setcontlabelparam( PLFLT offset, PLFLT size, PLFLT spacing, PLINT active );

%feature( "docstring", "Advance to subpage \"page\", or to the next one if \"page\" = 0." ) pladv;
void
pladv( PLINT page );

%feature( "docstring", "Plot an arc" ) plarc;
void
plarc( PLFLT x, PLFLT y, PLFLT a, PLFLT b, PLFLT angle1, PLFLT angle2,
       PLBOOL fill );

%feature( "docstring", "This functions similarly to plbox() except that the origin of the axes is placed at the user-specified point (x0, y0)." ) plaxes;
void
plaxes( PLFLT x0, PLFLT y0, const char *xopt, PLFLT xtick, PLINT nxsub,
        const char *yopt, PLFLT ytick, PLINT nysub );

%feature( "docstring", "Plot a histogram using x to store data values and y to store frequencies." ) plbin;
void
plbin( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLINT center );

%feature( "docstring", "Calculate broken-down time from continuous time for current stream." ) plbtime;
void
plbtime( PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT, PLFLT *OUTPUT, PLFLT ctime );

%feature( "docstring", "Start new page.  Should only be used with pleop()." ) plbop;
void
plbop( void );

%feature( "docstring", "Draw a box around the current viewport." ) plbox;
void
plbox( const char *xopt, PLFLT xtick, PLINT nxsub,
       const char *yopt, PLFLT ytick, PLINT nysub );

%feature( "docstring", "This is the 3-d analogue of plbox()." ) plbox3;
void
plbox3( const char *xopt, const char *xlabel, PLFLT xtick, PLINT nsubx,
        const char *yopt, const char *ylabel, PLFLT ytick, PLINT nsuby,
        const char *zopt, const char *zlabel, PLFLT ztick, PLINT nsubz );

%feature( "docstring", "Calculate world coordinates and subpage from relative device coordinates." ) plcalc_world;
void
plcalc_world( PLFLT rx, PLFLT ry, PLFLT *OUTPUT, PLFLT *OUTPUT, PLINT *OUTPUT );

%feature( "docstring", "Clear current subpage." ) plclear;
void
plclear( void );

%feature( "docstring", "Set color, map 0.  Argument is integer between 0 and 15." ) plcol0;
void
plcol0( PLINT icol0 );

%feature( "docstring", "Set color, map 1.  Argument is a float between 0. and 1." ) plcol1;
void
plcol1( PLFLT col1 );

%feature( "docstring", "Configure transformation between continuous and broken-down time (and vice versa) for current stream." ) plconfigtime;
void
plconfigtime( PLFLT scale, PLFLT offset1, PLFLT offset2, PLINT ccontrol, PLBOOL ifbtime_offset, PLINT year, PLINT month, PLINT day, PLINT hour, PLINT min, PLFLT sec );

%feature( "docstring", "Draw a contour plot." ) plcont;
void
plcont( PLFLT **Matrix, PLINT nx, PLINT ny, PLINT kx, PLINT lx,
        PLINT ky, PLINT ly, PLFLT *Array, PLINT n,
        pltr_func pltr,
        PLPointer SWIG_OBJECT_DATA );


%feature( "docstring", "Calculate continuous time from broken-down time for current stream." ) plctime;
void
plctime( PLINT year, PLINT month, PLINT day, PLINT hour, PLINT min, PLFLT sec, PLFLT *OUTPUT );

%feature( "docstring", "Copy state parameters from the reference stream to the current stream." ) plcpstrm;
void
plcpstrm( PLINT iplsr, PLBOOL flags );

%feature( "docstring", "End a plotting session for all open streams." ) plend;
void
plend( void );

%feature( "docstring", "End a plotting session for the current stream only." ) plend1;
void
plend1( void );

%feature( "docstring", "Simple interface for defining viewport and window." ) plenv;
void
plenv( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
       PLINT just, PLINT axis );

%feature( "docstring", "Simple interface for defining viewport and window." ) plenv0;
void
plenv0( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
        PLINT just, PLINT axis );

%feature( "docstring", "End current page.  Should only be used with plbop()." ) pleop;
void
pleop( void );

%feature( "docstring", "Plot horizontal error bars (xmin(i),y(i)) to (xmax(i),y(i))." ) plerrx;
void
plerrx( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk );

%feature( "docstring", "Plot vertical error bars (x,ymin(i)) to (x(i),ymax(i))." ) plerry;
void
plerry( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk );

%feature( "docstring", "Advance to the next family file on the next new page." ) plfamadv;
void
plfamadv( void );

%feature( "docstring", "Pattern fills the polygon bounded by the input points." ) plfill;
void
plfill( PLINT n, PLFLT *Array, PLFLT *ArrayCk );

%feature( "docstring", "Pattern fills the 3d polygon bounded by the input points." ) plfill3;
void
plfill3( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk );

%feature( "docstring", "Renders linear gradient for the polygon bounded by the input points." ) plgradient;
void
plgradient( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT angle );

%feature( "docstring", "Flush the output stream.  Use sparingly, if at all." ) plflush;
void
plflush( void );

%feature( "docstring", "Set the global font flag to 'ifont'." ) plfont;
void
plfont( PLINT ifont );

%feature( "docstring", "Load specified font set." ) plfontld;
void
plfontld( PLINT fnt );

%feature( "docstring", "Get character default height and current (scaled) height." ) plgchr;
void
plgchr( PLFLT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Get 8 bit RGB values for given color from color map 0." ) plgcol0;
void
plgcol0( PLINT icol0, PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT );

%feature( "docstring", "Get 8 bit RGB + alpha values for given color from color map 0." ) plgcol0a;
void
plgcol0a( PLINT icol0, PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Get 8-bit RGB background color." ) plgcolbg;
void
plgcolbg( PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT );

%feature( "docstring", "Get 8-bit RGB + alpha background color." ) plgcolbga;
void
plgcolbga( PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Get the current compression setting." ) plgcompression;
void
plgcompression( PLINT *OUTPUT );

%feature( "docstring", "Get the current device (keyword) name." ) plgdev;
void
plgdev( char *OUTPUT );

%feature( "docstring", "Retrieve current window into device space." ) plgdidev;
void
plgdidev( PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Get plot orientation." ) plgdiori;
void
plgdiori( PLFLT *OUTPUT );

%feature( "docstring", "Retrieve current window into plot space." ) plgdiplt;
void
plgdiplt( PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Get family file parameters." ) plgfam;
void
plgfam( PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT );

%feature( "docstring", "Get FCI (font characterization integer)." ) plgfci;
void
plgfci( PLUNICODE *OUTPUT );

%feature( "docstring", "Get the (current) output file name." ) plgfnam;
void
plgfnam( char *OUTPUT );

%feature( "docstring", "Get the current font family, style and weight." ) plgfont;
void
plgfont( PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT );

%feature( "docstring", "Get the (current) run level." ) plglevel;
void
plglevel( PLINT *OUTPUT );

%feature( "docstring", "Get output device parameters." ) plgpage;
void
plgpage( PLFLT *OUTPUT, PLFLT *OUTPUT,
         PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT, PLINT *OUTPUT );

%feature( "docstring", "Switch to graphics screen." ) plgra;
void
plgra( void );

%feature( "docstring", "Grid irregular sample data." ) plgriddata;
void
plgriddata( PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT n,
            PLFLT *ArrayX, PLINT nx, PLFLT *ArrayY, PLINT ny,
            PLFLT **OutMatrixCk, PLINT type, PLFLT data );


%feature( "docstring", "Get subpage boundaries in absolute coordinates." ) plgspa;
void
plgspa( PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Get current stream number." ) plgstrm;
void
plgstrm( PLINT *OUTPUT );

%feature( "docstring", "Get current library version number." ) plgver;
void
plgver( char *OUTPUT );

%feature( "docstring", "Get viewport boundaries in normalized device coordinates." ) plgvpd;
void
plgvpd( PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Get viewport boundaries in world coordinates." ) plgvpw;
void
plgvpw( PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Get x axis labeling parameters." ) plgxax;
void
plgxax( PLINT *OUTPUT, PLINT *OUTPUT );

%feature( "docstring", "Get y axis labeling parameters." ) plgyax;
void
plgyax( PLINT *OUTPUT, PLINT *OUTPUT );

%feature( "docstring", "Get z axis labeling parameters." ) plgzax;
void
plgzax( PLINT *OUTPUT, PLINT *OUTPUT );

%feature( "docstring", "Draw histogram." ) plhist;
void
plhist( PLINT n, PLFLT *Array, PLFLT datmin, PLFLT datmax,
        PLINT nbin, PLINT oldwin );

#if 0
// Deprecated function that we no longer want to propagate to the
// python API.

%feature( "docstring", "Set current color (map 0) by hue, lightness, and saturation." ) plhls;
void
plhls( PLFLT h, PLFLT l, PLFLT s );
#endif

%feature( "docstring", "Transform from HLS to RGB form of colours." ) plhlsrgb;
void
plhlsrgb( PLFLT h, PLFLT l, PLFLT s, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Initialize PLplot, using preset or default options." ) plinit;
void
plinit( void );

%feature( "docstring", "Draw a line segment from (x1, y1) to (x2, y2)." ) pljoin;
void
pljoin( PLFLT x1, PLFLT y1, PLFLT x2, PLFLT y2 );

%feature( "docstring", "Label graphs." ) pllab;
void
pllab( const char *xlabel, const char *ylabel, const char *tlabel );

%feature( "docstring", "Create a discrete annotated legend using boxes, lines, and/or lines of symbols." ) pllegend;
void
pllegend( PLFLT *OUTPUT, PLFLT *OUTPUT,
            PLINT opt, PLFLT x, PLFLT y, PLFLT plot_width,
            PLINT bg_color, PLINT bb_color, PLINT bb_style,
            PLINT nrow, PLINT ncolumn,
            PLINT n, const PLINT *Array,
            PLFLT text_offset, PLFLT text_scale, PLFLT text_spacing,
            PLFLT text_justification,
            const PLINT *ArrayCk, const char **ArrayCk,
            const PLINT *ArrayCk, const PLINT *ArrayCk,
            const PLFLT *ArrayCk, const PLINT *ArrayCk,
            const PLINT *ArrayCk, const PLINT *ArrayCk,
            const PLINT *ArrayCk,
            const PLINT *ArrayCk, const PLFLT *ArrayCk,
            const PLINT *ArrayCk, const char **ArrayCk );

%feature( "docstring", "Create a continous colorbar using an image, shade, or gradient with an optional axis label." ) plcolorbar;
void
plcolorbar( PLINT opt, PLFLT x, PLFLT y, PLFLT length, PLFLT width,
              PLFLT ticks, PLINT sub_ticks,
              const char *axis_opts, const char *label,
              PLINT n, PLFLT *Array, PLFLT *ArrayCk );

%feature( "docstring", "Set position of the light source." ) pllightsource;
void
pllightsource( PLFLT x, PLFLT y, PLFLT z );

%feature( "docstring", "Draw line segments connecting a series of points." ) plline;
void
plline( PLINT n, PLFLT *Array, PLFLT *ArrayCk );

%feature( "docstring", "Draw a line in 3 space." ) plline3;
void
plline3( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk );

%feature( "docstring", "Set line style." ) pllsty;
void
pllsty( PLINT lin );

%feature( "docstring", "Plot a 3-d mesh representation of z[x][y]." ) plmesh;
void
plmesh( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
        PLINT nx, PLINT ny, PLINT opt );

%feature( "docstring", "Plot a 3-d contoured mesh representation of the function z[x][y]." ) plmeshc;
void
plmeshc( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
         PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n );

%feature( "docstring", "Create a new stream and makes it the default." ) plmkstrm;
void
plmkstrm( PLINT *OUTPUT );

%feature( "docstring", "Print \"text\" at specified position relative to viewport." ) plmtex;
void
plmtex( const char *side, PLFLT disp, PLFLT pos, PLFLT just,
        const char *text );

%feature( "docstring", "Print \"text\" at specified position relative to 3D viewport." ) plmtex3;
void
plmtex3( const char *side, PLFLT disp, PLFLT pos, PLFLT just,
         const char *text );

%feature( "docstring", "Plot a 3-d representation of the function z[x][y]." ) plot3d;
void
plot3d( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
        PLINT nx, PLINT ny, PLINT opt, PLBOOL side );

%feature( "docstring", "Plot a 3-d contoured representation of the function z[x][y]." ) plot3dc;
void
plot3dc( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
         PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n );

%feature( "docstring", "Plot a 3-d contoured representation of the function z[x][y] with limits on the y index." ) plot3dcl;
void
plot3dcl( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
          PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n,
          PLINT ixstart, PLINT n, PLINT *Array, PLINT *ArrayCk );

%feature( "docstring", "Plot a 3-d contoured surface representation of the function z[x][y]." ) plsurf3d;
void
plsurf3d( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
          PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n );

%feature( "docstring", "Plot a 3-d contoured surface representation of the function z[x][y] with limits on the y index." ) plsurf3dl;
void
plsurf3dl( PLFLT *ArrayX, PLFLT *ArrayY, PLFLT **MatrixCk,
           PLINT nx, PLINT ny, PLINT opt, PLFLT *Array, PLINT n,
           PLINT ixstart, PLINT n, PLINT *Array, PLINT *ArrayCk );

%feature( "docstring", "Process options list using current options info." ) plparseopts;
PLINT
plparseopts( int *p_argc, const char **argv, PLINT mode );

%feature( "docstring", "Set fill pattern directly." ) plpat;
void
plpat( PLINT n, PLINT *Array, PLINT *ArrayCk );

%feature( "docstring", "Plot array y against x for n points using ASCII code \"code\"." ) plpoin;
void
plpoin( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLINT code );

%feature( "docstring", "Draw a series of points in 3 space." ) plpoin3;
void
plpoin3( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLINT code );

%feature( "docstring", "Draw a polygon in 3 space." ) plpoly3;
void
plpoly3( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, PLBOOL *ArrayCkMinus1,
         PLBOOL flag );

%feature( "docstring", "Set the floating point precision (in number of places) in numeric labels." ) plprec;
void
plprec( PLINT setp, PLINT prec );

%feature( "docstring", "Set fill pattern, using one of the predefined patterns." ) plpsty;
void
plpsty( PLINT patt );

%feature( "docstring", "Print \"text\" at world cooordinates (x,y)." ) plptex;
void
plptex( PLFLT x, PLFLT y, PLFLT dx, PLFLT dy, PLFLT just, const char *text );

%feature( "docstring", "Print \"text\" at world cooordinates (x,y,z)." ) plptex3;
void
plptex3( PLFLT x, PLFLT y, PLFLT z, PLFLT dx, PLFLT dy, PLFLT dz, PLFLT sx, PLFLT sy, PLFLT sz, PLFLT just, const char *text );

%feature( "docstring", "Return a random number in the range [0,1]." ) plrandd;
PLFLT
plrandd();

%feature( "docstring", "Replay contents of plot buffer to current device/file." ) plreplot;
void
plreplot( void );

%feature( "docstring", "Transform from RGB to HLS form of colours." ) plrgbhls;
void
plrgbhls( PLFLT r, PLFLT g, PLFLT b, PLFLT *OUTPUT, PLFLT *OUTPUT, PLFLT *OUTPUT );

%feature( "docstring", "Set character height." ) plschr;
void
plschr( PLFLT def, PLFLT scale );

%feature( "docstring", "Set color map 0 colors by 8 bit RGB values." ) plscmap0;
void
plscmap0( PLINT *Array, PLINT *ArrayCk, PLINT *ArrayCk, PLINT n );

%feature( "docstring", "Set color map 0 colors by 8 bit RGB + alpha values." ) plscmap0a;
void
plscmap0a( PLINT *Array, PLINT *ArrayCk, PLINT *ArrayCk, PLFLT *ArrayCk, PLINT n );

%feature( "docstring", "Set number of colors in cmap 0." ) plscmap0n;
void
plscmap0n( PLINT ncol0 );

%feature( "docstring", "Set color map 1 colors by 8 bit RGB values." ) plscmap1;
void
plscmap1( PLINT *Array, PLINT *ArrayCk, PLINT *ArrayCk, PLINT n );

%feature( "docstring", "Set color map 1 colors by 8 bit RGB + alpha values." ) plscmap1a;
void
plscmap1a( PLINT *Array, PLINT *ArrayCk, PLINT *ArrayCk, PLFLT *ArrayCk, PLINT n );

%feature( "docstring", "Set color map 1 colors using a piece-wise linear relationship between intensity [0,1] (cmap 1 index) and position in HLS or
RGB color space." ) plscmap1l;
void
plscmap1l( PLBOOL itype, PLINT n, PLFLT *Array,
           PLFLT *ArrayCk, PLFLT *ArrayCk, PLFLT *ArrayCk,
           PLBOOL *ArrayCkMinus1Null );

%feature( "docstring", "Set color map 1 colors using a piece-wise linear relationship between intensity [0,1] (cmap 1 index) and position in HLS or
RGB color space + alpha value." ) plscmap1la;
void
plscmap1la( PLBOOL itype, PLINT n, PLFLT *Array,
            PLFLT *ArrayCk, PLFLT *ArrayCk, PLFLT *ArrayCk, PLFLT *ArrayCk,
            PLBOOL *ArrayCkMinus1Null );

%feature( "docstring", "Set number of colors in cmap 1." ) plscmap1n;
void
plscmap1n( PLINT ncol1 );

%feature( "docstring", "Set 8-bit RGB value in cmap 0." ) plscol0;
void
plscol0( PLINT icol0, PLINT r, PLINT g, PLINT b );

%feature( "docstring", "Set 8-bit RGB + alpha value in cmap 0." ) plscol0a;
void
plscol0a( PLINT icol0, PLINT r, PLINT g, PLINT b, PLFLT a );

%feature( "docstring", "Set the background color using 8-bit RGB value." ) plscolbg;
void
plscolbg( PLINT r, PLINT g, PLINT b );

%feature( "docstring", "Set the background color using 8-bit RGB + alpha value." ) plscolbga;
void
plscolbga( PLINT r, PLINT g, PLINT b, PLFLT a );

%feature( "docstring", "Globally turn color output on/off." ) plscolor;
void
plscolor( PLINT color );

%feature( "docstring", "Set the compression level." ) plscompression;
void
plscompression( PLINT compression );

%feature( "docstring", "Set the device (keyword) name." ) plsdev;
void
plsdev( const char *devname );

%feature( "docstring", "Set window into device space using margin, aspect ratio, and justification." ) plsdidev;
void
plsdidev( PLFLT mar, PLFLT aspect, PLFLT jx, PLFLT jy );

%feature( "docstring", "Set up transformation from metafile coordinates." ) plsdimap;
void
plsdimap( PLINT dimxmin, PLINT dimxmax, PLINT dimymin, PLINT dimymax,
          PLFLT dimxpmm, PLFLT dimypmm );

%feature( "docstring", "Set plot orientation, specifying rotation in units of pi/2." ) plsdiori;
void
plsdiori( PLFLT rot );

%feature( "docstring", "Set window into plot space." ) plsdiplt;
void
plsdiplt( PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax );

%feature( "docstring", "Set window into plot space incrementally (zoom)." ) plsdiplz;
void
plsdiplz( PLFLT xmin, PLFLT ymin, PLFLT xmax, PLFLT ymax );

%feature( "docstring", "Seed the random number generator." ) plseed;
void
plseed( unsigned int s );

%feature( "docstring", "Set the escape character for text strings." ) plsesc;
void
plsesc( char esc );

%feature( "docstring", "Process input strings, treating them as an option and argument pair. The first is for the external API, the second the work routine declared here for backward compatibility." ) plsetopt;
PLINT
plsetopt( const char *opt, const char *optarg );

%feature( "docstring", "Set family file parameters." ) plsfam;
void
plsfam( PLINT fam, PLINT num, PLINT bmax );

%feature( "docstring", "Set FCI (font characterization integer)." ) plsfci;
void
plsfci( PLUNICODE fci );

%feature( "docstring", "Set the output file name." ) plsfnam;
void
plsfnam( const char *fnam );

%feature( "docstring", "Set the current font family, style and weight." ) plsfont;
void
plsfont( PLINT family, PLINT style, PLINT weight );

%feature( "docstring", "Shade regions with continuous range of colours." ) plshades;
void
plshades( PLFLT **Matrix, PLINT nx, PLINT ny, defined_func df,
          PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax,
          PLFLT *Array, PLINT n, PLINT fill_width,
          PLINT cont_color, PLINT cont_width,
          fill_func ff, PLBOOL rectangular,
          pltr_func pltr,
          PLPointer SWIG_OBJECT_DATA );

%feature( "docstring", "Shade region with discrete colour, pattern fill." ) plshade;
void
plshade( PLFLT **Matrix, PLINT nx, PLINT ny, defined_func df,
         PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
         PLFLT shade_min, PLFLT shade_max,
         PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
         PLINT min_color, PLINT min_width,
         PLINT max_color, PLINT max_width,
         fill_func ff, PLBOOL rectangular,
         pltr_func pltr,
         PLPointer SWIG_OBJECT_DATA );

%feature( "docstring", "Set up a user-provided custom labeling function" ) plslabelfunc;
void
plslabelfunc( label_func lf, PLPointer data );

%feature( "docstring", "Set up lengths of major tick marks." ) plsmaj;
void
plsmaj( PLFLT def, PLFLT scale );

#if defined ( PYTHON_HAVE_PYBUFFER ) && defined ( SWIG_PYTHON )

%feature( "docstring", "Set the RGB memory area to be plotted (with the 'mem' or 'memcairo' drivers)." ) plsmem;
void
plsmem( PLINT maxx, PLINT maxy, void *plotmem );

%feature( "docstring", "Set the RGBA memory area to be plotted (with the 'memcairo' driver)." ) plsmema;
void
plsmema( PLINT maxx, PLINT maxy, void *plotmem );

#endif

%feature( "docstring", "Set up lengths of minor tick marks." ) plsmin;
void
plsmin( PLFLT def, PLFLT scale );

%feature( "docstring", "Set orientation.  Must be done before calling plinit." ) plsori;
void
plsori( PLINT ori );

%feature( "docstring", "Set output device parameters.  Usually ignored by the driver." ) plspage;
void
plspage( PLFLT xp, PLFLT yp, PLINT xleng, PLINT yleng,
         PLINT xoff, PLINT yoff );

%feature( "docstring", "Set the color map 0 colors using a cmap0 color palette file." ) plspal0;
void
plspal0( const char *filename );

%feature( "docstring", "Set the color map 1 colors using a cmap1 color palette file." ) plspal1;
void
plspal1( const char *filename, PLBOOL interpolate );

%feature( "docstring", "Set the pause (on end-of-page) status." ) plspause;
void
plspause( PLBOOL pause );

%feature( "docstring", "Set stream number." ) plsstrm;
void
plsstrm( PLINT strm );

%feature( "docstring", "Set the number of subwindows in x and y." ) plssub;
void
plssub( PLINT nx, PLINT ny );

%feature( "docstring", "Set symbol height." ) plssym;
void
plssym( PLFLT def, PLFLT scale );

%feature( "docstring", "Initialize PLplot, passing in the windows/page settings." ) plstar;
void
plstar( PLINT nx, PLINT ny );

%feature( "docstring", "Initialize PLplot, passing the device name and windows/page settings." ) plstart;
void
plstart( const char *devname, PLINT nx, PLINT ny );

#if !defined ( SWIG_LUA ) && !defined ( SWIG_OCTAVE )
%feature( "docstring", "Set the coordinate transform." ) plstransform;
void
plstransform( ct_func ctf, PLPointer data );
#endif

%feature( "docstring", "Prints out the same string repeatedly at the n points in world coordinates given by the x and y arrays.  Supersedes plpoin and plsymbol for the case where text refers to a unicode glyph either directly as UTF-8 or indirectly via the standard text escape sequences allowed for PLplot input strings." ) plstring;
void
plstring( PLINT n, PLFLT *Array, PLFLT *ArrayCk, const char *string );

%feature( "docstring", "Prints out the same string repeatedly at the n 3D points in world coordinates given by the x, y, and z arrays.  Supersedes plpoin3 for the case where text refers to a unicode glyph either directly as UTF-8 or indirectly via the standard text escape sequences allowed for PLplot input strings." ) plstring3;
void
plstring3( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLFLT *ArrayCk, const char *string );

%feature( "docstring", "Add a point to a stripchart." ) plstripa;
void
plstripa( PLINT id, PLINT pen, PLFLT x, PLFLT y );

%feature( "docstring", "Create 1d stripchart." ) plstripc;
void
plstripc( PLINT *OUTPUT, const char *xspec, const char *yspec,
          PLFLT xmin, PLFLT xmax, PLFLT xjump, PLFLT ymin, PLFLT ymax,
          PLFLT xlpos, PLFLT ylpos,
          PLBOOL y_ascl, PLBOOL acc,
          PLINT colbox, PLINT collab,
          PLINT *Array, PLINT *ArrayCk, const char *legline[4],
          const char *labx, const char *laby, const char *labtop );

%feature( "docstring", "Deletes and releases memory used by a stripchart." ) plstripd;
void
plstripd( PLINT id );

%feature( "docstring", "Set up a new line style." ) plstyl;
void
plstyl( PLINT n, PLINT *Array, PLINT *ArrayCk );

%feature( "docstring", "Set vector arrow style." ) plsvect;
void
plsvect( PLFLT *Array, PLFLT *ArrayCk, PLINT n, PLBOOL fill );

%feature( "docstring", "Set the edges of the viewport to the specified absolute coordinates." ) plsvpa;
void
plsvpa( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax );

%feature( "docstring", "Set x axis labeling parameters." ) plsxax;
void
plsxax( PLINT digmax, PLINT digits );

%feature( "docstring", "Set y axis labeling parameters." ) plsyax;
void
plsyax( PLINT digmax, PLINT digits );

%feature( "docstring", "Plot array y against x for n points using Hershey symbol \"code\"." ) plsym;
void
plsym( PLINT n, PLFLT *Array, PLFLT *ArrayCk, PLINT code );

%feature( "docstring", "Set z axis labeling parameters." ) plszax;
void
plszax( PLINT digmax, PLINT digits );

%feature( "docstring", "Switch to text screen." ) pltext;
void
pltext( void );

%feature( "docstring", "Set the format for date / time labels" ) pltimefmt;
void
pltimefmt( const char *fmt );

%feature( "docstring", "Sets the edges of the viewport with the given aspect ratio, leaving room for labels." ) plvasp;
void
plvasp( PLFLT aspect );

%feature( "docstring", "Plot vectors." ) plvect;
void
plvect( PLFLT **Matrix, PLFLT **MatrixCk, PLINT nx, PLINT ny, PLFLT scale,
        pltr_func pltr,
        PLPointer SWIG_OBJECT_DATA );

%feature( "docstring", "Create the largest viewport of the specified aspect ratio that fits within the specified normalized subpage coordinates." ) plvpas;
void
plvpas( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT aspect );

%feature( "docstring", "Create a viewport with the specified normalized subpage coordinates." ) plvpor;
void
plvpor( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax );

%feature( "docstring", "Define a \"standard\" viewport with seven character heights for the left margin and four character heights everywhere else." ) plvsta;
void
plvsta( void );

%feature( "docstring", "Set up a window for three-dimensional plotting." ) plw3d;
void
plw3d( PLFLT basex, PLFLT basey, PLFLT height, PLFLT xmin0,
       PLFLT xmax0, PLFLT ymin0, PLFLT ymax0, PLFLT zmin0,
       PLFLT zmax0, PLFLT alt, PLFLT az );

%feature( "docstring", "Set pen width." ) plwid;
void
plwid( PLINT width );

%feature( "docstring", "Set up world coordinates of the viewport boundaries (2d plots)." ) plwind;
void
plwind( PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax );

%feature( "docstring", "Set xor mode; mode = 1-enter, 0-leave, status = 0 if not interactive device." ) plxormod;
void
plxormod( PLBOOL mode, PLBOOL *OUTPUT );

#if 0

// Deprecated functions that are in common API, but we don't want to
// propagate them to the python API.

%feature( "docstring", "Set line color by red, green, blue values in range from 0. to 1." ) plrgb;
void
plrgb( PLFLT r, PLFLT g, PLFLT b );

%feature( "docstring", "Set line color by 8 bit RGB values." ) plrgb1;
void
plrgb1( PLINT r, PLINT g, PLINT b );

void
plshade1( PLFLT *Matrix, PLINT nx, PLINT ny, defined_func df,
          PLFLT left, PLFLT right, PLFLT bottom, PLFLT top,
          PLFLT shade_min, PLFLT shade_max,
          PLINT sh_cmap, PLFLT sh_color, PLINT sh_width,
          PLINT min_color, PLINT min_width,
          PLINT max_color, PLINT max_width,
          fill_func ff, PLBOOL rectangular,
          pltr_func pltr,
          PLPointer SWIG_OBJECT_DATA );

#endif

//--------------------------------------------------------------------------
//		Functions for use from C or C++ only
//  N.B. If you want these in python, they should be officially put in
//  the common API for all front-ends to the PLplot library with "c_" suffix,
//  DocBook xml documentation in the api.xml chapter, etc.
//--------------------------------------------------------------------------

#if 0

// Draws a contour plot using the function evaluator f2eval and data stored
// by way of the f2eval_data pointer.  This allows arbitrary organizations
// of 2d array data to be used.
//
void
plfcont( f2eval_func f2eval,
         PLPointer SWIG_OBJECT_DATA,
         PLINT nx, PLINT ny, PLINT kx, PLINT lx,
         PLINT ky, PLINT ly, PLFLT *clevel, PLINT nlevel,
         pltr_func pltr,
         PLPointer SWIG_OBJECT_DATA );

#endif

// plot continental outline in world coordinates

void
plmap( mapform_func mapform, const char *type,
       PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat );

// Plot the latitudes and longitudes on the background.

void
plmeridians( mapform_func mapform,
             PLFLT dlong, PLFLT dlat,
             PLFLT minlong, PLFLT maxlong, PLFLT minlat, PLFLT maxlat );


#if 0

void
plfshade( f2eval_func,
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
          PLPointer SWIG_OBJECT_DATA );

// Converts input values from relative device coordinates to relative plot
// coordinates.

void
pldid2pc( PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT );

// Converts input values from relative plot coordinates to relative
// device coordinates.

void
pldip2dc( PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT, PLFLT *INOUT );
#endif

// plots a 2d image (or a matrix too large for plshade() ).

void
plimage( PLFLT **Matrix, PLINT nx, PLINT ny,
         PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
         PLFLT Dxmin, PLFLT Dxmax, PLFLT Dymin, PLFLT Dymax );

// plots a 2d image (or a matrix too large for plshade() ).

void
plimagefr( PLFLT **Matrix, PLINT nx, PLINT ny,
           PLFLT xmin, PLFLT xmax, PLFLT ymin, PLFLT ymax, PLFLT zmin, PLFLT zmax,
           PLFLT valuemin, PLFLT valuemax,
           pltr_func pltr_img, PLPointer SWIG_OBJECT_DATA_img );

#ifdef 0
// Returns a list of file-oriented device names and their menu strings
void
plgFileDevs( char ***p_menustr, char ***p_devname, PLINT *p_ndev );

// Returns a list of all device names and their menu strings

void
plgDevs( char ***p_menustr, char ***p_devname, PLINT *p_ndev );

// Set the function pointer for the keyboard event handler

void
plsKeyEH( void ( *KeyEH )( PLGraphicsIn *, void *, PLINT * ), void *KeyEH_data );

// Set the function pointer for the (mouse) button event handler

void
plsButtonEH( void ( *ButtonEH )( PLGraphicsIn *, void *, PLINT * ),
             void *ButtonEH_data );
#endif
// Set the variables to be used for storing error info

#if 0
// Cannot get this to work since plsError is not simply an output
// of an internal integer and character string.
%feature( "docstring", "Set the variables to be used for storing error info." ) plsError;
void
plsError( PLINT *OUTPUT, char *OUTPUT );
#endif

// Sets an optional user exit handler.
#if 0
void
plsexit( PLINT ( *handler )( const char * ) );
// Transformation routines
#endif

#if 0
// Just like pltr2() but uses pointer arithmetic to get coordinates from
// 2d grid tables.

void
pltr2p( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer pltr_data );
// Identity transformation for plots from Fortran.

void
pltr0f( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data );

// Does linear interpolation from doubly dimensioned coord arrays
// (row dominant, i.e. Fortran ordering).

void
pltr2f( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, void *pltr_data );

// Example linear transformation function for contour plotter.
// This is not actually part of the core library any more
//void
//xform(PLFLT x, PLFLT y, PLFLT *OUTPUT, PLFLT *OUTPUT);
// Function evaluators
// Does a lookup from a 2d function array.  Array is of type (PLFLT **),
// and is column dominant (normal C ordering).

PLFLT
plf2eval2( PLINT ix, PLINT iy, PLPointer plf2eval_data );

// Does a lookup from a 2d function array.  Array is of type (PLFLT *),
// and is column dominant (normal C ordering).

PLFLT
plf2eval( PLINT ix, PLINT iy, PLPointer plf2eval_data );

// Does a lookup from a 2d function array.  Array is of type (PLFLT *),
// and is row dominant (Fortran ordering).

PLFLT
plf2evalr( PLINT ix, PLINT iy, PLPointer plf2eval_data );
#endif
// Command line parsing utilities

// Clear internal option table info structure.

void
plClearOpts( void );

// Reset internal option table info structure.

void
plResetOpts( void );

// Merge user option table into internal info structure.
#if 0
PLINT
plMergeOpts( PLOptionTable *options, const char *name, const char **notes );
#endif
// Set the strings used in usage and syntax messages.

void
plSetUsage( const char *program_string, const char *usage_string );

#if 0
// This is wrapped by common API plsetopt so ignore.
PLINT
plSetOpt( const char *opt, const char *optarg );
#endif

// Print usage & syntax message.

void
plOptUsage( void );

// Miscellaneous
#if 0
// Set the output file pointer

void
plgfile( FILE **p_file );

// Get the output file pointer

void
plsfile( FILE *file );

// Get the escape character for text strings.

void
plgesc( char *p_esc );

// Front-end to driver escape function.

void
pl_cmd( PLINT op, void *ptr );

// Return full pathname for given file if executable

PLINT
plFindName( char *p );

// Looks for the specified executable file according to usual search path.

char *
plFindCommand( const char *fn );

// Gets search name for file by concatenating the dir, subdir, and file
// name, allocating memory as needed.

void
plGetName( const char *dir, const char *subdir, const char *filename, char **filespec );

// Prompts human to input an integer in response to given message.

PLINT
plGetInt( const char *s );

// Prompts human to input a float in response to given message.

PLFLT
plGetFlt( const char *s );

// Nice way to allocate space for a vectored 2d grid

// Allocates a block of memory for use as a 2-d grid of PLFLT's.

void
plAlloc2dGrid( PLFLT ***f, PLINT nx, PLINT ny );

// Frees a block of memory allocated with plAlloc2dGrid().

void
plFree2dGrid( PLFLT **f, PLINT nx, PLINT ny );

// Find the maximum and minimum of a 2d matrix allocated with plAllc2dGrid().

#endif

void
plMinMax2dGrid( PLFLT **Matrix, PLINT nx, PLINT ny, PLFLT *OUTPUT, PLFLT *OUTPUT );

// Wait for graphics input event and translate to world coordinates

%feature( "docstring", "Wait for graphics input event and translate to world coordinates." ) plGetCursor;
PLINT
plGetCursor( PLGraphicsIn *gin );

// Translates relative device coordinates to world coordinates.

#if 0
// Use plcalc_world instead of plTranslateCursor.
int
plTranslateCursor( PLGraphicsIn *gin );
#endif
