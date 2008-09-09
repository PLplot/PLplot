/* $Id$

	Contains all prototypes for driver functions.

    Copyright (C) 2004  Andrew Roach
    Copyright (C) 2005  Thomas J. Duck
    Copyright (C) 2006  Andrew Ross

    This file is part of PLplot.

    PLplot is free software; you can redistribute it and/or modify
    it under the terms of the GNU Library General Public License as published
    by the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    PLplot is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with PLplot; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

*/

#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include "pdf.h"
#include "plstrm.h"

#ifdef __cplusplus
extern "C" {
#endif

PLDLLIMPEXP void plD_dispatch_init_mac8	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_mac1	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_aqt	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_nx	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_os2	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_xw	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_gcw	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_gnome	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_tk	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_vga	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_mgr	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_win3	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_vga	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_vga	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_vga	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_tiff	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_jpg	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_jpeg	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_bmp	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_vga	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_xterm	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_tekt	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_tek4107t	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_mskermit	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_versaterm( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_vlt	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_conex	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_dg	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_plm	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_tekf	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_tek4107f	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_psm	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_psc	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_xfig	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_ljiip	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_ljii	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_hp7470	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_hp7580	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_hpgl	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_imp	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_pbm	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_png	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_gif	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_cgm	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_null	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_tkwin	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_pstex	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_psttfc	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_psttfm	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_ntk	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_mem	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_wingcc	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_wxwidgets	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_svg	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_pdf	( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_xcairo  ( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_pdfcairo  ( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_pscairo  ( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_svgcairo  ( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_pngcairo  ( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_memcairo  ( PLDispatchTable *pdt );
PLDLLIMPEXP void plD_dispatch_init_extcairo  ( PLDispatchTable *pdt );

/* Prototypes for plot buffer calls. */

void plbuf_init		(PLStream *);
void plbuf_line		(PLStream *, short, short, short, short);
void plbuf_polyline	(PLStream *, short *, short *, PLINT);
void plbuf_eop		(PLStream *);
void plbuf_bop		(PLStream *);
void plbuf_tidy		(PLStream *);
void plbuf_state	(PLStream *, PLINT);
void plbuf_esc		(PLStream *, PLINT, void *);
void * plbuf_save	(PLStream *, void *);
void * plbuf_switch	(PLStream *, void *);

void plRemakePlot	(PLStream *);

#ifdef __cplusplus
}
#endif

#endif	/* __DRIVERS_H__ */
