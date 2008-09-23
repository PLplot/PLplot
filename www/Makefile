# Makefile for remotely installing the PLplot web site
# (only works with GNU make)
#
# Rafael Labosisiere, 2004-02-04

include Makefile-vars

FILES =          \
    index_old.html   \
    demo         \
    news         \
    plplot_icons \
    resources    \
    source       \
    index.php     \
    corefunctions.php \
    credits.php       \
    css               \
    development.php   \
    download.php      \
    examples.php      \
    img               \
    js
    
    

WWW_FILES = $(patsubst %,$(WWW_DIR)/%,$(FILES))

all: install install-announce

install:
	ssh $(USER_AT_HOST) rm -rf $(WWW_FILES)
	for f in $(FILES) ; do \
	    if test -d $$f ; then \
	        dir=$$f ; \
	    else \
	        dir=`dirname $$f` ; \
	    fi ; \
	    scp -rp $$f $(USER_AT_HOST):$(WWW_DIR)/$$dir ; \
	done
	-ssh $(USER_AT_HOST) chgrp -R $(WWW_GROUP) $(WWW_FILES)
	-ssh $(USER_AT_HOST) chmod -R g=u $(WWW_FILES)

install-announce:
	-( cd announce ; $(MAKE) install )

clean:
	( cd announce ; $(MAKE) clean )
