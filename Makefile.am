ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES =
pkginclude_HEADERS =
noinst_HEADERS =
bin_PROGRAMS =
CLEANFILES =
EXTRA_DIST = saga.pc.in


pkgconfiginstalldir = $(libdir)/pkgconfig
pkgconfiginstall_DATA = saga.pc

# Shared library
lib_LTLIBRARIES += libsaga.la
libsaga_la_SOURCES = Util/LisUdf.c Util/Util.c Util/PosixCompat.c
libsaga_la_CFLAGS = -I$(srcdir)/Util -DSAGA_DICCDIR=\"$(pkgdatadir)/dicc/\"
libsaga_la_LDFLAGS = -no-undefined
libsaga_la_LIBADD = 

libsaga_la_SOURCES += src/SagaAcc.c src/SagaDic.c src/SagaExt.c \
                      src/SagaSil.c src/SagaTrn.c src/SagaCom.c \
                      src/SagaEngine.c

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
 dicc/Arg/ArgExc.dicc \
 dicc/Arg/ArgDicGrp.dicc \
 dicc/Arg/ArgNovFon.dicc \
 dicc/Arg/ArgReadme.txt \
 dicc/Arg/ArgSust.dicc

datadicccasdir = $(datadiccdir)/Cas
dist_datadicccas_DATA =  \
 dicc/Cas/CasTrnFon.dicc \
 dicc/Cas/CasDicSust.dicc \
 dicc/Cas/CasDicSustOPC.dicc \
 dicc/Cas/CasReadme.txt \
 dicc/Cas/CasDicGrp.dicc \
 dicc/Cas/CasTrnPal.dicc \
 dicc/Cas/CasExc.dicc \
 dicc/Cas/CasNovCns.dicc

datadiccchldir = $(datadiccdir)/Chl
dist_datadiccchl_DATA =  \
 dicc/Chl/ChlNovFon.dicc \
 dicc/Chl/ChlGrup.dicc \
 dicc/Chl/ChlReadme.txt \
 dicc/Chl/ChlSust.dicc \
 dicc/Chl/ChlExc.dicc

datadiccperdir = $(datadiccdir)/Per
dist_datadiccper_DATA =  \
 dicc/Per/PerSust.dicc \
 dicc/Per/PerExc.dicc \
 dicc/Per/PerReadme.txt

datadiccvendir = $(datadiccdir)/Ven
dist_datadiccven_DATA =  \
 dicc/Ven/VenExc.dicc \
 dicc/Ven/VenReadme.txt \
 dicc/Ven/VenSust.dicc

datadicccoldir = $(datadiccdir)/Col
dist_datadicccol_DATA =  \
 dicc/Col/ColExc.dicc \
 dicc/Col/ColReadme.txt \
 dicc/Col/ColSust.dicc

datadiccmexdir = $(datadiccdir)/Mex
dist_datadiccmex_DATA =  \
 dicc/Mex/MexSust.dicc \
 dicc/Mex/MexExc.dicc \
 dicc/Mex/MexReadme.txt


# Documentation
dist_man1_MANS = man/man1/saga.1

check_PROGRAMS = test/test_saga

test_test_saga_SOURCES = test/test_saga.c
test_test_saga_LDADD = libsaga.la 
test_test_saga_CFLAGS = $(libsaga_la_CFLAGS) \
    -DTEST_FILE_DIR=\"$(top_srcdir)/test/\"

TESTS = test/test_saga

EXTRA_DIST += gpl-3.0.txt Readme.txt \
 doc/Saga_man.pdf \
 doc/Saga_Reglas_Trans.pdf \
 doc/SAMPA_Spanish_93.pdf \
 doc/SpanishDialectalVariants-ICSLP98.pdf

clean-local:
	find . -name '*.gcno' -exec rm {} \;
	find . -name '*.gcda' -exec rm {} \;

