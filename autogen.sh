#!/bin/sh

autoreconf -f -i -Wall,no-obsolete
automake
autoconf
aclocal
libtoolize
