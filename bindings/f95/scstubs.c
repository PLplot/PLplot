//      C stub routines.
//
// Copyright (C) 2004-2014 Alan W. Irwin
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
//      The stubs contained here are the ones that are relatively simple,
//      i.e. involving only a call convention change or integer-to-string
//      conversion.  Exceptions are plparseopts and  plstripc  which have
//      a few more complications in them.
//

#include "plstubs.h"

#ifdef CVF
#define STDCALL    __stdcall
#else
#define STDCALL
#endif
static void ( STDCALL *plmapform )( PLINT *, PLFLT *, PLFLT * ); // Note: slightly different prototype than
                                                                 // (*mapform)!
// Slightly different to (*label_func) as we don't support PLPointer for
// additional data in f95.
// Note the hidden argument!
static void ( STDCALL *pllabelfunc )( PLINT *, PLFLT *, char *, PLINT *, PLINT );

// Slightly different to C version as we don't support PLPointer  for additional data
static void ( STDCALL *pltransform )( PLFLT *, PLFLT *, PLFLT *, PLFLT * );


static char **pllegend_text;
static char **pllegend_symbols;
static char **plcolorbar_labels;
static char **plcolorbar_axisopts;

// Function prototypes
static void pltransformf2c( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer data );
void PL_SETCONTLABELFORMAT( PLINT *lexp, PLINT *sigdig );
void PL_SETCONTLABELFORMATa( PLINT *lexp, PLINT *sigdig );
void PL_SETCONTLABELPARAM( PLFLT *offset, PLFLT *size, PLFLT *spacing, PLINT *active );
void PL_SETCONTLABELPARAMa( PLFLT *offset, PLFLT *size, PLFLT *spacing, PLINT *active );
void PLABORT7( const char *text );
void PLADV( PLINT *sub );
void PLARC( PLFLT *x, PLFLT *y, PLFLT *a, PLFLT *b, PLFLT *angle1, PLFLT *angle2, PLFLT *rotate, PLBOOL *fill );
void PLAXES7( PLFLT *x0, PLFLT *y0, const char *xopt, PLFLT *xtick,
              PLINT *nxsub, const char *yopt, PLFLT *ytick, PLINT *nysub );
void PLBIN( PLINT *nbin, PLFLT *x, PLFLT *y, PLINT *center );
void PLBTIME( PLINT *year, PLINT *month, PLINT *day, PLINT *hour, PLINT *min, PLFLT *sec, PLFLT *ctime );
void PLBOP( void );
void PLBOX7( const char *xopt, PLFLT *xtick, PLINT *nxsub,
             const char *yopt, PLFLT *ytick, PLINT *nysub );
void PLBOX37( const char *xopt, const char *xlabel, PLFLT *xtick, PLINT *nxsub,
              const char *yopt, const char *ylabel, PLFLT *ytick, PLINT *nysub,
              const char *zopt, const char *zlabel, PLFLT *ztick, PLINT *nzsub );
void PLCALC_WORLD( PLFLT *rx, PLFLT *ry, PLFLT *wx, PLFLT *wy, PLINT *window );
void PLCALC_WORLDa( PLFLT *rx, PLFLT *ry, PLFLT *wx, PLFLT *wy, PLINT *window );
void PLCLEAR( void );
void PLCOL0( PLINT *icol );
void PLCOL1( PLFLT *col );
void PLCOLORBAR( PLFLT *p_colorbar_width, PLFLT *p_colorbar_height,
                 PLINT *opt, PLINT *position, PLFLT *x, PLFLT *y,
                 PLFLT *x_length, PLFLT *y_length,
                 PLINT *bg_color, PLINT *bb_color, PLINT *bb_style,
                 PLFLT *low_cap_color, PLFLT *high_cap_color,
                 PLINT *cont_color, PLFLT *cont_width,
                 PLINT *n_labels, const PLINT *label_opts,
                 PLINT *n_axes,
                 const PLFLT *ticks, const PLINT *sub_ticks,
                 const PLINT *n_values, const PLFLT *values );
void PLCONFIGTIME( PLFLT *scale, PLFLT *offset1, PLFLT *offset2, PLINT *ccontrol, PLBOOL *ifbtime_offset, PLINT *year, PLINT *month, PLINT *day, PLINT *hour, PLINT *min, PLFLT *sec );
void PLCPSTRM( PLINT *iplsr, PLBOOL *flags );
void PLCTIME( PLINT *year, PLINT *month, PLINT *day, PLINT *hour, PLINT *min, PLFLT *sec, PLFLT *ctime );
void PLEND( void );
void PLEND1( void );
void PLENV( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLINT *just, PLINT *axis );
void PLENV0( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLINT *just, PLINT *axis );
void PLEOP( void );
void PLERRX( PLINT *n, PLFLT *xmin, PLFLT *xmax, PLFLT *y );
void PLERRY( PLINT *n, PLFLT *x, PLFLT *ymin, PLFLT *ymax );
void PLFAMADV( void );
void PLFILL( PLINT *n, PLFLT *x, PLFLT *y );
void PLFILL3( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z );
void PLFLUSH( void );
void PLFONT( PLINT *font );
void PLFONTLD( PLINT *charset );
void PLGCHR( PLFLT *chrdef, PLFLT *chrht );
void PLGCOL0( PLINT *icol0, PLINT *r, PLINT *g, PLINT *b );
void PLGCOL0A( PLINT *icol0, PLINT *r, PLINT *g, PLINT *b, PLFLT *a );
void PLGCOLBG( PLINT *r, PLINT *g, PLINT *b );
void PLGCOLBGA( PLINT *r, PLINT *g, PLINT *b, PLFLT *a );
void PLGCOMPRESSION( PLINT *compression );
void PLGCMAP1_RANGE( PLFLT *min_color, PLFLT *max_color );
void PLGDEV7( char *dev, int length );
void PLGDIDEV( PLFLT *p_mar, PLFLT *p_aspect, PLFLT *p_jx, PLFLT *p_jy );
void PLGDIORI( PLFLT *p_rot );
void PLGDIPLT( PLFLT *p_xmin, PLFLT *p_ymin, PLFLT *p_xmax, PLFLT *p_ymax );
void PLGETCURSOR( PLGraphicsIn *gin );
void PLGFAM( PLINT *fam, PLINT *num, PLINT *bmax );
void PLGFCI( PLUNICODE *pfci );
void PLGFNAM7( char *fnam, int length );
void PLGFONT( PLINT *family, PLINT *style, PLINT *weight );
void PLGLEVEL( PLINT *level );
void PLGPAGE( PLFLT *xpmm, PLFLT *ypmm, PLINT *xwid, PLINT *ywid, PLINT *xoff, PLINT *yoff );
void PLGRA( void );
void PLGRADIENT( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *angle );
void PLGSPA( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax );
void PLGSTRM( PLINT *strm );
void PLGVER7( char *ver );
void PLGVPD( PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax );
void PLGVPW( PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax );
void PLGXAX( PLINT *digmax, PLINT *digits );
void PLGYAX( PLINT *digmax, PLINT *digits );
void PLGZAX( PLINT *digmax, PLINT *digits );
void PLHIST( PLINT *n, PLFLT *data, PLFLT *datmin, PLFLT *datmax, PLINT *nbin, PLINT *oldwin );
void PLHLS( PLFLT *hue, PLFLT *light, PLFLT *sat );
void PLHLSRGB( PLFLT *h, PLFLT *l, PLFLT *s, PLFLT *r, PLFLT *g, PLFLT *b );
void PLIMAGEFR( PLFLT *idata, PLINT *nx, PLINT *ny,
                PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLFLT *zmin, PLFLT *zmax,
                PLFLT *Dxmin, PLFLT *Dxmax, PLFLT *Dymin, PLFLT *Dymax,
                PLFLT *valuemin, PLFLT *valuemax );
void PLIMAGE( PLFLT *idata, PLINT *nx, PLINT *ny,
              PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLFLT *zmin, PLFLT *zmax,
              PLFLT *Dxmin, PLFLT *Dxmax, PLFLT *Dymin, PLFLT *Dymax );
void PLINIT( void );
void PLJOIN( PLFLT *x1, PLFLT *y1, PLFLT *x2, PLFLT *y2 );
void PLLAB7( const char *xlab, const char *ylab, const char *title );
void PLLEGEND_CNV_TEXT( PLINT *id, PLINT *number, char *string, PLINT length );
void PLLEGEND( PLFLT *p_legend_width, PLFLT *p_legend_height,
               PLINT *opt, PLINT *position, PLFLT *x, PLFLT *y, PLFLT *plot_width,
               PLINT *bg_color, PLINT *bb_color, PLINT *bb_style,
               PLINT *nrow, PLINT *ncolumn,
               PLINT *nlegend, const PLINT *opt_array,
               PLFLT *text_offset, PLFLT *text_scale, PLFLT *text_spacing,
               PLFLT *text_justification,
               const PLINT *text_colors,
               const PLINT *box_colors, const PLINT *box_patterns,
               const PLFLT *box_scales, const PLFLT *box_line_widths,
               const PLINT *line_colors, const PLINT *line_styles,
               const PLFLT *line_widths,
               const PLINT *symbol_colors, const PLFLT *symbol_scales,
               const PLINT *symbol_numbers );
void PLLIGHTSOURCE( PLFLT *x, PLFLT *y, PLFLT *z );
void PLLINE( PLINT *n, PLFLT *x, PLFLT *y );
void PLLINE3( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z );
void PLLSTY( PLINT *lin );
void PLMAP7( const char *type,
             PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat );
void PLMAPFILL7( const char *type,
                 PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat, PLINT *plotentries, PLINT *nplotentries );
void PLMAPLINE7( const char *type,
                 PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat, PLINT *plotentries, PLINT *nplotentries );
void PLMAPSTRING7( const char *type, const char *string,
                   PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat, PLINT *plotentries, PLINT *nplotentries );
void PLMAPTEX7( const char *type, PLFLT *dx, PLFLT *dy, PLFLT *just, const char *text,
                PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat, PLINT *plotentry );
void PLMERIDIANS7( PLFLT *dlong, PLFLT *dlat,
                   PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat );
void PLMKSTRM( PLINT *p_strm );
void PLMTEX7( const char *side, PLFLT *disp, PLFLT *pos, PLFLT *just, const char *text );
void PLMTEX37( const char *side, PLFLT *disp, PLFLT *pos, PLFLT *just, const char *text );
void PLPARSEOPTS7( int *numargs, const char *iargs, PLINT *mode, PLINT *maxindex );
void PLPAT( PLINT *nlin, PLINT *inc, PLINT *del );
void PLPATH( PLINT *n, PLFLT *x1, PLFLT *y1, PLFLT *x2, PLFLT *y2 );
void PLPOIN( PLINT *n, PLFLT *x, PLFLT *y, PLINT *code );
void PLPOIN3( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT *code );
void PLPOLY3( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, PLBOOL *draw, PLBOOL *ifcc );
void PLPREC( PLINT *setp, PLINT *prec );
void PLPSTY( PLINT *patt );
void PLPTEX7( PLFLT *x, PLFLT *y, PLFLT *dx, PLFLT *dy, PLFLT *just, const char *text );
void PLPTEX37( PLFLT *x, PLFLT *y, PLFLT *z,
               PLFLT *dx, PLFLT *dy, PLFLT *dz,
               PLFLT *sx, PLFLT *sy, PLFLT *sz,
               PLFLT *just, const char *text );
PLFLT PLRANDD( void );
void PLREPLOT( void );
void PLRGB( PLFLT *red, PLFLT *green, PLFLT *blue );
void PLRGB1( PLINT *r, PLINT *g, PLINT *b );
void PLRGBHLS( PLFLT *r, PLFLT *g, PLFLT *b, PLFLT *h, PLFLT *l, PLFLT *s );
void PLSCHR( PLFLT *def, PLFLT *scale );
void PLSCMAP0( PLINT *r, PLINT *g, PLINT *b, PLINT *ncol0 );
void PLSCMAP0A( PLINT *r, PLINT *g, PLINT *b, PLFLT *a, PLINT *ncol0 );
void PLSCMAP0N( PLINT *n );
void PLSCMAP1( PLINT *r, PLINT *g, PLINT *b, PLINT *ncol1 );
void PLSCMAP1A( PLINT *r, PLINT *g, PLINT *b, PLFLT *a, PLINT *ncol1 );
void PLSCMAP1L( PLBOOL *itype, PLINT *npts, PLFLT *intensity,
                PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLBOOL *alt_hue_path );
void PLSCMAP1L2( PLBOOL *itype, PLINT *npts, PLFLT *intensity,
                 PLFLT *coord1, PLFLT *coord2, PLFLT *coord3 );
void PLSCMAP1LA( PLBOOL *itype, PLINT *npts, PLFLT *intensity,
                 PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLFLT *a, PLBOOL *alt_hue_path );
void PLSCMAP1LA2( PLBOOL *itype, PLINT *npts, PLFLT *intensity,
                  PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLFLT *a );
void PLSCMAP1N( PLINT *n );
void PLSCMAP1_RANGE( PLFLT *min_color, PLFLT *max_color );
void PLSCOL0( PLINT *icol0, PLINT *r, PLINT *g, PLINT *b );
void PLSCOL0A( PLINT *icol0, PLINT *r, PLINT *g, PLINT *b, PLFLT *a );
void PLSCOLBG( PLINT *r, PLINT *g, PLINT *b );
void PLSCOLBGA( PLINT *r, PLINT *g, PLINT *b, PLFLT *a );
void PLSCOLOR( PLINT *color );
void PLSCOMPRESSION( PLINT *compression );
void PLSDEV7( const char *dev );
void PLSDIDEV( PLFLT *mar, PLFLT *aspect, PLFLT *jx, PLFLT *jy );
void PLSDIMAP( PLINT *dimxmin, PLINT *dimxmax, PLINT *dimymin, PLINT *dimymax,
               PLFLT *dimxpmm, PLFLT *dimypmm );
void PLSDIORI( PLFLT *rot );
void PLSDIPLT( PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax );
void PLSDIPLZ( PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax );
void PLSEED( unsigned int *s );
void PLSESC( PLINT *esc );
void PLSETOPT7( const char *opt, const char *optarg );
void PLSFAM( PLINT *fam, PLINT *num, PLINT *bmax );
void PLSFCI( PLUNICODE *fci );
void PLSFNAM7( const char *fnam );
void PLSFONT( PLINT *family, PLINT *style, PLINT *weight );
void PLSLABELFUNC_ON( void ( STDCALL *labelfunc )( PLINT *, PLFLT *, char *, PLINT *, PLINT ) );
void PLSLABELFUNC_ONa( void ( STDCALL *labelfunc )( PLINT *, PLFLT *, char *, PLINT *, PLINT ) );
void PLSLABELFUNC_OFF( PLINT *dummy );
void PLSLABELFUNC_OFFa( PLINT *dummy );
void PLSLABELFUNC_NONE( void );
void PLSLABELFUNC_NONEa( void );
void PLSMAJ( PLFLT *def, PLFLT *scale );
void PLSMEM( PLINT *maxx, PLINT *maxy, void *plotmem );
void PLSMEMA( PLINT *maxx, PLINT *maxy, void *plotmem );
void PLSMIN( PLFLT *def, PLFLT *scale );
void PLSORI( PLINT *ori );
void PLSPAGE( PLFLT *xpmm, PLFLT *ypmm,
              PLINT *xwid, PLINT *ywid, PLINT *xoff, PLINT *yoff );
void PLSPAL07( const char *filename );
void PLSPAL17( const char *filename, PLBOOL *interpolate );
void PLSPAUSE( PLBOOL *pause );
void PLSSTRM( PLINT *strm );
void PLSSUB( PLINT *nx, PLINT *ny );
void PLSSYM( PLFLT *def, PLFLT *scale );
void PLSTAR( PLINT *nx, PLINT *ny );
void PLSTART7( const char *devname, PLINT *nx, PLINT *ny );
void PLSTRANSFORM1( void ( STDCALL *transformfunc )( PLFLT *, PLFLT *, PLFLT *, PLFLT * ) );
void PLSTRANSFORM2( PLINT *dummy );
void PLSTRANSFORM3( void );
void PLSTRING7( PLINT *n, PLFLT *x, PLFLT *y, const char *string );
void PLSTRING37( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, const char *string );
void PLSTRIPA( PLINT *id, PLINT *pen, PLFLT *x, PLFLT *y );
void PLSTRIPC( PLINT *id, const char *xspec, const char *yspec,
               PLFLT *xmin, PLFLT *xmax, PLFLT *xjump, PLFLT *ymin, PLFLT *ymax,
               PLFLT *xlpos, PLFLT *ylpos,
               PLBOOL *y_ascl, PLBOOL *acc,
               PLINT *colbox, PLINT *collab,
               PLINT *colline, PLINT *styline,
               const char *legline0, const char *legline1,
               const char *legline2, const char *legline3,
               const char *labx, const char *laby, const char *labtop );
void PLSTRIPD( PLINT *id );
void PLSTYL( PLINT *n, PLINT *mark, PLINT *space );
void PLSVECT1( PLFLT *arrowx, PLFLT *arrowy, PLINT *npts, PLBOOL *fill );
void PLSVECT2( void );
void PLSVPA( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax );
void PLSXAX( PLINT *digmax, PLINT *digits );
void PLSYAX( PLINT *digmax, PLINT *digits );
void PLSYM( PLINT *n, PLFLT *x, PLFLT *y, PLINT *code );
void PLSZAX( PLINT *digmax, PLINT *digits );
void PLTEXT( void );
void PLTIMEFMT7( const char *fmt );
void PLVASP( PLFLT *aspect );
void PLVPAS( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLFLT *aspect );
void PLVPOR( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax );
void PLVSTA( void );
void PLW3D( PLFLT *basex, PLFLT *basey, PLFLT *height,
            PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
            PLFLT *zmin, PLFLT *zmax,
            PLFLT *alt, PLFLT *az );
void PLWIDTH( PLFLT *width );
void PLWIND( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax );
void PLXORMOD( PLBOOL *mode, PLBOOL *status );

static void
pltransformf2c( PLFLT x, PLFLT y, PLFLT *tx, PLFLT *ty, PLPointer PL_UNUSED( data ) )
{
    ( *pltransform )( &x, &y, tx, ty );
}

void
PL_SETCONTLABELFORMAT( PLINT *lexp, PLINT *sigdig )
{
    c_pl_setcontlabelformat( *lexp, *sigdig );
}

void
PL_SETCONTLABELFORMATa( PLINT *lexp, PLINT *sigdig )
{
    c_pl_setcontlabelformat( *lexp, *sigdig );
}

void
PL_SETCONTLABELPARAM( PLFLT *offset, PLFLT *size, PLFLT *spacing, PLINT *active )
{
    c_pl_setcontlabelparam( *offset, *size, *spacing, *active );
}

void
PL_SETCONTLABELPARAMa( PLFLT *offset, PLFLT *size, PLFLT *spacing, PLINT *active )
{
    c_pl_setcontlabelparam( *offset, *size, *spacing, *active );
}

void
PLABORT7( const char *text )
{
    plabort( text );
}

void
PLADV( PLINT *sub )
{
    c_pladv( *sub );
}

void
PLARC( PLFLT *x, PLFLT *y, PLFLT *a, PLFLT *b, PLFLT *angle1, PLFLT *angle2, PLFLT *rotate, PLBOOL *fill )
{
    c_plarc( *x, *y, *a, *b, *angle1, *angle2, *rotate, *fill );
}

void
PLAXES7( PLFLT *x0, PLFLT *y0, const char *xopt, PLFLT *xtick,
         PLINT *nxsub, const char *yopt, PLFLT *ytick, PLINT *nysub )
{
    c_plaxes( *x0, *y0, xopt, *xtick, *nxsub, yopt, *ytick, *nysub );
}

void
PLBIN( PLINT *nbin, PLFLT *x, PLFLT *y, PLINT *center )
{
    c_plbin( *nbin, x, y, *center );
}

void
PLBTIME( PLINT *year, PLINT *month, PLINT *day, PLINT *hour, PLINT *min, PLFLT *sec, PLFLT *ctime )
{
    c_plbtime( year, month, day, hour, min, sec, *ctime );
}

void
PLBOP( void )
{
    c_plbop();
}

void
PLBOX7( const char *xopt, PLFLT *xtick, PLINT *nxsub,
        const char *yopt, PLFLT *ytick, PLINT *nysub )
{
    c_plbox( xopt, *xtick, *nxsub, yopt, *ytick, *nysub );
}

void
PLBOX37( const char *xopt, const char *xlabel, PLFLT *xtick, PLINT *nxsub,
         const char *yopt, const char *ylabel, PLFLT *ytick, PLINT *nysub,
         const char *zopt, const char *zlabel, PLFLT *ztick, PLINT *nzsub )
{
    c_plbox3( xopt, xlabel, *xtick, *nxsub,
        yopt, ylabel, *ytick, *nysub,
        zopt, zlabel, *ztick, *nzsub );
}

void
PLCALC_WORLD( PLFLT *rx, PLFLT *ry, PLFLT *wx, PLFLT *wy, PLINT *window )
{
    c_plcalc_world( *rx, *ry, wx, wy, window );
}

void
PLCALC_WORLDa( PLFLT *rx, PLFLT *ry, PLFLT *wx, PLFLT *wy, PLINT *window )
{
    c_plcalc_world( *rx, *ry, wx, wy, window );
}

void
PLCLEAR( void )
{
    c_plclear();
}

void
PLCOL0( PLINT *icol )
{
    c_plcol0( *icol );
}

void
PLCOL1( PLFLT *col )
{
    c_plcol1( *col );
}

void
PLCOLORBAR( PLFLT *p_colorbar_width, PLFLT *p_colorbar_height,
            PLINT *opt, PLINT *position, PLFLT *x, PLFLT *y,
            PLFLT *x_length, PLFLT *y_length,
            PLINT *bg_color, PLINT *bb_color, PLINT *bb_style,
            PLFLT *low_cap_color, PLFLT *high_cap_color,
            PLINT *cont_color, PLFLT *cont_width,
            PLINT *n_labels, const PLINT *label_opts,
            PLINT *n_axes,
            const PLFLT *ticks, const PLINT *sub_ticks,
            const PLINT *n_values, const PLFLT *values )
{
    // Need to allocate 2d C array for values
    PLFLT **a;
    int   i, j;

    a = (PLFLT **) malloc( sizeof ( PLFLT * ) * (size_t) ( *n_axes ) );
    for ( i = 0; i < *n_axes; i++ )
    {
        a[i] = (PLFLT *) malloc( sizeof ( PLFLT ) * (size_t) n_values[i] );
        for ( j = 0; j < n_values[i]; j++ )
        {
            a[i][j] = values[i + j * ( *n_axes )];
        }
    }

    c_plcolorbar( p_colorbar_width, p_colorbar_height,
        *opt, *position, *x, *y,
        *x_length, *y_length,
        *bg_color, *bb_color, *bb_style,
        *low_cap_color, *high_cap_color,
        *cont_color, *cont_width,
        *n_labels, label_opts, (const char * const *) plcolorbar_labels,
        *n_axes, (const char * const *) plcolorbar_axisopts,
        ticks, sub_ticks,
        n_values, (const PLFLT * const *) a );
    free( *plcolorbar_labels );
    free( plcolorbar_labels );
    free( *plcolorbar_axisopts );
    free( plcolorbar_axisopts );
    for ( i = 0; i < *n_axes; i++ )
    {
        free( a[i] );
    }
    free( a );
}

void
PLCONFIGTIME( PLFLT *scale, PLFLT *offset1, PLFLT *offset2, PLINT *ccontrol, PLBOOL *ifbtime_offset, PLINT *year, PLINT *month, PLINT *day, PLINT *hour, PLINT *min, PLFLT *sec )
{
    c_plconfigtime( *scale, *offset1, *offset2, *ccontrol, *ifbtime_offset, *year, *month, *day, *hour, *min, *sec );
}

void
PLCPSTRM( PLINT *iplsr, PLBOOL *flags )
{
    c_plcpstrm( *iplsr, *flags );
}

void
PLCTIME( PLINT *year, PLINT *month, PLINT *day, PLINT *hour, PLINT *min, PLFLT *sec, PLFLT *ctime )
{
    c_plctime( *year, *month, *day, *hour, *min, *sec, ctime );
}

void
PLEND( void )
{
    c_plend();
}

void
PLEND1( void )
{
    c_plend1();
}

void
PLENV( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
       PLINT *just, PLINT *axis )
{
    c_plenv( *xmin, *xmax, *ymin, *ymax, *just, *axis );
}

void
PLENV0( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
        PLINT *just, PLINT *axis )
{
    c_plenv0( *xmin, *xmax, *ymin, *ymax, *just, *axis );
}

void
PLEOP( void )
{
    c_pleop();
}

void
PLERRX( PLINT *n, PLFLT *xmin, PLFLT *xmax, PLFLT *y )
{
    c_plerrx( *n, xmin, xmax, y );
}

void
PLERRY( PLINT *n, PLFLT *x, PLFLT *ymin, PLFLT *ymax )
{
    c_plerry( *n, x, ymin, ymax );
}

void
PLFAMADV( void )
{
    c_plfamadv();
}

void
PLFILL( PLINT *n, PLFLT *x, PLFLT *y )
{
    c_plfill( *n, x, y );
}

void
PLFILL3( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z )
{
    c_plfill3( *n, x, y, z );
}

void
PLFLUSH( void )
{
    c_plflush();
}

void
PLFONT( PLINT *font )
{
    c_plfont( *font );
}

void
PLFONTLD( PLINT *charset )
{
    c_plfontld( *charset );
}

void
PLGCHR( PLFLT *chrdef, PLFLT *chrht )
{
    c_plgchr( chrdef, chrht );
}

void
PLGCMAP1_RANGE( PLFLT *min_color, PLFLT *max_color )
{
    c_plgcmap1_range( min_color, max_color );
}

void
PLGCOL0( PLINT *icol0, PLINT *r, PLINT *g, PLINT *b )
{
    c_plgcol0( *icol0, r, g, b );
}

void
PLGCOL0A( PLINT *icol0, PLINT *r, PLINT *g, PLINT *b, PLFLT *a )
{
    c_plgcol0a( *icol0, r, g, b, a );
}

void
PLGCOLBG( PLINT *r, PLINT *g, PLINT *b )
{
    c_plgcolbg( r, g, b );
}

void
PLGCOLBGA( PLINT *r, PLINT *g, PLINT *b, PLFLT *a )
{
    c_plgcolbga( r, g, b, a );
}

void
PLGCOMPRESSION( PLINT *compression )
{
    c_plgcompression( compression );
}

void
PLGDEV7( char *dev, int PL_UNUSED( length ) )
{
    c_plgdev( dev );
}

void
PLGDIDEV( PLFLT *p_mar, PLFLT *p_aspect, PLFLT *p_jx, PLFLT *p_jy )
{
    c_plgdidev( p_mar, p_aspect, p_jx, p_jy );
}

void
PLGDIORI( PLFLT *p_rot )
{
    c_plgdiori( p_rot );
}

void
PLGDIPLT( PLFLT *p_xmin, PLFLT *p_ymin, PLFLT *p_xmax, PLFLT *p_ymax )
{
    c_plgdiplt( p_xmin, p_ymin, p_xmax, p_ymax );
}

void
PLGETCURSOR( PLGraphicsIn *gin )
{
    plGetCursor( gin );
}

void
PLGFAM( PLINT *fam, PLINT *num, PLINT *bmax )
{
    c_plgfam( fam, num, bmax );
}

// Note: Fortran does not distinguish between unsigned and signed integers
// so the 32-bit PLUNICODE can be mapped to 4-byte Fortran integer outside
// this routine.
void
PLGFCI( PLUNICODE *pfci )
{
    c_plgfci( pfci );
}

void
PLGFNAM7( char *fnam, int PL_UNUSED( length ) )
{
    c_plgfnam( fnam );
}

void
PLGFONT( PLINT *family, PLINT *style, PLINT *weight )
{
    c_plgfont( family, style, weight );
}

void
PLGLEVEL( PLINT *level )
{
    c_plglevel( level );
}

void
PLGPAGE( PLFLT *xpmm, PLFLT *ypmm, PLINT *xwid, PLINT *ywid,
         PLINT *xoff, PLINT *yoff )
{
    c_plgpage( xpmm, ypmm, xwid, ywid, xoff, yoff );
}

void
PLGRA( void )
{
    c_plgra();
}

void
PLGRADIENT( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *angle )
{
    c_plgradient( *n, x, y, *angle );
}

// AM - removed: also defined, and better in sccont.c
// void
// PLGRIDDATA(PLFLT *x, PLFLT *y, PLFLT *z, PLINT *npts,
//         PLFLT *xg, PLINT *nptsx, PLFLT *yg,  PLINT *nptsy,
//         PLFLT **zg, PLINT *type, PLFLT *data)
// {
//  c_plgriddata(x, y, z, *npts,
//               xg, *nptsx, yg, *nptsy,
//               zg, *type, *data);
// }
//

void
PLGSPA( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax )
{
    c_plgspa( xmin, xmax, ymin, ymax );
}

void
PLGSTRM( PLINT *strm )
{
    c_plgstrm( strm );
}

void
PLGVER7( char *ver )
{
    c_plgver( ver );
}

void
PLGVPD( PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax )
{
    c_plgvpd( p_xmin, p_xmax, p_ymin, p_ymax );
}

void
PLGVPW( PLFLT *p_xmin, PLFLT *p_xmax, PLFLT *p_ymin, PLFLT *p_ymax )
{
    c_plgvpw( p_xmin, p_xmax, p_ymin, p_ymax );
}

void
PLGXAX( PLINT *digmax, PLINT *digits )
{
    c_plgxax( digmax, digits );
}

void
PLGYAX( PLINT *digmax, PLINT *digits )
{
    c_plgyax( digmax, digits );
}

void
PLGZAX( PLINT *digmax, PLINT *digits )
{
    c_plgzax( digmax, digits );
}

void
PLHIST( PLINT *n, PLFLT *data, PLFLT *datmin, PLFLT *datmax,
        PLINT *nbin, PLINT *oldwin )
{
    c_plhist( *n, data, *datmin, *datmax, *nbin, *oldwin );
}

#ifdef PL_DEPRECATED
void
PLHLS( PLFLT *hue, PLFLT *light, PLFLT *sat )
{
    c_plhls( *hue, *light, *sat );
}
#endif // PL_DEPRECATED

void
PLHLSRGB( PLFLT *h, PLFLT *l, PLFLT *s, PLFLT *r, PLFLT *g, PLFLT *b )
{
    c_plhlsrgb( *h, *l, *s, r, g, b );
}

#if 0
void
PLIMAGEFR( PLFLT *idata, PLINT *nx, PLINT *ny,
           PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLFLT *zmin, PLFLT *zmax,
           PLFLT *Dxmin, PLFLT *Dxmax, PLFLT *Dymin, PLFLT *Dymax,
           PLFLT *valuemin, PLFLT *valuemax )
{
    int   i, j;
    PLFLT **pidata;

    plAlloc2dGrid( &pidata, *nx, *ny );

    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            pidata[i][j] = idata[i + j * ( *nx )];
        }
    }

    c_plimagefr( (const PLFLT * const *) pidata, *nx, *ny,
        *xmin, *xmax, *ymin, *ymax, *zmin, *zmax,
        *Dxmin, *Dxmax, *Dymin, *Dymax,
        *valuemin, *valuemax );

    plFree2dGrid( pidata, *nx, *ny );
}
#endif

void
PLIMAGE( PLFLT *idata, PLINT *nx, PLINT *ny,
         PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLFLT *zmin, PLFLT *zmax,
         PLFLT *Dxmin, PLFLT *Dxmax, PLFLT *Dymin, PLFLT *Dymax )
{
    int   i, j;
    PLFLT **pidata;

    plAlloc2dGrid( &pidata, *nx, *ny );

    for ( i = 0; i < *nx; i++ )
    {
        for ( j = 0; j < *ny; j++ )
        {
            pidata[i][j] = idata[i + j * ( *nx )];
        }
    }

    c_plimage( (const PLFLT * const *) pidata, *nx, *ny,
        *xmin, *xmax, *ymin, *ymax, *zmin, *zmax,
        *Dxmin, *Dxmax, *Dymin, *Dymax );

    plFree2dGrid( pidata, *nx, *ny );
}

void
PLINIT( void )
{
    c_plinit();
}

void
PLJOIN( PLFLT *x1, PLFLT *y1, PLFLT *x2, PLFLT *y2 )
{
    c_pljoin( *x1, *y1, *x2, *y2 );
}

void
PLLAB7( const char *xlab, const char *ylab, const char *title )
{
    c_pllab( xlab, ylab, title );
}

static void
pllabelfuncf2c( PLINT axis, PLFLT value, char *label, PLINT length, PLPointer PL_UNUSED( data ) )
{
    int i;

    // (AM) Note the hidden argument "length" - it ensures that the string "label"
    // is recognised to have that length
    //
    ( *pllabelfunc )( &axis, &value, label, &length, length );

    // Ensure string is null terminated
    i = length - 1;
    while ( ( i >= 0 ) && ( label[i] == ' ' ) )
        i--;
    label[i + 1] = '\0';
}

// Auxiliary function to create a C-compatible string array
// Note the hidden argument
void
PLLEGEND_CNV_TEXT( PLINT *id, PLINT *number, char *string, PLINT length )
{
    int  j;
    int  i;
    char **p_string;
    char *data;

    // Ensure the strings are null terminated

    p_string = (char **) malloc( sizeof ( char * ) * (size_t) ( *number ) );
    data     = (char *) malloc( sizeof ( char * ) * (size_t) ( ( *number ) * ( length + 1 ) ) );

    for ( j = 0; j < ( *number ); j++ )
    {
        p_string[j] = data + j * ( length + 1 );
        memcpy( p_string[j], &string[j * length], (size_t) length );
        p_string[j][length] = '\0';
        i = length - 1;
        while ( ( i >= 0 ) && ( p_string[j][i] == ' ' ) )
            i--;
        p_string[j][i + 1] = '\0';
    }

    switch ( *id )
    {
    case 1:
        pllegend_text = p_string;
        break;
    case 2:
        pllegend_symbols = p_string;
        break;
    case 3:
        plcolorbar_labels = p_string;
        break;
    case 4:
        plcolorbar_axisopts = p_string;
        break;
    default:
        // Unknown
        free( data );
        free( p_string );
        break;
    }
}

void PLLEGEND(
    PLFLT *p_legend_width, PLFLT *p_legend_height,
    PLINT *opt, PLINT *position, PLFLT *x, PLFLT *y, PLFLT *plot_width,
    PLINT *bg_color, PLINT *bb_color, PLINT *bb_style,
    PLINT *nrow, PLINT *ncolumn,
    PLINT *nlegend, const PLINT *opt_array,
    PLFLT *text_offset, PLFLT *text_scale, PLFLT *text_spacing,
    PLFLT *text_justification,
    const PLINT *text_colors,
    const PLINT *box_colors, const PLINT *box_patterns,
    const PLFLT *box_scales, const PLFLT *box_line_widths,
    const PLINT *line_colors, const PLINT *line_styles,
    const PLFLT *line_widths,
    const PLINT *symbol_colors, const PLFLT *symbol_scales,
    const PLINT *symbol_numbers )
{
    c_pllegend( p_legend_width, p_legend_height,
        *opt, *position, *x, *y, *plot_width,
        *bg_color, *bb_color, *bb_style,
        *nrow, *ncolumn,
        *nlegend, opt_array,
        *text_offset, *text_scale, *text_spacing,
        *text_justification,
        text_colors, (const char * const *) pllegend_text,
        box_colors, box_patterns,
        box_scales, box_line_widths,
        line_colors, line_styles,
        line_widths,
        symbol_colors, symbol_scales,
        symbol_numbers, (const char * const *) pllegend_symbols );

    free( *pllegend_text );
    free( pllegend_text );
    free( *pllegend_symbols );
    free( pllegend_symbols );
}

void
PLLIGHTSOURCE( PLFLT *x, PLFLT *y, PLFLT *z )
{
    c_pllightsource( *x, *y, *z );
}

void
PLLINE( PLINT *n, PLFLT *x, PLFLT *y )
{
    c_plline( *n, x, y );
}

void
PLLINE3( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z )
{
    c_plline3( *n, x, y, z );
}

void
PLLSTY( PLINT *lin )
{
    c_pllsty( *lin );
}

static void
plmapf2c( PLINT n, PLFLT *x, PLFLT *y )
{
    ( *plmapform )( &n, x, y );
}

void
PLMAP7( const char *type,
        PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat )

{
    if ( plmapform )
        c_plmap( plmapf2c, type, *minlong, *maxlong, *minlat, *maxlat );
    else
        c_plmap( NULL, type, *minlong, *maxlong, *minlat, *maxlat );
}

void
PLMAPFILL7( const char *type,
            PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat, PLINT *plotentries, PLINT *nplotentries )

{
    PLINT *pplotentries = plotentries;
    if ( *nplotentries == 0 )
        pplotentries = NULL;
    if ( plmapform )
        c_plmapfill( plmapf2c, type, *minlong, *maxlong, *minlat, *maxlat, pplotentries, *nplotentries );
    else
        c_plmapfill( NULL, type, *minlong, *maxlong, *minlat, *maxlat, pplotentries, *nplotentries );
}

void
PLMAPLINE7( const char *type,
            PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat, PLINT *plotentries, PLINT *nplotentries )

{
    PLINT *pplotentries = plotentries;
    if ( *nplotentries == 0 )
        pplotentries = NULL;
    if ( plmapform )
        c_plmapline( plmapf2c, type, *minlong, *maxlong, *minlat, *maxlat, pplotentries, *nplotentries );
    else
        c_plmapline( NULL, type, *minlong, *maxlong, *minlat, *maxlat, pplotentries, *nplotentries );
}

void
PLMAPSTRING7( const char *type, const char *string,
              PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat, PLINT *plotentries, PLINT *nplotentries )

{
    PLINT *pplotentries = plotentries;
    if ( *nplotentries == 0 )
        pplotentries = NULL;
    if ( plmapform )
        c_plmapstring( plmapf2c, type, string, *minlong, *maxlong, *minlat, *maxlat, pplotentries, *nplotentries );
    else
        c_plmapstring( NULL, type, string, *minlong, *maxlong, *minlat, *maxlat, pplotentries, *nplotentries );
}

void
PLMAPTEX7( const char *type, PLFLT *dx, PLFLT *dy, PLFLT *just, const char *text,
           PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat, PLINT *plotentry )

{
    if ( plmapform )
        c_plmaptex( plmapf2c, type, *dx, *dy, *just, text, *minlong, *maxlong, *minlat, *maxlat, *plotentry );
    else
        c_plmaptex( NULL, type, *dx, *dy, *just, text, *minlong, *maxlong, *minlat, *maxlat, *plotentry );
}

void
PLMERIDIANS7( PLFLT *dlong, PLFLT *dlat,
              PLFLT *minlong, PLFLT *maxlong, PLFLT *minlat, PLFLT *maxlat )
{
    if ( plmapform )
        c_plmeridians( plmapf2c, *dlong, *dlat, *minlong, *maxlong, *minlat, *maxlat );
    else
        c_plmeridians( NULL, *dlong, *dlat, *minlong, *maxlong, *minlat, *maxlat );
}

void
PLMKSTRM( PLINT *p_strm )
{
    c_plmkstrm( p_strm );
}

void
PLMTEX7( const char *side, PLFLT *disp, PLFLT *pos, PLFLT *just, const char *text )
{
    c_plmtex( side, *disp, *pos, *just, text );
}

void
PLMTEX37( const char *side, PLFLT *disp, PLFLT *pos, PLFLT *just, const char *text )
{
    c_plmtex3( side, *disp, *pos, *just, text );
}

void
PLPARSEOPTS7( int *numargs, const char *iargs, PLINT *mode, PLINT *maxindex )
{
// Same as in plparseopts fortran subroutine that calls this one.
#define MAXARGS    20
    if ( *numargs <= MAXARGS )
    {
        const char *argv[MAXARGS];
        int        i;
        for ( i = 0; i < *numargs; i++ )
        {
            argv[i] = iargs + ( i * *maxindex );
//	 fprintf(stderr, "%d - %s\n", i, argv[i]);
        }
        c_plparseopts( numargs, argv, *mode );
    }
    else
        fprintf( stderr, "plparseopts7: numargs too large\n" );
}

void
PLPAT( PLINT *nlin, PLINT *inc, PLINT *del )
{
    c_plpat( *nlin, inc, del );
}

void
PLPATH( PLINT *n, PLFLT *x1, PLFLT *y1, PLFLT *x2, PLFLT *y2 )
{
    c_plpath( *n, *x1, *y1, *x2, *y2 );
}

void
PLPOIN( PLINT *n, PLFLT *x, PLFLT *y, PLINT *code )
{
    c_plpoin( *n, x, y, *code );
}

void
PLPOIN3( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, PLINT *code )
{
    c_plpoin3( *n, x, y, z, *code );
}

void
PLPOLY3( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, PLBOOL *draw, PLBOOL *ifcc )
{
    c_plpoly3( *n, x, y, z, draw, *ifcc );
}

void
PLPREC( PLINT *setp, PLINT *prec )
{
    c_plprec( *setp, *prec );
}

void
PLPSTY( PLINT *patt )
{
    c_plpsty( *patt );
}

void
PLPTEX7( PLFLT *x, PLFLT *y, PLFLT *dx, PLFLT *dy, PLFLT *just, const char *text )
{
    c_plptex( *x, *y, *dx, *dy, *just, text );
}

void
PLPTEX37(
    PLFLT *x, PLFLT *y, PLFLT *z,
    PLFLT *dx, PLFLT *dy, PLFLT *dz,
    PLFLT *sx, PLFLT *sy, PLFLT *sz,
    PLFLT *just, const char *text )
{
    c_plptex3( *x, *y, *z, *dx, *dy, *dz, *sx, *sy, *sz, *just, text );
}

PLFLT
PLRANDD()
{
    return c_plrandd();
}

void
PLREPLOT( void )
{
    c_plreplot();
}

#ifdef PL_DEPRECATED
void
PLRGB( PLFLT *red, PLFLT *green, PLFLT *blue )
{
    c_plrgb( *red, *green, *blue );
}
#endif // PL_DEPRECATED

#ifdef PL_DEPRECATED
void
PLRGB1( PLINT *r, PLINT *g, PLINT *b )
{
    c_plrgb1( *r, *g, *b );
}
#endif // PL_DEPRECATED

void
PLRGBHLS( PLFLT *r, PLFLT *g, PLFLT *b, PLFLT *h, PLFLT *l, PLFLT *s )
{
    c_plrgbhls( *r, *g, *b, h, l, s );
}

void
PLSCHR( PLFLT *def, PLFLT *scale )
{
    c_plschr( *def, *scale );
}

void
PLSCMAP0( PLINT *r, PLINT *g, PLINT *b, PLINT *ncol0 )
{
    c_plscmap0( r, g, b, *ncol0 );
}

void
PLSCMAP0A( PLINT *r, PLINT *g, PLINT *b, PLFLT *a, PLINT *ncol0 )
{
    c_plscmap0a( r, g, b, a, *ncol0 );
}

void
PLSCMAP0N( PLINT *n )
{
    c_plscmap0n( *n );
}

void
PLSCMAP1( PLINT *r, PLINT *g, PLINT *b, PLINT *ncol1 )
{
    c_plscmap1( r, g, b, *ncol1 );
}

void
PLSCMAP1A( PLINT *r, PLINT *g, PLINT *b, PLFLT *a, PLINT *ncol1 )
{
    c_plscmap1a( r, g, b, a, *ncol1 );
}

void
PLSCMAP1L( PLBOOL *itype, PLINT *npts, PLFLT *intensity,
           PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLBOOL *alt_hue_path )
{
    c_plscmap1l( *itype, *npts, intensity, coord1, coord2, coord3, alt_hue_path );
}

void
PLSCMAP1L2( PLBOOL *itype, PLINT *npts, PLFLT *intensity,
            PLFLT *coord1, PLFLT *coord2, PLFLT *coord3 )
{
    c_plscmap1l( *itype, *npts, intensity, coord1, coord2, coord3, NULL );
}

void
PLSCMAP1LA( PLBOOL *itype, PLINT *npts, PLFLT *intensity,
            PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLFLT *a, PLBOOL *alt_hue_path )
{
    c_plscmap1la( *itype, *npts, intensity, coord1, coord2, coord3, a, alt_hue_path );
}

void
PLSCMAP1LA2( PLBOOL *itype, PLINT *npts, PLFLT *intensity,
             PLFLT *coord1, PLFLT *coord2, PLFLT *coord3, PLFLT *a )
{
    c_plscmap1la( *itype, *npts, intensity, coord1, coord2, coord3, a, NULL );
}

void
PLSCMAP1N( PLINT *n )
{
    c_plscmap1n( *n );
}

void
PLSCMAP1_RANGE( PLFLT *min_color, PLFLT *max_color )
{
    c_plscmap1_range( *min_color, *max_color );
}

void
PLSCOL0( PLINT *icol0, PLINT *r, PLINT *g, PLINT *b )
{
    c_plscol0( *icol0, *r, *g, *b );
}

void
PLSCOL0A( PLINT *icol0, PLINT *r, PLINT *g, PLINT *b, PLFLT *a )
{
    c_plscol0a( *icol0, *r, *g, *b, *a );
}

void
PLSCOLBG( PLINT *r, PLINT *g, PLINT *b )
{
    c_plscolbg( *r, *g, *b );
}

void
PLSCOLBGA( PLINT *r, PLINT *g, PLINT *b, PLFLT *a )
{
    c_plscolbga( *r, *g, *b, *a );
}

void
PLSCOLOR( PLINT *color )
{
    c_plscolor( *color );
}

void
PLSCOMPRESSION( PLINT *compression )
{
    c_plscompression( *compression );
}

void
PLSDEV7( const char *dev )
{
    c_plsdev( dev );
}

void
PLSDIDEV( PLFLT *mar, PLFLT *aspect, PLFLT *jx, PLFLT *jy )
{
    c_plsdidev( *mar, *aspect, *jx, *jy );
}

void
PLSDIMAP( PLINT *dimxmin, PLINT *dimxmax, PLINT *dimymin, PLINT *dimymax,
          PLFLT *dimxpmm, PLFLT *dimypmm )
{
    c_plsdimap( *dimxmin, *dimxmax, *dimymin, *dimymax,
        *dimxpmm, *dimypmm );
}

void
PLSDIORI( PLFLT *rot )
{
    c_plsdiori( *rot );
}

void
PLSDIPLT( PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax )
{
    c_plsdiplt( *xmin, *ymin, *xmax, *ymax );
}

void
PLSDIPLZ( PLFLT *xmin, PLFLT *ymin, PLFLT *xmax, PLFLT *ymax )
{
    c_plsdiplz( *xmin, *ymin, *xmax, *ymax );
}

void
PLSEED( unsigned int *s )
{
    c_plseed( *s );
}

void
PLSESC( PLINT *esc )
{
    c_plsesc( (char) *esc );
}

// Auxiliary routine - not to be used publicly
//
#define    PLSETMAPFORMC      FNAME( PLSETMAPFORMC, plsetmapformc )
#define    PLCLEARMAPFORMC    FNAME( PLCLEARMAPFORMC, plclearmapformc )
void PLSETMAPFORMC( void ( STDCALL *mapform )( PLINT *, PLFLT *, PLFLT * ) );
void PLCLEARMAPFORMC( void );

void
PLSETMAPFORMC( void ( STDCALL *mapform )( PLINT *, PLFLT *, PLFLT * ) )
{
    plmapform = mapform;
}
void
PLCLEARMAPFORMC( void )
{
    plmapform = NULL;
}

void
PLSETOPT7( const char *opt, const char *oarg )
{
    c_plsetopt( opt, oarg );
}

void
PLSFAM( PLINT *fam, PLINT *num, PLINT *bmax )
{
    c_plsfam( *fam, *num, *bmax );
}

// Note: Fortran does not distinguish between unsigned and signed integers
// so the 32-bit PLUNICODE can be mapped to 4-byte Fortran integer outside
// this routine.
void
PLSFCI( PLUNICODE *fci )
{
    c_plsfci( *fci );
}

void
PLSFNAM7( const char *fnam )
{
    c_plsfnam( fnam );
}

void
PLSFONT( PLINT *family, PLINT *style, PLINT *weight )
{
    c_plsfont( *family, *style, *weight );
}

void
PLSLABELFUNC_ON( void ( STDCALL *labelfunc )( PLINT *, PLFLT *, char *, PLINT *, PLINT ) )
{
    pllabelfunc = labelfunc;
    // N.B. neglect pointer to additional data for f95
    c_plslabelfunc( pllabelfuncf2c, NULL );
}

void
PLSLABELFUNC_ONa( void ( STDCALL *labelfunc )( PLINT *, PLFLT *, char *, PLINT *, PLINT ) )
{
    pllabelfunc = labelfunc;
    // N.B. neglect pointer to additional data for f95
    c_plslabelfunc( pllabelfuncf2c, NULL );
}

void
PLSLABELFUNC_OFF( PLINT * PL_UNUSED( dummy ) )
{
    pllabelfunc = NULL;
    c_plslabelfunc( NULL, NULL );
}

void
PLSLABELFUNC_OFFa( PLINT * PL_UNUSED( dummy ) )
{
    pllabelfunc = NULL;
    c_plslabelfunc( NULL, NULL );
}

void
PLSLABELFUNC_NONE( void )
{
    pllabelfunc = NULL;
    c_plslabelfunc( NULL, NULL );
}
void
PLSLABELFUNC_NONEa( void )
{
    pllabelfunc = NULL;
    c_plslabelfunc( NULL, NULL );
}

void
PLSMAJ( PLFLT *def, PLFLT *scale )
{
    c_plsmaj( *def, *scale );
}

void
PLSMEM( PLINT *maxx, PLINT *maxy, void *plotmem )
{
    c_plsmem( *maxx, *maxy, plotmem );
}

void
PLSMEMA( PLINT *maxx, PLINT *maxy, void *plotmem )
{
    c_plsmema( *maxx, *maxy, plotmem );
}

void
PLSMIN( PLFLT *def, PLFLT *scale )
{
    c_plsmin( *def, *scale );
}

void
PLSORI( PLINT *ori )
{
    c_plsori( *ori );
}

void
PLSPAGE( PLFLT *xpmm, PLFLT *ypmm,
         PLINT *xwid, PLINT *ywid, PLINT *xoff, PLINT *yoff )
{
    c_plspage( *xpmm, *ypmm, *xwid, *ywid, *xoff, *yoff );
}

void
PLSPAL07( const char *filename )
{
    c_plspal0( filename );
}

void
PLSPAL17( const char *filename, PLBOOL *interpolate )
{
    c_plspal1( filename, *interpolate );
}

void
PLSPAUSE( PLBOOL *ipause )
{
    c_plspause( *ipause );
}

void
PLSSTRM( PLINT *strm )
{
    c_plsstrm( *strm );
}

void
PLSSUB( PLINT *nx, PLINT *ny )
{
    c_plssub( *nx, *ny );
}

void
PLSSYM( PLFLT *def, PLFLT *scale )
{
    c_plssym( *def, *scale );
}

void
PLSTAR( PLINT *nx, PLINT *ny )
{
    c_plstar( *nx, *ny );
}

void
PLSTART7( const char *devname, PLINT *nx, PLINT *ny )
{
    c_plstart( devname, *nx, *ny );
}

void
PLSTRANSFORM1( void ( STDCALL *transformfunc )( PLFLT *, PLFLT *, PLFLT *, PLFLT * ) )
{
    pltransform = transformfunc;

    c_plstransform( pltransformf2c, NULL );
}

void
PLSTRANSFORM2( PLINT * PL_UNUSED( dummy ) )
{
    pltransform = NULL;

    c_plstransform( NULL, NULL );
}

void
PLSTRANSFORM3( void )
{
    pltransform = NULL;

    c_plstransform( NULL, NULL );
}

void
PLSTRING7( PLINT *n, PLFLT *x, PLFLT *y, const char *string )
{
    c_plstring( *n, x, y, string );
}

void
PLSTRING37( PLINT *n, PLFLT *x, PLFLT *y, PLFLT *z, const char *string )
{
    c_plstring3( *n, x, y, z, string );
}

void
PLSTRIPA( PLINT *id, PLINT *pen, PLFLT *x, PLFLT *y )
{
    c_plstripa( *id, *pen, *x, *y );
}

void
PLSTRIPC( PLINT *id, const char *xspec, const char *yspec,
          PLFLT *xmin, PLFLT *xmax, PLFLT *xjump, PLFLT *ymin, PLFLT *ymax,
          PLFLT *xlpos, PLFLT *ylpos,
          PLBOOL *y_ascl, PLBOOL *acc,
          PLINT *colbox, PLINT *collab,
          PLINT *colline, PLINT *styline,
          const char *legline0, const char *legline1,
          const char *legline2, const char *legline3,
          const char *labx, const char *laby, const char *labtop )
{
    const char* legline[4];
    legline[0] = legline0;
    legline[1] = legline1;
    legline[2] = legline2;
    legline[3] = legline3;

    c_plstripc( id, xspec, yspec,
        *xmin, *xmax, *xjump, *ymin, *ymax,
        *xlpos, *ylpos,
        *y_ascl, *acc,
        *colbox, *collab,
        colline, styline, legline,
        labx, laby, labtop );
}

void
PLSTRIPD( PLINT *id )
{
    c_plstripd( *id );
}

void
PLSTYL( PLINT *n, PLINT *mark, PLINT *space )
{
    c_plstyl( *n, mark, space );
}

void
PLSVECT1( PLFLT *arrowx, PLFLT *arrowy, PLINT *npts, PLBOOL *fill )
{
    c_plsvect( arrowx, arrowy, *npts, *fill );
}

void
PLSVECT2( void )
{
    c_plsvect( NULL, NULL, 0, 0 );
}

void
PLSVPA( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax )
{
    c_plsvpa( *xmin, *xmax, *ymin, *ymax );
}

void
PLSXAX( PLINT *digmax, PLINT *digits )
{
    c_plsxax( *digmax, *digits );
}

void
PLSYAX( PLINT *digmax, PLINT *digits )
{
    c_plsyax( *digmax, *digits );
}

void
PLSYM( PLINT *n, PLFLT *x, PLFLT *y, PLINT *code )
{
    c_plsym( *n, x, y, *code );
}

void
PLSZAX( PLINT *digmax, PLINT *digits )
{
    c_plszax( *digmax, *digits );
}

void
PLTEXT( void )
{
    c_pltext();
}

void
PLTIMEFMT7( const char *fmt )
{
    c_pltimefmt( fmt );
}

void
PLVASP( PLFLT *aspect )
{
    c_plvasp( *aspect );
}

void
PLVPAS( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax, PLFLT *aspect )
{
    c_plvpas( *xmin, *xmax, *ymin, *ymax, *aspect );
}

void
PLVPOR( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax )
{
    c_plvpor( *xmin, *xmax, *ymin, *ymax );
}

void
PLVSTA( void )
{
    c_plvsta();
}

void
PLW3D( PLFLT *basex, PLFLT *basey, PLFLT *height,
       PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax,
       PLFLT *zmin, PLFLT *zmax,
       PLFLT *alt, PLFLT *az )
{
    c_plw3d( *basex, *basey, *height,
        *xmin, *xmax, *ymin, *ymax, *zmin, *zmax, *alt, *az );
}

void
PLWIDTH( PLFLT *width )
{
    c_plwidth( *width );
}

void
PLWIND( PLFLT *xmin, PLFLT *xmax, PLFLT *ymin, PLFLT *ymax )
{
    c_plwind( *xmin, *xmax, *ymin, *ymax );
}

void
PLXORMOD( PLBOOL *mode, PLBOOL *status )
{
    c_plxormod( *mode, status );
}
