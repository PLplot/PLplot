# doc/docbook/src/Makefile.am for PLplot
###
### Process this file with automake to produce Makefile.in
###
# Copyright (C) 2002, 2003 Rafael Laboissière
# Copyright (C) 2002, 2003 Alan W. Irwin
#
#This file is part of PLplot.
#
#This file is free software; you can redistribute it and/or modify
#it under the terms of the GNU Library General Public License as published by
#the Free Software Foundation; version 2 of the License.
#
#This file is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU Library General Public License for more details.
#
#You should have received a copy of the GNU Library General Public License
#along with the file; if not, write to the Free Software
#Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA

MASTER_XML = plplotdoc.xml
MASTER_XML_IN = $(MASTER_XML).in

SOURCE_FILES = \
  $(MASTER_XML_IN) \
  advanced.xml \
  api.xml \
  api-c.xml \
  api-fortran.xml \
  bibliography.xml \
  c.xml \
  fortran.xml \
  cplus.xml \
  drivers.xml \
  gui.xml \
  inline-html.ent \
  inline-print.ent \
  intro.xml \
  os_notes.xml \
  perl.xml \
  python.xml \
  simple.xml \
  tcl.xml \
  wish.xml

STYLESHEET_HTML = plplotdoc-html.dsl
STYLESHEET_PRINT = plplotdoc-print.dsl
STYLESHEETS_IN = \
  $(STYLESHEET_HTML).in \
  $(STYLESHEET_PRINT).in

built_files = \
  $(INFO_MANIFEST) $(HTML_MANIFEST) $(MAN_MANIFEST) \
  $(BASE).jadetex \
  $(html_DATA) $(print_DATA) $(man3_MANS) $(geninfo_DATA) \
  plplotdoc.txml plplotdoc.xml-info plplotdoc.texi

EXTRA_DIST = \
  $(SOURCE_FILES) $(STYLESHEETS_IN) \
  jadetex.cfg stylesheet.css $(built_files) \
  inline-info.ent math.ent pdftex.map

CLEANFILES = \
  *.aux inline.ent *.log *.eps TMP.* *-tmp *.out

MAINTAINERCLEANFILES = \
  $(MASTER_XML) $(STYLESHEET_HTML) $(STYLESHEET_PRINT) $(built_files)

if enable_docbook

htmldir = $(prefix)/share/doc/$(PACKAGE)/html
html_DATA = $(shell ls *.html)

printdir = $(prefix)/share/doc/$(PACKAGE)
print_DATA = $(BASE).dvi $(BASE).ps.gz $(BASE).pdf

man3_MANS = $(shell ls *.$(MANVOL))

geninfodir = $(infodir)
geninfo_DATA = $(shell ls *.info*)

MAX_TEX_RECURSION = 4

BINDIR = ../bin

all: $(TARGETS)

html: $(HTML_MANIFEST)

# N.B. $(STYLESHEET_HTML) assures that manifest of generated html files is
# output to the $(HTML_MANIFEST) file.
$(HTML_MANIFEST): $(SOURCE_FILES) $(STYLESHEET_HTML) inline-html.ent
	ln -fs inline-html.ent inline.ent
	$(OPENJADE) $(SGML_CATALOGS) -d $(STYLESHEET_HTML) -t sgml \
	  $(XML_DECL) $(MASTER_XML)

print: $(print_DATA)

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

$(BASE).pdf: $(BASE).jadetex jadetex.cfg
	-$(PDFJADETEX) $<
	if ! cmp $(BASE).aux prior-pdf.aux 2>/dev/null && \
	   expr $(MAKELEVEL) '<' $(MAX_TEX_RECURSION); then \
	        cp -pf $(BASE).aux prior-pdf.aux ; \
		rm -f $@ ; \
		$(MAKE) $@ ; \
	fi


$(BASE).jadetex: $(SOURCE_FILES) $(STYLESHEET_PRINT_IN) inline-print.ent
	ln -fs inline-print.ent inline.ent
	$(OPENJADE) $(SGML_CATALOGS) -d $(STYLESHEET_PRINT) -t tex \
	  -o $@ $(XML_DECL) $(MASTER_XML)


INFO_TMPDIR = info

info: plplotdoc.info

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

plplotdoc.xml-info: $(SOURCE_FILES)
	$(PERL) $(BINDIR)/info-clean.pl plplotdoc.xml > $@

man: $(MAN_MANIFEST)

$(MAN_MANIFEST): $(MASTER_XML_IN) api.xml
	$(PERL) $(BINDIR)/api2man.pl $(MASTER_XML) api.xml $(MANVOL) > $@

validate: plplotdoc.xml
if has_onsgmls
	@echo Validating $<:
	@$(ONSGMLS) $(XML_DECL) $< > /dev/null
	@echo "No problems found."
else
	@echo "onsgmls not found: cannot validate $<"
endif

.PHONY: all html print info man
.PRECIOUS: %.info %.texi %.txml %.xml-info %.pdf

# enable_docbook
endif
