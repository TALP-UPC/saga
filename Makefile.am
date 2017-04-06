ACLOCAL_AMFLAGS = -I autotools/m4

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
libsaga_la_SOURCES = src/utils/LisUdf.c src/utils/Util.c src/utils/PosixCompat.c
libsaga_la_CFLAGS = -DSAGA_DICCDIR=\"$(pkgdatadir)/dicc/\"
# version-info is defined according to the libtool manual:
#
# 1. Start with version information of ‘0:0:0’
# 2. Update the version information only immediately before a public release of
#    your software. More frequent updates are unnecessary, and only guarantee that
#    the current interface number gets larger faster.
# 3. If the library source code has changed at all since the last update, then
#    increment revision (‘c:r:a’ becomes ‘c:r+1:a’).
# 4. If any interfaces have been added, removed, or changed since the last update,
#    increment current, and set revision to 0.
# 5. If any interfaces have been added since the last public release, then
#    increment age.
# 6. If any interfaces have been removed or changed since the last public release,
#    then set age to 0.
libsaga_la_LDFLAGS = -no-undefined -version-info 2:0:0
libsaga_la_LIBADD = 

libsaga_la_SOURCES += src/SagaAcc.c src/SagaDic.c src/SagaExt.c \
                      src/SagaSil.c src/SagaTrn.c src/SagaCom.c \
                      src/SagaEngine.c

libsaga_la_CFLAGS += -I$(srcdir)/src -I$(srcdir)/src/utils -I$(srcdir)/include

# Headers
pkginclude_HEADERS += \
  include/Saga.h

noinst_HEADERS += \
  src/utils/LisUdf.h \
  src/utils/Util.h \
  src/utils/PosixCompat.h \
  src/ExcAcc.h \
  src/SagaInternal.h

# The main program
bin_PROGRAMS += saga

saga_CFLAGS = -I$(srcdir)/include -I$(srcdir)/src -I$(srcdir)/src/utils
saga_SOURCES = bin/Saga.c
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

check_SCRIPTS = test/test_saga_example.sh

EXTRA_DIST += test/test_readlisudf.txt

CLEANFILES += test/test_saga_example.sh
EXTRA_DIST += test/test_saga_example.sh.in

do_subst_test_saga_example = 

test/test_saga_example.sh: test/test_saga_example.sh.in Makefile
	sed -e 's,[@]abs_top_srcdir[@],$(abs_top_srcdir),g'  < $(srcdir)/test/test_saga_example.sh.in > test/test_saga_example.sh
	chmod +x test/test_saga_example.sh

noinst_PROGRAMS = test/saga_example

test_saga_example_SOURCES = test/saga_example.c
test_saga_example_LDADD = libsaga.la 
test_saga_example_CFLAGS = $(libsaga_la_CFLAGS)



test_test_saga_SOURCES = test/test_saga.c
test_test_saga_LDADD = libsaga.la 
test_test_saga_CFLAGS = $(libsaga_la_CFLAGS) \
    -DTEST_FILE_DIR=\"$(top_srcdir)/test/\"

TESTS = test/test_saga test/test_saga_example.sh

EXTRA_DIST += gpl-3.0.txt README.md README.es.md \
 doc/Saga_man.pdf \
 doc/Saga_Reglas_Trans.pdf \
 doc/SAMPA_Spanish_93.pdf \
 doc/SpanishDialectalVariants-ICSLP98.pdf

clean-local:
	find . -name '*.gcno' -exec rm {} \;
	find . -name '*.gcda' -exec rm {} \;

