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
INFO_TARBALL = $(BASE_INFO).tar.gz
INDEX_HTML = www/index.html

$(HTML_TARBALL):
	rm -rf $(BASE_HTML)
	mkdir $(BASE_HTML)
	( cd src ; \
	  $(MAKE) html ; \
          cp `cat $(HTML_MANIFEST)` ../$(BASE_HTML) )
	cp src/stylesheet.css $(BASE_HTML)
	tar cfz $(HTML_TARBALL) $(BASE_HTML)
	rm -rf $(BASE_HTML)

$(INFO_TARBALL):
	rm -rf $(BASE_INFO)
	mkdir $(BASE_INFO)
	( cd src ; \
	  $(MAKE) info ; \
          cp `cat $(INFO_MANIFEST)` ../$(BASE_INFO) )
	tar cfz $(INFO_TARBALL) $(BASE_INFO)
	rm -rf $(BASE_INFO)

$(BASE).tar.gz: dist-stamp
dist-stamp:
	$(MAKE) dist
	touch dist-stamp

$(INDEX_HTML): $(INDEX_HTML).in.in $(BASE).tar.gz $(HTML_TARBALL) \
               $(INFO_TARBALL)
	( cd src ; \
          $(MAKE) print )	
	$(PERL) bin/size-href.pl < $(INDEX_HTML).in > $@

$(TARBALL): $(INDEX_HTML)
	tar cfz $(TARBALL) $(HTML_TARBALL) $(BASE).tar.gz \
                $(INFO_TARBALL) -C www index.html \
                -C ../src $(BASE).ps.gz $(BASE).pdf.gz $(BASE).dvi

www-uninstall:
	$(RSH_COMMAND) rm -rf $(WWW_DIR)

www-install: $(TARBALL) www-uninstall
	$(MAKE)
	$(RSH_COMMAND) mkdir -p $(WWW_DIR)
	$(RCP) $(TARBALL) $(RCP_PREFIX)$(WWW_DIR)
	$(RSH_COMMAND) tar xfz $(WWW_DIR)/$(TARBALL) -C $(WWW_DIR)
	$(RSH_COMMAND) tar xfz $(WWW_DIR)/$(HTML_TARBALL) -C $(WWW_DIR)
	$(RSH_COMMAND) chgrp -R $(WWW_GROUP) $(WWW_DIR)
	$(RSH_COMMAND) chmod -R g=u $(WWW_DIR)
	$(RSH_COMMAND) rm -f $(WWW_DIR)/$(TARBALL)

.PHONY: www-uninstall www-install 
