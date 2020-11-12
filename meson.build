project('saga', 'c', default_options : ['c_std=c99'], version : '2.0.1')
c_compiler = meson.get_compiler('c')

saga_version = meson.project_version()

# Where the dictionaries are installed
diccdir = join_paths(get_option('datadir'), 'saga', 'dicc')

saga_inc = include_directories(['src', 'src/utils', 'include'])

build_dll = ['-DSAGA_BUILDING_SHARED']
if c_compiler.has_argument('-fvisibility=hidden')
  build_dll += ['-fvisibility=hidden']
endif

lib_src = ['src/utils/LisUdf.c', 'src/utils/Util.c', 'src/utils/PosixCompat.c',
           'src/SagaAcc.c', 'src/SagaDic.c', 'src/SagaExt.c', 'src/SagaSil.c',
           'src/SagaTrn.c', 'src/SagaCom.c', 'src/SagaEngine.c']

saga_shared_lib = shared_library('saga', lib_src,
        include_directories : saga_inc,
        version : saga_version,
        install : true,
        c_args : build_dll)

saga_static_lib = static_library('saga', lib_src,
        include_directories : saga_inc,
        install : true,
        c_args : ['-DSAGA_BUILDING_STATIC'])


sagashareddep = declare_dependency(link_with : saga_shared_lib, 
                                   include_directories : saga_inc)

sagastaticdep = declare_dependency(link_with : saga_static_lib, 
                                   include_directories : saga_inc)


conf_data = configuration_data()
conf_data.set('SAGA_DICCDIR', '"' + join_paths(get_option('prefix'), diccdir) + '"')

configure_file(output : 'config.h',
               configuration : conf_data,
               install: true,
               install_dir : get_option('includedir') / 'saga')


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

executable('saga', ['bin/Saga.c'],
           dependencies: sagastaticdep, # test_saga uses private functions
           install: true,
           c_args : ['-DSAGA_BUILDING_STATIC'])


# Tests:
e = executable('test_saga', 'test/test_saga.c',
               dependencies: sagastaticdep, # test_saga uses private functions
               c_args : ['-DTEST_FILE_DIR="@0@/test/"'.format(meson.current_source_dir()),
                         '-DSAGA_BUILDING_STATIC'])
test('test_saga', e)

e2 = executable('saga_example', 'test/saga_example.c',
                dependencies: sagashareddep) # this example uses only public functions

test('saga_example', e2, env : ['SAGA_DICCDIR=@0@/dicc/'.format(meson.current_source_dir())])

# pkg-config

pkg = import('pkgconfig')

pkg.generate(libraries : saga_shared_lib, name : 'saga',
             description: 'A library for Spanish phonetic transcription.',
             subdirs: 'saga',
             version: saga_version)


