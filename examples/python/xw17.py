#!/usr/bin/env python
#Have not yet converted to arrays.

#	equivalent to x17.c, but not implemented yet.

import math
import sys
import os

module_dir = "@MODULE_DIR@"

if module_dir[0] == '@':
	module_dir = os.getcwd ()

sys.path.insert (0, module_dir)

from Numeric import *
from pl import *

print "This python equivalent of x17c not implemented yet"
