dnl Use autogen.sh to regenerate the configure script
AC_PREREQ([2.62])
AC_INIT([saga],
        [2.0],
        [https://github.com/TALP-UPC/saga/])

AC_CONFIG_AUX_DIR([autotools])
AC_CONFIG_MACRO_DIR([autotools/m4])


AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_PROG_CC
AM_PROG_AR
AM_SILENT_RULES([no])
AC_PROG_INSTALL
AC_CONFIG_SRCDIR([src/SagaEngine.c])
AC_CONFIG_HEADERS([include/config.h])
LT_PREREQ([2.2])
AC_C_BIGENDIAN

LT_INIT()
AC_CHECK_DECLS([strdup, getopt])
AC_CHECK_FUNCS([strdup getopt])
AC_TYPE_SSIZE_T

AC_CONFIG_FILES([Makefile saga.pc])
AC_OUTPUT

