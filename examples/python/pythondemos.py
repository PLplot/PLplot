#!/usr/bin/env python

# Run all python plplot examples non-interactively.

# Append to effective python path so that can find plplot modules.
from plplot_python_start import *

import sys
from plplot import *

# Parse and process command line arguments
plParseOpts(sys.argv, PL_PARSE_FULL)

# Initialize plplot
plinit()

# xw14.py must be standalone so cannot be run from pythondemos.py
# xw17.py must be standalone and interactive so cannot be run from pythondemos.py

failed = []
for i in range(1, 14) + [15, 16, 18, 19]:
    script = 'xw' + '%02d' % i
    try:
	__import__(script, globals(), locals(), [])
    except:
	failed.append(script)

# Terminate plplot
plend()

if len(failed) != 0:
    import string
    print "Failed script(s): " + string.join(failed,", ")
    sys.exit(1)

