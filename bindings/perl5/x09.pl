#!/usr/bin/env perl

# $Id$

#	Contour plot demo.

use PLplot qw(:all);

$PI = 3.14159265;

$XPTS = 35;		# Data points in x
$YPTS = 46;		# Data points in y

$XSPA = 2./($XPTS-1);
$YSPA = 2./($YPTS-1);

@clevel = (-1., -.8, -.6, -.4, -.2, 0, .2, .4, .6, .8, 1.);

# Transformation function

@tr = ($XSPA, 0.0, -1.0, 0.0, $YSPA, -1.0);

sub mypltr
{
  my $x = shift;
  my $y = shift;
  my $tx = shift;
  my $ty = shift;
  
  $$tx = $tr[0] * $x + $tr[1] * $y + $tr[2];
  $$ty = $tr[3] * $x + $tr[4] * $y + $tr[5];

}

# --------------------------------------------------------------------------
#  main
# 
#  Does several contour plots using different coordinate mappings.
# --------------------------------------------------------------------------

$mark = 1500;
$space = 1500;

# Initialize plplot

plinit ();

# Set up function arrays

for ($i = 0; $i < $XPTS; $i++) {
  $xx = ($i - ($XPTS / 2)) / ($XPTS / 2);
  for ($j = 0; $j < $YPTS; $j++) {
    $yy = ($j - ($YPTS / 2)) / ($YPTS / 2) - 1.0;
    $z[$i][$j] = $xx * $xx - $yy * $yy;
    $w[$i][$j] = 2 * $xx * $yy;
  }
}

# Set up grids

for ($i = 0; $i < $XPTS; $i++) {
  for ($j = 0; $j < $YPTS; $j++) {

    mypltr ( $i, $j, \$xx, \$yy, 0);
    
    $argx = $xx * $PI/2;
    $argy = $yy * $PI/2;
    $distort = 0.4;
    
    $g1{xg}[$i] = $xx + $distort * cos($argx);
    $g1{yg}[$j] = $yy - $distort * cos($argy);
    
    $g2{xg}[$i][$j] = $xx + $distort * cos($argx) * cos($argy);
    $g2{yg}[$i][$j] = $yy - $distort * cos($argx) * cos($argy);

  }
}

$cgrid1 = plgrid_new (\%g1);
# $cgrid2 = plgrid_new (\%g2);

# Plot using identity transform

plenv (-1.0, 1.0, -1.0, 1.0, 0, 0);
plcol0 (2);
plcont (\@z, $XPTS, $YPTS, 1, $XPTS, 1, $YPTS, \@clevel, 11, "mypltr", "z");
plstyl (1, $mark, $space);
plcol0 (3);
plcont (\@w, $XPTS, $YPTS, 1, $XPTS, 1, $YPTS, \@clevel, 11, \&mypltr, "w");
plstyl (0, $mark, $space);
plcol0 (1);
pllab ("X Coordinate", "Y Coordinate", "Streamlines of flow");

pl_setcontlabelparam (0.006, 0.3, 0.1, 1);
plenv (-1.0, 1.0, -1.0, 1.0, 0, 0);
plcol0 (2);
plcont (\@z, $XPTS, $YPTS, 1, $XPTS, 1, $YPTS, \@clevel, 11, "mypltr", 0);
plstyl (1, $mark, $space);
plcol0 (3);
plcont (\@w, $XPTS, $YPTS, 1, $XPTS, 1, $YPTS, \@clevel, 11, "mypltr", 0);
plstyl(0, $mark, $space);
plcol0 (1);
pllab ("X Coordinate", "Y Coordinate", "Streamlines of flow");
    
# Plot using 1d coordinate transform

pl_setcontlabelparam (0.006, 0.3, 0.1, 0);

plenv (-1.0, 1.0, -1.0, 1.0, 0, 0);
plcol0 (2);
plcont (\@z, $XPTS, $YPTS, 1, $XPTS, 1, $YPTS, \@clevel, 11,
	"pltr1", $cgrid1);

plstyl (1, $mark, $space);
plcol0 (3);
plcont (\@w, $XPTS, $YPTS, 1, $XPTS, 1, $YPTS, \@clevel, 11,
	"pltr1", $cgrid1);
plstyl (0, $mark, $space);
plcol0 (1);
pllab ("X Coordinate", "Y Coordinate", "Streamlines of flow");

pl_setcontlabelparam (0.006, 0.3, 0.1, 1);
plenv (-1.0, 1.0, -1.0, 1.0, 0, 0);
plcol0 (2);
plcont (\@z, $XPTS, $YPTS, 1, $XPTS, 1, $YPTS, \@clevel, 11,
	"pltr1", $cgrid1);

plstyl (1, $mark, $space);
plcol0 (3);
plcont (\@w, $XPTS, $YPTS, 1, $XPTS, 1, $YPTS, \@clevel, 11,
	"pltr1", $cgrid1);
plstyl (0, $mark, $space);
plcol0 (1);
pllab ("X Coordinate", "Y Coordinate", "Streamlines of flow");
pl_setcontlabelparam (0.006, 0.3, 0.1, 0);
    
plend ();

__DATA__

/* Plot using 2d coordinate transform */

    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
    plcol0(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr2, (void *) &cgrid2);

    plstyl(1, &mark, &space);
    plcol0(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr2, (void *) &cgrid2);
    plstyl(0, &mark, &space);
    plcol0(1);
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");

    pl_setcontlabelparam(0.006, 0.3, 0.1, 1);
    plenv(-1.0, 1.0, -1.0, 1.0, 0, 0);
    plcol0(2);
    plcont(z, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr2, (void *) &cgrid2);

    plstyl(1, &mark, &space);
    plcol0(3);
    plcont(w, XPTS, YPTS, 1, XPTS, 1, YPTS, clevel, 11,
	   pltr2, (void *) &cgrid2);
    plstyl(0, &mark, &space);
    plcol0(1);
    pllab("X Coordinate", "Y Coordinate", "Streamlines of flow");
    
    plend();
    free((void *) w);
    free((void *) z);
    exit(0);
}
