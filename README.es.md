SAGA
========

SAGA es una aplicaci�n para la transcripci�n fon�tica del espa�ol, considerando
sus m�ltiples variantes dialectales. Esta aplicaci�n se distribuye bajo la GNU
General Public License (http://www.gnu.org/licenses/gpl-3.0.txt).

Instalaci�n
--------------

El primer paso depende de si dispone de una versi�n distribu�da en .tar.gz o
de un repositorio git.

Desde git:

    git clone https://github.com/TALP-UPC/saga.git
    cd saga
    ./autogen.sh # genera el script configure. Requiere autotools-dev

Desde tarball:

    tar xzf saga-1.1.tgz

Si se desea instalar SAGA en un directorio personalizado, hay que usar
`--prefix=` a fin de especificar el directorio d�nde se desee instalar SAGA.

    ./configure --prefix="$HOME"
    make
    make install

Uso de SAGA
-------------

SAGA puede cubrir varias transcripciones fon�ticas, cubriendo varias variantes
dialectales del espa�ol:

- La versi�n "can�nica" del espa�ol hablado en Espa�a, como se estableci� en el
  proyecto SAM-A.
- Las variedades dialectales de latinoam�rica definidas en el proyecto SALA
  (SpeechDat Across Latin-America)

Una vez SAGA est� instalado, los scripts `saga-*` en el directorio `bin`
permiten utilizar SAGA en todas las variantes dialectales soportadas.

Modificando SAGA
-----------------

En el directorio "work/dicc" se incluyen subdirectorios con los diccionarios 
que modifican el comportamiento de SAGA para las distintas variantes del 
espa�ol. Estos diccionarios pueden ser ampliados/modificados seg�n interese
de acuerdo con las propiedades de la transcripci�n fon�tica que se pretenda.
Adem�s, cada subdirectorio en `work/dicc` incluye un documento `Readme` que
indica las opciones de SAGA que han de tomarse para obtener la variante de
la transcripci�n.

El c�digo fuente de SAGA se encuentra en el directorio `src`.

M�s documentaci�n
------------------

En el directorio "doc" se encuentran los siguientes documentos:

- Manual de usuario del programa (SAGA_man)
- Detalle de las reglas de transcripci�n (Saga_Reglas_Trans)
- Trabajos publicados en los que se describe la transcripci�n que realiza SAGA:
   * J. Llisterri, Jos� B. Mari�o, "Spanish adaptation of SAMPA and automatic
     phonetic transcription", Proyecto Esprit 6819. Informe  SAM-A/UPC /001/V1
     (Feb. 1993). (SAMPA-Spanish-93)
   * A. Moreno, Jos� B. Mari�o, "Spanish dialects: phonetic transcription",
     Proc. ICSLP'98, pp. 189-192, Sydney, Australia (Nov. 1998).
     (SpanishDialectalVariants-ICSLP98)

Autores principales
--------------------

- Jos� B. Mari�o, Universidad Polit�cnica de Catalunya -- BarcelonaTech
- Albino Nogueiras Rodr�guez, Polytechnic University of Catalonia -- BarcelonaTech

Colaboradores
---------------

- Sergio Oller Moreno: Estandarizaci�n de scripts para compilar. Mejora de
                       documentaci�n.

