SAGA
========

SAGA es una aplicación para la transcripción fonética del español, considerando
sus múltiples variantes dialectales. Esta aplicación se distribuye bajo la GNU
General Public License (http://www.gnu.org/licenses/gpl-3.0.txt).

Instalación
--------------

El primer paso depende de si dispone de una versión distribuída en .tar.gz o
de un repositorio git.

Desde git:

    git clone https://github.com/TALP-UPC/saga.git
    cd saga
    ./autogen.sh # genera el script configure. Requiere autotools-dev

Desde tarball:

    tar xzf saga-1.1.tgz

Si se desea instalar SAGA en un directorio personalizado, hay que usar
`--prefix=` a fin de especificar el directorio dónde se desee instalar SAGA.

    ./configure --prefix="$HOME"
    make
    make install

Uso de SAGA
-------------

SAGA puede cubrir varias transcripciones fonéticas, cubriendo varias variantes
dialectales del español:

- La versión "canónica" del español hablado en España, como se estableció en el
  proyecto SAM-A.
- Las variedades dialectales de latinoamérica definidas en el proyecto SALA
  (SpeechDat Across Latin-America)

Una vez SAGA está instalado, los scripts `saga-*` en el directorio `bin`
permiten utilizar SAGA en todas las variantes dialectales soportadas.

Modificando SAGA
-----------------

En el directorio "work/dicc" se incluyen subdirectorios con los diccionarios 
que modifican el comportamiento de SAGA para las distintas variantes del 
español. Estos diccionarios pueden ser ampliados/modificados según interese
de acuerdo con las propiedades de la transcripción fonética que se pretenda.
Además, cada subdirectorio en `work/dicc` incluye un documento `Readme` que
indica las opciones de SAGA que han de tomarse para obtener la variante de
la transcripción.

El código fuente de SAGA se encuentra en el directorio `src`.

Usar SAGA como librería
------------------------

Este es un ejemplo de código para usar SAGA en un programa externo:

```c
#include <stdio.h>
#include <stdlib.h>

#include "Saga.h"

int main() {
  const char texto[] = "hola mundo\nhola otra vez";
  char *fon = NULL;
  SagaEngine *engine = SagaEngine_NewFromVariant("castilla");
  if (engine == NULL) {
		return EXIT_FAILURE;
	}
  if (SagaEngine_TranscribeText(engine, texto, "ISO-8859-15",
                            &fon, NULL, NULL, NULL, NULL) < 0) {
    SagaEngine_Clear(engine);
    free(engine);
		return EXIT_FAILURE;
	}
  printf("%s\n", fon);
  free(fon);
  SagaEngine_Clear(engine);
  free(engine);
  return EXIT_SUCCESS;
}
```

Si Saga está instalado correctamente, `pkg-config` debería poder mostrar
los parámetros de compilación necesarios:

 - `pkg-config --cflags saga`
 - `pkg-config --libs saga`


Más documentación
------------------

En el directorio "doc" se encuentran los siguientes documentos:

- Manual de usuario del programa (SAGA_man)
- Detalle de las reglas de transcripción (Saga_Reglas_Trans)
- Trabajos publicados en los que se describe la transcripción que realiza SAGA:
   * J. Llisterri, José B. Mariño, "Spanish adaptation of SAMPA and automatic
     phonetic transcription", Proyecto Esprit 6819. Informe  SAM-A/UPC /001/V1
     (Feb. 1993). (SAMPA-Spanish-93)
   * A. Moreno, José B. Mariño, "Spanish dialects: phonetic transcription",
     Proc. ICSLP'98, pp. 189-192, Sydney, Australia (Nov. 1998).
     (SpanishDialectalVariants-ICSLP98)

Autores principales
--------------------

- José B. Mariño, Universidad Politécnica de Catalunya -- BarcelonaTech
- Albino Nogueiras Rodríguez, Polytechnic University of Catalonia -- BarcelonaTech

Colaboradores
---------------

- Sergio Oller Moreno: Estandarización de scripts para compilar. Mejora de
                       documentación.

