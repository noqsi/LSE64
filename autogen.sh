#!/bin/sh

autoreconf -f -i -Wall,no-obsolete
automake
autoconf
libtoolize --ltdl --force
aclocal
