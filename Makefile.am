ACLOCAL_AMFLAGS = -I m4

SUBDIRS = .

EXTRA_DIST = gpl-3.0.txt Readme.txt \
 doc/Saga_man.pdf \
 doc/Saga_Reglas_Trans.pdf \
 doc/SAMPA_Spanish_93.pdf \
 doc/SpanishDialectalVariants-ICSLP98.pdf \
 man/man1/Saga.1 \
 work/Peru \
 work/Mexico \
 work/Argentina \
 work/Colombia \
 work/Castilla \
 work/Chile \
 work/Venezuela \
 work/dicc/Arg/ArgExc.dicc \
 work/dicc/Arg/ArgDicGrp.dicc \
 work/dicc/Arg/ArgNovFon.dicc \
 work/dicc/Arg/ArgReadme.txt \
 work/dicc/Arg/ArgSust.dicc \
 work/dicc/Cas/CasTrnFon.dicc \
 work/dicc/Cas/CasDicSust.dicc \
 work/dicc/Cas/CasDicSustOPC.dicc \
 work/dicc/Cas/CasReadme.txt \
 work/dicc/Cas/ArgReadme.txt \
 work/dicc/Cas/CasDicGrp.dicc \
 work/dicc/Cas/CasTrnPal.dicc \
 work/dicc/Cas/CasExc.dicc \
 work/dicc/Cas/CasNovCns.dicc \
 work/dicc/Chl/ChlNovFon.dicc \
 work/dicc/Chl/ChlGrup.dicc \
 work/dicc/Chl/ChlReadme.txt \
 work/dicc/Chl/ChlSust.dicc \
 work/dicc/Chl/ChlExc.dicc \
 work/dicc/Per/PerSust.dicc \
 work/dicc/Per/PerExc.dicc \
 work/dicc/Per/PerReadme.txt \
 work/dicc/Ven/VenExc.dicc \
 work/dicc/Ven/VenReadme.txt \
 work/dicc/Ven/VenSust.dicc \
 work/dicc/Col/ColExc.dicc \
 work/dicc/Col/ColReadme.txt \
 work/dicc/Col/ColSust.dicc \
 work/dicc/Mex/MexSust.dicc \
 work/dicc/Mex/MexExc.dicc \
 work/dicc/Mex/MexReadme.txt


noinst_HEADERS = 
include_HEADERS =
lib_LTLIBRARIES =

lib_LTLIBRARIES += libveu.la
libveu_la_SOURCES = Util/LisUdf.c Util/Util.c
libveu_la_CFLAGS = -I$(srcdir)/Util
libveu_la_LDFLAGS = -no-undefined
libveu_la_LIBADD = 

lib_LTLIBRARIES += libsaga.la
libsaga_la_SOURCES = src/SagaAcc.c src/SagaDic.c src/SagaExt.c src/SagaSil.c src/SagaTrn.c src/SagaCom.c
libsaga_la_CFLAGS = -I$(srcdir)/src -I$(srcdir)/Util
libsaga_la_LDFLAGS = -no-undefined
libsaga_la_LIBADD = libveu.la

bin_PROGRAMS = saga

saga_CFLAGS = -I$(srcdir)/src -I$(srcdir)/Util
saga_SOURCES = src/Saga.c
saga_LDADD = libsaga.la libveu.la

include_HEADERS += \
  Util/LisUdf.h \
  Util/Util.h \
  src/ExcAcc.h \
  src/Saga.h 
