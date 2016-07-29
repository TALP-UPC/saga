#!/bin/sh
mkdir -p "m4" "config"
libtoolize \
&& aclocal \
&& automake --add-missing \
&& autoconf
