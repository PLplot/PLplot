!      $Id$
!      Illustration of logarithmic axes, and redefinition of window
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

      use plplot
      implicit none
!      Process command-line arguments
      call plparseopts(PL_PARSE_FULL)

      call plinit
      call plfont(2)
!      Make log plots using two different styles.
      call plot1(0)
      call plot1(1)
      call plend
      end

      subroutine plot1(type)
      use plplot, PI => PL_PI
      implicit none
      real(kind=plflt) freql(0:100),ampl(0:100),phase(0:100), freq, f0
      integer i, type

      call pladv(0)
!      Set up data for log plot.
      f0 = 1._plflt
      do i=0,100
        freql(i)= -2.0_plflt + dble (i)/20.0_plflt
        freq=10.0_plflt**freql(i)
        ampl(i)=20.0_plflt*log10(1.0_plflt/sqrt(1.0_plflt+(freq/f0)**2))
        phase(i)=-(180.0_plflt/PI)*atan(freq/f0)
      enddo
      call plvpor(0.15_plflt, 0.85_plflt, 0.1_plflt, 0.9_plflt)
      call plwind(-2.0_plflt, 3.0_plflt, -80.0_plflt, 0.0_plflt)
      call plcol0(1)
!      Try different axis and labelling styles.
      if (type.eq.0) then
        call plbox('bclnst', 0.0_plflt, 0, 'bnstv', 0.0_plflt, 0)
      elseif (type.eq.1) then
        call plbox('bcfghlnst', 0.0_plflt, 0, 'bcghnstv', 0.0_plflt, 0)
      else
        stop 'plot1: invalid type'
      endif
!      Plot ampl vs freq.
      call plcol0(2)
      call plline(freql,ampl)
      call plcol0(1)
      call plptex(1.6_plflt, -30.0_plflt, 1.0_plflt, -20.0_plflt, 0.5_plflt, &
                  '-20 dB/decade')
!      Put labels on.
      call plcol0(1)
      call plmtex('b', 3.2_plflt, 0.5_plflt, 0.5_plflt, 'Frequency')
      call plmtex('t', 2.0_plflt, 0.5_plflt, 0.5_plflt, &
                  'Single Pole Low-Pass Filter')
      call plcol0(2)
      call plmtex('l', 5.0_plflt, 0.5_plflt, 0.5_plflt, 'Amplitude (dB)')
!      For the gridless case, put phase vs freq on same plot.
      if(type.eq.0) then
        call plcol0(1)
        call plwind(-2.0_plflt, 3.0_plflt, -100.0_plflt, 0.0_plflt)
        call plbox(' ', 0.0_plflt, 0, 'cmstv', 30.0_plflt, 3)
        call plcol0(3)
        call plline(freql,phase)
        call plcol0(3)
        call plmtex('r', 5.0_plflt, 0.5_plflt, 0.5_plflt, &
                    'Phase shift (degrees)')
      endif
      end
