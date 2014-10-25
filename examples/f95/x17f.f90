!   Copyright (C) 2004  Alan W. Irwin
!
!   This file is part of PLplot.
!
!   PLplot is free software; you can redistribute it and/or modify
!   it under the terms of the GNU Library General Public License as
!   published by the Free Software Foundation; either version 2 of the
!   License, or (at your option) any later version.
!
!   PLplot is distributed in the hope that it will be useful,
!   but WITHOUT ANY WARRANTY; without even the implied warranty of
!   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!   GNU Library General Public License for more details.
!
!   You should have received a copy of the GNU Library General Public
!   License along with PLplot; if not, write to the Free Software
!   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA


!--------------------------------------------------------------------------
!      main program
!--------------------------------------------------------------------------

program x17f95
    use plplot, PI => PL_PI
    implicit none

    integer            :: id1, n
    integer, parameter :: nsteps = 1000
    logical            :: autoy, acc
    real(kind=plflt)   :: y1, y2, y3, y4, ymin, ymax, xlab, ylab
    real(kind=plflt)   :: t, tmin, tmax, tjump, dt, noise
    integer            :: colbox, collab, colline(4), styline(4)
    character(len=20)  :: legline(4)

    logical            :: pl_errcode
    character(len=80)  :: errmsg

!   Process command-line arguments
    call plparseopts(PL_PARSE_FULL)

!   plplot initialization

!   If db is used the plot is much more smooth. However, because of the
!   async X behaviour, one does not have a real-time scripcharter.
!
!   call plsetopt('db', '')
!   call plsetopt('np', '')

!   User sets up plot completely except for window and data
!   Eventually settings in place when strip chart is created will be
!   remembered so that multiple strip charts can be used simultaneously.


!   Specify some reasonable defaults for ymin and ymax
!   The plot will grow automatically if needed (but not shrink)

    ymin = -0.1_plflt
    ymax = 0.1_plflt

!    Specify initial tmin and tmax -- this determines length of window.
!    Also specify maximum jump in t
!    This can accomodate adaptive timesteps

    tmin = 0._plflt
    tmax = 10._plflt
!   percentage of plot to jump
    tjump = 0.3_plflt

!   Axes options same as plbox.
!   Only automatic tick generation and label placement allowed
!   Eventually I'll make this fancier

    colbox = 1
    collab = 3
!   pens color and line style
    styline = (/ 2, 3, 4, 5 /)
    colline = (/ 2, 3, 4, 5 /)

!   pens legend
    legline = (/ 'sum    ', 'sin    ', 'sin*noi', 'sin+noi' /)

!   legend position
    xlab = 0._plflt
    ylab = 0.25_plflt

!   autoscale y
    autoy = .true.
!   don't scrip, accumulate
    acc = .true.

!   Initialize plplot

    call plinit()

    call pladv(0)
    call plvsta()

!   Register our error variables with PLplot
!   From here on, we're handling all errors here

!   TODO: call plsError(&pl_errcode, errmsg)

    call plstripc(id1, 'bcnst', 'bcnstv', &
        tmin, tmax, tjump, ymin, ymax, &
        xlab, ylab, &
        autoy, acc, &
        colbox, collab, &
        colline, styline, legline, &
        't', '', 'Strip chart demo')

    pl_errcode = .false.
    if ( pl_errcode ) then
        write(*,*) errmsg
        stop
    endif

!   Let plplot handle errors from here on

!   TODO: call plsError(NULL, NULL)

!   autoscale y
    autoy = .false.
!   accumulate
    acc = .true.

!   This is to represent a loop over time
!   Let's try a random walk process

    y1 = 0.0_plflt
    y2 = 0.0_plflt
    y3 = 0.0_plflt
    y4 = 0.0_plflt
    dt = 0.1_plflt

    do n = 0,nsteps-1
!       wait a little (10 ms) to simulate time elapsing.  gfortran
!       sleep has resolution of 1 sec so the call below is commented
!       out because it is like watching paint dry.  In any case, I
!       like the resulting speedier output when you drop sleep
!       altogether from this loop.
!       call sleep(1)

        t = dble(n) * dt
        noise = plrandd() - 0.5_plflt
        y1 = y1 + noise
        y2 = sin(t*PI/18._plflt)
        y3 = y2 * noise
        y4 = y2 + noise/3._plflt

!       There is no need for all pens to have the same number of
!       points or beeing equally time spaced.

        if ( mod(n,2) .ne. 0 ) then
            call plstripa(id1, 0, t, y1)
        endif
        if ( mod(n,3) .ne. 0 ) then
            call plstripa(id1, 1, t, y2)
        endif
        if ( mod(n,4) .ne. 0 ) then
            call plstripa(id1, 2, t, y3)
        endif
        if ( mod(n,5) .ne. 0 ) then
            call plstripa(id1, 3, t, y4)
        endif
!       use_ double buffer (-db on command line)
!       call pleop()
    enddo

!   Destroy strip chart and it's memory

    call plstripd(id1)
    call plend()
end program x17f95
