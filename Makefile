# Makefile for remotely installing the PLplot web site
# (only works with GNU make)
#
# Rafael Labosisiere, 2004-02-04

include Makefile-vars

WWW_FILES =      \
    index.html   \
    credits      \
    demo         \
    examples     \
    news         \
    plplot_icons \
    resources    \
    source

install: install-announce
	ssh $(USER_AT_HOST) rm -rf $(patsubst %,$(WWW_DIR)/%,$(WWW_FILES))
	scp -r $(WWW_FILES) $(USER_AT_HOST):$(WWW_DIR)
	ssh $(USER_AT_HOST) chgrp -R plplot $(WWW_DIR)
	ssh $(USER_AT_HOST) chmod -R g=u $(WWW_DIR)

install-announce:
	-( cd announce ; $(MAKE) install )
