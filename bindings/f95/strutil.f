! $Id$
!
! Copyright (C) 2004  Alan W. Irwin
!
! This file is part of PLplot.
!
! PLplot is free software; you can redistribute it and/or modify
! it under the terms of the GNU General Library Public License as published
! by the Free Software Foundation; either version 2 of the License, or
! (at your option) any later version.
!
! PLplot is distributed in the hope that it will be useful,
! but WITHOUT ANY WARRANTY; without even the implied warranty of
! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
! GNU Library General Public License for more details.
!
! You should have received a copy of the GNU Library General Public License
! along with PLplot; if not, write to the Free Software
! Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

      subroutine plstrf2c(string1, string2, maxlen)

      integer*4 maxlen
      character*(*) string1, string2

      integer*4 limit, islen
      external islen

      limit = min0(islen(string1),maxlen-1)
      do 100 i = 1,limit
         string2(i:i) = string1(i:i)
 100  continue
      string2(limit+1:limit+1) = char(0)

      return
      end subroutine plstrf2c


      subroutine plstrc2f(string1, string2)

      character*(*) string1, string2

      integer*4 limit
      character*300 stringbuf

      limit = 1
 10   if (ichar(string1(limit:limit)) .eq. 0) goto 20
      stringbuf(limit:limit) = string1(limit:limit)
      limit = limit + 1
      goto 10

 20   if(limit.gt.1) then
        string2 = stringbuf(1:limit-1)
      else
        string2 = ''
      endif

      return
      end subroutine plstrc2f


      integer function islen(string)
      character*(*) string
      integer i

      do 100 i = len(string),1,-1
         if (string(i:i) .ne. ' ') then
            islen = i
            return
         endif
 100  continue

C      If string is blank or length 0, return length 0
      islen = 0
      return
      end function islen
