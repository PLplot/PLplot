# Plwindow.tcl
# Vince Darley
# 1997-1998
# Geoffrey Furnish
# 9 May 1994
#
# @> [incr Tcl] interface to PLplot
#
# $Id$
#
# $Log$
# Revision 1.1  2002/07/02 09:03:44  vincentdarley
# x-platform tk code
#
# Revision 1.1.2.1  2000/07/27 16:11:05  airwin
# AWI: apply initial pltcl.patch for TEA-based approach
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

option add *Plwindow.width 250 widgetDefault
option add *Plwindow.height 250 widgetDefault

package require Itk 3.0
package require Plplotter

#
# Usual options.
#
itk::usual Plwindow {
    keep -background -cursor -foreground \
      -plotbackground -xhairs -doublebuffer -width -height
}
      
itcl::class Plwindow {
    inherit itk::Widget

    constructor {args} {
	itk_component add plwin {
	    plframe $itk_interior.plwin -relief sunken
	} { 
	    rename -background -plotbackground plotBackground Background

	    keep -width -height -xhairs -doublebuffer
	}
	grid $itk_component(plwin) -row 1 -column 0 -sticky nsew
	grid rowconfigure $itk_interior 1 -weight 1 -minsize 0

	eval itk_initialize $args
    }
    destructor {
    }
    
    method cmd {args} {
	uplevel 1 $itk_interior.plwin cmd $args
    }

    method plcol {color} {
	cmd plcol0 $color
    }

    method plcont {data clev} {
	upvar $data d
	upvar $clev c
	cmd plcont d c
    }

    method plenv {xmin xmax ymin ymax just axis} {
	cmd plenv $xmin $xmax $ymin $ymax $just $axis
    }

    method pllab {xlab ylab toplab} {
	cmd pllab $xlab $ylab $toplab
    }

    method plline {n x y} {
	cmd plline $n $x $y
    }

    method plpoin {n x y code} {
	cmd plpoin $n $x $y $code
    }

    method plshade {data xmin xmax ymin ymax sh_min sh_max sh_col} {
	cmd plshade $data $xmin $xmax $ymin $ymax \
	  $sh_min $sh_max 1 $sh_col 
    }

    public variable name
}









