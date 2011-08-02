#! /usr/bin/env perl
#
# Demo x20 for the PLplot PDL binding
#
# plimage demo
#
# Copyright (C) 2004  Rafael Laboissiere
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

# SYNC: x20c.c 1.16

use PDL;
use PDL::Graphics::PLplot;
use PDL::IO::Pnm;
use Math::Trig qw [pi];
use Getopt::Long qw(:config pass_through);
use Text::Wrap;

$Text::Wrap::columns = 72;

use constant XDIM => 260;
use constant YDIM => 220;

sub main {

  my $dbg = 0;
  my $nosombrero = 0;
  my $nointeractive = 0;
  my $f_name = "";

  GetOptions ("dbg"           => \$dbg,
              "nosombrero"    => \$nosombrero,
              "nointeractive" => \$nointeractive,
              "save=s"        => \$f_name,
              "help"          => \$help);

  if ($help) {
    print (<<EOT);
$0 options:
    --dbg                 Extra debugging plot
    --nosombrero          No sombrero plot
    --nointeractive       No interactive selection
    --save filename       Save sombrero plot in color postscript 'filename'

EOT
    print (wrap ('', '', @notes), "\n");
    push (@ARGV, "-h");
  }

  unshift (@ARGV, $0);

  # Parse and process command line arguments

  plParseOpts (\@ARGV, PL_PARSE_PARTIAL);

  # Initialize plplot

  plinit ();

  # view image border pixels

  if ($dbg) {
    plenv (1, XDIM, 1, YDIM, 1, 1); # no plot box

    # build a one pixel square border, for diagnostics
    my $z = zeroes (XDIM, YDIM);

    $z->slice (":," . (YDIM - 1)) .= 1; # right
    $z->slice (":,0") .= 1;             # left
    $z->slice ("0,:") .= 1;             # top
    $z->slice ((XDIM - 1) . ",:") .= 1; # botton

    pllab ("...around a blue square."," ","A red border should appear...");

    plimage ($z, 1, XDIM, 1, YDIM, 0, 0, 1, XDIM, 1, YDIM);
  }

  # sombrero-like demo

  if (not $nosombrero) {
    plcol0 (2); # draw a yellow plot box, useful for diagnostics! :(
    plenv (0., 2 * pi, 0, 3 * pi, 1, -1);

    my $x = (2 * pi * sequence (XDIM) / (XDIM - 1))->dummy (1, YDIM);
    my $y = (3 * pi * sequence (YDIM) / (YDIM - 1))->dummy (0, XDIM);;

    my $r = sqrt ($x ** 2 + $y ** 2) + 1e-3;
    my $z = sin ($r) / ($r);

    pllab ("No, an amplitude clipped \"sombrero\"", "", "Saturn?");
    plptex (2, 2, 3, 4, 0, "Transparent image");
    plimage ($z, 0, 2 * pi, 0, 3 * pi, 0.05, 1, 0, 2 * pi, 0, 3 * pi);

    # save the plot
    save_plot ($f_name)
      if $f_name;
  }

  # read Lena image

  my $lena = "lena.pgm";
  my ($img_f, $width, $height, $num_col) = read_img ($lena);
  if ($width == 0 && $height == 0) {
    my $lena = "../lena.pgm";
    ($img_f, $width, $height, $num_col) = read_img ($lena);
    if ($width == 0 && $height == 0) {
      die "Cannot find image file $lena";
    }
  }

  # set gray colormap

  gray_cmap ($num_col);

  # display Lena

  plenv (1, $width, 1, $height, 1, -1);

  pllab (((not $nointeractive)
          ? "Set and drag Button 1 to (re)set selection, Button 2 to finish."
          : ""), " ", "Lena...");

  plimage ($img_f, 1, $width, 1, $height, 0, 0, 1, $width, 1, $height);

  # selection/expansion demo

  if (not $nointeractive) {
    $xi = 200;
    $xe = 330;
    $yi = 280;
    $ye = 220;

    if (get_clip (\$xi, \$xe, \$yi, \$ye)) { # get selection rectangle
      plend ();
      exit (0);
    }

    plspause (0);
    pladv (0);

    # display selection only
    plimage ($img_f, 1, $width, 1, $height, 0, 0, $xi, $xe, $ye, $yi);

    plspause (1);

    # zoom in selection
    plenv ($xi, $xe, $ye, $yi, 1, -1);
    plimage ($img_f, 1, $width, 1, $height, 0, 0, $xi, $xe, $ye, $yi);
  }

  # Base the dynamic range on the image contents.
  my ($img_min, $img_max) = $img_f->minmax;

  # Draw a saturated version of the original image.  Only use the middle 50%
  # of the image's full dynamic range.
  plcol0(2);
  plenv(0, $width, 0, $height, 1, -1);
  pllab("", "", "Reduced dynamic range image example");
  plimagefr($img_f, 0, $width, 0, $height, 0, 0, 
	    $img_min + $img_max * 0.25, $img_max - $img_max * 0.25, 0, 0);

  # Draw a distorted version of the original image, showing its full dynamic range.
  plenv(0, $width, 0, $height, 1, -1);
  pllab("", "", "Distorted image example");

  my $stretch = [0, $width, 0, $height, 0.5];

  # both xmap and ymap are 2D
  my ($xmap, $ymap) = mypltr(sequence($width+1), sequence($height+1), $stretch);

  my $grid = plAlloc2dGrid ($xmap, $ymap); # set of 2 2D maps

  plimagefr($img_f, 0, $width, 0, $height, 0, 0, $img_min, $img_max, \&pltr2, $grid);

  plFree2dGrid ($grid);

  plend ();
}

# set up 2D transformation matrices
sub mypltr {
  my ($x, $y, $stretch) = @_; # pdl, pdl, listref
  my $pi = atan2(1,1)*4;
  my $x0 = ($stretch->[0] + $stretch->[1])*0.5;
  my $y0 = ($stretch->[2] + $stretch->[3])*0.5;
  my $dy = ($stretch->[3] - $stretch->[2])*0.5;
  my $result0 = $x0 + outer(($x0-$x),(1.0 - $stretch->[4]*cos(($y-$y0)/$dy*$pi*0.5)));
  my $result1 = outer(ones($x->nelem),$y);
  return ($result0, $result1);
}


# set gray colormap

sub gray_cmap {
  my $num_col = shift;

  my $r = pdl [0, 1];
  my $g = pdl [0, 1];
  my $b = pdl [0, 1];
  my $pos = pdl [0, 1];

  plscmap1n ($num_col);

  plscmap1l (1, $pos, $r, $g, $b, pdl ([]));
}

# read image from file in PGN format

sub read_img {
  my $fname = shift;
  if (-r $fname) {
    my $img = rpnm ($fname);
    # We want the number of colours from the header, not the maximum 
    # colour. We can't get this via PDL, so hard code it to 255.
    #return ($img, $img->dims (), $img->max);
    return ($img, $img->dims (), 255);
  }
  else {
    return (0,0,0,0);
  }
}

# Get selection square interactively

sub get_clip {
  my ($xi, $xe, $yi, $ye) = @_;

  my $xxi = $$xi;
  my $yyi = $$yi;
  my $xxe = $$xe;
  my $yye = $$ye;
  my $start = 0;

  my $st = plxormod (1); # enter xor mode to draw a selection rectangle

  if ($st) { # driver has xormod capability, continue
    my $sx = zeroes (5);
    my $sy = zeroes (5);

    while (1) {
      plxormod (0);
      my %gin = plGetCursor ();
      plxormod (1);

      if ($gin{button} == 1) {
        $xxi = $gin{wX};
        $yyi = $gin{wY};

        plline ($sx, $sy) # clear previous rectangle
          if $start;

        $start = 0;

        $sx->index (0) .= $xxi;
        $sy->index (0) .= $yyi;
        $sx->index (4) .= $xxi;
        $sy->index (4) .= $yyi;
      }

      if ($gin{state} & 0x100) {
        $xxe = $gin{wX};
        $yye = $gin{wY};

        plline ($sx, $sy) # clear previous rectangle
          if ($start);

        $start = 1;

        $sx->index (1) .= $xxe;
        $sx->index (2) .= $xxe;
        $sx->index (3) .= $xxi;

        $sy->index (1) .= $yyi;
        $sy->index (2) .= $yye;
        $sy->index (3) .= $yye;


        plline ($sx, $sy); # draw new rectangle
      }

      if ($gin{button} == 3 or $gin{keysym} == PLK_Return
          or $gin{keysym} eq 'Q') {
        plline ($sx, $sy) # clear previous rectangle
          if ($start);
        last;
      }
    }
    plxormod (0); # leave xor mod
    if ($xxe < $xxi) {
	my $t = $xxi;
	$xxi = $xxe;
	$xxe = $t;
    }
    
    if ($yyi < $yye) {
	my $t = $yyi;
	$yyi = $yye;
	$yye = $t;
    }
    
    $$xe = $xxe;
    $$xi = $xxi;
    $$ye = $yye;
    $$yi = $yyi;
    
    return ($gin{keysym} eq 'Q');
  }

  return 0;
}

# save plot

sub save_plot {
  my $fname = shift;

  my $cur_strm = plgstrm ();  # get current stream
  my $new_strm = plmkstrm (); # create a new one

  plsdev ("psc");             # new device type. Use a known existing driver
  plsfnam ($fname);           # file name

  plcpstrm ($cur_strm, 0);    # copy old stream parameters to new stream
  plreplot ();	              # do the save
  plend1 ();                  # close new device

  plsstrm ($cur_strm);	      # and return to previous one
}

main ();
