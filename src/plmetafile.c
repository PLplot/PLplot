// PLplot metafile I/O routines.  Originally implemented in the plmeta
// driver and plrender.  The decision was made to provide the capability
// to always read/write metafiles, thus the routines have been merged
// into the core of the library.
//
// Copyright (C) 2006  Hazen Babcock
// Copyright (C) 2015  Jim Dishaw

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

#define NEED_PLDEBUG
#include "plplotP.h"
#include "metadefs.h"
#include <stddef.h>       // For the offsetof() macro

#define MAX_BUFFER    256 // Character buffer size for reading records

// Status codes
enum _plm_status
{
    PLM_UNKNOWN_DATATYPE = -6,
    PLM_INVALID_STATE    = -5,
    PLM_INVALID_CMD      = -4,
    PLM_FORMAT_ERROR     = -3,
    PLM_UNKNOWN_VERSION  = -2,
    PLM_NOT_PLMETA_FILE  = -1,
    PLM_SUCCESS          = 0
};

// Portable data format types (perhaps should be defined elsewhere?)
enum _pdf_types
{
    PDF_NULL = 0, // No-data type
    PDF_UBYTE,    // one-byte unsigned integer
    PDF_USHORT,   // two-byte unsigned integer
    PDF_ULONG,    // four-byte unsigned integer
    PDF_IEEEF     // IEEE 32 bit floating point
};

// Data types used by PLplot (perhaps should be defined elsewhere?)
enum _plp_types
{
    PLP_NULL = 0, // No-data type
    PLP_UBYTE,
    PLP_UCHAR,
    PLP_USHORT,
    PLP_SHORT,
    PLP_PLINT,
    PLP_PLFLT
};

// Data format entry structure
struct _plm_format
{
    char            *name;     // Name of the field
    enum _pdf_types pdf_type;  // Field data type in metafile
    enum _plp_types plp_type;  // Field data type in PLplot
    const size_t    offset;    // Byte offset in the plmeta device
};

// Metafile index information header
static const struct _plm_format index_2005_header[] = {
    { "pages", PDF_USHORT, PLP_USHORT, offsetof( PLmIndex, pages ) },
    // Last entry, do not change
    { NULL,    PDF_NULL,   PLP_NULL, 0 }
};

// Device information header
static const struct _plm_format dev_2005_header[] = {
    { "xmin", PDF_USHORT, PLP_PLINT, offsetof( PLmDev, xmin ) },
    { "xmax", PDF_USHORT, PLP_PLINT, offsetof( PLmDev, xmax ) },
    { "ymin", PDF_USHORT, PLP_PLINT, offsetof( PLmDev, ymin ) },
    { "ymax", PDF_USHORT, PLP_PLINT, offsetof( PLmDev, ymax ) },
    { "pxlx", PDF_IEEEF,  PLP_PLFLT, offsetof( PLmDev, pxlx ) },
    { "pxly", PDF_IEEEF,  PLP_PLFLT, offsetof( PLmDev, pxly ) },
    // Last entry, do not change
    { NULL,   PDF_NULL,   PLP_NULL, 0 }
};

// Plot information header
static const struct _plm_format plot_2005_header[] = {
    { "xdpi",    PDF_IEEEF,  PLP_PLFLT, offsetof( PLStream, xdpi )    },
    { "ydpi",    PDF_IEEEF,  PLP_PLFLT, offsetof( PLStream, ydpi )    },
    { "xlength", PDF_USHORT, PLP_PLINT, offsetof( PLStream, xlength ) },
    { "ylength", PDF_USHORT, PLP_PLINT, offsetof( PLStream, ylength ) },
    { "xoffset", PDF_USHORT, PLP_PLINT, offsetof( PLStream, xoffset ) },
    { "yoffset", PDF_USHORT, PLP_PLINT, offsetof( PLStream, yoffset ) },
    { "",        PDF_NULL,   PLP_NULL,                 0 },
    // Last entry, do not change
    { NULL,               0, 0 }
};

// Page information header
static const struct _plm_format page_2005_header[] = {
    { "", PDF_USHORT, PLP_PLINT, offsetof( PLmDev, page )      },
    { "", PDF_ULONG,  PLP_PLINT, offsetof( PLmDev, lp_offset ) },
    { "", PDF_ULONG,  PLP_NULL, 0 }
};

static struct _plm_version
{
    const char *identifier;
    const struct _plm_format *index_header;
    const struct _plm_format *device_header;
    const struct _plm_format *plot_header;
} metafile_format[] = {
    { PLMETA_VERSION,
      index_2005_header, dev_2005_header, plot_2005_header }
};

static
enum _plm_status set_ubyte_plp_value( uint8_t x,
                                      void *dest, enum _plp_types type )
{
    enum _plm_status rc = PLM_SUCCESS;

    switch ( type )
    {
    case PLP_NULL:
        break;
    case PLP_UBYTE:
        *(uint8_t *) dest = x;
        break;
    case PLP_UCHAR:
        *(unsigned char *) dest = x;
        break;
    case PLP_USHORT:
        *(U_SHORT *) dest = x;
        break;
    case PLP_SHORT:
        *(short *) dest = x;
        break;
    case PLP_PLINT:
        *(PLINT *) dest = x;
        break;
    case PLP_PLFLT:
        *(PLFLT *) dest = x;
        break;
    default:
        plwarn( "Unhandled datatype conversion in set_plp_value." );
        rc = PLM_UNKNOWN_DATATYPE;
        break;
    }

    return rc;
}

static
enum _plm_status set_ushort_plp_value( U_SHORT x,
                                       void *dest, enum _plp_types type )
{
    enum _plm_status rc = PLM_SUCCESS;

    switch ( type )
    {
    case PLP_NULL:
        break;
    case PLP_UBYTE:
        *(uint8_t *) dest = x;
        break;
    case PLP_UCHAR:
        *(unsigned char *) dest = x;
        break;
    case PLP_USHORT:
        *(U_SHORT *) dest = x;
        break;
    case PLP_SHORT:
        *(short *) dest = x;
        break;
    case PLP_PLINT:
        *(PLINT *) dest = x;
        break;
    case PLP_PLFLT:
        *(PLFLT *) dest = x;
        break;
    default:
        plwarn( "Unhandled datatype conversion in set_plp_value." );
        rc = PLM_UNKNOWN_DATATYPE;
        break;
    }

    return rc;
}

static
enum _plm_status set_ulong_plp_value( unsigned long x,
                                      void *dest, enum _plp_types type )
{
    enum _plm_status rc = PLM_SUCCESS;

    switch ( type )
    {
    case PLP_NULL:
        break;
    case PLP_UBYTE:
        *(uint8_t *) dest = x;
        break;
    case PLP_UCHAR:
        *(unsigned char *) dest = x;
        break;
    case PLP_USHORT:
        *(U_SHORT *) dest = x;
        break;
    case PLP_SHORT:
        *(short *) dest = x;
        break;
    case PLP_PLINT:
        *(PLINT *) dest = x;
        break;
    case PLP_PLFLT:
        *(PLFLT *) dest = x;
        break;
    default:
        plwarn( "Unhandled datatype conversion in set_plp_value." );
        rc = PLM_UNKNOWN_DATATYPE;
        break;
    }

    return rc;
}

static
enum _plm_status set_ieeef_plp_value( float x,
                                      void *dest, enum _plp_types type )
{
    enum _plm_status rc = PLM_SUCCESS;

    switch ( type )
    {
    case PLP_NULL:
        break;
    case PLP_UBYTE:
        *(uint8_t *) dest = x;
        break;
    case PLP_UCHAR:
        *(unsigned char *) dest = x;
        break;
    case PLP_USHORT:
        *(U_SHORT *) dest = x;
        break;
    case PLP_SHORT:
        *(short *) dest = x;
        break;
    case PLP_PLINT:
        *(PLINT *) dest = x;
        break;
    case PLP_PLFLT:
        *(PLFLT *) dest = x;
        break;
    default:
        plwarn( "Unhandled datatype conversion in set_plp_value." );
        rc = PLM_UNKNOWN_DATATYPE;
        break;
    }

    return rc;
}

static
enum _plm_status read_entry( PDFstrm *plm,
                             enum _pdf_types from_type,
                             enum _plp_types to_type,
                             void *dest )
{
    uint8_t          b;
    unsigned long    l;
    U_SHORT          x;
    float            f;
    enum _plm_status rc;

    switch ( from_type )
    {
    case PLP_NULL:
        rc = PLM_SUCCESS;
        break;

    case PDF_UBYTE:     // Unsigned one-byte integer
        pdf_rd_1byte( plm, &b );
        rc = set_ubyte_plp_value( b, dest, to_type );
        break;

    case PDF_USHORT:      // Unsigned two-byte integer
        pdf_rd_2bytes( plm, &x );
        rc = set_ushort_plp_value( x, dest, to_type );
        break;

    case PDF_ULONG:      // Unsigned four-byte integer
        pdf_rd_4bytes( plm, &l );
        rc = set_ulong_plp_value( l, dest, to_type );
        break;

    case PDF_IEEEF:      // IEEE 32 bit float
        pdf_rd_ieeef( plm, &f );
        rc = set_ieeef_plp_value( f, dest, to_type );
        break;

    default:
        plwarn( "Unhandled datatype conversion in read_entry." );
        rc = PLM_UNKNOWN_DATATYPE;
        break;
    }

    return rc;
}

//--------------------------------------------------------------------------
// read_metafile_header()
//
// Attempts to read the metafile header information in order to see if
// is a metafile and identify the version.
//--------------------------------------------------------------------------
static
enum _plm_status read_metafile_header( PDFstrm *plm, PLmDev *dev )
{
    char  buffer[MAX_BUFFER];
    PLINT version;
    PLINT num_format_entries = sizeof ( metafile_format ) / sizeof ( struct _plm_version );

    dbug_enter( "read_metafile_enter()" );

    // Attempt to identify that this is a PLplot metafile
    plm_rd( pdf_rd_header( plm, buffer ) );
    if ( strcmp( buffer, PLMETA_HEADER ) != 0 )
    {
        return PLM_NOT_PLMETA_FILE;
    }

    // Attempt to identify the version number
    plm_rd( pdf_rd_header( plm, buffer ) );
    for ( version = 0;
          version < num_format_entries
          && strcmp( metafile_format[version].identifier, buffer ) != 0;
          version++ )
        ;

    if ( version >= num_format_entries )
        return ( PLM_UNKNOWN_VERSION );

    dev->version = version;

    return PLM_SUCCESS;
}

//--------------------------------------------------------------------------
// read_header()
//
// Read a header block from the plot metafile.
//
// NOTE:  Currently we enforce rigid adherence to the order
// specified by the format.  This can be changed so that the
// entries a looked up instead.
//--------------------------------------------------------------------------
static
enum _plm_status read_header( PDFstrm *plm,
                              const struct _plm_format *header,
                              uint8_t * dest )
{
    char             buffer[MAX_BUFFER];
    unsigned int     entry;
    enum _plm_status rc;

    for ( entry = 0;
          header[entry].name != NULL;
          entry++ )
    {
        // Get the name of this field and verify it is correct
        plm_rd( pdf_rd_header( plm, buffer ) );
        if ( strcmp( header[entry].name, buffer ) != 0 )
        {
            return PLM_FORMAT_ERROR;
        }

        rc = read_entry( plm,
            header[entry].pdf_type,
            header[entry].plp_type,
            dest + header[entry].offset );

        if ( rc != PLM_SUCCESS )
            return rc;
    }

    return PLM_SUCCESS;
}

//--------------------------------------------------------------------------
// read_line()
//
// Process a LINE command from the metafile
//--------------------------------------------------------------------------
static
enum _plm_status read_line( PDFstrm *plm, PLmDev *dev, PLStream *pls )
{
    PLFLT            x_list[2], y_list[2];
    enum _plm_status rc;

    // Read the start and end points
    // The metafile stores the points as x,y pairs
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &x_list[0] );  // x1
    if ( rc != PLM_SUCCESS )
        return rc;
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &y_list[0] );  // y1
    if ( rc != PLM_SUCCESS )
        return rc;
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &x_list[1] );  // x2
    if ( rc != PLM_SUCCESS )
        return rc;
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &y_list[1] );  // y2
    if ( rc != PLM_SUCCESS )
        return rc;

    // Draw the line
    plline( 2, x_list, y_list );

    // Preserve the last XY coords for the LINETO command
    dev->xold = (short) x_list[1];
    dev->yold = (short) y_list[1];

    return PLM_SUCCESS;
}

//--------------------------------------------------------------------------
// read_lineto()
//
// Process a LINE command from the metafile
//--------------------------------------------------------------------------
static
enum _plm_status read_lineto( PDFstrm *plm, PLmDev *dev, PLStream *pls )
{
    PLFLT            x_list[2], y_list[2];
    int              i;
    enum _plm_status rc;

    // Set the start to the last known position
    x_list[0] = (PLFLT) dev->xold;
    y_list[0] = (PLFLT) dev->yold;

    // Read the end point
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &x_list[1] );
    if ( rc != PLM_SUCCESS )
        return rc;
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &y_list[1] );
    if ( rc != PLM_SUCCESS )
        return rc;

    // Draw the line
    plline( 2, x_list, y_list );

    // Preserve the last XY coords for the LINETO command
    dev->xold = (short) x_list[1];
    dev->yold = (short) y_list[1];

    return PLM_SUCCESS;
}

//--------------------------------------------------------------------------
// read_lineto()
//
// Process a POLYLINE command from the metafile
//--------------------------------------------------------------------------
static
enum _plm_status read_polyline( PDFstrm *plm, PLmDev *dev, PLStream *pls )
{
    PLINT            i, npts;
    PLFLT            *x_list, *y_list;
    enum _plm_status rc;

    // Read the number of control points
    rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &npts );
    if ( rc != PLM_SUCCESS )
        return rc;

    // Not optimal to alloc/dealloc memory, but it works for now
    if ( ( x_list = malloc( npts * sizeof ( PLFLT ) ) ) == NULL )
    {
        plexit( "read_polyline: Unable to allocate memory\n" );
    }
    if ( ( y_list = malloc( npts * sizeof ( PLFLT ) ) ) == NULL )
    {
        plexit( "read_polyline: Unable to allocate memory\n" );
    }

    // Read the points and insert into the plot buffer
    // The x values
    for ( i = 0; i < npts; i++ )
    {
        rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, x_list + i );
        if ( rc != PLM_SUCCESS )
            return rc;
    }
    // The y values
    for ( i = 0; i < npts; i++ )
    {
        rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, y_list + i );
        if ( rc != PLM_SUCCESS )
            return rc;
    }

    // Draw the line
    plline( npts, x_list, y_list );

    // Preserve the last X coord for the LINETO command
    dev->xold = (short) x_list[npts - 1];
    dev->yold = (short) y_list[npts - 1];

    free( x_list );
    free( y_list );

    return PLM_SUCCESS;
}

//--------------------------------------------------------------------------
// read_escape()
//
// Process a escape command from the metafile
//--------------------------------------------------------------------------
static
enum _plm_status read_escape( PDFstrm *plm, PLmDev *dev, PLStream *pls )
{
    uint8_t          op;
    enum _plm_status rc = PLM_SUCCESS;

    // Read the state operation, return if an error
    if ( pdf_rd_1byte( plm, &op ) != 0 )
        return PLM_FORMAT_ERROR;

    switch ( op )
    {
    case PLESC_FILL:
    {
        PLINT i, npts;
        PLFLT *x_list, *y_list;

        // Get the number of control points for the fill
        rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &npts );
        if ( rc != PLM_SUCCESS )
            return rc;

        // Not optimal to alloc/dealloc memory, but it works for now
        if ( ( x_list = malloc( npts * sizeof ( PLFLT ) ) ) == NULL )
        {
            plexit( "read_escape: Unable to allocate memory\n" );
        }
        if ( ( y_list = malloc( npts * sizeof ( PLFLT ) ) ) == NULL )
        {
            plexit( "read_escape: Unable to allocate memory\n" );
        }

        for ( i = 0; i < npts; i++ )
        {
            rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, x_list + i );
            if ( rc != PLM_SUCCESS )
                return rc;

            rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, y_list + i );
            if ( rc != PLM_SUCCESS )
                return rc;
        }

        plfill( npts, x_list, y_list );

        free( x_list );
        free( y_list );
    }
    break;

    case PLESC_SWIN:
        rc = PLM_SUCCESS;
        break;

    default:
        break;
    }

    return rc;
}

//--------------------------------------------------------------------------
// read_state()
//
// Process a state command from the metafile
//--------------------------------------------------------------------------
static
enum _plm_status read_state( PDFstrm *plm, PLmDev *dev, PLStream *pls )
{
    uint8_t          op;
    enum _plm_status rc = PLM_SUCCESS;

    // Read the state operation, return if an error
    if ( pdf_rd_1byte( plm, &op ) != 0 )
        return PLM_FORMAT_ERROR;

    switch ( op )
    {
    case PLSTATE_WIDTH:
        pldebug( "state: WIDTH" );

        {
            PLFLT width;

            rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &width );
            if ( rc != PLM_SUCCESS )
                return rc;

            plwidth( width );
        }
        break;

    case PLSTATE_COLOR0:
        pldebug( "state: COLOR0" );

        {
            PLINT icol;

            // Read the color index number
            rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &icol );
            if ( rc != PLM_SUCCESS )
                return rc;

            if ( icol == PL_RGB_COLOR )
            {
                // Set pen 0 to the specified RGB value
                PLColor color;

                rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &color.r );
                if ( rc != PLM_SUCCESS )
                    return rc;

                rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &color.g );
                if ( rc != PLM_SUCCESS )
                    return rc;

                rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &color.b );
                if ( rc != PLM_SUCCESS )
                    return rc;

                color.a    = 1.0;
                color.name = NULL;

                plscol0( icol, color.r, color.g, color.b );
            }
            else
            {
                // Set color for pen 0
                plcol0( icol );
            }
        }
        break;

    case PLSTATE_COLOR1:
        pldebug( "state: COLOR1" );

        {
            PLINT icol;

            // Read the color index number
            rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &icol );
            if ( rc != PLM_SUCCESS )
                return rc;

            plcol1( icol );
        }
        break;

    case PLSTATE_FILL:
        pldebug( "state: FILL" );

        {
            U_CHAR pattern;

            // Read the color index number
            rc = read_entry( plm, PDF_USHORT, PLP_UCHAR, &pattern );
            if ( rc != PLM_SUCCESS )
                return rc;

            plpsty( pattern );
        }
        break;

    case PLSTATE_CMAP0:
        pldebug( "state: CMAP0" );

        {
            PLINT   i, ncol;
            PLColor color;

            // Read the number of colors
            rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &ncol );
            if ( rc != PLM_SUCCESS )
                return rc;

            // Set the number of colors in the colormap
            plscmap0n( ncol );

            // Read the colormap
            for ( i = 0; i < ncol; i++ )
            {
                rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &color.r );
                if ( rc != PLM_SUCCESS )
                    return rc;

                rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &color.g );
                if ( rc != PLM_SUCCESS )
                    return rc;

                rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &color.b );
                if ( rc != PLM_SUCCESS )
                    return rc;

                color.a    = 1.0;
                color.name = NULL;

                // Save the color for this entry
                plsc->cmap0[i].r    = color.r;
                plsc->cmap0[i].g    = color.g;
                plsc->cmap0[i].b    = color.b;
                plsc->cmap0[i].a    = color.a;
                plsc->cmap0[i].name = color.name;
            }

            if ( plsc->level > 0 )
                plP_state( PLSTATE_CMAP0 );
        }
        break;

    case PLSTATE_CMAP1:
        pldebug( "state: CMAP0" );

        {
            PLINT   i, ncol;
            PLColor color;

            // Read the number of colors
            rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &ncol );
            if ( rc != PLM_SUCCESS )
                return rc;

            // Set the number of colors in the colormap
            plscmap1n( ncol );

            // Read the colormap
            for ( i = 0; i < ncol; i++ )
            {
                rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &color.r );
                if ( rc != PLM_SUCCESS )
                    return rc;

                rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &color.g );
                if ( rc != PLM_SUCCESS )
                    return rc;

                rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &color.b );
                if ( rc != PLM_SUCCESS )
                    return rc;

                color.a    = 1.0;
                color.name = NULL;

                // Save the color for this entry
                plsc->cmap1[i].r    = color.r;
                plsc->cmap1[i].g    = color.g;
                plsc->cmap1[i].b    = color.b;
                plsc->cmap1[i].a    = color.a;
                plsc->cmap1[i].name = color.name;
            }

            if ( plsc->level > 0 )
                plP_state( PLSTATE_CMAP1 );
        }
        break;

    case PLSTATE_CHR:
        pldebug( "state: CHR" );

        // Note: The operation is not put into the buffer because
        // this operation is not supported in the 2005 format.
        // This code is a placeholder until a new version is made
        // Put the command into the plot buffer

        if ( 0 )
        {
            PLFLT chrdef;
            PLFLT chrht;

            rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &chrdef );
            if ( rc != PLM_SUCCESS )
                return rc;

            rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &chrht );
            if ( rc != PLM_SUCCESS )
                return rc;
        }
        break;

    case PLSTATE_SYM:
        pldebug( "state: SYM" );

        // Note: The operation is not put into the buffer because
        // this operation is not supported in the 2005 format.
        // This code is a placeholder until a new version is made
        // Put the command into the plot buffer

        if ( 0 )
        {
            PLFLT symdef;
            PLFLT symht;

            rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &symdef );
            if ( rc != PLM_SUCCESS )
                return rc;

            rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &symht );
            if ( rc != PLM_SUCCESS )
                return rc;
        }
        break;

    default:
        pldebug( "state: INVALID STATE" );
        return PLM_INVALID_STATE;
    }

    return PLM_SUCCESS;
}

//--------------------------------------------------------------------------
// read_plot_commands()
//
// Reads the plot commands from the metafile and places them into the
// plot buffer
//--------------------------------------------------------------------------
static
enum _plm_status read_plot_commands( PDFstrm *plm, PLmDev *dev, PLStream *pls )
{
    uint8_t          cmd;
    enum _plm_status rc = PLM_SUCCESS;

    dbug_enter( "read_plot_commands()" );

    // Read the metafile until a non-zero result occurs, which typically
    // indicates an end-of-file condition
    while ( rc == PLM_SUCCESS && pdf_rd_1byte( plm, &cmd ) == 0 )
    {
        switch ( cmd )
        {
        case INITIALIZE:
            pldebug( "cmd: INITIALIZE" );
            // No action needed
            break;

        case CLOSE:
            pldebug( "cmd: CLOSE" );
            // No action needed
            break;

        case EOP:
            pldebug( "cmd: EOP" );
            // No action needed
            break;

        case BOP:
        case BOP0:                   // First BOP in a file
            pldebug( "cmd: BOP/BOP0" );

            // Read the metadata for this page
            rc = read_entry( plm,
                page_2005_header[0].pdf_type,
                page_2005_header[0].plp_type,
                (uint8_t *) dev + page_2005_header[0].offset );
            if ( rc != PLM_SUCCESS )
                break;

            rc = read_entry( plm,
                page_2005_header[1].pdf_type,
                page_2005_header[1].plp_type,
                (uint8_t *) dev + page_2005_header[1].offset );
            if ( rc != PLM_SUCCESS )
                break;

            rc = read_entry( plm,
                page_2005_header[2].pdf_type,
                page_2005_header[2].plp_type,
                (uint8_t *) dev + page_2005_header[2].offset );
            if ( rc != PLM_SUCCESS )
                break;


            // Setup the subwindows
            //plP_ssub( pls->nsubx, pls->nsuby, pls->cursub );
            plP_setsub();

            plvpor(
                dev->vpxmin, dev->vpxmax,
                dev->vpymin, dev->vpymax );
            plwind(
                (PLFLT) dev->xmin, (PLFLT) dev->xmax,
                (PLFLT) dev->ymin, (PLFLT) dev->ymax );

            break;

        case LINE:
            pldebug( "cmd: LINE" );

            rc = read_line( plm, dev, pls );
            break;

        case LINETO:
            pldebug( "cmd: LINETO" );

            rc = read_lineto( plm, dev, pls );
            break;

        case ESCAPE:
            pldebug( "cmd: ESCAPE" );

            rc = read_escape( plm, dev, pls );
            break;

        case POLYLINE:
            pldebug( "cmd: POLYLINE" );

            rc = read_polyline( plm, dev, pls );
            break;

        case CHANGE_STATE:
            pldebug( "cmd: CHANGE_STATE" );

            rc = read_state( plm, dev, pls );
            break;

        case END_OF_FIELD:
            pldebug( "cmd: EOF" );

            // No action needed

            break;

        case SWITCH_TO_TEXT:        // Obsolete, replaced by ESCAPE
        case SWITCH_TO_GRAPH:       // Obsolete, replaced by ESCAPE
        case NEW_COLOR:             // Obsolete, replaced by CHANGE_STATE
        case NEW_WIDTH:             // Obsolete, replaced by CHANGE_STATE
        case ADVANCE:               // Obsolete, BOP/EOP used instead
        case NEW_COLOR1:
            pldebug( "cmd: OBSOLETE CMD" );
            plabort( "OBSOLETE CMD" );

            break;

        default:
            pldebug( "cmd: INVALID CMD" );
            plabort( "INVALID CMD" );

            return PLM_INVALID_CMD;
        }
    }

    return PLM_SUCCESS;
}

static
void setup_page( PLmDev *dev )
{
    PLFLT aspect;
    PLINT dev_xmin, dev_ymin;
    PLINT dev_xmax, dev_ymax;
    PLINT dev_xlen, dev_ylen;
    PLFLT dev_xpmm, dev_ypmm;
    PLFLT dev_aspect;
    PLFLT ratio;
    PLFLT vpxlen, vpylen;
    PLINT nsubx, nsuby, cursub;

    // Determine the current subpage
    plP_gsub( &nsubx, &nsuby, &cursub );

    // Are we on a valid subpage?
    if ( cursub == 0 || cursub > nsubx * nsuby )
    {
        // No, it appears that the current subpage is unintialized or the
        // subpage is not valid for the current supbage configuration
        cursub = 1;

        // Set the subpage information
        plP_ssub( nsubx, nsuby, cursub );
        plP_setsub();
    }

    // Set aspect ratio to the natural ratio of the metafile coordinate system.

    // For older versions of plplot, the length is not set.  Do it here
    // just in case.
    dev->xlen = dev->xmax - dev->xmin;
    dev->ylen = dev->ymax - dev->ymin;

    // Aspect ration of the plot metafile
    aspect = ( ( (PLFLT) dev->ylen ) / dev->pxly )
             / ( ( (PLFLT) dev->xlen ) / dev->pxlx );

    // Aspect ratio of the current output device
    plP_gphy( &dev_xmin, &dev_xmax, &dev_ymin, &dev_ymax );
    plP_gpixmm( &dev_xpmm, &dev_ypmm );

    dev_xlen = dev_xmax - dev_xmin;
    dev_ylen = dev_ymax - dev_ymin;

    dev_aspect = ( ( (PLFLT) dev_ylen ) / dev_ypmm )
                 / ( ( (PLFLT) dev_xlen ) / dev_xpmm );

    if ( dev_aspect <= 0. )
        fprintf( stderr, "Aspect ratio error: dev_aspect = %f\n", dev_aspect );

    ratio = aspect / dev_aspect;

    // Default relative coordinate space
    vpxlen      = 1.0;
    vpylen      = 1.0;
    dev->vpxmin = 0.5 - vpxlen / 2.;
    dev->vpymin = 0.5 - vpylen / 2.;
    dev->vpxmax = dev->vpxmin + vpxlen;
    dev->vpymax = dev->vpymin + vpylen;

    //
    // Construct viewport that preserves the aspect ratio of the original
    // device (plmeta output file).  Thus you automatically get all physical
    // coordinate plots to come out correctly.  Note: this could also be done
    // using the driver interface function plsdimap.
    //

    if ( ratio <= 0 )
        fprintf( stderr, "Aspect ratio error: ratio = %f\n", ratio );
    else if ( ratio < 1 )
        vpylen = ratio;
    else
        vpxlen = 1. / ratio;

    dev->vpxmin = ( 1. - vpxlen ) / 2.;
    dev->vpxmax = dev->vpxmin + vpxlen;

    dev->vpymin = ( 1. - vpylen ) / 2.;
    dev->vpymax = dev->vpymin + vpylen;
}

//--------------------------------------------------------------------------
// plreadmetafile()
//
//! Reads a PLplot metafile and uses the current plot stream to display
//! the contents.  If the plot stream has not been initialized, this
//! routine will attempt to intialize the plot stream via a plinit()
//! call.  For an initialized plot stream, the metafile will start at the
//! current page/subpage.
//!
//! @param infile  Input PLplot metafile name.
//!
//! Pass NULL for infile to use the filename passed from the command line
//! option -mfi.
//!
//! Returns void
//--------------------------------------------------------------------------
void plreadmetafile( char *infile )
{
    PDFstrm          *plm;
    PLStream         mf_pls;
    PLmDev           dev;
    PLmIndex         index;
    enum _plm_status rc;

    if ( plsc->mf_infile == NULL && infile == NULL )
    {
        plexit( "No PLplot metafile set for input" );
    }
    else if ( infile != NULL )
    {
        plm = pdf_fopen( infile, "rb" );
    }
    else
    {
        plm = pdf_fopen( plsc->mf_infile, "rb" );
    }
    if ( plm == NULL )
    {
        plexit( "Unable to open PLplot metafile for input" );
    }

    // Read the file header
    if ( ( rc = read_metafile_header( plm, &dev ) ) != PLM_SUCCESS )
    {
        pdf_close( plm );
        plwarn( "Failed to parse PLplot metafile, ignoring file." );
        return;
    }

    // Read the index header
    rc = read_header( plm,
        metafile_format[dev.version].index_header,
        (U_CHAR *) &index );
    if ( rc != PLM_SUCCESS )
    {
        pdf_close( plm );
        plwarn( "Corrupted index in metafile, ignoring file." );
        return;
    }

    // Read the device header
    rc = read_header( plm,
        metafile_format[dev.version].device_header,
        (U_CHAR *) &dev );
    if ( rc != PLM_SUCCESS )
    {
        pdf_close( plm );
        plwarn( "Corrupted device information in metafile, ignoring file." );
        return;
    }

    // Read the plot header into a local version of a plot stream.
    // We do this because some of the parameters from the metafile may
    // be invalid or inappropriate for the current plot device
    // (e.g. xlength = 0).  The plspage() call should be smart enough
    // to setup the page correctly.
    rc = read_header( plm,
        metafile_format[dev.version].plot_header,
        (U_CHAR *) &mf_pls );
    if ( rc != PLM_SUCCESS )
    {
        pdf_close( plm );
        plwarn( "Corrupted device information in metafile, ignoring file." );
        return;
    }

    // Set the physical characterisics of the output device if the current
    // stream has not been initialized
    if ( plsc->level == 0 )
    {
        plspage( mf_pls.xdpi, mf_pls.ydpi,
            mf_pls.xlength, mf_pls.ylength,
            mf_pls.xoffset, mf_pls.yoffset );

        plinit();
    }

    // Configure the plot match the configuration used when the plot
    // was created
    setup_page( &dev );

    // At this point we should be in the plot commands
    rc = read_plot_commands( plm, &dev, plsc );
    if ( rc != PLM_SUCCESS )
    {
        pdf_close( plm );
        plwarn( "Corrupted plot information in metafile, ignoring file." );
        return;
    }

    pdf_close( plm );
}
