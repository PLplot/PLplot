# Makefile for remotely installing the release announcements in the PLplot
# web site.  This file only works with GNU make.  The packages xmlto,
# passivetex, and opensp must be installed.
#
# Rafael Laboissiere,
# Created on 2004-02-04
# $Id$

include ../Makefile-vars

XML_DCL = /usr/lib/sgml/declaration/xml.dcl
URL = http://plplot.sf.net
PWD = $(shell cd .. ; pwd)

SOURCES =                 \
    announce-plplot-5.3.0 \
    announce-plplot-5.3.1

TARGETS = $(SOURCES:%=%.xhtml) $(SOURCES:%=%.txt)

WWW_FILES =			\
    announce-plplot-5.1.0.html	\
    announce-plplot-5.2.1.html	\
    announce.css		\
    ChangeLog-5.2.1-5.3.0	\
    ChangeLog-5.3.0-5.3.1	\
    $(SOURCES:%=%.xml)		\
    $(TARGETS)			\
    Makefile

WWW_ANNOUNCE_DIR = $(WWW_DIR)/announce

all: $(TARGETS)

%.html: %.xml html.xsl
	xmlto -m html.xsl xhtml-nochunks $<

%.xhtml: %.html
	mv $< $@

%.txt: %.xhtml
	lynx -force_html -dump -width=80 $< \
	| sed "s,file://localhost$(PWD),$(URL)," > $@

validate:
	for f in $(SOURCES:%=%.xml) ; do \
	    onsgmls $(XML_DCL) $$f > /dev/null ; \
	done

clean:
	rm -f *.xhtml *.txt

install: $(TARGETS)
	ssh $(USER_AT_HOST) rm -rf $(WWW_ANNOUNCE_DIR)
	ssh $(USER_AT_HOST) mkdir $(WWW_ANNOUNCE_DIR)
	scp -r $(WWW_FILES) $(USER_AT_HOST):$(WWW_ANNOUNCE_DIR)
	-ssh $(USER_AT_HOST) chgrp -R $(WWW_GROUP) $(WWW_ANNOUNCE_DIR)
	-ssh $(USER_AT_HOST) chmod -R g=u $(WWW_ANNOUNCE_DIR)

.PHONY: all clean validate install
