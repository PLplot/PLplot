/* $Id$
   $Log$
   Revision 1.2.2.1  2001/04/19 12:31:46  rlaboiss
   First merge against MAIN

   Revision 1.2  1992/09/29 04:46:08  furnish
   Massive clean up effort to remove support for garbage compilers (K&R).

 * Revision 1.1  1992/05/20  21:34:39  furnish
 * Initial checkin of the whole PLPLOT project.
 *
*/

/*	plmtex.c

	Writes a text string at a specified position relative to
	the viewport.
*/

#include "plplot.h"

/*----------------------------------------------------------------------*\
* void plmtex()
*
* Prints out "text" at specified position relative to viewport
* (may be inside or outside)
*
* side	String which is one of the following:
*	B or b  :  Bottom of viewport
*	T or t  :  Top of viewport
*	L or l  :  Left of viewport
*	R or r  :  Right of viewport
*	LV or lv : Left of viewport, vertical text
*	RV or rv : Right of viewport, vertical text
* disp	Displacement from specified edge of viewport, measured
*	outwards from the viewport in units of the current
*	character height. The CENTRELINES of the characters are
*	aligned with the specified position.
* pos	Position of the reference point of the string relative
*	to the viewport edge, ranging from 0.0 (left-hand edge)
*	to 1.0 (right-hand edge)
* just	Justification of string relative to reference point
*	just = 0.0 => left hand edge of string is at reference
*	just = 1.0 => right hand edge of string is at reference
*	just = 0.5 => centre of string is at reference
\*----------------------------------------------------------------------*/

void
c_plmtex (char *side, PLFLT disp, PLFLT pos, PLFLT just, char *text)
{
    PLINT clpxmi, clpxma, clpymi, clpyma;
    PLINT sppxmi, sppxma, sppymi, sppyma;
    PLINT vert, refx, refy;
    PLFLT shift, xform[4];
    PLFLT vpdxmi, vpdxma, vpdymi, vpdyma;
    PLFLT chrdef, chrht;
    PLFLT mpxscl, mpxoff, mpyscl, mpyoff;
    PLINT level;

    glev(&level);
    if (level < 2)
	plexit("Please set up viewport before calling plmtex.");

    /* Open clip limits to subpage limits */

    gclp(&clpxmi, &clpxma, &clpymi, &clpyma);
    gspp(&sppxmi, &sppxma, &sppymi, &sppyma);
    sclp(sppxmi, sppxma, sppymi, sppyma);

    gvpd(&vpdxmi, &vpdxma, &vpdymi, &vpdyma);
    gmp(&mpxscl, &mpxoff, &mpyscl, &mpyoff);
    gchr(&chrdef, &chrht);

    shift = 0.0;
    if (just != 0.0)
	shift = just * plstrl(text);

    if (stsearch(side, 'b')) {
	vert = 0;
	refx = dcpcx((PLFLT) (vpdxmi + (vpdxma - vpdxmi) * pos)) - shift * mpxscl;
	refy = mmpcy((PLFLT) (dcmmy(vpdymi) - disp * chrht));
    }
    else if (stsearch(side, 't')) {
	vert = 0;
	refx = dcpcx((PLFLT) (vpdxmi + (vpdxma - vpdxmi) * pos)) - shift * mpxscl;
	refy = mmpcy((PLFLT) (dcmmy(vpdyma) + disp * chrht));
    }
    else if (stindex(side, "LV") != -1 || stindex(side, "lv") != -1) {
	vert = 0;
	refy = dcpcy((PLFLT) (vpdymi + (vpdyma - vpdymi) * pos));
	refx = mmpcx((PLFLT) (dcmmx(vpdxmi) - disp * chrht - shift));
    }
    else if (stindex(side, "RV") != -1 || stindex(side, "rv") != -1) {
	vert = 0;
	refy = dcpcy((PLFLT) (vpdymi + (vpdyma - vpdymi) * pos));
	refx = mmpcx((PLFLT) (dcmmx(vpdxma) + disp * chrht - shift));
    }
    else if (stsearch(side, 'l')) {
	vert = 1;
	refy = dcpcy((PLFLT) (vpdymi + (vpdyma - vpdymi) * pos)) - shift * mpyscl;
	refx = mmpcx((PLFLT) (dcmmx(vpdxmi) - disp * chrht));
    }
    else if (stsearch(side, 'r')) {
	vert = 1;
	refy = dcpcy((PLFLT) (vpdymi + (vpdyma - vpdymi) * pos)) - shift * mpyscl;
	refx = mmpcx((PLFLT) (dcmmx(vpdxma) + disp * chrht));
    }
    else {
	sclp(clpxmi, clpxma, clpymi, clpyma);
	return;
    }

    if (vert != 0) {
	xform[0] = 0.0;
	xform[1] = -1.0;
	xform[2] = 1.0;
	xform[3] = 0.0;
    }
    else {
	xform[0] = 1.0;
	xform[1] = 0.0;
	xform[2] = 0.0;
	xform[3] = 1.0;
    }
    plstr(0, xform, refx, refy, text);
    sclp(clpxmi, clpxma, clpymi, clpyma);
}
