ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES =
pkginclude_HEADERS =
noinst_HEADERS =
bin_PROGRAMS =
CLEANFILES =
EXTRA_DIST =

# Shared library
lib_LTLIBRARIES += libsaga.la
libsaga_la_SOURCES = Util/LisUdf.c Util/Util.c
libsaga_la_CFLAGS = -I$(srcdir)/Util
libsaga_la_LDFLAGS = -no-undefined
libsaga_la_LIBADD = 

libsaga_la_SOURCES += src/SagaAcc.c src/SagaDic.c src/SagaExt.c \
                      src/SagaSil.c src/SagaTrn.c src/SagaCom.c src/SagaEngine.c

libsaga_la_CFLAGS += -I$(srcdir)/src

# Headers
pkginclude_HEADERS += \
  src/Saga.h

noinst_HEADERS += \
  Util/LisUdf.h \
  Util/Util.h \
  src/ExcAcc.h \
  src/SagaInternal.h

# The main program
bin_PROGRAMS += saga

saga_CFLAGS = -I$(srcdir)/src -I$(srcdir)/Util
saga_SOURCES = src/Saga.c
saga_LDADD = libsaga.la

# Data:
datadiccdir = $(pkgdatadir)/dicc

datadiccargdir = $(datadiccdir)/Arg
dist_datadiccarg_DATA =  \
 work/dicc/Arg/ArgExc.dicc \
 work/dicc/Arg/ArgDicGrp.dicc \
 work/dicc/Arg/ArgNovFon.dicc \
 work/dicc/Arg/ArgReadme.txt \
 work/dicc/Arg/ArgSust.dicc

datadicccasdir = $(datadiccdir)/Cas
dist_datadicccas_DATA =  \
 work/dicc/Cas/CasTrnFon.dicc \
 work/dicc/Cas/CasDicSust.dicc \
 work/dicc/Cas/CasDicSustOPC.dicc \
 work/dicc/Cas/CasReadme.txt \
 work/dicc/Cas/CasDicGrp.dicc \
 work/dicc/Cas/CasTrnPal.dicc \
 work/dicc/Cas/CasExc.dicc \
 work/dicc/Cas/CasNovCns.dicc

datadiccchldir = $(datadiccdir)/Chl
dist_datadiccchl_DATA =  \
 work/dicc/Chl/ChlNovFon.dicc \
 work/dicc/Chl/ChlGrup.dicc \
 work/dicc/Chl/ChlReadme.txt \
 work/dicc/Chl/ChlSust.dicc \
 work/dicc/Chl/ChlExc.dicc

datadiccperdir = $(datadiccdir)/Per
dist_datadiccper_DATA =  \
 work/dicc/Per/PerSust.dicc \
 work/dicc/Per/PerExc.dicc \
 work/dicc/Per/PerReadme.txt

datadiccvendir = $(datadiccdir)/Ven
dist_datadiccven_DATA =  \
 work/dicc/Ven/VenExc.dicc \
 work/dicc/Ven/VenReadme.txt \
 work/dicc/Ven/VenSust.dicc

datadicccoldir = $(datadiccdir)/Col
dist_datadicccol_DATA =  \
 work/dicc/Col/ColExc.dicc \
 work/dicc/Col/ColReadme.txt \
 work/dicc/Col/ColSust.dicc

datadiccmexdir = $(datadiccdir)/Mex
dist_datadiccmex_DATA =  \
 work/dicc/Mex/MexSust.dicc \
 work/dicc/Mex/MexExc.dicc \
 work/dicc/Mex/MexReadme.txt

# Scripts, that need to be built:
bin_SCRIPTS =  work/saga-peru \
 work/saga-mexico \
 work/saga-argentina \
 work/saga-colombia \
 work/saga-castilla \
 work/saga-chile \
 work/saga-venezuela

CLEANFILES += $(bin_SCRIPTS)
EXTRA_DIST += work/saga-peru.in \
 work/saga-mexico.in \
 work/saga-argentina.in \
 work/saga-colombia.in \
 work/saga-castilla.in \
 work/saga-chile.in \
 work/saga-venezuela.in

do_subst = sed -e 's|__bindir__|$(bindir)|g' \
               -e 's|__datadir__|$(pkgdatadir)|g'

$(bin_SCRIPTS): %: %.in
	mkdir -p work
	$(do_subst) < $< > $@
	chmod +x $@

# Documentation
dist_man1_MANS = man/man1/saga.1

EXTRA_DIST += gpl-3.0.txt Readme.txt \
 doc/Saga_man.pdf \
 doc/Saga_Reglas_Trans.pdf \
 doc/SAMPA_Spanish_93.pdf \
 doc/SpanishDialectalVariants-ICSLP98.pdf

