#############################################################################
# $Id$
# Makefile for plplot document
#
# Eventually the figure stuff will be converted over to directly import
# postscript files.
#
#############################################################################

MAIN = \
	plotdoc.tex \
	chapter1.tex \
	chapter2.tex \
	chapter3.tex \
	chapter4.tex \
	lang.tex \
	dev.tex \
	sys.tex \
	macros.tex

SYS = \
	amiga.tex \
	dos.tex \
	dos386.tex \
	linux.tex \
	os2.tex \
	unix.tex \
	vms.tex 

FIGS = figpg.tex figs.tex

#############################################################################
#
# Targets:
#	all	manual .ps file
#	again	latex it again
#	figs	Just the figure pages
#	clean	cleans up
#
# Default is to build just dvi file since I'm usually in development mode.
#
#############################################################################

default:plotdoc.dvi

all:	plotdoc.ps

plotdoc.ps: plotdoc.dvi
	dvips -o plotdoc.ps plotdoc.dvi

plotdoc.dvi: $(MAIN) $(SYS)
	latex plotdoc
	latex plotdoc

again:
	latex plotdoc

figs:  figpg.ps

figpg.ps: figpg.dvi
	dvips -o figpg.ps figpg.dvi

figpg.dvi: $(FIGS)
	latex figpg

clean:
	-rm -f *~ *.aux *.log *.dvi plotdoc.ps figpg.ps *.blg *.toc
