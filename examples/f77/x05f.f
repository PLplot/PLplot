! $Id$
! $Log$
! Revision 1.2  1994/05/26 19:34:18  mjl
! Inserted missing CVS Id and Log fields for all Fortran demos.  Comment
! character changed to "!" everywhere, to work well with font-lock in Lucid
! emacs (requires a small change to fortran-mode.el).
!
!
      program example05
!     =================
!
! Test of drawing a histogram

      real data(2048)

! Fill up data points

      do 1 i=1,2048
        data(i)=sin(0.01*real(i))
    1 continue

      call plinit()
      call plcol(1)
      call plhist(2048,data,-1.1,1.1,44,0)
      call plcol(2)
      call pllab('#frValue','#frFrequency',
     *  '#frPLPLOT Example 5 - Probability function of Oscillator')

      call plend
      end
