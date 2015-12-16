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
      use iso_c_binding
      implicit none
      include 'plplot_interface_private_types.inc'
      integer                :: mode
      integer                :: iargs, numargs, idx
      integer, parameter     :: maxargs = 20
      character (len=maxlen), dimension(0:maxargs) :: arg

      interface
          subroutine f_plparseopts( nargs, arg, mode, length ) bind(c,name='f_plparseopts')
              implicit none
              include 'plplot_interface_private_types.inc'
              integer(kind=private_plint), value :: nargs, mode, length
              character(len=1), dimension(*)     :: arg
          end subroutine f_plparseopts
      end interface

      numargs = command_argument_count()
      if (numargs < 0) then
!       This actually happened historically on a badly linked Cygwin platform.
        write(0,'(a)') 'plparseopts: negative number of arguments'
        return
      endif
      if(numargs+1 > maxargs) then
        write(0,'(a)') 'plparseopts: too many arguments'
        return
      endif
      do iargs = 0, numargs
        call get_command_argument(iargs, arg(iargs))
        idx = 1 + len_trim(arg(iargs))
        if (idx >= maxlen) then
            idx = idx -1
        endif
        arg(iargs)(idx:idx) = c_null_char
      enddo
      call f_plparseopts(int(numargs+1,kind=private_plint), arg, int(mode,kind=private_plint), len(arg(1),kind=private_plint))
      end subroutine plparseopts
