c
c     program example16
c     =================
c
c Demonstration of plshade plotting      

c Reduce colors in cmap 0 so that cmap 1 is useful on a 16-color display

      call plscmap0n(3)

c Initialize plplot

      call plinit()

c Rectangular coordinate plot

      call rect()

c Polar coordinate plot

      call polar()

      call plend
      end

c----------------------------------------------------------------------------!
c Plot function using the identity transform

      subroutine rect()

      implicit character(a-z)
      integer	NX, NY, NCONTR
      parameter (NX = 35, NY = 46, NCONTR = 14)

      real	z(NX, NY), w(NX, NY), clevel(NCONTR)
      real	xmin, xmax, ymin, ymax, zmin, zmax, x, y
      real	shade_min, shade_max, sh_color
      integer	i, j, sh_cmap, sh_width
      integer	min_color, min_width, max_color, max_width

      xmin = -1.
      ymin = -1.
      xmax =  1.
      ymax =  1.

c Set up for plshade call

      sh_cmap = 1
      min_color = 1
      min_width = 0
      max_color = 0
      max_width = 0

c Set up data arrays

      do 12 i = 1, NX
         x = (i - 1 - (NX/2)) / real(NX/2)
         do 10 j = 1, NY
            y = (j - 1 - (NY/2)) / real(NY/2) - 1.0
            z(i,j) = x*x - y*y + (x - y) / (x*x + y*y + 0.1)
            w(i,j) = 2*x*y
 10      continue
 12   continue

      call a2mnmx(z, NX, NY, zmin, zmax)
      do 20 i = 1, NCONTR
         clevel(i) = zmin + (zmax - zmin) * (i + 0.5) / real(NCONTR)
 20   continue

c Plot using identity transform 

      call pladv(0)
      call plvpor(0.1, 0.9, 0.1, 0.9)
      call plwind(-1.0, 1.0, -1.0, 1.0)

      do 100 i = 1, NCONTR
         shade_min = zmin + (zmax - zmin) * real(i - 1) / real(NCONTR)
         shade_max = zmin + (zmax - zmin) * real(i)     / real(NCONTR)
         sh_color = real(i - 1) / real(NCONTR - 1)
         sh_width = 2
         call plpsty(0)

         call plshade0(z, NX, NY, ' ', 
     &        -1., 1., -1., 1., 
     &        shade_min, shade_max, 
     &        sh_cmap, sh_color, sh_width, 
     &        min_color, min_width, max_color, max_width)

 100  continue

      call plcol(1)
      call plbox('bcnst', 0.0, 0, 'bcnstv', 0.0, 0)
      call plcol(2)
      call pllab('distance', 'altitude', 'Bogon flux')

      return
      end

c----------------------------------------------------------------------------!
c Routine for demonstrating use of transformation arrays in contour plots.

      subroutine polar()

      implicit character(a-z)
      integer	NX, NY, NCONTR, NBDRY
      real	TWOPI
      parameter (NX = 40, NY = 64, NCONTR = 14, NBDRY=200)
      parameter (TWOPI=6.2831853071795864768)

      real	z(NX, NY), ztmp(NX, NY+1)
      real	xg(NX, NY+1), yg(NX, NY+1), xtm(NBDRY), ytm(NBDRY)
      real	clevel(NCONTR)
      real	xmin, xmax, ymin, ymax, zmin, zmax
      real	xpmin, xpmax, ypmin, ypmax
      real	x, y, r, theta, rmax, x0, y0
      real	eps, q1, d1, q1i, d1i, q2, d2, q2i, d2i
      real	div1, div1i, div2, div2i

      real	shade_min, shade_max, sh_color
      real	xtick, ytick
      integer	nxsub, nysub
      integer	ncolbox, ncollab
      integer	i, j, kx, lx, ky, ly
      integer	sh_cmap, sh_width
      integer	min_color, min_width, max_color, max_width
      character*8 xopt, yopt

c Set up for plshade call

      sh_cmap = 1
      min_color = 1
      min_width = 0
      max_color = 0
      max_width = 0

      kx = 1
      lx = NX
      ky = 1
      ly = NY

c Set up r-theta grids
c Tack on extra cell in theta to handle periodicity.

      do 12 i = 1, NX
         r = i - 0.5
         do 10 j = 1, NY
            theta = TWOPI/float(NY) * (j-0.5)
            xg(i,j) = r * cos(theta)
            yg(i,j) = r * sin(theta)
 10      continue
         xg(i, NY+1) = xg(i, 1)
         yg(i, NY+1) = yg(i, 1)
 12   continue
      call a2mnmx(xg, NX, NY, xmin, xmax)
      call a2mnmx(yg, NX, NY, ymin, ymax)

      rmax = r
      x0 = (xmin + xmax)/2.
      y0 = (ymin + ymax)/2.

c Potential inside a conducting cylinder (or sphere) by method of images.
c Charge 1 is placed at (d1, d1), with image charge at (d2, d2).
c Charge 2 is placed at (d1, -d1), with image charge at (d2, -d2).
c Also put in smoothing term at small distances.

      eps = 2.

      q1 = 1.
      d1 = r/4.

      q1i = - q1*r/d1
      d1i = r**2/d1

      q2 = -1.
      d2 = r/4.

      q2i = - q2*r/d2
      d2i = r**2/d2

      do 22 i = 1, NX
         do 20 j = 1, NY
            div1 = sqrt((xg(i,j)-d1)**2 + (yg(i,j)-d1)**2 + eps**2)
            div1i = sqrt((xg(i,j)-d1i)**2 + (yg(i,j)-d1i)**2 + eps**2)

            div2 = sqrt((xg(i,j)-d2)**2 + (yg(i,j)+d2)**2 + eps**2)
            div2i = sqrt((xg(i,j)-d2i)**2 + (yg(i,j)+d2i)**2 + eps**2)

            z(i,j) = q1/div1 + q1i/div1i + q2/div2 + q2i/div2i
 20      continue
 22   continue

c Tack on extra cell in theta to handle periodicity.

      do 32 i = 1, NX
         do 30 j = 1, NY
            ztmp(i,j) = z(i,j)
 30      continue
         ztmp(i, NY+1) = z(i, 1)
 32   continue
      call a2mnmx(z, NX, NY, zmin, zmax)

c Set up contour levels.

      do 40 i = 1, NCONTR
         clevel(i) = zmin + (i-0.5)*abs(zmax - zmin)/float(NCONTR)
 40   continue

c Advance graphics frame and get ready to plot.

      ncolbox = 1
      ncollab = 2

      call pladv(0)
      call plcol(ncolbox)

c Scale window to user coordinates.
c Make a bit larger so the boundary does not get clipped.

      eps = 0.05
      xpmin = xmin - abs(xmin)*eps
      xpmax = xmax + abs(xmax)*eps
      ypmin = ymin - abs(ymin)*eps
      ypmax = ymax + abs(ymax)*eps

      call plvpas(0.1, 0.9, 0.1, 0.9, 1.0)
      call plwind(xpmin, xpmax, ypmin, ypmax)

      xopt = ' '
      yopt = ' '
      xtick = 0.
      nxsub = 0
      ytick = 0.
      nysub = 0

      call plbox(xopt, xtick, nxsub, yopt, ytick, nysub)

c Call plotter once for z < 0 (dashed), once for z > 0 (solid lines).

      do 100 i = 1, NCONTR
         shade_min = zmin + (zmax - zmin) * real(i - 1) / real(NCONTR)
         shade_max = zmin + (zmax - zmin) * real(i)     / real(NCONTR)
         sh_color = real(i - 1) / real(NCONTR - 1)
         sh_width = 2
         call plpsty(0)

         call plshade2(z, NX, NY, ' ', 
     &        -1., 1., -1., 1., 
     &        shade_min, shade_max, 
     &        sh_cmap, sh_color, sh_width, 
     &        min_color, min_width, max_color, max_width, xg, yg)

 100  continue

c Draw boundary.

      do 110 i = 1, NBDRY
         theta = (TWOPI)/(NBDRY-1) * float(i-1)
         xtm(i) = x0 + rmax * cos(theta)
         ytm(i) = y0 + rmax * sin(theta)
 110  continue
      call plcol(ncolbox)
      call plline(NBDRY, xtm, ytm)

      call plcol(ncollab)
      call pllab(' ', ' ', 
     &'Shielded potential of charges in a conducting sphere')

      return
      end

c----------------------------------------------------------------------------!
c Subroutine a2mnmx
c----------------------------------------------------------------------------!
c Minimum and the maximum elements of a 2-d array.
c----------------------------------------------------------------------------!

      subroutine a2mnmx(f, nx, ny, fmin, fmax)

      integer	nx, ny
      real	f(nx, ny), fmin, fmax

      fmax = f(1, 1)
      fmin = fmax
      do 12 j = 1, ny
         do 10 i = 1, nx
            fmax = max(fmax, f(i, j))
            fmin = min(fmin, f(i, j))
 10      continue
 12   continue

      return 
      end
