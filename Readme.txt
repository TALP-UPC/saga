     *******************************************************************
     ************************** SAGA ***********************************
     *******************************************************************

SAGA es una aplicaci�n para la transcripci�n fon�tica del espa�ol. Esta 
aplicaci�n se distribuye bajo la GNU General Public License 
(http://www.gnu.org/licenses/gpl-3.0.txt).

INSTALACI�N

Para instalar Saga, hay que descomprimir el fichero Saga.tgz en el
directorio donde se desee realizar la instalacion. Saga.tgz es un fichero
creado con el comando tar de Unix/Linux que puede ser descomprimido usando
la orden:

	tar xvzf Saga.tgz

A continuacion, ejecutese el comando make para obtener el fichero
ejecutable.  Este, asi como los objetos y las librerias necesarios para
generarlo, se crearan en los directorios bin, obj y lib, respectivamente.
Por defecto, estos directorios cuelgan directamente del directorio raiz de
la instalacion, aunque este comportamiento puede modificarse editando el
fichero makefile.

Tambien se crearan los directorios man, donde se aloja la pagina de
manual; doc, donde se alojan las referencias en formato pdf; y
work, donde se presentan distintos scripts de ejemplo que permiten realizar
la transcripcion fonetica en varios dialectos del espanhol.

En la presente distribuci�n se incluyen las variantes siguientes:
1.- versi�n can�nica del espa�ol hablado en Espa�a seg�n se estableci� en 
el proyecto SAM-A, y 
2.- las variedades dialectales de latinoam�rica definidas en el proyecto
SALA (Spanish Across Latino-America).

En el directorio "work/dicc" se incluyen subdirectorios con los diccionarios 
que modifican el comportamiento de SAGA para las distintas variantes del 
espa�ol. Estos diccionarios pueden ser ampliados/modificados seg�n interese
de acuerdo con las propiedades de la transcripci�n fon�tica que se pretenda.
Adem�s, el documento Readme.txt indica las opciones de SAGA que han de 
tomarse para obtener la variante de la transcripci�n.

En el directorio "doc" se encuentran los siguientes documentos:
1.- Manual de usuario del programa (SAGA_man)
2.- Detalle de las reglas de transcripci�n (Saga_Reglas_Trans)
3.- Trabajos publicados en los que se describe la transcripci�n que realiza 
SAGA:
 (SAMPA-Spanish-93): J. Llisterri, Jos� B. Mari�o,"Spanish adaptation of 
SAMPA and automatic phonetic transcription", Proyecto Esprit 6819. Informe 
SAM-A/UPC /001/V1 (febrero 1993).
 (SpanishDialectalVariants-ICSLP98): A. Moreno, Jos� B. Mari�o, �Spanish 
dialects: phonetic transcription�, Proc. ICSLP�98, pp. 189-192, Sydney, 
Australia (noviembre 1998).

En el directorio "src" se incluye el c�digo fuente de SAGA.

Jos� B. Mari�o
UPC
Junio 2009.
