!      Copyright (C) 2004-2014 Alan W. Irwin
!
!      This file is part of PLplot.
!
!      PLplot is free software; you can redistribute it and/or modify
!      it under the terms of the GNU Library General Public License as
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

      subroutine plparseopts(mode)
      use plplot
      implicit none
      integer(kind=plint) :: mode
      integer(kind=plint) :: maxargs, iargs, numargs, index, maxindex
      parameter(maxindex = maxlen/4_plint)
      parameter (maxargs=20_plint)
      character (len=maxlen) :: arg
      integer(kind=plint), dimension(maxindex, maxargs) :: iargsarr
      numargs = command_argument_count()
      if(numargs.lt.0_plint) then
!       This actually happened historically on a badly linked Cygwin platform.
        write(0,'(a)') 'plparseopts: negative number of arguments'
        return
      endif
      if(numargs+1_plint.gt.maxargs) then
        write(0,'(a)') 'plparseopts: too many arguments'
        return
      endif
      do 10 iargs = 0_plint, numargs
        call get_command_argument(iargs, arg)
        call plstrf2c(trim(arg), string1)
        s1 = transfer( string1, s1 )
        do 5 index = 1_plint, maxindex
          iargsarr(index, iargs+1) = s1(index)
    5     continue
   10   continue
      call plparseopts7(numargs+1, iargsarr, mode, maxindex*4)
      end subroutine plparseopts
