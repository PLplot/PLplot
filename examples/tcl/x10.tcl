#----------------------------------------------------------------------------
# $Id$
#----------------------------------------------------------------------------

proc x10 {{w loopback}} {
  $w cmd pladv 0
# use color index 1 in cmap0.  Should be red unless cmap0 has been
# fiddled with by another tcl example.
  $w cmd plcol0 1
  $w cmd pladv 0
  $w cmd plvpor 0.0 1.0 0.0 1.0
  $w cmd plwind 0.0 1.0 0.0 1.0
  $w cmd plbox bc 0.0 0 bc 0.0 0
  $w cmd plsvpa 50.0 150.0 100.0 150.0
  $w cmd plwind 0.0 1.0 0.0 1.0
  $w cmd plbox bc 0.0 0 bc 0.0 0
  $w cmd plptex 0.5 0.5 1.0 0.0 0.5 "BOX at (50,150,100,150)"
  $w cmd pleop
}
