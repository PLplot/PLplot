#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

# Does a series of 3-d plots for a given data set, with different
# viewing options in each plot.

# Routine for restoring colour map1 to default.
# See static void plcmap1_def(void) in plctrl.c for reference.
proc restore_cmap1 {w} {
   # For center control points, pick black or white, whichever is closer to bg 
   # Be careful to pick just short of top or bottom else hue info is lost
   $w cmd plgcolbg rbg gbg bbg
   set vertex [expr ($rbg + $gbg + $bbg)/(3.*255.)]
   if {$vertex < 0.5} {
      set vertex 0.01
      set midpt 0.10
   } else {
      set vertex 0.99
      set midpt 0.90
   }
   # Independent variable of control points.
   matrix i f 6 = {0., 0.44, 0.50, 0.50, 0.56, 1.}
   # Hue for control points.  Blue-violet to red
   matrix h f 6 = {260., 260., 260., 0., 0., 0.}
   # Lightness ranging from medium to vertex to medium
   # double quotes required rather than braces so that variables
   # get evaluated for initialization.
   matrix l f 6  = "0.5, $midpt, $vertex, $vertex, $midpt, 0.5"
   # Saturation is complete for default
   matrix s f 6 = {1., 1., 1., 1., 1., 1.}
   # Integer flag array is zero (no interpolation along far-side of colour
   # figure
   matrix rev i 6 = {0, 0, 0, 0, 0, 0}
   # Default number of cmap1 colours
   $w cmd plscmap1n 128
   # Interpolate between control points to set up default cmap1.
   $w cmd plscmap1l 0 6 i h l s rev
}

# Routine for initializing color map 1 in HLS space.
# Basic grayscale variation from half-dark (which makes more interesting
# looking plot compared to dark) to light.
proc cmap1_init {w gray} {
   # Independent variable of control points.
   matrix i f 2 = {0., 1.}
   if {$gray == 1} {
      # Hue for control points.  Doesn't matter since saturation is zero.
      matrix h f 2 = {0., 0.}
      # Lightness ranging from half-dark (for interest) to light.
      matrix l f 2 = {0.5, 1.}
      # Gray scale has zero saturation.
      matrix s f 2 = {0., 0.}
   } else {
      # Hue ranges from blue (240 deg) to red (0 or 360 deg)
      matrix h f 2 = {240., 0.}
      # Lightness and saturation are constant (values taken from C example).
      matrix l f 2 = {0.6, 0.6}
      matrix s f 2 = {0.8, 0.8}
   }

   # Integer flag array is zero (no interpolation along far-side of colour
   # figure
   matrix rev i 2 = {0, 0}
   # Number of cmap1 colours is 256 in this case. 
   $w cmd plscmap1n 256
   # Interpolate between control points to set up default cmap1.
   $w cmd plscmap1l 0 2 i h l s rev
}

proc x08 {{w loopback}} {

    # this should be defined elsewhere.
    set SURF_CONT 0x10
    set BASE_CONT 0x20
    set FACETED   0x80
    set MAG_COLOR 0x04

    matrix opt i 4 = {1, 2, 3, 3}
    matrix alt f 4 = {60.0, 20.0, 60.0, 60.0}
    matrix az  f 4 = {30.0, 60.0, 120.0, 160.0}

    set xpts 35
    set ypts 46
    set n_col 256
    set two_pi [expr 2.0 * 3.14159265358979323846 ]

    matrix x f $xpts
    matrix y f $ypts
    matrix z f $xpts $ypts

    for {set i 0} {$i < $xpts} {incr i} {
	x $i = [expr ($i - ($xpts/2)) / double($xpts/2) ]
    }

    for {set i 0} {$i < $ypts} {incr i} {
	y $i = [expr ($i - ($ypts/2)) / double($ypts/2) ]
    }

    for {set i 0} {$i < $xpts} {incr i} {
	set xx [x $i]
	for {set j 0} {$j < $ypts} {incr j} {
	    set yy [y $j]
	    set r [expr sqrt( $xx * $xx + $yy * $yy ) ]

	    z $i $j = [expr exp(-$r * $r) * cos( $two_pi * $r ) ]
	}
    }

    set zmin [z min [ expr $xpts * $ypts]]
    set zmax [z max [ expr $xpts * $ypts]]

    set nlev 10
    matrix clev f $nlev
    set step [expr ($zmax-$zmin)/$nlev]
    for {set i 0} {$i < $nlev} {incr i} {
	# odd, there seems to be some floating point problems here, I have to
	# add a small number, else I get the runtime error:
	# "plcol1: Invalid color map position: -0.000000, aborting operation"
	# humm, I remember to see something about tcl precision -- numbers are stored
	# as strings with a small default precision?
	clev $i = [expr $zmin + $i * $step + 0.000001]
    }

    $w cmd pllightsource 1. 1. 1.
    for {set k 0} {$k < 4} {incr k} {
	for {set ifshade 0} {$ifshade < 6} {incr ifshade} {
	    $w cmd pladv 0
	    $w cmd plvpor 0.0 1.0 0.0 0.9
	    $w cmd plwind -1.0 1.0 -0.9 1.1
	    $w cmd plcol0 3
	    set title [format "#frPLplot Example 8 - Alt=%.0f, Az=%.0f" \
			   [alt $k] [az $k]]
	$w cmd plmtex "t" 1.0 0.5 0.5 $title
	$w cmd plcol0 1
	$w cmd plw3d 1.0 1.0 1.0 -1.0 1.0 -1.0 1.0 -1.0 1.0 [alt $k] [az $k]
	$w cmd plbox3 "bnstu" "x axis" 0.0 0 \
	    "bnstu" "y axis" 0.0 0 \
	    "bcdmnstuv" "z axis" 0.0 0
  	  $w cmd plcol0 2
	  if {$ifshade == 0} {
	     $w cmd plot3d x y z [opt $k] 1
	  } elseif {$ifshade == 1} {
	     #set up modified false colour cmap1.
	     cmap1_init $w 0
	     $w cmd plmesh x y z [expr [opt $k] | $MAG_COLOR]
	  } elseif {$ifshade == 2} {
	     #set up modified gray scale cmap1.
	     cmap1_init $w 1
	     $w cmd plsurf3d x y z 0
	  } elseif {$ifshade == 3} {
	     #set up modified false colour cmap1.
	     cmap1_init $w 0
	     $w cmd plsurf3d x y z [expr [opt $k] | $MAG_COLOR]
	  } elseif {$ifshade == 4} {
	     #set up modified false colour cmap1.
	     cmap1_init $w 0
	     $w cmd plsurf3d x y z [expr [opt $k] | $MAG_COLOR | $FACETED]
	  } else {
	     #set up false colour cmap1.
	     cmap1_init $w 0
	     $w cmd plsurf3d x y z $xpts $ypts \
		 [expr [opt $k] | $MAG_COLOR | $SURF_CONT | $BASE_CONT] clev $nlev
	  }
       }
    }

# Restore defaults
    $w cmd plcol0 1
    restore_cmap1 $w
}
