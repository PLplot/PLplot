# File: Makefile.am
# Description: Automake template for the PLplot-DocBook project
# Author: Rafael Laboissière <rafael@icp.inpg.fr>
# Created on: Sat Nov 18 16:36:10 CET 2000
# Last modified on: Mon Dec  4 17:18:04 CET 2000
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

EXTRA_DIST = \
  $(SOURCE_FILES) $(STYLESHEETS_IN) \
  jadetex.cfg stylesheet.css
CLEANFILES = \
  *.$(HTML_EXT) $(HTML_MANIFEST) $(INFO_MANIFEST) $(MAN_MANIFEST) \
  *.jadetex *.aux *.dvi *.ps* *.pdf* inline.ent *.log *.eps TMP.* \
  *.out *.info* *.texi *.txml *.xml-info *-tmp *.$(MANVOL)


JADE = @JADE@
JADETEX = @JADETEX@
PDFJADETEX = @PDFJADETEX@
PERL = @PERL@
LATEX = @LATEX@
DVIPS = @DVIPS@
MAKEINFO = @MAKEINFO@
HTML_EXT = @HTML_EXT@
MANVOL = @MANVOL@

MAX_TEX_RECURSION = 4

BINDIR = ../bin


all: @TARGETS@


html: $(HTML_MANIFEST)

$(HTML_MANIFEST): $(SOURCE_FILES) $(STYLESHEET_HTML).in inline-html.ent
	ln -fs inline-html.ent inline.ent
	$(JADE) $(SGML_CATALOGS) -d $(STYLESHEET_HTML) -t sgml \
	  $(XML_DECL) $(MASTER_XML)


print: $(BASE).ps.gz $(BASE).dvi $(BASE).pdf.gz

%.ps.gz: %.dvi
	$(DVIPS) -f $< | gzip -c > $@

$(BASE).dvi: $(BASE).jadetex jadetex.cfg
	-$(JADETEX) $< 
	if ! cmp $(BASE).aux prior-dvi.aux 2>/dev/null && \
	   expr $(MAKELEVEL) '<' $(MAX_TEX_RECURSION); then \
	        cp -pf $(BASE).aux prior-dvi.aux ; \
		rm -f $@ ; \
		$(MAKE) $@ ; \
	fi

%.pdf.gz: %.pdf
	gzip -9cf < $< > $@

$(BASE).pdf: $(BASE).jadetex jadetex.cfg
	-$(PDFJADETEX) $<
	if ! cmp $(BASE).aux prior-pdf.aux 2>/dev/null && \
	   expr $(MAKELEVEL) '<' $(MAX_TEX_RECURSION); then \
	        cp -pf $(BASE).aux prior-pdf.aux ; \
		rm -f $@ ; \
		$(MAKE) $@ ; \
	fi


$(BASE).jadetex: $(SOURCE_FILES) $(STYLESHEET_PRINT).in inline-print.ent
	ln -fs inline-print.ent inline.ent
	$(JADE) $(SGML_CATALOGS) -d $(STYLESHEET_PRINT) -t tex \
	  -o $@ $(XML_DECL) $(MASTER_XML)


DOCBOOK2TEXIXML = @DOCBOOK2TEXIXML@
TEXI_XML = @TEXI_XML@

INFO_TMPDIR = info

info: $(PACKAGE).info

%.info: %.texi
	rm -rf $(INFO_TMPDIR)
	mkdir -p $(INFO_TMPDIR)
	-$(MAKEINFO) --force -o $(INFO_TMPDIR)/$@ $<
	( cd $(INFO_TMPDIR) ; ls ) | cat > $(INFO_MANIFEST)
	mv $(INFO_TMPDIR)/* .
	rmdir $(INFO_TMPDIR)

%.texi: %.txml
	$(PERL) $(DOCBOOK2X_INC) $(TEXI_XML) $<

%.txml: %.xml-info $(SOURCE_FILES) inline-info.ent
	ln -fs inline-info.ent inline.ent
	if $(PERL) $(DOCBOOK2X_INC) $(DOCBOOK2TEXIXML) $< > $@-tmp ; then \
	  mv $@-tmp $@ ; \
	else \
	  false ; \
	fi

%.xml-info: %.xml $(SOURCE_FILES)
	$(PERL) $(BINDIR)/info-clean.pl $< > $@

man: $(MAN_MANIFEST)

$(MAN_MANIFEST): $(MASTER_XML_IN) api.xml
	$(PERL) $(BINDIR)/api2man.pl $(MASTER_XML) api.xml $(MANVOL) > $@


.PHONY: all html print info man
.PRECIOUS: %.info %.texi %.txml %.xml-info %.pdf

