#!/usr/bin/env python

#	Window positioning demo.

import math
import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

# main
#
# Demonstrates absolute positioning of graphs on a page.

def main():

    plinit()

    pladv(0)
    plvpor(0.0, 1.0, 0.0, 1.0)
    plwind(0.0, 1.0, 0.0, 1.0)
    plbox("bc", 0.0, 0, "bc", 0.0, 0)

    plsvpa(50.0, 150.0, 100.0, 150.0)
    plwind(0.0, 1.0, 0.0, 1.0)
    plbox("bc", 0.0, 0, "bc", 0.0, 0)
    plptex(0.5, 0.5, 1.0, 0.0, 0.5, "BOX at (50,150,100,150)")
    pleop()
    
    plend()

main()
