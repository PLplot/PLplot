# $Id$

#  Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008 Alan W. Irwin

#  Bar chart demo.
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

# main
#
# Does a simple bar chart, using color fill.  If color fill is
# unavailable, pattern fill is used instead (automatic).

def main():

    pladv(0)
    plvsta()
    plwind(1980.0, 1990.0, 0.0, 35.0)
    plbox("bc", 1.0, 0, "bcnv", 10.0, 0)
    plcol0(2)
    pllab("Year", "Widget Sales (millions)", "#frPLplot Example 12")

    y0 = [5, 15, 12, 24, 28, 30, 20, 8, 12, 3]

    for i in range(10):
	plcol0(i + 1)
	plpsty(0)
	fbox( (1980. + i), y0[i] )
	string = `y0[i]`
	plptex((1980. + i + .5), (y0[i] + 1.), 1.0, 0.0, .5, string)
	string = `1980 + i`
	plmtex("b", 1.0, ((i + 1) * .1 - .05), 0.5, string)

    # Restore defaults
    #plcol0(1)

def fbox( x0, y0 ):

    x = [x0, x0, x0 + 1., x0 + 1.]
    y = [0., y0, y0, 0.]
    plfill(x, y)
    plcol0(1)
    pllsty(1)
    plline(x, y)

main()
