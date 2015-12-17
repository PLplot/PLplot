!      Copyright (C) 2004-2016 Alan W. Irwin
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
      include 'plplot_interface_private_types.inc'
      integer                :: mode
      integer                :: iargs, numargs
      integer, parameter     :: maxargs = 100
      character (len=maxlen), dimension(0:maxargs) :: arg

      interface
           subroutine interface_plparseopts( length, nargs, arg, mode ) bind(c,name='fc_plparseopts')
              use iso_c_binding, only: c_int, c_char
              include 'plplot_interface_private_types.inc'
              integer(kind=private_plint), value :: length, nargs, mode
              ! This Fortran argument requires special processing done
              ! in fc_plparseopts at the C level to interoperate properly
              ! with the C version of plparseopts.
              character(c_char) arg(length, nargs)
          end subroutine interface_plparseopts
      end interface

      numargs = command_argument_count()
      if (numargs < 0) then
!       This actually happened historically on a badly linked Cygwin platform.
        write(0,'(a)') 'plparseopts: negative number of arguments'
        return
      endif
      if(numargs > maxargs) then
        write(0,'(a)') 'plparseopts: too many arguments'
        return
      endif
      do iargs = 0, numargs
        call get_command_argument(iargs, arg(iargs))
      enddo
      call interface_plparseopts(len(arg(0), kind=private_plint), int(numargs+1, kind=private_plint), arg, &
           int(mode, kind=private_plint))
      end subroutine plparseopts
