! $Id$
! $Log$
! Revision 1.2  1994/05/26 19:34:26  mjl
! Inserted missing CVS Id and Log fields for all Fortran demos.  Comment
! character changed to "!" everywhere, to work well with font-lock in Lucid
! emacs (requires a small change to fortran-mode.el).
!
!
!     Demonstration program for PLPLOT: Bar chart example. 

      character*20 string
      real y0(10)

      call plinit()

      call pladv(0)
      call plvsta
      call plwind(1980.,1990.,0.,35.)
      call plbox("bc",1.,0,"bcnv",10.,0)
      call plcol(2)
      call pllab("Year","Widget Sales (millions)",
     & "#frPLPLOT Example 12")

      y0(1) = 5
      y0(2) = 15
      y0(3) = 12
      y0(4) = 24
      y0(5) = 28
      y0(6) = 30
      y0(7) = 20
      y0(8) = 8
      y0(9) = 12
      y0(10) = 3

      do 1 i=0,9
         call plcol(mod(i,4)+1)
         call plpsty(mod(i+3,8)+1)
         call pllsty(mod(i,8)+1)
         call plfbox(1980.+i,y0(i+1))
         write(string,"(i8)")int(y0(i+1))
         call lalign(string)
         call plptex(1980.+i+.5,y0(i+1)+1.,1.,0.,.5,string)
         write(string,"(i8)")1980+i
         call lalign(string)
         call plmtex("b",1.,(i+1)*.1-.05,.5,string)
1     continue

!     Don't forget to call PLEND to finish off!
      call plend

      end

!     Suck up leading spaces
      subroutine lalign(string)
      character*(*) string
      
      max = len(string)

      do 1 i=1,max
         if(string(i:i).ne." ") then
            if(i.eq.1) return
            k = 1
            do 2 j=i,max
               string(k:k) = string(j:j)
               k = k + 1
2           continue
            do 3 j=k,max
               string(j:j) = " "
3           continue
            return
         endif
1     continue

      end

      subroutine plfbox(x0,y0)

      real x(4), y(4)

      x(1) = x0
      y(1) = 0.
      x(2) = x0
      y(2) = y0
      x(3) = x0+1.
      y(3) = y0
      x(4) = x0+1.
      y(4) = 0.
      call plfill(4,x,y)
      call plcol(1)
      call pllsty(1)
      call plline(4,x,y)

      end
