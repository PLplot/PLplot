/* $Id$
   $Log$
   Revision 1.4  1993/03/15 21:45:05  mjl
   Changed _clear/_page driver functions to the names _eop/_bop, to be
   more representative of what's actually going on.

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

void xte_init		(PLStream *);
void xte_line		(PLStream *, short, short, short, short);
void xte_polyline	(PLStream *, short *, short *, PLINT);
void xte_eop		(PLStream *);
void xte_bop		(PLStream *);
void xte_tidy		(PLStream *);
void xte_color		(PLStream *);
void xte_text		(PLStream *);
void xte_graph		(PLStream *);
void xte_width		(PLStream *);
void xte_esc		(PLStream *, PLINT, char *);

void xm_init		(PLStream *);
void xm_line		(PLStream *, short, short, short, short);
void xm_polyline	(PLStream *, short *, short *, PLINT);
void xm_eop		(PLStream *);
void xm_bop		(PLStream *);
void xm_tidy		(PLStream *);
void xm_color		(PLStream *);
void xm_text		(PLStream *);
void xm_graph		(PLStream *);
void xm_width		(PLStream *);
void xm_esc		(PLStream *, PLINT, char *);

void xw_init		(PLStream *);
void xw_line		(PLStream *, short, short, short, short);
void xw_polyline	(PLStream *, short *, short *, PLINT);
void xw_eop		(PLStream *);
void xw_bop		(PLStream *);
void xw_tidy		(PLStream *);
void xw_color		(PLStream *);
void xw_text		(PLStream *);
void xw_graph		(PLStream *);
void xw_width		(PLStream *);
void xw_esc		(PLStream *, PLINT, char *);

void tekt_init		(PLStream *);
void tekf_init		(PLStream *);
void tek_line		(PLStream *, short, short, short, short);
void tek_polyline	(PLStream *, short *, short *, PLINT);
void tek_eop		(PLStream *);
void tek_bop		(PLStream *);
void tek_tidy		(PLStream *);
void tek_color		(PLStream *);
void tek_text		(PLStream *);
void tek_graph		(PLStream *);
void tek_width		(PLStream *);
void tek_esc		(PLStream *, PLINT, char *);

void dg_init		(PLStream *);
void dg_line		(PLStream *, short, short, short, short);
void dg_polyline	(PLStream *, short *, short *, PLINT);
void dg_eop		(PLStream *);
void dg_bop		(PLStream *);
void dg_tidy		(PLStream *);
void dg_color		(PLStream *);
void dg_text		(PLStream *);
void dg_graph		(PLStream *);
void dg_width		(PLStream *);
void dg_esc		(PLStream *, PLINT, char *);

void hp7470_init	(PLStream *);
void hp7470_line	(PLStream *, short, short, short, short);
void hp7470_polyline	(PLStream *, short *, short *, PLINT);
void hp7470_eop		(PLStream *);
void hp7470_bop		(PLStream *);
void hp7470_tidy	(PLStream *);
void hp7470_color	(PLStream *);
void hp7470_text	(PLStream *);
void hp7470_graph	(PLStream *);
void hp7470_width	(PLStream *);
void hp7470_esc		(PLStream *, PLINT, char *);

void hp7580_init	(PLStream *);
void hp7580_line	(PLStream *, short, short, short, short);
void hp7580_polyline	(PLStream *, short *, short *, PLINT);
void hp7580_eop		(PLStream *);
void hp7580_bop		(PLStream *);
void hp7580_tidy	(PLStream *);
void hp7580_color	(PLStream *);
void hp7580_text	(PLStream *);
void hp7580_graph	(PLStream *);
void hp7580_width	(PLStream *);
void hp7580_esc		(PLStream *, PLINT, char *);

void imp_init		(PLStream *);
void imp_line		(PLStream *, short, short, short, short);
void imp_polyline	(PLStream *, short *, short *, PLINT);
void imp_eop		(PLStream *);
void imp_bop		(PLStream *);
void imp_tidy		(PLStream *);
void imp_color		(PLStream *);
void imp_text		(PLStream *);
void imp_graph		(PLStream *);
void imp_width		(PLStream *);
void imp_esc		(PLStream *, PLINT, char *);

void xfig_init		(PLStream *);
void xfig_line		(PLStream *, short, short, short, short);
void xfig_polyline	(PLStream *, short *, short *, PLINT);
void xfig_eop		(PLStream *);
void xfig_bop		(PLStream *);
void xfig_tidy		(PLStream *);
void xfig_color		(PLStream *);
void xfig_text		(PLStream *);
void xfig_graph		(PLStream *);
void xfig_width		(PLStream *);
void xfig_esc		(PLStream *, PLINT, char *);

void jet_init		(PLStream *);
void jet_line		(PLStream *, short, short, short, short);
void jet_polyline	(PLStream *, short *, short *, PLINT);
void jet_eop		(PLStream *);
void jet_bop		(PLStream *);
void jet_tidy		(PLStream *);
void jet_color		(PLStream *);
void jet_text		(PLStream *);
void jet_graph		(PLStream *);
void jet_width		(PLStream *);
void jet_esc		(PLStream *, PLINT, char *);

void ps_init		(PLStream *);
void ps_line		(PLStream *, short, short, short, short);
void ps_polyline	(PLStream *, short *, short *, PLINT);
void ps_eop		(PLStream *);
void ps_bop		(PLStream *);
void ps_tidy		(PLStream *);
void ps_color		(PLStream *);
void ps_text		(PLStream *);
void ps_graph		(PLStream *);
void ps_width		(PLStream *);
void ps_esc		(PLStream *, PLINT, char *);

void nx_init		(PLStream *);
void nx_line		(PLStream *, short, short, short, short);
void nx_polyline	(PLStream *, short *, short *, PLINT);
void nx_eop		(PLStream *);
void nx_bop		(PLStream *);
void nx_tidy		(PLStream *);
void nx_color		(PLStream *);
void nx_text		(PLStream *);
void nx_graph		(PLStream *);
void nx_width		(PLStream *);
void nx_esc		(PLStream *, PLINT, char *);

void plm_init		(PLStream *);
void plm_line		(PLStream *, short, short, short, short);
void plm_polyline	(PLStream *, short *, short *, PLINT);
void plm_eop		(PLStream *);
void plm_bop		(PLStream *);
void plm_tidy		(PLStream *);
void plm_color		(PLStream *);
void plm_text		(PLStream *);
void plm_graph		(PLStream *);
void plm_width		(PLStream *);
void plm_esc		(PLStream *, PLINT, char *);

void vga_init		(PLStream *);
void vga_line		(PLStream *, short, short, short, short);
void vga_polyline	(PLStream *, short *, short *, PLINT);
void vga_eop		(PLStream *);
void vga_bop		(PLStream *);
void vga_tidy		(PLStream *);
void vga_color		(PLStream *);
void vga_text		(PLStream *);
void vga_graph		(PLStream *);
void vga_width		(PLStream *);
void vga_esc		(PLStream *, PLINT, char *);

void svga_init		(PLStream *);
void svga_line		(PLStream *, short, short, short, short);
void svga_polyline	(PLStream *, short *, short *, PLINT);
void svga_eop		(PLStream *);
void svga_bop		(PLStream *);
void svga_tidy		(PLStream *);
void svga_color		(PLStream *);
void svga_text		(PLStream *);
void svga_graph		(PLStream *);
void svga_width		(PLStream *);
void svga_esc		(PLStream *, PLINT, char *);

void os2_init		(PLStream *);				 
void os2_line		(PLStream *, short, short, short, short);
void os2_polyline	(PLStream *, short *, short *, PLINT);
void os2_eop		(PLStream *);				 
void os2_bop		(PLStream *);				 
void os2_tidy		(PLStream *);				 
void os2_color		(PLStream *);				 
void os2_text		(PLStream *);				 
void os2_graph		(PLStream *);				 
void os2_width		(PLStream *);				 
void os2_esc		(PLStream *, PLINT, char *);		 

void amiwn_init		(PLStream *);
void amiwn_line		(PLStream *, short, short, short, short);
void amiwn_polyline	(PLStream *, short *, short *, PLINT);
void amiwn_eop		(PLStream *);
void amiwn_bop		(PLStream *);
void amiwn_tidy		(PLStream *);
void amiwn_color	(PLStream *);
void amiwn_text		(PLStream *);
void amiwn_graph	(PLStream *);
void amiwn_width	(PLStream *);
void amiwn_esc		(PLStream *, PLINT, char *);

void amipr_init		(PLStream *);
void amipr_line		(PLStream *, short, short, short, short);
void amipr_polyline	(PLStream *, short *, short *, PLINT);
void amipr_eop		(PLStream *);
void amipr_bop		(PLStream *);
void amipr_tidy		(PLStream *);
void amipr_color	(PLStream *);
void amipr_text		(PLStream *);
void amipr_graph	(PLStream *);
void amipr_width	(PLStream *);
void amipr_esc		(PLStream *, PLINT, char *);

void iff_init		(PLStream *);
void iff_line		(PLStream *, short, short, short, short);
void iff_polyline	(PLStream *, short *, short *, PLINT);
void iff_eop		(PLStream *);
void iff_bop		(PLStream *);
void iff_tidy		(PLStream *);
void iff_color		(PLStream *);
void iff_text		(PLStream *);
void iff_graph		(PLStream *);
void iff_width		(PLStream *);
void iff_esc		(PLStream *, PLINT, char *);

void aegis_init		(PLStream *);
void aegis_line		(PLStream *, short, short, short, short);
void aegis_polyline	(PLStream *, short *, short *, PLINT);
void aegis_eop		(PLStream *);
void aegis_bop		(PLStream *);
void aegis_tidy		(PLStream *);
void aegis_color	(PLStream *);
void aegis_text		(PLStream *);
void aegis_graph	(PLStream *);
void aegis_width	(PLStream *);
void aegis_esc		(PLStream *, PLINT, char *);

void null_init		(PLStream *);
void null_line		(PLStream *, short, short, short, short);
void null_polyline	(PLStream *, short *, short *, PLINT);
void null_eop		(PLStream *);
void null_bop		(PLStream *);
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
void plbuf_line		(PLStream *, short, short, short, short);
void plbuf_polyline	(PLStream *, short *, short *, PLINT);
void plbuf_eop		(PLStream *);
void plbuf_bop		(PLStream *);
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
