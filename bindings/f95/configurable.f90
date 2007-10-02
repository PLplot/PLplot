!      $Id$
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

      subroutine plparseopts(mode)
      implicit none
      include 'sfstubs.h'
      integer mode
      integer maxargs, iargs, numargs, index, maxindex, iargc, islen
      parameter(maxindex = maxlen/4)
      parameter (maxargs=20)
      character*(maxlen) arg
      integer*4 iargsarr(maxindex, maxargs)
!       write(0,'(a)') 'plparseopts not implemented on this fortran'//
!      & ' platform because iargc or getarg are not available'
      numargs = iargc()
      if(numargs.lt.0) then
!       This actually happened on badly linked Cygwin platform.
        write(0,'(a)') 'plparseopts: negative number of arguments'
        return
      endif
      if(numargs+1.gt.maxargs) then
        write(0,'(a)') 'plparseopts: too many arguments'
        return
      endif
      do 10 iargs = 0, numargs
        call getarg(iargs, arg)
        call plstrf2c(arg(:islen(arg)), string1, maxlen)
        s1 = transfer( string1, s1 )
        do 5 index = 1, maxindex
          iargsarr(index, iargs+1) = s1(index)
    5     continue
   10   continue
      call plparseopts7(numargs+1, iargsarr, mode, maxindex*4)
      end subroutine plparseopts
