# File: Makefile.am
# Description: Automake template for the PLplot-DocBook project
# Author: Rafael Laboissière <rafael@icp.inpg.fr>
# Created on: Sat Nov 18 16:36:10 CET 2000
# Last modified on: Fri Nov 24 15:01:39 CET 2000
# $Id$

MASTER_XML = plplotdoc.xml
MASTER_XML_IN = $(MASTER_XML).in

SOURCE_FILES = \
  $(MASTER_XML_IN) \
  advanced.xml \
  api.xml \
  cplus.xml \
  drivers.xml \
  gui.xml \
  inline-html.ent \
  inline-print.ent \
  intro.xml \
  jadetex.cfg \
  over-under.tex \
  perl.xml \
  python.xml \
  scheme.xml \
  simple.xml \
  tcl.xml \
  wish.xml

STYLESHEET_HTML = plplotdoc-html.dsl
STYLESHEET_PRINT = plplotdoc-print.dsl
STYLESHEETS_IN = \
  $(STYLESHEET_HTML).in \
  $(STYLESHEET_PRINT).in

EXTRA_DIST = $(SOURCE_FILES) $(STYLESHEETS_IN)
CLEANFILES = \
  *.$(HTML_EXT) *.$(JPEG_EXT) $(HTML_MANIFEST) $(JPEG_MANIFEST) \
  *.jadetex *.aux *.dvi *.ps.gz inline.ent *.log *.eps TMP.*

JADE = @JADE@
JADETEX = @JADETEX@
PERL = @PERL@
LATEX = @LATEX@
DVIPS = @DVIPS@
HTML_EXT = @HTML_EXT@
JPEG_EXT = @JPEG_EXT@

MAX_TEX_RECURSION = 4

BINDIR = ../bin


all: html print


html: $(HTML_MANIFEST) $(JPEG_MANIFEST)

$(HTML_MANIFEST): $(SGML_SOURCES) $(STYLESHEET_HTML).in inline-html.ent
	ln -fs inline-html.ent inline.ent
	$(JADE) $(SGML_CATALOGS) -d $(STYLESHEET_HTML) -t sgml \
	  $(XML_DECL) $(MASTER_XML)

$(JPEG_MANIFEST): $(BINDIR)/inline-greek.pl.in over-under.jpeg
	$(PERL) $(BINDIR)/inline-greek.pl $(BINDIR) > $@
	echo over-under.jpeg >> $@

over-under.eps: over-under.tex $(BINDIR)/tex2eps.pl.in
	$(PERL) $(BINDIR)/tex2eps.pl -s `cat over-under.tex` -x 1000 -o $@

over-under-big.eps: over-under.tex $(BINDIR)/tex2eps.pl.in
	$(PERL) $(BINDIR)/tex2eps.pl -s `cat over-under.tex` -x 5000 -o $@

over-under.jpeg: over-under-big.eps $(BINDIR)/eps2jpeg.pl.in
	$(PERL) $(BINDIR)/eps2jpeg.pl -g 35% -i over-under-big.eps -o $@


print: $(BASE).ps.gz $(BASE).dvi

%.ps.gz: %.dvi
	$(DVIPS) -f $< | gzip -c > $@

%.dvi: %.jadetex over-under.eps
	$(JADETEX) $< 
	if ! cmp $(shell basename $< .tex).aux prior.aux 2>/dev/null && \
	   expr $(MAKELEVEL) '<' $(MAX_TEX_RECURSION); then \
	        cp -pf $(shell basename $< .tex).aux prior.aux ; \
		rm -f $@ ; \
		$(MAKE) $@ ; \
	fi

%.jadetex: $(SOURCE_FILES) $(STYLESHEET_PRINT).in inline-print.ent
	ln -fs inline-print.ent inline.ent
	$(JADE) $(SGML_CATALOGS) -d $(STYLESHEET_PRINT) -t tex \
	  -o $@ $(XML_DECL) $(MASTER_XML)


.PHONY: all html print
