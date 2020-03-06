## Process this file with automake to produce Makefile.in

SUBDIRS	=	src
bin_SCRIPTS= runfresco

## Copied from glade editor example

EXTRA_DIST = \
	autogen.sh \
	xfresco2.gld \
	xfresco2.gldp

#dist_icon_DATA = xfresco_logo3.xpm
#icondir = $(datadir)/pixmaps



install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/pixmaps; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/pixmaps; \
	  for pixmap in $(srcdir)/pixmaps/*; do \
	    if test -f $$pixmap; then \
	      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/pixmaps; \
	    fi \
	  done \
	fi

dist-hook:
	if test -d pixmaps; then \
	  mkdir $(distdir)/pixmaps; \
	  for pixmap in pixmaps/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/pixmaps; \
	    fi \
	  done \
	fi


