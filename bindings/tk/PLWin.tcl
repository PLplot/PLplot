# PLWin.tcl
# Geoffrey Furnish
# 9 May 1994
#
# @> [incr Tcl] interface to PLplot
#
# $Id$
#
# $Log$
# Revision 1.1  1994/05/09 17:59:19  furnish
# The new [incr Tcl] interface to the PLplot Tcl extensions.
#
###############################################################################

itcl_class PLWin {

    constructor {config} {
	puts stdout "make a PLWin w/ name $name"

	set class [$this info class]
	::rename $this $this-tmp-
	::frame $this -class $class
	::rename $this $this-win-
	::rename $this-tmp- $this

#	frame $this

	plframe $this.plwin -relief sunken

	pack append $this $this.plwin {left expand fill}

	plw_configure_TopRow $this
    }

    method configure {config} {}

    method init {} {
	$this.plwin cmd init
    }

    method plcol {color} {
	# Should set the plstream here.
	::plcol $color
    }

    method plenv {xmin xmax ymin ymax just axis} {
	# Should set the plstream here.
	::plenv $xmin $xmax $ymin $ymax $just $axis
    }

    method pllab {xlab ylab toplab} {
	# Should set the plstream here.
	::pllab $xlab $ylab $toplab
    }

    method plline {pairs} {
	# Should set the plstream here.
	::plline $pairs
    }

# Should have a stream ID here.
    public name
}
