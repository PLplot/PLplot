# PLWin.tcl
# Geoffrey Furnish
# 9 May 1994
#
# @> [incr Tcl] interface to PLplot
#
# $Id$
#
# $Log$
# Revision 1.5  1994/06/16 18:33:51  mjl
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

    method plcol {color} {
	$this.plwin cmd plcol0 $color
    }

    method plenv {xmin xmax ymin ymax just axis} {
	$this.plwin cmd plenv $xmin $xmax $ymin $ymax $just $axis
    }

    method pllab {xlab ylab toplab} {
	$this.plwin cmd pllab $xlab $ylab $toplab
    }

    method plline {n x y} {
	$this.plwin cmd plline $n $x $y
    }

    method plpoin {n x y code} {
	$this.plwin cmd plpoin $n $x $y $code
    }

    public name
}
