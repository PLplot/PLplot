!***********************************************************************
!  $Id$
!  sfstubs.f
!
!  Copyright (C) 2004  Alan W. Irwin
!
!  This file is part of PLplot.
!
!  PLplot is free software; you can redistribute it and/or modify
!  it under the terms of the GNU General Library Public License as published
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
!  This file contains the more complicated fortran stub routines
!  that the more generic scstubs.c cannot handle.
!  Typical stubs here must convert a fortran character string
!  to C or vice versa.
!  N.B. the called routines (that are defined in scstubs.c) have
!  a suffix of '7' to avoid name clashes and also presumably as a
!  signal in scstubs.c that they were called from these routines.
!  Where arguments are floating-point we explicitly type them as real(kind=plflt).
!  This typing is never used since these arguments are
!  actually passed by reference.  That implies these routines should
!  work if the calling routine and libplplot itself are both
!  double precision or both single precision.
!
!***********************************************************************

      subroutine plsetopt(opt, optarg)

      implicit none
      character*(*) opt, optarg

      include 'sfstubs.h'

      call plstrf2c(opt, string1, maxlen)
      call plstrf2c(optarg, string2, maxlen)
      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      call plsetopt7(s1, s2)

      end subroutine

!***********************************************************************

      subroutine plsdev(dnam)

      implicit none
      character*(*) dnam

      include 'sfstubs.h'

      call plstrf2c(dnam, string1, maxlen)
      s1 = transfer( string1, s1 )
      call plsdev7(s1)

      end subroutine

!***********************************************************************

      subroutine plgdev(dnam)

      implicit none
      character*(*) dnam

      include 'sfstubs.h'

      call plgdev7(string1)
      call plstrc2f(string1, dnam)

      end subroutine

!***********************************************************************

      subroutine plsfnam(fnam)

      implicit none
      character*(*) fnam

      include 'sfstubs.h'

      call plstrf2c(fnam, string1, maxlen)
      s1 = transfer( string1, s1 )
      call plsfnam7(s1)

      end subroutine

!***********************************************************************

      subroutine plgfnam(fnam)

      implicit none
      character*(*) fnam

      include 'sfstubs.h'

      call plgfnam7(string1)
      call plstrc2f(string1, fnam)

      end subroutine

!***********************************************************************

      subroutine plgver(ver)

      implicit none
      character*(*) ver

      include 'sfstubs.h'

      call plgver7(s1)
      string1 = transfer( s1, string1 )
      call plstrc2f(string1, ver)

      end subroutine

!***********************************************************************

      subroutine plaxes(x0,y0,xopt,xtick,nxsub,yopt,ytick,nysub)

      implicit none
      real(kind=plflt) x0, y0, xtick, ytick
      integer nxsub, nysub
      character*(*) xopt,yopt

      include 'sfstubs.h'

      call plstrf2c(xopt, string1, maxlen)
      call plstrf2c(yopt, string2, maxlen)

      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      call plaxes7(x0,y0,s1,xtick,nxsub,s2,ytick,nysub)

      end subroutine

!***********************************************************************

      subroutine plbox(xopt,xtick,nxsub,yopt,ytick,nysub)

      implicit none
      real(kind=plflt) xtick, ytick
      integer nxsub, nysub
      character*(*) xopt,yopt

      include 'sfstubs.h'

      call plstrf2c(xopt, string1, maxlen)
      call plstrf2c(yopt, string2, maxlen)

      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      call plbox7(s1,xtick,nxsub,s2,ytick,nysub)

      end subroutine

!***********************************************************************

      subroutine plbox3(xopt,xlabel,xtick,nxsub,yopt,ylabel,ytick,nysub, &
       zopt,zlabel,ztick,nzsub)

      implicit none
      real(kind=plflt) xtick, ytick, ztick
      character*(*) xopt,xlabel,yopt,ylabel,zopt,zlabel
      integer nxsub, nysub, nzsub

      include 'sfstubs.h'

      call plstrf2c(xopt, string1, maxlen)
      call plstrf2c(xlabel, string2, maxlen)
      call plstrf2c(yopt, string3, maxlen)
      call plstrf2c(ylabel, string4, maxlen)
      call plstrf2c(zopt, string5, maxlen)
      call plstrf2c(zlabel, string6, maxlen)

      s1 = transfer( string1, s1 )
      s2 = transfer( string1, s2 )
      s3 = transfer( string1, s3 )
      s4 = transfer( string1, s4 )
      s5 = transfer( string1, s5 )
      s6 = transfer( string1, s6 )
      call plbox37(s1,s2,xtick,nxsub, &
        s3,s4,ytick,nysub, &
        s5,s6,ztick,nzsub)

      end subroutine

!***********************************************************************

      subroutine plcontour_0(z,kx,lx,ky,ly,clevel)

      implicit none
      integer kx, lx, ky, ly
      real(kind=plflt) z(:,:), clevel(:)

      call plcon07(z,size(z,1),size(z,2),kx,lx,ky,ly,clevel,size(clevel))

      end subroutine

!***********************************************************************

      subroutine plcontour_1(z,kx,lx,ky,ly,clevel,xg,yg)

      implicit none
      integer kx, lx, ky, ly
      real(kind=plflt) z(:,:), xg(:), yg(:), clevel(:)

      call plcon17(z,size(z,1),size(z,2),kx,lx,ky,ly,clevel,size(clevel),xg,yg)

      end subroutine

!***********************************************************************

      subroutine plcontour_2(z,kx,lx,ky,ly,clevel,xg,yg)

      implicit none
      integer kx, lx, ky, ly
      real(kind=plflt) z(:,:), xg(:,:), yg(:,:), clevel(:)

      call plcon27(z,size(z,1),size(z,2),kx,lx,ky,ly,clevel,size(clevel),xg,yg)

      end subroutine

!***********************************************************************

      subroutine plcontour_tr(z,kx,lx,ky,ly,clevel,tr)

      implicit none
      integer kx, lx, ky, ly
      real(kind=plflt) z(:,:), clevel(:)
      real(kind=plflt) tr(6)

      call plcont7(z,size(z,1),size(z,2),kx,lx,ky,ly,clevel,size(clevel),tr)

      end subroutine

!***********************************************************************

      subroutine plcontour_0_all(z,clevel)

      implicit none
      integer kx, lx, ky, ly
      real(kind=plflt) z(:,:), clevel(:)

      kx = 1
      lx = size(z,1)
      ky = 1
      ly = size(z,2)
      call plcon07(z,size(z,1),size(z,2),kx,lx,ky,ly,clevel,size(clevel))

      end subroutine

!***********************************************************************

      subroutine plcontour_1_all(z,clevel,xg,yg)

      implicit none
      integer kx, lx, ky, ly
      real(kind=plflt) z(:,:), xg(:), yg(:), clevel(:)

      kx = 1
      lx = size(z,1)
      ky = 1
      ly = size(z,2)
      call plcon17(z,size(z,1),size(z,2),kx,lx,ky,ly,clevel,size(clevel),xg,yg)

      end subroutine

!***********************************************************************

      subroutine plcontour_2_all(z,clevel,xg,yg)

      implicit none
      integer kx, lx, ky, ly
      real(kind=plflt) z(:,:), xg(:,:), yg(:,:), clevel(:)

      kx = 1
      lx = size(z,1)
      ky = 1
      ly = size(z,2)
      call plcon27(z,size(z,1),size(z,2),kx,lx,ky,ly,clevel,size(clevel),xg,yg)

      end subroutine

!***********************************************************************

      subroutine plcontour_tr_all(z,clevel,tr)

      implicit none
      integer kx, lx, ky, ly
      real(kind=plflt) z(:,:), clevel(:)
      real(kind=plflt) tr(6)

      kx = 1
      lx = size(z,1)
      ky = 1
      ly = size(z,2)
      call plcont7(z,size(z,1),size(z,2),kx,lx,ky,ly,clevel,size(clevel),tr)

      end subroutine

!***********************************************************************

      subroutine plvectors_0(u, v, scale)

      implicit none
      real(kind=plflt) u(:,:), v(:,:), scale

      call plvec07(u,v,size(u,1),size(u,2),scale)

      end subroutine

!***********************************************************************

      subroutine plvectors_1(u, v, scale, xg, yg)

      implicit none
      real(kind=plflt) u(:,:), v(:,:), xg(:), yg(:), scale

      call plvec17(u,v,size(u,1),size(u,2),scale,xg,yg)

      end subroutine

!***********************************************************************

      subroutine plvectors_2(u, v, scale, xg, yg)

      implicit none
      real(kind=plflt) u(:,:), v(:,:), xg(:,:), yg(:,:), &
                       scale

      call plvec27(u,v,size(u,1),size(u,2),scale,xg,yg)

      end subroutine

!***********************************************************************

      subroutine plvectors_tr(u, v, scale, tr)

      implicit none
      real(kind=plflt) u(:,:), v(:,:), scale
      real(kind=plflt) tr(6)

      call plvect7(u,v,size(u,1),size(u,2),scale,tr)

      end subroutine

!***********************************************************************

      subroutine plshade_single_0(z, defined, &
        xmin, xmax, ymin, ymax, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width)

      implicit none
      character defined*(*)
      integer sh_cmap, sh_width
      integer min_color, min_width, max_color, max_width
      real(kind=plflt) shade_min, shade_max, sh_color
      real(kind=plflt) z(:,:), xmin, xmax, ymin, ymax

      include 'sfstubs.h'

!	call plstrf2c(dnam, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plshade07(z, size(z,1), size(z,2), s1, &
        xmin, xmax, ymin, ymax, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width, size(z,1))

      end subroutine

!***********************************************************************

      subroutine plshade_single_1(z, defined, &
        xmin, xmax, ymin, ymax, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width, &
        xg, yg )

      implicit none
      character defined*(*)
      integer sh_cmap, sh_width
      integer min_color, min_width, max_color, max_width
      real(kind=plflt) shade_min, shade_max, sh_color
      real(kind=plflt) z(:,:), xmin, xmax, ymin, ymax, xg(:), yg(:)

      include 'sfstubs.h'

!	call plstrf2c(dnam, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plshade17(z, size(z,1), size(z,2), s1, &
        xmin, xmax, ymin, ymax, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width, &
        xg, yg, size(z,1))

      end subroutine

!***********************************************************************

      subroutine plshade_single_2(z, defined, &
        xmin, xmax, ymin, ymax, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width, &
        xg, yg )

      implicit none
      character defined*(*)
      integer sh_cmap, sh_width
      integer min_color, min_width, max_color, max_width
      real(kind=plflt) shade_min, shade_max, sh_color
      real(kind=plflt) z(:,:), xmin, xmax, ymin, ymax, xg(:,:), yg(:,:)

      include 'sfstubs.h'

!	call plstrf2c(dnam, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plshade27(z, size(z,1), size(z,2), s1, &
        xmin, xmax, ymin, ymax, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width, &
        xg, yg, size(z,1) )

      end subroutine

!***********************************************************************

      subroutine plshade_single_tr(z, defined, &
        xmin, xmax, ymin, ymax, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width, tr)

      implicit none
      character(len=*) defined
      integer sh_cmap, sh_width
      integer min_color, min_width, max_color, max_width
      real(kind=plflt) shade_min, shade_max, sh_color
      real(kind=plflt) z(:,:), xmin, xmax, ymin, ymax
      real(kind=plflt) tr(6)

      include 'sfstubs.h'

      s1 = transfer( string1, s1 )
      call plshade7(z, size(z,1), size(z,2), s1, &
        xmin, xmax, ymin, ymax, &
        shade_min, shade_max, &
        sh_cmap, sh_color, sh_width, &
        min_color, min_width, max_color, max_width, tr, size(z,1))

      end subroutine

!***********************************************************************

      subroutine plshades_multiple_0(z, defined, &
        xmin, xmax, ymin, ymax, &
        clevel, fill_width, &
        cont_color, cont_width )

      implicit none
      character defined*(*)
      integer fill_width, cont_color, cont_width
      real(kind=plflt) clevel(:)
      real(kind=plflt) z(:,:), xmin, xmax, ymin, ymax

      include 'sfstubs.h'

!	call plstrf2c(dnam, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plshades07(z, size(z,1), size(z,2), s1, &
        xmin, xmax, ymin, ymax, &
        clevel, size(clevel), fill_width, &
        cont_color, cont_width, size(z,1))

      end subroutine

!***********************************************************************

      subroutine plshades_multiple_1(z, defined, &
        xmin, xmax, ymin, ymax, &
        clevel, fill_width, &
        cont_color, cont_width, xg1, yg1)

      implicit none
      character defined*(*)
      integer fill_width, cont_color, cont_width
      real(kind=plflt) clevel(:)
      real(kind=plflt) z(:,:), xmin, xmax, ymin, ymax, &
                       xg1(:), yg1(:)

      include 'sfstubs.h'

!	call plstrf2c(dnam, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plshades17(z, size(z,1), size(z,2), s1, &
        xmin, xmax, ymin, ymax, &
        clevel, size(clevel), fill_width, &
        cont_color, cont_width, xg1, yg1, size(z,1))

      end subroutine

!***********************************************************************

      subroutine plshades_multiple_2(z, defined, &
        xmin, xmax, ymin, ymax, &
        clevel, fill_width, &
        cont_color, cont_width, xg2, yg2)

      implicit none
      character defined*(*)
      integer fill_width, cont_color, cont_width
      real(kind=plflt) clevel(:)
      real(kind=plflt) z(:,:), xmin, xmax, ymin, ymax, &
        xg2(:,:), yg2(:,:)

      include 'sfstubs.h'

!	call plstrf2c(dnam, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plshades27(z, size(z,1), size(z,2), s1, &
        xmin, xmax, ymin, ymax, &
        clevel, size(clevel), fill_width, &
        cont_color, cont_width, xg2, yg2, size(z,1))

      end subroutine

!***********************************************************************

      subroutine plshades_multiple_tr(z, defined, &
        xmin, xmax, ymin, ymax, &
        clevel, fill_width, &
        cont_color, cont_width, tr)

      implicit none
      character defined*(*)
      integer fill_width, cont_color, cont_width
      real(kind=plflt) clevel(:)
      real(kind=plflt) z(:,:), xmin, xmax, ymin, ymax
      real(kind=plflt) tr(6)

      include 'sfstubs.h'

!	call plstrf2c(dnam, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plshades7(z, size(z,1), size(z,2), s1, &
        xmin, xmax, ymin, ymax, &
        clevel, size(clevel), fill_width, &
        cont_color, cont_width, tr, size(z,1))

      end subroutine

!***********************************************************************

      subroutine pllab(xlab,ylab,title)

      implicit none
      character*(*) xlab,ylab,title

      include 'sfstubs.h'

      call plstrf2c(xlab, string1, maxlen)
      call plstrf2c(ylab, string2, maxlen)
      call plstrf2c(title, string3, maxlen)

      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      s3 = transfer( string3, s3 )
      call pllab7(s1,s2,s3)

      end subroutine

!***********************************************************************

      subroutine plmtex(side,disp,pos,xjust,text)

      implicit none
      real(kind=plflt) disp, pos, xjust
      character*(*) side, text

      include 'sfstubs.h'

      call plstrf2c(side, string1, maxlen)
      call plstrf2c(text, string2, maxlen)

      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      call plmtex7(s1,disp,pos,xjust,s2)

      end subroutine

!***********************************************************************

      subroutine plmtex3(side,disp,pos,xjust,text)

      implicit none
      real(kind=plflt) disp, pos, xjust
      character*(*) side, text

      include 'sfstubs.h'

      call plstrf2c(side, string1, maxlen)
      call plstrf2c(text, string2, maxlen)

      s1 = transfer( string1, s1 )
      s2 = transfer( string2, s2 )
      call plmtex37(s1,disp,pos,xjust,s2)

      end subroutine

!***********************************************************************

      subroutine plptex(x,y,dx,dy,xjust,text)

      implicit none
      real(kind=plflt) x, y, dx, dy, xjust
      character*(*) text

      include 'sfstubs.h'

      call plstrf2c(text, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plptex7(x,y,dx,dy,xjust,s1)

      end subroutine

!***********************************************************************

      subroutine plptex3(x,y,z,dx,dy,dz,sx,sy,sz,xjust,text)

      implicit none
      real(kind=plflt) x, y, z, dx, dy, dz, sx, sy, sz, xjust
      character*(*) text

      include 'sfstubs.h'

      call plstrf2c(text, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plptex37(x,y,z,dx,dy,dz,sx,sy,sz,xjust,s1)

      end subroutine

!***********************************************************************

      subroutine plstart(devname, nx, ny)

      implicit none
      character*(*) devname
      integer nx, ny

      include 'sfstubs.h'

      call plstrf2c(devname, string1, maxlen)

      s1 = transfer( string1, s1 )
      call plstart7(s1, nx, ny)

      end subroutine

!***********************************************************************

      subroutine plmap(mapform,mapname,minx,maxx,miny,maxy)

      implicit none
      real(kind=plflt) minx, maxx, miny, maxy
      character*(*) mapname
      external mapform

      include 'sfstubs.h'

      call plstrf2c(mapname, string1, maxlen)

      call plsetmapformc(mapform)
      s1 = transfer( string1, s1 )
      call plmapc(s1,minx,maxx,miny,maxy)

      end subroutine

!***********************************************************************

      subroutine plmeridians(mapform,dlong,dlat,minlong,maxlong, &
                             minlat,maxlat)

      implicit none
      real(kind=plflt) dlong, dlat, minlong, maxlong, minlat, maxlat
      external mapform

      include 'sfstubs.h'

      call plsetmapformc(mapform)
      call plmeridiansc(dlong,dlat,minlong,maxlong,minlat,maxlat)

      end subroutine

