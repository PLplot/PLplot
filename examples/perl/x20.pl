#! /usr/bin/env perl
#
# Demo x20 for PLplot PDL bindings
#
# plimage demo
#
# Copyright (C) 2004  Rafael Laboissiere
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Library Public License as published
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
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

use PDL;
use PDL::Graphics::PLplot;
use PDL::IO::Pnm;
use Math::Trig qw [pi];
use Getopt::Long qw(:config pass_through);
use Text::Wrap;

$Text::Wrap::columns = 72;

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
  my $img = rpnm ($fname);
  return ($img, $img->dims (), $img->max);
}

use constant XDIM => 260;
use constant YDIM => 220;

my $dbg = 0;
my $nosombrero = 0;
my $nointeractive = 0;
my $f_name = "";

GetOptions ("dbg"           => \$dbg,
            "nosombrero"    => \$nosombrero,
            "nointeractive" => \$nointeractive,
            "save=s"        => \$f_name);

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
#if ($dbg) {
#  plenv (1, XDIM, 1, YDIM, 1, 1); # no plot box
#
#  # build a one pixel square border, for diagnostics
#    for (i=0; i<XDIM; i++)
#      z[i][YDIM-1] = 1.; /* right */
#    for (i=0; i<XDIM; i++)
#      z[i][0] = 1.; /* left */
#
#    for (i=0; i<YDIM; i++)
#      z[0][i] = 1.; /* top */
#    for (i=0; i<YDIM; i++)
#      z[XDIM-1][i] = 1.; /* botton */
#
#    pllab("...around a blue square."," ","A red border should appear...");
#
#    plimage(z, XDIM, YDIM,
#	    1., (PLFLT) XDIM, 1., (PLFLT) YDIM, 0., 0.,
#	    1., (PLFLT) XDIM, 1., (PLFLT) YDIM);
#
#    pladv(0);
#  }
#
#  /* sombrero-like demo */
#  if (!nosombrero) {
#    plAlloc2dGrid(&r, XDIM, YDIM);
#    plcol0(2); /* draw a yellow plot box, useful for diagnostics! :( */
#    plenv(0., 2.*PI, 0, 3.*PI, 1, -1);
#
#    for (i=0; i<XDIM; i++)
#      x[i] = i*2.*PI/(XDIM-1);
#    for (i=0; i<YDIM; i++)
#      y[i] = i*3.*PI/(YDIM-1);
#
#    for (i=0; i<XDIM; i++)
#      for (j=0; j<YDIM; j++) {
#	r[i][j] = sqrt(x[i]*x[i]+y[j]*y[j])+1e-3;
#	z[i][j] = sin(r[i][j]) / (r[i][j]);
#      }
#
#    pllab("No, an amplitude clipped \"sombrero\"", "", "Saturn?");
#    plptex(2., 2., 3., 4., 0., "Transparent image");
#    plimage(z, XDIM, YDIM, 0., 2.*PI, 0, 3.*PI, 0.05, 1.,
#	    0., 2.*PI, 0, 3.*PI);
#    plFree2dGrid(r, XDIM, YDIM);
#
#    /* save the plot */
#    if (f_name)
#      save_plot(f_name);
#
#    pladv(0);
#    }
#
#  plFree2dGrid(z, XDIM, YDIM);
#

# read Lena image

my ($img_f, $width, $height, $num_col) = read_img ("../c/lena.pgm");

# set gray colormap

gray_cmap ($num_col);

# display Lena

plenv (1, $width, 1, $height, 1, -1);

pllab ((not $nointeractive
        ? "Set and drag Button 1 to (re)set selection, Button 2 to finish."
        : ""), " ", "Lena...");

plimage ($img_f, 1, $width, 1, $height, 0, 0, 1, $width, 1, $height);

#  /* selection/expansion demo */
#  if (!nointeractive) {
#    xi = 200.; xe = 330.;
#    yi = 280.; ye = 220.;
#
#    if (get_clip(&xi, &xe, &yi, &ye)) { /* get selection rectangle */
#      plend();
#      exit(0);
#    }
#
#    /*
#       I'm unable to continue, clearing the plot and advancing to the next
#       one, without hiting the enter key, or pressing the button... help!
#
#       Forcing the xwin driver to leave locate mode and destroying the
#       xhairs (in GetCursorCmd()) solves some problems, but I still have
#       to press the enter key or press Button-2 to go to next plot, even
#       if a pladv() is not present!  Using plbop() solves the problem, but
#       it shouldn't be needed!
#    */
#
#    /* plbop(); */
#
#    /*
#       plspause(0), pladv(0), plspause(1), also works,
#       but the above question remains.
#       With this approach, the previous pause state is lost,
#       as there is no API call to get its current state.
#    */
#
#    plspause(0);
#    pladv(0);
#
#    /* display selection only */
#    plimage(img_f, width, height, 1., width, 1., height, 0., 0., xi, xe, ye, yi);
#
#    plspause(1);
#    pladv(0);
#
#    /* zoom in selection */
#    plenv(xi, xe, ye, yi, 1, -1);
#    plimage(img_f, width, height, 1., width, 1., height, 0., 0., xi, xe, ye, yi);
#    pladv(0);
#  }
#
#  plFree2dGrid(img_f, width, height);
#
#  plend();
#  exit(0);
#}
#
#/* save plot */
#void save_plot(char *fname)
#{
#  int cur_strm, new_strm;
#
#  plgstrm(&cur_strm); /* get current stream */
#  plmkstrm(&new_strm); /* create a new one */
#
#  plsdev("psc"); /* new device type. Use a known existing driver */
#  plsfnam(fname); /* file name */
#
#  plcpstrm(cur_strm, 0); /* copy old stream parameters to new stream */
#  plreplot();	/* do the save */
#  plend1(); /* close new device */
#
#  plsstrm(cur_strm);	/* and return to previous one */
#}
#
#/*  get selection square interactively */
#int get_clip(PLFLT *xi, PLFLT *xe, PLFLT *yi, PLFLT *ye)
#{
#  PLGraphicsIn gin;
#  PLFLT xxi=*xi, yyi=*yi, xxe=*xe, yye=*ye, t;
#  int st, start = 0;
#
#  plxormod(1, &st); /* enter xor mode to draw a selection rectangle */
#
#  if (st) { /* driver has xormod capability, continue */
#    while(1) {
#      PLFLT sx[5], sy[5];
#
#      plxormod(0, &st);
#      plGetCursor(&gin);
#      plxormod(1, &st);
#
#      if (gin.button == 1) {
#	xxi = gin.wX; yyi = gin.wY;
#	if (start)
#	  plline(5, sx, sy); /* clear previous rectangle */
#
#	start = 0;
#
#	sx[0] = xxi; sy[0] = yyi;
#	sx[4] = xxi; sy[4] = yyi;
#      }
#
#      if (gin.state && 0x100) {
#	xxe = gin.wX; yye = gin.wY;
#	if (start)
#	  plline(5, sx, sy); /* clear previous rectangle */
#
#	start = 1;
#
#	sx[2] = xxe; sy[2] = yye;
#	sx[1] = xxe; sy[1] = yyi;
#	sx[3] = xxi; sy[3] = yye;
#	plline(5, sx, sy); /* draw new rectangle */
#      }
#
#      if (gin.button == 3 || gin.keysym == PLK_Return || gin.keysym == 'Q') {
#	if (start)
#	  plline(5, sx, sy); /* clear previous rectangle */
#	break;
#      }
#    }
#    plxormod(0, &st); /* leave xor mod */
#  }
#
#  if (xxe < xxi) {
#    t=xxi; xxi=xxe; xxe=t;}
#
#  if (yyi < yye) {
#    t=yyi; yyi=yye; yye=t;}
#
#  *xe = xxe; *xi = xxi;
#  *ye = yye; *yi = yyi;
#
#  return (gin.keysym == 'Q');
#}
#
#
#

plend ();
