dnl Use autogen.sh to regenerate the configure script
AC_PREREQ([2.62])
AC_INIT([saga],
        [1.0.9000],
        [https://github.com/TALP-UPC/saga/])

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])


AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_PROG_CC
AM_PROG_AR
AM_SILENT_RULES([no])
AC_PROG_INSTALL
AC_CONFIG_SRCDIR([src/Saga.c])
AC_CONFIG_HEADERS([config.h])
LT_PREREQ([2.2])
AC_C_BIGENDIAN

dnl By default disable building static libraries.
dnl use ./configure --enable-static
dnl to build them.
LT_INIT([disable-static])


AC_CONFIG_FILES([Makefile saga.pc])
AC_OUTPUT

