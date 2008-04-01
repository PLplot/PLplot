! $Id: x20c.c 8033 2007-11-23 15:28:09Z andrewross $
!
!      Copyright (C) 2004  Alan W. Irwin
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
!
!
!      plimage demo
!
!

!static PLOptionTable options[] = {
!{
!    "dbg",			/* extra debugging plot */
!    NULL,
!    NULL,
!    &dbg,
!    PL_OPT_BOOL,
!    "-dbg",
!    "Extra debugging plot" },
!{
!    "nosombrero",			/* Turns on test of xor function */
!    NULL,
!    NULL,
!    &nosombrero,
!    PL_OPT_BOOL,
!    "-nosombrero",
!    "No sombrero plot" },
!{
!    "nointeractive",			/* Turns on test of xor function */
!    NULL,
!    NULL,
!    &nointeractive,
!    PL_OPT_BOOL,
!    "-nointeractive",
!    "No interactive selection" },
!{
!    "save",			/* For saving in postscript */
!    NULL,
!    NULL,
!    &f_name,
!    PL_OPT_STRING,
!    "-save filename",
!      "Save sombrero plot in color postscript `filename'" },
!{
!    NULL,			/* option */
!    NULL,			/* handler */
!    NULL,			/* client data */
!    NULL,			/* address of variable to set */
!    0,				/* mode flag */
!    NULL,			/* short syntax */
!    NULL }			/* long syntax */
!};

      program x20f
      use plplot, M_PI => PL_PI

      implicit none

      integer, parameter          :: XDIM = 260, YDIM = 260
      real(kind=plflt), parameter :: XDIMR = XDIM, YDIMR = YDIM

      real(kind=plflt) ::  x(XDIM), y(YDIM), z(XDIM,YDIM), r(XDIM,YDIM)
      real(kind=plflt) ::  xi, yi, xe, ye
      integer i, j
      real(kind=plflt)  width_r, height_r

!
!     Dimensions taken from "lena.pgm"
!
      integer width, height, num_col
      real(kind=plflt), dimension(:,:), pointer :: img_f

!
!     Parameters from command-line
!
      logical dbg
      logical nosombrero
      logical nointeractive
      character*80 f_name

!
!     Bugs in plimage():
!      -at high magnifications, the left and right edge are ragged, try
!         ./x20c -dev xwin -wplt 0.3,0.3,0.6,0.6 -ori 0.5
!
!     Bugs in x20c.c:
!      -if the window is resized after a selection is made on 'lena', when
!       making a new selection the old one will re-appear.
!
!
!     Parse and process command line arguments
!
!      call plMergeOpts(options, 'x20c options', NULL)

      dbg            = .true.
      nosombrero     = .false.
      nointeractive  = .true.
      f_name         = ' '
      call plparseopts(PL_PARSE_FULL)

!     Initialize plplot

      call plinit

!     View image border pixels
      if (dbg) then
          call plenv(1._plflt, XDIMR, 1._plflt, YDIMR, 1, 1)

          z = 0.0_plflt

!         Build a one pixel square border, for diagnostics
          do i = 1,XDIM
!             Right
              z(i,YDIM) = 1._plflt
!             Left
              z(i,1)    = 1._plflt
          enddo

          do i = 1,YDIM
!             Top
              z(1,i)    = 1._plflt
!             Bottom
              z(XDIM,i) = 1._plflt
          enddo

          call pllab('...around a blue square.',' ', &
              'A red border should appear...')

          call plimage(z, 1._plflt, XDIMR, 1._plflt, YDIMR, 0._plflt, 0._plflt, &
              1._plflt, XDIMR, 1._plflt, YDIMR)

          call pladv(0)
      endif

!     Sombrero-like demo
      if (.not. nosombrero) then
!         draw a yellow plot box, useful for diagnostics! :(
          call plcol0(2)
          call plenv(0._plflt, 2._plflt*M_PI, 0.0_plflt, 3._plflt*M_PI, 1, -1)

          do i=1,XDIM
              x(i) = (i-1)*2._plflt*M_PI/(XDIM-1)
          enddo
          do i=1,YDIM
            y(i) = (i-1)*3._plflt*M_PI/(YDIM-1)
          enddo

          do i=1,XDIM
              do j=1,YDIM
                  r(i,j) = sqrt(x(i)*x(i)+y(j)*y(j))+1e-3
                  z(i,j) = sin(r(i,j)) / (r(i,j))
              enddo
          enddo

          call pllab('No, an amplitude clipped ''sombrero''', '', &
              'Saturn?')
          call plptex(2._plflt, 2._plflt, 3._plflt, 4._plflt, 0._plflt, 'Transparent image')
          call plimage(z, 0._plflt, 2._plflt*M_PI, 0.0_plflt, 3._plflt*M_PI, &
              0.05_plflt, 1._plflt, 0._plflt, 2._plflt*M_PI, 0._plflt, 3._plflt*M_PI)

!         Save the plot
          if (f_name .ne. ' ') then
              call save_plot(f_name)
          endif

          call pladv(0)
      endif

!
!     Read Lena image
!     Note we try two different locations to cover the case where this
!     examples is being run from the test_c.sh script
!
      if (.not. read_img('lena.pgm', img_f, width, height, num_col)) then
          if (.not. read_img('../lena.pgm', img_f, width, height, num_col)) then
!C            call plabort('No such file')
              write(*,*) 'Image could not be read'
              call plend()
              stop
          endif
      endif

!     Set gray colormap
      call gray_cmap(num_col)

!     Display Lena
      width_r  = width
      height_r = height
      call plenv(1._plflt, width_r, 1._plflt, height_r, 1, -1)

      if (.not. nointeractive) then
          call pllab('Set and drag Button 1 to (re)set selection, Butto&
     &n 2 to finish.',' ','Lena...')
      else
          call pllab('',' ','Lena...')
      endif

      call plimage(img_f, 1._plflt, width_r, 1._plflt, &
          height_r, 0._plflt, 0._plflt, 1._plflt, width_r, 1._plflt, height_r)

!     Selection/expansion demo
      if (.not. nointeractive) then
          xi = 200.0_plflt
          xe = 330.0_plflt
          yi = 280.0_plflt
          ye = 220.0_plflt

          if (get_clip(xi, xe, yi, ye)) then
              call plend()
              call exit(0)
          endif

!
!         I'm unable to continue, clearing the plot and advancing to the next
!         one, without hiting the enter key, or pressing the button... help!
!         Forcing the xwin driver to leave locate mode and destroying the
!         xhairs (in GetCursorCmd()) solves some problems, but I still have
!         to press the enter key or press Button-2 to go to next plot, even
!         if a pladv() is not present!  Using plbop() solves the problem, but
!         it shouldn't be needed!
!
!         plspause(0), pladv(0), plspause(1), also works,
!         but the above question remains.
!         With this approach, the previous pause state is lost,
!         as there is no API call to get its current state.
!

          call plspause(.false.)
          call pladv(0)

!         Display selection only
          call plimage(img_f, 1._plflt, width_r, 1._plflt, &
              height_r, 0._plflt, 0._plflt, xi, xe, ye, yi)

          call plspause(.true.)
          call pladv(0)

!         Zoom in selection
          call plenv(xi, xe, ye, yi, 1, -1)
          call plimage(img_f, 1._plflt, width_r, 1._plflt, &
              height_r, 0._plflt, 0._plflt, xi, xe, ye, yi)
          call pladv(0)
      endif

      call plend()
      call exit(0)

      contains

!     Read image from file in binary ppm format
      logical function read_img(fname, img_f, width, height, num_col)

      character(*), intent(in)                  :: fname
      integer, intent(out)                      :: width, height
      real(kind=plflt), dimension(:,:), pointer :: img_f
      integer      num_col

      character    img
      character*80 ver
      integer      i, j, k, w, h

      integer      ierr
      integer      count
      integer      record

!     Naive grayscale binary ppm reading. If you know how to, improve it

      open( 10, file = fname, status = 'old', iostat = ierr )

      if (ierr .ne. 0 ) then
          read_img = .false.
          return
      endif

!
!     Read the first lines (count them for later re-reading)
!
      count = 1
      read( 10, '(a)', iostat = ierr ) ver

!     I only understand "P5"!
      if (ver .ne. 'P5' .or. ierr .ne. 0) then
          read_img = .false.
          return
      endif

      do
          count = count + 1
          read( 10, '(a)', iostat = ierr ) ver

          if (ierr .ne. 0) then
              read_img = .false.
              write(*,*) 'Error!'
              return
          endif

          if (ver(1:1) .ne. '#' ) then
              exit
          endif
      enddo

!     Found the line with the sizes, copy this one and the next

      open( 11, status = 'scratch' )
      write( 11, '(a)' ) ver

      count = count + 1
      read( 10, '(a)', iostat = ierr ) ver
      write( 11, '(a)' ) ver

      rewind( 11 )
      read( 11, * ) w, h, num_col

      allocate( img_f(w,h) )

      close( 10 )
      close( 11 )

!
!     Read the second part - we need to do it the hard way :(
!
!     Note:
!     The algorithm only works if the unit of record length is a byte!
!     (Some compilers _use_ a word (4 bytes) instead, but often provide
!     a compile switch to _use_ bytes)
!
!     NOTE: _use_ is used instead of the ordinary word because of a
!     bug in CMake
!
      open( 10, file = fname, access = 'direct', recl = 1 )

      record = 0
      do while ( count > 0 )
!
!     Look for the end of the line with sizes
!
          record = record + 1
          read( 10, rec = record, iostat = ierr ) img

          if ( img .eq. char(10) ) count = count - 1
          if ( ierr .ne. 0 ) exit
      enddo

!
!     We have found the picture bytes!
!     The picture needs to be flipped vertically.
!     So do that rightaway
!
!
      do j = 1,h
          do i = 1,w
              record = record + 1
              read( 10, rec = record ) img
              img_f(i,h-j+1) = dble(ichar(img))
          enddo
      enddo

      width  = w
      height = h
      read_img = .true.

      end function

!     Save plot
      subroutine save_plot(fname)

      character*(*) fname

      integer cur_strm, new_strm

!     Get current stream
      call plgstrm(cur_strm)

!     Create a new one
      call plmkstrm(new_strm)

!     New device type. _Use_ a known existing driver
      call plsdev('psc')
      call plsfnam(fname)

!     Copy old stream parameters to new stream
      call plcpstrm(cur_strm, .false.)
      call plreplot()
      call plend1()

!     Return to previous one
      call plsstrm(cur_strm)

      end subroutine

!     Get selection square interactively
      logical function get_clip(xi, xe, yi, ye)

      real(kind=plflt) xi, xe, yi, ye

!      PLGraphicsIn gin
      integer gin
      real(kind=plflt) xxi, yyi, xxe, yye, t
      logical st, start
      real(kind=plflt) sx(5), sy(5)

      xxi = xi
      yyi = yi
      xxe = xe
      yye = ye
      start = .false.

!     Enter xor mode to draw a selection rectangle
      call plxormod(.true., st)

!     Driver has xormod capability, continue
      if (st) then
  100     continue

          call plxormod(.false., st)
!C        call plGetCursor(gin)
          call plxormod(.true., st)

!C         if (gin.button == 1) {
!C             xxi = gin.wX
!C             yyi = gin.wY
              if (start) then
!C                clear previous rectangle
                  call plline(sx, sy)
              endif

              start = .false.

              sx(1) = xxi
              sy(1) = yyi
              sx(5) = xxi
              sy(5) = yyi
!C         endif

!C         if (gin.state & 0x100) then
!C             xxe = gin.wX
!C             yye = gin.wY
              if (start) then
!                 Clear previous rectangle
                  call plline(sx, sy)
              endif
              start = .true.

              sx(3) = xxe
              sy(3) = yye
              sx(2) = xxe
              sy(2) = yyi
              sx(4) = xxi
              sy(4) = yye
!             Draw new rectangle
              call plline(sx, sy)
!C         endif

!C         if (gin.button == 3 || gin.keysym == PLK_Return || gin.keysym == 'Q') then
              if (start) then
!                 Clear previous rectangle
                  call plline(sx, sy)
                  goto 110
              endif
!C         endif

  110     continue
!         Leave xor mode
          call plxormod(.false., st)

          if (xxe .lt. xxi) then
              t   = xxi
              xxi = xxe
              xxe = t
          endif

          if (yyi .lt. yye) then
              t   = yyi
              yyi = yye
              yye = t
          endif

          xe = xxe
          xi = xxi
          ye = yye
          yi = yyi

!C         get_clip = gin.keysym == 'Q'
      else
!         driver has no xormod capability, just do nothing
          get_clip = .false.
          return
      endif

      end function

!     Set gray colormap
      subroutine gray_cmap(num_col)

      integer num_col
      real(kind=plflt) r(2), g(2), b(2), pos(2)
      logical rev(2)

      r(1) = 0.0
      g(1) = 0.0
      b(1) = 0.0
      r(2) = 1.0
      g(2) = 1.0
      b(2) = 1.0

      pos(1) = 0.0
      pos(2) = 1.0
      rev(1) = .false.
      rev(2) = .false.

      call plscmap1n(num_col)
      call plscmap1l(.true., pos, r, g, b, rev)

      end subroutine

      end program
