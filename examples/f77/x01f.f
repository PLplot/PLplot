*
      program example01
*     =================
*
* Demonstration program for PLPLOT:
*
* Plots three simple functions, each function occupies a separate page
*
	real x(101), y(101)
	real xs(6), ys(6)
	real	xscale, yscale, xoff, yoff
        character*80 version
	integer digmax
	common /plotdat/ x, y, xs, ys, xscale, yscale, xoff, yoff
*
* Print plplot version
*
        call plgver(version)
        write (6,*) 'Plplot version is ', version
*
* Ask user to specify the output device
*
        call plssub(2,2)
        call plinit()

* Set up the data
* Original case

	xscale = 6.
	yscale = 1.
	xoff = 0.
	yoff = 0.

* Do a plot

	call plot1()

* Set up the data

	xscale = 1.
	yscale = 0.0014
	yoff = 0.0185

* Do a plot

	digmax = 5
	call plsyax(digmax, 0)
	call plot1()

*======================================================================
*
* Set up the viewport and window using PLENV. The range in X is
*  -2.0 to 10.0, and the range in Y is -0.4 to 2.0. The axes are
*  scaled separately (just = 0), and we draw a box with axes 
*  (axis = 1).
*
      call plcol(1)
      call plenv(-2.0,10.0,-0.4,1.2,0,1)
      call plcol(2)
      call pllab('(x)','sin(x)/x','#frPLPLOT Example 1 - Sinc Function')
*
* Fill up the arrays
*
      do 2 i = 1,100
        x(i) = (i-20.0)/6.0
        y(i) = 1.0
        if (x(i).ne.0.0) y(i) = sin(x(i))/x(i)
    2 continue
*
* Draw the line
*
      call plcol(3)
      call plline(100,x,y)
*
*======================================================================
*
* For the final graph we wish to override the default tick intervals,
*  and so do not use PLENV
*
      call pladv(0)
*
* Use standard viewport, and define X range from 0 to 360 degrees,
*  Y range from -1.2 to 1.2.
*
      call plvsta      
      call plwind(0.0,360.0,-1.2,1.2)
*
* Draw a box with ticks spaced 60 degrees apart in X, and 0.2 in Y.
*
      call plcol(1)
      call plbox('bcnst',60.0,2,'bcnstv',0.2,2)
*
* Superimpose a dashed line grid, with 1.5 mm marks and spaces. With
*  only a single mark and space element, we do not need arrays
*
      call plstyl(1,1500,1500)            
      call plcol(2)
      call plbox('g',30.0,0,'g',0.2,0)
      call plstyl(0,0,0)
*
      call plcol(3)
      call pllab('Angle (degrees)','sine',
     *           '#frPLPLOT Example 1 - Sine function')
*
      do 3 i = 1,101
        x(i) = 3.6 * (i-1)
        y(i) = sin(x(i)*3.141592654/180.0)
    3 continue
*
      call plcol(4)
      call plline(101,x,y)
*
* Don't forget to call PLEND to finish off!
*
      call plend
*
      end

*======================================================================

	subroutine plot1()

	real x(101), y(101)
	real xs(6), ys(6)
	real	xscale, yscale, xoff, yoff
	common /plotdat/ x, y, xs, ys, xscale, yscale, xoff, yoff

	do 10 i = 1, 60
	    x(i)=xoff + xscale * float(i)/60.0
	    y(i)=yoff + yscale * x(i)**2
10	continue

	xmin = x(1)
	xmax = x(60)
	ymin = y(1)
	ymax = y(60)

	do 20 i = 1, 6
	    xs(i) = x((i-1)*10+3)
	    ys(i) = y((i-1)*10+3)
20	continue

c Set up the viewport and window using PLENV. The range in X is
c 0.0 to 6.0, and the range in Y is 0.0 to 30.0. The axes are
c scaled separately (just = 0), and we just draw a labelled
c box (axis = 0).

	call plcol(1)
	call plenv(xmin, xmax, ymin, ymax, 0, 0)
	call plcol(6)
	call pllab("(x)","(y)","#frPLPLOT Example 1 - y=x#u2")

c Plot the data points

	call plcol(9)
	call plpoin(6,xs,ys,9)

c Draw the line through the data

	call plcol(4)
	call plline(60,x,y)

	return
	end
