EXTRA_DIST = \
  README.developers \
  bootstrap.sh \
  bin cnf perl www

CLEANFILES = $(TARBALL) *.tar.gz *-stamp conftest.*
DISTCLEANFILES = $(JADELOG) $(INDEX_HTML)

SUBDIRS = src

if enable_docbook

RSH_COMMAND = $(RSH) $(WWW_USER)$(WWW_HOST)
RCP_PREFIX = $(shell if test -z "$(WWW_USER)$(WWW_HOST)" ; then \
                       echo "" ; \
                     else \
                       echo "$(WWW_USER)$(WWW_HOST):" ; \
                     fi)

TARBALL = tarball.tgz
HTML_TARBALL = $(BASE_HTML).tar.gz
INFO_TARBALL = $(BASE_INFO).tar.gz
MAN_TARBALL = $(BASE_MAN).tar.gz
INDEX_HTML = www/index.html

noinst_DATA = $(TARBALL)

$(HTML_TARBALL): src/$(HTML_MANIFEST) src/stylesheet.css
	rm -rf $(BASE_HTML)
	mkdir $(BASE_HTML)
	( cd src ; \
          cp `cat $(HTML_MANIFEST)` ../$(BASE_HTML) )
	cp src/stylesheet.css $(BASE_HTML)
	tar cfz $(HTML_TARBALL) $(BASE_HTML)
	rm -rf $(BASE_HTML)

$(INFO_TARBALL): src/$(INFO_MANIFEST)
	rm -rf $(BASE_INFO)
	mkdir $(BASE_INFO)
	( cd src ; \
          cp `cat $(INFO_MANIFEST)` ../$(BASE_INFO) )
	tar cfz $(INFO_TARBALL) $(BASE_INFO)
	rm -rf $(BASE_INFO)

$(MAN_TARBALL): src/$(MAN_MANIFEST)
	rm -rf $(BASE_MAN)
	mkdir $(BASE_MAN)
	( cd src ; \
          cp `cat $(MAN_MANIFEST)` ../$(BASE_MAN) )
	tar cfz $(MAN_TARBALL) $(BASE_MAN)
	rm -rf $(BASE_MAN)

$(BASE).tar.gz: dist-stamp
dist-stamp:
	$(MAKE) dist && touch dist-stamp

# size-href.pl requires as input (for size) every file mentioned as
# an href in $(INDEX_HTML).in with a @SIZE@ attribute.  Make the 
# prerequisite list consistent with this requirement.

$(INDEX_HTML): bin/size-href.pl $(INDEX_HTML).in \
	src/$(BASE).ps.gz src/$(BASE).pdf src/$(BASE).dvi \
	$(INFO_TARBALL) $(MAN_TARBALL) $(HTML_TARBALL)
	$(PERL) bin/size-href.pl < $(INDEX_HTML).in > $@

$(TARBALL): $(INDEX_HTML) \
	src/$(BASE).ps.gz src/$(BASE).pdf src/$(BASE).dvi \
	$(INFO_TARBALL) $(MAN_TARBALL) $(HTML_TARBALL)
	tar cfz $(TARBALL) $(HTML_TARBALL) \
                $(INFO_TARBALL) $(MAN_TARBALL) -C www index.html \
                -C ../src $(BASE).ps.gz $(BASE).pdf $(BASE).dvi

www-uninstall:
	$(RSH_COMMAND) rm -rf $(WWW_DIR)

www-install: $(TARBALL)
	$(RSH_COMMAND) rm -rf $(WWW_DIR)
	$(RSH_COMMAND) mkdir -p $(WWW_DIR)
	$(RCP) $(TARBALL) $(RCP_PREFIX)$(WWW_DIR)
	$(RSH_COMMAND) tar xfz $(WWW_DIR)/$(TARBALL) -C $(WWW_DIR)
	$(RSH_COMMAND) tar xfz $(WWW_DIR)/$(HTML_TARBALL) -C $(WWW_DIR)
	$(RSH_COMMAND) chgrp -R $(WWW_GROUP) $(WWW_DIR)
	$(RSH_COMMAND) chmod -R g=u $(WWW_DIR)
	$(RSH_COMMAND) rm -f $(WWW_DIR)/$(TARBALL)

.PHONY: www-uninstall www-install
# enable_docbook
endif
