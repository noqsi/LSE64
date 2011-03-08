#!/bin/sh

autoreconf -f -i -Wall,no-obsolete
automake --add-missing
#autoconf
#libtoolize --ltdl --force
aclocal
