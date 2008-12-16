# $Id$

#  Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008 Alan W. Irwin

#  Font demo.
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
# Displays the entire "plsym" symbol (font) set.

def main():

    base = [0, 200, 500, 600, 700, 800, 900, 2000, 2100,
	    2200, 2300, 2400, 2500, 2600, 2700, 2800, 2900]

    for l in range(17):
	pladv(0)

	# Set up viewport and window

	plcol0(2)
	plvpor(0.15, 0.95, 0.1, 0.9)
	plwind(0.0, 1.0, 0.0, 1.0)

	# Draw the grid using plbox

	plbox("bcg", 0.1, 0, "bcg", 0.1, 0)

	# Write the digits below the frame

	plcol0(15)
	for i in range(10):
	    plmtex("b", 1.5, (0.1 * i + 0.05), 0.5, `i`)

	k = 0
	for i in range(10):

	    # Write the digits to the left of the frame

	    text = `base[l] + 10 * i`
	    plmtex("lv", 1.0, (0.95 - 0.1 * i), 1.0, text)

	    for j in range(10):
		x = [ 0.1 * j + 0.05 ]
		y = [ 0.95 - 0.1 * i ]

		# Display the symbols

		plsym(x, y, base[l] + k)
		k = k + 1

	plmtex("t", 1.5, 0.5, 0.5, "PLplot Example 7 - PLSYM symbols")

    # Restore defaults
    #plcol0(1)

main()
