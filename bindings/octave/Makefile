
SUBDIRS= demos etc

include $(TOPDIR)/Makefile.glob

ifndef LINUX
	LIBS += -lplplotdtk -lMatrix $(TK_LIB) -ltcl8.0 -lX11 -lsocket
else
	LIBS += -L/usr/X11R6/lib -L/usr/local/lib -lplplotdtk -lMatrix \
	$(TK_LIB) -ltcl8.0 -lX11 -lm
endif

all:: plplot_octave.oct plplot_stub.m

plplot_octave.oct: plplot_octave.cc
	$(MKOCTFILE) plplot_octave.cc $(LIBS)
	$(CP) plplot_octave.oct PLplot

plplot_octave.cc tmp_stub: plplot.h plplot_rej.h
	matwrap -language octave plplot.h -o plplot_octave.cc -stub tmp_stub 2> matwrap_warnings

plplot_stub.m: etc/plplot.doc etc/po.dvi tmp_stub massage
	head -5 tmp_stub > plplot_stub.m
	echo "# It was also massaged to add online documentation" >> plplot_stub.m
	echo "# extracted from some PLplot distribution files" >> plplot_stub.m
	echo	>> plplot_stub.m
	echo "1;" >> plplot_stub.m
	echo >> plplot_stub.m
	./massage >> plplot_stub.m
	$(CP) plplot_stub.m PLplot

massage: massage.c
	$(C) $^ -o $@

distclean:
	-$(RM) *.o plplot_octave.oct plplot_octave.cc plplot_stub.m massage \
	octave-core tmp_stub core PLplot/plplot_octave.oct PLplot/plplot_stub.m

clean::
	-$(RM) *.o plplot_octave.oct massage \
	octave-core core

install::
	$(CP) -r PLplot $(DISTDIR)/PLplot
