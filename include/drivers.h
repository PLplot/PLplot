/* $Id$

	Contains all prototypes for driver functions.
*/

#ifndef __DRIVERS_H__
#define __DRIVERS_H__

#include "plplot/pdf.h"
#include "plplot/plstrm.h"

#ifdef __cplusplus
extern "C" {
#endif

void plD_dispatch_init_mac8	( PLDispatchTable *pdt );
void plD_dispatch_init_mac1	( PLDispatchTable *pdt );
void plD_dispatch_init_nx	( PLDispatchTable *pdt );
void plD_dispatch_init_os2	( PLDispatchTable *pdt );
void plD_dispatch_init_xw	( PLDispatchTable *pdt );
void plD_dispatch_init_gnome	( PLDispatchTable *pdt );
void plD_dispatch_init_tk	( PLDispatchTable *pdt );
void plD_dispatch_init_vga	( PLDispatchTable *pdt );
void plD_dispatch_init_mgr	( PLDispatchTable *pdt );
void plD_dispatch_init_win3	( PLDispatchTable *pdt );
void plD_dispatch_init_vga	( PLDispatchTable *pdt );
void plD_dispatch_init_vga	( PLDispatchTable *pdt );
void plD_dispatch_init_vga	( PLDispatchTable *pdt );
void plD_dispatch_init_tiff	( PLDispatchTable *pdt );
void plD_dispatch_init_jpg	( PLDispatchTable *pdt );
void plD_dispatch_init_bmp	( PLDispatchTable *pdt );
void plD_dispatch_init_vga	( PLDispatchTable *pdt );
void plD_dispatch_init_xterm	( PLDispatchTable *pdt );
void plD_dispatch_init_tekt	( PLDispatchTable *pdt );
void plD_dispatch_init_tek4107t	( PLDispatchTable *pdt );
void plD_dispatch_init_mskermit	( PLDispatchTable *pdt );
void plD_dispatch_init_versaterm( PLDispatchTable *pdt );
void plD_dispatch_init_vlt	( PLDispatchTable *pdt );
void plD_dispatch_init_conex	( PLDispatchTable *pdt );
void plD_dispatch_init_dg	( PLDispatchTable *pdt );
void plD_dispatch_init_plm	( PLDispatchTable *pdt );
void plD_dispatch_init_tekf	( PLDispatchTable *pdt );
void plD_dispatch_init_tek4107f	( PLDispatchTable *pdt );
void plD_dispatch_init_psm	( PLDispatchTable *pdt );
void plD_dispatch_init_psc	( PLDispatchTable *pdt );
void plD_dispatch_init_xfig	( PLDispatchTable *pdt );
void plD_dispatch_init_ljiip	( PLDispatchTable *pdt );
void plD_dispatch_init_ljii	( PLDispatchTable *pdt );
void plD_dispatch_init_hp7470	( PLDispatchTable *pdt );
void plD_dispatch_init_hp7580	( PLDispatchTable *pdt );
void plD_dispatch_init_hpgl	( PLDispatchTable *pdt );
void plD_dispatch_init_imp	( PLDispatchTable *pdt );
void plD_dispatch_init_pbm	( PLDispatchTable *pdt );
void plD_dispatch_init_png	( PLDispatchTable *pdt );
void plD_dispatch_init_png	( PLDispatchTable *pdt );
void plD_dispatch_init_null	( PLDispatchTable *pdt );

#if 0
void plD_init_tk		(PLStream *);
void plD_init_dp		(PLStream *);
void plD_line_tk		(PLStream *, short, short, short, short);
void plD_polyline_tk		(PLStream *, short *, short *, PLINT);
void plD_eop_tk			(PLStream *);
void plD_bop_tk			(PLStream *);
void plD_tidy_tk		(PLStream *);
void plD_state_tk		(PLStream *, PLINT);
void plD_esc_tk			(PLStream *, PLINT, void *);

void plD_init_pstex		(PLStream *);
void plD_esc_pstex		(PLStream *, PLINT, void *);
void plD_tidy_pstex		(PLStream *);
void plD_bop_pstex		(PLStream *);

void plD_init_xw		(PLStream *);
void plD_line_xw		(PLStream *, short, short, short, short);
void plD_polyline_xw		(PLStream *, short *, short *, PLINT);
void plD_eop_xw			(PLStream *);
void plD_bop_xw			(PLStream *);
void plD_tidy_xw		(PLStream *);
void plD_state_xw		(PLStream *, PLINT);
void plD_esc_xw			(PLStream *, PLINT, void *);

void plD_init_gnome		(PLStream *);
void plD_line_gnome		(PLStream *, short, short, short, short);
void plD_polyline_gnome		(PLStream *, short *, short *, PLINT);
void plD_eop_gnome		(PLStream *);
void plD_bop_gnome		(PLStream *);
void plD_tidy_gnome		(PLStream *);
void plD_state_gnome		(PLStream *, PLINT);
void plD_esc_gnome		(PLStream *, PLINT, void *);

void plD_init_xterm		(PLStream *);
void plD_init_tekt		(PLStream *);
void plD_init_tekf		(PLStream *);
void plD_init_tek4107t		(PLStream *);
void plD_init_tek4107f		(PLStream *);
void plD_init_mskermit		(PLStream *);
void plD_init_versaterm		(PLStream *);
void plD_init_vlt		(PLStream *);
void plD_init_conex		(PLStream *);

void plD_line_tek		(PLStream *, short, short, short, short);
void plD_polyline_tek		(PLStream *, short *, short *, PLINT);
void plD_eop_tek		(PLStream *);
void plD_bop_tek		(PLStream *);
void plD_tidy_tek		(PLStream *);
void plD_state_tek		(PLStream *, PLINT);
void plD_esc_tek		(PLStream *, PLINT, void *);

void plD_init_dg		(PLStream *);
void plD_line_dg		(PLStream *, short, short, short, short);
void plD_polyline_dg		(PLStream *, short *, short *, PLINT);
void plD_eop_dg			(PLStream *);
void plD_bop_dg			(PLStream *);
void plD_tidy_dg		(PLStream *);
void plD_state_dg		(PLStream *, PLINT);
void plD_esc_dg			(PLStream *, PLINT, void *);

void plD_init_hp7470		(PLStream *);
void plD_init_hp7580		(PLStream *);
void plD_init_lj_hpgl		(PLStream *);

void plD_line_hpgl		(PLStream *, short, short, short, short);
void plD_polyline_hpgl		(PLStream *, short *, short *, PLINT);
void plD_eop_hpgl		(PLStream *);
void plD_bop_hpgl		(PLStream *);
void plD_tidy_hpgl		(PLStream *);
void plD_state_hpgl		(PLStream *, PLINT);
void plD_esc_hpgl		(PLStream *, PLINT, void *);

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

void plD_init_ljiip		(PLStream *);
void plD_line_ljiip		(PLStream *, short, short, short, short);
void plD_polyline_ljiip		(PLStream *, short *, short *, PLINT);
void plD_eop_ljiip		(PLStream *);
void plD_bop_ljiip		(PLStream *);
void plD_tidy_ljiip		(PLStream *);
void plD_state_ljiip		(PLStream *, PLINT);
void plD_esc_ljiip		(PLStream *, PLINT, void *);

void plD_init_ljii		(PLStream *);
void plD_line_ljii		(PLStream *, short, short, short, short);
void plD_polyline_ljii		(PLStream *, short *, short *, PLINT);
void plD_eop_ljii		(PLStream *);
void plD_bop_ljii		(PLStream *);
void plD_tidy_ljii		(PLStream *);
void plD_state_ljii		(PLStream *, PLINT);
void plD_esc_ljii		(PLStream *, PLINT, void *);

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

void plD_init_vga		(PLStream *);
void plD_line_vga		(PLStream *, short, short, short, short);
void plD_polyline_vga		(PLStream *, short *, short *, PLINT);
void plD_eop_vga		(PLStream *);
void plD_bop_vga		(PLStream *);
void plD_tidy_vga		(PLStream *);
void plD_state_vga		(PLStream *, PLINT);
void plD_esc_vga		(PLStream *, PLINT, void *);

void plD_init_tiff		(PLStream *);
void plD_eop_tiff		(PLStream *);
void plD_bop_tiff		(PLStream *);
void plD_tidy_tiff		(PLStream *);
void plD_esc_tiff		(PLStream *, PLINT, void *);

void plD_init_bmp		(PLStream *);
void plD_eop_bmp		(PLStream *);
void plD_bop_bmp		(PLStream *);
void plD_tidy_bmp		(PLStream *);
void plD_esc_bmp		(PLStream *, PLINT, void *);

/*These are for the GRX20-based jpeg driver in the djgpp area*/
void plD_init_jpg		(PLStream *);
void plD_eop_jpg		(PLStream *);
void plD_bop_jpg		(PLStream *);
void plD_tidy_jpg		(PLStream *);
void plD_esc_jpg		(PLStream *, PLINT, void *);


void plD_init_mac1		(PLStream *);
void plD_init_mac8		(PLStream *);
void plD_line_mac		(PLStream *, short, short, short, short);
void plD_polyline_mac		(PLStream *, short *, short *, PLINT);
void plD_eop_mac		(PLStream *);
void plD_bop_mac		(PLStream *);
void plD_tidy_mac		(PLStream *);
void plD_state_mac		(PLStream *, PLINT);
void plD_esc_mac		(PLStream *, PLINT, void *);

void plD_init_win3		(PLStream *);
void plD_line_win3		(PLStream *, short, short, short, short);
void plD_polyline_win3		(PLStream *, short *, short *, PLINT);
void plD_eop_win3		(PLStream *);
void plD_bop_win3		(PLStream *);
void plD_tidy_win3		(PLStream *);
void plD_state_win3		(PLStream *, PLINT);
void plD_esc_win3		(PLStream *, PLINT, void *);

void plD_init_os2		(PLStream *);				 
void plD_line_os2		(PLStream *, short, short, short, short);
void plD_polyline_os2		(PLStream *, short *, short *, PLINT);
void plD_eop_os2		(PLStream *);				 
void plD_bop_os2		(PLStream *);				 
void plD_tidy_os2		(PLStream *);				 
void plD_state_os2		(PLStream *, PLINT);
void plD_esc_os2		(PLStream *, PLINT, void *);		 

/* These are for the general PNG and JPEG drivers based on libgd */
void plD_init_png               (PLStream *);
void plD_line_png               (PLStream *, short, short, short, short);
void plD_polyline_png           (PLStream *, short *, short *, PLINT);
void plD_eop_png                (PLStream *);
void plD_bop_png                (PLStream *);
void plD_tidy_png               (PLStream *);
void plD_state_png              (PLStream *, PLINT);
void plD_esc_png                (PLStream *, PLINT, void *);
void plD_eop_jpeg                (PLStream *);
#endif

/* Prototypes for plot buffer calls. */

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
