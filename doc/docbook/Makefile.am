EXTRA_DIST = README.developers bootstrap.sh cnf/* www/index.html.in
CLEANFILES = $(TARBALL) $(WWW_BASE)
DISTCLEANFILES = @CONFTEST@.* @JADELOG@ www/index.html
SUBDIRS = src bin

RSH_COMMAND = $(RSH) $(WWW_USER)$(WWW_HOST)
RCP_PREFIX = $(shell if test -z "$(WWW_USER)$(WWW_HOST)" ; then \
                       echo "" ; \
                     else \
                       echo "$(WWW_USER)$(WWW_HOST):" ; \
                     fi)

TARBALL = tarball.tgz
WWW_BASE = $(BASE).tar.gz $(BASE_HTML) $(BASE_HTML).tar.gz

www-uninstall:
	$(RSH_COMMAND) rm -rf $(WWW_DIR)

www-install: dist www-uninstall
	( cd src ; \
          $(MAKE) )
	rm -rf $(BASE_HTML)
	mkdir $(BASE_HTML)
	( cd src ; \
          cp `cat $(HTML_MANIFEST)` `cat $(JPEG_MANIFEST)` \
             ../$(BASE_HTML) )
	tar cfz $(BASE_HTML).tar.gz $(BASE_HTML)
	tar cfz $(TARBALL) $(WWW_BASE) \
                -C www index.html -C ../src $(BASE).ps.gz 
	$(RSH_COMMAND) mkdir -p $(WWW_DIR)
	$(RCP) $(TARBALL) $(RCP_PREFIX)$(WWW_DIR)
	$(RSH_COMMAND) tar xfz $(WWW_DIR)/$(TARBALL) -C $(WWW_DIR)
	$(RSH_COMMAND) chgrp -R $(WWW_GROUP) $(WWW_DIR)
	$(RSH_COMMAND) chmod -R g=u $(WWW_DIR)
	$(RSH_COMMAND) rm -f $(WWW_DIR)/$(TARBALL)
	rm -rf $(TARBALL) $(WWW_BASE)

.PHONY: www-uninstall www-install 
