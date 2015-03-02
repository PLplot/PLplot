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

// Placeholder until decision into which header file this goes into
void plbuf_write( PLStream *pls, void *data, size_t bytes );

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
    PLP_PLFLT,
    PLP_ULONG,
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
    case PLP_ULONG:
        *(U_LONG *) dest = x;
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
    case PLP_ULONG:
        *(U_LONG *) dest = x;
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
    case PLP_ULONG:
        *(U_LONG *) dest = x;
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
    case PLP_ULONG:
        *(U_LONG *) dest = x;
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
    PLFLT            x1, y1, x2, y2;
    short            xy_list[4];
    enum _plm_status rc;

    // Read the start and end points
    // The metafile stores the points as x,y pairs
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &x1 );
    if ( rc != PLM_SUCCESS )
        return rc;
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &y1 );
    if ( rc != PLM_SUCCESS )
        return rc;
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &x2 );
    if ( rc != PLM_SUCCESS )
        return rc;
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &y2 );
    if ( rc != PLM_SUCCESS )
        return rc;

    // Transform the coordinates from the meta device to the current
    // device coordinate system
    xy_list[0] = (short)lround( dev->mfpcxa * x1 + dev->mfpcxb );
    xy_list[2] = (short)lround( dev->mfpcya * y1 + dev->mfpcyb );
    xy_list[1] = (short)lround( dev->mfpcxa * x2 + dev->mfpcxb );
    xy_list[3] = (short)lround( dev->mfpcya * y2 + dev->mfpcyb );
    
    // Put the coordinates into the plot buffer
    plbuf_write( pls, xy_list, sizeof(xy_list) );

    // Preserve the last XY coords for the LINETO command
    dev->xold = (short) xy_list[1];
    dev->yold = (short) xy_list[3];

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
    PLFLT            x2, y2;
    short            xy_list[4];
    int              i;
    enum _plm_status rc;

    // Set the start to the last known position
    xy_list[0] = (PLFLT) dev->xold;
    xy_list[2] = (PLFLT) dev->yold;

    // Read the end point
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &x2 );
    if ( rc != PLM_SUCCESS )
        return rc;
    rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &y2 );
    if ( rc != PLM_SUCCESS )
        return rc;

    // Transform the coordinates from the meta device to the current
    // device coordinate system
    xy_list[1] = (short)lround( dev->mfpcxa * x2 + dev->mfpcxb );
    xy_list[3] = (short)lround( dev->mfpcya * y2 + dev->mfpcyb );
    
    // Put the coordinates into the plot buffer
    plbuf_write( pls, xy_list, sizeof(xy_list) );

    // Preserve the last XY coords for the LINETO command
    dev->xold = (short) xy_list[1];
    dev->yold = (short) xy_list[1];

    return PLM_SUCCESS;
}

//--------------------------------------------------------------------------
// read_polyline()
//
// Process a POLYLINE command from the metafile
//--------------------------------------------------------------------------
static
enum _plm_status read_polyline( PDFstrm *plm, PLmDev *dev, PLStream *pls )
{
    PLINT            i, npts;
    PLFLT            x, y;
    short            xd, yd;
    enum _plm_status rc;

    // Read the number of control points and put into the plot buffer
    rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &npts );
    if ( rc != PLM_SUCCESS )
        return rc;

    plbuf_write( pls, &npts, sizeof(npts) );

    // Read the points and insert into the plot buffer
    // The x values
    for ( i = 0; i < npts; i++ )
    {
        rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &x );
        if ( rc != PLM_SUCCESS )
            return rc;

	// Transform the coordinates from the meta device to the current
	// device coordinate system
	xd = (short)lround( dev->mfpcxa * x + dev->mfpcxb );

	plbuf_write( pls, &xd, sizeof(xd) );
    }
    // Preserve the last X value for the LINETO command
    dev->xold = xd;

    // The y values
    for ( i = 0; i < npts; i++ )
    {
        rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &y );
        if ( rc != PLM_SUCCESS )
            return rc;

	// Transform the coordinates from the meta device to the current
	// device coordinate system
	yd = (short)lround( dev->mfpcya * y + dev->mfpcyb );

	plbuf_write( pls, &yd, sizeof(yd) );
    }
    // Preserve the last Y value for the LINETO command
    dev->yold = yd;

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

    // Put this operation into the plot buffer
    plbuf_write( pls, &op, sizeof(op) );

    switch ( op )
    {
    case PLESC_FILL:
    {
        PLINT i, npts;
	PLFLT x, y;
	short xy[2];

        // Get the number of control points for the fill
        rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &npts );
        if ( rc != PLM_SUCCESS )
            return rc;

	// Write the number of control points into the plot buffer
	plbuf_write( pls, &npts, sizeof(npts) );

        for ( i = 0; i < npts; i++ )
        {
            rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &x );
            if ( rc != PLM_SUCCESS )
                return rc;

            rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &y );
            if ( rc != PLM_SUCCESS )
                return rc;

	    // Transform the coordinates from the meta device to the current
	    // device coordinate system
	    xy[0] = (short)lround( dev->mfpcxa * x + dev->mfpcxb );
	    xy[1] = (short)lround( dev->mfpcya * y + dev->mfpcyb );

	    plbuf_write( pls, xy, sizeof(xy) );
        }
    }
    break;

    case PLESC_SWIN:
        rc = PLM_SUCCESS;
        break;


    // Unicode and non-Unicode text handling
    case PLESC_HAS_TEXT:
    {
        EscText   text;
	PLFLT     xform[4];
        PLUNICODE fci, ch;
	PLINT     i;
	PLFLT     xmin, xmax, ymin, ymax;
	PLFLT     x, y, refx, refy;

	// Setup storage for the transformation matrix
	text.xform = xform;

	// Read the information from the metafile
	rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &pls->chrht );
	rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &pls->diorot );
	rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &xmin );
	rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &xmax );
	rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &ymin );
	rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &ymax );

	rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &text.base );
	rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &text.just );
	rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &text.xform[0] );
	rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &text.xform[1] );
	rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &text.xform[2] );
	rc = read_entry( plm, PDF_IEEEF, PLP_PLFLT, &text.xform[3] );
	rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &x );
	rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &y );
	rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &refx );
	rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &refy );
	rc = read_entry( plm, PDF_UBYTE, PLP_UCHAR, &text.font_face );

	// Translate coordinates to the device coordinate system

	pls->clpxmi = (short)lround( dev->mfpcxa * xmin + dev->mfpcxb );
	pls->clpxma = (short)lround( dev->mfpcxa * xmax + dev->mfpcxb );
	pls->clpymi = (short)lround( dev->mfpcxa * ymin + dev->mfpcxb );
	pls->clpyma = (short)lround( dev->mfpcxa * ymax + dev->mfpcxb );

	text.x = (short)lround( dev->mfpcxa * x + dev->mfpcxb );
	text.y = (short)lround( dev->mfpcya * y + dev->mfpcyb );
	text.refx = (short)lround( dev->mfpcxa * refx + dev->mfpcxb );
	text.refy = (short)lround( dev->mfpcya * refy + dev->mfpcyb );

	// Copy the information that the buffer expects

	plbuf_write( pls, &pls->chrht, sizeof ( pls->chrht ) );
	plbuf_write( pls, &pls->diorot, sizeof ( pls->diorot) );
	plbuf_write( pls, &pls->clpxmi, sizeof ( pls->clpxmi ) );
	plbuf_write( pls, &pls->clpxma, sizeof ( pls->clpxma ) );
	plbuf_write( pls, &pls->clpymi, sizeof ( pls->clpymi ) );
	plbuf_write( pls, &pls->clpyma, sizeof ( pls->clpyma ) );
	
	plbuf_write( pls, &text.base, sizeof ( text.base ) );
	plbuf_write( pls, &text.just, sizeof ( text.just ) );
	plbuf_write( pls, text.xform, sizeof ( text.xform[0] ) * 4 );
	plbuf_write( pls, &text.x, sizeof ( text.x ) );
	plbuf_write( pls, &text.y, sizeof ( text.y ) );
	plbuf_write( pls, &text.refx, sizeof ( text.refx ) );
	plbuf_write( pls, &text.refy, sizeof ( text.refy ) );
	plbuf_write( pls, &text.font_face, sizeof( text.font_face ) );

	// Metafiles use Unicode strings, so store the data needed to
	// handle Unicode strings

        // Retrieve the font characterization integer
	rc = read_entry( plm, PDF_ULONG, PLP_ULONG, &fci );
	plbuf_write( pls, &fci, sizeof ( fci ) );

	// Retrieve the text
	rc = read_entry( plm, PDF_USHORT, PLP_USHORT, &text.unicode_array_len );
	plbuf_write( pls, &text.unicode_array_len, sizeof ( U_SHORT ) );

	for(i = 0; i < text.unicode_array_len; i++) {
	    rc = read_entry( plm, PDF_ULONG, PLP_ULONG, &ch );
	    plbuf_write( pls, &ch, sizeof ( PLUNICODE ));
	}
	
    }
    rc = PLM_SUCCESS;
    break;

    // Alternate unicode text handling
    case PLESC_BEGIN_TEXT:
    case PLESC_TEXT_CHAR:
    case PLESC_CONTROL_CHAR:
    case PLESC_END_TEXT:
        // NOP these for now until a decision is made
        // which method should be implemented for metafiles
        plwarn("plmetafile: Alternate Unicode text handling is not implemented");
	rc = PLM_INVALID_CMD;
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

    // Write the operation to the plot buffer
    plbuf_write( pls, &op, sizeof(op) );

    switch ( op )
    {
    case PLSTATE_WIDTH:
        pldebug( "state: WIDTH" );

        {
            PLFLT width;

            rc = read_entry( plm, PDF_USHORT, PLP_PLFLT, &width );
            if ( rc != PLM_SUCCESS )
                return rc;

	    plbuf_write( pls, &width, sizeof(width) );
        }
        break;

    case PLSTATE_COLOR0:
    case PLSTATE_COLOR1:
        pldebug( "state: COLOR0/COLOR1" );

        {
            PLINT icol;

            // Read the color index number
            rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &icol );
            if ( rc != PLM_SUCCESS )
                return rc;

	    // Write the color index value to the plot buffer
	    plbuf_write( pls, &icol, sizeof(icol) );

	    // Was pen 0 set to an RGB value rather than color index?
            if ( op == PLSTATE_COLOR0 && icol == PL_RGB_COLOR )
            {
                // Get the RGB value and copy to the plot buffer
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

		plbuf_write( pls, &color, sizeof(color) );
            }
        }
        break;

    case PLSTATE_FILL:
        pldebug( "state: FILL" );

        {
            U_CHAR pattern;

            // Read the pattern and put into the plot buffer
            rc = read_entry( plm, PDF_USHORT, PLP_UCHAR, &pattern );
            if ( rc != PLM_SUCCESS )
                return rc;

	    plbuf_write( pls, &pattern, sizeof(pattern) );
        }
        break;

    case PLSTATE_CMAP0:
    case PLSTATE_CMAP1:
        pldebug( "state: CMAP0/CMAP1" );

        {
            PLINT   i, ncol;
            PLColor color;

            // Read the number of colors
            rc = read_entry( plm, PDF_USHORT, PLP_PLINT, &ncol );
            if ( rc != PLM_SUCCESS )
                return rc;

	    // Put the number of colors into the plot buffer
	    plbuf_write( pls, &ncol, sizeof(ncol) );

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

		plbuf_write( pls, &color, sizeof(color) );
            }
        }
        break;

    case PLSTATE_CHR:
        pldebug( "state: CHR" );

	// The 2005 version and earlier do not support this
	// operation, so put in the current value
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

	    plbuf_write( pls, &chrdef, sizeof(chrdef) );
	    plbuf_write( pls, &chrht, sizeof(chrht) );
        }
	else 
	{
	    plbuf_write( pls, &pls->chrdef, sizeof(pls->chrdef) );
	    plbuf_write( pls, &pls->chrht, sizeof(pls->chrht) );
	}
        break;

    case PLSTATE_SYM:
        pldebug( "state: SYM" );

	// The 2005 version and earlier do not support this
	// operation, so put in the current value
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

	    plbuf_write( pls, &symdef, sizeof(symdef) );
	    plbuf_write( pls, &symht, sizeof(symht) );
        }
	else 
	{
	    plbuf_write( pls, &pls->symdef, sizeof(pls->symdef) );
	    plbuf_write( pls, &pls->symht, sizeof(pls->symht) );
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

	    // Put the command into the plot buffer
	    plbuf_write( pls, &cmd, sizeof(cmd) );

            break;

        case CLOSE:
            pldebug( "cmd: CLOSE" );
            // No action needed
            break;

        case EOP:
            pldebug( "cmd: EOP" );

	    // Put the command into the plot buffer
	    plbuf_write( pls, &cmd, sizeof(cmd) );

            break;

        case BOP:
        case BOP0:                   // First BOP in a file
            pldebug( "cmd: BOP/BOP0" );

	    // Put the command into the plot buffer
	    cmd = BOP;
	    plbuf_write( pls, &cmd, sizeof(cmd) );

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


	    // Push state information into the plot buffer
	    plbuf_write( pls, &(pls->icol0), sizeof(pls->icol0) );
	    plbuf_write( pls, &(pls->icol1), sizeof(pls->icol1) );
	    plbuf_write( pls, &(pls->curcolor), sizeof(pls->curcolor) );

            break;

        case LINE:
            pldebug( "cmd: LINE" );

	    // Put the command into the plot buffer
	    plbuf_write( pls, &cmd, sizeof(cmd) );
	    
            rc = read_line( plm, dev, pls );
            break;

        case LINETO:
            pldebug( "cmd: LINETO" );

	    // Put the command into the plot buffer
	    cmd = LINE;
	    plbuf_write( pls, &cmd, sizeof(cmd) );

            rc = read_lineto( plm, dev, pls );
            break;

        case ESCAPE:
            pldebug( "cmd: ESCAPE" );

	    // Put the command into the plot buffer
	    plbuf_write( pls, &cmd, sizeof(cmd) );

            rc = read_escape( plm, dev, pls );
            break;

        case POLYLINE:
            pldebug( "cmd: POLYLINE" );

	    // Put the command into the plot buffer
	    plbuf_write( pls, &cmd, sizeof(cmd) );

            rc = read_polyline( plm, dev, pls );
            break;

        case CHANGE_STATE:
            pldebug( "cmd: CHANGE_STATE" );

	    // Put the command into the plot buffer
	    plbuf_write( pls, &cmd, sizeof(cmd) );

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
void setup_page( PLmDev *mf_dev, PLStream *pls )
{
    PLmDev *dev = pls->dev;

    mf_dev->mfpcxa = (PLFLT) dev->xlen 
        / (PLFLT)( mf_dev->xmax - mf_dev->xmin );
    mf_dev->mfpcxb = (PLFLT) dev->xmin;
    mf_dev->mfpcya = (PLFLT) dev->ylen 
        / (PLFLT)( mf_dev->ymax - mf_dev->ymin );
    mf_dev->mfpcyb = (PLFLT) dev->ymin;
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
    PLmDev           mf_dev;
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
    if ( ( rc = read_metafile_header( plm, &mf_dev ) ) != PLM_SUCCESS )
    {
        pdf_close( plm );
        plwarn( "Failed to parse PLplot metafile, ignoring file." );
        return;
    }

    // Read the index header
    rc = read_header( plm,
        metafile_format[mf_dev.version].index_header,
        (U_CHAR *) &index );
    if ( rc != PLM_SUCCESS )
    {
        pdf_close( plm );
        plwarn( "Corrupted index in metafile, ignoring file." );
        return;
    }

    // Read the device header
    rc = read_header( plm,
        metafile_format[mf_dev.version].device_header,
        (U_CHAR *) &mf_dev );
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
        metafile_format[mf_dev.version].plot_header,
        (U_CHAR *) &mf_pls );
    if ( rc != PLM_SUCCESS )
    {
        pdf_close( plm );
        plwarn( "Corrupted device information in metafile, ignoring file." );
        return;
    }

    // Is the plot stream initialized?
    if( plsc->level == 0 ) {
        // No, we must intialize it in order to get the
        // device configuation set
        plinit();
    }
    setup_page( &mf_dev, plsc);

    // At this point we should be in the plot commands
    rc = read_plot_commands( plm, &mf_dev, plsc );
    if ( rc != PLM_SUCCESS )
    {
        pdf_close( plm );
        plwarn( "Corrupted plot information in metafile, ignoring file." );
        return;
    }

    pdf_close( plm );

    plreplot();
}
