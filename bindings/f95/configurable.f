C      $Id$
C
C      Copyright (C) 2004  Alan W. Irwin
C
C      This file is part of PLplot.
C
C      PLplot is free software; you can redistribute it and/or modify
C      it under the terms of the GNU General Library Public License as
C      published by the Free Software Foundation; either version 2 of the
C      License, or (at your option) any later version.
C
C      PLplot is distributed in the hope that it will be useful,
C      but WITHOUT ANY WARRANTY; without even the implied warranty of
C      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
C      GNU Library General Public License for more details.
C
C      You should have received a copy of the GNU Library General Public
C      License along with PLplot; if not, write to the Free Software
C      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

      subroutine plparseopts(mode)
      implicit none
      include 'sfstubs.h'
      integer mode
      integer maxargs, iargs, numargs, index, maxindex, iargc
      parameter(maxindex = maxlen/4)
      parameter (maxargs=20)
      character*(maxlen) arg
      integer*4 iargsarr(maxindex, maxargs)
C       write(0,'(a)') 'plparseopts not implemented on this fortran'//
C      & ' platform because iargc or getarg are not available'
      numargs = iargc()
      if(numargs.lt.0) then
C       This actually happened on badly linked Cygwin platform.
        write(0,'(a)') 'plparseopts: negative number of arguments'
        return
      endif
      if(numargs+1.gt.maxargs) then
        write(0,'(a)') 'plparseopts: too many arguments'
        return
      endif
      do 10 iargs = 0, numargs
        call getarg(iargs, arg)
        call plstrf2c(arg, string1, maxlen)
        do 5 index = 1, maxindex
          iargsarr(index, iargs+1) = s1(index)
    5     continue
   10   continue
      call plparseopts7(numargs+1, iargsarr, mode, maxindex*4)
      end subroutine plparseopts
