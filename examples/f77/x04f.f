! $Id$
! $Log$
! Revision 1.2  1994/05/26 19:34:17  mjl
! Inserted missing CVS Id and Log fields for all Fortran demos.  Comment
! character changed to "!" everywhere, to work well with font-lock in Lucid
! emacs (requires a small change to fortran-mode.el).
!
!
      program example04
!     =================
!
! Illustration of logarithmic axes, and redefinition of window

      real freql(0:100),ampl(0:100),phase(0:100)
      call plinit()
      call pladv(0)
      call plfont(2)

      f0 = 1000.0
      do 1 i=0,100
        freql(i)=1.0+real(i)/20.0
        freq=10.0**freql(i)
        ampl(i)=20.0*log10(1.0/sqrt(1.0+(freq/f0)**2))
        phase(i)=-(180.0/3.141592654)*atan(freq/f0)
    1 continue

      call plvpor(0.15,0.85,0.1,0.9)
      call plwind(1.0,6.0,-80.0,0.0)
      call plcol(1)
      call plbox('bclnst',0.0,0,'bnstv',0.0,0)
      call plcol(2)
      call plline(101,freql,ampl)
      call plcol(1)
      call plptex(5.0,-30.0,1.0,-20.0,0.5,'-20 dB/decade')
      call plwind(1.0,6.0,-100.0,0.0)
      call plbox(' ',0.0,0,'cmstv',30.0,3)
      call plcol(3)
      call plline(101,freql,phase)

      call plcol(1)
      call plmtex('b',3.2,0.5,0.5,'Frequency')
      call plmtex('t',2.0,0.5,0.5,'Single Pole Low-Pass Filter')
      call plcol(2)
      call plmtex('l',5.0,0.5,0.5,'Amplitude (dB)')
      call plcol(3)
      call plmtex('r',5.0,0.5,0.5,'Phase shift (degrees)')

      call plend      
      end
