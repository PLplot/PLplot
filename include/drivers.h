/* $Id$
   $Log$
   Revision 1.15  1994/03/23 06:57:46  mjl
   Changed function names in prototypes for xterm, mskermit, and added
   prototypes for versaterm and vlt drivers (minor tek devices).

 * Revision 1.14  1994/01/15  17:32:14  mjl
 * Added include of pdf.h.
 *
 * Revision 1.13  1993/12/21  10:34:40  mjl
 * Added prototype for new dp driver initialization function.
 *
 * Revision 1.12  1993/12/08  20:26:21  mjl
 * Changes to support MS-Kermit output device.
 *
 * Revision 1.11  1993/10/18  19:41:49  mjl
 * Prototypes for Borland C driver under DOS.
 *
 * Revision 1.10  1993/08/03  03:26:51  mjl
 * Additions to support new MGR and LinuxVGA drivers.
 *
 * Revision 1.9  1993/07/31  07:56:50  mjl
 * Several driver functions consolidated, for all drivers.  The width and color
 * commands are now part of a more general "state" command.  The text and
 * graph commands used for switching between modes is now handled by the
 * escape function (very few drivers require it).  The device-specific PLDev
 * structure is now malloc'ed for each driver that requires it, and freed when
 * the stream is terminated.
 *
 * Revision 1.8  1993/07/28  05:48:24  mjl
 * Removed unnecessary function prototypes for old xterm driver, added new
 * ones for tek4107 terminal & file driver.
 *
 * Revision 1.7  1993/07/16  22:25:12  mjl
 * Added explicit support for color vs monochrome postscript output.
 *
 * Revision 1.6  1993/07/02  07:19:58  mjl
 * Changed over to new namespace.
 *
 * Revision 1.5  1993/04/26  20:00:09  mjl
 * The beginnings of a TK driver added.
*/

/*	drivers.h

	Contains all prototypes for driver functions.
*/

#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include "pdf.h"
#include "plstream.h"

#ifdef __cplusplus
extern "C" {
#endif

void plD_init_tk		(PLStream *);
void plD_init_dp		(PLStream *);
void plD_line_tk		(PLStream *, short, short, short, short);
void plD_polyline_tk		(PLStream *, short *, short *, PLINT);
void plD_eop_tk			(PLStream *);
void plD_bop_tk			(PLStream *);
void plD_tidy_tk		(PLStream *);
void plD_state_tk		(PLStream *, PLINT);
void plD_esc_tk			(PLStream *, PLINT, void *);

void plD_init_xw		(PLStream *);
void plD_line_xw		(PLStream *, short, short, short, short);
void plD_polyline_xw		(PLStream *, short *, short *, PLINT);
void plD_eop_xw			(PLStream *);
void plD_bop_xw			(PLStream *);
void plD_tidy_xw		(PLStream *);
void plD_state_xw		(PLStream *, PLINT);
void plD_esc_xw			(PLStream *, PLINT, void *);

void plD_init_xterm		(PLStream *);
void plD_init_tekt		(PLStream *);
void plD_init_tekf		(PLStream *);
void plD_init_tek4107t		(PLStream *);
void plD_init_tek4107f		(PLStream *);
void plD_init_mskermit		(PLStream *);
void plD_init_versaterm		(PLStream *);
void plD_init_vlt		(PLStream *);

void plD_line_tek		(PLStream *, short, short, short, short);
void plD_polyline_tek		(PLStream *, short *, short *, PLINT);
void plD_eop_tek		(PLStream *);
void plD_bop_tek		(PLStream *);
void plD_tidy_tek		(PLStream *);
void plD_state_tek		(PLStream *, PLINT);
void plD_esc_tek		(PLStream *, PLINT, void *);

void plD_init_lxvga		(PLStream *);
void plD_line_lxvga		(PLStream *, short, short, short, short);
void plD_polyline_lxvga		(PLStream *, short *, short *, PLINT);
void plD_eop_lxvga		(PLStream *);
void plD_bop_lxvga		(PLStream *);
void plD_tidy_lxvga		(PLStream *);
void plD_state_lxvga		(PLStream *, PLINT);
void plD_esc_lxvga		(PLStream *, PLINT, void *);

void plD_init_mgr		(PLStream *);
void plD_line_mgr		(PLStream *, short, short, short, short);
void plD_polyline_mgr		(PLStream *, short *, short *, PLINT);
void plD_eop_mgr		(PLStream *);
void plD_bop_mgr		(PLStream *);
void plD_tidy_mgr		(PLStream *);
void plD_state_mgr		(PLStream *, PLINT);
void plD_esc_mgr		(PLStream *, PLINT, void *);

void plD_init_dg		(PLStream *);
void plD_line_dg		(PLStream *, short, short, short, short);
void plD_polyline_dg		(PLStream *, short *, short *, PLINT);
void plD_eop_dg			(PLStream *);
void plD_bop_dg			(PLStream *);
void plD_tidy_dg		(PLStream *);
void plD_state_dg		(PLStream *, PLINT);
void plD_esc_dg			(PLStream *, PLINT, void *);

void plD_init_hp7470		(PLStream *);
void plD_line_hp7470		(PLStream *, short, short, short, short);
void plD_polyline_hp7470	(PLStream *, short *, short *, PLINT);
void plD_eop_hp7470		(PLStream *);
void plD_bop_hp7470		(PLStream *);
void plD_tidy_hp7470		(PLStream *);
void plD_state_hp7470		(PLStream *, PLINT);
void plD_esc_hp7470		(PLStream *, PLINT, void *);

void plD_init_hp7580		(PLStream *);
void plD_line_hp7580		(PLStream *, short, short, short, short);
void plD_polyline_hp7580	(PLStream *, short *, short *, PLINT);
void plD_eop_hp7580		(PLStream *);
void plD_bop_hp7580		(PLStream *);
void plD_tidy_hp7580		(PLStream *);
void plD_state_hp7580		(PLStream *, PLINT);
void plD_esc_hp7580		(PLStream *, PLINT, void *);

void plD_init_imp		(PLStream *);
void plD_line_imp		(PLStream *, short, short, short, short);
void plD_polyline_imp		(PLStream *, short *, short *, PLINT);
void plD_eop_imp		(PLStream *);
void plD_bop_imp		(PLStream *);
void plD_tidy_imp		(PLStream *);
void plD_state_imp		(PLStream *, PLINT);
void plD_esc_imp		(PLStream *, PLINT, void *);

void plD_init_xfig		(PLStream *);
void plD_line_xfig		(PLStream *, short, short, short, short);
void plD_polyline_xfig		(PLStream *, short *, short *, PLINT);
void plD_eop_xfig		(PLStream *);
void plD_bop_xfig		(PLStream *);
void plD_tidy_xfig		(PLStream *);
void plD_state_xfig		(PLStream *, PLINT);
void plD_esc_xfig		(PLStream *, PLINT, void *);

void plD_init_jet		(PLStream *);
void plD_line_jet		(PLStream *, short, short, short, short);
void plD_polyline_jet		(PLStream *, short *, short *, PLINT);
void plD_eop_jet		(PLStream *);
void plD_bop_jet		(PLStream *);
void plD_tidy_jet		(PLStream *);
void plD_state_jet		(PLStream *, PLINT);
void plD_esc_jet		(PLStream *, PLINT, void *);

void plD_init_psm		(PLStream *);
void plD_init_psc		(PLStream *);
void plD_line_ps		(PLStream *, short, short, short, short);
void plD_polyline_ps		(PLStream *, short *, short *, PLINT);
void plD_eop_ps			(PLStream *);
void plD_bop_ps			(PLStream *);
void plD_tidy_ps		(PLStream *);
void plD_state_ps		(PLStream *, PLINT);
void plD_esc_ps			(PLStream *, PLINT, void *);

void plD_init_nx		(PLStream *);
void plD_line_nx		(PLStream *, short, short, short, short);
void plD_polyline_nx		(PLStream *, short *, short *, PLINT);
void plD_eop_nx			(PLStream *);
void plD_bop_nx			(PLStream *);
void plD_tidy_nx		(PLStream *);
void plD_state_nx		(PLStream *, PLINT);
void plD_esc_nx			(PLStream *, PLINT, void *);

void plD_init_plm		(PLStream *);
void plD_line_plm		(PLStream *, short, short, short, short);
void plD_polyline_plm		(PLStream *, short *, short *, PLINT);
void plD_eop_plm		(PLStream *);
void plD_bop_plm		(PLStream *);
void plD_tidy_plm		(PLStream *);
void plD_state_plm		(PLStream *, PLINT);
void plD_esc_plm		(PLStream *, PLINT, void *);

void plD_init_vga		(PLStream *);
void plD_line_vga		(PLStream *, short, short, short, short);
void plD_polyline_vga		(PLStream *, short *, short *, PLINT);
void plD_eop_vga		(PLStream *);
void plD_bop_vga		(PLStream *);
void plD_tidy_vga		(PLStream *);
void plD_state_vga		(PLStream *, PLINT);
void plD_esc_vga		(PLStream *, PLINT, void *);

void plD_init_svga		(PLStream *);
void plD_line_svga		(PLStream *, short, short, short, short);
void plD_polyline_svga		(PLStream *, short *, short *, PLINT);
void plD_eop_svga		(PLStream *);
void plD_bop_svga		(PLStream *);
void plD_tidy_svga		(PLStream *);
void plD_state_svga		(PLStream *, PLINT);
void plD_esc_svga		(PLStream *, PLINT, void *);

void plD_init_bgi		(PLStream *);
void plD_line_bgi		(PLStream *, short, short, short, short);
void plD_polyline_bgi		(PLStream *, short *, short *, PLINT);
void plD_eop_bgi		(PLStream *);
void plD_bop_bgi		(PLStream *);
void plD_tidy_bgi		(PLStream *);
void plD_state_bgi		(PLStream *, PLINT);
void plD_esc_bgi		(PLStream *, PLINT, void *);

void plD_init_os2		(PLStream *);				 
void plD_line_os2		(PLStream *, short, short, short, short);
void plD_polyline_os2		(PLStream *, short *, short *, PLINT);
void plD_eop_os2		(PLStream *);				 
void plD_bop_os2		(PLStream *);				 
void plD_tidy_os2		(PLStream *);				 
void plD_state_os2		(PLStream *, PLINT);
void plD_esc_os2		(PLStream *, PLINT, void *);		 

void plD_init_amiwn		(PLStream *);
void plD_line_amiwn		(PLStream *, short, short, short, short);
void plD_polyline_amiwn		(PLStream *, short *, short *, PLINT);
void plD_eop_amiwn		(PLStream *);
void plD_bop_amiwn		(PLStream *);
void plD_tidy_amiwn		(PLStream *);
void plD_state_amiwn		(PLStream *, PLINT);
void plD_esc_amiwn		(PLStream *, PLINT, void *);

void plD_init_amipr		(PLStream *);
void plD_line_amipr		(PLStream *, short, short, short, short);
void plD_polyline_amipr		(PLStream *, short *, short *, PLINT);
void plD_eop_amipr		(PLStream *);
void plD_bop_amipr		(PLStream *);
void plD_tidy_amipr		(PLStream *);
void plD_state_amipr		(PLStream *, PLINT);
void plD_esc_amipr		(PLStream *, PLINT, void *);

void plD_init_iff		(PLStream *);
void plD_line_iff		(PLStream *, short, short, short, short);
void plD_polyline_iff		(PLStream *, short *, short *, PLINT);
void plD_eop_iff		(PLStream *);
void plD_bop_iff		(PLStream *);
void plD_tidy_iff		(PLStream *);
void plD_state_iff		(PLStream *, PLINT);
void plD_esc_iff		(PLStream *, PLINT, void *);

void plD_init_aegis		(PLStream *);
void plD_line_aegis		(PLStream *, short, short, short, short);
void plD_polyline_aegis		(PLStream *, short *, short *, PLINT);
void plD_eop_aegis		(PLStream *);
void plD_bop_aegis		(PLStream *);
void plD_tidy_aegis		(PLStream *);
void plD_state_aegis		(PLStream *, PLINT);
void plD_esc_aegis		(PLStream *, PLINT, void *);

void plD_init_null		(PLStream *);
void plD_line_null		(PLStream *, short, short, short, short);
void plD_polyline_null		(PLStream *, short *, short *, PLINT);
void plD_eop_null		(PLStream *);
void plD_bop_null		(PLStream *);
void plD_tidy_null		(PLStream *);
void plD_state_null		(PLStream *, PLINT);
void plD_esc_null		(PLStream *, PLINT, void *);

/*----------------------------------------------------------------------*\
* Prototypes for plot buffer calls.
\*----------------------------------------------------------------------*/

void plbuf_init		(PLStream *);
void plbuf_line		(PLStream *, short, short, short, short);
void plbuf_polyline	(PLStream *, short *, short *, PLINT);
void plbuf_eop		(PLStream *);
void plbuf_bop		(PLStream *);
void plbuf_tidy		(PLStream *);
void plbuf_state	(PLStream *, PLINT);
void plbuf_esc		(PLStream *, PLINT, void *);

void plRemakePlot	(PLStream *);

#ifdef __cplusplus
}
#endif

#endif	/* __DRIVERS_H__ */
