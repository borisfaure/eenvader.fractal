ACLOCAL_AMFLAGS = -I m4
MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.guess config.h.in \
		       config.sub configure depcomp install-sh ltmain.sh \
		       missing module.desktop config.rpath mkinstalldirs

SUBDIRS = src

EDJE_FLAGS = -v

filesdir = $(datadir)
files_DATA = eenvaders.edj

EXTRA_DIST = data/eenvaders.edc

eenvaders.edj:  data/eenvaders.edc
	$(EDJE_CC) $(EDJE_FLAGS) $< $@

clean-local:
	 rm -rf eenvaders.edj *~

uninstall:
	 rm -rf $(DESTDIR)$(datadir)
