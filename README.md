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

The `work/dicc` directory contains the dictionaries that change SAGA behaviour
for the Spanish variants. These dictionaries can be modified to customize the
phonetic transcriptions as desired. Each of the `work/dicc` directories contains
a README file, describing the additional SAGA command line options needed.

SAGA source code is available under the `src` directory.

Using SAGA as a library
------------------------

This is a sample code to use SAGA in an external program:

```c
#include <stdio.h>
#include <stdlib.h>

#include "Saga.h"

int main() {
  /* Input text */
  const char texto[] = "hola mundo";
  /* Placeholders for all possible outputs: They will be allocated as needed. We must free them */
  char *phonemes = NULL; /* 'o-la m'un-do */
  char *phonemes_separated = NULL; /* 'o l a m 'u n d o */
  char *phonemes_sep_words = NULL; /* 'o - l a / m 'u n - d o */
  char *semisyllables = NULL; /* o- -'o la- -a mu- -'un do- -o  */
  char *semiphonemes = NULL; /* .-'o 'o+l 'o-l l+a l-a a+. .-m m+'u m-'u 'u+n 'u-n n+d n-d d+o d-o o+. */

  /* Load engine */
  SagaEngine *engine = SagaEngine_NewFromVariant("castilla");
  if (engine == NULL) {
		return EXIT_FAILURE;
  }

  /* Transcribe the text */
  if (SagaEngine_TranscribeText(engine, texto, "UTF-8", &phonemes, &phonemes_separated,
                                &phonemes_sep_words, &semisyllables, &semiphonemes) < 0) {
    SagaEngine_Clear(engine);
    free(engine);
    printf("Error in transcribe text\n");
	return EXIT_FAILURE;
  }
  /* Print all outputs */
  printf("Phonemes: %s\n", phonemes);
  /* Phonemes: 'o-la m'un-do */
  printf("Phonemes separated: %s\n", phonemes_separated);
  /* Phonemes separated:  'o l a m 'u n d o  */
  printf("Phonemes separated and separating words: %s\n", phonemes_sep_words);
  /* Phonemes separated and separating words: 'o - l a / m 'u n - d o */
  printf("Semisyllables: %s\n", semisyllables);
  /* Semisyllables: o- -'o la- -a mu- -'un do- -o  */
  printf("Semiphonemes: %s\n", semiphonemes);
  /* Semiphonemes: .-'o 'o+l 'o-l l+a l-a a+. .-m m+'u m-'u 'u+n 'u-n n+d n-d d+o d-o o+. */

  /* Free the output */
  free(phonemes);
  free(phonemes_separated);
  free(phonemes_sep_words);
  free(semisyllables);
  free(semiphonemes);

  /* Clear the engine, prepare it for the next transcription */
  SagaEngine_Refresh(engine);

  /* Another text to transcribe */
  const char texto2[] = "hola mundo me gusta la carne";
  /* Now we just want one output */
  phonemes_sep_words = NULL;
  if (SagaEngine_TranscribeText(engine, texto2, "UTF-8",
                            NULL, NULL, &phonemes_sep_words, NULL, NULL) < 0) {
    SagaEngine_Clear(engine);
    free(engine);
    printf("Error in transcribe text\n");
	return EXIT_FAILURE;
  }
  printf("%s\n", phonemes_sep_words);
  /* 'o - l a / m 'u n - d o / m e / G 'u s - t a / l a / k 'a r - n e */
  free(phonemes_sep_words);
  /* Final clear */
  SagaEngine_Refresh(engine);
  SagaEngine_Clear(engine);
  free(engine);
  return EXIT_SUCCESS;
}
```

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

