#!/bin/sh
mkdir -p "autotools/m4"
case `uname` in Darwin*) glibtoolize --copy ;;
*) libtoolize --copy ;; esac

aclocal \
&& autoheader \
&& automake --add-missing --copy \
&& autoconf
