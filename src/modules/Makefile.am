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

ico_la_sources = ico.c
ico_la_CFLAGS = -pthread
ico_la_LDFLAGS = $(common_ldflags)
ico_la_LIBADD = $(PIGPIOLIBS)

if HAVE_COMEDI
COMEDI = comedi.la
endif

if HAVE_SEQUENCER
SEQ = sequencer.la
endif

if HAVE_PIGPIO
ICO = ico.la
endif

MODULES = math.la fpga.la $(SEQ) $(COMEDI) $(ICO)

pkglib_LTLIBRARIES = $(MODULES)
