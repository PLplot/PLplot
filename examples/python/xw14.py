#!/usr/bin/env python

#	Demo of multiple stream/window capability (requires Tk or Tcl-DP).

import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

# This demo could not be converted to Python because it uses a part of
# the plplot API that has not yet been implemented in Python.
print "This python equivalent of x14c not implemented yet"
