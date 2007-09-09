from plplot_py_demos import *

# main
#
# Test program for filling polygons and proper clipping


def main():

    plssub(3,3)

    xextreme = [ [ -120.0,  120.0 ],
                 [ -120.0,  120.0 ],
                 [ -120.0,  120.0 ],
                 [  -80.0,   80.0 ],
                 [ -220.0, -120.0 ],
                 [  -20.0,   20.0 ],
                 [  -20.0,   20.0 ],
                 [  -80.0,   80.0 ],
                 [   20.0,  120.0 ] ]

    yextreme = [ [ -120.0,  120.0 ],
                 [   20.0,  120.0 ],
                 [  -20.0,  120.0 ],
                 [  -20.0,  120.0 ],
                 [ -120.0,  120.0 ],
                 [ -120.0,  120.0 ],
                 [  -20.0,   20.0 ],
                 [  -80.0,   80.0 ],
                 [ -120.0,  120.0 ] ]
                 
    for j in range(4):

        # Polygon 1: a diamond
        if (j == 0):
            x0 = [0, -100, 0, 100]
            y0 = [-100, 0, 100, 0]
        # Polygon 2: a diamond - reverse direction
        elif (j == 1):
            x0 = [100, 0, -100, 0]
            y0 = [0, 100, 0, -100]
        # Polygon 3: a square with punctures
        elif (j == 2):
            x0 = [-100, -100, 80, -100, -100, -80, 0, 80, 100, 100]
            y0 = [-100, -80, 0, 80, 100, 100, 80, 100, 100, -100]
        # Polygon 4: a square with punctures - reverse direction
        elif (j == 3):
            x0 = [100, 100, 80, 0, -80, -100, -100, 80, -100, -100]
            y0 = [-100, 100, 100, 80, 100, 100, 80, 0, -80, -100]

        for i in range(9):
            pladv(0)
            plvsta()
            plwind(xextreme[i][0], xextreme[i][1], yextreme[i][0], yextreme[i][1])

            plcol0(2)
            plbox("bc", 1.0, 0, "bcnv", 10.0, 0)
            plcol0(1)
            plpsty(0)
            plfill(x0,y0)
            plcol0(2)
            pllsty(1)
            plline(x0,y0)

    # Don't forget to call plend() to finish off!

    plend()

main()
