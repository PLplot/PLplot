#----------------------------------------------------------------------------
# about w
#
# Create a top-level window that displays info for About.. menu item.
# Maurice LeBrun, IFS
# Adapted from mkStyles.tcl from widget demo.
#
# Arguments:
#    w -	Name to use for new top-level window.
#
# Note:
# There is a fundamental problem with the text widget, in that you can't 
# get both decent word wrap and nice scrolling.  When proportional fonts are
# used (which are nicest for reading), the spacing comes out much different
# from that in the text file, leading to a very ragged right edge.  One way
# to try to fix this is to eliminate all <CR>'s from paragraphs and turn
# on word wrap with the "-wrap word" configuration option.  But this sucks
# because then scrolling becomes jerky, as a single line scroll moves the
# text by a whole paragraph!  (I tried it, it feels terrible.)  So, the only
# remaining thing to do is to format it by hand.  Ugh.
#----------------------------------------------------------------------------

proc about {{w .about}} {
    catch {destroy $w}
    toplevel $w
    dpos $w
    wm title $w "About PLPLOT"
    wm iconname $w "About"
    normal_text_setup $w 56

    insertWithTags $w.t {\
      This is the TK output driver for PLPLOT, a scientific plotting package.
PLPLOT is relatively small, portable, freely distributable, and is rich
enough to satisfy most users.  It has a wide range of plot types including
line (linear, log), contour, 3D, fill, and almost 1000 characters (including
Greek and mathematical) in its extended font set.  The package is 
designed to make it easy to quickly get graphical output; only a handful of
function calls is typically required.  For more advanced use, virtually all
aspects of plotting are configurable.

At present, PLPLOT is known to work on the following systems:

	Unix:	A/IX 		(IBM)
		DG/UX		(Data General)
		HP-UX		(Hewlett-Packard)
		IRIX		(Silicon Graphics)
		Linux		(various)
		OSF/1		(DEC)
		SunOS		(Sun)
		Super-UX	(NEC)
		SysV		(various)
		Ultrix		(DEC)
		Unicos		(Cray)

	Amiga/Exec
	MS-DOS
	OS/2
	NeXT

      Many different output device drivers are available, including postscript,
tektronix, HP laserjet, HP pen plotters, XFig, xterm, X-windows, and a
portable metafile format, among others.  The TK driver under Unix/X 
provides a full user interface, with facilities to zoom, pan, orient,
determine placement on the output page, and save the plot or any set of
plots to a specified file and device type.  The metafile renderer (} normal
    insertWithTags $w.t {plrender} bold
    insertWithTags $w.t {)
supports seeking backwards or forwards to any specified page when
interactively viewing metafile output.

      Tcl stands for ``tool command language'' and is pronounced ``tickle.''  
TK is an X11 toolkit that provides the Motif look and feel and is 
implemented using the Tcl command language.  All of the fancy graphical
elements (buttons, menus, scrollbars, etc) in the user interface are
implemented via TK with Tcl the glue that holds everything together.  
The combination of Tcl and TK is very powerful, providing easy access at
a high level to the workings of the graphical interface, which in turn
enables rapid prototyping and an unparalleled degree of flexibility.  The
PLPLOT TK driver is built with these principles in mind, and provides 
ways for the user to customize the interface (add buttons, menu entries,
callbacks to user code, etc).

      The PLPLOT package is freely distributable, but NOT in the public 
domain. The PLPLOT source code, except header files and those files
explicitly granting permission, may not be used in a commercial software
package without consent of the authors.  You are allowed and encouraged 
to include the PLPLOT object library and header files in a commercial 
package provided that: (1) it is explicitly and prominently stated that the
PLPLOT library is freely available, and (2) the full copyrights on the 
PLPLOT package be displayed somewhere in the documentation for the 
package.

      Suggestions on how to improve this code are welcome, especially in the 
form of user-contributed enhancements or bug fixes.  If PLPLOT is used 
in any published papers, please include an acknowledgment or citation as 
this will help us to continue improving PLPLOT.  Also, please remember 
that as PLPLOT is not a commercial product, the authors cannot be 
expected to offer the kind of support that a commercial product may.  
There is great interest in extending PLPLOT and fixing bugs, but we can 
only afford to work on it part time.  Improvements will necessarily focus 
on those which help us get our work done.

      PLPLOT is written in C, enabling it to run on many platforms 
practically without modification.  Fortran programs may use it 
transparently; stub routines are provided to handle the C <-> Fortran
interface without any modification of the user program (although some of 
the more advanced features are only available from C).  C programs are
required to include the header file ``plplot.h''; see the documentation for
more details.

      The main documentation for PLPLOT is in the doc/ directory of the 
source distribution in the form of several TeX files; please consult this for
additional information (``latex plotdoc'').  Unfortunately documentation
tends to lag actual improvements to the code, so don't be surprised if some
known features are not explained there.  Consult ``Changes.log'' to see a
list of recent changes. For more information on how to get or use PLPLOT 
on your system, see (1) the appendices to the PLPLOT manual, and (2) the
system-specific documentation in the appropriate sys/<system> directory 
of the source distribution.

      To become a subscriber to the PLPLOT mailing list, send a request to
plplot-request@dino.ph.utexas.edu.  The latest plplot source distribution 
and document in postscript form can be obtained via anonymous ftp from
hagar.ph.utexas.edu, in the pub/plplot directory.

Maurice LeBrun
Institute for Fusion Studies
University of Texas
mjl@dino.ph.utexas.edu 
} normal
    $w.t configure -state disabled
    $w.t mark set insert 0.0
}
