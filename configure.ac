dnl Process this file with autoconf to produce a configure script.

AC_INIT(src/main.c)
AM_CONFIG_HEADER(config.h)

PACKAGE=xfresco
VERSION=2.0

AM_INIT_AUTOMAKE($PACKAGE,$VERSION)

AC_PROG_CC
AC_PROG_INSTALL
AC_STDC_HEADERS

PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2)
AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)

AC_OUTPUT([
Makefile 
src/Makefile 
pixmaps/Makefile
])


