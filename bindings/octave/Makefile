
all: massage plplot_octave.cc plplot_stub.m

plplot_octave.cc: plplot_octave_org.h plplot_octave_rej.h
	sed plplot_octave_org.h -e 's/REPLACE_ME/typedef double PLFLT;/' > plplot_octave.h
	matwrap -language octave plplot_octave.h -o plplot_octave.cc \
		-stub tmp_stub 2> matwrap_warnings
	mv plplot_octave.cc plplot_octave_d.cc
	mv plplot_octave.h plplot_octave_d.h
	sed plplot_octave_org.h -e 's/REPLACE_ME/typedef float PLFLT;/' > plplot_octave.h
	matwrap -language octave plplot_octave.h -o plplot_octave.cc \
		-stub tmp_stub 2> matwrap_warnings
	mv plplot_octave.cc plplot_octave_f.cc
	mv plplot_octave.h plplot_octave_f.h

plplot_stub.m: etc/plplot.doc tmp_stub massage
	head -5 tmp_stub > plplot_stub.m
	echo "# It was also massaged to add online documentation" >> plplot_stub.m
	echo "# extracted from some PLplot distribution files" >> plplot_stub.m
	echo	>> plplot_stub.m
	echo "1;" >> plplot_stub.m
	echo >> plplot_stub.m
	./massage >> plplot_stub.m 2> missing_help
	cp plplot_stub.m PLplot

massage: massage.c
	$(CC) $^ -o $@

