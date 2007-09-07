# plimage demo
#
# Copyright (C) 2007 Andrew Ross

from plplot_py_demos import *

import os.path
import sys

XDIM = 260
YDIM = 220

dbg = 0
nosombrero = 0
nointeractive = 0
f_name = ""

def gray_cmap(num_col):
    r = [0.0, 1.0]
    g = [0.0, 1.0]
    b = [0.0, 1.0]
    pos = [0.0, 1.0]

    plscmap1n(num_col)
    plscmap1l(1, pos, r, g, b)


def save_plot(fname):
    # Get current stream and create a new one
    cur_strm = plgstrm()
    new_strm = plmkstrm()

    # Set new device type and file name - use a known existing driver
    plsdev("psc")
    plsfnam(fname)

    # Copy old stream parameters to new stream, do the save,
    # then close new device.
    plcpstrm(cur_strm,0)
    plreplot()
    plend1()

    # Return to previous stream
    plsstrm(cur_strm)

def read_img(fname):

    if (not os.path.exists(fname)):
        return [1,[],0,0,0]

    fp = open(fname, mode='rb')

    # Check correct version
    ver = fp.readline()

    if (ver != "P5\n"):
        fp.close()
        return [1,[],0,0,0]

    # Skip over comments
    ptr = fp.tell()
    while ( fp.read(1) == '#' ):
        fp.readline()
        ptr = fp.tell()
    fp.seek(ptr)

    # Get width, height, num colors
    [w, h] = fp.readline().split(' ')
    w = int(w)
    h = int(h)
    nc = int(fp.readline())

    tmp = fp.read(w*h)

    img = zeros(w*h)
    for i in range(w):
        for j in range(h):
            img[i*h+j] = ord(tmp[(h-1-j)*w+i])
        
    img = reshape(img,[w,h])
    

    fp.close()
    return [0,img,w,h,nc]
    

def get_clip(xi, xe, yi, ye):


    sx = zeros(5)
    sy = zeros(5)

    start = 0

    st = plxormod(1)

    if (st):
        gin = PLGraphicsIn()
        gin.button = 0
        while (1):
            st = plxormod(0)
            plGetCursor(gin)
            st = plxormod(1)

            if (gin.button == 1):
                xi = gin.wX
                yi = gin.wY
                if (start):
                    plline(sx,sy)  # Clear previous rectangle
                    
                start = 0
                sx[0] = xi
                sy[0] = yi
                sx[4] = xi
                sy[4] = yi

            if (gin.state & 0x100):
                xe = gin.wX
                ye = gin.wY
                if (start):
                    plline(sx,sy)

                start = 1

                sx[2] = xe
                sy[2] = ye
                sx[1] = xe
                sy[1] = yi
                sx[3] = xi
                sy[3] = ye

                plline(sx,sy)  # Draw new rectangle

            if (gin.button == 3 or gin.keysym == 0x0D or gin.keysym == 'Q'):
                if (start):
                    plline(sx,sy) # Clear previous rectangle
                break
            
        st = plxormod(0)  # Leave xor mode
                
        if (xe < xi):
            t = xi
            xi = xe
            xe = t

        if (yi < ye):
            t = yi
            yi = ye
            ye = t

        return [gin.keysym == 'Q', xi, xe, yi, ye]
        
    else:  # Driver has no xormod capability, just do nothing
        return [0, xi, xe, yi, ye]
    
# main
#
#
def main():

    z = reshape(zeros(XDIM*YDIM),[XDIM,YDIM])

    # View image border pixels
    if (dbg):
        plenv(1.0, XDIM, 1.0, YDIM, 1, 1)

        for i in range(XDIM):
            z[i][YDIM-1] = 1.0
            z[i][0] = 1.0

        for j in range(YDIM):
            z[0][j] = 1.0
            z[XDIM-1][j] = 1.0


        pllab("...around a blue square."," ","A ref border should appear...")

        plimage(z, 1.0, XDIM, 1.0, YDIM, 0.0, 0.0, 1.0, XDIM, 1.0, YDIM)

        pladv(0)

    if (not nosombrero):
        plcol0(2)  # Draw a yellow box, useful for diagnostics!
        plenv(0.0, 2.0*pi, 0, 3.0*pi, 1, -1)

        
        x = arange(XDIM)*2.0*pi/(XDIM-1)
        y = arange(YDIM)*3.0*pi/(YDIM-1)

        r = sqrt( multiply.outer(x*x,ones(YDIM)) + multiply.outer(ones(XDIM),y*y)) + 1e-3
        z = sin(r) / r

    pllab("No, an amplitude clipped \"sombrero\"", "", "Saturn?");
    plptex(2., 2., 3., 4., 0., "Transparent image");
    plimage(z, 0., 2.*pi, 0, 3.*pi, 0.05, 1.,0., 2.*pi, 0, 3.*pi); 

    # Save the plot
    if (f_name != ""):
        save_plot(f_name)

    pladv(0)

    # Read Lena image
    # Note: we try two different locations to cover the case where
    # this example is being run from the test_c.sh script
    [err, img, width, height, num_col] = read_img("lena.pgm")
    if (err):
        [err, img, width, height, num_col] = read_img("../lena.pgm")
        if (err):
            plabort("No such file")
            plend()
            sys.exit(1)

    # Set gray colormap
    gray_cmap(num_col)

    # Display Lena
    plenv(1., width, 1., height, 1, -1)

    if (not nointeractive):
        pllab("Set and drag Button 1 to (re)set selection, Button 2 to finish."," ","Lena...")
    else:
        pllab(""," ","Lena...")

    plimage(img, 1., width, 1., height, 0., 0., 1., width, 1., height)

    # selection/expansion demo
    if (not nointeractive): 
        xi = 200.
        xe = 330.
        yi = 280.
        ye = 220.

        [err, xi, xe, yi, ye] = get_clip(xi, xe, yi, ye)
        if (err):
            plend()
            sys.exit(0)
            
        plspause(0)
        pladv(0)

        # display selection only
        plimage(img, 1., width, 1., height, 0., 0., xi, xe, ye, yi)

        plspause(1)
        pladv(0)

        # zoom in selection
        plenv(xi, xe, ye, yi, 1, -1)
        plimage(img, 1., width, 1., height, 0., 0., xi, xe, ye, yi)
        pladv(0)
    plend()
    sys.exit(0)

    
main()
