# File: Makefile
# Description: Makefile for PLplot-DocBook project
# Author: Rafael Laboissière <rafael@icp.inpg.fr>
# Created on: Thu May 20 11:14:38 CEST 1999
# Last modified on: Sun Jun 27 20:50:35 CEST 1999
# $Id$

# WARNING: This Makefile must be run from the directory containing the src/
# subdirectory.
# Thus from this same directory above src you must execute 
# ln -s src/Makefile
# and
# ln -s src/jadetex.cfg
# before running make.
# The above symbolic link to jadetex.cfg is required to get jadetex to work
# properly.

#Rafael original for slink debian?
#docbook_stylesheets_dir = /usr/lib/dsssl/stylesheets/docbook
#works for potato Debian
docbook_stylesheets_dir = /usr/lib/sgml/stylesheet/dsssl/docbook/nwalsh
docbook_html_dsssl = $(docbook_stylesheets_dir)/html/docbook.dsl
docbook_dsssl = $(docbook_stylesheets_dir)/print/docbook.dsl
srcdir = src
stylesheet_html = $(srcdir)/plplotdoc-html.dsl
stylesheet_print = $(srcdir)/plplotdoc-print.dsl
html_destdir = html
sgml_sources = $(shell ls src/*.sgml src/*.ent)
perl_scripts = $(shell ls src/*.pl)
sources = $(sgml_sources) $(perl_scripts)
plplotdoc_html=plplotdoc-html

base = plplotdoc
main_html = book1.html

all: html ps

inline.manifest: src/inline-greek.pl
	( cd src ; \
          ./inline-greek.pl ; \
	  mv `cat inline.manifest` .. ; \
	  mv inline.manifest .. )

over-under.eps: src/over-under.tex
	src/tex2eps.pl -s `cat src/over-under.tex` -x 1000 -o over-under.eps

over-under-big.eps: src/over-under.tex
	src/tex2eps.pl -s `cat src/over-under.tex` -x 5000 \
          -o over-under-big.eps

over-under.jpeg: over-under-big.eps
	src/eps2jpeg.pl -g 35% -i over-under-big.eps -o over-under.jpeg

html: html-stamp 
html-stamp: $(sgml_sources) $(stylesheet_html) src/inline-html.ent \
            over-under.jpeg inline.manifest
#	jade -d $(docbook_html_dsssl) -t sgml $(srcdir)/$(base).sgml
	ln -fs inline-html.ent src/inline.ent
	jade -d $(stylesheet_html) -t sgml $(srcdir)/$(base).sgml
	touch html-stamp

ps: ps-stamp
ps-stamp: $(base).tex over-under.eps
	jadetex $(base).tex
	jadetex $(base).tex
	dvips -f $(base) | gzip -c > $(base).ps.gz
	touch ps-stamp

$(base).tex: $(sgml_sources) $(stylesheet_print) src/inline-print.ent
#	jade -d $(docbook_dsssl) -t tex $(srcdir)/$(base).sgml
	ln -fs inline-print.ent src/inline.ent
	jade -d $(stylesheet_print) -t tex $(srcdir)/$(base).sgml
	mv $(srcdir)/$(base).tex .

#version: plplot-docbook.prj
#	prcs info -r.@ plplot-docbook.prj | awk '{print $$2}' > version
#n.b version and index.html are temporary kludges by AWI to get make install
#to work.  plplot-docbook.prj does not exist, and I don't think we can
#assume prcs is installed.  Furthermore, index.html does not exist.
#Rafael, I am sure you will want to change the following.
version:
	echo '0.0.1' >version

$(srcdir)/index.html:
	touch $(srcdir)/index.html

install: install-stamp
install-stamp: version html-stamp ps-stamp $(srcdir)/index.html
	mkdir -p $(html_destdir)
	rm -rf $(html_destdir)/*
	mkdir $(plplotdoc_html)-`cat version`
	cp `src/included-html-files.pl < $(main_html)` $(main_html) \
	   $(srcdir)/index.html `cat inline.manifest` \
           over-under.jpeg $(plplotdoc_html)-`cat version` 
	cp $(plplotdoc_html)-`cat version`/* $(html_destdir)
	tar cfz $(plplotdoc_html)-`cat version`.tar.gz \
            $(plplotdoc_html)-`cat version`
	rm -rf $(plplotdoc_html)-`cat version`
	mkdir -p $(html_destdir)/$(srcdir)
	cp $(sources) Makefile $(stylesheet_html) $(stylesheet_print) \
          src/included-html-files.pl $(html_destdir)/$(srcdir)
	mv $(base).ps.gz $(plplotdoc_html)-`cat version`.tar.gz \
          $(html_destdir)
	ln -sf ./$(plplotdoc_html)-`cat version`.tar.gz \
               $(html_destdir)/$(plplotdoc_html).tar.gz
	touch install-stamp

clean:
	rm -f *.html $(base).* *-stamp version TMP* \
              src/inline.ent *.eps *.jpeg inline.manifest

.PHONY: all html ps install clean
