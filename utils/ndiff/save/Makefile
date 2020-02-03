# Generated automatically from Makefile.in by configure.
###=====================================================================
### Makefile for files in /u/sy/beebe/src/ndiff
###
### Copyright (C) 2000 Nelson H. F. Beebe
### See the file ndiff.sin for a full copyright notice.
###
### Current target list:
###	all
###	$(PROGNAME).jar
###	$(PROGNAME).tar
###	$(PROGNAME).zip
###	$(PROGNAME).zoo
###	check
###	check-ndiff
###	clean
###	cleandist
###	cleandocs
###	clobber
###	config.hin
###	configure
###	dist
###	distclean
###	docs
###	dust
###	dw
###	html
###	install
###	install-exe
###	install-ftp
###	install-man
###	install-show
###	install-shrlib
###	maintainer-check-regress
###	maintainer-checknr
###	maintainer-clean
###	Makefile
###	maketab
###	mostlyclean
###	pdf
###	ps
###	spell
###	tgrind
###	txt
###	uninstall
###	uninstall-exe
###	uninstall-man
###	uninstall-shrlib
###
### [10-Dec-2000] -- update for version 2.00
### [28-Jan-2000] -- original version 1.00
###=====================================================================
### Program definitions

PROGNAME	= ndiff
PROGRAMS	= $(PROGNAME)
VERSION		= 2.00
DATE		= 10-Dec-2000

###=====================================================================
### Installation locations

prefix		= /usr/local
bindir		= $(prefix)/bin
catdir		= $(prefix)/man/cat$(manext)
mandir		= $(prefix)/man/man$(manext)
manext		= 1
shrlibdir	= $(shrlibtop)/$(PROGNAME)-$(VERSION)
shrlibtop       = $(prefix)/share/lib/$(PROGNAME)

FTPDIR		= /u/ftp/pub/misc

###=====================================================================
### Macro definitions

AUTOCONF	= autoconf

AUTOHEADER	= autoheader

AWK		= /usr/local/bin/mawk

CAT		= cat

CC		= cc

CHECKNR		= checknr

CHECKOKAY	= okay-gmp

CHECKSUM	= checksum

CHMOD		= chmod

CMP		= cmp

CP		= scp -p

DEROFF		= deroff

DIFF		= diff

DISTILL		= distill

DITROFF		= groff

### dw is available at:
###	ftp://ftp.math.utah.edu/pub/misc/dw.jar
###	ftp://ftp.math.utah.edu/pub/misc/dw.shar
###	ftp://ftp.math.utah.edu/pub/misc/dw.tar.gz
###	ftp://ftp.math.utah.edu/pub/misc/dw.zip
###	ftp://ftp.math.utah.edu/pub/misc/dw.zoo
###
###	http://www.math.utah.edu/pub/misc/dw.jar
###	http://www.math.utah.edu/pub/misc/dw.shar
###	http://www.math.utah.edu/pub/misc/dw.tar.gz
###	http://www.math.utah.edu/pub/misc/dw.zip
###	http://www.math.utah.edu/pub/misc/dw.zoo
DW		= dw

ETAGS		= etags

GREP		= grep

GZIP		= gzip

ISPELL		= ispell

JAR		= jar

LN		= ln

LS		= ls

### man2html is available at
###	ftp://ftp.math.utah.edu/pub/sgml/man2html-x.yy.*
###	http://www.math.utah.edu/pub/sgml/index.html#man2html
MAN2HTML	= man2html

MAN2HTMLFLAGS	= -grammar-level 3.2

MKDIR		= /u/sy/beebe/src/ndiff/ndiff-2.00/mkdir.sh -p

MV		= mv

NROFF		= nroff

RM		= rm -f

SED		= sed

SEDCMD		= $(SED) -e 's%AWK=.*$$%AWK=$(AWK)%' \
			-e 's%LIBDIR=/.*$$%LIBDIR=$(shrlibdir)%' \
			-e 's%@LIBDIR@%$(shrlibdir)%g' \
			-e 's%@BINDIR@%$(bindir)%g' \
			-e 's%2[.]00%$(VERSION)%g' \
			-e 's%10-Dec-2000%$(DATE)%g' \
			-e 's%^\#! /bin/sh%\#! $(SHELL)%'

SHELL		= /bin/sh

SORT		= sort

SPELL		= spell

TAGSFILES	= awklib.c awklib.h ndiff.c ndiff.h

TAR		= tar

### The enhanced tgrind utility is available at:
###
###	ftp://ftp.math.utah.edu/pub/misc/tgrind-3.00.jar
###	ftp://ftp.math.utah.edu/pub/misc/tgrind-3.00.tar.gz
###	ftp://ftp.math.utah.edu/pub/misc/tgrind-3.00.zip
###	ftp://ftp.math.utah.edu/pub/misc/tgrind-3.00.zoo
###
###	http://www.math.utah.edu/pub/misc/tgrind-3.00.jar
###	http://www.math.utah.edu/pub/misc/tgrind-3.00.tar.gz
###	http://www.math.utah.edu/pub/misc/tgrind-3.00.zip
###	http://www.math.utah.edu/pub/misc/tgrind-3.00.zoo
TGRIND		= tgrind

UNZIP		= unzip

ZIP		= zip

ZOO		= zoo

###=====================================================================
### Compiler flags:

CFLAGS		= -I/usr/local/include $(INCLUDES) $(DEFINES) $(OPT) $(XCFLAGS)

DEFINES		= -DVERSION='"2.00"' -DDATE='"10-Dec-2000"' $(XDEFINES)

INCLUDES	= -I. -I$(prefix)/include $(XINCLUDES)

LDFLAGS		=  -L$(prefix)/lib $(XLDFLAGS)

LIBS		=  -L/usr/local/lib -lgmp -lgmp $(XLIBS)

OPT		= -O $(XOPT)

TGRINDFLAGS	= -i 2 -l awk -fn Bookman $(XTGRINDFLAGS)

###=====================================================================
### Extra values to be appended to the X-less names at make time:

XCFLAGS		=

XDEFINES	=

XINCLUDES	=

XLDFLAGS	=

XLIBS		=

XOPT		=

XTGRINDFLAGS	=

###=====================================================================
### Source files:

AWKFILES	= $(PROGNAME).awk

AWKTESTFILES	= awktest.awk awktest.c awktest.dat

CHECKFILES      = test/check001.in1 test/check001.in2 test/check001.opt \
		  test/check002.in1 test/check002.in2 test/check002.opt \
		  test/check003.in1 test/check003.in2 test/check003.opt \
		  test/check004.in1 test/check004.in2 test/check004.opt \
		  test/check005.in1 test/check005.in2 test/check005.opt \
		  test/check006.in1 test/check006.in2 test/check006.opt \
		  test/check007.in1 test/check007.in2 test/check007.opt \
		  test/check008.in1 test/check008.in2 test/check008.opt \
		  test/check009.in1 test/check009.in2 test/check009.opt \
		  test/check010.in1 test/check010.in2 test/check010.opt \
		  test/check011.in1 test/check011.in2 test/check011.opt \
		  test/check012.in1 test/check012.in2 test/check012.opt \
		  test/check013.in1 test/check013.in2 test/check013.opt \
		  test/check014.in1 test/check014.in2 test/check014.opt \
		  test/check015.in1 test/check015.in2 test/check015.opt \
		  test/check016.in1 test/check016.in2 test/check016.opt \
		  test/check017.in1 test/check017.in2 test/check017.opt \
		  test/check018.in1 test/check018.in2 test/check018.opt \
		  test/check019.in1 test/check019.in2 test/check019.opt \
		  test/check020.in1 test/check020.in2 test/check020.opt \
		  test/check021.in1 test/check021.in2 test/check021.opt \
		  test/check022.in1 test/check022.in2 test/check022.opt \
		  test/check023.in1 test/check023.in2 test/check023.opt \
		  test/check024.in1 test/check024.in2 test/check024.opt \
		  test/check025.in1 test/check025.in2 test/check025.opt \
		  test/check026.in1 test/check026.in2 test/check026.opt \
		  test/okay-dbl/check001.err test/okay-dbl/check001.out \
		  test/okay-dbl/check002.err test/okay-dbl/check002.out \
		  test/okay-dbl/check003.err test/okay-dbl/check003.out \
		  test/okay-dbl/check004.err test/okay-dbl/check004.out \
		  test/okay-dbl/check005.err test/okay-dbl/check005.out \
		  test/okay-dbl/check006.err test/okay-dbl/check006.out \
		  test/okay-dbl/check007.err test/okay-dbl/check007.out \
		  test/okay-dbl/check008.err test/okay-dbl/check008.out \
		  test/okay-dbl/check009.err test/okay-dbl/check009.out \
		  test/okay-dbl/check010.err test/okay-dbl/check010.out \
		  test/okay-dbl/check011.err test/okay-dbl/check011.out \
		  test/okay-dbl/check012.err test/okay-dbl/check012.out \
		  test/okay-dbl/check013.err test/okay-dbl/check013.out \
		  test/okay-dbl/check014.err test/okay-dbl/check014.out \
		  test/okay-dbl/check015.err test/okay-dbl/check015.out \
		  test/okay-dbl/check016.err test/okay-dbl/check016.out \
		  test/okay-dbl/check017.err test/okay-dbl/check017.out \
		  test/okay-dbl/check018.err test/okay-dbl/check018.out \
		  test/okay-dbl/check019.err test/okay-dbl/check019.out \
		  test/okay-dbl/check020.err test/okay-dbl/check020.out \
		  test/okay-dbl/check021.err test/okay-dbl/check021.out \
		  test/okay-dbl/check022.err test/okay-dbl/check022.out \
		  test/okay-dbl/check023.err test/okay-dbl/check023.out \
		  test/okay-dbl/check024.err test/okay-dbl/check024.out \
		  test/okay-dbl/check025.err test/okay-dbl/check025.out \
		  test/okay-dbl/check026.err test/okay-dbl/check026.out \
		  test/okay-gmp/check001.err test/okay-gmp/check001.out \
		  test/okay-gmp/check002.err test/okay-gmp/check002.out \
		  test/okay-gmp/check003.err test/okay-gmp/check003.out \
		  test/okay-gmp/check004.err test/okay-gmp/check004.out \
		  test/okay-gmp/check005.err test/okay-gmp/check005.out \
		  test/okay-gmp/check006.err test/okay-gmp/check006.out \
		  test/okay-gmp/check007.err test/okay-gmp/check007.out \
		  test/okay-gmp/check008.err test/okay-gmp/check008.out \
		  test/okay-gmp/check009.err test/okay-gmp/check009.out \
		  test/okay-gmp/check010.err test/okay-gmp/check010.out \
		  test/okay-gmp/check011.err test/okay-gmp/check011.out \
		  test/okay-gmp/check012.err test/okay-gmp/check012.out \
		  test/okay-gmp/check013.err test/okay-gmp/check013.out \
		  test/okay-gmp/check014.err test/okay-gmp/check014.out \
		  test/okay-gmp/check015.err test/okay-gmp/check015.out \
		  test/okay-gmp/check016.err test/okay-gmp/check016.out \
		  test/okay-gmp/check017.err test/okay-gmp/check017.out \
		  test/okay-gmp/check018.err test/okay-gmp/check018.out \
		  test/okay-gmp/check019.err test/okay-gmp/check019.out \
		  test/okay-gmp/check020.err test/okay-gmp/check020.out \
		  test/okay-gmp/check021.err test/okay-gmp/check021.out \
		  test/okay-gmp/check022.err test/okay-gmp/check022.out \
		  test/okay-gmp/check023.err test/okay-gmp/check023.out \
		  test/okay-gmp/check024.err test/okay-gmp/check024.out \
		  test/okay-gmp/check025.err test/okay-gmp/check025.out \
		  test/okay-gmp/check026.err test/okay-gmp/check026.out \
		  test/okay-ldb/check001.err test/okay-ldb/check001.out \
		  test/okay-ldb/check002.err test/okay-ldb/check002.out \
		  test/okay-ldb/check003.err test/okay-ldb/check003.out \
		  test/okay-ldb/check004.err test/okay-ldb/check004.out \
		  test/okay-ldb/check005.err test/okay-ldb/check005.out \
		  test/okay-ldb/check006.err test/okay-ldb/check006.out \
		  test/okay-ldb/check007.err test/okay-ldb/check007.out \
		  test/okay-ldb/check008.err test/okay-ldb/check008.out \
		  test/okay-ldb/check009.err test/okay-ldb/check009.out \
		  test/okay-ldb/check010.err test/okay-ldb/check010.out \
		  test/okay-ldb/check011.err test/okay-ldb/check011.out \
		  test/okay-ldb/check012.err test/okay-ldb/check012.out \
		  test/okay-ldb/check013.err test/okay-ldb/check013.out \
		  test/okay-ldb/check014.err test/okay-ldb/check014.out \
		  test/okay-ldb/check015.err test/okay-ldb/check015.out \
		  test/okay-ldb/check016.err test/okay-ldb/check016.out \
		  test/okay-ldb/check017.err test/okay-ldb/check017.out \
		  test/okay-ldb/check018.err test/okay-ldb/check018.out \
		  test/okay-ldb/check019.err test/okay-ldb/check019.out \
		  test/okay-ldb/check020.err test/okay-ldb/check020.out \
		  test/okay-ldb/check021.err test/okay-ldb/check021.out \
		  test/okay-ldb/check022.err test/okay-ldb/check022.out \
		  test/okay-ldb/check023.err test/okay-ldb/check023.out \
		  test/okay-ldb/check024.err test/okay-ldb/check024.out \
		  test/okay-ldb/check025.err test/okay-ldb/check025.out \
		  test/okay-ldb/check026.err test/okay-ldb/check026.out

CHECKOPTFILES   = check001.opt check002.opt check003.opt check004.opt \
		  check005.opt check006.opt check007.opt check008.opt \
		  check009.opt check010.opt check011.opt check012.opt \
		  check013.opt check014.opt check015.opt check016.opt \
		  check017.opt check018.opt check019.opt check020.opt \
		  check021.opt check022.opt check023.opt check024.opt \
		  check025.opt check026.opt

CHECKNAMES	= $(CHECKOPTFILES:.opt=)

CONFIGINFILES   = config.hin configure.in $(PROGNAME).sin

DISTFILES       = ChangeLog COPYING INSTALL Makefile.in README README.AWK \
		  README.QUAD mkdir.sh $(CHECKFILES) $(CONFIGINFILES) \
		  configure $(SINFILES) $(SHRLIBFILES) $(SOKFILES) \
		  $(SRCFILES) $(DOCFILES) $(REGEXPFILES) $(SAVEFILES)

DOCFILES	= $(HTMLFILES) $(MANFILES) $(PDFFILES) $(PSFILES) $(TXTFILES)

HTMLFILES	= $(MANFILES:.man=.html)

MANFILES	= $(PROGNAME).man

PDFFILES	= $(MANFILES:.man=.pdf)

PSFILES		= $(MANFILES:.man=.ps)

REGEXPFILES     = regexp/Makefile regexp/README regexp/README.STDC \
		  regexp/args.h regexp/regerror.c regexp/regexp.3 \
		  regexp/regexp.c regexp/regexp.h regexp/regmagic.h \
		  regexp/regsub.c regexp/tests regexp/timer.c regexp/try.c \
		  regexp/try.errs

SAVEFILES       = save/Makefile save/configure save/config.h

SHFILES		= $(AWKFILES:.awk=.sh)

SHRLIBFILES	= $(AWKFILES)

SINFILES	= $(AWKFILES:.awk=.sin)

SOKFILES	= $(MANFILES:.man=.sok)

SRCFILES        = acconfig.h awklib.c awklib.h confix.h ndiff.c ndiff.h \
		  store.c

TMPINSTDIR	= $(TMPDIR)/$(PROGNAME)

TMPDIR		= /tmp

TXTFILES	= $(MANFILES:.man=.txt)

###=====================================================================
### Suffixes and extra rule(s):

.SUFFIXES:	.dw .ser .html .pdf .ps .txt .man .i .c

.c.i:
	$(CC) $(CFLAGS) -E $< | $(GREP) -v '^[ 	]*$$' > $@

.man.dw:
	$(DEROFF) $< | $(DW) >$*.dw
	@if [ -s $*.dw ] ; \
	then \
		echo "Doubled words in $*.dw" ; \
		cat $*.dw ; \
	fi

.man.html:
	$(MAN2HTML) $(MAN2HTMLFLAGS) $<
	$(MV) $@ $*.tmp
	$(SEDCMD) < $*.tmp >$@
	-$(RM) $*.tmp

.man.ps:
	$(SEDCMD) $< | $(DITROFF) -Tps -man - >$@

SPELL-FILTER = $(SED) -e "s/[^a-zA-Z'-]/ /g"
.man.ser:
	$(DEROFF) $< | $(SPELL-FILTER) | $(SPELL) +$*.sok >$*.ser
	$(DEROFF) $< | $(SPELL-FILTER) | $(ISPELL) -l -p $*.sok >>$*.ser
	@if [ -s $*.ser ] ; \
	then \
		echo "Spelling exceptions in $*.ser" ; \
		$(SORT) -u $*.ser >/tmp/$*.tmp ; \
		$(MV) /tmp/$*.tmp $*.ser ; \
		$(CAT) $*.ser ; \
	fi

.man.txt:
	$(SEDCMD) $< | $(NROFF) -man - | col -b | expand >$@

.ps.pdf:
	$(DISTILL) $<

###=====================================================================
### Targets:

all:	$(PROGNAME)

awktest:	awktest.o store.o awklib.o regexp.o regerror.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) awktest.o store.o awklib.o regexp.o regerror.o $(LIBS)

$(PROGNAME):	$(PROGNAME).o store.o awklib.o regexp.o regerror.o
	$(CC) $(CFLAGS) -o $@ $(LDFLAGS) $(PROGNAME).o store.o awklib.o regexp.o regerror.o $(LIBS)

### A Java archive (.jar) file is read and written like a UNIX .tar
### file, but its format is compatible with InfoZip (zip/unzip), a
### cross-platform archive format.  As Java becomes widely available,
### .jar files will become familiar to users, and are therefore a
### reasonable distribution format.
###
### jar creates one additional file, META-INF/MANIFEST.MF, which
### provides a contents listing, and checksums, e.g.,
###
###	% cat META-INF/MANIFEST.MF
###	Manifest-Version: 1.0
###
###	Name: foo.1
###	Digest-Algorithms: SHA MD5
###	SHA-Digest: 3o3sk+dlhmdbQB0A7Upx4qRRUzU=
###	MD5-Digest: N7t5JDQ07HAcNKJSNTAhig==

$(PROGNAME).jar:	$(DISTFILES)
	-$(RM) $(PROGNAME).jar
	$(JAR) cf $(PROGNAME).jar $(DISTFILES)
	-$(MKDIR) $(PROGNAME)-$(VERSION)
	cd $(PROGNAME)-$(VERSION); $(JAR) xf ../$(PROGNAME).jar
	$(JAR) cf $(PROGNAME)-$(VERSION).jar $(PROGNAME)-$(VERSION)
	-$(RM) -r $(PROGNAME)-$(VERSION)
	-$(RM) $(PROGNAME).jar
	$(LN) $(PROGNAME)-$(VERSION).jar $(PROGNAME).jar

$(PROGNAME).tar:	$(DISTFILES)
	-$(RM) $(PROGNAME).tar $(PROGNAME).tar-lst
	$(TAR) chf $(PROGNAME).tar $(DISTFILES)
	-$(MKDIR) $(PROGNAME)-$(VERSION)
	cd $(PROGNAME)-$(VERSION); $(TAR) xf ../$(PROGNAME).tar
	$(TAR) cf $(PROGNAME)-$(VERSION).tar $(PROGNAME)-$(VERSION)
	-$(RM) -r $(PROGNAME)-$(VERSION)
	-$(RM) $(PROGNAME).tar
	$(LN) $(PROGNAME)-$(VERSION).tar $(PROGNAME).tar

$(PROGNAME).zip:	$(DISTFILES)
	-$(RM) $(PROGNAME)*.zip $(PROGNAME)*.zip-lst
	$(ZIP) $(PROGNAME)-$(VERSION).zip $(DISTFILES)
	$(UNZIP) -v $(PROGNAME)-$(VERSION).zip >$(PROGNAME)-$(VERSION).zip-lst
	$(LN) $(PROGNAME)-$(VERSION).zip $(PROGNAME).zip

$(PROGNAME).zoo:	$(DISTFILES)
	-$(RM) $(PROGNAME)*.zoo $(PROGNAME)*.zoo-lst
	$(ZOO) a $(PROGNAME)-$(VERSION).zoo $(DISTFILES)
	$(ZOO) v $(PROGNAME)-$(VERSION).zoo >$(PROGNAME)-$(VERSION).zoo-lst
	$(LN) $(PROGNAME)-$(VERSION).zoo $(PROGNAME).zoo

# Because a good bit of the command-line processing is done in the
# shell script interface to ndiff.awk, we need to make the checks
# using a temporarily-installed version of the program.
check:	check-install-tmp check-runs check-uninstall-tmp

check-install-tmp:
	@-$(MKDIR) $(TMPINSTDIR) $(TMPINSTDIR)/bin $(TMPINSTDIR)/man \
		$(TMPINSTDIR)/man/man1 $(TMPINSTDIR)/share \
		$(TMPINSTDIR)/share/lib $(TMPINSTDIR)/share/lib/$(PROGNAME)
	@$(MAKE) install prefix=$(TMPINSTDIR) >/dev/null

# Because jar seems to have problems with empty directories, we have a copy
# of the *.opt files in test/okay/check??? and in test/check???.
check-runs:
	@echo ""
	@echo ========================================================================
	@echo Test of $(PROGNAME): there should be no output, other than the test and
	@echo command names.  The tests take about 10 sec on a fast workstation.
	@case $(CHECKOKAY) in \
	okay-dbl) PRECISION="native double-precision" ;; \
	okay-ldb) PRECISION="native long-double-precision" ;; \
	okay-gmp) PRECISION="GNU gmp multiple-precision" ;; \
	*)        PRECISION="unknown-precision" ;; \
	esac ; \
	echo This version of $(PROGNAME) has been built with $$PRECISION arithmetic.
	@echo ========================================================================
	@-cd test ; \
	for f in $(CHECKNAMES) ; \
	do \
		echo ============ $$f: $(PROGNAME) `cat $$f.opt` ; \
		$(MKDIR) $$f 2>/dev/null; \
		$(TMPINSTDIR)/bin/$(PROGNAME) --quick --logfile $$f.err \
			--outfile $$f.out `cat $$f.opt` $$f.in1 $$f.in2 ; \
		if $(CMP) -s $(CHECKOKAY)/$$f.err $$f.err ; \
		then \
			$(RM) $$f.err ; \
		else \
			$(DIFF) $(CHECKOKAY)/$$f.err $$f.err ; \
		fi ; \
		if $(CMP) -s $(CHECKOKAY)/$$f.out $$f.out ; \
		then \
			$(RM) $$f.out ; \
		else \
			$(DIFF) $(CHECKOKAY)/$$f.out $$f.out ; \
		fi ; \
	done
	@echo ========================================================================
	@echo ""

check-uninstall-tmp:
	@-$(RM) -r $(TMPINSTDIR)

checknr:
	$(CHECKNR) $(MANFILES)

clean:	clean-test
	-$(RM) *.dw
	-$(RM) *.i
	-$(RM) *.o
	-$(RM) *.ser
	-$(RM) *.tmp
	-$(RM) *~
	-$(RM) \#*
	-$(RM) a.out
	-$(RM) core
	-$(RM) tg_[0-9][0-9][0-9][0-9][0-9].*

clean-test:
	-$(RM) -r test/check???
	-$(RM) test/check???.err test/check???.out
	-$(RM) test/*~
	-$(RM) test/\#*
	-$(RM) test/a.out
	-$(RM) test/core

cleandist:
	-$(RM) $(PROGNAME)*.jar $(PROGNAME)*.jar-lst
	-$(RM) $(PROGNAME)*.tar $(PROGNAME)*.tar-lst
	-$(RM) $(PROGNAME)*.zip $(PROGNAME)*.zip-lst
	-$(RM) $(PROGNAME)*.zoo $(PROGNAME)*.zoo-lst

cleandocs:
	-$(RM) $(HTMLFILES)
	-$(RM) $(PDFFILES)
	-$(RM) $(PSFILES)
	-$(RM) $(TXTFILES)

clobber:	distclean

### While this step requires autoconf and autoheader, it should only
### ever be used at the developer's site, where it is universally
### available.
configure:	configure.in
	$(AUTOHEADER)
	$(AUTOCONF)

dist:	docs $(PROGNAME).jar $(PROGNAME).tar $(PROGNAME).zip $(PROGNAME).zoo

distclean:	mostlyclean cleandist
	-$(RM) config.cache config.h config.log config.status Makefile $(PROGNAME).sh
	-$(RM) $(PROGNAME)
	-$(RM) awktest

docs:	$(DOCFILES)

dw:	$(MANFILES:.man=.dw)

FRC:

html:	$(HTMLFILES)

install:	install-exe install-shrlib install-man install-show

install-exe:	uninstall-exe
	if test -f $(PROGNAME) ; \
	then \
		$(CP) $(PROGNAME) $(bindir)/$(PROGNAME) ; \
		$(LN) $(bindir)/$(PROGNAME) $(bindir)/$(PROGNAME)-$(VERSION) ; \
	else \
		for f in $(SHFILES) ; \
		do \
			g=`basename $$f .sh` ; \
			$(SEDCMD) $$f | $(CHECKSUM) > $(bindir)/$$g ; \
			$(LN) $(bindir)/$$g $(bindir)/$$g-$(VERSION) ; \
			$(CHMOD) 775 $(bindir)/$$g $(bindir)/$$g-$(VERSION) ; \
		done ; \
	fi

install-ftp:	dist
	$(TAR) tvf $(PROGNAME)-$(VERSION).tar >$(FTPDIR)/$(PROGNAME)-$(VERSION).tar-lst
	$(GZIP) <$(PROGNAME)-$(VERSION).tar >$(FTPDIR)/$(PROGNAME)-$(VERSION).tar.gz
	$(CP) $(PROGNAME)-$(VERSION).jar $(FTPDIR)
	$(JAR) tvf $(PROGNAME)-$(VERSION).jar >$(FTPDIR)/$(PROGNAME)-$(VERSION).jar-lst 2>&1
	$(CP) $(PROGNAME)-$(VERSION).zip $(FTPDIR)
	$(CP) $(PROGNAME)-$(VERSION).zip-lst $(FTPDIR)
	$(CP) $(PROGNAME)-$(VERSION).zoo $(FTPDIR)
	$(CP) $(PROGNAME)-$(VERSION).zoo-lst $(FTPDIR)
	$(LS) -l $(FTPDIR)/$(PROGNAME)*

install-man:	uninstall-man
	for f in $(MANFILES) ; \
	do \
		g=`basename $$f .man` ; \
		$(SEDCMD) $$f | $(CHECKSUM) > $(mandir)/$$g.$(manext) ; \
		$(CHMOD) 664 $(mandir)/$$g.$(manext) ; \
	done

install-show:
	@echo ''
	@echo Installed files...
	@$(LS) -l $(bindir)/$(PROGNAME) $(bindir)/$(PROGNAME)-$(VERSION)
	@$(LS) -l $(mandir)/$(PROGNAME).$(manext)
	@if test -d $(shrlibdir) ; then $(LS) -lR $(shrlibdir)/* ; fi
	@echo ''

install-shrlib:	uninstall-shrlib
	-$(MKDIR) $(shrlibtop)
	@if test -d $(shrlibtop) ; then true ; else echo Cannot create $(shrlibtop) ; exit 1 ; fi
	-$(MKDIR) $(shrlibdir)
	@if test -d $(shrlibdir) ; then true ; else echo Cannot create $(shrlibdir) ; exit 1 ; fi
	for f in $(SHRLIBFILES) ; \
	do \
		$(SEDCMD) < $$f > $(shrlibdir)/$$f ; \
	done
	$(CHMOD) 664 $(shrlibdir)/*

maintainer-clean:	distclean cleandocs
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."
	-$(RM) configure config.hin

Makefile:	Makefile.in
	./configure

mostlyclean:	clean

pdf:	$(PDFFILES)

ps:	$(PSFILES)

regerror.o:	regexp/regerror.c
	(cd regexp; $(CC) $(CFLAGS) -c -I. regerror.c; $(MV) regerror.o ..)

regexp.o:	regexp/regexp.c
	(cd regexp; $(CC) $(CFLAGS) -c -I. regexp.c; $(MV) regexp.o ..)

spell:	$(MANFILES:.man=.ser)

reconfigure:	FRC
	$(AUTOHEADER)
	$(AUTOCONF)
	CC="$(CC)" ./configure

TAGS:	$(TAGSFILES)
	$(ETAGS) $(TAGSFILES)

tgrind:	tgrind-awk tgrind-c

tgrind-awk:
	@echo $(PROGNAME).awk
	$(TGRIND) $(TGRINDFLAGS) $(PROGNAME).awk 2>&1 | $(GREP) '^-.*[.]ps'

tgrind-c:	vgrindefs.v
	@for f in *.[ch] ; \
	do \
		echo $$f ; \
		$(TGRIND) $(TGRINDFLAGS) -l C -d vgrindefs.v $$f 2>&1 | $(GREP) '^-.*[.]ps' ; \
		sleep 1 ; \
	done
	-@$(RM) tg_[0-9][0-9][0-9][0-9][0-9].dvi tg_[0-9][0-9][0-9][0-9][0-9].log tg_[0-9][0-9][0-9][0-9][0-9].tex

txt:	$(TXTFILES)

uninstall:	uninstall-exe uninstall-shrlib uninstall-man

uninstall-exe:
	-for f in $(PROGRAMS) ; \
	do \
		$(RM) $(bindir)/$$f $(bindir)/$$f-$(VERSION) 2>/dev/null; \
	done

uninstall-man:
	-for f in $(MANFILES) ; \
	do \
		g=`basename $$f .man` ; \
		$(RM) $(mandir)/$$g.$(manext) 2>/dev/null; \
		$(RM) $(catdir)/$$g.$(manext) 2>/dev/null; \
	done

uninstall-shrlib:
	-$(RM) -r $(shrlibdir) 2>/dev/null

$(PROGNAME).o:	$(PROGNAME).c awklib.h ndiff.h
awklib.o:	awklib.c awklib.h
store.o:	store.c ndiff.h
