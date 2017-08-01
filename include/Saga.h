/* coding: utf-8 */
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

#if defined _WIN32 || defined __CYGWIN__
  #ifdef SAGA_BUILDING_SHARED
    #ifdef __GNUC__
      #define SAGA_DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define SAGA_DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #elif defined(SAGA_BUILDING_STATIC)
     #define SAGA_DLL_PUBLIC
  #else
    #ifdef __GNUC__
      #define SAGA_DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define SAGA_DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define SAGA_DLL_LOCAL
#else
  #define SAGA_DLL_PUBLIC __attribute__ ((visibility ("default")))
  #define SAGA_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

    typedef struct SagaEngine_Outputs_struct_ {
        char *fon;
        char *fnm;
        char *fnmpal;
        char *sefo;
        char *sem;

        size_t fon_size;
        size_t fnm_size;
        size_t fnmpal_size;
        size_t sefo_size;
        size_t sem_size;

        size_t fon_filled;
        size_t fnm_filled;
        size_t fnmpal_filled;
        size_t sefo_filled;
        size_t sem_filled;
    } SagaEngine_Outputs;

    typedef struct SagaEngine_struct_ {
        /* Argumentos de entrada (linea de comandos) */
        int TrnPalAis;          /* Palabras aisladas? */
        int TrnLinAis;          /* Lineas aisladas? */
        int ConSil;             /* Conservar los silencios */
        const char *StrIniPal;  /* Marca inicio palabra */
        const char *StrFinPal;  /* Marca fin de palabra */

        char *FicDicExc;  /* Nombre fichero Diccionario excepciones */
        char *FicTrnFon;  /* Nombre fichero Diccionario transcripcion de grafemas */
        char *FicTrnPal;  /* Nombre fichero Diccionario transcripcion de palabras */
        char *FicDicSust; /* Nombre fichero Diccionario de substitucion de fonemas */
        char *FicDicGrp;  /* Nombre fichero Diccionario de substitucion de grupos foneticos */
        char *FicNovVoc;  /* Nombre fichero Lista de grafemas vocalicos introducidos */
        char *FicNovCons; /* Nombre fichero Lista de grafemas consonanticos introducidos */
        char *FicNovFon;  /* Nombre fichero Lista de fonemas introducidos */
        int FreeDiccNames;          /* Hacer free de los nombre fichero diccionarios */

        int32_t ClaveModif;        /* Incluye las opciones -M de la linea de comandos */

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
        int SalFon;             /* Transcripcion fonetica */
        int SalFnm;             /* Transcripcion en fonemas */
        int SalFnmPal;          /* En fonemas por palabras */
        int SalSefo;            /* En semifonemas */
        int SalSem;             /* En semisilabas */

        /* Entrada */
        char *TxtIn;            /* Si la entrada es texto */
        intptr_t TxtInOffset;   /* Si la entrada es texto, param interno */
        const char *FicInName;  /* Si la entrada es de un fichero */
        FILE *FpIn;             /* Si la entrada es de un fichero */
        int close_in;           /* Si FpIn debe cerrarse */
        char *in_encoding;      /* Codificacion FpIn */

        /* Lista de palabras extranhas en TxtOrt. Usado para reportar errores */
        char **PalExt;          /* Lista de palabras extranhas */
        size_t NumPalExt;       /* Número de palabras extranhas */

        char *TxtOrt;           /* Texto de entrada a transcribir */

        /* Salida */
        char *TrnFon;           /* Transcripcion fonetica */
        char *TrnFnm;           /* Transcripcion en fonemas */
        char *TrnFnmPal;        /* En fonemas por palabras */
        char *TrnSem;           /* En semisilabas */
        char *TrnSefo;          /* En semifonemas */

        /* Ficheros de Salida */
        FILE *FpFon;
        FILE *FpFnm;
        FILE *FpFnmPal;
        FILE *FpSem;
        FILE *FpSefo;


        int close_FpFon;
        int close_FpFnm;
        int close_FpFnmPal;
        int close_FpSem;
        int close_FpSefo;

        /* Streams de salida */
        SagaEngine_Outputs StreamOutputs;

        /* Error stream */
        FILE *FpErr;
        int close_err;

    } SagaEngine;

/** Initializes engine. Has to be called after the engine is allocated
 * 
 * @param[in,out] engine The engine to initialize
 *  */
SAGA_DLL_PUBLIC    int SagaEngine_Initialize(SagaEngine *engine);

/** Opens `NomErr` as the file for error output. 
 * 
 * @param[in] NomErr The file name, or "-" for stderr or NULL for no error output. */
SAGA_DLL_PUBLIC    int SagaEngine_OpenErrorFile(SagaEngine *engine, const char *NomErr);

/** Opens `NomIn` to transcribe its contents. Use "-" for `stdin` */
SAGA_DLL_PUBLIC    int SagaEngine_InputFromFileName(SagaEngine *engine, const char *NomIn);

/** Sets `text` encoded in `encoding` to be transcribed by `engine`.
 * 
 * @param[in] text The text to transcribe
 * @param[in] encoding `text` encoding. Has to be "ISO-8859-15"  or "UTF-8".
 * 
 */
SAGA_DLL_PUBLIC    int SagaEngine_InputFromText(SagaEngine *engine, const char *text,
                                 const char *encoding);

/** Opens Files for output
 *  * @param[in] NomOut The file name, or "-" for stdout or NULL for no output.
 */
SAGA_DLL_PUBLIC    int SagaEngine_OpenOutputFiles(SagaEngine *engine, const char *NomOut);
/** Loads the dictionaries and builds the characters list */
SAGA_DLL_PUBLIC    int SagaEngine_Prepare(SagaEngine *engine);
/** Reads / Points to the next piece of text for transcription */
SAGA_DLL_PUBLIC    int SagaEngine_ReadText(SagaEngine *engine);
/** Transcribes the text */
SAGA_DLL_PUBLIC    int SagaEngine_Transcribe(SagaEngine *engine);
/** Writes the transcription output to the output files */
SAGA_DLL_PUBLIC    int SagaEngine_WriteOutputFiles(SagaEngine *engine);
/** Writes the words that have invalid characters */
SAGA_DLL_PUBLIC    int SagaEngine_WriteErrorWords(SagaEngine *engine);
/** Closes the input (FILE handler or char*) */
SAGA_DLL_PUBLIC    int SagaEngine_CloseInput(SagaEngine *engine);
/** Closes all the output files, if given */
SAGA_DLL_PUBLIC    int SagaEngine_CloseOutputFiles(SagaEngine *engine);
/** Closes the error file, if given */
SAGA_DLL_PUBLIC    int SagaEngine_CloseErrorFile(SagaEngine *engine);
/** Clears the engine for another transcription */
SAGA_DLL_PUBLIC    int SagaEngine_Refresh(SagaEngine *engine);
/** Clears the engine to load other dictionaries */
SAGA_DLL_PUBLIC    int SagaEngine_Clear(SagaEngine *engine);

SAGA_DLL_PUBLIC    int SagaArgentinaParams(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaCastillaParams(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaChileParams(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaColombiaParams(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaMexicoParams(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaPeruParams(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaVenezuelaParams(SagaEngine *engine);

SAGA_DLL_PUBLIC    int SagaEngine_SetParamsFromVariant(SagaEngine *engine,
                                        const char *variant);
SAGA_DLL_PUBLIC    int SagaEngine_SetVariant(SagaEngine *engine, const char *variant);
SAGA_DLL_PUBLIC    SagaEngine *SagaEngine_NewFromVariant(const char *variant);
SAGA_DLL_PUBLIC    int SagaEngine_TranscribeText(SagaEngine *engine, const char *text,
                                  const char *encoding);


SAGA_DLL_PUBLIC    int SagaEngine_EnableFonOutput(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_EnableFnmOutput(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_EnableFnmPalOutput(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_EnableSefoOutput(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_EnableSemOutput(SagaEngine *engine, int enable);

SAGA_DLL_PUBLIC    char *SagaEngine_GetFonOutput(SagaEngine *engine, int copy);
SAGA_DLL_PUBLIC    char *SagaEngine_GetFnmOutput(SagaEngine *engine, int copy);
SAGA_DLL_PUBLIC    char *SagaEngine_GetFnmPalOutput(SagaEngine *engine, int copy);
SAGA_DLL_PUBLIC    char *SagaEngine_GetSefoOutput(SagaEngine *engine, int copy);
SAGA_DLL_PUBLIC    char *SagaEngine_GetSemOutput(SagaEngine *engine, int copy);

SAGA_DLL_PUBLIC    int SagaEngine_ClearOutputs(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaEngine_ClearFonOutput(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaEngine_ClearFnmOutput(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaEngine_ClearFnmPalOutput(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaEngine_ClearSefoOutput(SagaEngine *engine);
SAGA_DLL_PUBLIC    int SagaEngine_ClearSemOutput(SagaEngine *engine);

SAGA_DLL_PUBLIC    int SagaEngine_Opt_Seseo(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_X_KS(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_SC_KS(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_SAspInc(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_SAspCond(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_BDG_Andes(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_ElimB(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_ElimD(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_ElimG(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_NVelar(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_NasalVelar(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_ArchImpl(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_YVocal(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_RImpl(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_GrupoSil(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_MarcaImpl(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_OclusExpl(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_VocalNasal(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_VocalPTON(SagaEngine *engine, int enable);
SAGA_DLL_PUBLIC    int SagaEngine_Opt_IniFinPal(SagaEngine *engine, int enable);


#ifdef __cplusplus
}
#endif
#endif                          /* SAGA_H */
