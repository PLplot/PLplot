! $Id$
! $Log$
! Revision 1.2  1994/05/26 19:34:24  mjl
! Inserted missing CVS Id and Log fields for all Fortran demos.  Comment
! character changed to "!" everywhere, to work well with font-lock in Lucid
! emacs (requires a small change to fortran-mode.el).
!
!
      program example10
!     =================
!
! Demonstration program for PLPLOT illustrating absolute positioning
!  of graphs on a page

      call plinit()

      call pladv(0)
      call plvpor(0.0,1.0,0.0,1.0)
      call plwind(0.0,1.0,0.0,1.0)
      call plbox('bc',0.0,0,'bc',0.0,0)

      call plsvpa(50.0,150.0,100.0,150.0)
      call plwind(0.0,1.0,0.0,1.0)
      call plbox('bc',0.0,0,'bc',0.0,0)
      call plptex(0.5,0.5,1.0,0.0,0.5,'BOX at (50,150,100,150)')

      call plend

      end
