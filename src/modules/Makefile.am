common_ldflags = -module

modsequencer_la_SOURCES = modsequencer.c
modsequencer_la_LDFLAGS = $(common_ldflags)

modcomedi_la_SOURCES = modcomedi.c
modcomedi_la_LDFLAGS = $(common_ldflags)
modcomedi_la_LIBADD = $(COMEDILIBS)

modmath_la_SOURCES = modmath.c
modmath_la_LDFLAGS = $(common_ldflags)
modmath_la_LIBADD = -lm

if HAVE_COMEDI
COMEDI = modcomedi.la
endif

if HAVE_SEQUENCER
SEQ = modsequencer.la
endif

if HAVE_SHARED_MODULES
MODULES = $(SEQ) $(COMEDI) modmath.la
endif

pkglib_LTLIBRARIES = $(MODULES)
