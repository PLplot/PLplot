# PLWin.tcl
# Geoffrey Furnish
# 9 May 1994
#
# @> [incr Tcl] interface to PLplot
#
# $Id$
#
# $Log$
# Revision 1.10  1995/07/19 18:02:16  furnish
# Completely overhauled.  Now mirrors all Tcl API commands which make
# any sense on a widget, 83 in all, plus a "cmd" proxy so that code
# written for a bare Tk plframe can work unmodified on a PLWin.
#
# Revision 1.9  1995/07/04  21:53:43  furnish
# plcont and plshade now have long, complex argument lists.  Moreover,
# since they support defaulting of arguments, not all args even have to
# be specified.  So we convert to using the [incr Tcl] args metaphor so
# that we don't have to worry about exactly how these shadow methods
# were called.  Note that this object needs major fleshing out now that
# the entire Tcl API is implemented.
#
# Revision 1.8  1995/06/09  22:48:07  mjl
# Lots of fleshing out.  Now supports every Tcl API call available so far.
#
# Revision 1.7  1994/10/10  19:44:45  furnish
# Stub method for plshade.
#
# Revision 1.6  1994/10/10  17:22:32  furnish
# New method, but need many more.
#
# Revision 1.5  1994/06/16  18:33:51  mjl
# Modified argument lists for plline and plpoin methods to use matrix names.
#
# Revision 1.4  1994/06/10  20:46:58  furnish
# Mirror plpoin.  More of the API still needs doing.
#
# Revision 1.3  1994/06/09  20:07:13  mjl
# Cleaned up and switched to plplot direct commands embedded in plframe
# widget (through "<widget> cmd <command> <args>" syntax).
#
# Revision 1.2  1994/05/11  08:07:30  furnish
# Debugging.  Needs to do own toprow for some reason.
#
# Revision 1.1  1994/05/09  17:59:19  furnish
# The new [incr Tcl] interface to the PLplot Tcl extensions.
#
###############################################################################

itcl_class PLWin {

    public name

    constructor {config} {
	set class [$this info class]
	::rename $this $this-tmp-
	::frame $this -class $class
	::rename $this $this-win-
	::rename $this-tmp- $this

	plframe $this.plwin -relief sunken
	pack append $this $this.plwin {bottom expand fill}
    }

    method configure {config} {}

# We add this one just so that user code can be oblivious as to
# whether or not it is driving a PLWin object, or just a plframe.  In
# other words, Tcl code which works fine on a plframe or the "loopback
# widget", can work unmodified with the PLWin [incr Tcl] object.

    method cmd {args} {
	eval $this.plwin cmd $args
    }

# From here on down, we just insert what comes out of plitclgen (file
# gen.itcl).  If you don't like the handling of one of these for some
# reason, then add the method name to the ignore list in plitclgen,
# and provide an explicit coding, ABOVE THIS LINE.

    method pladv {args} {
        eval $this.plwin cmd pladv $args
    }

    method plaxes {args} {
        eval $this.plwin cmd plaxes $args
    }

    method plbin {args} {
        eval $this.plwin cmd plbin $args
    }

    method plbop {args} {
        eval $this.plwin cmd plbop $args
    }

    method plbox {args} {
        eval $this.plwin cmd plbox $args
    }

    method plbox3 {args} {
        eval $this.plwin cmd plbox3 $args
    }

    method plcol0 {args} {
        eval $this.plwin cmd plcol0 $args
    }

    method plcol1 {args} {
        eval $this.plwin cmd plcol1 $args
    }

    method pldid2pc {args} {
        eval $this.plwin cmd pldid2pc $args
    }

    method pldip2dc {args} {
        eval $this.plwin cmd pldip2dc $args
    }

    method plenv {args} {
        eval $this.plwin cmd plenv $args
    }

    method pleop {args} {
        eval $this.plwin cmd pleop $args
    }

    method plerrx {args} {
        eval $this.plwin cmd plerrx $args
    }

    method plerry {args} {
        eval $this.plwin cmd plerry $args
    }

    method plfill {args} {
        eval $this.plwin cmd plfill $args
    }

    method plfont {args} {
        eval $this.plwin cmd plfont $args
    }

    method plfontld {args} {
        eval $this.plwin cmd plfontld $args
    }

    method plgchr {args} {
        eval $this.plwin cmd plgchr $args
    }

    method plgcol0 {args} {
        eval $this.plwin cmd plgcol0 $args
    }

    method plgdidev {args} {
        eval $this.plwin cmd plgdidev $args
    }

    method plgdiori {args} {
        eval $this.plwin cmd plgdiori $args
    }

    method plgdiplt {args} {
        eval $this.plwin cmd plgdiplt $args
    }

    method plgpage {args} {
        eval $this.plwin cmd plgpage $args
    }

    method plgra {args} {
        eval $this.plwin cmd plgra $args
    }

    method plgspa {args} {
        eval $this.plwin cmd plgspa $args
    }

    method plgstrm {args} {
        eval $this.plwin cmd plgstrm $args
    }

    method plgxax {args} {
        eval $this.plwin cmd plgxax $args
    }

    method plgyax {args} {
        eval $this.plwin cmd plgyax $args
    }

    method plgzax {args} {
        eval $this.plwin cmd plgzax $args
    }

    method plhist {args} {
        eval $this.plwin cmd plhist $args
    }

    method plhls {args} {
        eval $this.plwin cmd plhls $args
    }

    method pljoin {args} {
        eval $this.plwin cmd pljoin $args
    }

    method pllab {args} {
        eval $this.plwin cmd pllab $args
    }

    method plline {args} {
        eval $this.plwin cmd plline $args
    }

    method plline3 {args} {
        eval $this.plwin cmd plline3 $args
    }

    method pllsty {args} {
        eval $this.plwin cmd pllsty $args
    }

    method plmtex {args} {
        eval $this.plwin cmd plmtex $args
    }

    method plpat {args} {
        eval $this.plwin cmd plpat $args
    }

    method plpoin {args} {
        eval $this.plwin cmd plpoin $args
    }

    method plpoin3 {args} {
        eval $this.plwin cmd plpoin3 $args
    }

    method plpoly3 {args} {
        eval $this.plwin cmd plpoly3 $args
    }

    method plprec {args} {
        eval $this.plwin cmd plprec $args
    }

    method plpsty {args} {
        eval $this.plwin cmd plpsty $args
    }

    method plptex {args} {
        eval $this.plwin cmd plptex $args
    }

    method plreplot {args} {
        eval $this.plwin cmd plreplot $args
    }

    method plrgb {args} {
        eval $this.plwin cmd plrgb $args
    }

    method plrgb1 {args} {
        eval $this.plwin cmd plrgb1 $args
    }

    method plschr {args} {
        eval $this.plwin cmd plschr $args
    }

    method plscmap0n {args} {
        eval $this.plwin cmd plscmap0n $args
    }

    method plscmap1n {args} {
        eval $this.plwin cmd plscmap1n $args
    }

    method plscmap0 {args} {
        eval $this.plwin cmd plscmap0 $args
    }

    method plscmap1 {args} {
        eval $this.plwin cmd plscmap1 $args
    }

    method plscmap1l {args} {
        eval $this.plwin cmd plscmap1l $args
    }

    method plscol0 {args} {
        eval $this.plwin cmd plscol0 $args
    }

    method plscolbg {args} {
        eval $this.plwin cmd plscolbg $args
    }

    method plscolor {args} {
        eval $this.plwin cmd plscolor $args
    }

    method plsdidev {args} {
        eval $this.plwin cmd plsdidev $args
    }

    method plsdimap {args} {
        eval $this.plwin cmd plsdimap $args
    }

    method plsdiori {args} {
        eval $this.plwin cmd plsdiori $args
    }

    method plsdiplt {args} {
        eval $this.plwin cmd plsdiplt $args
    }

    method plsdiplz {args} {
        eval $this.plwin cmd plsdiplz $args
    }

    method plsmaj {args} {
        eval $this.plwin cmd plsmaj $args
    }

    method plsmin {args} {
        eval $this.plwin cmd plsmin $args
    }

    method plsori {args} {
        eval $this.plwin cmd plsori $args
    }

    method plspage {args} {
        eval $this.plwin cmd plspage $args
    }

    method plspause {args} {
        eval $this.plwin cmd plspause $args
    }

    method plssub {args} {
        eval $this.plwin cmd plssub $args
    }

    method plssym {args} {
        eval $this.plwin cmd plssym $args
    }

    method plstyl {args} {
        eval $this.plwin cmd plstyl $args
    }

    method plsvpa {args} {
        eval $this.plwin cmd plsvpa $args
    }

    method plsxax {args} {
        eval $this.plwin cmd plsxax $args
    }

    method plsxwin {args} {
        eval $this.plwin cmd plsxwin $args
    }

    method plsyax {args} {
        eval $this.plwin cmd plsyax $args
    }

    method plsym {args} {
        eval $this.plwin cmd plsym $args
    }

    method plszax {args} {
        eval $this.plwin cmd plszax $args
    }

    method pltext {args} {
        eval $this.plwin cmd pltext $args
    }

    method plvasp {args} {
        eval $this.plwin cmd plvasp $args
    }

    method plvpas {args} {
        eval $this.plwin cmd plvpas $args
    }

    method plvpor {args} {
        eval $this.plwin cmd plvpor $args
    }

    method plvsta {args} {
        eval $this.plwin cmd plvsta $args
    }

    method plw3d {args} {
        eval $this.plwin cmd plw3d $args
    }

    method plwid {args} {
        eval $this.plwin cmd plwid $args
    }

    method plwind {args} {
        eval $this.plwin cmd plwind $args
    }

}
