! $Id$
! $Log$
! Revision 1.2  1994/05/26 19:34:19  mjl
! Inserted missing CVS Id and Log fields for all Fortran demos.  Comment
! character changed to "!" everywhere, to work well with font-lock in Lucid
! emacs (requires a small change to fortran-mode.el).
!
!
      program example06
!     =================
!
! Displays the plotter symbols for PLPOIN

      character*3 text

! Full sized page for display

      call plinit()
      call pladv(0)

      call plcol(2)

! Set up viewport and window

      call plvpor(0.1,1.0,0.1,0.9)
      call plwind(0.0,1.0,0.0,1.3)

! Draw the grid using plbox

      call plbox('bcgt',0.1,0,'bcgt',0.1,0)
      call plcol(15)

! Write the digits below the frame

      do 5 i=0,9
        write (text,'(i1)') i
        call plmtex('b',1.5,0.1*i+0.05,0.5,text)
    5 continue

      k=0
      do 1 i=0,12

! Write the digits to the left of the frame

        write (text,'(i3)') 10*i
        call plmtex('lv',1.0,1.0-(2*i+1)/26.0,1.0,text)
        do 2 j=0,9
          x=0.1*j+0.05
          y=1.25-0.1*i

! Display the symbols

          if (k.lt.128) call plpoin(1,x,y,k)
          k=k+1
    2   continue
    1 continue

      call plmtex('t',1.5,0.5,0.5,'PLPLOT Example 6 - PLPOIN symbols')
      call plend
      end
