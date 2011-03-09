common_ldflags = -module

sequencer_la_SOURCES = sequencer.c
sequencer_la_LDFLAGS = $(common_ldflags)

comedi_la_SOURCES = comedi.c
comedi_la_LDFLAGS = $(common_ldflags)
comedi_la_LIBADD = $(COMEDILIBS)

math_la_SOURCES = math.c
math_la_LDFLAGS = $(common_ldflags)
math_la_LIBADD = -lm

fpga_la_sources = fpga.c
fpga_la_LDFLAGS = $(common_ldflags)

if HAVE_COMEDI
COMEDI = comedi.la
endif

if HAVE_SEQUENCER
SEQ = sequencer.la
endif

if HAVE_SHARED_MODULES
MODULES = $(SEQ) $(COMEDI) math.la fpga.la
endif

pkglib_LTLIBRARIES = $(MODULES)
