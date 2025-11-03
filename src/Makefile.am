## Process this file with automake to produce Makefile.in

bin_PROGRAMS = xfresco

#LDADD	=@GTK_LIBS@
INCLUDES = \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	@DEPS_CFLAGS@

xfresco_SOURCES = \
	main.c \
	interface.c interface.h \
	callbacks.c callbacks.h \
	clist_compat.c clist_compat.h \
	namelist.c namelist.h \
	savefile.c savefile.h \
	openfile.c openfile.h \
	utils.c	utils.h \
	support.c support.h

xfresco_LDADD = @DEPS_LIBS@
