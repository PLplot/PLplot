# $Id$

#  Copyright (C) 2007 Alan W. Irwin

# plmtex3, plptex3 demo.
#
#  This file is part of PLplot.
#
#  PLplot is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Library Public License as published
#  by the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  PLplot is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Library General Public License for more details.
#
#  You should have received a copy of the GNU Library General Public License
#  along with PLplot; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#
from plplot_py_demos import *

# Choose these values to correspond to tick marks.
XPTS = 2
YPTS = 2

NREVOLUTION = 16
NROTATION = 8
NSHEAR = 8

xmin = 0.
xmax = 1.0
xmid = 0.5*(xmax + xmin)
xrange = xmax - xmin

ymin = 0.
ymax = 1.0
ymid = 0.5*(ymax + ymin)
yrange = ymax - ymin

zmin = 0.
zmax = 1.0
zmid = 0.5*(zmax + zmin)
zrange = zmax - zmin

pstring = "The future of our civilization depends on software freedom."
def main():

 if 1:
    x = xmin + (xmax-xmin)*arange(XPTS)/float(XPTS-1)
    y = ymin + (ymax-ymin)*arange(YPTS)/float(YPTS-1)
    z = reshape(0.+zeros(XPTS*YPTS),[XPTS,YPTS])

 if 1:
    # Page 1: Demonstrate inclination and shear capability pattern.
   
    pladv(0)
    plvpor(-0.15, 1.15, -0.05, 1.05)
    plwind(-1.2, 1.2, -0.8, 1.5)
    plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 20., 45.)
   
    plcol0(2)
    plbox3(
    "b", "", xmax-xmin, 0,
    "b", "", ymax-ymin, 0,
    "bcd", "", zmax-zmin, 0)

    # z = zmin.
    plschr(0., 1.0)
    for i in range(NREVOLUTION):
	omega = 2.*pi*(float(i)/float(NREVOLUTION))
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	x_inclination = 0.5*xrange*cos_omega
	y_inclination = 0.5*yrange*sin_omega
	z_inclination = 0.
	x_shear = -0.5*xrange*sin_omega
	y_shear = 0.5*yrange*cos_omega
	z_shear = 0.
	plptex3(
	xmid, ymid, zmin,
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear, 
	0.0, "  revolution")

    # x = xmax.
    plschr(0., 1.0)
    for i in range(NREVOLUTION):
	omega = 2.*pi*(float(i)/float(NREVOLUTION))
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	x_inclination = 0.
	y_inclination = -0.5*yrange*cos_omega
	z_inclination = 0.5*zrange*sin_omega
	x_shear = 0.
	y_shear = 0.5*yrange*sin_omega
	z_shear = 0.5*zrange*cos_omega
	plptex3(
	xmax, ymid, zmid,
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear, 
	0.0, "  revolution")

    # y = ymax.
    plschr(0., 1.0)
    for i in range(NREVOLUTION):
	omega = 2.*pi*(float(i)/float(NREVOLUTION))
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	x_inclination = 0.5*xrange*cos_omega
	y_inclination = 0.
	z_inclination = 0.5*zrange*sin_omega
	x_shear = -0.5*xrange*sin_omega
	y_shear = 0.
	z_shear = 0.5*zrange*cos_omega
	plptex3(
	xmid, ymax, zmid,
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear, 
	0.0, "  revolution")
    # Draw minimal 3D grid to finish defining the 3D box.
    plmesh(x, y, z, DRAW_LINEXY)

    # Page 2: Demonstrate rotation of string around its axis.

    pladv(0)
    plvpor(-0.15, 1.15, -0.05, 1.05)
    plwind(-1.2, 1.2, -0.8, 1.5)
    plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 20., 45.)
   
    plcol0(2)
    plbox3(
    "b", "", xmax-xmin, 0, 
    "b", "", ymax-ymin, 0,
    "bcd", "", zmax-zmin, 0)

    # y = ymax.
    plschr(0., 1.0)
    x_inclination = 1.
    y_inclination = 0.
    z_inclination = 0.
    x_shear = 0.
    for i in range(NROTATION):
	omega = 2.*pi*(float(i)/float(NROTATION))
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	y_shear = 0.5*yrange*sin_omega
	z_shear = 0.5*zrange*cos_omega
	plptex3(
	xmid, ymax, zmax -(zmax-0.2)*(float(i)/float(NROTATION-1)),
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear,
	0.5, "rotation for y = y#dmax#u")

    # x = xmax.
    plschr(0., 1.0)
    x_inclination = 0.
    y_inclination = -1.
    z_inclination = 0.
    y_shear = 0.
    for i in range(NROTATION):
	omega = 2.*pi*(float(i)/float(NROTATION))
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	x_shear = 0.5*xrange*sin_omega
	z_shear = 0.5*zrange*cos_omega
	plptex3(
	xmax, ymid, zmax -(zmax-0.2)*(float(i)/float(NROTATION-1)),
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear,
	0.5, "rotation for x = x#dmax#u")

    # z = zmin.
    plschr(0., 1.0)
    x_inclination = 1.
    y_inclination = 0.
    z_inclination = 0.
    x_shear = 0.
    for i in range(NROTATION):
	omega = 2.*pi*(float(i)/float(NROTATION))
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	y_shear = 0.5*yrange*cos_omega
	z_shear = 0.5*zrange*sin_omega
	plptex3(
	xmid, ymax -(ymax-0.2)*(float(i)/float(NROTATION-1)), zmin,
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear,
	0.5, "rotation for z = z#dmin#u")

    # Draw minimal 3D grid to finish defining the 3D box.
    plmesh(x, y, z, DRAW_LINEXY)

    # Page 3: Demonstrate shear of string along its axis.

    # Work around xcairo and pngcairo (but not pscairo) problems for 
    # shear vector too close to axis of string. (N.B. no workaround
    # would be domega = 0.)
    domega = 0.05
    pladv(0)
    plvpor(-0.15, 1.15, -0.05, 1.05)
    plwind(-1.2, 1.2, -0.8, 1.5)
    plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 20., 45.)
   
    plcol0(2)
    plbox3(
    "b", "", xmax-xmin, 0, 
    "b", "", ymax-ymin, 0,
    "bcd", "", zmax-zmin, 0)

    # y = ymax.
    plschr(0., 1.0)
    x_inclination = 1.
    y_inclination = 0.
    z_inclination = 0.
    y_shear = 0.
    for i in range(NSHEAR):
	omega = domega + 2.*pi*(float(i)/float(NSHEAR))
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	x_shear = 0.5*xrange*sin_omega
	z_shear = 0.5*zrange*cos_omega
	plptex3(
	xmid, ymax, zmax -(zmax-0.2)*(float(i)/float(NSHEAR-1)),
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear,
	0.5, "shear for y = y#dmax#u")

    # x = xmax.
    plschr(0., 1.0)
    x_inclination = 0.
    y_inclination = -1.
    z_inclination = 0.
    x_shear = 0.
    for i in range(NSHEAR):
	omega = domega + 2.*pi*(float(i)/float(NSHEAR))
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	y_shear = -0.5*yrange*sin_omega
	z_shear = 0.5*zrange*cos_omega
	plptex3(
	xmax, ymid, zmax -(zmax-0.2)*(float(i)/float(NSHEAR-1)),
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear,
	0.5, "shear for x = x#dmax#u")

    # z = zmin.
    plschr(0., 1.0)
    x_inclination = 1.
    y_inclination = 0.
    z_inclination = 0.
    z_shear = 0.
    for i in range(NSHEAR):
	omega = domega + 2.*pi*(float(i)/float(NSHEAR))
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	y_shear = 0.5*yrange*cos_omega
	x_shear = 0.5*xrange*sin_omega
	plptex3(
	xmid, ymax -(ymax-0.2)*(float(i)/float(NSHEAR-1)), zmin,
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear,
	0.5, "shear for z = z#dmin#u")

    # Draw minimal 3D grid to finish defining the 3D box.
    plmesh(x, y, z, DRAW_LINEXY)

    # Page 4: Demonstrate drawing a string on a 3D path.
    pladv(0)
    plvpor(-0.15, 1.15, -0.05, 1.05)
    plwind(-1.2, 1.2, -0.8, 1.5)
    plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 40., -30.)
   
    plcol0(2)
    plbox3(
    "b", "", xmax-xmin, 0, 
    "b", "", ymax-ymin, 0,
    "bcd", "", zmax-zmin, 0)

    plschr(0., 1.2)
    # domega controls the spacing between the various characters of the
    # string and also the maximum value of omega for the given number
    # of characters in *pstring.
    domega = 2.*pi/len(pstring)
    omega = 0.
    # 3D function is a helix of the given radius and pitch
    radius = 0.5
    pitch = 1./(2.*pi)
    for character in pstring:
	sin_omega = sin(omega)
	cos_omega = cos(omega)
	xpos = xmid + radius*sin_omega
	ypos = ymid - radius*cos_omega
	zpos = zmin + pitch*omega
	# In general, the inclination is proportional to the derivative of 
	# the position wrt theta.
	x_inclination = radius*cos_omega
	y_inclination = radius*sin_omega
	z_inclination = pitch
	# The shear vector should be perpendicular to the 3D line with Z
	# component maximized, but for low pitch a good approximation is
	# a constant vector that is parallel to the Z axis.
	x_shear = 0.
	y_shear = 0.
	z_shear = 1.
	plptex3(
	xpos, ypos, zpos,
	x_inclination, y_inclination, z_inclination,
	x_shear, y_shear, z_shear,
	0.5, character)
	omega += domega

    # Draw minimal 3D grid to finish defining the 3D box.
    plmesh(x, y, z, DRAW_LINEXY)

    # Page 5: Demonstrate plmtex3 axis labelling capability.
    pladv(0)
    plvpor(-0.15, 1.15, -0.05, 1.05)
    plwind(-1.2, 1.2, -0.8, 1.5)
    plw3d(1.0, 1.0, 1.0, xmin, xmax, ymin, ymax, zmin, zmax, 20., 45.)
   
    plcol0(2)
    plbox3(
    "b", "", xmax-xmin, 0, 
    "b", "", ymax-ymin, 0,
    "bcd", "", zmax-zmin, 0)

    plschr(0., 1.0)
    plmtex3("xp", 3.0, 0.5, 0.5, "Arbitrarily displaced")
    plmtex3("xp", 4.5, 0.5, 0.5, "primary X-axis label")
    plmtex3("xs", -2.5, 0.5, 0.5, "Arbitrarily displaced")
    plmtex3("xs", -1.0, 0.5, 0.5, "secondary X-axis label")
    plmtex3("yp", 3.0, 0.5, 0.5, "Arbitrarily displaced")
    plmtex3("yp", 4.5, 0.5, 0.5, "primary Y-axis label")
    plmtex3("ys", -2.5, 0.5, 0.5, "Arbitrarily displaced")
    plmtex3("ys", -1.0, 0.5, 0.5, "secondary Y-axis label")
    plmtex3("zp", 4.5, 0.5, 0.5, "Arbitrarily displaced")
    plmtex3("zp", 3.0, 0.5, 0.5, "primary Z-axis label")
    plmtex3("zs", -2.5, 0.5, 0.5, "Arbitrarily displaced")
    plmtex3("zs", -1.0, 0.5, 0.5, "secondary Z-axis label")
    # Draw minimal 3D grid to finish defining the 3D box.
    plmesh(x, y, z, DRAW_LINEXY)
	    
main()
