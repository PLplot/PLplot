#!@SH_EXECUTABLE@
# $Id$
#
# Copyright (C) 2004, 2007  Alan W. Irwin
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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

# test suite for executing all configured demos (either in plplot/tmp
# or else in the install area) capable of file output.  Our ultimate aim
# is to compare the generated files with results generated on a system
# that is known to give good results.  However, single-precision
# contamination (even when double is configured) and the consequent large
# roundoff error is currently hampering our cross-platform comparisons.
# So for now, this is only a test suite in the sense that it checks the
# files corresponding to our demo plots can all be generated.  It is
# up to the user to display those files with the appropriate software
# (e.g., gv for postscript files and kview or a browser for png, gif,
# or jpeg files on Linux systems) to make sure they display properly.

version=@VERSION@

EXAMPLES_DIR=${EXAMPLES_DIR:-.}
SRC_EXAMPLES_DIR=${SRC_EXAMPLES_DIR:-.}
OUTPUT_DIR=${OUTPUT_DIR:-.}
device=${DEVICE:-psc}
export EXAMPLES_DIR SRC_EXAMPLES_DIR OUTPUT_DIR device DEBUG_CMD

usage()
{
echo '
Usage: plplot-test.sh [OPTIONS]

Options:
   [--device=DEVICE] (DEVICE = any cmake-enabled device.  psc is the default)
   [--front-end=FE]  (FE = one of c, cxx, f77, f95 java, octave, python, tcl, perl, ada, ocaml, or lua)
                     If this option is not specified, then all front-ends will
                     be tested.  More than one front-end may be given, like
                     this --front-end="c cxx"
   [--examples-dir=/path/to/examples/dir]
                     Specify path to the examples directory.  Defaults to "."
   [--src-examples-dir=/path/to/src/examples/dir]
                     Specify path to the src examples directory.  Defaults to "."
   [--output-dir=/path/to/output/dir]
                     Specify location where the resulting files are stored.
                     Defaults to "."
   [--verbose]	     Echo each PLplot example that is executed.
   [--debug="debug command"]         
                     Run examples with given debug command.
   [--debug]         Run examples with default debug command (valgrind).
   [--version]
   [--help]

Environment variables:
   DEVICE, FRONT_END, EXAMPLES_DIR, SRC_EXAMPLES_DIR, and OUTPUT_DIR can be 
   used to specify the devices, front-ends, the examples directory, the
   source examples directory (used for the special case of build-tree checks 
   when that tree is separated from the source tree), and output directory.
   These environment variables are overridden by the options --device,
   --front-end, --examples-dir, --src-examples-dir, and --output-dir.
'
   exit $1
}

while test $# -gt 0; do
   if [ "@HAVE_BASH@" = "ON" ] ; then
      case "$1" in
      -*=*) optarg=${1#*=} ;;
      *) optarg= ;;
      esac
   else
      case "$1" in
      -*=*) optarg=`echo "$1" | sed 's/[-_a-zA-Z0-9]*=//'` ;;
      *) optarg= ;;
      esac
   fi

   case $1 in
      --device=*)
         device=$optarg
         ;;
      --verbose)
         export verbose_test=on
         ;;
      --version)
         echo $version
	 exit 0
         ;;
      --front-end=*)
         FRONT_END=$optarg
         for i in $FRONT_END ; do
           [ $i = "c"         \
             -o $i = "cxx"    \
             -o $i = "f77"    \
             -o $i = "f95"    \
             -o $i = "java"   \
             -o $i = "octave" \
             -o $i = "python" \
             -o $i = "tcl"    \
             -o $i = "perl"    \
             -o $i = "ada"    \
             -o $i = "ocaml"    \
             -o $i = "lua"    \
           ] \
           || usage 0 1>&2
         done
	 ;;
      --examples-dir=*)
         EXAMPLES_DIR=$optarg
         ;;
      --src-examples-dir=*)
         SRC_EXAMPLES_DIR=$optarg
         ;;
      --output-dir=*)
         OUTPUT_DIR=$optarg
         ;;
      --debug=*)
         DEBUG_CMD=$optarg
         ;;
      --debug)
         DEBUG_CMD="valgrind --leak-check=full --show-reachable=yes --log-file=valgrind.x%q{index}%q{lang}.%q{dsuffix}.log"
         ;;
      --help)
         usage 0 1>&2
         ;;
      *)
         usage 1 1>&2
         ;;
   esac
   shift
done

# This script is only designed to work when EXAMPLES_DIR is a directory
# with a subdirectory called "c".  Check whether this conditions is true.

if [ ! -d $EXAMPLES_DIR/c ] ; then
echo '
This script is only designed to work when the EXAMPLES_DIR environment
variable (overridden by option --examples-dir) is a directory with a
subdirectory called "c".  This condition has been violated.
'
exit 1
fi

# These variables set by default assuming you are going to run this
# script from the installed demos directory $prefix/lib/plplot$version/examples.
cdir=$EXAMPLES_DIR/c
cxxdir=$EXAMPLES_DIR/c++
f77dir=$EXAMPLES_DIR/f77
f95dir=$EXAMPLES_DIR/f95
pythondir=$EXAMPLES_DIR/python
tcldir=$EXAMPLES_DIR/tcl
perldir=$SRC_EXAMPLES_DIR/perl
javadir=$EXAMPLES_DIR/java
adadir=$EXAMPLES_DIR/ada
ocamldir=$EXAMPLES_DIR/ocaml
luadir=$EXAMPLES_DIR/lua
octave=@OCTAVE@
octavedir=\
$EXAMPLES_DIR/../bindings/octave:\
$SRC_EXAMPLES_DIR/../bindings/octave/PLplot:\
$SRC_EXAMPLES_DIR/../bindings/octave/PLplot/support:\
$SRC_EXAMPLES_DIR/../bindings/octave/demos:\
$SRC_EXAMPLES_DIR/../bindings/octave/misc:\
$SRC_EXAMPLES_DIR/octave:\
@PLPLOT_OCTAVE_DIR@:\
@PLPLOT_OCTAVE_DIR@/support:\
@OCTAVE_M_DIR@/PLplot:\
@OCTAVE_OCT_DIR@:
PATH=$EXAMPLES_DIR/../utils:@BIN_DIR@:$PATH
export cdir cxxdir f77dir f95dir pythondir javadir octave octavedir tcldir perldir adadir ocamldir luadir PATH

fe=""

# List of non-interactive (i.e., file) devices for PLplot that 
# _might_ be enabled.  For completeness you may want to specify all devices
# here, but be sure to comment out the interactive ones since the configured
# plplot-test.sh script does not work with interactive devices.

#interactive PLD_aqt=@PLD_aqt@
PLD_cgm=@PLD_cgm@
PLD_conex=@PLD_conex@
PLD_dg300=@PLD_dg300@
PLD_epsqt=@PLD_epsqt@
PLD_pdfqt=@PLD_pdfqt@
#interactive PLD_qtwidget=@PLD_qtwidget@
PLD_rasterqt=@PLD_rasterqt@
PLD_svgqt=@PLD_svgqt@
#interactive PLD_gcw=@PLD_gcw@
PLD_gif=@PLD_gif@
#interactive PLD_gnome=@PLD_gnome@
PLD_hp7470=@PLD_hp7470@
PLD_hp7580=@PLD_hp7580@
PLD_imp=@PLD_imp@
PLD_jpeg=@PLD_jpeg@
PLD_linuxvga=@PLD_linuxvga@
PLD_lj_hpgl=@PLD_lj_hpgl@
PLD_ljii=@PLD_ljii@
PLD_ljiip=@PLD_ljiip@
#not a file device PLD_mem=@PLD_mem@
#not a file device PLD_memcairo=@PLD_memcairo@
PLD_mskermit=@PLD_mskermit@
#interactive PLD_ntk=@PLD_ntk@
PLD_null=@PLD_null@
PLD_pbm=@PLD_pbm@
PLD_pdf=@PLD_pdf@
PLD_pdfcairo=@PLD_pdfcairo@
PLD_plmeta=@PLD_plmeta@
PLD_png=@PLD_png@
PLD_pngcairo=@PLD_pngcairo@
PLD_ps=@PLD_ps@
# special case
PLD_psc=@PLD_ps@
PLD_pscairo=@PLD_pscairo@
PLD_pstex=@PLD_pstex@
PLD_psttf=@PLD_psttf@
# special case
PLD_psttfc=@PLD_psttf@
PLD_svg=@PLD_svg@
PLD_svgcairo=@PLD_svgcairo@
PLD_tek4010=@PLD_tek4010@
PLD_tek4010f=@PLD_tek4010f@
PLD_tek4107=@PLD_tek4107@
PLD_tek4107f=@PLD_tek4107f@
#interactive PLD_tk=@PLD_tk@
#interactive PLD_tkwin=@PLD_tkwin@
PLD_versaterm=@PLD_versaterm@
PLD_vlt=@PLD_vlt@
#interactive PLD_wingcc=@PLD_wingcc@
PLD_wxpng=@PLD_wxpng@
#interactive PLD_wxwidgets=@PLD_wxwidgets@
#interactive PLD_xcairo=@PLD_xcairo@
PLD_xfig=@PLD_xfig@
PLD_xterm=@PLD_xterm@
#interactive PLD_xwin=@PLD_xwin@

eval pld_device='$'PLD_$device
if [ -z "$pld_device" ] ; then
echo '
Never heard of a file device called '"$device"'.  Either this is not a
legitimate file (i.e. non-interactive) device for PLplot or else 
plplot-test.sh.cmake needs some maintenance to include this file device in
the list of possible PLplot file devices.
'
exit 1
fi

if [ ! "$pld_device" = "ON" ] ; then
echo '
PLD_'"$device"' is defined as '"$pld_device"'.  It must be ON (i.e., enabled
by your cmake configuration and built properly) before you can use this
script with DEVICE='"$device"'.
'
exit 1
fi

# Some devices require special options others do not.
case "$device" in
   png|pngcairo|jpeg|xfig|svg|svgcairo)
      options="-fam -fflen 2"
      ;;
   gif)
   # gif standard is limited to 256 colours so cannot have aliasing turned
   # on (which uses additional colours) for examples which already have
   # a substantial number of colours (such as example 2 with its 116 colours).
      options="-fam -fflen 2 -drvopt smooth=0"
      ;;
   *)
      options=
      ;;
esac
dsuffix=$device
export dsuffix options

# Find out what front-ends have been configured
if [ -z "$FRONT_END" ] ; then
   FRONT_END=c
   test "@ENABLE_cxx@" = "ON" && FRONT_END="$FRONT_END cxx"
   test "@ENABLE_f77@" = "ON"    && FRONT_END="$FRONT_END f77"
   test "@ENABLE_f95@" = "ON"    && FRONT_END="$FRONT_END f95"
   test "@ENABLE_java@" = "ON"   && FRONT_END="$FRONT_END java"
   test "@ENABLE_octave@" = "ON" && FRONT_END="$FRONT_END octave"
   test "@ENABLE_python@" = "ON" && FRONT_END="$FRONT_END python"
   test "@ENABLE_tcl@" = "ON"    && FRONT_END="$FRONT_END tcl"
   test "@ENABLE_pdl@" = "ON"    && FRONT_END="$FRONT_END perl"
   test "@ENABLE_ada@" = "ON"    && FRONT_END="$FRONT_END ada"
   test "@ENABLE_ocaml@" = "ON"  && FRONT_END="$FRONT_END ocaml"
   test "@ENABLE_lua@" = "ON"  && FRONT_END="$FRONT_END lua"
fi

# Find where the front-end scripts are by looking at the directory name of the
# current script.  

if [ "@WIN32@" = "1" ] ; then
   scripts_dir=${0%/*}
else
   scripts_dir=`echo $0 | sed 's:/[^/][^/]*$::'`
fi

# Call the front-end scripts
status=0

if [ "@WIN32@" = "1" ] ; then
 critical_examples="14 17"
 exesuffix=".exe"
else
 critical_examples="14 17 29"
 exesuffix= 
fi
export critical_examples exesuffix
for i in $FRONT_END ; do
   echo "Testing front-end $i"
   script=$scripts_dir/test_$i.sh
   if [ "@WIN32@" != "1" ] ; then
      chmod +x $script
   fi
   @SH_EXECUTABLE@ $script || status=1
done

exit $status
