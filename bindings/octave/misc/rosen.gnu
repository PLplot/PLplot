#!/usr/local/bin/gnuplot
#From: shen@athena.cs.uga.edu (Mingzuo Shen)
#Subject: Rosenbrock's function: some answers and thanks
#Date: Wed, 23 Jun 1993 20:50:36 GMT
#
#    Last night I asked for help with a contour plot for the function:
#
#    (1-x)**2 + 100 * (y - x**2)**2
#
#which should have a minimum at (x=1,y=1). This is the 2D case of a more
#general function named after Rosenbrock (the book I am reading does not
#give a reference to any paper/book by this person):
#
#    for even integer n,
#
#    f(x) = \sum_{j=1,3,5,\ldots,n} [(1 - x_j)^2 + 100(x_{j+1} - x_j^2)^2]
#
#where x is a vector (x1, x2, ..., x_n). This function also have a minimum
#at (1,1,...,1), which lies at the base of a "banana-shaped valley".
#Here are the new commands I have tried.
#
set auto
set surface
set nocontour
set nokey
set cntrparam levels 5
set cntrparam linear
set samples 100
set logscale z
set title "Rosenbrock Function"

set logscale z
set hidden3d
set isosamples 60
set xlabel "x"
set ylabel "y"
set contour base
splot [-1.5:1.5] [-0.5:1.5] (1-x)**2 + 100*(y - x**2)**2
#pause -1 "Continue -- new plot window"
set term x11 1
set contour
set nosurface
set view 0,0,1.5,1
replot
pause -1 "Continue to quit"
