#!/usr/bin/env python

# Copyright (C) 2004  Alan W. Irwin
# Copyright (C) 2004  Andrew Ross
#
# This file is part of PLplot.
#
# PLplot is free software; you can redistribute it and/or modify
# it under the terms of the GNU Library General Public License as published
# by the Free Software Foundation; version 2 of the License.
#
# PLplot is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public License
# along with the file PLplot; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA


# Run all python plplot examples non-interactively.

# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *

# Parse and process command line arguments
plparseopts(sys.argv, PL_PARSE_FULL)

# Initialize plplot
plinit()

# Exclude interactive demos 14, 17, 31 which call plinit in the xw??.py 
# script
failed = []
for i in range(1, 14)+[15,16]+range(18,31):
    script = 'xw' + '%02d' % i
    try:
	__import__(script, globals(), locals(), [])
    except:
	failed.append(script)
    # Reset color here rather than in individual examples to ensure results
    # for individual tests are identical to C versions
    plcol0(1)

# Terminate plplot
plend()

if len(failed) != 0:
    import string
    print "Failed script(s): " + string.join(failed,", ")
    sys.exit(1)

