/* -*-C-*- */
/* $Id$
 * $Log$
 * Revision 1.2  2000/05/12 18:05:32  furnish
 * Imported latest Mac port work by Rob Managan.
 *
 * Revision 1.1  1995/04/11  09:39:15  mjl
 * Mac port, contributed by Mark Franz.
 *
*/

/*
    plDevs.h

    Mac-specific version.

    Contains macro definitions that determine what device drivers are
    compiled into the PLplot library.  On a Unix system, typically the
    configure script builds plDevs.h from plDevs.h.in.  Elsewhere, it's
    best to hand-configure a plDevs.h file and keep it with the
    system-specific files.
*/

#ifndef __PLDEVS_H__
#define __PLDEVS_H__

#undef PLD_plmeta
#define PLD_null
#undef PLD_xterm
#undef PLD_tek4010
#undef PLD_tek4107
#undef PLD_mskermit
#undef PLD_vlt
#undef PLD_versaterm
#undef PLD_conex
#undef PLD_dg300
#define PLD_ps
#undef PLD_xfig
#undef PLD_ljii
#undef PLD_lj_hpgl
#undef PLD_hp7470
#undef PLD_hp7580
#undef PLD_imp
#undef PLD_xwin
#undef PLD_tk
#undef PLD_dp
#define PLD_mac

#endif	/* __PLDEVS_H__ */
