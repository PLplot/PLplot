/* $Id$
   $Log$
   Revision 1.6  1993/07/02 07:19:58  mjl
   Changed over to new namespace.

 * Revision 1.5  1993/04/26  20:00:09  mjl
 * The beginnings of a TK driver added.
 *
 * Revision 1.4  1993/03/15  21:45:05  mjl
 * Changed _clear/_page driver functions to the names _eop/_bop, to be
 * more representative of what's actually going on.
 *
 * Revision 1.3  1993/03/03  19:42:15  mjl
 * Changed PLSHORT -> short everywhere; now all device coordinates are expected
 * to fit into a 16 bit address space (reasonable, and good for performance).
 *
 * Revision 1.2  1993/02/23  04:49:42  mjl
 * Eliminated references to the xxx_adv (gradv) driver functions.
 *
 * Revision 1.1  1993/01/23  05:28:31  mjl
 * Added this file for inclusion by all drivers, in lieu of what was
 * previously dispatch.h.  This now has only driver prototypes -- NO
 * data structure declarations.
 *
*/

/*	drivers.h

	Contains all prototypes for driver functions.
*/

#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include "plstream.h"

#ifdef __cplusplus
extern "C" {
#endif

void plD_init_xte		(PLStream *);
void plD_line_xte		(PLStream *, short, short, short, short);
void plD_polyline_xte		(PLStream *, short *, short *, PLINT);
void plD_eop_xte		(PLStream *);
void plD_bop_xte		(PLStream *);
void plD_tidy_xte		(PLStream *);
void plD_color_xte		(PLStream *);
void plD_text_xte		(PLStream *);
void plD_graph_xte		(PLStream *);
void plD_width_xte		(PLStream *);
void plD_esc_xte		(PLStream *, PLINT, void *);

void plD_init_tk		(PLStream *);
void plD_line_tk		(PLStream *, short, short, short, short);
void plD_polyline_tk		(PLStream *, short *, short *, PLINT);
void plD_eop_tk			(PLStream *);
void plD_bop_tk			(PLStream *);
void plD_tidy_tk		(PLStream *);
void plD_color_tk		(PLStream *);
void plD_text_tk		(PLStream *);
void plD_graph_tk		(PLStream *);
void plD_width_tk		(PLStream *);
void plD_esc_tk			(PLStream *, PLINT, void *);

void plD_init_xw		(PLStream *);
void plD_line_xw		(PLStream *, short, short, short, short);
void plD_polyline_xw		(PLStream *, short *, short *, PLINT);
void plD_eop_xw			(PLStream *);
void plD_bop_xw			(PLStream *);
void plD_tidy_xw		(PLStream *);
void plD_color_xw		(PLStream *);
void plD_text_xw		(PLStream *);
void plD_graph_xw		(PLStream *);
void plD_width_xw		(PLStream *);
void plD_esc_xw			(PLStream *, PLINT, void *);

void plD_init_tekt		(PLStream *);
void plD_init_tekf		(PLStream *);
void plD_line_tek		(PLStream *, short, short, short, short);
void plD_polyline_tek		(PLStream *, short *, short *, PLINT);
void plD_eop_tek		(PLStream *);
void plD_bop_tek		(PLStream *);
void plD_tidy_tek		(PLStream *);
void plD_color_tek		(PLStream *);
void plD_text_tek		(PLStream *);
void plD_graph_tek		(PLStream *);
void plD_width_tek		(PLStream *);
void plD_esc_tek		(PLStream *, PLINT, void *);

void plD_init_dg		(PLStream *);
void plD_line_dg		(PLStream *, short, short, short, short);
void plD_polyline_dg		(PLStream *, short *, short *, PLINT);
void plD_eop_dg			(PLStream *);
void plD_bop_dg			(PLStream *);
void plD_tidy_dg		(PLStream *);
void plD_color_dg		(PLStream *);
void plD_text_dg		(PLStream *);
void plD_graph_dg		(PLStream *);
void plD_width_dg		(PLStream *);
void plD_esc_dg			(PLStream *, PLINT, void *);

void plD_init_hp7470		(PLStream *);
void plD_line_hp7470		(PLStream *, short, short, short, short);
void plD_polyline_hp7470	(PLStream *, short *, short *, PLINT);
void plD_eop_hp7470		(PLStream *);
void plD_bop_hp7470		(PLStream *);
void plD_tidy_hp7470		(PLStream *);
void plD_color_hp7470		(PLStream *);
void plD_text_hp7470		(PLStream *);
void plD_graph_hp7470		(PLStream *);
void plD_width_hp7470		(PLStream *);
void plD_esc_hp7470		(PLStream *, PLINT, void *);

void plD_init_hp7580		(PLStream *);
void plD_line_hp7580		(PLStream *, short, short, short, short);
void plD_polyline_hp7580	(PLStream *, short *, short *, PLINT);
void plD_eop_hp7580		(PLStream *);
void plD_bop_hp7580		(PLStream *);
void plD_tidy_hp7580		(PLStream *);
void plD_color_hp7580		(PLStream *);
void plD_text_hp7580		(PLStream *);
void plD_graph_hp7580		(PLStream *);
void plD_width_hp7580		(PLStream *);
void plD_esc_hp7580		(PLStream *, PLINT, void *);

void plD_init_imp		(PLStream *);
void plD_line_imp		(PLStream *, short, short, short, short);
void plD_polyline_imp		(PLStream *, short *, short *, PLINT);
void plD_eop_imp		(PLStream *);
void plD_bop_imp		(PLStream *);
void plD_tidy_imp		(PLStream *);
void plD_color_imp		(PLStream *);
void plD_text_imp		(PLStream *);
void plD_graph_imp		(PLStream *);
void plD_width_imp		(PLStream *);
void plD_esc_imp		(PLStream *, PLINT, void *);

void plD_init_xfig		(PLStream *);
void plD_line_xfig		(PLStream *, short, short, short, short);
void plD_polyline_xfig		(PLStream *, short *, short *, PLINT);
void plD_eop_xfig		(PLStream *);
void plD_bop_xfig		(PLStream *);
void plD_tidy_xfig		(PLStream *);
void plD_color_xfig		(PLStream *);
void plD_text_xfig		(PLStream *);
void plD_graph_xfig		(PLStream *);
void plD_width_xfig		(PLStream *);
void plD_esc_xfig		(PLStream *, PLINT, void *);

void plD_init_jet		(PLStream *);
void plD_line_jet		(PLStream *, short, short, short, short);
void plD_polyline_jet		(PLStream *, short *, short *, PLINT);
void plD_eop_jet		(PLStream *);
void plD_bop_jet		(PLStream *);
void plD_tidy_jet		(PLStream *);
void plD_color_jet		(PLStream *);
void plD_text_jet		(PLStream *);
void plD_graph_jet		(PLStream *);
void plD_width_jet		(PLStream *);
void plD_esc_jet		(PLStream *, PLINT, void *);

void plD_init_ps		(PLStream *);
void plD_line_ps		(PLStream *, short, short, short, short);
void plD_polyline_ps		(PLStream *, short *, short *, PLINT);
void plD_eop_ps			(PLStream *);
void plD_bop_ps			(PLStream *);
void plD_tidy_ps		(PLStream *);
void plD_color_ps		(PLStream *);
void plD_text_ps		(PLStream *);
void plD_graph_ps		(PLStream *);
void plD_width_ps		(PLStream *);
void plD_esc_ps			(PLStream *, PLINT, void *);

void plD_init_nx		(PLStream *);
void plD_line_nx		(PLStream *, short, short, short, short);
void plD_polyline_nx		(PLStream *, short *, short *, PLINT);
void plD_eop_nx			(PLStream *);
void plD_bop_nx			(PLStream *);
void plD_tidy_nx		(PLStream *);
void plD_color_nx		(PLStream *);
void plD_text_nx		(PLStream *);
void plD_graph_nx		(PLStream *);
void plD_width_nx		(PLStream *);
void plD_esc_nx			(PLStream *, PLINT, void *);

void plD_init_plm		(PLStream *);
void plD_line_plm		(PLStream *, short, short, short, short);
void plD_polyline_plm		(PLStream *, short *, short *, PLINT);
void plD_eop_plm		(PLStream *);
void plD_bop_plm		(PLStream *);
void plD_tidy_plm		(PLStream *);
void plD_color_plm		(PLStream *);
void plD_text_plm		(PLStream *);
void plD_graph_plm		(PLStream *);
void plD_width_plm		(PLStream *);
void plD_esc_plm		(PLStream *, PLINT, void *);

void plD_init_vga		(PLStream *);
void plD_line_vga		(PLStream *, short, short, short, short);
void plD_polyline_vga		(PLStream *, short *, short *, PLINT);
void plD_eop_vga		(PLStream *);
void plD_bop_vga		(PLStream *);
void plD_tidy_vga		(PLStream *);
void plD_color_vga		(PLStream *);
void plD_text_vga		(PLStream *);
void plD_graph_vga		(PLStream *);
void plD_width_vga		(PLStream *);
void plD_esc_vga		(PLStream *, PLINT, void *);

void splD_init_vga		(PLStream *);
void splD_line_vga		(PLStream *, short, short, short, short);
void splD_polyline_vga		(PLStream *, short *, short *, PLINT);
void splD_eop_vga		(PLStream *);
void splD_bop_vga		(PLStream *);
void splD_tidy_vga		(PLStream *);
void splD_color_vga		(PLStream *);
void splD_text_vga		(PLStream *);
void splD_graph_vga		(PLStream *);
void splD_width_vga		(PLStream *);
void splD_esc_vga		(PLStream *, PLINT, void *);

void plD_init_os2		(PLStream *);				 
void plD_line_os2		(PLStream *, short, short, short, short);
void plD_polyline_os2		(PLStream *, short *, short *, PLINT);
void plD_eop_os2		(PLStream *);				 
void plD_bop_os2		(PLStream *);				 
void plD_tidy_os2		(PLStream *);				 
void plD_color_os2		(PLStream *);				 
void plD_text_os2		(PLStream *);				 
void plD_graph_os2		(PLStream *);				 
void plD_width_os2		(PLStream *);				 
void plD_esc_os2		(PLStream *, PLINT, void *);		 

void plD_init_amiwn		(PLStream *);
void plD_line_amiwn		(PLStream *, short, short, short, short);
void plD_polyline_amiwn		(PLStream *, short *, short *, PLINT);
void plD_eop_amiwn		(PLStream *);
void plD_bop_amiwn		(PLStream *);
void plD_tidy_amiwn		(PLStream *);
void plD_color_amiwn		(PLStream *);
void plD_text_amiwn		(PLStream *);
void plD_graph_amiwn		(PLStream *);
void plD_width_amiwn		(PLStream *);
void plD_esc_amiwn		(PLStream *, PLINT, void *);

void plD_init_amipr		(PLStream *);
void plD_line_amipr		(PLStream *, short, short, short, short);
void plD_polyline_amipr		(PLStream *, short *, short *, PLINT);
void plD_eop_amipr		(PLStream *);
void plD_bop_amipr		(PLStream *);
void plD_tidy_amipr		(PLStream *);
void plD_color_amipr		(PLStream *);
void plD_text_amipr		(PLStream *);
void plD_graph_amipr		(PLStream *);
void plD_width_amipr		(PLStream *);
void plD_esc_amipr		(PLStream *, PLINT, void *);

void plD_init_iff		(PLStream *);
void plD_line_iff		(PLStream *, short, short, short, short);
void plD_polyline_iff		(PLStream *, short *, short *, PLINT);
void plD_eop_iff		(PLStream *);
void plD_bop_iff		(PLStream *);
void plD_tidy_iff		(PLStream *);
void plD_color_iff		(PLStream *);
void plD_text_iff		(PLStream *);
void plD_graph_iff		(PLStream *);
void plD_width_iff		(PLStream *);
void plD_esc_iff		(PLStream *, PLINT, void *);

void plD_init_aegis		(PLStream *);
void plD_line_aegis		(PLStream *, short, short, short, short);
void plD_polyline_aegis		(PLStream *, short *, short *, PLINT);
void plD_eop_aegis		(PLStream *);
void plD_bop_aegis		(PLStream *);
void plD_tidy_aegis		(PLStream *);
void plD_color_aegis		(PLStream *);
void plD_text_aegis		(PLStream *);
void plD_graph_aegis		(PLStream *);
void plD_width_aegis		(PLStream *);
void plD_esc_aegis		(PLStream *, PLINT, void *);

void plD_init_null		(PLStream *);
void plD_line_null		(PLStream *, short, short, short, short);
void plD_polyline_null		(PLStream *, short *, short *, PLINT);
void plD_eop_null		(PLStream *);
void plD_bop_null		(PLStream *);
void plD_tidy_null		(PLStream *);
void plD_color_null		(PLStream *);
void plD_text_null		(PLStream *);
void plD_graph_null		(PLStream *);
void plD_width_null		(PLStream *);
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
void plbuf_color	(PLStream *);
void plbuf_text		(PLStream *);
void plbuf_graph	(PLStream *);
void plbuf_width	(PLStream *);
void plbuf_esc		(PLStream *, PLINT, void *);

void plRemakePlot	(PLStream *);

#ifdef __cplusplus
}
#endif

#endif	/* __DRIVERS_H__ */
