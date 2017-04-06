#!/bin/sh
mkdir -p "autotools/m4"
libtoolize --copy \
&& aclocal \
&& autoheader \
&& automake --add-missing --copy \
&& autoconf
