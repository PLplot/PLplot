EXTRA_DIST = README.developers bootstrap.sh cnf/* www/index.html.in
CLEANFILES = $(TARBALL) *.tar.gz *-stamp
DISTCLEANFILES = @CONFTEST@.* @JADELOG@ www/index.html
SUBDIRS = src bin

PERL = @PERL@

RSH_COMMAND = $(RSH) $(WWW_USER)$(WWW_HOST)
RCP_PREFIX = $(shell if test -z "$(WWW_USER)$(WWW_HOST)" ; then \
                       echo "" ; \
                     else \
                       echo "$(WWW_USER)$(WWW_HOST):" ; \
                     fi)

TARBALL = tarball.tgz
HTML_TARBALL = $(BASE_HTML).tar.gz
INDEX_HTML = www/index.html

$(HTML_TARBALL): src/$(HTML_MANIFEST) src/$(JPEG_MANIFEST)
	( cd src ; \
          $(MAKE) html )
	rm -rf $(BASE_HTML)
	mkdir $(BASE_HTML)
	( cd src ; \
          cp `cat $(HTML_MANIFEST)` `cat $(JPEG_MANIFEST)` \
             ../$(BASE_HTML) )
	tar cfz $(HTML_TARBALL) $(BASE_HTML)
	rm -rf $(BASE_HTML)

$(BASE).tar.gz: dist-stamp
dist-stamp:
	$(MAKE) dist
	touch dist-stamp

$(INDEX_HTML): $(INDEX_HTML).in.in $(BASE).tar.gz $(HTML_TARBALL) \
                src/$(BASE).ps.gz src/$(BASE).dvi
	$(PERL) bin/size-href.pl < $(INDEX_HTML).in > $@

$(TARBALL): $(INDEX_HTML)
	( cd src ; \
          $(MAKE) print )
	tar cfz $(TARBALL) $(HTML_TARBALL) $(BASE).tar.gz \
                -C www index.html \
                -C ../src $(BASE).ps.gz $(BASE).pdf.gz $(BASE).dvi

www-uninstall:
	$(RSH_COMMAND) rm -rf $(WWW_DIR)

www-install: $(TARBALL) www-uninstall
	$(RSH_COMMAND) mkdir -p $(WWW_DIR)
	$(RCP) $(TARBALL) $(RCP_PREFIX)$(WWW_DIR)
	$(RSH_COMMAND) tar xfz $(WWW_DIR)/$(TARBALL) -C $(WWW_DIR)
	$(RSH_COMMAND) tar xfz $(WWW_DIR)/$(HTML_TARBALL) -C $(WWW_DIR)
	$(RSH_COMMAND) chgrp -R $(WWW_GROUP) $(WWW_DIR)
	$(RSH_COMMAND) chmod -R g=u $(WWW_DIR)
	$(RSH_COMMAND) rm -f $(WWW_DIR)/$(TARBALL)

.PHONY: www-uninstall www-install 
