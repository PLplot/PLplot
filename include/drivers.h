/* $Id$
   $Log$
   Revision 1.1  1993/01/23 05:28:31  mjl
   Added this file for inclusion by all drivers, in lieu of what was
   previously dispatch.h.  This now has only driver prototypes -- NO
   data structure declarations.

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

void xte_init		(PLStream *);
void xte_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void xte_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void xte_clear		(PLStream *);
void xte_page		(PLStream *);
void xte_adv		(PLStream *);
void xte_tidy		(PLStream *);
void xte_color		(PLStream *);
void xte_text		(PLStream *);
void xte_graph		(PLStream *);
void xte_width		(PLStream *);
void xte_esc		(PLStream *, PLINT, char *);

void xm_init		(PLStream *);
void xm_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void xm_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void xm_clear		(PLStream *);
void xm_page		(PLStream *);
void xm_adv		(PLStream *);
void xm_tidy		(PLStream *);
void xm_color		(PLStream *);
void xm_text		(PLStream *);
void xm_graph		(PLStream *);
void xm_width		(PLStream *);
void xm_esc		(PLStream *, PLINT, char *);

void xw_init		(PLStream *);
void xw_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void xw_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void xw_clear		(PLStream *);
void xw_page		(PLStream *);
void xw_adv		(PLStream *);
void xw_tidy		(PLStream *);
void xw_color		(PLStream *);
void xw_text		(PLStream *);
void xw_graph		(PLStream *);
void xw_width		(PLStream *);
void xw_esc		(PLStream *, PLINT, char *);

void tekt_init		(PLStream *);
void tekf_init		(PLStream *);
void tek_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void tek_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void tek_clear		(PLStream *);
void tek_page		(PLStream *);
void tek_adv		(PLStream *);
void tek_tidy		(PLStream *);
void tek_color		(PLStream *);
void tek_text		(PLStream *);
void tek_graph		(PLStream *);
void tek_width		(PLStream *);
void tek_esc		(PLStream *, PLINT, char *);

void dg_init		(PLStream *);
void dg_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void dg_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void dg_clear		(PLStream *);
void dg_page		(PLStream *);
void dg_adv		(PLStream *);
void dg_tidy		(PLStream *);
void dg_color		(PLStream *);
void dg_text		(PLStream *);
void dg_graph		(PLStream *);
void dg_width		(PLStream *);
void dg_esc		(PLStream *, PLINT, char *);

void hp7470_init	(PLStream *);
void hp7470_line	(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void hp7470_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void hp7470_clear	(PLStream *);
void hp7470_page	(PLStream *);
void hp7470_adv		(PLStream *);
void hp7470_tidy	(PLStream *);
void hp7470_color	(PLStream *);
void hp7470_text	(PLStream *);
void hp7470_graph	(PLStream *);
void hp7470_width	(PLStream *);
void hp7470_esc		(PLStream *, PLINT, char *);

void hp7580_init	(PLStream *);
void hp7580_line	(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void hp7580_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void hp7580_clear	(PLStream *);
void hp7580_page	(PLStream *);
void hp7580_adv		(PLStream *);
void hp7580_tidy	(PLStream *);
void hp7580_color	(PLStream *);
void hp7580_text	(PLStream *);
void hp7580_graph	(PLStream *);
void hp7580_width	(PLStream *);
void hp7580_esc		(PLStream *, PLINT, char *);

void imp_init		(PLStream *);
void imp_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void imp_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void imp_clear		(PLStream *);
void imp_page		(PLStream *);
void imp_adv		(PLStream *);
void imp_tidy		(PLStream *);
void imp_color		(PLStream *);
void imp_text		(PLStream *);
void imp_graph		(PLStream *);
void imp_width		(PLStream *);
void imp_esc		(PLStream *, PLINT, char *);

void xfig_init		(PLStream *);
void xfig_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void xfig_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void xfig_clear		(PLStream *);
void xfig_page		(PLStream *);
void xfig_adv		(PLStream *);
void xfig_tidy		(PLStream *);
void xfig_color		(PLStream *);
void xfig_text		(PLStream *);
void xfig_graph		(PLStream *);
void xfig_width		(PLStream *);
void xfig_esc		(PLStream *, PLINT, char *);

void jet_init		(PLStream *);
void jet_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void jet_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void jet_clear		(PLStream *);
void jet_page		(PLStream *);
void jet_adv		(PLStream *);
void jet_tidy		(PLStream *);
void jet_color		(PLStream *);
void jet_text		(PLStream *);
void jet_graph		(PLStream *);
void jet_width		(PLStream *);
void jet_esc		(PLStream *, PLINT, char *);

void ps_init		(PLStream *);
void ps_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void ps_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void ps_clear		(PLStream *);
void ps_page		(PLStream *);
void ps_adv		(PLStream *);
void ps_tidy		(PLStream *);
void ps_color		(PLStream *);
void ps_text		(PLStream *);
void ps_graph		(PLStream *);
void ps_width		(PLStream *);
void ps_esc		(PLStream *, PLINT, char *);

void nx_init             (PLStream *);
void nx_line             (PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void nx_polyline         (PLStream *, PLSHORT *, PLSHORT *, PLINT);
void nx_clear            (PLStream *);
void nx_page             (PLStream *);
void nx_adv              (PLStream *);
void nx_tidy             (PLStream *);
void nx_color            (PLStream *);
void nx_text             (PLStream *);
void nx_graph            (PLStream *);
void nx_width            (PLStream *);
void nx_esc              (PLStream *, PLINT, char *);

void plm_init		(PLStream *);
void plm_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void plm_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void plm_clear		(PLStream *);
void plm_page		(PLStream *);
void plm_adv		(PLStream *);
void plm_tidy		(PLStream *);
void plm_color		(PLStream *);
void plm_text		(PLStream *);
void plm_graph		(PLStream *);
void plm_width		(PLStream *);
void plm_esc		(PLStream *, PLINT, char *);

void vga_init		(PLStream *);
void vga_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void vga_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void vga_clear		(PLStream *);
void vga_page		(PLStream *);
void vga_adv		(PLStream *);
void vga_tidy		(PLStream *);
void vga_color		(PLStream *);
void vga_text		(PLStream *);
void vga_graph		(PLStream *);
void vga_width		(PLStream *);
void vga_esc		(PLStream *, PLINT, char *);

void svga_init		(PLStream *);
void svga_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void svga_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void svga_clear		(PLStream *);
void svga_page		(PLStream *);
void svga_adv		(PLStream *);
void svga_tidy		(PLStream *);
void svga_color		(PLStream *);
void svga_text		(PLStream *);
void svga_graph		(PLStream *);
void svga_width		(PLStream *);
void svga_esc		(PLStream *, PLINT, char *);

void os2_init		(PLStream *);				 
void os2_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void os2_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void os2_clear		(PLStream *);				 
void os2_page		(PLStream *);				 
void os2_adv		(PLStream *);				 
void os2_tidy		(PLStream *);				 
void os2_color		(PLStream *);				 
void os2_text		(PLStream *);				 
void os2_graph		(PLStream *);				 
void os2_width		(PLStream *);				 
void os2_esc		(PLStream *, PLINT, char *);		 

void amiwn_init		(PLStream *);
void amiwn_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void amiwn_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void amiwn_clear	(PLStream *);
void amiwn_page		(PLStream *);
void amiwn_adv		(PLStream *);
void amiwn_tidy		(PLStream *);
void amiwn_color	(PLStream *);
void amiwn_text		(PLStream *);
void amiwn_graph	(PLStream *);
void amiwn_width	(PLStream *);
void amiwn_esc		(PLStream *, PLINT, char *);

void amipr_init		(PLStream *);
void amipr_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void amipr_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void amipr_clear	(PLStream *);
void amipr_page		(PLStream *);
void amipr_adv		(PLStream *);
void amipr_tidy		(PLStream *);
void amipr_color	(PLStream *);
void amipr_text		(PLStream *);
void amipr_graph	(PLStream *);
void amipr_width	(PLStream *);
void amipr_esc		(PLStream *, PLINT, char *);

void iff_init		(PLStream *);
void iff_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void iff_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void iff_clear		(PLStream *);
void iff_page		(PLStream *);
void iff_adv		(PLStream *);
void iff_tidy		(PLStream *);
void iff_color		(PLStream *);
void iff_text		(PLStream *);
void iff_graph		(PLStream *);
void iff_width		(PLStream *);
void iff_esc		(PLStream *, PLINT, char *);

void aegis_init		(PLStream *);
void aegis_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void aegis_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void aegis_clear	(PLStream *);
void aegis_page		(PLStream *);
void aegis_adv		(PLStream *);
void aegis_tidy		(PLStream *);
void aegis_color	(PLStream *);
void aegis_text		(PLStream *);
void aegis_graph	(PLStream *);
void aegis_width	(PLStream *);
void aegis_esc		(PLStream *, PLINT, char *);

void null_init		(PLStream *);
void null_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void null_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void null_clear		(PLStream *);
void null_page		(PLStream *);
void null_adv		(PLStream *);
void null_tidy		(PLStream *);
void null_color		(PLStream *);
void null_text		(PLStream *);
void null_graph		(PLStream *);
void null_width		(PLStream *);
void null_esc		(PLStream *, PLINT, char *);

/*----------------------------------------------------------------------*\
* Prototypes for plot buffer calls.
\*----------------------------------------------------------------------*/

void plbuf_init		(PLStream *);
void plbuf_line		(PLStream *, PLSHORT, PLSHORT, PLSHORT, PLSHORT);
void plbuf_polyline	(PLStream *, PLSHORT *, PLSHORT *, PLINT);
void plbuf_clear	(PLStream *);
void plbuf_page		(PLStream *);
void plbuf_adv		(PLStream *);
void plbuf_tidy		(PLStream *);
void plbuf_color	(PLStream *);
void plbuf_text		(PLStream *);
void plbuf_graph	(PLStream *);
void plbuf_width	(PLStream *);
void plbuf_esc		(PLStream *, PLINT, char *);

void plRemakePlot	(PLStream *);

#ifdef __cplusplus
}
#endif

#endif	/* __DRIVERS_H__ */
