# Grid data demo
#
# Copyright (C) 2007 Andrew Ross

from plplot_py_demos import *
import time

pts = 500
xp = 25
yp = 20
nl = 15
knn_order = 20
threshold = 1.001
wmin = -1e3
randn = 0
rosen = 0


def cmap1_init():
    i = [0, 1]
    h = [240, 0]
    l = [0.6, 0.6]
    s = [0.8, 0.8]

    plscmap1n(256)
    plscmap1l(0, i, h, l, s)

    

    
# main
#
#
def main():

    title = ["Cubic Spline Approximation",
		   "Delaunay Linear Interpolation",
		   "Natural Neighbors Interpolation",
		   "KNN Inv. Distance Weighted",
		   "3NN Linear Interpolation",
		   "4NN Around Inv. Dist. Weighted"]

    opt = [0., 0., 0., 0., 0., 0.]

    xm = -0.2
    ym = -0.2
    xM = 0.8
    yM = 0.8

    opt[2] = wmin
    opt[3] = knn_order
    opt[4] = threshold

    # Create the sampled data
    xt = random(pts)
    yt = random(pts)
    if randn == 0 :
        x = xt + xm
        y = yt + ym
    else:
        x = sqrt(-2.*log(xt))*cos(2.*pi*yt) + xm
        y = sqrt(-2.*log(xt))*sin(2.*pi*yt) + ym

    if rosen == 0:
        r = sqrt( x*x + y*y)
        z = exp(-r*r)*cos(2*pi*r)
    else:
        z = log(pow(1.-x,2) + 100.*pow(y-pow(x,2),2))

    zmin = min(z)
    zmax = max(z)

    # Create regular grid
    xg = xm + (xM-xm)*arange(xp)/(xp-1.)
    yg = ym + (yM-ym)*arange(yp)/(yp-1.)

    xlab = 'Npts='+`pts`+' gridx='+`xp`+' gridy='+`yp`
    plcol0(1)
    plenv(xm,xM,ym,yM,2,0)
    plcol0(15)
    pllab(xlab,'','The original data')
    plcol0(2)
    plpoin(x,y,5)
    pladv(0)

    plssub(3,2)

    for k in range(2):
        pladv(0)
        for alg in range(1,7):
            ct = time.clock()
            zg = reshape(zeros(xp*yp),(xp,yp))
            plgriddata(x, y, z, xg, yg, zg, alg, opt[alg-1])
            xlab = 'time='+`int((time.clock()-ct)*1000)`+' ms'
            ylab = 'opt='+`opt[alg-1]`

            if alg == GRID_CSA or alg == GRID_DTLI or alg == GRID_NNLI or alg == GRID_NNI:
                for i in range(xp):
                    for j in range(yp):
                        # Average (IDW) over the 8 neighbours for Nan
                        if isnan(zg[i][j]):
                            zg[i][j] = 0.0
                            dist = 0.0
                            for ii in range(max(i-1,0),min(i+1,xp+1)):
                                for jj in range(max(j-1,0),min(j+1,yp+1)):
                                    if (not isnan(zg[ii][jj])):
                                        d = abs(ii-i) + abs(jj-j)
                                        if (d != 1.0) :
                                            d = 1.4142
                                        zg[i][j] += zg[ii][jj]/(d*d)
                                        dist += d
                                        
                            if dist != 0.0 :
                                zg[i][j] /= dist
                            else:
                                zg[i][j] = zmin
            
            lzM = max(zg.flat)
            lzm = min(zg.flat)

            plcol0(1)
            pladv(alg)

            if (k == 0):
                lzm = min(lzm,zmin)
                lzM = max(lzM,zmax)
                clev = lzm + (lzM-lzm)*arange(nl)/(nl-1)
                plenv0(xm,xM,ym,yM,2,0)
                plcol0(15)
                pllab(xlab,ylab,title[alg-1])
                plshades(zg, xm, xM, ym, yM, clev, 1, 1)
                plcol0(2)
            else:
                clev = lzm + (lzM-lzm)*arange(nl)/(nl-1)
                cmap1_init()
                plvpor(0.0, 1.0, 0.0, 0.9)
                plwind(-1.0, 1.0, -1.0, 1.5)
                plw3d(1.0, 1.0, 1.0, xm, xM, ym, yM, lzm, lzM, 30, -60)
                plbox3('bnstu', ylab, 0.0, 0,'bnstu', xlab, 0.0, 0,'bcdmnstuv', '', 0.0, 4)
                plcol0(15)
                pllab('','',title[alg-1])
                plot3dc(xg, yg, zg, DRAW_LINEXY | MAG_COLOR | BASE_CONT, clev)
    plend()
    
main()
