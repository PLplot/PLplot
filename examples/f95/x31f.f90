!      $Id:$
!      set / get tester.
!
!      Copyright (C) 2008  Alan W. Irwin
!      Copyright (C) 2008  Andrew Ross
!
!      This file is part of PLplot.
!
!      PLplot is free software; you can redistribute it and/or modify
!      it under the terms of the GNU General Library Public License as
!      published by the Free Software Foundation; either version 2 of the
!      License, or (at your option) any later version.
!
!      PLplot is distributed in the hope that it will be useful,
!      but WITHOUT ANY WARRANTY; without even the implied warranty of
!      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!      GNU Library General Public License for more details.
!
!      You should have received a copy of the GNU Library General Public
!      License along with PLplot; if not, write to the Free Software
!      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

!--------------------------------------------------------------------------
! main
!
! Demonstrates absolute positioning of graphs on a page.
!--------------------------------------------------------------------------

program x31f95
  use plplot
  
  implicit none
  real(kind=plflt) xmin, xmax, ymin, ymax, wx, wy
  real(kind=plflt) mar, aspect, jx, jy, ori
  integer win, level, digmax, digits, compression
  real(kind=plflt) xp, yp, xp2, yp2
  integer xleng, yleng, xoff, yoff, xleng2, yleng2
  integer fam, num, bmax, fam1, num1, bmax1, r, g, b
  real(kind=plflt) a
  integer r1(2), g1(2), b1(2)
  data r1 /0, 255/
  data g1 /255, 0/
  data b1 /0, 0/
  real(kind=plflt) a1(2)
  data a1 /1.0_plflt, 1.0_plflt/
  character*80 fnam
  
!     Parse and process command line arguments

  call plparseopts(PL_PARSE_FULL)
  
!     Test setting / getting page size
  call plgpage(xp, yp, xleng, yleng, xoff, yoff)
  xp2 = xp*0.9_plflt
  yp2 = yp*0.9_plflt
  xleng2 = int(xleng*0.9_plflt)
  yleng2 = int(yleng*0.9_plflt)
  call plspage(xp2, yp2, xleng2, yleng2, xoff, yoff)
  call plgpage(xp, yp, xleng, yleng, xoff, yoff)
  if (xp.ne.xp2 .or. yp .ne. yp2 .or. xleng .ne. xleng2 .or. &
       yleng .ne. yleng2) then
     write (0,*) "plgpage test failed"
     call plend()
     call exit(1)
  endif
  
  call plscompression(1)
  call plgcompression(compression)
  if (compression .ne. 1) then
     write (0,*) "plgcompression test failed"
     call plend()
     call exit(1)
  endif
  
  call plgfam(fam, num, bmax)
  call plsfam(1,1,100000)
  call plgfam(fam1, num1, bmax1)
  if (fam1 .ne. 1 .or. num1 .ne. 1 .or. bmax1 .ne. 100000) then
     write (0,*) "plgfam test failed"
     call plend()
     call exit(1)
  endif
  call plsfam(fam, num, bmax)
  
!     Initialize plplot

  call plinit()

  call plscolor(1)

  call plscol0(1, 255, 0, 0)

  call plscmap1(r1,g1,b1)
  call plscmap1a(r1,g1,b1,a1)
  
  call plglevel(level)
  if (level .ne. 1) then
     write(0,*) "plglevel test failed. Level is ",level, &     
          ", but 1 expected."
     call plend()
     call exit(1)
  endif
  
  call pladv(0)
  call plvpor(0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)

  call plwind(0.2_plflt, 0.3_plflt, 0.4_plflt, 0.5_plflt)
  call plgvpw(xmin, xmax, ymin, ymax)
  if (xmin .ne. 0.2_plflt .or. xmax .ne. 0.3_plflt .or. ymin .ne. 0.4_plflt &
       .or. ymax .ne. 0.5_plflt) then
     write (0,*) "plgvpw test failed",xmin,xmax,ymin,ymax
     call plend()
     call exit(1)
  endif
  
  call plgvpd(xmin, xmax, ymin, ymax)
  if (xmin .ne. 0.0_plflt .or. xmax .ne. 1.0_plflt .or. ymin .ne. 0.0_plflt &
       .or. ymax .ne. 1.0_plflt) then
     write (0,*) "plgvpd test failed"
     call plend()
     call exit(1)
  endif

!     Get world coordinates for 0.5,0.5 which is in the middle of 
!     the window
  call plcalc_world(0.5_plflt,0.5_plflt,wx,wy,win)
  if (abs(wx-0.25_plflt).gt.1.0d-5 .or. abs(wy-0.45_plflt).gt.1.0d-5) then
     write (0,*) "plcalc_world test failed"
     call plend()
     call exit(1)  
  endif

!     Retrieve and print the name of the output file (if any)
  call plgfnam(fnam)
  print *,"Output file name is ",fnam

!     Set and get the number of digits used to display axis labels
!     Note digits is currently ignored in pls[xyz]ax and 
!     therefore it does not make sense to test the returned 
!     value
  call plsxax(3,0)
  call plgxax(digmax,digits)
  if (digmax .ne. 3) then
     write (0,*) "plgxax test failed"
     call plend()
     call exit(1)
  endif

  call plsyax(3,0)
  call plgyax(digmax,digits)
  if (digmax .ne. 3) then
     write (0,*) "plgyax test failed"
     call plend()
     call exit(1)
  endif

  call plszax(3,0)
  call plgzax(digmax,digits)
  if (digmax .ne. 3) then
     write(0,*) "plgzax test failed"
     call plend()
     call exit(1)
  endif
      
  call plsdidev(0.05_plflt, dble(PL_NOTSET), 0.0_plflt, 0.0_plflt)
  call plgdidev(mar, aspect, jx, jy)
  if (mar .ne. 0.05_plflt .or. jx .ne. 0.0_plflt .or. jy .ne. 0.0_plflt) then
     write(0,*) "plgdidev test failed"
     call plend()
     call exit(1)
  endif
      
  call plsdiori(1.0_plflt)
  call plgdiori(ori)
  if (ori .ne. 1.0_plflt) then
     write(0,*) "plgdiori test failed"
     call plend()
     call exit(1)
  endif
      
  call plsdiplt(0.1_plflt, 0.1_plflt, 0.9_plflt, 0.9_plflt)
  call plgdiplt(xmin, ymin, xmax, ymax)
  if (xmin .ne. 0.1_plflt .or. xmax .ne. 0.9_plflt .or. ymin .ne. 0.1_plflt &
       .or. ymax .ne. 0.9_plflt) then
     write(0,*) "plgdiplt test failed"
     call plend()
     call exit(1)
  endif
  
  call plsdiplz(0.1_plflt, 0.1_plflt, 0.9_plflt, 0.9_plflt)
  call plgdiplt(xmin, ymin, xmax, ymax)
  if (xmin .ne. 0.1_plflt+0.8_plflt*0.1_plflt .or. &
       xmax .ne. 0.1_plflt+0.8_plflt*0.9_plflt .or. &
       ymin .ne. 0.1_plflt+0.8_plflt*0.1_plflt .or. &
       ymax .ne. 0.1_plflt+0.8_plflt*0.9_plflt) then
     write(0,*) "plsdiplz test failed"
     call plend()
     call exit(1)
  endif
  
  call plscolbg(0,0,0)
  call plgcolbg(r, g, b)
  if (r .ne. 0 .or. g .ne. 0 .or. b .ne. 0) then
     write(0,*) "plgcolbg test failed"
     call plend()
     call exit(1)
  endif
  
  call plscolbga(0,0,0,1.0_plflt)
  call plgcolbga(r, g, b, a)
  if (r .ne. 0 .or. g .ne. 0 .or. b .ne. 0 .or. a .ne. 1.0_plflt) then
     write(0,*) "plgcolbga test failed"
     call plend()
     call exit(1)
  endif
  
  call plend()
  call exit(0)
end program x31f95
