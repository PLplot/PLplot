!***********************************************************************
!
!  sfstubs.f
!
!  This file contains all the fortran stub routines.
!  REAL variables correspond to: real*4
!
!***********************************************************************

	subroutine plsdev(dnam)

        implicit character(a-z)
	character*(*) dnam
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plstrf2c(dnam, string1, maxlen)
	call plsdev7(s1)

	end

!***********************************************************************

	subroutine plsfnam(fnam)

        implicit character(a-z)
	character*(*) fnam
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plstrf2c(fnam, string1, maxlen)
	call plsfnam7(s1)

	end

!***********************************************************************

	subroutine plgfnam(fnam)

        implicit character(a-z)
	character*(*) fnam
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plgfnam7(string1)
        call plstrc2f(string1, fnam)

	end

!***********************************************************************

	subroutine plgver(ver)

        implicit character(a-z)
	character*(*) ver
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plgver7(s1)
        call plstrc2f(string1, ver)

	end

!***********************************************************************

	subroutine plaxes(x0,y0,xopt,xtick,nxsub,yopt,ytick,nysub)

        implicit character(a-z)
	real*4 x0, y0, xtick, ytick
	integer nxsub, nysub
	character*(*) xopt,yopt
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plstrf2c(xopt, string1, maxlen)
	call plstrf2c(yopt, string2, maxlen)

	call plaxes7(x0,y0,s1,xtick,nxsub,s2,ytick,nysub)

	end

!***********************************************************************

	subroutine plbox(xopt,xtick,nxsub,yopt,ytick,nysub)

        implicit character(a-z)
	real*4 xtick, ytick
	integer nxsub, nysub
	character*(*) xopt,yopt
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plstrf2c(xopt, string1, maxlen)
	call plstrf2c(yopt, string2, maxlen)

	call plbox7(s1,xtick,nxsub,s2,ytick,nysub)

	end

!***********************************************************************

	subroutine plbox3(xopt,xlabel,xtick,nxsub,yopt,ylabel,ytick,nysub,
     &		    zopt,zlabel,ztick,nzsub)

        implicit character(a-z)
	real*4 xtick, ytick, ztick
	character*(*) xopt,xlabel,yopt,ylabel,zopt,zlabel
	integer nxsub, nysub, nzsub
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plstrf2c(xopt, string1, maxlen)
	call plstrf2c(xlabel, string2, maxlen)
	call plstrf2c(yopt, string3, maxlen)
	call plstrf2c(ylabel, string4, maxlen)
	call plstrf2c(zopt, string5, maxlen)
	call plstrf2c(zlabel, string6, maxlen)

	call plbox37(s1,s2,xtick,nxsub,
     &               s3,s4,ytick,nysub,
     &               s5,s6,ztick,nzsub)

	end

!***********************************************************************

	subroutine plcon0(z,nx,ny,kx,lx,ky,ly,clevel,nlevel)

        implicit character(a-z)
	integer nx, ny, kx, lx, ky, ly, nlevel
	real*4 z(nx, ny), clevel(nlevel)

	call plcon07(z,nx,ny,kx,lx,ky,ly,clevel,nlevel)

	end

!***********************************************************************

	subroutine plcon1(z,nx,ny,kx,lx,ky,ly,clevel,nlevel,xg,yg)

        implicit character(a-z)
	integer nx, ny, kx, lx, ky, ly, nlevel
	real*4 z(nx, ny), xg(nx), yg(ny), clevel(nlevel)

	call plcon17(z,nx,ny,kx,lx,ky,ly,clevel,nlevel,xg,yg)

	end

!***********************************************************************

	subroutine plcon2(z,nx,ny,kx,lx,ky,ly,clevel,nlevel,xg,yg)

        implicit character(a-z)
	integer nx, ny, kx, lx, ky, ly, nlevel
	real*4 z(nx, ny), xg(nx, ny), yg(nx, ny), clevel(nlevel)

	call plcon27(z,nx,ny,kx,lx,ky,ly,clevel,nlevel,xg,yg)

	end

!***********************************************************************

	subroutine plcont(z,nx,ny,kx,lx,ky,ly,clevel,nlevel)

        implicit character(a-z)
	integer nx, ny, kx, lx, ky, ly, nlevel
	real*4 z(nx, ny), clevel(nlevel)
	real*4 tr(6)
	common /plplot/ tr

	call plcont7(z,nx,ny,kx,lx,ky,ly,clevel,nlevel,tr)

	end

!***********************************************************************

	subroutine plshade0(z, nx, ny, defined, 
     &       xmin, xmax, ymin, ymax,
     &       shade_min, shade_max, 
     &       sh_cmap, sh_color, sh_width, 
     &       min_color, min_width, max_color, max_width)

        implicit character(a-z)
        character defined*(*)
	integer nx, ny, sh_cmap, sh_width
        integer min_color, min_width, max_color, max_width
        real*4 shade_min, shade_max, sh_color
	real*4 z(nx, ny), xmin, xmax, ymin, ymax
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


!	call plstrf2c(dnam, string1, maxlen)

        call plshade07(z, nx, ny, s1, 
     &       xmin, xmax, ymin, ymax,
     &       shade_min, shade_max, 
     &       sh_cmap, sh_color, sh_width, 
     &       min_color, min_width, max_color, max_width)

	end

!***********************************************************************

	subroutine plshade1(z, nx, ny, defined, 
     &       xmin, xmax, ymin, ymax,
     &       shade_min, shade_max, 
     &       sh_cmap, sh_color, sh_width, 
     &       min_color, min_width, max_color, max_width, 
     &       xg, yg)

        implicit character(a-z)
        character defined*(*)
	integer nx, ny, sh_cmap, sh_width
        integer min_color, min_width, max_color, max_width
        real*4 shade_min, shade_max, sh_color
	real*4 z(nx, ny), xmin, xmax, ymin, ymax, xg(nx), yg(ny)
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


!	call plstrf2c(dnam, string1, maxlen)

        call plshade17(z, nx, ny, s1, 
     &       xmin, xmax, ymin, ymax,
     &       shade_min, shade_max, 
     &       sh_cmap, sh_color, sh_width, 
     &       min_color, min_width, max_color, max_width, 
     &       xg, yg)

	end

!***********************************************************************

	subroutine plshade2(z, nx, ny, defined, 
     &       xmin, xmax, ymin, ymax,
     &       shade_min, shade_max, 
     &       sh_cmap, sh_color, sh_width, 
     &       min_color, min_width, max_color, max_width, 
     &       xg, yg)

        implicit character(a-z)
        character defined*(*)
	integer nx, ny, sh_cmap, sh_width
        integer min_color, min_width, max_color, max_width, rectangular
        real*4 shade_min, shade_max, sh_color
	real*4 z(nx, ny), xmin, xmax, ymin, ymax, xg(nx, ny), yg(nx, ny)
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


!	call plstrf2c(dnam, string1, maxlen)

        call plshade27(z, nx, ny, s1, 
     &       xmin, xmax, ymin, ymax,
     &       shade_min, shade_max, 
     &       sh_cmap, sh_color, sh_width, 
     &       min_color, min_width, max_color, max_width, 
     &       xg, yg)

	end

!***********************************************************************

	subroutine pllab(xlab,ylab,title)

        implicit character(a-z)
	character*(*) xlab,ylab,title
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plstrf2c(xlab, string1, maxlen)
	call plstrf2c(ylab, string2, maxlen)
	call plstrf2c(title, string3, maxlen)
	
	call pllab7(s1,s2,s3)

	end

!***********************************************************************

	subroutine plmtex(side,disp,pos,xjust,text)

        implicit character(a-z)
	real*4 disp, pos, xjust
	character*(*) side, text
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plstrf2c(side, string1, maxlen)
	call plstrf2c(text, string2, maxlen)

	call plmtex7(s1,disp,pos,xjust,s2)

	end

!***********************************************************************

	subroutine plptex(x,y,dx,dy,xjust,text)

        implicit character(a-z)
	real*4 x, y, dx, dy, xjust
	character*(*) text
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plstrf2c(text, string1, maxlen)

	call plptex7(x,y,dx,dy,xjust,s1)

	end

!***********************************************************************

	subroutine plstart(devname, nx, ny)

        implicit character(a-z)
	character*(*) devname
	integer nx, ny
        
	integer maxlen
	parameter (maxlen = 320)
	character*(maxlen) string1, string2, string3
	character*(maxlen) string4, string5, string6
	integer s1(80), s2(80), s3(80), s4(80), s5(80), s6(80)
	equivalence ( s1, string1 ), ( s2, string2 )
	equivalence ( s3, string3 ), ( s4, string4 )
	equivalence ( s5, string5 ), ( s6, string6 )
	common /zzplstr1/ string1
	common /zzplstr2/ string2
	common /zzplstr3/ string3
	common /zzplstr4/ string4
	common /zzplstr5/ string5
	common /zzplstr6/ string6


	call plstrf2c(devname, string1, maxlen)

	call plstart7(s1, nx, ny)

	end

!***********************************************************************
