!***********************************************************************
!  included_plplot_real_interfaces.f90
!
!  Copyright (C) 2005-2016  Arjen Markus
!  Copyright (C) 2006-2016 Alan W. Irwin
!
!  This file is part of PLplot.
!
!  PLplot is free software; you can redistribute it and/or modify
!  it under the terms of the GNU Library General Public License as published
!  by the Free Software Foundation; either version 2 of the License, or
!  (at your option) any later version.
!
!  PLplot is distributed in the hope that it will be useful,
!  but WITHOUT ANY WARRANTY; without even the implied warranty of
!  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
!  GNU Library General Public License for more details.
!
!  You should have received a copy of the GNU Library General Public License
!  along with PLplot; if not, write to the Free Software
!  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
!
!
!  This file defines the Fortran interfaces for the the subset of the
!  PLplot API which has at least one real argument.  Keeping this part
!  of the interface definitions separate allows convenient defining of
!  these interfaces for both single and double precision real
!  arguments.  See plplot_bindings.f90 for the definition of the
!  Fortran interfaces for the remaining part of the PLplot API.
!
!***********************************************************************

    private :: matrix_to_c

    interface pl_setcontlabelparam
        module procedure pl_setcontlabelparam_impl
    end interface pl_setcontlabelparam
    private :: pl_setcontlabelparam_impl

    interface plarc
        module procedure plarc_log_impl
        module procedure plarc_impl
    end interface plarc
    private :: plarc_impl

    interface plbin
        module procedure plbin_impl
    end interface plbin
    private :: plbin_impl

    interface plbox
        module procedure plbox_impl
    end interface plbox
    private :: plbox_impl

    interface plbox3
        module procedure plbox3_impl
    end interface plbox3
    private :: plbox3_impl

    interface plcalc_world
        module procedure plcalc_world_impl
    end interface plcalc_world
    private :: plcalc_world_impl

    interface plcol1
        module procedure plcol1_impl
    end interface plcol1
    private :: plcol1_impl

    interface plcolorbar
        module procedure plcolorbar_impl
    end interface plcolorbar
    private :: plcolorbar_impl

    interface plenv
        module procedure plenv_impl
    end interface plenv
    private :: plenv_impl

    interface plerrx
        module procedure plerrx_impl
    end interface plerrx
    private :: plerrx_impl

    interface plerry
        module procedure plerry_impl
    end interface plerry
    private :: plerry_impl

    interface plfill
        module procedure plfill_impl
    end interface plfill
    private :: plfill_impl

    interface plfill3
        module procedure plfill3_impl
    end interface plfill3
    private :: plfill3_impl

    interface plgchr
        module procedure plgchr_impl
    end interface plgchr
    private :: plgchr_impl

    interface plgcmap1_range
        module procedure plgcmap1_range_impl
    end interface plgcmap1_range
    private :: plgcmap1_range_impl

    interface plgcol0a
        module procedure plgcol0a_impl
    end interface plgcol0a
    private :: plgcol0a_impl

    interface plgcolbga
        module procedure plgcolbga_impl
    end interface plgcolbga
    private :: plgcolbga_impl

    interface plgdidev
        module procedure plgdidev_impl
    end interface plgdidev
    private :: plgdidev_impl

    interface plgdiori
        module procedure plgdiori_impl
    end interface plgdiori
    private :: plgdiori_impl

    interface plgdiplt
        module procedure plgdiplt_impl
    end interface plgdiplt
    private :: plgdiplt_impl

    interface plgpage
        module procedure plgpage_impl
    end interface plgpage
    private :: plgpage_impl

    interface plgradient
        module procedure plgradient_impl
    end interface plgradient
    private :: plgradient_impl

    interface plgspa
        module procedure plgspa_impl
    end interface plgspa
    private :: plgspa_impl

    interface plgvpd
        module procedure plgvpd_impl
    end interface plgvpd
    private :: plgvpd_impl

    interface plgvpw
        module procedure plgvpw_impl
    end interface plgvpw
    private :: plgvpw_impl

    interface plhist
        module procedure plhist_impl
    end interface plhist
    private :: plhist_impl

    interface plhlsrgb
        module procedure plhlsrgb_impl
    end interface plhlsrgb
    private :: plhlsrgb_impl

    interface pljoin
        module procedure pljoin_impl
    end interface pljoin
    private :: pljoin_impl

    interface pllegend
        module procedure pllegend_impl
    end interface pllegend
    private :: pllegend_impl

    interface pllightsource
        module procedure pllightsource_impl
    end interface pllightsource
    private :: pllightsource_impl

    interface plline
        module procedure plline_impl
    end interface plline
    private :: plline_impl

    interface plline3
        module procedure plline3_impl
    end interface plline3
    private :: plline3_impl

    interface plmesh
        module procedure plmesh_impl
    end interface plmesh
    private :: plmesh_impl

    interface plmeshc
        module procedure plmeshc_impl
    end interface plmeshc
    private :: plmeshc_impl

    interface plmtex
        module procedure plmtex_impl
    end interface plmtex
    private :: plmtex_impl

    interface plot3d
        module procedure plot3d_impl
    end interface plot3d
    private :: plot3d_impl

    interface plot3dc
        module procedure plot3dc_impl
    end interface plot3dc
    private :: plot3dc_impl

    interface plot3dcl
        module procedure plot3dcl_impl
    end interface plot3dcl
    private :: plot3dcl_impl

    interface plpath
        module procedure plpath_impl
    end interface plpath
    private :: plpath_impl

    interface plpoin
        module procedure plpoin_impl
    end interface plpoin
    private :: plpoin_impl

    interface plpoin3
        module procedure plpoin3_impl
    end interface plpoin3
    private :: plpoin3_impl

    interface plpoly3
        module procedure plpoly3_impl
    end interface plpoly3
    private :: plpoly3_impl

    interface plptex
        module procedure plptex_impl
    end interface plptex
    private :: plptex_impl

    interface plrgbhls
        module procedure plrgbhls_impl
    end interface plrgbhls
    private :: plrgbhls_impl

    interface plschr
        module procedure plschr_impl
    end interface plschr
    private :: plschr_impl

    interface plscmap0a
        module procedure plscmap0a_impl
    end interface plscmap0a
    private :: plscmap0a_impl

    interface plscmap1_range
        module procedure plscmap1_range_impl
    end interface plscmap1_range
    private :: plscmap1_range_impl

    interface plscmap1a
        module procedure plscmap1a_impl
    end interface plscmap1a
    private :: plscmap1a_impl

    interface plscmap1l
        module procedure plscmap1l_impl
    end interface plscmap1l
    private :: plscmap1l_impl

    interface plscmap1la
        module procedure plscmap1la_impl
    end interface plscmap1la
    private :: plscmap1la_impl

    interface plscol0a
        module procedure plscol0a_impl
    end interface plscol0a
    private :: plscol0a_impl

    interface plsdidev
        module procedure plsdidev_impl
    end interface plsdidev
    private :: plsdidev_impl

    interface plsdimap
        module procedure plsdimap_impl
    end interface plsdimap
    private :: plsdimap_impl

    interface plsmaj
        module procedure plsmaj_impl
    end interface plsmaj
    private :: plsmaj_impl

    interface plsmin
        module procedure plsmin_impl
    end interface plsmin
    private :: plsmin_impl

    interface plspage
        module procedure plspage_impl
    end interface plspage
    private :: plspage_impl

    interface plstring
        module procedure plstring_impl
    end interface plstring
    private :: plstring_impl

    interface plsurf3d
        module procedure plsurf3d_impl
    end interface plsurf3d
    private :: plsurf3d_impl

    interface plsurf3dl
        module procedure plsurf3dl_impl
    end interface plsurf3dl
    private :: plsurf3dl_impl

    interface plsvpa
        module procedure plsvpa_impl
    end interface plsvpa
    private :: plsvpa_impl

    interface plsym
        module procedure plsym_impl
    end interface plsym
    private :: plsym_impl

    interface plvasp
        module procedure plvasp_impl
    end interface plvasp
    private :: plvasp_impl

    interface plvpas
        module procedure plvpas_impl
    end interface plvpas
    private :: plvpas_impl

    interface plvpor
        module procedure plvpor_impl
    end interface plvpor
    private :: plvpor_impl

    interface plw3d
        module procedure plw3d_impl
    end interface plw3d
    private :: plw3d_impl

    interface plwidth
        module procedure plwidth_impl
    end interface plwidth
    private :: plwidth_impl

    interface plwind
        module procedure plwind_impl
    end interface plwind
    private :: plwind_impl


contains

subroutine matrix_to_c( array, carray, caddress )
    real(kind=wp), dimension(:,:), intent(in) :: array
    real(kind=private_plflt), dimension(:,:), allocatable, target, intent(out) :: carray
    type(c_ptr), dimension(:), allocatable, intent(out) :: caddress

    integer :: i

    allocate( carray(size(array,2),size(array,1)) )
    allocate( caddress(size(array,2)) )

    carray = transpose( array )

    do i = 1,size(array,2)
        caddress(i) = c_loc(carray(1,i))
    enddo
end subroutine matrix_to_c

subroutine pl_setcontlabelparam_impl( offset, size, spacing, active )
   real(kind=wp), intent(in) :: offset, size, spacing
   integer, intent(in) :: active

   interface
       subroutine c_pl_setcontlabelparam( offset, size, spacing, active) bind(c,name='c_pl_setcontlabelparam')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value :: active
           real(kind=private_plflt), value :: offset, size, spacing
       end subroutine c_pl_setcontlabelparam
   end interface

   call c_pl_setcontlabelparam( real(offset,kind=private_plflt), real(size,kind=private_plflt), &
        real(spacing,kind=private_plflt), int(active,kind=private_plint) )
end subroutine pl_setcontlabelparam_impl

subroutine plarc_log_impl( x, y, a, b, angle1, angle2, rotate, fill )
   real(kind=wp), intent(in) :: x, y, a, b, angle1, angle2, rotate
   logical, intent(in) :: fill

   call plarc_impl( x, y, a, b, angle1, angle2, rotate, int( merge(1,0,fill), kind=private_plint ) )
end subroutine plarc_log_impl

subroutine plarc_impl( x, y, a, b, angle1, angle2, rotate, fill )
   real(kind=wp), intent(in) :: x, y, a, b, angle1, angle2, rotate
   integer, intent(in) :: fill

   interface
       subroutine c_plarc( x, y, a, b, angle1, angle2, rotate, fill ) bind(c,name='c_plarc')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value :: fill
           real(kind=private_plflt), value :: x, y, a, b, angle1, angle2, rotate
       end subroutine c_plarc
   end interface

   call c_plarc( real(x,kind=private_plflt), real(y,kind=private_plflt), real(a,kind=private_plflt), &
                 real(b,kind=private_plflt), real(angle1,kind=private_plflt), real(angle2,kind=private_plflt), &
                 real(rotate,kind=private_plflt), int(fill,kind=private_plint) )
end subroutine plarc_impl

subroutine plbin_impl( x, y, center )
   real(kind=wp), dimension(:), intent(in) :: x, y
   integer, intent(in) :: center

   interface
       subroutine c_plbin( sz, x, y, center ) bind(c,name='c_plbin')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value :: sz, center
           real(kind=private_plflt), dimension(*), intent(in) :: x, y
       end subroutine c_plbin
   end interface

   call c_plbin( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                 int(center,kind=private_plint) )
end subroutine plbin_impl

subroutine plbox_impl(xopt,xtick,nxsub,yopt,ytick,nysub)
    real(kind=wp), intent(in) :: xtick, ytick
    integer, intent(in) :: nxsub, nysub
    character*(*), intent(in) :: xopt,yopt

    interface
        subroutine c_plbox(xopt,xtick,nxsub,yopt,ytick,nysub) bind(c,name='c_plbox')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: xtick, ytick
            integer(kind=private_plint), value :: nxsub, nysub
            character(len=1), dimension(*), intent(in) :: xopt, yopt
        end subroutine c_plbox
    end interface

    call c_plbox( trim(xopt)//c_null_char, real(xtick,kind=private_plflt), int(nxsub,kind=private_plint), &
                  trim(yopt)//c_null_char, real(ytick,kind=private_plflt), int(nysub,kind=private_plint) )
end subroutine plbox_impl

subroutine plbox3_impl(xopt,xlabel,xtick,nxsub,yopt,ylabel,ytick,nysub, &
                       zopt,zlabel,ztick,nzsub)

    real(kind=wp), intent(in) :: xtick, ytick, ztick
    character*(*), intent(in) :: xopt, xlabel, yopt, ylabel, zopt, zlabel
    integer, intent(in) :: nxsub, nysub, nzsub

    interface
        subroutine c_plbox3(xopt,xlabel,xtick,nxsub,yopt,ylabel,ytick,nysub, &
                            zopt,zlabel,ztick,nzsub) bind(c,name='c_plbox3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value    :: xtick, ytick, ztick
            integer(kind=private_plint), value :: nxsub, nysub, nzsub
            character(len=1), dimension(*), intent(in) :: xopt, yopt, zopt, xlabel, ylabel, zlabel
        end subroutine c_plbox3
    end interface

  call c_plbox3( trim(xopt)//c_null_char, trim(xlabel)//c_null_char, real(xtick,kind=private_plflt), &
                 int(nxsub,kind=private_plint),                                                      &
                 trim(yopt)//c_null_char, trim(ylabel)//c_null_char, real(ytick,kind=private_plflt), &
                 int(nysub,kind=private_plint),                                                      &
                 trim(zopt)//c_null_char, trim(zlabel)//c_null_char, real(ztick,kind=private_plflt), &
                 int(nzsub,kind=private_plint)                                                       )
end subroutine plbox3_impl

subroutine plcalc_world_impl( rx, ry, wx, wy, window )
    real(kind=wp), intent(in) :: rx, ry
    real(kind=wp), intent(out) :: wx, wy
    integer, intent(out) :: window

    real(kind=private_plflt) :: wxx, wyy
    integer(kind=private_plint) :: windowout

    interface
        subroutine c_plcalc_world( rx, ry, wx, wy, window ) bind(c,name='c_plcalc_world')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(in) :: window
            real(kind=private_plflt), value :: rx, ry
            real(kind=private_plflt), intent(out) :: wx, wy
        end subroutine c_plcalc_world
    end interface

    call c_plcalc_world( real(rx,kind=private_plflt), real(ry,kind=private_plflt), wxx, wyy, windowout )
    wx = wxx
    wy = wyy
    window = windowout
end subroutine plcalc_world_impl

subroutine plcol1_impl( col )
    real(kind=wp), intent(in) :: col

    interface
        subroutine c_plcol1( col ) bind(c,name='c_plcol1')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: col
        end subroutine c_plcol1
    end interface

    call c_plcol1( real(col,kind=private_plflt) )
end subroutine plcol1_impl

subroutine plcolorbar_impl( &
     colorbar_width, colorbar_height, &
     opt, position, x, y, &
     x_length, y_length, bg_color, bb_color, bb_style, &
     low_cap_color, high_cap_color, &
     cont_color, cont_width, &
     label_opts, labels, &
     axis_opts, ticks, sub_ticks, n_values, values )
     

    real(kind=wp), intent(out) :: colorbar_width, colorbar_height
    real(kind=wp), intent(in) :: x_length, y_length, x, y, low_cap_color, high_cap_color, cont_width
    real(kind=wp), dimension(0:, 0:), intent(in) :: values
    integer, intent(in) :: position, opt, bg_color, bb_color, bb_style, cont_color

    integer, dimension(0:), intent(in) :: label_opts, sub_ticks, n_values
    real(kind=wp), dimension(0:), intent(in) :: ticks
    character(len=*), dimension(0:), intent(in) :: labels, axis_opts

    integer :: n_labels, n_axes
    real(kind=private_plflt) :: colorbar_width_out, colorbar_height_out
    real(kind=private_plflt), dimension(:,:), allocatable :: values_c
    type(c_ptr), dimension(:), allocatable :: values_address

    interface
       subroutine interface_plcolorbar( &
            colorbar_width, colorbar_height, &
            opt, position, x, y, &
            x_length, y_length, bg_color, bb_color, bb_style, &
            low_cap_color, high_cap_color, &
            cont_color, cont_width, &
            n_labels, label_opts, length_labels, labels, &
            n_axes, length_axis_opts, axis_opts, ticks, sub_ticks, n_values, values_address ) &
            bind(c,name='fc_plcolorbar')

         use iso_c_binding, only: c_char, c_ptr
         implicit none
         include 'included_plplot_interface_private_types.f90'

         real(kind=private_plflt), intent(out) :: colorbar_width, colorbar_height
         real(kind=private_plflt), intent(in), value :: x_length, y_length, x, y, &
              low_cap_color, high_cap_color, cont_width
         integer(kind=private_plint), intent(in), value :: position, opt, bg_color, bb_color, bb_style, cont_color
         integer(kind=private_plint), intent(in), value :: n_labels, length_labels, n_axes, length_axis_opts
         real(kind=private_plflt), intent(in), dimension(*) :: ticks
         integer(kind=private_plint), intent(in), dimension(*) :: label_opts, sub_ticks, n_values
         type(c_ptr), intent(in), dimension(*) :: values_address
         
         ! These Fortran arguments require special processing done
         ! in fc_plcolorbar at the C level to interoperate properly
         ! with the C version of plcolorbar.
         character(c_char), intent(in) :: labels(length_labels, n_labels), axis_opts(length_axis_opts, n_axes)
         
       end subroutine interface_plcolorbar
    end interface


    ! Determine number of label entries and demand consistent
    ! array sizes.

    n_labels = size(label_opts)
    if( n_labels /= size(labels) ) then
       write(0,*) "f95 plcolorbar: inconsistent array sizes not allowed for the following arrays:"
       write(0,*) "label_opts"
       write(0,*) "labels"
       return
    end if

    n_axes = size(axis_opts)
    if( &
         n_axes /= size(ticks) .or. &
         n_axes /= size(sub_ticks) .or. &
         n_axes /= size(n_values) .or. &
         n_axes /= size(values,1) &
         ) then
       write(0,*) "f95 plcolorbar: inconsistent array sizes not allowed for the following arrays:"
       write(0,*) "axis_opts"
       write(0,*) "ticks"
       write(0,*) "sub_ticks"
       write(0,*) "n_values"
       write(0,*) "first dimension of values"
       return
    end if

    if(maxval(n_values) > size(values,2) ) then
       write(0,*) "f95: plcolorbar: maximum of n_values > second dimension of values"
       return
    end if

    call matrix_to_c( values, values_c, values_address )

    call interface_plcolorbar( &
         colorbar_width_out, colorbar_height_out, &
         int(opt,kind=private_plint), int(position,kind=private_plint), &
         real(x,kind=private_plflt), real(y,kind=private_plflt), &
         real(x_length,kind=private_plflt), real(y_length,kind=private_plflt), &
         int(bg_color,kind=private_plint), &
         int(bb_color,kind=private_plint), int(bb_style,kind=private_plint), &
         real(low_cap_color,kind=private_plflt), real(high_cap_color,kind=private_plflt), &
         int(cont_color,kind=private_plint), real(cont_width,kind=private_plflt), &
         int(n_labels, kind=private_plint), int(label_opts, kind=private_plint), &
         len(labels(0), kind=private_plint), labels, &
         int(n_axes, kind=private_plint), len(axis_opts(0), kind=private_plint), &
         axis_opts, real(ticks, kind=private_plflt), int(sub_ticks, kind=private_plint), &
         int(n_values, kind=private_plint), values_address &
         )
    ! Transform output real values.
    colorbar_width = real(colorbar_width_out, kind=wp)
    colorbar_height = real(colorbar_height_out, kind=wp)

end subroutine plcolorbar_impl

subroutine plenv_impl( xmin, xmax, ymin, ymax, just, axis )
    real(kind=wp), intent(in)  :: xmin, xmax, ymin, ymax
    integer, intent(in) :: just, axis

    interface
        subroutine c_plenv( xmin, xmax, ymin, ymax, just, axis ) bind(c, name='c_plenv')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: xmin, xmax, ymin, ymax
            integer(kind=private_plint), value :: just, axis
        end subroutine c_plenv
    end interface

    call c_plenv( real(xmin,private_plflt), real(xmax,private_plflt), real(ymin,private_plflt), real(ymax,private_plflt), &
                  int(just,private_plint), int(axis,private_plint) )
end subroutine plenv_impl

subroutine plerrx_impl( xmin, xmax, y )
    real(kind=wp), intent(in) :: xmin, xmax
    real(kind=wp), dimension(:), intent(in) :: y

    interface
        subroutine c_plerrx( n, xmin, xmax, y ) bind( c, name='c_plerrx')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: n
            real(kind=private_plflt), value :: xmin, xmax
            real(kind=private_plflt), dimension(*), intent(in) :: y
        end subroutine c_plerrx
    end interface

    call c_plerrx( size(y,kind=private_plint), real(xmin,private_plflt), real(xmax,private_plflt), real(y,private_plflt) )
end subroutine plerrx_impl

subroutine plerry_impl( x, ymin, ymax )
    real(kind=wp), dimension(:), intent(in) :: x
    real(kind=wp), intent(in) :: ymin, ymax

    interface
        subroutine c_plerry( n, x, ymin, ymax ) bind( c, name='c_plerry')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: n
            real(kind=private_plflt), dimension(*), intent(in) :: x
            real(kind=private_plflt), value :: ymin, ymax
        end subroutine c_plerry
    end interface

    call c_plerry( size(x,kind=private_plint), real(x,private_plflt), real(ymin,private_plflt), real(ymax,private_plflt) )
end subroutine plerry_impl

subroutine plfill_impl( x, y )
    real(kind=wp), dimension(:), intent(in) :: x, y

    interface
        subroutine c_plfill( n, x, y ) bind( c, name='c_plfill')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: n
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
        end subroutine c_plfill
    end interface

    call c_plfill( size(x,kind=private_plint), real(x,private_plflt), real(y,private_plflt) )
end subroutine plfill_impl

subroutine plfill3_impl( x, y, z )
    real(kind=wp), dimension(:), intent(in) :: x, y, z

    interface
        subroutine c_plfill3( n, x, y, z ) bind( c, name='c_plfill3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: n
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, z
        end subroutine c_plfill3
    end interface

    call c_plfill3( size(x,kind=private_plint), real(x,private_plflt), real(y,private_plflt) , real(z,private_plflt) )
end subroutine plfill3_impl

subroutine plgchr_impl( chrdef, chrht )
    real(kind=wp), intent(out) :: chrdef, chrht

    real(kind=private_plflt) :: cchrdef, cchrht

    interface
       subroutine c_plgchr( chrdef, chrht ) bind(c,name='c_plgchr')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           real(kind=private_plflt), intent(out) :: chrdef, chrht
       end subroutine c_plgchr
   end interface

   call c_plgchr( cchrdef, cchrht )
   chrdef = cchrdef
   chrht  = cchrht
end subroutine plgchr_impl

subroutine plgcmap1_range_impl( min_color, max_color )
    real(kind=wp), intent(out) :: min_color, max_color

    real(kind=private_plflt) :: mmin_color, mmax_color

    interface
        subroutine c_plgcmap1_range( min_color, max_color ) bind(c,name='c_plgcmap1_range')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: min_color, max_color
        end subroutine c_plgcmap1_range
    end interface

    call c_plgcmap1_range( mmin_color, mmax_color )
    min_color = mmin_color
    max_color = mmax_color
end subroutine plgcmap1_range_impl

subroutine plgcol0a_impl( icol, r, g, b, a )
    integer, intent(in) :: icol
    integer, intent(out) :: r, g, b
    real(kind=wp), intent(out) :: a

    integer(kind=private_plint) :: rr, gg, bb
    real(kind=private_plflt) :: aa

    interface
        subroutine c_plgcol0a( icol, r, g, b, a ) bind(c,name='c_plgcol0a')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: icol
            integer(kind=private_plint)        :: r, g, b
            real(kind=private_plflt)           :: a
        end subroutine c_plgcol0a
    end interface

    call c_plgcol0a( int(icol,kind=private_plint), rr, gg, bb, aa )
    r = rr
    g = gg
    b = bb
    a = aa
end subroutine plgcol0a_impl

subroutine plgcolbga_impl( r, g, b, a )
    integer, intent(out) :: r, g, b
    real(kind=wp), intent(out) :: a

    integer(kind=private_plint) :: rr, gg, bb
    real(kind=private_plflt)    :: aa

    interface
        subroutine c_plgcolbga( r, g, b, a ) bind(c,name='c_plgcolbga')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: r, g, b
            real(kind=private_plflt), intent(out) :: a
        end subroutine c_plgcolbga
    end interface

    call c_plgcolbga( rr, gg, bb, aa )
    r = rr
    g = gg
    b = bb
    a = aa
end subroutine plgcolbga_impl

subroutine plgdidev_impl( mar, aspect, jx, jy )
    real(kind=wp), intent(out) :: mar, aspect, jx, jy

    real(kind=private_plflt) :: mmar, aaspect, jjx, jjy

    interface
        subroutine c_plgdidev( mar, aspect, jx, jy ) bind(c,name='c_plgdidev')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: mar, aspect, jx, jy
        end subroutine c_plgdidev
    end interface

    call c_plgdidev( mmar, aaspect, jjx, jjy )
    mar    = mmar
    aspect = aaspect
    jx     = jjx
    jy     = jjy
end subroutine plgdidev_impl

subroutine plgdiori_impl( rot )
    real(kind=wp), intent(out) :: rot

    real(kind=private_plflt) :: rrot

    interface
        subroutine c_plgdiori( rot ) bind(c,name='c_plgdiori')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: rot
        end subroutine c_plgdiori
    end interface

    call c_plgdiori( rrot )
    rot = rrot
end subroutine plgdiori_impl

subroutine plgdiplt_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(out) :: xmin, xmax, ymin, ymax

    real(kind=private_plflt)  :: xxmin, xxmax, yymin, yymax

    interface
        subroutine c_plgdiplt( xmin, xmax, ymin, ymax ) bind(c,name='c_plgdiplt')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: xmin, xmax, ymin, ymax
        end subroutine c_plgdiplt
    end interface

    call c_plgdiplt( xxmin, xxmax, yymin, yymax )
    xmin = xxmin
    xmax = xxmax
    ymin = yymin
    ymax = yymax
end subroutine plgdiplt_impl

subroutine plgpage_impl( xpmm, ypmm, xwid, ywid, xoff, yoff )
    real(kind=wp), intent(out) :: xpmm, ypmm
    integer, intent(out) :: xwid, ywid, xoff, yoff

    real(kind=private_plflt) :: xxpmm, yypmm
    integer(kind=private_plint) :: xxwid, yywid, xxoff, yyoff
    interface
        subroutine c_plgpage( xpmm, ypmm, xwid, ywid, xoff, yoff ) bind(c,name='c_plgpage')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: xpmm, ypmm
            integer(kind=private_plint), intent(out) :: xwid, ywid, xoff, yoff
        end subroutine c_plgpage
    end interface

    call c_plgpage( xxpmm, yypmm, xxwid, yywid, xxoff, yyoff )
    xpmm = xxpmm
    ypmm = yypmm
    xwid = xxwid
    ywid = yywid
    xoff = xxoff
    yoff = yyoff
end subroutine plgpage_impl

subroutine plgradient_impl( x, y, angle )
    real(kind=wp), dimension(:), intent(in) :: x, y
    real(kind=wp), intent(in) :: angle

    interface
       subroutine c_plgradient( sz, x, y, angle ) bind(c,name='c_plgradient')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value :: sz
           real(kind=private_plflt), dimension(*), intent(in) :: x, y
           real(kind=private_plflt), value        :: angle
       end subroutine c_plgradient
    end interface

    call c_plgradient( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                       real(angle,kind=private_plflt) )
end subroutine plgradient_impl

subroutine plgspa_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(out) :: xmin, xmax, ymin, ymax

    real(kind=private_plflt) :: xxmin, xxmax, yymin, yymax

    interface
        subroutine c_plgspa( xmin, xmax, ymin, ymax ) bind(c,name='c_plgspa')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt) :: xmin, xmax, ymin, ymax
        end subroutine c_plgspa
    end interface

    call c_plgspa( xxmin, xxmax, yymin, yymax )
    xmin = xxmin
    xmax = xxmax
    ymin = yymin
    ymax = yymax
end subroutine plgspa_impl

subroutine plgvpd_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(out) :: xmin, xmax, ymin, ymax

    real(kind=private_plflt)  :: xxmin, xxmax, yymin, yymax

    interface
        subroutine c_plgvpd( xmin, xmax, ymin, ymax ) bind(c,name='c_plgvpd')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: xmin, xmax, ymin, ymax
        end subroutine c_plgvpd
    end interface

    call c_plgvpd( xxmin, xxmax, yymin, yymax )
    xmin = xxmin
    xmax = xxmax
    ymin = yymin
    ymax = yymax
end subroutine plgvpd_impl

subroutine plgvpw_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(out) :: xmin, xmax, ymin, ymax

    real(kind=private_plflt)  :: xxmin, xxmax, yymin, yymax

    interface
        subroutine c_plgvpw( xmin, xmax, ymin, ymax ) bind(c,name='c_plgvpw')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: xmin, xmax, ymin, ymax
        end subroutine c_plgvpw
    end interface

    call c_plgvpw( xxmin, xxmax, yymin, yymax )
    xmin = xxmin
    xmax = xxmax
    ymin = yymin
    ymax = yymax
end subroutine plgvpw_impl

subroutine plhist_impl( data, datmin, datmax, nbin, oldwin )
    real(kind=wp), dimension(:), intent(in) :: data
    real(kind=wp), intent(in) :: datmin, datmax
    integer, intent(in) :: nbin, oldwin

    interface
        subroutine c_plhist( n, data, datmin, datmax, nbin, oldwin ) bind(c,name='c_plhist')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), dimension(*), intent(in) :: data
            real(kind=private_plflt), value :: datmin, datmax
            integer(kind=private_plint), value :: n, nbin, oldwin
        end subroutine c_plhist
    end interface

   call c_plhist( size(data,kind=private_plint), real(data,kind=private_plflt),     &
                  real(datmin,kind=private_plflt), real(datmax,kind=private_plflt), &
                  int(nbin,kind=private_plint), int(oldwin,kind=private_plint) )
end subroutine plhist_impl

subroutine plhlsrgb_impl( h, l, s, r, g, b )
    real(kind=wp), intent(in) :: h, l, s
    real(kind=wp), intent(out) :: r, g, b

    real(kind=private_plflt) :: rr, gg, bb

    interface
        subroutine c_plhlsrgb( h, l, s, r, g, b ) bind(c,name='c_plhlsrgb')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: h, l, s
            real(kind=private_plflt), intent(out) :: r, g, b
        end subroutine c_plhlsrgb
    end interface

    call c_plhlsrgb( real(h,kind=private_plflt), real(l,kind=private_plflt), real(s,kind=private_plflt), &
                     rr, gg, bb )
    r = rr
    b = bb
    g = gg
end subroutine plhlsrgb_impl

subroutine pljoin_impl( x1, y1, x2, y2 )
    real(kind=wp), intent(in) :: x1, y1, x2, y2

    interface
        subroutine c_pljoin( x1, y1, x2, y2 ) bind(c,name='c_pljoin')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: x1, y1, x2, y2
        end subroutine c_pljoin
    end interface

    call c_pljoin( real(x1,kind=private_plflt), real(y1,kind=private_plflt), &
                   real(x2,kind=private_plflt), real(y2,kind=private_plflt) )
end subroutine pljoin_impl

subroutine pllegend_impl( &
     legend_width, legend_height, &
     opt, position, x, y, &
     plot_width, bg_color, bb_color, bb_style, &
     nrow, ncolumn, opt_array, &
     text_offset, text_scale, text_spacing, &
     text_justification, text_colors, text, &
     box_colors, box_patterns, box_scales, &
     box_line_widths, &
     line_colors, line_styles, line_widths, &
     symbol_colors, symbol_scales, &
     symbol_numbers, symbols )

    real(kind=wp), intent(out) :: legend_width, legend_height
    real(kind=wp), intent(in) :: plot_width, x, y
    real(kind=wp), intent(in) :: text_offset, text_scale, text_spacing, text_justification
    integer, intent(in) :: position, opt, bg_color, bb_color, bb_style
    integer, intent(in) :: nrow, ncolumn

    character(len=*), dimension(0:), intent(in) :: text, symbols

    integer, dimension(0:), intent(in) :: opt_array, text_colors, box_colors
    integer, dimension(0:), intent(in) :: box_patterns
    real(kind=wp), dimension(0:), intent(in) :: box_line_widths
    integer, dimension(0:), intent(in) :: line_colors, line_styles
    real(kind=wp), dimension(0:), intent(in) :: line_widths
    integer, dimension(0:), intent(in) :: symbol_colors, symbol_numbers
    real(kind=wp), dimension(0:), intent(in) :: box_scales, symbol_scales

    integer(kind=private_plint) :: nlegend
    real(kind=private_plflt) :: legend_width_out, legend_height_out

    interface
       subroutine interface_pllegend( &
            legend_width, legend_height, &
            opt, position, x, y, &
            plot_width, bg_color, bb_color, bb_style, &
            nrow, ncolumn, nlegend, opt_array, &
            text_offset, text_scale, text_spacing, &
            text_justification, text_colors, length_text, text, &
            box_colors, box_patterns, box_scales, &
            box_line_widths, &
            line_colors, line_styles, line_widths, &
            symbol_colors, symbol_scales, &
            symbol_numbers, length_symbols, symbols ) &
            bind(c,name='fc_pllegend')

         use iso_c_binding, only: c_char
         implicit none
         include 'included_plplot_interface_private_types.f90'

         real(kind=private_plflt), intent(out) :: legend_width, legend_height
         real(kind=private_plflt), intent(in), value :: plot_width, x, y
         real(kind=private_plflt), intent(in), value :: text_offset, text_scale, text_spacing, text_justification
         integer(kind=private_plint), intent(in), value :: position, opt, bg_color, bb_color, bb_style
         integer(kind=private_plint), intent(in), value :: nrow, ncolumn, nlegend, length_text, length_symbols
         
         ! These Fortran arguments require special processing done
         ! in fc_pllegend at the C level to interoperate properly
         ! with the C version of pllegend.
         character(c_char), intent(in) :: text(length_text, nlegend), symbols(length_symbols, nlegend)
         
         integer(kind=private_plint), dimension(*), intent(in) :: opt_array, text_colors, box_colors
         integer(kind=private_plint), dimension(*), intent(in) :: box_patterns
         real(kind=private_plflt), dimension(*), intent(in) :: box_line_widths
         integer(kind=private_plint), dimension(*), intent(in) :: line_colors, line_styles
         real(kind=private_plflt), dimension(*), intent(in) :: line_widths
         integer(kind=private_plint), dimension(*), intent(in) :: symbol_colors, symbol_numbers
         real(kind=private_plflt), dimension(*), intent(in) :: box_scales, symbol_scales
       end subroutine interface_pllegend
    end interface


    ! Determine number of legend entries and demand consistent
    ! array sizes.

    nlegend = size(opt_array)
    if( &
         nlegend /= size(text_colors) .or. &
         nlegend /= size(text) .or. & 
         nlegend /= size(box_colors) .or. & 
         nlegend /= size(box_patterns) .or. & 
         nlegend /= size(box_scales) .or. & 
         nlegend /= size(box_line_widths) .or. & 
         nlegend /= size(line_colors) .or. & 
         nlegend /= size(line_styles) .or. & 
         nlegend /= size(line_widths) .or. & 
         nlegend /= size(symbol_colors) .or. & 
         nlegend /= size(symbol_scales) .or. & 
         nlegend /= size(symbol_numbers) .or. & 
         nlegend /= size(symbols) &
         ) then
       write(0,*) "f95 pllegend: inconsistent array sizes not allowed for the following arrays:"
       write(0,*) "opt_array"
       write(0,*) "text_colors"
       write(0,*) "text"
       write(0,*) "box_colors"
       write(0,*) "box_patterns"
       write(0,*) "box_scales"
       write(0,*) "box_line_widths"
       write(0,*) "line_colors"
       write(0,*) "line_styles"
       write(0,*) "line_widths"
       write(0,*) "symbol_colors"
       write(0,*) "symbol_scales"
       write(0,*) "symbol_numbers"
       write(0,*) "symbols"
       return
    end if

    call interface_pllegend( &
         legend_width_out, legend_height_out, &
         int(opt,kind=private_plint), int(position,kind=private_plint), &
         real(x,kind=private_plflt), real(y,kind=private_plflt), &
         real(plot_width,kind=private_plflt), int(bg_color,kind=private_plint), &
         int(bb_color,kind=private_plint), int(bb_style,kind=private_plint), &
         int(nrow,kind=private_plint), int(ncolumn,kind=private_plint), &
         int(nlegend,kind=private_plint), int(opt_array,kind=private_plint), &
         real(text_offset,kind=private_plflt), real(text_scale,kind=private_plflt), &
         real(text_spacing,kind=private_plflt), &
         real(text_justification,kind=private_plflt), int(text_colors,kind=private_plint), &
         len(text(0), kind=private_plint), text, &
         int(box_colors,kind=private_plint), int(box_patterns,kind=private_plint), &
         real(box_scales,kind=private_plflt), &
         real(box_line_widths,kind=private_plflt), &
         int(line_colors,kind=private_plint), int(line_styles,kind=private_plint), &
         real(line_widths,kind=private_plflt), &
         int(symbol_colors,kind=private_plint), real(symbol_scales,kind=private_plflt), &
         int(symbol_numbers,kind=private_plint), len(symbols(0), kind=private_plint), symbols )
    ! Transform output real values.
    legend_width = real(legend_width_out, kind=wp)
    legend_height = real(legend_height_out, kind=wp)

end subroutine pllegend_impl

subroutine pllightsource_impl( x, y, z )
    real(kind=wp), intent(in) :: x, y, z

    interface
        subroutine c_pllightsource( x, y, z ) bind(c,name='c_pllightsource')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: x, y, z
        end subroutine c_pllightsource
    end interface

    call c_pllightsource( real(x,kind=private_plflt), real(y,kind=private_plflt), real(z,kind=private_plflt) )
end subroutine pllightsource_impl

subroutine plline_impl( x, y )
    real(kind=wp), dimension(:), intent(in) :: x, y

    interface
        subroutine c_plline( sz, x, y ) bind(c,name='c_plline')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: sz
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
        end subroutine c_plline
    end interface

    call c_plline( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt) )
end subroutine plline_impl

subroutine plline3_impl( x, y, z )
   real(kind=wp), dimension(:), intent(in) :: x, y, z

   interface
       subroutine c_plline3( sz, x, y, z ) bind(c,name='c_plline3')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value :: sz
           real(kind=private_plflt), dimension(*), intent(in) :: x, y, z
       end subroutine c_plline3
   end interface

   call c_plline3( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                   real(z,kind=private_plflt) )
end subroutine plline3_impl

subroutine plmesh_impl( x, y, z, opt )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y
    real(kind=wp), dimension(:,:), intent(in) :: z

    real(kind=private_plflt), dimension(:,:), allocatable :: zz
    type(c_ptr), dimension(:), allocatable :: zaddress

    interface
        subroutine c_plmesh( x, y, zaddress, nx, ny, opt ) bind(c,name='c_plmesh')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: nx, ny, opt
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine c_plmesh
    end interface

    call matrix_to_c( z, zz, zaddress )

    call c_plmesh( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress, &
                      size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint))

end subroutine plmesh_impl

subroutine plmeshc_impl( x, y, z, opt, clevel )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z

    real(kind=private_plflt), dimension(:,:), allocatable :: zz
    type(c_ptr), dimension(:), allocatable :: zaddress

    interface
        subroutine c_plmeshc( x, y, zaddress, nx, ny, opt, clevel, nlevel ) bind(c,name='c_plmeshc')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: nx, ny, opt, nlevel
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine c_plmeshc
    end interface

    call matrix_to_c( z, zz, zaddress )

    call c_plmeshc( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress, &
                      size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                      real(clevel,kind=private_plflt), size(clevel,kind=private_plint) )

end subroutine plmeshc_impl

subroutine plmtex_impl( side, disp, pos, xjust, text )
    real(kind=wp), intent(in) :: disp, pos, xjust
    character*(*), intent(in) :: side, text

    interface
        subroutine c_plmtex( side, disp, pos, xjust, text ) bind(c,name='c_plmtex')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            character(len=1), dimension(*), intent(in) :: side, text
            real(kind=private_plflt), value :: disp, pos, xjust
        end subroutine c_plmtex
    end interface

    call c_plmtex( trim(side)//c_null_char, real(disp,kind=private_plflt), real(pos,kind=private_plflt), &
                   real(xjust,kind=private_plflt), trim(text)//c_null_char )
end subroutine plmtex_impl

subroutine plot3d_impl( x, y, z, opt, side)
    logical, intent(in) :: side
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y
    real(kind=wp), dimension(:,:), intent(in) :: z

    integer(kind=private_plint)                        :: iside
    real(kind=private_plflt), dimension(:,:), allocatable :: zz
    type(c_ptr), dimension(:), allocatable :: zaddress
    

    interface
        subroutine c_plot3d( x, y, zaddress, nx, ny, opt, iside ) bind(c,name='c_plot3d')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: nx, ny, opt, iside
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine c_plot3d
    end interface

    call matrix_to_c( z, zz, zaddress )
    iside = merge(1,0,side)

    call c_plot3d( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress,  &
         size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), iside)

end subroutine plot3d_impl

subroutine plot3dc_impl( x, y, z, opt, clevel )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z

    real(kind=private_plflt), dimension(:,:), allocatable :: zz
    type(c_ptr), dimension(:), allocatable :: zaddress

    interface
        subroutine c_plot3dc( x, y, zaddress, nx, ny, opt, clevel, nlevel ) bind(c,name='c_plot3dc')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: nx, ny, opt, nlevel
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine c_plot3dc
    end interface

    call matrix_to_c( z, zz, zaddress )

    call c_plot3dc( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress,  &
                     size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                     real(clevel,kind=private_plflt), size(clevel,kind=private_plint) )

end subroutine plot3dc_impl

subroutine plot3dcl_impl( x, y, z, opt, clevel, indexxmin, indexxmax, indexymin, indexymax )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z
    integer, intent(in) :: indexxmin, indexxmax
    integer, dimension(:), intent(in) :: indexymin, indexymax

    real(kind=private_plflt), dimension(:,:), allocatable :: zz
    type(c_ptr), dimension(:), allocatable :: zaddress

    interface
        subroutine c_plot3dcl( x, y, zaddress, nx, ny, opt, clevel, nlevel, &
                                indexxmin, indexxmax, indexymin, indexymax ) bind(c,name='c_plot3dcl')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: nx, ny, opt, nlevel, indexxmin, indexxmax
            integer(kind=private_plint), dimension(*), intent(in) :: indexymin, indexymax
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine c_plot3dcl
    end interface

    call matrix_to_c( z, zz, zaddress )

    call c_plot3dcl( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress, &
                      size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                      real(clevel,kind=private_plflt), size(clevel,kind=private_plint), &
                      int(indexxmin-1,kind=private_plint), int(indexxmax-1,kind=private_plint), &
                      int(indexymin-1,kind=private_plint), int(indexymax-1,kind=private_plint) )

end subroutine plot3dcl_impl

subroutine plpath_impl( n, x1, y1, x2, y2 )
    integer, intent(in) :: n
    real(kind=wp), intent(in) :: x1, y1, x2, y2

    interface
        subroutine c_plpath( n, x1, y1, x2, y2 ) bind(c,name='c_plpath')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: n
            real(kind=private_plflt), value :: x1, y1, x2, y2
        end subroutine c_plpath
    end interface

    call c_plpath( int(n,kind=private_plint), real(x1,kind=private_plflt), &
                   real(y1,kind=private_plflt), real(x2,kind=private_plflt), real(y2,kind=private_plflt) )
end subroutine plpath_impl

subroutine plpoin_impl( x, y, code )
    integer, intent(in) :: code
    real(kind=wp), dimension(:), intent(in) :: x, y

      interface
          subroutine c_plpoin( n, x, y, code ) bind(c,name='c_plpoin')
              implicit none
              include 'included_plplot_interface_private_types.f90'
              integer(kind=private_plint), value :: n, code
              real(kind=private_plflt), dimension(*), intent(in) :: x, y
          end subroutine c_plpoin
      end interface

     call c_plpoin( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                    int(code,kind=private_plint) )
end subroutine plpoin_impl

subroutine plpoin3_impl( x, y, z, code )
    integer, intent(in) :: code
    real(kind=wp), dimension(:), intent(in) :: x, y, z

    interface
        subroutine c_plpoin3( n, x, y, z, code ) bind(c,name='c_plpoin3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: n, code
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, z
        end subroutine c_plpoin3
    end interface

   call c_plpoin3( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                   real(z,kind=private_plflt), int(code,kind=private_plint) )
end subroutine plpoin3_impl

subroutine plpoly3_impl( x, y, z, draw, ifcc )
    logical, intent(in) :: ifcc
    logical, dimension(:), intent(in) :: draw
    real(kind=wp), dimension(:), intent(in) :: x, y, z

    integer(kind=private_plint), dimension(size(draw)) :: idraw
    integer(kind=private_plint)                        :: iifcc

    interface
        subroutine c_plpoly3( n, x, y, z, draw, ifcc ) bind(c,name='c_plpoly3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: n, ifcc
            integer(kind=private_plint), dimension(*), intent(in) :: draw
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, z
        end subroutine c_plpoly3
    end interface

    iifcc = merge(1,0,ifcc)
    idraw = merge(1,0,draw)
   call c_plpoly3( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                   real(z,kind=private_plflt), idraw, iifcc )
end subroutine plpoly3_impl

subroutine plptex_impl( x, y, dx, dy, xjust, text )

    real(kind=wp), intent(in) :: x, y, dx, dy, xjust
    character*(*), intent(in) :: text

    interface
        subroutine c_plptex( x, y, dx, dy, xjust, text ) bind(c,name='c_plptex')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            character(len=1), dimension(*), intent(in) :: text
            real(kind=private_plflt), value :: x, y, dx, dy, xjust
        end subroutine c_plptex
    end interface

    call c_plptex( real(x,kind=private_plflt), real(y,kind=private_plflt), real(dx,kind=private_plflt), &
                   real(dy,kind=private_plflt), real(xjust,kind=private_plflt), trim(text)//c_null_char)

end subroutine plptex_impl

subroutine plrgbhls_impl( r, g, b, h, l, s )
    real(kind=wp), intent(in) :: r, g, b
    real(kind=wp), intent(out) :: h, l, s

    real(kind=private_plflt) :: hh, ll, ss

    interface
        subroutine c_plrgbhls( r, g, b, h, l, s ) bind(c,name='c_plrgbhls')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: r, g, b
            real(kind=private_plflt), intent(out) :: h, l, s
        end subroutine c_plrgbhls
    end interface

    call c_plrgbhls( real(r,kind=private_plflt), real(g,kind=private_plflt), real(b,kind=private_plflt), &
                     hh, ll, ss )
    h = hh
    l = ll
    s = ss
end subroutine plrgbhls_impl

subroutine plschr_impl( chrdef, chrht )
    real(kind=wp), intent(in) :: chrdef, chrht

    interface
        subroutine c_plschr( chrdef, chrht ) bind(c,name='c_plschr')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: chrdef, chrht
        end subroutine c_plschr
    end interface

    call c_plschr( real(chrdef,kind=private_plflt), real(chrht,kind=private_plflt) )
end subroutine plschr_impl

subroutine plscmap0a_impl( r, g, b, a )
    integer, dimension(:), intent(in) :: r, g, b
    real(kind=wp), dimension(:), intent(in) :: a

    interface
        subroutine c_plscmap0a( r, g, b, a, n ) bind(c,name='c_plscmap0a')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), dimension(*), intent(in) :: r, g, b
            real(kind=private_plflt), dimension(*), intent(in) :: a
            integer(kind=private_plint), value :: n
        end subroutine c_plscmap0a
    end interface

    call c_plscmap0a( int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint), &
                      real(a,kind=private_plflt), size(r,kind=private_plint) )
end subroutine plscmap0a_impl

subroutine plscmap1_range_impl( chrdef, chrht )
    real(kind=wp), intent(in) :: chrdef, chrht

    interface
        subroutine c_plscmap1_range( chrdef, chrht ) bind(c,name='c_plscmap1_range')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: chrdef, chrht
        end subroutine c_plscmap1_range
    end interface

    call c_plscmap1_range( real(chrdef,kind=private_plflt), real(chrht,kind=private_plflt) )
end subroutine plscmap1_range_impl

subroutine plscmap1a_impl( r, g, b, a )
    integer, dimension(:), intent(in) :: r, g, b
    real(kind=wp), dimension(:), intent(in) :: a

    interface
        subroutine c_plscmap1a( r, g, b, a, n ) bind(c,name='c_plscmap1a')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), dimension(*), intent(in) :: r, g, b
            real(kind=private_plflt), dimension(*), intent(in) :: a
            integer(kind=private_plint), value :: n
        end subroutine c_plscmap1a
    end interface

    call c_plscmap1a( int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint), &
                      real(a,kind=private_plflt), size(r,kind=private_plint) )
end subroutine plscmap1a_impl

subroutine plscmap1l_impl( rgbtype, intensity, coord1, coord2, coord3, alt_hue_path)
    logical, intent(in) :: rgbtype
    real(kind=wp), dimension(:), intent(in) :: intensity, coord1, coord2, coord3
    logical, dimension(:), intent(in), optional :: alt_hue_path

    integer :: i
    integer(kind=private_plint), dimension(:), allocatable, target :: ialt_hue_path

    interface
        subroutine c_plscmap1l( type, npts, intensity, coord1, coord2, coord3, alt_hue_path ) bind(c,name='c_plscmap1l')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: type, npts
            real(kind=private_plflt), dimension(*), intent(in) :: intensity, coord1, coord2, coord3
            !integer(kind=private_plint), dimension(*), intent(in) :: alt_hue_path
            type(c_ptr), value :: alt_hue_path
        end subroutine c_plscmap1l
    end interface

    if ( present(alt_hue_path) ) then
        allocate( ialt_hue_path(size(alt_hue_path)) )
        ialt_hue_path = merge(1,0,alt_hue_path)

        call c_plscmap1l( int(merge(1,0,rgbtype),kind=private_plint), size(intensity,kind=private_plint), &
                          real(intensity,kind=private_plflt), real(coord1,kind=private_plflt), &
                          real(coord2,kind=private_plflt), real(coord3,kind=private_plflt), &
                          c_loc(ialt_hue_path) )
    else
        call c_plscmap1l( int(merge(1,0,rgbtype),kind=private_plint), size(intensity,kind=private_plint), &
                          real(intensity,kind=private_plflt), real(coord1,kind=private_plflt), &
                          real(coord2,kind=private_plflt), real(coord3,kind=private_plflt), &
                          c_null_ptr )
    endif
end subroutine plscmap1l_impl

subroutine plscmap1la_impl( rgbtype, intensity, coord1, coord2, coord3, alpha, alt_hue_path)
    logical, intent(in) :: rgbtype
    real(kind=wp), dimension(:), intent(in) :: intensity, coord1, coord2, coord3, alpha
    logical, dimension(:), intent(in) :: alt_hue_path

    integer(kind=private_plint), dimension(size(alt_hue_path)) :: ialt_hue_path

    interface
        subroutine c_plscmap1la( type, n, intensity, coord1, coord2, coord3, alpha, alt_hue_path ) bind(c,name='c_plscmap1la')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), dimension(*), intent(in) :: intensity, coord1, coord2, coord3, alpha
            integer(kind=private_plint), dimension(*), intent(in) :: alt_hue_path
            integer(kind=private_plint), value :: type, n
        end subroutine c_plscmap1la
    end interface

   ialt_hue_path = merge(1,0,alt_hue_path)
   call c_plscmap1la( int(merge(1,0,rgbtype),kind=private_plint), size(intensity,kind=private_plint), &
                      real(intensity,kind=private_plflt), real(coord1,kind=private_plflt), real(coord2,kind=private_plflt), &
                      real(coord3,kind=private_plflt), real(alpha,kind=private_plflt), ialt_hue_path )
end subroutine plscmap1la_impl

subroutine plscol0a_impl( icol, r, g, b, a )
    integer, intent(in) :: icol, r, g, b
    real(kind=wp), intent(in) :: a

    interface
        subroutine c_plscol0a( icol, r, g, b, a ) bind(c,name='c_plscol0a')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: icol, r, g, b
            real(kind=private_plflt), value :: a
        end subroutine c_plscol0a
    end interface

    call c_plscol0a( int(icol,kind=private_plint), int(r,kind=private_plint), int(g,kind=private_plint), &
                     int(b,kind=private_plint), real(a,kind=private_plflt) )
end subroutine plscol0a_impl

subroutine plscolbga_impl( r, g, b, a )
    integer, intent(in) :: r, g, b
    real(kind=wp), intent(in) :: a

    interface
        subroutine c_plscolbga( r, g, b, a ) bind(c,name='c_plscolbga')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: r, g, b
            real(kind=private_plflt), value :: a
        end subroutine c_plscolbga
    end interface

    call c_plscolbga( int(r,kind=private_plint), int(g,kind=private_plint), &
                      int(b,kind=private_plint), real(a,kind=private_plflt) )
end subroutine plscolbga_impl

subroutine plsdidev_impl( mar, aspect, jx, jy )
    real(kind=wp), intent(in) :: mar, aspect, jx, jy

    interface
        subroutine c_plsdidev( mar, aspect, jx, jy ) bind(c,name='c_plsdidev')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: mar, aspect, jx, jy
        end subroutine c_plsdidev
    end interface

    call c_plsdidev( real(mar,kind=private_plflt), real(aspect,kind=private_plflt), &
                     real(jx,kind=private_plflt), real(jy,kind=private_plflt) )
end subroutine plsdidev_impl

subroutine plsdimap_impl( dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm )
    real(kind=wp), intent(in) :: dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm

    interface
        subroutine c_plsdimap( dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm ) bind(c,name='c_plsdimap')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm
        end subroutine c_plsdimap
    end interface

    call c_plsdimap( real(dimxmi,kind=private_plflt), real(dimxmax,kind=private_plflt), &
                     real(diymin,kind=private_plflt), real(dimymax,kind=private_plflt), &
                     real(dimxpmm,kind=private_plflt), real(diypmm,kind=private_plflt) )
end subroutine plsdimap_impl

subroutine plsmaj_impl( def, scale )
    real(kind=wp), intent(in) :: def, scale

    interface
        subroutine c_plsmaj( def, scale ) bind(c,name='c_plsmaj')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: def, scale
        end subroutine c_plsmaj
    end interface

    call c_plsmaj( real(def,kind=private_plflt), real(scale,kind=private_plflt) )

end subroutine plsmaj_impl

subroutine plsmin_impl( def, scale )
    real(kind=wp), intent(in) :: def, scale

    interface
        subroutine c_plsmin( def, scale ) bind(c,name='c_plsmin')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: def, scale
        end subroutine c_plsmin
    end interface

    call c_plsmin( real(def,kind=private_plflt), real(scale,kind=private_plflt) )

end subroutine plsmin_impl

subroutine plspage_impl( xp, yp, xleng, yleng, xoff, yoff )
    integer, intent(in) :: xleng, yleng, xoff, yoff
    real(kind=wp), intent(in) :: xp, yp

    interface
        subroutine c_plspage( xp, yp, xleng, yleng, xoff, yoff ) bind(c,name='c_plspage')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: xleng, yleng, xoff, yoff
            real(kind=private_plflt), value, intent(in) :: xp, yp
        end subroutine c_plspage
    end interface

    call c_plspage( real(xp,kind=private_plflt), real(yp,kind=private_plflt), &
         int(xleng,kind=private_plint), int(yleng,kind=private_plint), &
         int(xoff,kind=private_plint), int(yoff,kind=private_plint) )

end subroutine plspage_impl

subroutine plstring_impl( x, y, string )

   real(kind=wp), intent(in) :: x(:), y(:)
   character(len=*), intent(in) :: string

   interface
       subroutine c_plstring( n, x, y, string ) bind(c,name='c_plstring')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value :: n
           real(kind=private_plflt), dimension(*), intent(in) :: x, y
           character(len=1), dimension(*), intent(in) :: string
       end subroutine c_plstring
   end interface

   call c_plstring( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                    trim(string//c_null_char) )
end subroutine plstring_impl

subroutine plsurf3d_impl( x, y, z, opt, clevel )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z

    real(kind=private_plflt), dimension(:,:), allocatable :: zz
    type(c_ptr), dimension(:), allocatable :: zaddress

    interface
        subroutine c_plsurf3d( x, y, zaddress, nx, ny, opt, clevel, nlevel ) bind(c,name='c_plsurf3d')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: nx, ny, opt, nlevel
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine c_plsurf3d
    end interface

    call matrix_to_c( z, zz, zaddress )

    call c_plsurf3d( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress,  &
                     size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                     real(clevel,kind=private_plflt), size(clevel,kind=private_plint) )

end subroutine plsurf3d_impl

subroutine plsurf3dl_impl( x, y, z, opt, clevel, indexxmin, indexxmax, indexymin, indexymax )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z
    integer, intent(in) :: indexxmin, indexxmax
    integer, dimension(:), intent(in) :: indexymin, indexymax

    real(kind=private_plflt), dimension(:,:), allocatable :: zz
    type(c_ptr), dimension(:), allocatable :: zaddress

    interface
        subroutine c_plsurf3dl( x, y, zaddress, nx, ny, opt, clevel, nlevel, &
                                indexxmin, indexxmax, indexymin, indexymax ) bind(c,name='c_plsurf3dl')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: nx, ny, opt, nlevel, indexxmin, indexxmax
            integer(kind=private_plint), dimension(*), intent(in) :: indexymin, indexymax
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine c_plsurf3dl
    end interface

    call matrix_to_c( z, zz, zaddress )

    call c_plsurf3dl( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress, &
                      size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                      real(clevel,kind=private_plflt), size(clevel,kind=private_plint), &
                      int(indexxmin-1,kind=private_plint), int(indexxmax-1,kind=private_plint), &
                      int(indexymin-1,kind=private_plint), int(indexymax-1,kind=private_plint) )

end subroutine plsurf3dl_impl

subroutine plsvpa_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(in) :: xmin, xmax, ymin, ymax

    interface
        subroutine c_plsvpa( xmin, xmax, ymin, ymax ) bind(c,name='c_plsvpa')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: xmin, xmax, ymin, ymax
        end subroutine c_plsvpa
    end interface

    call c_plsvpa( real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), &
         real(ymin,kind=private_plflt), real(ymax,kind=private_plflt) )
end subroutine plsvpa_impl

subroutine plsym_impl( x, y, code )
    integer, intent(in) :: code
    real(kind=wp), dimension(:), intent(in) :: x, y

    interface
        subroutine c_plsym( n, x, y, code ) bind(c,name='c_plsym')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value :: n, code
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
        end subroutine c_plsym
    end interface

    call c_plsym( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                  int(code,kind=private_plint) )
end subroutine plsym_impl

subroutine plvasp_impl( aspect)
    real(kind=wp), intent(in) :: aspect

    interface
        subroutine c_plvasp( aspect ) bind(c,name='c_plvasp')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: aspect
        end subroutine c_plvasp
    end interface

    call c_plvasp( real(aspect,kind=private_plflt) )
end subroutine plvasp_impl

subroutine plvpas_impl( xmin, xmax, ymin, ymax, aspect )
    real(kind=wp), intent(in) :: xmin, xmax, ymin, ymax, aspect

    interface
        subroutine c_plvpas( xmin, xmax, ymin, ymax, aspect ) bind(c,name='c_plvpas')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: xmin, xmax, ymin, ymax, aspect
        end subroutine c_plvpas
    end interface

    call c_plvpas( real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), &
                   real(ymin,kind=private_plflt), real(ymax,kind=private_plflt), real(aspect,kind=private_plflt) )
end subroutine plvpas_impl

subroutine plvpor_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(in) :: xmin, xmax, ymin, ymax

    interface
        subroutine c_plvpor( xmin, xmax, ymin, ymax ) bind(c,name='c_plvpor')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: xmin, xmax, ymin, ymax
        end subroutine c_plvpor
    end interface

    call c_plvpor( real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), &
                   real(ymin,kind=private_plflt), real(ymax,kind=private_plflt)  )
end subroutine plvpor_impl

subroutine plw3d_impl( basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az )
    real(kind=wp), intent(in) :: basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az

    interface
        subroutine c_plw3d( basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az ) bind(c,name='c_plw3d')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az
        end subroutine c_plw3d
    end interface

    call c_plw3d( real(basex,kind=private_plflt), real(basey,kind=private_plflt), real(height,kind=private_plflt), &
                  real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), real(ymin,kind=private_plflt),     &
                  real(ymax,kind=private_plflt), real(zmin,kind=private_plflt), real(zmax,kind=private_plflt),     &
                  real(alt,kind=private_plflt), real(az,kind=private_plflt) )
end subroutine plw3d_impl

subroutine plwidth_impl( width )
    real(kind=wp), intent(in) :: width

    interface
        subroutine c_plwidth( width ) bind(c,name='c_plwidth')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: width
        end subroutine c_plwidth
    end interface

    call c_plwidth( real(width,kind=private_plflt) )
end subroutine plwidth_impl

subroutine plwind_impl( xmin, xmax, ymin, ymax )
    real(kind=wp) :: xmin, xmax, ymin, ymax

    interface
        subroutine c_plwind( xmin, xmax, ymin, ymax ) bind(c,name='c_plwind')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value :: xmin, xmax, ymin, ymax
        end subroutine c_plwind
    end interface

    call c_plwind( real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), &
                   real(ymin,kind=private_plflt), real(ymax,kind=private_plflt) )
end subroutine plwind_impl


