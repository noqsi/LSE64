# $Id: Makefile.am,v 1.2 2006/09/09 15:58:13 jpd Exp $

bin_PROGRAMS = lse64
noinst_PROGRAMS = t64
noinst_HEADERS = lse64.h

if HAVE_SHARED_MODULES
common_ldflags = -export-dynamic
loadmod = loadmod.c
endif

lse64_SOURCES = lse64.c primitives.c strings.c $(loadmod)
lse64_CFLAGS = -DBOOT_FILE=\"${pkgdatadir}/boot.lse\" -DPKGLIBDIR=\"${pkglibdir}\"
lse64_LDFLAGS = $(common_ldflags)
t64_SOURCES = lse64.c primitives.c strings.c $(loadmod)
t64_CFLAGS = -Wall -g
t64_LDFLAGS = $(common_ldflags)

dist_pkgdata_DATA = boot.lse

# $Log: Makefile.am,v $
# Revision 1.2  2006/09/09 15:58:13  jpd
# Release 0.3 with license and copyright notices
#
# Revision 1.1  2005/07/03 15:37:19  jpd
# *** empty log message ***
#
# Revision 1.3  2004/11/12 21:53:02  jpd
# Added support to drive sequencer through /dev/lp0.
#
# Revision 1.2  2004/09/28 17:15:52  jpd
# Use unsigned conversion for \ to avoid overflows.
# Now builds installable and test versions.
#
# Revision 1.1  2004/09/24 19:54:07  jpd
# Now using automake/autoconf.
#
