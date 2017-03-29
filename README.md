SAGA
=========

Para leer la documentación en español consulte el fichero [README.es.md].

SAGA is an automatic tool for grapheme-to-allophone trasncription in Spanish,
considering its multiple dialectal variants. It is distributed under the GNU
General Public License (http://www.gnu.org/licenses/gpl-3.0.txt).

Installation
--------------

The first steps depend on whether you have a distribution tarball or a cloned
git repository.

From git only:

    git clone https://github.com/TALP-UPC/saga.git
    cd saga
    ./autogen.sh # generates the configure script. Requires autotools-dev

From a downloaded tarball only

    tar xzf saga-1.1.tgz

Typically you will want to install SAGA to a custom directory, use `--prefix=`
to specify the directory where Saga will be installed.
    
    ./configure --prefix="$HOME"
    make
    make install

Usage
------

SAGA is able to cover several grapheme-to-allophone transcriptions, covering
some of the Spanish dialectal variants:

- The canonical Spanish from Spain, as stablished in the SAM-A project
- The latinoamerica dialectal variants defined in the SALA
  (SpeechDat Across Latin-America) project.

Once installed, the `saga-*` scripts in the `bin` directory cover allow to use
SAGA in all the supported dialectal variants.


Modifying SAGA
---------------

The `dicc` directory contains the dictionaries that change SAGA behaviour
for the Spanish variants. These dictionaries can be modified to customize the
phonetic transcriptions as desired. Each of the `dicc` directories contains
a README file, describing the additional SAGA command line options needed.

SAGA source code is available under the `src` directory.

Using SAGA as a library
------------------------

In `test/saga_example.c` there is an example on how to use SAGA in an
external program.


If Saga has been installed properly, `pkg-config` should be able to show
the necessary compiler flags:

 - `pkg-config --cflags saga`
 - `pkg-config --libs saga`


Further documentation
------------------------

In the `doc` directory the following documents are available:

- SAGA user manual (SAGA_man)
- Phonetic transcrition rule documentation (Saga_Reglas_Trans)
- Publications describing SAGA transcriptions:
   * J. Llisterri, José B. Mariño, "Spanish adaptation of SAMPA and automatic
     phonetic transcription", Esprit project 6819. Report  SAM-A/UPC /001/V1
     (Feb. 1993). (SAMPA-Spanish-93)
   * A. Moreno, José B. Mariño, "Spanish dialects: phonetic transcription",
     Proc. ICSLP'98, pp. 189-192, Sydney, Australia (Nov. 1998).
     (SpanishDialectalVariants-ICSLP98)

Main Authors
--------------

- José B. Mariño, Polytechnic University of Catalonia -- BarcelonaTech
- Albino Nogueiras Rodríguez, Polytechnic University of Catalonia -- BarcelonaTech

Contributors
-------------

- Sergio Oller Moreno: Standarize build system. Documentation improvements.

