# Makefile for remotely installing the release announcements in the PLplot
# web site (only works with GNU make)
#
# Rafael Labosisiere, 2004-02-04

include ../Makefile-vars

SOURCES = announce-plplot-5.3.0
DOCBOOK_XSL = /usr/share/sgml/docbook/stylesheet/xsl/nwalsh/xhtml/docbook.xsl
ENV = XML_CATALOG_FILES=/usr/share/sgml/docbook/dtd/xml/4.2/catalog.xml
XML_DCL = /usr/lib/sgml/declaration/xml.dcl
URL = http://plplot.sf.net
PWD = $(shell cd .. ; pwd)

TARGETS = $(SOURCES:%=%.xhtml) $(SOURCES:%=%.txt) $(SOURCES:%=%.pdf)

WWW_FILES =                    \
    announce-plplot-5.1.0.html \
    announce-plplot-5.2.1.html \
    announce.css               \
    ChangeLog-5.2.1-5.3.0      \
    $(SOURCES:%=%.xml)         \
    $(TARGETS)                 \
    Makefile
    

all: $(TARGETS)

%.html: %.xml html.xsl
	$(ENV) xmlto -m html.xsl xhtml-nochunks $<

%.xhtml: %.html
	mv $< $@

%.txt: %.xhtml
	lynx -force_html -dump -width=80 $< \
	| sed "s,file://localhost$(PWD),$(URL)," > $@

%.pdf: %.xml print.xsl
	$(ENV) xmlto pdf -m print.xsl $<

validate:
	for f in $(SOURCES:%=%.xml) ; do \
	    onsgmls $(XML_DCL) $$f > /dev/null ; \
	done

clean:
	rm -f *.xhtml *.html *.txt *.pdf

install: $(TARGETS)
	ssh $(USER_AT_HOST) rm -rf $(WWW_DIR)/announce
	ssh $(USER_AT_HOST) mkdir $(WWW_DIR)/announce
	scp -r $(WWW_FILES) $(USER_AT_HOST):$(WWW_DIR)/announce

.PHONY: all clean validate 

