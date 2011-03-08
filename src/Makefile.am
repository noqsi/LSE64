SUBDIRS=modules

bin_PROGRAMS = lse64
noinst_PROGRAMS = t64
noinst_HEADERS = lse64.h

if LTDL_MODULES
INCLUDES = $(INCLTDL)
common_ldflags = -export-dynamic
common_ldadd = $(LIBLTDL)
loadmod = loadmod_ltdl.c
endif

if DL_MODULES
common_ldflags = -export-dynamic
common_ldadd = -ldl
loadmod = loadmod_dl.c
endif

lse64_SOURCES = lse64.c primitives.c strings.c $(loadmod)
lse64_CFLAGS = -DBOOT_FILE=\"${pkgdatadir}/boot.lse\" -DPKGLIBDIR=\"${pkglibdir}\"
lse64_LDFLAGS = $(common_ldflags)
lse64_LDADD = $(common_ldadd)
lse64_DEPENDENCIES = $(LTDLDEPS)

t64_SOURCES = lse64.c primitives.c strings.c $(loadmod)
t64_CFLAGS = -Wall -g
t64_LDFLAGS = $(common_ldflags)
t64_LDADD = $(common_ldadd)
t64_DEPENDENCIES = $(LTDLDEPS)

dist_pkgdata_DATA = boot.lse
