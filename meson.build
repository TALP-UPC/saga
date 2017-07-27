project('saga', 'c', default_options : ['c_std=c99'])

saga_version = '2.0.0'

# Where the dictionaries are installed
diccdir = join_paths(get_option('datadir'), 'saga', 'dicc')

saga_inc = include_directories(['src', 'src/utils', 'include'])

sagalib = library('saga',
        ['src/utils/LisUdf.c', 'src/utils/Util.c', 'src/utils/PosixCompat.c',
         'src/SagaAcc.c', 'src/SagaDic.c', 'src/SagaExt.c', 'src/SagaSil.c',
         'src/SagaTrn.c', 'src/SagaCom.c', 'src/SagaEngine.c'],
        include_directories : saga_inc,
        version : saga_version,
        install : true)

sagadep = declare_dependency(link_with : sagalib, 
                             include_directories : saga_inc)

executable('saga', 'bin/Saga.c', dependencies: sagadep, install: true)


conf_data = configuration_data()
conf_data.set('SAGA_DICCDIR', '"' + join_paths(get_option('prefix'), diccdir) + '"')

configure_file(output : 'config.h',
               configuration : conf_data, install: true)


install_headers('include/Saga.h', subdir : 'saga')

dialects = [['Arg', ['ArgExc.dicc', 'ArgDicGrp.dicc',
                     'ArgNovFon.dicc', 'ArgSust.dicc']],
            ['Cas', ['CasTrnFon.dicc', 'CasDicSust.dicc', 'CasDicSustOPC.dicc',
                     'CasDicGrp.dicc', 'CasTrnPal.dicc', 'CasExc.dicc',
                     'CasNovCns.dicc']],
            ['Chl', ['ChlNovFon.dicc', 'ChlGrup.dicc',
                     'ChlSust.dicc', 'ChlExc.dicc']],
            ['Per', ['PerSust.dicc', 'PerExc.dicc']],
            ['Ven', ['VenExc.dicc', 'VenSust.dicc']],
            ['Col', ['ColExc.dicc', 'ColSust.dicc']],
            ['Mex', ['MexSust.dicc',  'MexExc.dicc']]]

# Install dictionaries to the directory:
foreach dialect_files : dialects
  install_dialect_files = join_paths(diccdir, dialect_files[0])
  foreach install_file : dialect_files[1]
    install_data(join_paths('dicc', dialect_files[0], install_file),
                 install_dir : install_dialect_files)
  endforeach
endforeach


install_man('man/man1/saga.1')

# Tests:

e = executable('test_saga', 'test/test_saga.c',
               dependencies: sagadep,
               c_args : '-DTEST_FILE_DIR="@0@/test/"'.format(meson.current_source_dir()))
test('test_saga', e)

e2 = executable('saga_example', 'test/saga_example.c',
                dependencies: sagadep)

test('saga_example', e2, env : ['SAGA_DICCDIR=@0@/dicc/'.format(meson.current_source_dir())])

# pkg-config

pkg = import('pkgconfig')

pkg.generate(libraries : sagalib, name : 'saga',
             description: 'A library for Spanish phonetic transcription.',
             subdirs: 'saga',
             version: saga_version)


