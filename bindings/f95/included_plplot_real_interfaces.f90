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
        module procedure plarc_impl
    end interface plarc
    private :: plarc_impl

    interface plaxes
        module procedure plaxes_impl
    end interface plaxes
    private :: plaxes_impl

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

    interface plbtime
        module procedure plbtime_impl
    end interface plbtime
    private :: plbtime_impl

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

    interface plconfigtime
        module procedure plconfigtime_impl
    end interface plconfigtime
    private :: plconfigtime_impl

    interface plcont
        module procedure plcontour_0
        module procedure plcontour_1
        module procedure plcontour_2
        module procedure plcontour_tr
    end interface plcont
    private :: plcontour_0
    private :: plcontour_1
    private :: plcontour_2
    private :: plcontour_tr

    interface plctime
        module procedure plctime_impl
    end interface plctime
    private :: plctime_impl

    interface plenv
        module procedure plenv_impl
    end interface plenv
    private :: plenv_impl

    interface plenv0
        module procedure plenv0_impl
    end interface plenv0
    private :: plenv0_impl

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

    interface plgriddata
        module procedure plgriddata_impl
    end interface plgriddata
    private :: plgriddata_impl

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

    interface plimage
       module procedure plimage_impl
    end interface plimage
    private :: plimage_impl

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

    interface plmtex3
        module procedure plmtex3_impl
    end interface plmtex3
    private :: plmtex3_impl

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

    interface plptex3
        module procedure plptex3_impl
    end interface plptex3
    private :: plptex3_impl

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

    interface plscolbga
        module procedure plscolbga_impl
    end interface plscolbga
    private :: plscolbga_impl

    interface plsdidev
        module procedure plsdidev_impl
    end interface plsdidev
    private :: plsdidev_impl

    interface plsdimap
        module procedure plsdimap_impl
    end interface plsdimap
    private :: plsdimap_impl

    interface plsdiori
        module procedure plsdiori_impl
    end interface plsdiori
    private :: plsdiori_impl

    interface plsdiplt
        module procedure plsdiplt_impl
    end interface plsdiplt
    private :: plsdiplt_impl

    interface plsdiplz
        module procedure plsdiplz_impl
    end interface plsdiplz
    private :: plsdiplz_impl

    interface plshade
       module procedure plshade_single_0
    end interface plshade
    private :: plshade_single_0

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

    interface plssym
        module procedure plssym_impl
    end interface plssym
    private :: plssym_impl

    interface plstring
        module procedure plstring_impl
    end interface plstring
    private :: plstring_impl

    interface plstring3
        module procedure plstring3_impl
    end interface plstring3
    private :: plstring3_impl

    interface plstripa
        module procedure plstripa_impl
    end interface plstripa
    private :: plstripa_impl

    interface plstripc
        module procedure plstripc_impl
    end interface plstripc
    private :: plstripc_impl

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

    integer :: i_local

    allocate( carray(size(array,2),size(array,1)) )
    allocate( caddress(size(array,1)) )

    carray = transpose( array )

    do i_local = 1,size(array,1)
        caddress(i_local) = c_loc(carray(1,i_local))
    enddo
end subroutine matrix_to_c

!subroutine plplot_private_pltr( x, y, tx, ty, pltr_data ) bind(c)
!   real(kind=private_plflt), value :: x, y
!   real(kind=private_plflt), intent(out) :: tx, ty
!   type(c_ptr), value :: pltr_data
!
!   real(kind=private_plflt), dimension(:), pointer :: tr
!
!   write(*,*) 'pltr:', x, y
!   call c_f_pointer( pltr_data, tr, [6] )
!   tx = tr(1) * x + tr(2) * y + tr(3)
!   ty = tr(4) * x + tr(5) * y + tr(6)
!end subroutine plplot_private_pltr

subroutine pl_setcontlabelparam_impl( offset, size, spacing, active )
   real(kind=wp), intent(in) :: offset, size, spacing
   integer, intent(in) :: active

   interface
       subroutine interface_pl_setcontlabelparam( offset, size, spacing, active) bind(c,name='c_pl_setcontlabelparam')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value, intent(in) :: active
           real(kind=private_plflt), value, intent(in) :: offset, size, spacing
       end subroutine interface_pl_setcontlabelparam
   end interface

   call interface_pl_setcontlabelparam( real(offset,kind=private_plflt), real(size,kind=private_plflt), &
        real(spacing,kind=private_plflt), int(active,kind=private_plint) )
end subroutine pl_setcontlabelparam_impl

subroutine plarc_impl( x, y, a, b, angle1, angle2, rotate, fill )
   real(kind=wp), intent(in) :: x, y, a, b, angle1, angle2, rotate
   logical, intent(in) :: fill

   interface
       subroutine interface_plarc( x, y, a, b, angle1, angle2, rotate, fill ) bind(c,name='c_plarc')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plbool), value, intent(in) :: fill
           real(kind=private_plflt), value, intent(in) :: x, y, a, b, angle1, angle2, rotate
       end subroutine interface_plarc
   end interface

   call interface_plarc( real(x,kind=private_plflt), real(y,kind=private_plflt), real(a,kind=private_plflt), &
                 real(b,kind=private_plflt), real(angle1,kind=private_plflt), real(angle2,kind=private_plflt), &
                 real(rotate,kind=private_plflt), int(merge(1,0,fill),kind=private_plbool) )
end subroutine plarc_impl

subroutine plaxes_impl(x0, y0, xopt,xtick,nxsub,yopt,ytick,nysub)
    real(kind=wp), intent(in) :: x0, y0, xtick, ytick
    integer, intent(in) :: nxsub, nysub
    character*(*), intent(in) :: xopt,yopt

    interface
        subroutine interface_plaxes(x0, y0, xopt,xtick,nxsub,yopt,ytick,nysub) bind(c,name='c_plaxes')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: x0, y0, xtick, ytick
            integer(kind=private_plint), value, intent(in) :: nxsub, nysub
            character(len=1), dimension(*), intent(in) :: xopt, yopt
        end subroutine interface_plaxes
    end interface

    call interface_plaxes( &
         real(x0,kind=private_plflt), real(y0,kind=private_plflt), &
         trim(xopt)//c_null_char, real(xtick,kind=private_plflt), int(nxsub,kind=private_plint), &
         trim(yopt)//c_null_char, real(ytick,kind=private_plflt), int(nysub,kind=private_plint) )
end subroutine plaxes_impl

subroutine plbin_impl( x, y, center )
   real(kind=wp), dimension(:), intent(in) :: x, y
   integer, intent(in) :: center

   interface
       subroutine interface_plbin( sz, x, y, center ) bind(c,name='c_plbin')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value, intent(in) :: sz, center
           real(kind=private_plflt), dimension(*), intent(in) :: x, y
       end subroutine interface_plbin
   end interface

   call interface_plbin( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                 int(center,kind=private_plint) )
end subroutine plbin_impl

subroutine plbox_impl(xopt,xtick,nxsub,yopt,ytick,nysub)
    real(kind=wp), intent(in) :: xtick, ytick
    integer, intent(in) :: nxsub, nysub
    character*(*), intent(in) :: xopt,yopt

    interface
        subroutine interface_plbox(xopt,xtick,nxsub,yopt,ytick,nysub) bind(c,name='c_plbox')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: xtick, ytick
            integer(kind=private_plint), value, intent(in) :: nxsub, nysub
            character(len=1), dimension(*), intent(in) :: xopt, yopt
        end subroutine interface_plbox
    end interface

    call interface_plbox( trim(xopt)//c_null_char, real(xtick,kind=private_plflt), int(nxsub,kind=private_plint), &
                  trim(yopt)//c_null_char, real(ytick,kind=private_plflt), int(nysub,kind=private_plint) )
end subroutine plbox_impl

subroutine plbox3_impl(xopt,xlabel,xtick,nxsub,yopt,ylabel,ytick,nysub, &
                       zopt,zlabel,ztick,nzsub)

    real(kind=wp), intent(in) :: xtick, ytick, ztick
    character*(*), intent(in) :: xopt, xlabel, yopt, ylabel, zopt, zlabel
    integer, intent(in) :: nxsub, nysub, nzsub

    interface
        subroutine interface_plbox3(xopt,xlabel,xtick,nxsub,yopt,ylabel,ytick,nysub, &
                            zopt,zlabel,ztick,nzsub) bind(c,name='c_plbox3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in):: xtick, ytick, ztick
            integer(kind=private_plint), value, intent(in) :: nxsub, nysub, nzsub
            character(len=1), dimension(*), intent(in) :: xopt, yopt, zopt, xlabel, ylabel, zlabel
        end subroutine interface_plbox3
    end interface

  call interface_plbox3( trim(xopt)//c_null_char, trim(xlabel)//c_null_char, real(xtick,kind=private_plflt), &
                 int(nxsub,kind=private_plint),                                                      &
                 trim(yopt)//c_null_char, trim(ylabel)//c_null_char, real(ytick,kind=private_plflt), &
                 int(nysub,kind=private_plint),                                                      &
                 trim(zopt)//c_null_char, trim(zlabel)//c_null_char, real(ztick,kind=private_plflt), &
                 int(nzsub,kind=private_plint) )
end subroutine plbox3_impl

subroutine plbtime_impl( year, month, day, hour, min, sec, ctime )
   real(kind=wp), intent(in) :: ctime
   integer, intent(out) :: year, month, day, hour, min
   real(kind=wp), intent(out) :: sec

   integer(kind=private_plint) :: year_out, month_out, day_out, hour_out, min_out
   real(kind=private_plflt) :: sec_out

   interface
       subroutine interface_plbtime( year, month, day, hour, min, sec, ctime ) bind(c,name='c_plbtime')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           real(kind=private_plflt), value, intent(in) :: ctime
           integer(kind=private_plint), intent(out) :: year, month, day, hour, min
           real(kind=private_plflt), intent(out) :: sec
       end subroutine interface_plbtime
   end interface

   call interface_plbtime( year_out, month_out, day_out, hour_out, min_out, sec_out, real(ctime, kind=private_plflt))
   year = int(year_out)
   month = int(month_out)
   day = int(day_out)
   hour = int(hour_out)
   min = int(min_out)
   sec = real(sec_out, kind=wp)

end subroutine plbtime_impl

subroutine plcalc_world_impl( rx, ry, wx, wy, window )
    integer, intent(out) :: window
    real(kind=wp), intent(in) :: rx, ry
    real(kind=wp), intent(out) :: wx, wy

    real(kind=private_plflt) :: wx_out, wy_out
    integer(kind=private_plint) window_out


    interface
        subroutine interface_plcalc_world( rx, ry, wx, wy, window ) bind(c,name='c_plcalc_world')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: window
            real(kind=private_plflt), value, intent(in) :: rx, ry
            real(kind=private_plflt), intent(out) :: wx, wy
        end subroutine interface_plcalc_world
    end interface

    call interface_plcalc_world( real(rx,kind=private_plflt), real(ry,kind=private_plflt), wx_out, wy_out, window_out )
    window = int(window_out)
    wx = real(wx_out, kind=wp)
    wy = real(wy_out, kind=wp)

end subroutine plcalc_world_impl

subroutine plcol1_impl( col )
    real(kind=wp), intent(in) :: col

    interface
        subroutine interface_plcol1( col ) bind(c,name='c_plcol1')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: col
        end subroutine interface_plcol1
    end interface

    call interface_plcol1( real(col,kind=private_plflt) )
end subroutine plcol1_impl

subroutine plcolorbar_impl( &
     colorbar_width, colorbar_height, &
     opt, position, x, y, &
     x_length, y_length, bg_color, bb_color, bb_style, &
     low_cap_color, high_cap_color, &
     cont_color, cont_width, &
     label_opts, labels, &
     axis_opts, ticks, sub_ticks, n_values, values )


    real(kind=wp), intent(in) :: x_length, y_length, x, y, low_cap_color, high_cap_color, cont_width
    real(kind=wp), dimension(0:, 0:), intent(in) :: values
    integer, intent(in) :: position, opt, bg_color, bb_color, bb_style, cont_color
    integer, dimension(0:), intent(in) :: label_opts, sub_ticks, n_values
    real(kind=wp), dimension(0:), intent(in) :: ticks
    character(len=*), dimension(0:), intent(in) :: labels, axis_opts
    real(kind=wp), intent(out) :: colorbar_width, colorbar_height

    integer :: n_labels_local, n_axes_local
    real(kind=private_plflt) :: colorbar_width_out, colorbar_height_out
    real(kind=private_plflt), dimension(:,:), allocatable :: values_c_local
    type(c_ptr), dimension(:), allocatable :: values_address_local

    interface
       subroutine interface_plcolorbar( &
            colorbar_width, colorbar_height, &
            opt, position, x, y, &
            x_length, y_length, bg_color, bb_color, bb_style, &
            low_cap_color, high_cap_color, &
            cont_color, cont_width, &
            n_labels, label_opts, length_labels, labels, &
            n_axes, length_axis_opts, axis_opts, ticks, sub_ticks, n_values, values ) &
            bind(c,name='fc_plcolorbar')

         use iso_c_binding, only: c_char, c_ptr
         implicit none
         include 'included_plplot_interface_private_types.f90'

         real(kind=private_plflt), value, intent(in) :: x_length, y_length, x, y, &
              low_cap_color, high_cap_color, cont_width
         integer(kind=private_plint), value, intent(in) :: position, opt, bg_color, bb_color, bb_style, cont_color
         integer(kind=private_plint), value, intent(in) :: n_labels, length_labels, n_axes, length_axis_opts
         real(kind=private_plflt), dimension(*), intent(in) :: ticks
         integer(kind=private_plint), dimension(*), intent(in) :: label_opts, sub_ticks, n_values
         type(c_ptr), dimension(*), intent(in) :: values

         ! These Fortran arguments require special processing done
         ! in fc_plcolorbar at the C level to interoperate properly
         ! with the C version of plcolorbar.
         character(c_char), intent(in) :: labels(length_labels, n_labels), axis_opts(length_axis_opts, n_axes)
         real(kind=private_plflt), intent(out) :: colorbar_width, colorbar_height

       end subroutine interface_plcolorbar
    end interface


    ! Determine number of label entries and demand consistent
    ! array sizes.

    n_labels_local = size(label_opts)
    if( n_labels_local /= size(labels) ) then
       write(0,*) "f95 plcolorbar ERROR: inconsistent array sizes not allowed for the following arrays:"
       write(0,*) "label_opts"
       write(0,*) "labels"
       return
    end if

    n_axes_local = size(axis_opts)
    if( &
         n_axes_local /= size(ticks) .or. &
         n_axes_local /= size(sub_ticks) .or. &
         n_axes_local /= size(n_values) .or. &
         n_axes_local /= size(values,1) &
         ) then
       write(0,*) "f95 plcolorbar ERROR: inconsistent array sizes not allowed for the following arrays:"
       write(0,*) "axis_opts"
       write(0,*) "ticks"
       write(0,*) "sub_ticks"
       write(0,*) "n_values"
       write(0,*) "first dimension of values"
       return
    end if

    if(maxval(n_values) > size(values,2) ) then
       write(0,*) "f95: plcolorbar ERROR: maximum of n_values > second dimension of values"
       return
    end if

    call matrix_to_c( values, values_c_local, values_address_local )

    call interface_plcolorbar( &
         colorbar_width_out, colorbar_height_out, &
         int(opt,kind=private_plint), int(position,kind=private_plint), &
         real(x,kind=private_plflt), real(y,kind=private_plflt), &
         real(x_length,kind=private_plflt), real(y_length,kind=private_plflt), &
         int(bg_color,kind=private_plint), &
         int(bb_color,kind=private_plint), int(bb_style,kind=private_plint), &
         real(low_cap_color,kind=private_plflt), real(high_cap_color,kind=private_plflt), &
         int(cont_color,kind=private_plint), real(cont_width,kind=private_plflt), &
         int(n_labels_local, kind=private_plint), int(label_opts, kind=private_plint), &
         len(labels(0), kind=private_plint), labels, &
         int(n_axes_local, kind=private_plint), len(axis_opts(0), kind=private_plint), &
         axis_opts, real(ticks, kind=private_plflt), int(sub_ticks, kind=private_plint), &
         int(n_values, kind=private_plint), values_address_local &
         )
    colorbar_width = real(colorbar_width_out, kind=wp)
    colorbar_height = real(colorbar_height_out, kind=wp)

end subroutine plcolorbar_impl

subroutine plconfigtime_impl( scale, offset1, offset2, ccontrol, ifbtime_offset, year, month, day, hour, min, sec )
   integer, intent(in) :: ccontrol, year, month, day, hour, min
   logical, intent(in) :: ifbtime_offset
   real(kind=wp), intent(in) :: scale, offset1, offset2, sec

   interface
      subroutine interface_plconfigtime( scale, offset1, offset2, ccontrol, ifbtime_offset, &
           year, month, day, hour, min, sec) bind(c,name='c_plconfigtime')
        implicit none
        include 'included_plplot_interface_private_types.f90'
        integer(kind=private_plint), value, intent(in) :: ccontrol, year, month, day, hour, min
        integer(kind=private_plbool), value, intent(in) :: ifbtime_offset
        real(kind=private_plflt), value, intent(in) :: scale, offset1, offset2, sec
      end subroutine interface_plconfigtime
   end interface

   call interface_plconfigtime( &
        real(scale, kind=private_plflt), real(offset1, kind=private_plflt), real(offset2, kind=private_plflt), &
        int(ccontrol, kind=private_plint), int(merge(1,0,ifbtime_offset), kind=private_plbool), &
        int(year, kind=private_plint), int(month, kind=private_plint), int(day, kind=private_plint), &
        int(hour, kind=private_plint), int(min, kind=private_plint), real(sec, kind=private_plflt) )
end subroutine plconfigtime_impl

subroutine plcontour_0( z, kx, lx, ky, ly, clevel )
    integer, intent(in) :: kx, lx, ky, ly
    real(kind=wp), dimension(:,:), intent(in) :: z
    real(kind=wp), dimension(:), intent(in) :: clevel

    real(kind=private_plflt), dimension(:,:), allocatable, target :: z_in
    type(PLfGrid), target :: fgrid

    interface
        function plf2evalr( ix, iy, data ) bind(c, name = 'plf2evalr' )
            use iso_c_binding
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt) :: plf2evalr
            integer(kind=private_plint), value, intent(in) :: ix, iy
            type(c_ptr), value, intent(in) :: data
        end function plf2evalr
    end interface

    interface
        subroutine interface_plfcont( lookup, grid, nx, ny, kx, lx, ky, ly, clevel, nlevel, transform, data ) bind(c,name='plfcont')
            use iso_c_binding
            use plplot_types, only: private_plint, private_plflt, PLfGrid
            implicit none
            integer(kind=private_plint), value, intent(in) :: nx, ny, kx, lx, ky, ly, nlevel
            type(c_ptr), value, intent(in)  :: grid
            real(kind=private_plflt), dimension(*), intent(in) :: clevel
            type(c_ptr), intent(in) :: data
            type(c_funptr) :: lookup
            type(c_funptr) :: transform
        end subroutine interface_plfcont
    end interface

    allocate( z_in(size(z,1),size(z,2)) )
    z_in = z
    fgrid%f  = c_loc(z_in)
    fgrid%nx = size(z,1)
    fgrid%ny = size(z,2)

    call interface_plfcont( c_funloc(plf2evalr), c_loc(fgrid), size(z,1,kind=private_plint), size(z,2,kind=private_plint), &
                  kx, lx, ky, ly, real(clevel, kind=private_plflt), size(clevel,kind=private_plint), &
                  c_null_funptr, c_null_ptr )
end subroutine plcontour_0

subroutine plcontour_1( z, kx, lx, ky, ly, clevel, xg, yg )
    integer, intent(in) :: kx, lx, ky, ly
    real(kind=wp), dimension(:,:), intent(in) :: z
    real(kind=wp), dimension(:), intent(in) :: clevel, xg, yg

    real(kind=private_plflt), dimension(:,:), allocatable, target :: z_in
    real(kind=private_plflt), dimension(:), allocatable, target :: xg_in, yg_in
    type(PLfGrid), target :: fgrid
    type(PLcGrid), target :: cgrid

    interface
        function plf2evalr( ix, iy, data ) bind(c, name = 'plf2evalr' )
            use iso_c_binding
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt) :: plf2evalr
            integer(kind=private_plint), value, intent(in) :: ix, iy
            type(c_ptr), value, intent(in)  :: data
        end function plf2evalr
    end interface

    interface
        subroutine pltr1( x, y, tx, ty, data ) bind(c, name = 'pltr1' )
            use iso_c_binding
            use plplot_types, only: private_plflt, PLcGrid
            implicit none
            real(kind=private_plflt), value, intent(in) :: x, y
            real(kind=private_plflt), intent(out) :: tx, ty
            type(PLcGrid), intent(in) :: data
        end subroutine pltr1
    end interface

    interface
        subroutine interface_plfcont( lookup, grid, nx, ny, kx, lx, ky, ly, clevel, nlevel, transform, data ) bind(c,name='plfcont')
            use iso_c_binding
            use plplot_types, only: private_plint, private_plflt, PLfGrid
            implicit none
            integer(kind=private_plint), value, intent(in) :: nx, ny, kx, lx, ky, ly, nlevel
            type(c_ptr), value, intent(in)  :: grid
            real(kind=private_plflt), dimension(*), intent(in) :: clevel
            type(c_ptr), value, intent(in)  :: data
            interface
                function lookup( ix, iy, data ) bind(c)
                    use iso_c_binding
                    implicit none
                    include 'included_plplot_interface_private_types.f90'
                    real(kind=private_plflt) :: lookup
                    integer(kind=private_plint), value, intent(in)  :: ix, iy
                    type(c_ptr), value, intent(in)  :: data
                end function lookup
            end interface
            interface
                subroutine transform( x, y, tx, ty, data ) bind(c)
                    use iso_c_binding
                    use plplot_types, only: private_plflt, PLcGrid
                    implicit none
                    real(kind=private_plflt), value, intent(in)  :: x, y
                    real(kind=private_plflt), intent(out) :: tx, ty
                    type(PLcGrid), intent(in) :: data
                end subroutine transform
            end interface
        end subroutine interface_plfcont
    end interface

    allocate( z_in(size(z,1),size(z,2)) )
    z_in = z
    fgrid%f  = c_loc(z_in)
    fgrid%nx = size(z,1)
    fgrid%ny = size(z,2)

    allocate( xg_in(size(z,1)), yg_in(size(z,2)) )
    xg_in = xg
    yg_in = yg
    cgrid%nx = size(z,1)
    cgrid%ny = size(z,2)
    cgrid%xg = c_loc(xg_in)
    cgrid%yg = c_loc(yg_in)

    call interface_plfcont( plf2evalr, c_loc(fgrid), size(z,1,kind=private_plint), size(z,2,kind=private_plint), &
                  kx, lx, ky, ly, real(clevel, kind=private_plflt), size(clevel,kind=private_plint), &
                  pltr1, c_loc(cgrid) )
end subroutine plcontour_1

subroutine plcontour_2( z, kx, lx, ky, ly, clevel, xg, yg )
    integer, intent(in) :: kx, lx, ky, ly
    real(kind=wp), dimension(:,:), intent(in) :: z
    real(kind=wp), dimension(:), intent(in) :: clevel
    real(kind=wp), dimension(:,:), intent(in) :: xg, yg

    real(kind=private_plflt), dimension(:,:), allocatable, target :: z_in
    real(kind=private_plflt), dimension(:,:), allocatable, target :: xg_in, yg_in
    type(PLfGrid), target :: fgrid
    type(PLcGrid), target :: cgrid

    interface
        function plf2evalr( ix, iy, data ) bind(c, name = 'plf2evalr' )
            use iso_c_binding
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt) :: plf2evalr
            integer(kind=private_plint), value, intent(in)  :: ix, iy
            type(c_ptr), value, intent(in)  :: data
        end function plf2evalr
    end interface

    interface
        subroutine pltr2f( x, y, tx, ty, data ) bind(c, name = 'pltr2f' )
            use iso_c_binding
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in)  :: x, y
            real(kind=private_plflt), intent(out) :: tx, ty
            type(c_ptr), value, intent(in)  :: data
        end subroutine pltr2f
    end interface

    interface
        subroutine interface_plfcont( lookup, grid, nx, ny, kx, lx, ky, ly, clevel, nlevel, transform, data ) bind(c,name='plfcont')
            use iso_c_binding
            use plplot_types, only: private_plint, private_plflt, PLfGrid
            implicit none
            integer(kind=private_plint), value, intent(in) :: nx, ny, kx, lx, ky, ly, nlevel
            type(c_ptr), value, intent(in)  :: grid
            real(kind=private_plflt), dimension(*), intent(in) :: clevel
            type(c_ptr), value, intent(in)  :: data
            interface
                function lookup( ix, iy, data ) bind(c)
                    use iso_c_binding
                    implicit none
                    include 'included_plplot_interface_private_types.f90'
                    real(kind=private_plflt) :: lookup
                    integer(kind=private_plint), value, intent(in)  :: ix, iy
                    type(c_ptr), value, intent(in)  :: data
                end function lookup
            end interface

            interface
                subroutine transform( x, y, tx, ty, data ) bind(c)
                    use iso_c_binding
                    implicit none
                    include 'included_plplot_interface_private_types.f90'
                    real(kind=private_plflt), value, intent(in)  :: x, y
                    real(kind=private_plflt), intent(out) :: tx, ty
                    type(c_ptr), value, intent(in)  :: data
                end subroutine transform
            end interface
        end subroutine interface_plfcont
    end interface

    allocate( z_in(size(z,1),size(z,2)) )
    z_in = z
    fgrid%f  = c_loc(z_in)
    fgrid%nx = size(z,1)
    fgrid%ny = size(z,2)

    allocate( xg_in(size(z,1),size(z,2)), yg_in(size(z,1),size(z,2)) )
    xg_in = xg
    yg_in = yg
    cgrid%nx = size(z,1)
    cgrid%ny = size(z,2)
    cgrid%xg = c_loc(xg_in)
    cgrid%yg = c_loc(yg_in)

    call interface_plfcont( plf2evalr, c_loc(fgrid), size(z,1,kind=private_plint), size(z,2,kind=private_plint), &
                  kx, lx, ky, ly, real(clevel, kind=private_plflt), size(clevel,kind=private_plint), &
                  pltr2f, c_loc(cgrid) )
end subroutine plcontour_2

subroutine plcontour_tr( z, kx, lx, ky, ly, clevel, tr )
    integer, intent(in) :: kx, lx, ky, ly
    real(kind=wp), dimension(:,:), intent(in) :: z
    real(kind=wp), dimension(:), intent(in) :: clevel
    real(kind=wp), dimension(:), intent(in) :: tr

    real(kind=private_plflt), dimension(6), target :: tr_in
    real(kind=private_plflt), dimension(:,:), allocatable, target :: z_in
    type(PLfGrid), target :: fgrid

    interface
        function plf2evalr( ix, iy, data ) bind(c, name = 'plf2evalr' )
            use iso_c_binding
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt) :: plf2evalr
            integer(kind=private_plint), value, intent(in) :: ix, iy
            type(c_ptr), value, intent(in)  :: data
        end function plf2evalr
    end interface

   ! interface
   !     subroutine pltr( x, y, tx, ty, data ) bind(c, name = 'pltr' )
   !         use iso_c_binding
   !         implicit none
   !         include 'included_plplot_interface_private_types.f90'
   !         real(kind=private_plflt), value :: x, y
   !         real(kind=private_plflt), intent(out) :: tx, ty
   !         type(c_ptr), value :: data
   !     end subroutine pltr
   ! end interface

    interface
        subroutine interface_plfcont( lookup, grid, nx, ny, kx, lx, ky, ly, clevel, nlevel, transform, data ) bind(c,name='plfcont')
            use iso_c_binding
            use plplot_types, only: private_plint, private_plflt, PLfGrid
            implicit none
            integer(kind=private_plint), value, intent(in) :: nx, ny, kx, lx, ky, ly, nlevel
            type(c_ptr), value, intent(in)  :: grid
            real(kind=private_plflt), dimension(*), intent(in) :: clevel
            real(kind=private_plflt), dimension(*), intent(in) :: data
            interface
                function lookup( ix, iy, data ) bind(c)
                    use iso_c_binding
                    implicit none
                    include 'included_plplot_interface_private_types.f90'
                    real(kind=private_plflt) :: lookup
                    integer(kind=private_plint), value, intent(in)  :: ix, iy
                    type(c_ptr), value, intent(in)  :: data
                end function lookup
            end interface
            interface
                subroutine transform( x, y, tx, ty, data ) bind(c)
                    use iso_c_binding
                    implicit none
                    include 'included_plplot_interface_private_types.f90'
                    real(kind=private_plflt), value, intent(in)  :: x, y
                    real(kind=private_plflt), intent(out) :: tx, ty
                    real(kind=private_plflt), dimension(*), intent(in) :: data
                    !type(c_ptr), value :: data
                end subroutine transform
            end interface
        end subroutine interface_plfcont
    end interface

    allocate( z_in(size(z,1),size(z,2)) )
    z_in = z
    fgrid%f  = c_loc(z_in)
    fgrid%nx = size(z,1)
    fgrid%ny = size(z,2)

    tr_in = tr(1:6)

    call interface_plfcont( plf2evalr, c_loc(fgrid), size(z,1,kind=private_plint), size(z,2,kind=private_plint), &
                  kx, lx, ky, ly, real(clevel, kind=private_plflt), size(clevel,kind=private_plint), &
                  plplot_private_pltr, tr_in )
end subroutine plcontour_tr

subroutine plctime_impl( year, month, day, hour, min, sec, ctime )
   integer, intent(in) :: year, month, day, hour, min
   real(kind=wp), intent(in) :: sec
   real(kind=wp), intent(out) :: ctime

   real(kind=private_plflt) :: ctime_out

   interface
       subroutine interface_plctime( year, month, day, hour, min, sec, ctime ) bind(c,name='c_plctime')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value, intent(in) :: year, month, day, hour, min
           real(kind=private_plflt), value, intent(in) :: sec
           real(kind=private_plflt), intent(out) :: ctime
       end subroutine interface_plctime
   end interface

   call interface_plctime( &
        int(year, kind=private_plint), int(month, kind=private_plint), int(day, kind=private_plint), &
        int(hour, kind=private_plint), int(min, kind=private_plint), real(sec, kind=private_plflt), ctime_out )
   ! Transform output real values.
   ctime = real(ctime_out, kind=wp)

end subroutine plctime_impl

subroutine plenv_impl( xmin, xmax, ymin, ymax, just, axis )
    real(kind=wp), intent(in)  :: xmin, xmax, ymin, ymax
    integer, intent(in) :: just, axis

    interface
        subroutine interface_plenv( xmin, xmax, ymin, ymax, just, axis ) bind(c, name='c_plenv')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: xmin, xmax, ymin, ymax
            integer(kind=private_plint), value, intent(in) :: just, axis
        end subroutine interface_plenv
    end interface

    call interface_plenv( real(xmin,private_plflt), real(xmax,private_plflt), real(ymin,private_plflt), real(ymax,private_plflt), &
                  int(just,private_plint), int(axis,private_plint) )
end subroutine plenv_impl

subroutine plenv0_impl( xmin, xmax, ymin, ymax, just, axis )
    real(kind=wp), intent(in)  :: xmin, xmax, ymin, ymax
    integer, intent(in) :: just, axis

    interface
        subroutine interface_plenv0( xmin, xmax, ymin, ymax, just, axis ) bind(c, name='c_plenv0')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: xmin, xmax, ymin, ymax
            integer(kind=private_plint), value, intent(in) :: just, axis
        end subroutine interface_plenv0
    end interface

    call interface_plenv0( real(xmin,private_plflt), real(xmax,private_plflt), real(ymin,private_plflt), real(ymax,private_plflt), &
                  int(just,private_plint), int(axis,private_plint) )
end subroutine plenv0_impl

subroutine plerrx_impl( xmin, xmax, y )
    real(kind=wp), dimension(:), intent(in) :: xmin, xmax, y

    integer(kind=private_plint) n

    interface
        subroutine interface_plerrx( n, xmin, xmax, y ) bind( c, name='c_plerrx')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
            real(kind=private_plflt), dimension(*), intent(in) :: xmin, xmax, y
        end subroutine interface_plerrx
    end interface

    n = size(y,kind=private_plint)
    if( n /= size(xmin, kind=private_plint) .or. n /= size(xmax, kind=private_plint) ) then
       write(0,*) "f95 plerrx ERROR: inconsistent sizes for xmin, xmax, and/or y"
       return
    end if

    call interface_plerrx( n, real(xmin,private_plflt), real(xmax,private_plflt), real(y,private_plflt) )
end subroutine plerrx_impl

subroutine plerry_impl( x, ymin, ymax )
    real(kind=wp), dimension(:), intent(in) :: x, ymin, ymax

    integer(kind=private_plint) n

    interface
        subroutine interface_plerry( n, x, ymin, ymax ) bind( c, name='c_plerry')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
            real(kind=private_plflt), dimension(*), intent(in) :: x, ymin, ymax
        end subroutine interface_plerry
    end interface

    n = size(x,kind=private_plint)
    if( n /= size(ymin, kind=private_plint) .or. n /= size(ymax, kind=private_plint) ) then
       write(0,*) "f95 plerry ERROR: inconsistent sizes for x, ymin, and/or ymax"
       return
    end if

    call interface_plerry( n, real(x,private_plflt), real(ymin,private_plflt), real(ymax,private_plflt) )
end subroutine plerry_impl

subroutine plfill_impl( x, y )
    real(kind=wp), dimension(:), intent(in) :: x, y

    interface
        subroutine interface_plfill( n, x, y ) bind( c, name='c_plfill')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
        end subroutine interface_plfill
    end interface

    call interface_plfill( size(x,kind=private_plint), real(x,private_plflt), real(y,private_plflt) )
end subroutine plfill_impl

subroutine plfill3_impl( x, y, z )
    real(kind=wp), dimension(:), intent(in) :: x, y, z

    interface
        subroutine interface_plfill3( n, x, y, z ) bind( c, name='c_plfill3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, z
        end subroutine interface_plfill3
    end interface

    call interface_plfill3( size(x,kind=private_plint), real(x,private_plflt), real(y,private_plflt) , real(z,private_plflt) )
end subroutine plfill3_impl

subroutine plgchr_impl( chrdef, chrht )
    real(kind=wp), intent(out) :: chrdef, chrht

    real(kind=private_plflt) :: chrdef_out, chrht_out

    interface
       subroutine interface_plgchr( chrdef, chrht ) bind(c,name='c_plgchr')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           real(kind=private_plflt), intent(out) :: chrdef, chrht
       end subroutine interface_plgchr
   end interface

   call interface_plgchr( chrdef_out, chrht_out )
   chrdef = real(chrdef_out, kind=wp)
   chrht = real(chrht_out, kind=wp)

end subroutine plgchr_impl

subroutine plgcmap1_range_impl( min_color, max_color )
    real(kind=wp), intent(out) :: min_color, max_color

    real(kind=private_plflt) :: min_color_out, max_color_out

    interface
        subroutine interface_plgcmap1_range( min_color, max_color ) bind(c,name='c_plgcmap1_range')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: min_color, max_color
        end subroutine interface_plgcmap1_range
    end interface

    call interface_plgcmap1_range( min_color_out, max_color_out )
    min_color = real(min_color_out, kind=wp)
    max_color = real(max_color_out, kind=wp)

end subroutine plgcmap1_range_impl

subroutine plgcol0a_impl( icol, r, g, b, a )
    integer, intent(in) :: icol
    integer, intent(out) :: r, g, b
    real(kind=wp), intent(out) :: a

    integer(kind=private_plint) :: r_out, g_out, b_out
    real(kind=private_plflt) :: a_out

    interface
        subroutine interface_plgcol0a( icol, r, g, b, a ) bind(c,name='c_plgcol0a')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: icol
            integer(kind=private_plint), intent(out) :: r, g, b
            real(kind=private_plflt), intent(out) :: a
        end subroutine interface_plgcol0a
    end interface

    call interface_plgcol0a( int(icol,kind=private_plint), r_out, g_out, b_out, a_out )
    r = int(r_out)
    g = int(g_out)
    b = int(b_out)
    a = real(a_out, kind=private_plflt)
end subroutine plgcol0a_impl

subroutine plgcolbga_impl( r, g, b, a )
    integer, intent(out) :: r, g, b
    real(kind=wp), intent(out) :: a

    integer(kind=private_plint) :: r_out, g_out, b_out
    real(kind=private_plflt) :: a_out

    interface
        subroutine interface_plgcolbga( r, g, b, a ) bind(c,name='c_plgcolbga')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: r, g, b
            real(kind=private_plflt), intent(out) :: a
        end subroutine interface_plgcolbga
    end interface

    call interface_plgcolbga( r_out, g_out, b_out, a_out )
    r = int(r_out)
    g = int(g_out)
    b = int(b_out)
    a = real(a_out, kind=private_plflt)
end subroutine plgcolbga_impl

subroutine plgdidev_impl( mar, aspect, jx, jy )
    real(kind=wp), intent(out) :: mar, aspect, jx, jy

    real(kind=private_plflt) :: mar_out, aspect_out, jx_out, jy_out

    interface
        subroutine interface_plgdidev( mar, aspect, jx, jy ) bind(c,name='c_plgdidev')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: mar, aspect, jx, jy
        end subroutine interface_plgdidev
    end interface

    call interface_plgdidev( mar_out, aspect_out, jx_out, jy_out )
    mar    = real(mar_out, kind=wp)
    aspect = real(aspect_out, kind=wp)
    jx     = real(jx_out, kind=wp)
    jy     = real(jy_out, kind=wp)
end subroutine plgdidev_impl

subroutine plgdiori_impl( rot )
    real(kind=wp), intent(out) :: rot

    real(kind=private_plflt) :: rot_out

    interface
        subroutine interface_plgdiori( rot ) bind(c,name='c_plgdiori')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: rot
        end subroutine interface_plgdiori
    end interface

    call interface_plgdiori( rot_out )
    rot = real(rot_out, kind=wp)
end subroutine plgdiori_impl

subroutine plgdiplt_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(out) :: xmin, xmax, ymin, ymax

    real(kind=private_plflt)  :: xmin_out, xmax_out, ymin_out, ymax_out

    interface
        subroutine interface_plgdiplt( xmin, xmax, ymin, ymax ) bind(c,name='c_plgdiplt')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: xmin, xmax, ymin, ymax
        end subroutine interface_plgdiplt
    end interface

    call interface_plgdiplt( xmin_out, xmax_out, ymin_out, ymax_out )
    xmin = real(xmin_out, kind=wp)
    xmax = real(xmax_out, kind=wp)
    ymin = real(ymin_out, kind=wp)
    ymax = real(ymax_out, kind=wp)
end subroutine plgdiplt_impl

subroutine plgpage_impl( xpmm, ypmm, xwid, ywid, xoff, yoff )
    integer, intent(out) :: xwid, ywid, xoff, yoff
    real(kind=wp), intent(out) :: xpmm, ypmm

    integer(kind=private_plint) :: xwid_out, ywid_out, xoff_out, yoff_out
    real(kind=private_plflt) :: xpmm_out, ypmm_out

    interface
        subroutine interface_plgpage( xpmm, ypmm, xwid, ywid, xoff, yoff ) bind(c,name='c_plgpage')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), intent(out) :: xwid, ywid, xoff, yoff
            real(kind=private_plflt), intent(out) :: xpmm, ypmm
        end subroutine interface_plgpage
    end interface

    call interface_plgpage( xpmm_out, ypmm_out, xwid_out, ywid_out, xoff_out, yoff_out )
    xwid = int(xwid_out)
    ywid = int(ywid_out)
    xoff = int(xoff_out)
    yoff = int(yoff_out)
    xpmm = real(xpmm_out, kind=wp)
    ypmm = real(ypmm_out, kind=wp)
 end subroutine plgpage_impl

subroutine plgradient_impl( x, y, angle )
    real(kind=wp), dimension(:), intent(in) :: x, y
    real(kind=wp), intent(in) :: angle

    interface
       subroutine interface_plgradient( sz, x, y, angle ) bind(c,name='c_plgradient')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value, intent(in) :: sz
           real(kind=private_plflt), dimension(*), intent(in) :: x, y
           real(kind=private_plflt), value, intent(in) :: angle
       end subroutine interface_plgradient
    end interface

    call interface_plgradient( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                       real(angle,kind=private_plflt) )
end subroutine plgradient_impl

subroutine plgriddata_impl( x, y, z, xg, yg, zg, type, data )
    integer, intent(in) :: type
    real(kind=wp), intent(in) :: data
    real(kind=wp), dimension(:), intent(in) :: x, y, z, xg, yg
    real(kind=wp), dimension(:, :), intent(out) :: zg

    real(kind=private_plflt), dimension(:,:), allocatable, target :: transpose_local
    type(c_ptr), dimension(:), allocatable :: transpose_address_local
    integer(kind=private_plint) :: npts_local, nptsx_local, nptsy_local
    integer :: i_local

    interface
       subroutine interface_plgriddata( x, y, z, npts, xg, nptsx, yg, nptsy, zg, type, data ) bind(c,name='c_plgriddata')
           use iso_c_binding, only: c_ptr
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value, intent(in) :: npts, nptsx, nptsy, type
           real(kind=private_plflt), value, intent(in) :: data
           real(kind=private_plflt), dimension(*), intent(in) :: x, y, z, xg, yg
           type(c_ptr), dimension(*), intent(out) :: zg
       end subroutine interface_plgriddata
    end interface

    npts_local = size(x, kind=private_plint)

    if( &
         npts_local /= size(y, kind=private_plint) .or. &
         npts_local /= size(z, kind=private_plint) ) then
       write(0,*) "f95 plgriddata ERROR: inconsistent sizes for x, y, and/or z"
       return
    end if

    nptsx_local = size(xg, kind=private_plint)
    nptsy_local = size(yg, kind=private_plint)

    if( &
         nptsx_local /= size(zg, 1, kind=private_plint) .or. &
         nptsy_local /= size(zg, 2, kind=private_plint) ) then
       write(0,*) "f95 plgriddata ERROR: inconsistent sizes for xg and first dimension of zg or yg and second dimension of zg"
       return
    end if

    ! Prepare array areas to be written to by C version of plgriddata
    ! following relevant parts of code in matrix_to_c.
    allocate( transpose_local(nptsy_local, nptsx_local) )
    allocate( transpose_address_local(nptsx_local) )
    do i_local = 1, nptsx_local
       transpose_address_local(i_local) = c_loc(transpose_local(1,i_local))
    enddo

    call interface_plgriddata( &
         real(x,kind=private_plflt), real(y,kind=private_plflt), real(z,kind=private_plflt), npts_local, &
         real(xg,kind=private_plflt), nptsx_local, real(yg,kind=private_plflt), nptsy_local, &
         transpose_address_local, int(type, kind=private_plint), real(data, kind=private_plflt) )

    zg = real(transpose(transpose_local), kind=wp)
    deallocate(transpose_local, transpose_address_local)
end subroutine plgriddata_impl

subroutine plgspa_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(out) :: xmin, xmax, ymin, ymax

    real(kind=private_plflt) :: xmin_out, xmax_out, ymin_out, ymax_out

    interface
        subroutine interface_plgspa( xmin, xmax, ymin, ymax ) bind(c,name='c_plgspa')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: xmin, xmax, ymin, ymax
        end subroutine interface_plgspa
    end interface

    call interface_plgspa( xmin_out, xmax_out, ymin_out, ymax_out )
    xmin = real(xmin_out, kind=private_plflt)
    xmax = real(xmax_out, kind=private_plflt)
    ymin = real(ymin_out, kind=private_plflt)
    ymax = real(ymax_out, kind=private_plflt)
end subroutine plgspa_impl

subroutine plgvpd_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(out) :: xmin, xmax, ymin, ymax

    real(kind=private_plflt) :: xmin_out, xmax_out, ymin_out, ymax_out

    interface
        subroutine interface_plgvpd( xmin, xmax, ymin, ymax ) bind(c,name='c_plgvpd')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: xmin, xmax, ymin, ymax
        end subroutine interface_plgvpd
    end interface

    call interface_plgvpd( xmin_out, xmax_out, ymin_out, ymax_out )
    xmin = real(xmin_out, kind=private_plflt)
    xmax = real(xmax_out, kind=private_plflt)
    ymin = real(ymin_out, kind=private_plflt)
    ymax = real(ymax_out, kind=private_plflt)
end subroutine plgvpd_impl

subroutine plgvpw_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(out) :: xmin, xmax, ymin, ymax

    real(kind=private_plflt) :: xmin_out, xmax_out, ymin_out, ymax_out
    interface
        subroutine interface_plgvpw( xmin, xmax, ymin, ymax ) bind(c,name='c_plgvpw')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), intent(out) :: xmin, xmax, ymin, ymax
        end subroutine interface_plgvpw
    end interface

    call interface_plgvpw( xmin_out, xmax_out, ymin_out, ymax_out )
    xmin = real(xmin_out, kind=private_plflt)
    xmax = real(xmax_out, kind=private_plflt)
    ymin = real(ymin_out, kind=private_plflt)
    ymax = real(ymax_out, kind=private_plflt)
end subroutine plgvpw_impl

subroutine plhist_impl( data, datmin, datmax, nbin, oldwin )
    real(kind=wp), dimension(:), intent(in) :: data
    real(kind=wp), intent(in) :: datmin, datmax
    integer, intent(in) :: nbin, oldwin

    interface
        subroutine interface_plhist( n, data, datmin, datmax, nbin, oldwin ) bind(c,name='c_plhist')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), dimension(*), intent(in) :: data
            real(kind=private_plflt), value, intent(in) :: datmin, datmax
            integer(kind=private_plint), value, intent(in) :: n, nbin, oldwin
        end subroutine interface_plhist
    end interface

   call interface_plhist( size(data,kind=private_plint), real(data,kind=private_plflt),     &
                  real(datmin,kind=private_plflt), real(datmax,kind=private_plflt), &
                  int(nbin,kind=private_plint), int(oldwin,kind=private_plint) )
end subroutine plhist_impl

subroutine plhlsrgb_impl( h, l, s, r, g, b )
    real(kind=wp), intent(in) :: h, l, s
    real(kind=wp), intent(out) :: r, g, b

    real(kind=private_plflt) :: r_out, g_out, b_out

    interface
        subroutine interface_plhlsrgb( h, l, s, r, g, b ) bind(c,name='c_plhlsrgb')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: h, l, s
            real(kind=private_plflt), intent(out) :: r, g, b
        end subroutine interface_plhlsrgb
    end interface

    call interface_plhlsrgb( real(h,kind=private_plflt), real(l,kind=private_plflt), real(s,kind=private_plflt), &
         r_out, g_out, b_out )
    r = real(r_out, kind=wp)
    g = real(g_out, kind=wp)
    b = real(b_out, kind=wp)
end subroutine plhlsrgb_impl

subroutine plimage_impl( idata, xmin, xmax, ymin, ymax, zmin, zmax, Dxmin, Dxmax, Dymin, Dymax )
    real(kind=wp), intent(in) :: xmin, xmax, ymin, ymax, zmin, zmax, Dxmin, Dxmax, Dymin, Dymax
    real(kind=wp), dimension(:, :), intent(in) :: idata

    real(kind=private_plflt), dimension(:,:), allocatable :: idata_local
    type(c_ptr), dimension(:), allocatable :: idata_address_local

    interface
       subroutine interface_plimage( idata, nx, ny, &
            xmin, xmax, ymin, ymax, &
            zmin, zmax, Dxmin, Dxmax, Dymin, Dymax ) bind(c,name='c_plimage')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny
            real(kind=private_plflt), value, intent(in) :: xmin, xmax, ymin, ymax, zmin, zmax, Dxmin, Dxmax, Dymin, Dymax
            type(c_ptr), dimension(*), intent(in) :: idata
        end subroutine interface_plimage
     end interface

     call matrix_to_c( idata, idata_local, idata_address_local )

     call interface_plimage( &
          idata_address_local, size(idata, 1, kind=private_plint), size(idata, 2, kind=private_plint), &
          real(xmin, kind=private_plflt), real(xmax, kind=private_plflt), &
          real(ymin, kind=private_plflt), real(ymax, kind=private_plflt), &
          real(zmin, kind=private_plflt), real(zmax, kind=private_plflt), &
          real(Dxmin, kind=private_plflt), real(Dxmax, kind=private_plflt), &
          real(Dymin, kind=private_plflt), real(Dymax, kind=private_plflt) &
          )
end subroutine plimage_impl

subroutine pljoin_impl( x1, y1, x2, y2 )
    real(kind=wp), intent(in) :: x1, y1, x2, y2

    interface
        subroutine interface_pljoin( x1, y1, x2, y2 ) bind(c,name='c_pljoin')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: x1, y1, x2, y2
        end subroutine interface_pljoin
    end interface

    call interface_pljoin( real(x1,kind=private_plflt), real(y1,kind=private_plflt), &
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

    real(kind=wp), intent(in) :: plot_width, x, y
    real(kind=wp), intent(in) :: text_offset, text_scale, text_spacing, text_justification
    integer, intent(in) :: position, opt, bg_color, bb_color, bb_style
    integer, intent(in) :: nrow, ncolumn
    real(kind=wp), intent(out) :: legend_width, legend_height

    character(len=*), dimension(0:), intent(in) :: text, symbols

    integer, dimension(0:), intent(in) :: opt_array, text_colors, box_colors
    integer, dimension(0:), intent(in) :: box_patterns
    real(kind=wp), dimension(0:), intent(in) :: box_line_widths
    integer, dimension(0:), intent(in) :: line_colors, line_styles
    real(kind=wp), dimension(0:), intent(in) :: line_widths
    integer, dimension(0:), intent(in) :: symbol_colors, symbol_numbers
    real(kind=wp), dimension(0:), intent(in) :: box_scales, symbol_scales

    integer(kind=private_plint) :: nlegend_local
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

         real(kind=private_plflt), value, intent(in) :: plot_width, x, y
         real(kind=private_plflt), value, intent(in) :: text_offset, text_scale, text_spacing, text_justification
         integer(kind=private_plint), value, intent(in) :: position, opt, bg_color, bb_color, bb_style
         integer(kind=private_plint), value, intent(in) :: nrow, ncolumn, nlegend, length_text, length_symbols

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
         real(kind=private_plflt), intent(out) :: legend_width, legend_height
       end subroutine interface_pllegend
    end interface


    ! Determine number of legend entries and demand consistent
    ! array sizes.

    nlegend_local = size(opt_array)
    if( &
         nlegend_local /= size(text_colors) .or. &
         nlegend_local /= size(text) .or. &
         nlegend_local /= size(box_colors) .or. &
         nlegend_local /= size(box_patterns) .or. &
         nlegend_local /= size(box_scales) .or. &
         nlegend_local /= size(box_line_widths) .or. &
         nlegend_local /= size(line_colors) .or. &
         nlegend_local /= size(line_styles) .or. &
         nlegend_local /= size(line_widths) .or. &
         nlegend_local /= size(symbol_colors) .or. &
         nlegend_local /= size(symbol_scales) .or. &
         nlegend_local /= size(symbol_numbers) .or. &
         nlegend_local /= size(symbols) &
         ) then
       write(0,*) "f95 pllegend ERROR: inconsistent array sizes not allowed for the following arrays:"
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
         int(nlegend_local,kind=private_plint), int(opt_array,kind=private_plint), &
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
    legend_width = real(legend_width_out, kind=wp)
    legend_height = real(legend_height_out, kind=wp)

end subroutine pllegend_impl

subroutine pllightsource_impl( x, y, z )
    real(kind=wp), intent(in) :: x, y, z

    interface
        subroutine interface_pllightsource( x, y, z ) bind(c,name='c_pllightsource')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: x, y, z
        end subroutine interface_pllightsource
    end interface

    call interface_pllightsource( real(x,kind=private_plflt), real(y,kind=private_plflt), real(z,kind=private_plflt) )
end subroutine pllightsource_impl

subroutine plline_impl( x, y )
    real(kind=wp), dimension(:), intent(in) :: x, y

    interface
        subroutine interface_plline( sz, x, y ) bind(c,name='c_plline')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: sz
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
        end subroutine interface_plline
    end interface

    call interface_plline( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt) )
end subroutine plline_impl

subroutine plline3_impl( x, y, z )
   real(kind=wp), dimension(:), intent(in) :: x, y, z

   interface
       subroutine interface_plline3( sz, x, y, z ) bind(c,name='c_plline3')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value, intent(in) :: sz
           real(kind=private_plflt), dimension(*), intent(in) :: x, y, z
       end subroutine interface_plline3
   end interface

   call interface_plline3( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                   real(z,kind=private_plflt) )
end subroutine plline3_impl

subroutine plmesh_impl( x, y, z, opt )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y
    real(kind=wp), dimension(:,:), intent(in) :: z

    real(kind=private_plflt), dimension(:,:), allocatable :: zz_local
    type(c_ptr), dimension(:), allocatable :: zaddress_local

    interface
        subroutine interface_plmesh( x, y, zaddress, nx, ny, opt ) bind(c,name='c_plmesh')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny, opt
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine interface_plmesh
    end interface

    call matrix_to_c( z, zz_local, zaddress_local )

    call interface_plmesh( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress_local, &
                      size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint))

end subroutine plmesh_impl

subroutine plmeshc_impl( x, y, z, opt, clevel )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z

    real(kind=private_plflt), dimension(:,:), allocatable :: zz_local
    type(c_ptr), dimension(:), allocatable :: zaddress_local

    interface
        subroutine interface_plmeshc( x, y, zaddress, nx, ny, opt, clevel, nlevel ) bind(c,name='c_plmeshc')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny, opt, nlevel
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine interface_plmeshc
    end interface

    call matrix_to_c( z, zz_local, zaddress_local )

    call interface_plmeshc( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress_local, &
                      size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                      real(clevel,kind=private_plflt), size(clevel,kind=private_plint) )

end subroutine plmeshc_impl

subroutine plmtex_impl( side, disp, pos, just, text )
    real(kind=wp), intent(in) :: disp, pos, just
    character*(*), intent(in) :: side, text

    interface
        subroutine interface_plmtex( side, disp, pos, just, text ) bind(c,name='c_plmtex')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            character(len=1), dimension(*), intent(in) :: side, text
            real(kind=private_plflt), value, intent(in) :: disp, pos, just
        end subroutine interface_plmtex
    end interface

    call interface_plmtex( trim(side)//c_null_char, real(disp,kind=private_plflt), real(pos,kind=private_plflt), &
                   real(just,kind=private_plflt), trim(text)//c_null_char )
end subroutine plmtex_impl

subroutine plmtex3_impl( side, disp, pos, just, text )
    real(kind=wp), intent(in) :: disp, pos, just
    character*(*), intent(in) :: side, text

    interface
        subroutine interface_plmtex3( side, disp, pos, just, text ) bind(c,name='c_plmtex3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            character(len=1), dimension(*), intent(in) :: side, text
            real(kind=private_plflt), value, intent(in) :: disp, pos, just
        end subroutine interface_plmtex3
    end interface

    call interface_plmtex3( trim(side)//c_null_char, real(disp,kind=private_plflt), real(pos,kind=private_plflt), &
                   real(just,kind=private_plflt), trim(text)//c_null_char )
end subroutine plmtex3_impl

subroutine plot3d_impl( x, y, z, opt, side)
    logical, intent(in) :: side
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y
    real(kind=wp), dimension(:,:), intent(in) :: z

    real(kind=private_plflt), dimension(:,:), allocatable :: zz_local
    type(c_ptr), dimension(:), allocatable :: zaddress_local


    interface
        subroutine interface_plot3d( x, y, zaddress, nx, ny, opt, side ) bind(c,name='c_plot3d')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny, opt
            integer(kind=private_plbool), value, intent(in) :: side
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine interface_plot3d
    end interface

    call matrix_to_c( z, zz_local, zaddress_local )

    call interface_plot3d( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress_local,  &
         size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
         int(merge(1,0,side),kind=private_plbool) )

end subroutine plot3d_impl

subroutine plot3dc_impl( x, y, z, opt, clevel )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z

    real(kind=private_plflt), dimension(:,:), allocatable :: zz_local
    type(c_ptr), dimension(:), allocatable :: zaddress_local

    interface
        subroutine interface_plot3dc( x, y, zaddress, nx, ny, opt, clevel, nlevel ) bind(c,name='c_plot3dc')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny, opt, nlevel
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine interface_plot3dc
    end interface

    call matrix_to_c( z, zz_local, zaddress_local )

    call interface_plot3dc( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress_local,  &
                     size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                     real(clevel,kind=private_plflt), size(clevel,kind=private_plint) )

end subroutine plot3dc_impl

subroutine plot3dcl_impl( x, y, z, opt, clevel, indexxmin, indexxmax, indexymin, indexymax )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z
    integer, intent(in) :: indexxmin, indexxmax
    integer, dimension(:), intent(in) :: indexymin, indexymax

    real(kind=private_plflt), dimension(:,:), allocatable :: zz_local
    type(c_ptr), dimension(:), allocatable :: zaddress_local

    interface
        subroutine interface_plot3dcl( x, y, zaddress, nx, ny, opt, clevel, nlevel, &
                                indexxmin, indexxmax, indexymin, indexymax ) bind(c,name='c_plot3dcl')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny, opt, nlevel, indexxmin, indexxmax
            integer(kind=private_plint), dimension(*), intent(in) :: indexymin, indexymax
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine interface_plot3dcl
    end interface

    call matrix_to_c( z, zz_local, zaddress_local )

    call interface_plot3dcl( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress_local, &
                      size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                      real(clevel,kind=private_plflt), size(clevel,kind=private_plint), &
                      int(indexxmin-1,kind=private_plint), int(indexxmax-1,kind=private_plint), &
                      int(indexymin-1,kind=private_plint), int(indexymax-1,kind=private_plint) )

end subroutine plot3dcl_impl

subroutine plpath_impl( n, x1, y1, x2, y2 )
    integer, intent(in) :: n
    real(kind=wp), intent(in) :: x1, y1, x2, y2

    interface
        subroutine interface_plpath( n, x1, y1, x2, y2 ) bind(c,name='c_plpath')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
            real(kind=private_plflt), value, intent(in) :: x1, y1, x2, y2
        end subroutine interface_plpath
    end interface

    call interface_plpath( int(n,kind=private_plint), real(x1,kind=private_plflt), &
                   real(y1,kind=private_plflt), real(x2,kind=private_plflt), real(y2,kind=private_plflt) )
end subroutine plpath_impl

subroutine plpoin_impl( x, y, code )
    integer, intent(in) :: code
    real(kind=wp), dimension(:), intent(in) :: x, y

      interface
          subroutine interface_plpoin( n, x, y, code ) bind(c,name='c_plpoin')
              implicit none
              include 'included_plplot_interface_private_types.f90'
              integer(kind=private_plint), value, intent(in) :: n, code
              real(kind=private_plflt), dimension(*), intent(in) :: x, y
          end subroutine interface_plpoin
      end interface

     call interface_plpoin( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                    int(code,kind=private_plint) )
end subroutine plpoin_impl

subroutine plpoin3_impl( x, y, z, code )
    integer, intent(in) :: code
    real(kind=wp), dimension(:), intent(in) :: x, y, z

    interface
        subroutine interface_plpoin3( n, x, y, z, code ) bind(c,name='c_plpoin3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n, code
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, z
        end subroutine interface_plpoin3
    end interface

   call interface_plpoin3( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                   real(z,kind=private_plflt), int(code,kind=private_plint) )
end subroutine plpoin3_impl

subroutine plpoly3_impl( x, y, z, draw, ifcc )
    logical, intent(in) :: ifcc
    logical, dimension(:), intent(in) :: draw
    real(kind=wp), dimension(:), intent(in) :: x, y, z

    interface
        subroutine interface_plpoly3( n, x, y, z, draw, ifcc ) bind(c,name='c_plpoly3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
            integer(kind=private_plbool), value, intent(in) :: ifcc
            integer(kind=private_plbool), dimension(*), intent(in) :: draw
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, z
        end subroutine interface_plpoly3
    end interface

    call interface_plpoly3( size(x,kind=private_plint), &
         real(x,kind=private_plflt), real(y,kind=private_plflt), real(z,kind=private_plflt), &
         int(merge(1,0,draw),kind=private_plbool), int(merge(1,0,ifcc),kind=private_plbool) )
end subroutine plpoly3_impl

subroutine plptex_impl( x, y, dx, dy, just, text )

    real(kind=wp), intent(in) :: x, y, dx, dy, just
    character*(*), intent(in) :: text

    interface
        subroutine interface_plptex( x, y, dx, dy, just, text ) bind(c,name='c_plptex')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            character(len=1), dimension(*), intent(in) :: text
            real(kind=private_plflt), value, intent(in) :: x, y, dx, dy, just
        end subroutine interface_plptex
    end interface

    call interface_plptex( real(x,kind=private_plflt), real(y,kind=private_plflt), real(dx,kind=private_plflt), &
                   real(dy,kind=private_plflt), real(just,kind=private_plflt), trim(text)//c_null_char )

end subroutine plptex_impl

subroutine plptex3_impl( wx, wy, wz, dx, dy, dz, sx, sy, sz, just, text )

    real(kind=wp), intent(in) :: wx, wy, wz, dx, dy, dz, sx, sy, sz, just
    character*(*), intent(in) :: text

    interface
        subroutine interface_plptex3( wx, wy, wz, dx, dy, dz, sx, sy, sz, just, text ) bind(c,name='c_plptex3')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            character(len=1), dimension(*), intent(in) :: text
            real(kind=private_plflt), value, intent(in) :: wx, wy, wz, dx, dy, dz, sx, sy, sz, just
        end subroutine interface_plptex3
    end interface

    call interface_plptex3( real(wx,kind=private_plflt), real(wy,kind=private_plflt), real(wz,kind=private_plflt), &
         real(dx,kind=private_plflt), real(dy,kind=private_plflt), real(dz,kind=private_plflt), &
         real(sx,kind=private_plflt), real(sy,kind=private_plflt), real(sz,kind=private_plflt), &
         real(just,kind=private_plflt), trim(text)//c_null_char )

end subroutine plptex3_impl

subroutine plrgbhls_impl( r, g, b, h, l, s )
    real(kind=wp), intent(in) :: r, g, b
    real(kind=wp), intent(out) :: h, l, s

    real(kind=private_plflt) :: h_out, l_out, s_out

    interface
        subroutine interface_plrgbhls( r, g, b, h, l, s ) bind(c,name='c_plrgbhls')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: r, g, b
            real(kind=private_plflt), intent(out) :: h, l, s
        end subroutine interface_plrgbhls
    end interface

    call interface_plrgbhls( real(r,kind=private_plflt), real(g,kind=private_plflt), real(b,kind=private_plflt), &
                     h_out, l_out, s_out )
    h = real(h_out, kind=wp)
    l = real(l_out, kind=wp)
    s = real(s_out, kind=wp)
end subroutine plrgbhls_impl

subroutine plschr_impl( chrdef, chrht )
    real(kind=wp), intent(in) :: chrdef, chrht

    interface
        subroutine interface_plschr( chrdef, chrht ) bind(c,name='c_plschr')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: chrdef, chrht
        end subroutine interface_plschr
    end interface

    call interface_plschr( real(chrdef,kind=private_plflt), real(chrht,kind=private_plflt) )
end subroutine plschr_impl

subroutine plscmap0a_impl( r, g, b, a )
    integer, dimension(:), intent(in) :: r, g, b
    real(kind=wp), dimension(:), intent(in) :: a

    interface
        subroutine interface_plscmap0a( r, g, b, a, n ) bind(c,name='c_plscmap0a')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), dimension(*), intent(in) :: r, g, b
            real(kind=private_plflt), dimension(*), intent(in) :: a
            integer(kind=private_plint), value, intent(in) :: n
        end subroutine interface_plscmap0a
    end interface

    call interface_plscmap0a( int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint), &
                      real(a,kind=private_plflt), size(r,kind=private_plint) )
end subroutine plscmap0a_impl

subroutine plscmap1_range_impl( chrdef, chrht )
    real(kind=wp), intent(in) :: chrdef, chrht

    interface
        subroutine interface_plscmap1_range( chrdef, chrht ) bind(c,name='c_plscmap1_range')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: chrdef, chrht
        end subroutine interface_plscmap1_range
    end interface

    call interface_plscmap1_range( real(chrdef,kind=private_plflt), real(chrht,kind=private_plflt) )
end subroutine plscmap1_range_impl

subroutine plscmap1a_impl( r, g, b, a )
    integer, dimension(:), intent(in) :: r, g, b
    real(kind=wp), dimension(:), intent(in) :: a

    interface
        subroutine interface_plscmap1a( r, g, b, a, n ) bind(c,name='c_plscmap1a')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), dimension(*), intent(in) :: r, g, b
            real(kind=private_plflt), dimension(*), intent(in) :: a
            integer(kind=private_plint), value, intent(in) :: n
        end subroutine interface_plscmap1a
    end interface

    call interface_plscmap1a( int(r,kind=private_plint), int(g,kind=private_plint), int(b,kind=private_plint), &
                      real(a,kind=private_plflt), size(r,kind=private_plint) )
end subroutine plscmap1a_impl

subroutine plscmap1l_impl( rgbtype, intensity, coord1, coord2, coord3, alt_hue_path)
    logical, intent(in) :: rgbtype
    real(kind=wp), dimension(:), intent(in) :: intensity, coord1, coord2, coord3
    logical, dimension(:), optional, intent(in) :: alt_hue_path

    integer(kind=private_plbool), dimension(:), allocatable, target :: ialt_hue_path_local
    
    interface
        subroutine interface_plscmap1l( rgbtype, npts, intensity, coord1, coord2, coord3, alt_hue_path ) &
            bind(c,name='c_plscmap1l')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: npts
            integer(kind=private_plbool), value, intent(in) :: rgbtype
            real(kind=private_plflt), dimension(*), intent(in) :: intensity, coord1, coord2, coord3
            !integer(kind=private_plbool), dimension(*), intent(in) :: alt_hue_path
            type(c_ptr), value, intent(in) :: alt_hue_path
        end subroutine interface_plscmap1l
    end interface

    if ( present(alt_hue_path) ) then
       allocate( ialt_hue_path_local(size(alt_hue_path)) )
       ialt_hue_path_local = int(merge(1,0,alt_hue_path),kind=private_plbool)
       
       call interface_plscmap1l( int(merge(1,0,rgbtype),kind=private_plbool), size(intensity,kind=private_plint), &
                          real(intensity,kind=private_plflt), real(coord1,kind=private_plflt), &
                          real(coord2,kind=private_plflt), real(coord3,kind=private_plflt), &
                          c_loc(ialt_hue_path_local) )
       deallocate( ialt_hue_path_local )
    else
        call interface_plscmap1l( int(merge(1,0,rgbtype),kind=private_plbool), size(intensity,kind=private_plint), &
                          real(intensity,kind=private_plflt), real(coord1,kind=private_plflt), &
                          real(coord2,kind=private_plflt), real(coord3,kind=private_plflt), &
                          c_null_ptr )
    endif
end subroutine plscmap1l_impl

subroutine plscmap1la_impl( rgbtype, intensity, coord1, coord2, coord3, alpha, alt_hue_path)
    logical, intent(in) :: rgbtype
    real(kind=wp), dimension(:), intent(in) :: intensity, coord1, coord2, coord3, alpha
    logical, dimension(:), optional, intent(in) :: alt_hue_path

    integer(kind=private_plbool), dimension(:), allocatable, target :: ialt_hue_path_local

    interface
        subroutine interface_plscmap1la( rgbtype, n, intensity, coord1, coord2, coord3, alpha, alt_hue_path ) &
            bind(c,name='c_plscmap1la')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n
            integer(kind=private_plbool), value, intent(in) :: rgbtype
            real(kind=private_plflt), dimension(*), intent(in) :: intensity, coord1, coord2, coord3, alpha
            !integer(kind=private_plbool), dimension(*), intent(in) :: alt_hue_path
            type(c_ptr), value, intent(in) :: alt_hue_path
         end subroutine interface_plscmap1la
    end interface

    if ( present(alt_hue_path) ) then
        allocate( ialt_hue_path_local(size(alt_hue_path)) )
        ialt_hue_path_local = int(merge(1,0,alt_hue_path),kind=private_plbool)

        call interface_plscmap1la( int(merge(1,0,rgbtype),kind=private_plbool), size(intensity,kind=private_plint), &
                          real(intensity,kind=private_plflt), real(coord1,kind=private_plflt), &
                          real(coord2,kind=private_plflt), real(coord3,kind=private_plflt), &
                          real(alpha,kind=private_plflt), c_loc(ialt_hue_path_local) )
        deallocate(ialt_hue_path_local)
    else
        call interface_plscmap1la( int(merge(1,0,rgbtype),kind=private_plbool), size(intensity,kind=private_plint), &
                          real(intensity,kind=private_plflt), real(coord1,kind=private_plflt), &
                          real(coord2,kind=private_plflt), real(coord3,kind=private_plflt), &
                          real(alpha,kind=private_plflt), c_null_ptr )
    endif
end subroutine plscmap1la_impl

subroutine plscol0a_impl( icol, r, g, b, a )
    integer, intent(in) :: icol, r, g, b
    real(kind=wp), intent(in) :: a

    interface
        subroutine interface_plscol0a( icol, r, g, b, a ) bind(c,name='c_plscol0a')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: icol, r, g, b
            real(kind=private_plflt), value, intent(in) :: a
        end subroutine interface_plscol0a
    end interface

    call interface_plscol0a( int(icol,kind=private_plint), int(r,kind=private_plint), int(g,kind=private_plint), &
                     int(b,kind=private_plint), real(a,kind=private_plflt) )
end subroutine plscol0a_impl

subroutine plscolbga_impl( r, g, b, a )
    integer, intent(in) :: r, g, b
    real(kind=wp), intent(in) :: a

    interface
        subroutine interface_plscolbga( r, g, b, a ) bind(c,name='c_plscolbga')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: r, g, b
            real(kind=private_plflt), value, intent(in) :: a
        end subroutine interface_plscolbga
    end interface

    call interface_plscolbga( int(r,kind=private_plint), int(g,kind=private_plint), &
                      int(b,kind=private_plint), real(a,kind=private_plflt) )
end subroutine plscolbga_impl

subroutine plsdidev_impl( mar, aspect, jx, jy )
    real(kind=wp), intent(in) :: mar, aspect, jx, jy

    interface
        subroutine interface_plsdidev( mar, aspect, jx, jy ) bind(c,name='c_plsdidev')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: mar, aspect, jx, jy
        end subroutine interface_plsdidev
    end interface

    call interface_plsdidev( real(mar,kind=private_plflt), real(aspect,kind=private_plflt), &
                     real(jx,kind=private_plflt), real(jy,kind=private_plflt) )
end subroutine plsdidev_impl

subroutine plsdimap_impl( dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm )
    real(kind=wp), intent(in) :: dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm

    interface
        subroutine interface_plsdimap( dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm ) bind(c,name='c_plsdimap')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: dimxmi, dimxmax, diymin, dimymax, dimxpmm, diypmm
        end subroutine interface_plsdimap
    end interface

    call interface_plsdimap( real(dimxmi,kind=private_plflt), real(dimxmax,kind=private_plflt), &
                     real(diymin,kind=private_plflt), real(dimymax,kind=private_plflt), &
                     real(dimxpmm,kind=private_plflt), real(diypmm,kind=private_plflt) )
end subroutine plsdimap_impl

subroutine plsdiori_impl( rot )
    real(kind=wp), intent(in) :: rot

    interface
        subroutine interface_plsdiori( rot) bind(c,name='c_plsdiori')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: rot
        end subroutine interface_plsdiori
    end interface

    call interface_plsdiori( real(rot,kind=private_plflt) )
end subroutine plsdiori_impl

subroutine plsdiplt_impl( xmin, ymin, xmax, ymax )
    real(kind=wp), intent(in) :: xmin, ymin, xmax, ymax

    interface
        subroutine interface_plsdiplt( xmin, ymin, xmax, ymax ) bind(c,name='c_plsdiplt')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: xmin, ymin, xmax, ymax
        end subroutine interface_plsdiplt
    end interface

    call interface_plsdiplt( &
         real(xmin,kind=private_plflt), real(ymin,kind=private_plflt), &
         real(xmax,kind=private_plflt), real(ymax,kind=private_plflt) )
end subroutine plsdiplt_impl

subroutine plsdiplz_impl( xmin, ymin, xmax, ymax )
    real(kind=wp), intent(in) :: xmin, ymin, xmax, ymax

    interface
        subroutine interface_plsdiplz( xmin, ymin, xmax, ymax ) bind(c,name='c_plsdiplz')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: xmin, ymin, xmax, ymax
        end subroutine interface_plsdiplz
    end interface

    call interface_plsdiplz( &
         real(xmin,kind=private_plflt), real(ymin,kind=private_plflt), &
         real(xmax,kind=private_plflt), real(ymax,kind=private_plflt) )
end subroutine plsdiplz_impl

subroutine plshade_single_0( z, defined, xmin, xmax, ymin, ymax, shade_min, shade_max, sh_cmap, sh_color, sh_width, &
                  min_color, min_width, max_color, max_width )
    real(kind=wp), dimension(:,:), intent(in) :: z
    character(len=*), intent(in) :: defined
    real(kind=wp), intent(in) :: xmin, xmax, ymin, ymax
    real(kind=wp), intent(in) :: sh_width, min_width, max_width
    real(kind=wp), intent(in) :: shade_min, shade_max, sh_color
    integer, intent(in) :: sh_cmap, min_color, max_color

    integer(kind=private_plint), parameter :: rectangular = 1
    real(kind=private_plflt), dimension(:,:), allocatable :: z_c_local
    type(c_ptr), dimension(:), allocatable :: z_address_local

    interface
        subroutine fill( n, x, y ) bind(c, name = 'c_plfill')
            use plplot_types, only: private_plint, private_plflt
            integer(kind=private_plint), value, intent(in) :: n
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
        end subroutine fill
    end interface

    interface
        subroutine interface_plshade( a, nx, ny, defined, xmin, xmax, ymin, ymax, &
                                      shade_min, shade_max, sh_cmap, sh_color, sh_width, &
                                      min_color, min_width, max_color, max_width, &
                                      fill, rectangular, transform, data ) bind(c, name = 'c_plshade' )
            use iso_c_binding, only: c_ptr, c_funptr, c_null_ptr
            use plplot_types, only: private_plint, private_plflt
            implicit none
            type(c_ptr), value, intent(in)  :: a
            integer(kind=private_plint), value, intent(in) :: nx, ny, sh_cmap, min_color, max_color, rectangular
            real(kind=private_plflt), value, intent(in) :: xmin, xmax, ymin, ymax
            real(kind=private_plflt), value, intent(in) :: sh_width, min_width, max_width
            real(kind=private_plflt), value, intent(in) :: shade_min, shade_max, sh_color
            type(c_ptr), value, intent(in) :: data
            type(c_ptr), value, intent(in) :: defined ! Not used in this case
            type(c_ptr), value, intent(in) :: transform ! Not used in this case
            interface
                subroutine fill( n, x, y ) bind(c)
                    use plplot_types, only: private_plint, private_plflt
                    integer(kind=private_plint), value, intent(in) :: n
                    real(kind=private_plflt), dimension(*), intent(in) :: x, y
                end subroutine fill
            end interface
        end subroutine interface_plshade
    end interface

    call matrix_to_c( z, z_c_local, z_address_local )

    call interface_plshade( z_address_local, size(z,1,kind=private_plint), size(z,2,kind=private_plint), c_null_ptr, &
                   real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), &
                   real(ymin,kind=private_plflt), real(ymax,kind=private_plflt), &
                   real(shade_min,kind=private_plflt), real(shade_max,kind=private_plflt), &
                   int(sh_cmap,kind=private_plint), real(sh_color,kind=private_plflt), &
                   real(sh_width,kind=private_plflt), &
                   int(min_color,kind=private_plint), real(min_width,kind=private_plflt), &
                   int(max_color,kind=private_plint), real(max_width,kind=private_plflt), &
                   fill, rectangular, c_null_ptr, c_null_ptr )
end subroutine plshade_single_0

!            interface
!                subroutine transform( x, y, tx, ty, data ) bind(c)
!                    use iso_c_binding
!                    use plplot_types, only: private_plflt, PLcGrid
!                    implicit none
!                    real(kind=private_plflt), value, intent(in) :: x, y
!                    real(kind=private_plflt), intent(out) :: tx, ty
!                    type(PLcGrid), intent(in) :: data
!                end subroutine transform
!            end interface

subroutine plsmaj_impl( def, scale )
    real(kind=wp), intent(in) :: def, scale

    interface
        subroutine interface_plsmaj( def, scale ) bind(c,name='c_plsmaj')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: def, scale
        end subroutine interface_plsmaj
    end interface

    call interface_plsmaj( real(def,kind=private_plflt), real(scale,kind=private_plflt) )

end subroutine plsmaj_impl

subroutine plsmin_impl( def, scale )
    real(kind=wp), intent(in) :: def, scale

    interface
        subroutine interface_plsmin( def, scale ) bind(c,name='c_plsmin')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: def, scale
        end subroutine interface_plsmin
    end interface

    call interface_plsmin( real(def,kind=private_plflt), real(scale,kind=private_plflt) )

end subroutine plsmin_impl

subroutine plspage_impl( xp, yp, xleng, yleng, xoff, yoff )
    integer, intent(in) :: xleng, yleng, xoff, yoff
    real(kind=wp), intent(in) :: xp, yp

    interface
        subroutine interface_plspage( xp, yp, xleng, yleng, xoff, yoff ) bind(c,name='c_plspage')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: xleng, yleng, xoff, yoff
            real(kind=private_plflt), value, intent(in) :: xp, yp
        end subroutine interface_plspage
    end interface

    call interface_plspage( real(xp,kind=private_plflt), real(yp,kind=private_plflt), &
         int(xleng,kind=private_plint), int(yleng,kind=private_plint), &
         int(xoff,kind=private_plint), int(yoff,kind=private_plint) )

end subroutine plspage_impl

subroutine plssym_impl( def, scale )

   real(kind=wp), intent(in) :: def, scale

   interface
       subroutine interface_plssym( def, scale ) bind(c,name='c_plssym')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           real(kind=private_plflt), value, intent(in) :: def, scale
       end subroutine interface_plssym
   end interface

   call interface_plssym( real(def,kind=private_plflt), real(scale,kind=private_plflt) )
end subroutine plssym_impl

subroutine plstring_impl( x, y, string )

  real(kind=wp), dimension (:), intent(in) :: x, y
  character(len=*), intent(in) :: string

  integer(kind=private_plint) :: n_local

  interface
     subroutine interface_plstring( n, x, y, string ) bind(c,name='c_plstring')
       implicit none
       include 'included_plplot_interface_private_types.f90'
       integer(kind=private_plint), value, intent(in) :: n
       real(kind=private_plflt), dimension(*), intent(in) :: x, y
       character(len=1), dimension(*), intent(in) :: string
     end subroutine interface_plstring
  end interface

  n_local = size(x, kind=private_plint)
  if(n_local /= size(y, kind=private_plint) ) then
     write(0,*) "f95 plstring ERROR: inconsistent array sizes not allowed for x and y"
     return
  end if

  call interface_plstring( n_local, real(x,kind=private_plflt), real(y,kind=private_plflt), &
       trim(string)//c_null_char )
end subroutine plstring_impl

subroutine plstring3_impl( x, y, z, string )

  real(kind=wp), dimension (:), intent(in) :: x, y, z
  character(len=*), intent(in) :: string

  integer(kind=private_plint) :: n_local

  interface
     subroutine interface_plstring3( n, x, y, z, string ) bind(c,name='c_plstring3')
       implicit none
       include 'included_plplot_interface_private_types.f90'
       integer(kind=private_plint), value, intent(in) :: n
       real(kind=private_plflt), dimension(*), intent(in) :: x, y, z
       character(len=1), dimension(*), intent(in) :: string
     end subroutine interface_plstring3
  end interface

  n_local = size(x, kind=private_plint)
  if(n_local /= size(y, kind=private_plint) .or. n_local /= size(z, kind=private_plint) ) then
     write(0,*) "f95 plstring3 ERROR: inconsistent array sizes not allowed for x, y, and z"
     write(0, *)
     return
  end if

  call interface_plstring3( n_local, real(x,kind=private_plflt), real(y,kind=private_plflt), real(z,kind=private_plflt), &
       trim(string)//c_null_char )
end subroutine plstring3_impl

subroutine plstripa_impl( id, pen, x, y )

   integer, intent(in) :: id, pen
   real(kind=wp), intent(in) :: x, y

   interface
       subroutine interface_plstripa( id, pen, x, y ) bind(c,name='c_plstripa')
           implicit none
           include 'included_plplot_interface_private_types.f90'
           integer(kind=private_plint), value, intent(in) :: id, pen
           real(kind=private_plflt), value, intent(in) :: x, y
       end subroutine interface_plstripa
   end interface

   call interface_plstripa( int(id, kind=private_plint), int(pen, kind=private_plint), &
        real(x,kind=private_plflt), real(y,kind=private_plflt) )
end subroutine plstripa_impl

subroutine plstripc_impl( &
     id, xspec, yspec, &
     xmin, xmax, xjump, ymin, ymax, &
     xlpos, ylpos, &
     y_ascl, acc, &
     colbox, collab, &
     colline, styline, legline, &
     labx, laby, labtop )

  logical, intent(in) :: y_ascl, acc
  integer, intent(in) :: colbox, collab
  integer, dimension(:), intent(in) :: colline, styline
  real(kind=wp), intent(in) :: xmin, xmax, xjump, ymin, ymax, xlpos, ylpos
  character(len=*), intent(in) :: xspec, yspec, labx, laby, labtop
  character(len=*), dimension(:), intent(in) :: legline
  integer, intent(out) :: id

  integer(kind=private_plint) :: id_out, n_pens_local

  interface
     subroutine interface_plstripc( &
          id, xspec, yspec, &
          xmin, xmax, xjump, ymin, ymax, &
          xlpos, ylpos, &
          y_ascl, acc, &
          colbox, collab, &
          n_pens, colline, styline, length_legline, legline, &
          labx, laby, labtop ) bind(c,name='fc_plstripc')
       use iso_c_binding, only: c_char
       implicit none
       include 'included_plplot_interface_private_types.f90'
       integer(kind=private_plint), value, intent(in) :: colbox, collab, n_pens, length_legline
       integer(kind=private_plint), dimension(*), intent(in) :: colline, styline
       integer(kind=private_plbool), value, intent(in) :: y_ascl, acc
       real(kind=private_plflt), value, intent(in) :: xmin, xmax, xjump, ymin, ymax, xlpos, ylpos
       character(len=1), dimension(*), intent(in) :: xspec, yspec, labx, laby, labtop
       ! These Fortran arguments require special processing done
       ! in fc_pllegend at the C level to interoperate properly
       ! with the C version of pllegend.
       character(c_char), intent(in) :: legline(length_legline, n_pens)
       integer(kind=private_plint), intent(out) :: id

     end subroutine interface_plstripc
  end interface

  n_pens_local = size(colline, kind=private_plint)
  if( &
       n_pens_local /= 4 .or. &
       n_pens_local /= size(styline, kind=private_plint) .or. &
       n_pens_local /= size(legline, kind=private_plint) ) then
     write(0,*) "f95 plstripc ERROR: sizes of colline, styline, and/or legline are not 4"
     return
  endif

  call interface_plstripc( &
       id_out, trim(xspec)//c_null_char, trim(yspec)//c_null_char, &
       real(xmin, kind=private_plflt), real(xmax, kind=private_plflt), real(xjump, kind=private_plflt), &
       real(ymin, kind=private_plflt), real(ymax, kind=private_plflt), &
       real(xlpos, kind=private_plflt), real(ylpos, kind=private_plflt), &
       int(merge(1,0,y_ascl),kind=private_plbool), int(merge(1,0,acc),kind=private_plbool),&
       int(colbox, kind=private_plint), int(collab, kind=private_plint), &
       n_pens_local, int(colline, kind=private_plint), int(styline, kind=private_plint), &
       len(legline, kind=private_plint), legline, &
       trim(labx)//c_null_char, trim(laby)//c_null_char, trim(labtop)//c_null_char )
  id = int(id_out, kind=private_plint)
end subroutine plstripc_impl

subroutine plsurf3d_impl( x, y, z, opt, clevel )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z

    real(kind=private_plflt), dimension(:,:), allocatable :: zz_local
    type(c_ptr), dimension(:), allocatable :: zaddress_local

    interface
        subroutine interface_plsurf3d( x, y, zaddress, nx, ny, opt, clevel, nlevel ) bind(c,name='c_plsurf3d')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny, opt, nlevel
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine interface_plsurf3d
    end interface

    call matrix_to_c( z, zz_local, zaddress_local )

    call interface_plsurf3d( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress_local,  &
                     size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                     real(clevel,kind=private_plflt), size(clevel,kind=private_plint) )

end subroutine plsurf3d_impl

subroutine plsurf3dl_impl( x, y, z, opt, clevel, indexxmin, indexxmax, indexymin, indexymax )
    integer, intent(in) :: opt
    real(kind=wp), dimension(:), intent(in) :: x, y, clevel
    real(kind=wp), dimension(:,:), intent(in) :: z
    integer, intent(in) :: indexxmin, indexxmax
    integer, dimension(:), intent(in) :: indexymin, indexymax

    real(kind=private_plflt), dimension(:,:), allocatable :: zz_local
    type(c_ptr), dimension(:), allocatable :: zaddress_local

    interface
        subroutine interface_plsurf3dl( x, y, zaddress, nx, ny, opt, clevel, nlevel, &
                                indexxmin, indexxmax, indexymin, indexymax ) bind(c,name='c_plsurf3dl')
            use iso_c_binding, only: c_ptr
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: nx, ny, opt, nlevel, indexxmin, indexxmax
            integer(kind=private_plint), dimension(*), intent(in) :: indexymin, indexymax
            real(kind=private_plflt), dimension(*), intent(in) :: x, y, clevel
            type(c_ptr), dimension(*), intent(in) :: zaddress
        end subroutine interface_plsurf3dl
    end interface

    call matrix_to_c( z, zz_local, zaddress_local )

    call interface_plsurf3dl( real(x,kind=private_plflt), real(y,kind=private_plflt), zaddress_local, &
                      size(x,kind=private_plint), size(y,kind=private_plint), int(opt,kind=private_plint), &
                      real(clevel,kind=private_plflt), size(clevel,kind=private_plint), &
                      int(indexxmin-1,kind=private_plint), int(indexxmax-1,kind=private_plint), &
                      int(indexymin-1,kind=private_plint), int(indexymax-1,kind=private_plint) )

end subroutine plsurf3dl_impl

subroutine plsvpa_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(in) :: xmin, xmax, ymin, ymax

    interface
        subroutine interface_plsvpa( xmin, xmax, ymin, ymax ) bind(c,name='c_plsvpa')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: xmin, xmax, ymin, ymax
        end subroutine interface_plsvpa
    end interface

    call interface_plsvpa( real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), &
         real(ymin,kind=private_plflt), real(ymax,kind=private_plflt) )
end subroutine plsvpa_impl

subroutine plsym_impl( x, y, code )
    integer, intent(in) :: code
    real(kind=wp), dimension(:), intent(in) :: x, y

    interface
        subroutine interface_plsym( n, x, y, code ) bind(c,name='c_plsym')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            integer(kind=private_plint), value, intent(in) :: n, code
            real(kind=private_plflt), dimension(*), intent(in) :: x, y
        end subroutine interface_plsym
    end interface

    call interface_plsym( size(x,kind=private_plint), real(x,kind=private_plflt), real(y,kind=private_plflt), &
                  int(code,kind=private_plint) )
end subroutine plsym_impl

subroutine plvasp_impl( aspect)
    real(kind=wp), intent(in) :: aspect

    interface
        subroutine interface_plvasp( aspect ) bind(c,name='c_plvasp')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: aspect
        end subroutine interface_plvasp
    end interface

    call interface_plvasp( real(aspect,kind=private_plflt) )
end subroutine plvasp_impl

subroutine plvpas_impl( xmin, xmax, ymin, ymax, aspect )
    real(kind=wp), intent(in) :: xmin, xmax, ymin, ymax, aspect

    interface
        subroutine interface_plvpas( xmin, xmax, ymin, ymax, aspect ) bind(c,name='c_plvpas')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: xmin, xmax, ymin, ymax, aspect
        end subroutine interface_plvpas
    end interface

    call interface_plvpas( real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), &
                   real(ymin,kind=private_plflt), real(ymax,kind=private_plflt), real(aspect,kind=private_plflt) )
end subroutine plvpas_impl

subroutine plvpor_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(in) :: xmin, xmax, ymin, ymax

    interface
        subroutine interface_plvpor( xmin, xmax, ymin, ymax ) bind(c,name='c_plvpor')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: xmin, xmax, ymin, ymax
        end subroutine interface_plvpor
    end interface

    call interface_plvpor( real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), &
                   real(ymin,kind=private_plflt), real(ymax,kind=private_plflt)  )
end subroutine plvpor_impl

subroutine plw3d_impl( basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az )
    real(kind=wp), intent(in) :: basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az

    interface
        subroutine interface_plw3d( basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az ) bind(c,name='c_plw3d')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: basex, basey, height, xmin, xmax, ymin, ymax, zmin, zmax, alt, az
        end subroutine interface_plw3d
    end interface

    call interface_plw3d( real(basex,kind=private_plflt), real(basey,kind=private_plflt), real(height,kind=private_plflt), &
                  real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), real(ymin,kind=private_plflt),     &
                  real(ymax,kind=private_plflt), real(zmin,kind=private_plflt), real(zmax,kind=private_plflt),     &
                  real(alt,kind=private_plflt), real(az,kind=private_plflt) )
end subroutine plw3d_impl

subroutine plwidth_impl( width )
    real(kind=wp), intent(in) :: width

    interface
        subroutine interface_plwidth( width ) bind(c,name='c_plwidth')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: width
        end subroutine interface_plwidth
    end interface

    call interface_plwidth( real(width,kind=private_plflt) )
end subroutine plwidth_impl

subroutine plwind_impl( xmin, xmax, ymin, ymax )
    real(kind=wp), intent(in):: xmin, xmax, ymin, ymax

    interface
        subroutine interface_plwind( xmin, xmax, ymin, ymax ) bind(c,name='c_plwind')
            implicit none
            include 'included_plplot_interface_private_types.f90'
            real(kind=private_plflt), value, intent(in) :: xmin, xmax, ymin, ymax
        end subroutine interface_plwind
    end interface

    call interface_plwind( real(xmin,kind=private_plflt), real(xmax,kind=private_plflt), &
                   real(ymin,kind=private_plflt), real(ymax,kind=private_plflt) )
end subroutine plwind_impl
