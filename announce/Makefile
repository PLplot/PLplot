SOURCES = announce-plplot-5.2.2
DOCBOOK_XSL = /usr/share/sgml/docbook/stylesheet/xsl/nwalsh/xhtml/docbook.xsl
ENV = XML_CATALOG_FILES=/usr/share/sgml/docbook/dtd/xml/4.2/catalog.xml
XML_DCL = /usr/lib/sgml/declaration/xml.dcl

TARGETS = $(SOURCES:%=%.xhtml) $(SOURCES:%=%.txt) $(SOURCES:%=%.pdf)

all: $(TARGETS)

%.html: %.xml html.xsl
	$(ENV) xmlto -m html.xsl xhtml-nochunks $<

%.xhtml: %.html
	mv $< $@

%.txt: %.xhtml
	links -dump -dump-width 80 $< > $@

%.pdf: %.xml print.xsl
	$(ENV) xmlto pdf -m print.xsl $<

validate:
	for f in $(SOURCES:%=%.xml) ; do \
	    onsgmls $(XML_DCL) $$f > /dev/null ; \
	done

clean:
	rm -f *.xhtml *.html *.txt *.pdf

.PHONY: all clean validate 

