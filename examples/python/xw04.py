from Numeric import *
from pl import *

# main
# 
# Illustration of logarithmic axes, and redefinition of window.

def main():

    pladv(0)
    plfont(2)

    f0 = 1000.0
    freql = 1.0 + arrayrange(101)/20.0
    freq = pow(10.0, freql)
    ampl = 20.0 * log10(1.0 / sqrt(1.0 + pow((freq / f0), 2.)))
    phase = -(180.0 / pi) * arctan(freq / f0)

    plvpor(0.15, 0.85, 0.1, 0.9)
    plwind(1.0, 6.0, -80.0, 0.0)
    plbox("bclnst", 0.0, 0, "bnstv", 0.0, 0)
    plcol0(2)
    plline(freql, ampl)
    plcol0(1)
    plptex(5.0, -30.0, 1.0, -20.0, 0.5, "-20 dB/decade")

    plwind(1.0, 6.0, -100.0, 0.0)
    plbox("", 0.0, 0, "cmstv", 30.0, 3)
    plcol0(3)
    plline(freql, phase)

    plcol0(1)
    plmtex("b", 3.2, 0.5, 0.5, "Frequency")
    plmtex("t", 2.0, 0.5, 0.5, "Single Pole Low-Pass Filter")
    plcol0(2)
    plmtex("l", 5.0, 0.5, 0.5, "Amplitude (dB)")
    plcol0(3)
    plmtex("r", 5.0, 0.5, 0.5, "Phase shift (degrees)")

    pleop()

    # restore defaults
    plfont(1)
    plcol0(1)
    
main()
