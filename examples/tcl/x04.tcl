# $Id$
# $Log$
# Revision 1.2  1994/08/09 08:23:24  mjl
# Changed to new tclMatrix notation.
#
# Revision 1.1  1994/06/30  18:49:36  mjl
# Tcl demo programs, which fully reproduce their x??c counterpart.
#
#----------------------------------------------------------------------------
# PLplot Tcl demo #4
#
# After sourcing, just type "4".
#----------------------------------------------------------------------------

proc 4 {} {
    matrix freql f 101
    matrix ampl f 101
    matrix phase f 101

    plssub 1 1
    plbop
    plfont 2

    set f0 1000.0
    for {set i 1} {$i <= 100} {incr i} {
	freql $i = [expr 1.0 + $i / 20.0]
	set freq [expr pow(10.0, [freql $i])]
	ampl $i = [expr 20.0 * log10(1.0 / sqrt(1.0 + pow(($freq/$f0), 2)))]
	phase $i = [expr -(180.0 / 3.141592654) * atan($freq/$f0)]
    }

    plvpor 0.15 0.85 0.1 0.9
    plwind 1.0 6.0 -80.0 0.0

    plcol 1
    plbox "bclnst" 0.0 0 "bnstv" 0.0 0

    plcol 2
    plline 101 freql ampl

    plcol 1
    plptex 5.0 -30.0 1.0 -20.0 0.5 "-20 dB/decade"
    plwind 1.0 6.0 -100.0 0.0
    plbox "" 0.0 0 "cmstv" 30.0 3

    plcol 3
    plline 101 freql phase

    plcol 1
    plmtex "b" 3.2 0.5 0.5 "Frequency"
    plmtex "t" 2.0 0.5 0.5 "Single Pole Low-Pass Filter"

    plcol 2
    plmtex "l" 5.0 0.5 0.5 "Amplitude (dB)"

    plcol 3
    plmtex "r" 5.0 0.5 0.5 "Phase shift (degrees)"

    pleop
}
