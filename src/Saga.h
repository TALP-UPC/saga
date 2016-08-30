/* Saga - Un transcriptor fonético para el idioma español
 *
 * Copyright (C) 1993-2009  Albino Nogueiras Rodríguez y José B. Mariño
 *       TALP - Universitat Politècnica de Catalunya, ESPAÑA
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SAGA_H
#define SAGA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <inttypes.h>

#if defined(_WIN32) && !defined(__attribute__)
#define __attribute__(A)
#endif

/* Accessor methods instead of defines could be used */
#define SESEO		0x00001
#define EQUIS_KS	0x00002
#define SC_KS		0x00004
#define ESE_ASP_INC	0x00008
#define ESE_ASP_CON	0x00010
#define BDG_ANDES	0x00020
#define ELIM_B		0x00040
#define ELIM_D		0x00080
#define ELIM_G		0x00100
#define ENE_VELAR	0x00200
#define NAS_VELAR	0x00400
#define ARCHI_IMPL	0x00800
#define Y_VOCAL		0x01000
#define ERRE_IMPL	0x02000
#define GRUPO_SIL	0x04000
#define MARCA_IMPL	0x08000
#define VOCAL_NASAL	0x10000
#define VOCAL_PTON	0x20000
#define OCLUS_EXPL	0x40000
#define INI_FIN_PAL	0x80000

typedef struct struct_SagaEngine {
  /* Argumentos de entrada (linea de comandos) */
  int TrnPalAis; /* Palabras aisladas? */
  int TrnLinAis; /* Lineas aisladas? */
  int ConSil; /* Conservar los silencios */
  const char *StrIniPal; /* Marca inicio palabra */
  const char *StrFinPal; /* Marca fin de palabra */
  const char *FicDicExc; /* Nombre fichero Diccionario excepciones */
  const char *FicTrnFon; /* Nombre fichero Diccionario transcripcion de grafemas */
  const char *FicTrnPal; /* Nombre fichero Diccionario transcripcion de palabras */
  const char *FicDicSust; /* Nombre fichero Diccionario de substitucion de fonemas */
  const char *FicDicGrp; /* Nombre fichero Diccionario de substitucion de grupos foneticos */
  const char *FicNovVoc; /* Nombre fichero Lista de grafemas vocalicos introducidos */
  const char *FicNovCons; /* Nombre fichero Lista de grafemas consonanticos introducidos */
  const char *FicNovFon; /* Nombre fichero Lista de fonemas introducidos */
  long ClaveModif; /* Incluye las opciones -M de la linea de comandos */

  /* Diccionarios cargados */
  char ***DicExc;
  char ***DicTrnFon;
  char ***DicTrnPal;
  char ***DicSust;
  char ***DicGrp;

  /* Listas de nuevos fonemas */
  char **LisNovVoc;
  char **LisNovCons;
  char **LisNovFon;
  /* Contenido de las listas */
  char **Letras;
  char **Fonemas;
  char **ConsTxt;
  char **Vocales;

  /* Opciones de salida */
  int SalFon; /* Transcripcion fonetica */
  int SalFnm; /* Transcripcion en fonemas */
  int SalFnmPal; /* En fonemas por palabras */
  int SalSefo; /* En semifonemas */
  int SalSem; /* En semisilabas */

  /* Entrada */
  char *TxtIn; /* Si la entrada es texto */
  intptr_t TxtInOffset; /* Si la entrada es texto, param interno */
  const char *FicInName; /* Si la entrada es de un fichero */
  FILE *FpIn; /* Si la entrada es de un fichero */
  int close_in; /* Si FpIn debe cerrarse */
    
  /* Lista de palabras extranhas en TxtOrt. Usado para reportar errores */
  char	**PalExt; /* Lista de palabras extranhas */
  size_t NumPalExt; /* Número de palabras extranhas */
  
  char* TxtOrt; /* Texto de entrada a transcribir */

  /* Salida */
  char* TrnFon; /* Transcripcion fonetica */
  char* TrnFnm; /* Transcripcion en fonemas */
  char* TrnFnmPal; /* En fonemas por palabras */
  char* TrnSem; /* En semisilabas */
  char* TrnSefo; /* En semifonemas */

  /* Ficheros de Salida */
  FILE *FpFon;
  FILE *FpFnm;
  FILE *FpFnmPal;
  FILE *FpSem;
  FILE *FpSefo;
  
  /* Error stream */
  FILE* FpErr;
  int close_err;
  
} SagaEngine;

/** Initializes engine. Has to be called after the engine is allocated
 * 
 * @param[in,out] engine The engine to initialize
 *  */
int SagaEngine_Initialize(SagaEngine *engine);

/** Opens `NomErr` as the file for error output. 
 * 
 * @param[in] NomErr The file name, or "-" for stderr or NULL for no error output. */
int SagaEngine_OpenErrorFile(SagaEngine *engine, const char *NomErr);

/** Opens `NomIn` to transcribe its contents. Use "-" for `stdin` */
int SagaEngine_InputFromFileName(SagaEngine *engine, const char *NomIn);

/** Sets `text` encoded in `encoding` to be transcribed by `engine`.
 * 
 * @param[in] text The text to transcribe
 * @param[in] encoding `text` encoding. Has to be ISO-8859-15 for now.
 * 
 */
int SagaEngine_InputFromText(SagaEngine *engine, const char *text, const char *encoding);

/** Opens Files for output
 *  * @param[in] NomOut The file name, or "-" for stdout or NULL for no output.
 */
int SagaEngine_OpenOutputFiles(SagaEngine *engine, const char *NomOut);
/** Loads the dictionaries and builds the characters list */
int SagaEngine_LoadData(SagaEngine *engine);
/** Reads / Points to the next piece of text for transcription */
int SagaEngine_ReadText(SagaEngine *engine);
/** Transcribes the text */
int SagaEngine_Transcribe(SagaEngine *engine);
/** Writes the transcription output to the output files */
int SagaEngine_WriteOutputFiles(SagaEngine *engine);
/** Writes the words that have invalid characters */
int SagaEngine_WriteErrorWords(SagaEngine *engine);
/** Closes the input file, if given */
int SagaEngine_CloseInputFile(SagaEngine *engine);
/** Closes all the output files, if given */
int SagaEngine_CloseOutputFiles(SagaEngine *engine);
/** Closes the error file, if given */
int SagaEngine_CloseErrorFile(SagaEngine *engine);
/** Clears the engine for another transcription */
int SagaEngine_Refresh(SagaEngine *engine);
/** Clears the engine to load other dictionaries */
int SagaEngine_Clear(SagaEngine *engine);


#ifdef __cplusplus
}
#endif

#endif /* SAGA_H */
