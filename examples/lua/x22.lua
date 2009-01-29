--[[ $Id$

	Simple vector plot example

  Copyright (C) 2008  Werner Smekal

  This file is part of PLplot.

  PLplot is free software you can redistribute it and/or modify
  it under the terms of the GNU General Library Public License as published
  by the Free Software Foundation either version 2 of the License, or
  (at your option) any later version.

  PLplot is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Library General Public License for more details.

  You should have received a copy of the GNU Library General Public License
  along with PLplot if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
--]]

-- initialise Lua bindings for PLplot examples.
dofile("plplot_examples.lua")

-- Pairs of points making the line segments used to plot the user defined arrow 
arrow_x  = { -0.5, 0.5, 0.3, 0.5,  0.3, 0.5 }
arrow_y  = {    0,   0, 0.2,   0, -0.2,   0 }
arrow2_x = { -0.5, 0.3, 0.3, 0.5,  0.3, 0.3 }
arrow2_y = {    0,   0, 0.2,   0, -0.2,   0 }


-- Vector plot of the circulation about the origin
function circulation()
	nx = 20
	ny = 20
	dx = 1
	dy = 1

	xmin = -nx/2*dx
	xmax = nx/2*dx
	ymin = -ny/2*dy
	ymax = ny/2*dy

	cgrid2["xg"] = {}
	cgrid2["yg"] = {}
	cgrid2["nx"] = nx
	cgrid2["ny"] = ny
	u = {}
	v = {}
	
	-- Create data - circulation around the origin. 
	for i=1, nx do	
		x = (i-1-nx/2+0.5)*dx
		cgrid2["xg"][i] = {}
		cgrid2["yg"][i] = {}
		u[i] = {}
		v[i] = {}
		for j=1, ny do
			y = (j-1-ny/2+0.5)*dy
			cgrid2xg[i][j] = x
			cgrid2yg[i][j] = y
			u[i][j] = y
			v[i][j] = -x
		end
	end

	-- Plot vectors with default arrows 
	pl.env(xmin, xmax, ymin, ymax, 0, 0)
	pl.lab("(x)", "(y)", "#frPLplot Example 22 - circulation")
	pl.col0(2)
	pl.vect(u, v, nx, ny, 0.0, pltr2, (void *)&cgrid2 )
	pl.col0(1)
end


-- Vector plot of flow through a constricted pipe
function constriction()
	nx = 20
	ny = 20
	dx = 1
	dy = 1

	xmin = -nx/2*dx
	xmax = nx/2*dx
	ymin = -ny/2*dy
	ymax = ny/2*dy

	cgrid2["xg"] = {}
	cgrid2["yg"] = {}
	cgrid2["nx"] = nx
	cgrid2["ny"] = ny
	u = {}
	v = {}
	
	Q = 2.0
	for i=1, nx do	
		x = (i-1-nx/2+0.5)*dx
		cgrid2["xg"][i] = {}
		cgrid2["yg"][i] = {}
		u[i] = {}
		v[i] = {}
		for j=1, ny do
			y = (j-1-ny/2+0.5)*dy
	    cgrid2["xg"][i][j] = x
	    cgrid2["yg"][i][j] = y
	    b = ymax/4*(3-math.cos(math.pi*x/xmax))
			if math.abs(y)<b then
				dbdx = ymax/4*math.sin(math.pi*x/xmax)*y/b
				u[i][j] = Q*ymax/b
				v[i][j] = dbdx*u[i][j]
			else
				u[i][j] = 0
				v[i][j] = 0
			end
		end
	end

	pl.env(xmin, xmax, ymin, ymax, 0, 0)
	pl.lab("(x)", "(y)", "#frPLplot Example 22 - constriction")
	pl.col0(2)
	pl.vect(u, v, nx, ny, -0.5, pltr2, cgrid2)
	pl.col0(1)
end


function f2mnmx(f, nx, ny)
	fmax = f[1][1]
	fmin = fmax

	for i=1, nx do
		for j=1, ny do
	    fmax = math.max(fmax, f[i][j])
	    fmin = math.min(fmin, f[i][j])
		end
	end
		
	return fmin, fmax
}

-- Vector plot of the gradient of a shielded potential (see example 9)
 
void potential() {
#if !defined(WIN32)
    const int nper = 100
    const int nlevel = 10
    const int nr = 20
    const int ntheta = 20
#else
#define nper    100
#define nlevel   10
#define nr       20
#define ntheta   20
#endif

    int i,j
    PLFLT eps, q1, d1, q1i, d1i, q2, d2, q2i, d2i
    PLFLT div1, div1i, div2, div2i
    PLFLT **u, **v, **z, r, theta, x, y, dz
    PLFLT xmin, xmax, ymin, ymax, rmax, zmax, zmin
    PLFLT px[nper], py[nper], clevel[nlevel]
    PLcGrid2 cgrid2


    -- Create data to be plotted 
    plAlloc2dGrid(&cgrid2.xg,nr,ntheta)
    plAlloc2dGrid(&cgrid2.yg,nr,ntheta)
    plAlloc2dGrid(&u,nr,ntheta)
    plAlloc2dGrid(&v,nr,ntheta)
    plAlloc2dGrid(&z,nr,ntheta)

    cgrid2.nx = nr
    cgrid2.ny = ntheta

    -- Potential inside a conducting cylinder (or sphere) by method of images.
     * Charge 1 is placed at (d1, d1), with image charge at (d2, d2).
     * Charge 2 is placed at (d1, -d1), with image charge at (d2, -d2).
     * Also put in smoothing term at small distances.
     

    rmax = (double) nr

    eps = 2.

    q1 = 1.
    d1 = rmax/4.

    q1i = - q1*rmax/d1
    d1i = pow(rmax, 2.)/d1

    q2 = -1.
    d2 = rmax/4.

    q2i = - q2*rmax/d2
    d2i = pow(rmax, 2.)/d2

    for (i = 0 i < nr i++) {
	r = 0.5 + (double) i
	for (j = 0 j < ntheta j++) {
	    theta = 2.*M_PI/(ntheta-1)*(0.5+(double)j)
	    x = r*cos(theta)
	    y = r*sin(theta)
	    cgrid2.xg[i][j] = x
	    cgrid2.yg[i][j] = y
	    div1 = sqrt(pow(x-d1, 2.) + pow(y-d1, 2.) + pow(eps, 2.))
	    div1i = sqrt(pow(x-d1i, 2.) + pow(y-d1i, 2.) + pow(eps, 2.))
	    div2 = sqrt(pow(x-d2, 2.) + pow(y+d2, 2.) + pow(eps, 2.))
	    div2i = sqrt(pow(x-d2i, 2.) + pow(y+d2i, 2.) + pow(eps, 2.))
	    z[i][j] = q1/div1 + q1i/div1i + q2/div2 + q2i/div2i
	    u[i][j] = -q1*(x-d1)/pow(div1,3.) - q1i*(x-d1i)/pow(div1i,3.0)
		- q2*(x-d2)/pow(div2,3.) - q2i*(x-d2i)/pow(div2i,3.)
	    v[i][j] = -q1*(y-d1)/pow(div1,3.) - q1i*(y-d1i)/pow(div1i,3.0)
		- q2*(y+d2)/pow(div2,3.) - q2i*(y+d2i)/pow(div2i,3.)
	}
    }

    f2mnmx(cgrid2.xg, nr, ntheta, &xmin, &xmax)
    f2mnmx(cgrid2.yg, nr, ntheta, &ymin, &ymax)
    f2mnmx(z, nr, ntheta, &zmin, &zmax)

    plenv(xmin, xmax, ymin, ymax, 0, 0)
    pllab("(x)", "(y)", "#frPLplot Example 22 - potential gradient vector plot")
    -- Plot contours of the potential 
    dz = (zmax-zmin)/(double) nlevel
    for (i = 0 i < nlevel i++) {
	clevel[i] = zmin + ((double) i + 0.5)*dz
    }
    plcol0(3)
    pllsty(2)
    plcont(z,nr,ntheta,1,nr,1,ntheta,clevel,nlevel,pltr2,(void *) &cgrid2)
    pllsty(1)
    plcol0(1)

    -- Plot the vectors of the gradient of the potential 
    plcol0(2)
    plvect(u,v,nr,ntheta,25.0,pltr2,(void *)&cgrid2)
    plcol0(1)

    -- Plot the perimeter of the cylinder 
    for (i=0i<nperi++) {
	theta = (2.*M_PI/(nper-1))*(double)i
	px[i] = rmax*cos(theta)
	py[i] = rmax*sin(theta)
    }
    plline(nper,px,py)

    plFree2dGrid(z,nr,ntheta)
    plFree2dGrid(cgrid2.xg,nr,ntheta)
    plFree2dGrid(cgrid2.yg,nr,ntheta)
    plFree2dGrid(u,nr,ntheta)
    plFree2dGrid(v,nr,ntheta)


}


----------------------------------------------------------------------------
-- main
--
-- Generates several simple vector plots.
----------------------------------------------------------------------------

-- Parse and process command line arguments 
pl.parseopts(arg, pl.PL_PARSE_FULL)

-- Initialize plplot 
pl.init()

circulation()

narr = 6
fill = 0

-- Set arrow style using arrow_x and arrow_y then
-- plot using these arrows.
pl.svect(arrow_x, arrow_y, narr, fill)
constriction()

-- Set arrow style using arrow2_x and arrow2_y then
-- plot using these filled arrows. 
fill = 1
pl.svect(arrow2_x, arrow2_y, narr, fill)
constriction()

potential()

pl.plend()
