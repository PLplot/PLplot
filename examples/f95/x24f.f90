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
  
  integer(kind=plint) i, j
  integer(kind=plint) red(7_plint)
  integer(kind=plint) green(7_plint)
  integer(kind=plint) blue(7_plint)
  
  real(kind=plflt)  px(4_plint)
  real(kind=plflt)  py(4_plint)
  real(kind=plflt)  sx(12_plint)
  real(kind=plflt)  sy(12_plint)
  
  character(len=30) peace(12_plint)
  
  data (red(i) ,i=1,7_plint)   / 240_plint, 204_plint, 204_plint, 204_plint,   0_plint,  39_plint, 125_plint /
  data (green(i) ,i=1,7_plint) / 240_plint,   0_plint, 125_plint, 204_plint, 204_plint,  80_plint,   0_plint /
  data (blue(i) ,i=1,7_plint)  / 240_plint,   0_plint,   0_plint,   0_plint,   0_plint, 204_plint, 125_plint /
  
  data (px(i) ,i=1,4_plint) / 0.0d0, 0.0d0, 1.0d0, 1.0d0 /
  data (py(i) ,i=1,4_plint) / 0.0d0, 0.25d0, 0.25d0, 0.0d0 /
  
  data (sx(i) ,i=1,12_plint) / &
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


  data (sy(i) ,i=1,12_plint) / &
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
  peace(1_plint) = '#<0x00>和平'

  !  Hindi
  peace(2_plint) = '#<0x20>शांति'

  !  English
  peace(3_plint) = '#<0x10>Peace'

  !  Hebrew
  peace(4_plint) = '#<0x10>שלום'

  !  Russian
  peace(5_plint) = '#<0x10>Мир'

  !  German
  peace(6_plint) = '#<0x10>Friede'

  !  Korean
  peace(7_plint) = '#<0x30>평화'

  !  French
  peace(8_plint) = '#<0x10>Paix'

  !  Spanish
  peace(9_plint) = '#<0x10>Paz'

  !  Arabic
  peace(10_plint) = '#<0x10>ﺳﻼم'

  !  Turkish
  peace(11_plint) = '#<0x10>Barış'

  !  Kurdish
  peace(12_plint) = '#<0x10>Hasîtî'

  call plparseopts(PL_PARSE_FULL)

  call plinit()

  call pladv(0_plint)
  call plvpor (0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)
  call plwind (0.0_plflt, 1.0_plflt, 0.0_plflt, 1.0_plflt)
  call plcol0 (0_plint)
  call plbox ('', 1.0_plflt, 0_plint, '', 1.0_plflt, 0_plint)

  call plscmap0n (7_plint)
  call plscmap0 (red, green, blue)

  call plschr (0.0_plflt, 4.0_plflt)
  call plfont (1_plint)

  do i = 1_plint,4

     call plcol0 (i)
     call plfill (px, py)

     do j = 1_plint,4
        py (j) = py (j) + 1.0_plflt / 4.0_plflt
     enddo
  enddo

  call plcol0 (0_plint)
  do i = 1_plint,12
     call plptex (sx (i), sy (i), 1.0_plflt, 0.0_plflt, 0.5_plflt, peace (i))
  enddo

  call plend()

end program x24f
