# PLWin.tcl
# Geoffrey Furnish
# 9 May 1994
#
# @> [incr Tcl] interface to PLplot
#
# $Id$
#
# $Log$
# Revision 1.8  1995/06/09 22:48:07  mjl
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

    method pladv {{subwindow {}}} {
	$this.plwin cmd pladv $subwindow
    }

    method plbop {} {
	$this.plwin cmd plbop
    }

    method plbox {xopt xtick nxsub yopt ytick nysub} {
	$this.plwin cmd plbox $xopt $xtick $nxsub $yopt $ytick $nysub
    }

    method plcol0 {col0} {
	$this.plwin cmd plcol0 $col0
    }

    method plcol {col0} {
	$this.plwin cmd plcol0 $col0
    }

    method plcont {data clev} {
	$this.plwin cmd plcont $data $clev
    }

    method plend {} {
	$this.plwin cmd plend 
    }

    method plend1 {} {
	$this.plwin cmd plend1 
    }

    method plenv {xmin xmax ymin ymax just axis} {
	$this.plwin cmd plenv $xmin $xmax $ymin $ymax $just $axis
    }

    method pleop {} {
	$this.plwin cmd pleop 
    }

    method plfont {font} {
	$this.plwin cmd plfont $font
    }

    method plfontld {font} {
	$this.plwin cmd plfontld $font
    }

    method plgra {} {
	$this.plwin cmd plgra 
    }

    method plhist {n data datmin datmax nbin oldwin} {
	$this.plwin cmd plhist $n $data $datmin $datmax $nbin $oldwin
    }

    method plinit {} {
	$this.plwin cmd plinit 
    }

    method pljoin {x1 y1 x2 y2} {
	$this.plwin cmd pljoin $x1 $y1 $x2 $y2
    }

    method pllab {xlabel ylabel tlabel} {
	$this.plwin cmd pllab $xlabel $ylabel $tlabel
    }

    method plline {n xarray yarray} {
	$this.plwin cmd plline $n $xarray $yarray
    }

    method plmtex {side disp pos just text} {
	$this.plwin cmd plmtex $side $disp $pos $just $text
    }

    method plpoin {n xarray yarray code} {
	$this.plwin cmd plpoin $n $xarray $yarray $code
    }

    method plptex {x y dx dy just text} {
	$this.plwin cmd plptex $x $y $dx $dy $just $text
    }

    method plschr {def scale} {
	$this.plwin cmd plschr $def $scale
    }

    method plscol0 {index r g b} {
	$this.plwin cmd plscol0 $index $r $g $b
    }

    method plscolbg {r g b} {
	$this.plwin cmd plscolbg $r $g $b
    }

    method plsetopt {option arg} {
	$this.plwin cmd plsetopt $option $arg
    }

    method plshade {data xmin xmax ymin ymax sh_min sh_max sh_col} {
	$this.plwin cmd plshade $data $xmin $xmax $ymin $ymax \
	    $sh_min $sh_max $sh_col
    }

    method plsstrm {stream} {
	$this.plwin cmd plsstrm $stream
    }

    method plssub {nsubx nsuby} {
	$this.plwin cmd plssub $nsubx $nsuby
    }

    method plstyl {nels mark space} {
	$this.plwin cmd plstyl $nels $mark $space
    }

    method plsxax {digmax} {
	$this.plwin cmd plsxax $digmax
    }

    method plsyax {digmax} {
	$this.plwin cmd plsyax $digmax
    }

    method plsym {n xarray yarray code} {
	$this.plwin cmd plsym $n $xarray $yarray $code
    }

    method pltext {} {
	$this.plwin cmd pltext 
    }

    method plvpor {xmin xmax ymin ymax} {
	$this.plwin cmd plvpor $xmin $xmax $ymin $ymax
    }

    method plvsta {} {
	$this.plwin cmd plvsta 
    }

    method plwid {width} {
	$this.plwin cmd plwid $width
    }

    method plwind {xmin xmax ymin ymax} {
	$this.plwin cmd plwind $xmin $xmax $ymin $ymax
    }

    public name
}
