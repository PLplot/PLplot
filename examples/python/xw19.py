#!/usr/bin/env python
#Have not yet converted to arrays.

#	Illustrates backdrop plotting of world, US maps.
#	Contributed by Wesley Ebisuzaki.

# This demo could not be converted to Python because it uses a part of
# the plplot API that has not yet been implemented in Python.

import math
import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

print "This python equivalent of x19c not implemented yet"
