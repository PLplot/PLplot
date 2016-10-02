!   Unicode Pace Flag
!
!   Copyright (C) 2005 Rafael Laboissiere
!   Copyright (2) 2008 Andrew Ross
!
!   This file is part of PLplot.
!
!   PLplot is free software; you can redistribute it and/or modify
!   it under the terms of the GNU Library General Public License as published
!   by the Free Software Foundation; either version 2 of the License, or
!   (at your option) any later version.
!
!   PLplot is distributed in the hope that it will be useful,
!   but WITHOUT ANY WARRANTY; without even the implied warranty of
!   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!   GNU Library General Public License for more details.
!
!   You should have received a copy of the GNU Library General Public License
!   along with PLplot; if not, write to the Free Software
!   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
!
!
!   In Debian, run like this:
!
!   ( TTFDIR=/usr/share/fonts/truetype ; \
!     PLPLOT_FREETYPE_SANS_FONT=$TTFDIR/arphic/bkai00mp.ttf \
!     PLPLOT_FREETYPE_SERIF_FONT=$TTFDIR/freefont/FreeSerif.ttf \
!     PLPLOT_FREETYPE_MONO_FONT=$TTFDIR/ttf-devanagari-fonts/lohit_hi.ttf \
!     PLPLOT_FREETYPE_SCRIPT_FONT=$TTFDIR/unfonts/UnBatang.ttf \
!     PLPLOT_FREETYPE_SYMBOL_FONT=$TTFDIR/ttf-bengali-fonts/JamrulNormal.ttf \
!     ./x24f -dev png -o x24f95.png )
!
!   Packages needed:
!
!   ttf-arphic-bkai00mp
!   ttf-freefont
!   ttf-devanagari-fonts
!   ttf-unfonts
!   ttf-bengali-fonts
!
!   For the latest Ubuntu systems lohit_hi.ttf has been moved to the
!   ttf-indic-fonts-core package instead of ttf-devanagari-fonts so you
!   will have to use this package instead and update the font path.


program x24f
  use plplot
  implicit none

  integer i, j
  integer red(7)
  integer green(7)
  integer blue(7)

  real(kind=plflt)  px(4)
  real(kind=plflt)  py(4)
  real(kind=plflt)  sx(12)
  real(kind=plflt)  sy(12)

  character(len=30) peace(12)

  data (red(i) ,i=1,7)   / 240, 204, 204, 204,   0,  39, 125 /
  data (green(i) ,i=1,7) / 240,   0, 125, 204, 204,  80,   0 /
  data (blue(i) ,i=1,7)  / 240,   0,   0,   0,   0, 204, 125 /

  data (px(i) ,i=1,4) / 0.0d0, 0.0d0, 1.0d0, 1.0d0 /
  data (py(i) ,i=1,4) / 0.0d0, 0.25d0, 0.25d0, 0.0d0 /

  data (sx(i) ,i=1,12) / &
       0.16374, &
       0.15844, &
       0.15255, &
       0.17332, &
       0.50436, &
       0.51721, &
       0.49520, &
       0.48713, &
       0.83976, &
       0.81688, &
       0.82231, &
       0.82647/


  data (sy(i) ,i=1,12) / &
       0.125, &
       0.375, &
       0.625, &
       0.875, &
       0.125, &
       0.375, &
       0.625, &
       0.875, &
       0.125, &
       0.375, &
       0.625, &
       0.875/


  !  Taken from http://www.columbia.edu/~fdc/pace/

  !  Mandarin
  peace(1) = '#<0x00>和平'

  !  Hindi
  peace(2) = '#<0x20>शांति'

  !  English
  peace(3) = '#<0x10>Peace'

  !  Hebrew
  peace(4) = '#<0x10>שלום'

  !  Russian
  peace(5) = '#<0x10>Мир'

  !  German
  peace(6) = '#<0x10>Friede'

  !  Korean
  peace(7) = '#<0x30>평화'

  !  French
  peace(8) = '#<0x10>Paix'

  !  Spanish
  peace(9) = '#<0x10>Paz'

  !  Arabic
  peace(10) = '#<0x10>ﺳﻼم'

  !  Turkish
  peace(11) = '#<0x10>Barış'

  !  Kurdish
  peace(12) = '#<0x10>Hasîtî'

  call plparseopts(PL_PARSE_FULL)

  call plinit()

  call pladv(0)
  call plvpor (0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)
  call plwind (0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)
  call plcol0 (0)
  call plbox ('', 1.0_plflt, 0, '', 1.0_plflt, 0)

  call plscmap0n (7)
  call plscmap0 (red, green, blue)

  call plschr (0.0_plflt, 4.0_plflt)
  call plfont (1)

  do i = 1,4

     call plcol0 (i)
     call plfill (px, py)

     do j = 1,4
        py (j) = py (j) + 1.0_plflt / 4.0_plflt
     enddo
  enddo

  call plcol0 (0)
  do i = 1,12
     call plptex (sx (i), sy (i), 1.0_plflt, 0.0_plflt, 0.5_plflt, peace (i))
  enddo

  call plend()

end program x24f
