#!/usr/bin/env python

#  Copyright (C) 2010 Simon Wood

#  Simple demo of plsmem, plsmema and the memcairo device under Python.
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

import Image
import math
import numpy.numarray

from plplot_python_start import *

from plplot import *

# ---------------------------------------------------
# Build random array (aka fake data)

plseed(1234)
x=[]
y=[]

for i in range(500) :
    x.append(plrandd() * 360)
    y.append(plrandd() * 90)

# compute binned density on 15' boundaries
# 360' gives 24 divisions
# 90' gives 6 divisions

width = 24
height = 6

max_val = 0
data = numpy.numarray.zeros((width,height))

for i in range(len(x)):
    data[int(x[i]/(360/width))][int(y[i]/(90/height))] += 1
    if data[int(x[i]/(360/width))][int(y[i]/(90/height))] > max_val:
        max_val +=1

# ---------------------------------------------------
# Initialise buffer

# Start from a blank canvas
width = 480
height = 320
# Darg gray background (necessarily opaque for RGB format).
background = numpy.numarray.zeros(3, numpy.uint8) + 30
my_buffer = numpy.numarray.zeros((height,width,3), numpy.uint8) + background
# Dark gray semi-transparent background.
background = numpy.numarray.zeros(4, numpy.uint8) + 30
background[3] = 200
my_buffer_alpha = numpy.numarray.zeros((height,width,4), numpy.uint8) + background

'''
# Or open an existing image
# (note 'asarray' will fail as it sees PIL image as an array already and 
# does not perform a copy to make a writable array)
src_img = Image.open("input.png")
my_buffer = numpy.array(src_img.convert('RGB'), numpy.uint8)
(width, height) = src_img.size
'''

# ---------------------------------------------------
# Rectangular plot w/o alpha values

# initialise for mem driver
plsmem(width,height,my_buffer)
plstart ("memcairo", 1, 1);

plcol0(1)
plenv(0, 360, 0, 90, 0, 2)
plcol0(2)
pllab("Azimuth", "Elevation", "Rectangular Sky View")

# plot binned density
plimage(data, 0, 360, 0, 90, 0, max_val, 0, 360, 0, 90)

# plot points
plpoin(x,y,5)

# have to finish plotting so that the plot data gets
# transferred back from the cairo working surface
# to the user supplied memory.
plend1()

# Use fromstring as frombuffer will invert the image
my_image = Image.fromstring("RGB", (width,height), my_buffer)
my_image.save('memcairo_RGB.png')


# ---------------------------------------------------
# Rectangular plot with alpha values

# initialise for mem driver
plsmema(width,height,my_buffer_alpha)
plstart ("memcairo", 1, 1);

plcol0(1)
plenv(0, 360, 0, 90, 0, 2)
plcol0(2)
pllab("Azimuth", "Elevation", "Rectangular Sky View")

# plot binned density
plimage(data, 0, 360, 0, 90, 0, max_val, 0, 360, 0, 90)

# plot points
plpoin(x,y,5)

# have to finish plotting so that the plot data gets
# transferred back from the cairo working surface
# to the user supplied memory.
plend()

# Use fromstring as frombuffer will invert the image
my_image = Image.fromstring("RGBA", (width,height), my_buffer_alpha)
my_image.save('memcairo_RGBA.png')
