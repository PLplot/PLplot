include $(TOPDIR)/Makefile.glob

all:: po.dvi po/po.css

po/po.css:
	latex2html -no_images -ascii_mode -no_navigation -info "" -address "" po.tex
	./toascii

po.dvi:	po.tex api.tex
	latex po.tex
	latex po.tex
	dvips po.tex

clean::
	-$(RM) -R po po.dvi po.ps po.log po.aux

