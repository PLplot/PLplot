#!/bin/sh
# Run *one* python plplot example non-interactively.
# usage:
# ./test_single_python.sh 01
# n.b. must use two digits to specify any number between 01 and 19, to get 
# any of the first, through 19th examples.

python <<EOF

import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from plplot import *

plsfnam("x$1p.ps")
plsdev("psc")
# Initialize plplot
plinit()

import xw$1

# Terminate plplot
plend()
EOF
