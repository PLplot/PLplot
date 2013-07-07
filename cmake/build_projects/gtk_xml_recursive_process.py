#!/usr/bin/env python

# Recursively read gtk jhbuild xml module files following include
# elements to produce the equivalent single xml file.

# Copyright (C) 2013 Alan W. Irwin

# This file is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# This file is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with this file; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

import sys

def recursive_process(path, filename, if_included):
    fullpath = path + "/" + filename
    fileobject = open(fullpath)
    line = fileobject.readline()
    while line:
        sys.stdout.write(line)
        line = fileobject.readline()
    fileobject.close()

fullpath = sys.argv[1]
index = fullpath.rfind("/")
if index >=0 and len(fullpath) > index + 1:
    path = fullpath[:index]
    filename = fullpath[index+1:]
elif index < 0 and len(fullpath) >= 1:
    path = "."
    filename = fullpath
else:
    print("%s is invalid fullpath name of a file" % fullpath)
    sys.exit()

recursive_process(path, filename, False)
