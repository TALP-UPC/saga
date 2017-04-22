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

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<limits.h>
#include    <stdarg.h>
#include	<values.h>
#include	"Util.h"
#include	"LisUdf.h"
#include	"SagaInternal.h"
#include "Saga.h"

static int SagaEngine_WriteOutputStream(SagaEngine *engine);

static int SagaEngine_FreeDiccFileNames(SagaEngine *engine)
{
  if (engine->FreeDiccNames > 0)
  {
    if (engine->FicDicExc != NULL) free(engine->FicDicExc);
    engine->FicDicExc = NULL;
    if (engine->FicTrnFon != NULL) free(engine->FicTrnFon);
    engine->FicTrnFon = NULL;
    if (engine->FicTrnPal != NULL) free(engine->FicTrnPal);
    engine->FicTrnPal = NULL;
    if (engine->FicDicSust != NULL) free(engine->FicDicSust);
    engine->FicDicSust = NULL;
    if (engine->FicDicGrp != NULL) free(engine->FicDicGrp);
    engine->FicDicGrp = NULL;
    if (engine->FicNovFon != NULL) free(engine->FicNovFon);
    engine->FicNovFon = NULL;
    if (engine->FicNovVoc != NULL) free(engine->FicNovVoc);
    engine->FicNovVoc = NULL;
    if (engine->FicNovCons != NULL) free(engine->FicNovCons);
    engine->FicNovCons = NULL;
  }
    return 0;
}


int SagaEngine_Initialize(SagaEngine *engine)
{
    /*
     * Valores por defecto.
     */
    engine->FicDicExc = NULL;
    engine->FicTrnFon = NULL;
    engine->FicTrnPal = NULL;
    engine->FicDicSust = NULL;
    engine->FicDicGrp = NULL;
    engine->FicNovFon = NULL;
    engine->FicNovVoc = NULL;
    engine->FicNovCons = NULL;
    engine->FreeDiccNames = 0;

    engine->StrIniPal = ".-";
    engine->StrFinPal = "+.";
    engine->TrnPalAis = 0;
    engine->TrnLinAis = 0;

    engine->DicExc = NULL;
    engine->DicTrnFon = NULL;
    engine->DicTrnPal = NULL;
    engine->DicSust = NULL;
    engine->DicGrp = NULL;

    engine->LisNovVoc = NULL;
    engine->LisNovCons = NULL;
    engine->LisNovFon = NULL;
    engine->Letras = NULL;
    engine->Fonemas = NULL;
    engine->ConsTxt = NULL;
    engine->Vocales = NULL;

    engine->SalFon = 0;
    engine->SalFnm = 0;
    engine->SalFnmPal = 0;
    engine->SalSem = 0;
    engine->SalSefo = 0;
    engine->ConSil = 0;
    engine->ClaveModif = 0;

    engine->TxtIn = NULL;
    engine->TxtInOffset = 0;

    engine->close_in = 0;
    engine->FicInName = NULL;
    engine->FpIn = NULL;
    engine->in_encoding = NULL;

    engine->TxtOrt = NULL;

    engine->PalExt = NULL;
    engine->NumPalExt = 0;

    engine->TrnFon = NULL;
    engine->TrnFnm = NULL;
    engine->TrnFnmPal = NULL;
    engine->TrnSem = NULL;
    engine->TrnSefo = NULL;

    engine->FpFon = NULL;
    engine->FpFnm = NULL;
    engine->FpFnmPal = NULL;
    engine->FpSem = NULL;
    engine->FpSefo = NULL;

    engine->close_FpFon = 0;
    engine->close_FpFnm = 0;
    engine->close_FpFnmPal = 0;
    engine->close_FpSem = 0;
    engine->close_FpSefo = 0;

    SagaEngine_Outputs_Initialize(&engine->StreamOutputs);

    engine->FpErr = stderr;
    engine->close_err = 0;
    return 0;
}

/* Clean engine to make a new transcription */
int SagaEngine_Refresh(SagaEngine *engine)
{
    if (engine->TxtOrt != NULL)
    {
        free(engine->TxtOrt);
        engine->TxtOrt = NULL;
    }
    if (engine->TrnFon != NULL)
    {
        free(engine->TrnFon);
        engine->TrnFon = NULL;
    }
    if (engine->TrnFnm != NULL)
    {
        free(engine->TrnFnm);
        engine->TrnFnm = NULL;
    }
    if (engine->TrnFnmPal != NULL)
    {
        free(engine->TrnFnmPal);
        engine->TrnFnmPal = NULL;
    }
    if (engine->TrnSem != NULL)
    {
        free(engine->TrnSem);
        engine->TrnSem = NULL;
    }
    if (engine->TrnSefo != NULL)
    {
        free(engine->TrnSefo);
        engine->TrnSefo = NULL;
    }

    if (engine->PalExt != NULL)
    {
        LiberaMatStr(engine->PalExt);
        engine->PalExt = NULL;
        engine->NumPalExt = 0;
    }
    return 0;
}

int SagaEngine_OpenErrorFile(SagaEngine *engine, const char *NomErr)
{
    if (NomErr == NULL)
    {
        engine->close_err = 0;
        engine->FpErr = stderr;
    }
    else if (strcmp(NomErr, "-") == 0)
    {
        engine->close_err = 0;
        engine->FpErr = stderr;
    }
    else
    {
        engine->close_err = 1;
        engine->FpErr = fopen(NomErr, "wt");
        if (engine->FpErr == NULL)
        {
            SagaEngine_ErrMsg(engine, "Error al abrir %s\n", NomErr);
            return -1;
        }
    }
    return 0;
}

int SagaEngine_ReadText(SagaEngine *engine)
{
    SagaEngine_Refresh(engine);
    if (engine->FpIn != NULL)
    {
        return CargTxtOrt(engine->FpIn, engine->TrnLinAis, engine->in_encoding, &engine->TxtOrt);
    }
    else if (engine->TxtIn != NULL)
    {
        return CargTxtOrtChar(engine->TxtIn, &engine->TxtInOffset,
                                                   engine->TrnLinAis, &engine->TxtOrt);
    }
    else
    {
        return -1;
    }
}

int SagaEngine_CloseOutputFiles(SagaEngine *engine)
{
    if (engine->close_FpFon)
        fclose(engine->FpFon);
    if (engine->close_FpFnm)
        fclose(engine->FpFnm);
    if (engine->close_FpFnmPal)
        fclose(engine->FpFnmPal);
    if (engine->close_FpSem)
        fclose(engine->FpSem);
    if (engine->close_FpSefo)
        fclose(engine->FpSefo);
    return 0;
}

int SagaEngine_CloseErrorFile(SagaEngine *engine)
{
    if (engine->close_err)
        fclose(engine->FpErr);
    return 0;
}

/* Clean engine */
int SagaEngine_Clear(SagaEngine *engine)
{
    BorraDicExc(engine->DicExc);
    BorraDicExc(engine->DicTrnFon);
    BorraDicExc(engine->DicTrnPal);
    BorraDicExc(engine->DicSust);
    BorraDicExc(engine->DicGrp);

    LiberaMatStr(engine->LisNovVoc);
    LiberaMatStr(engine->LisNovCons);
    LiberaMatStr(engine->LisNovFon);
    LiberaMatStr(engine->Letras);
    LiberaMatStr(engine->ConsTxt);
    LiberaMatStr(engine->Fonemas);
    LiberaMatStr(engine->Vocales);
    SagaEngine_CloseInput(engine);
    SagaEngine_CloseErrorFile(engine);
    SagaEngine_CloseOutputFiles(engine);
    SagaEngine_ClearOutputs(engine);
    SagaEngine_FreeDiccFileNames(engine);
    SagaEngine_Initialize(engine);
    return 0;
}

int SagaEngine_WriteErrorWords(SagaEngine *engine)
{
    /*
     * Escribimos las palabras extranhas encontradas.
     */
    if (EscrPalExt(engine->PalExt) < 0)
    {
        SagaEngine_ErrMsg(engine, "Error al escribir las palabras extranhas\n");
        return -1;
    }
    return 0;
}

static int SagaEngine_LoadDictionaries(SagaEngine *engine)
{
    BorraDicExc(engine->DicExc);
    BorraDicExc(engine->DicTrnFon);
    BorraDicExc(engine->DicTrnPal);
    BorraDicExc(engine->DicSust);
    BorraDicExc(engine->DicGrp);
    /*
     * Cargamos los diccionarios de excepciones y substituciones.
     */

    if (engine->FicDicExc != NULL
        && (engine->DicExc = CargDicExc(engine->FicDicExc)) == NULL)
    {
        fprintf(engine->FpErr,
                "Error al cargar el diccionario de excepciones\n");
        return -1;
    }
    if (engine->FicTrnFon != NULL
        && (engine->DicTrnFon = CargDicExc(engine->FicTrnFon)) == NULL)
    {
        fprintf(engine->FpErr,
                "Error al cargar el diccionario de transcripcion de fonemas\n");
        return -1;
    }
    if (engine->FicTrnPal != NULL
        && (engine->DicTrnPal = CargDicExc(engine->FicTrnPal)) == NULL)
    {
        fprintf(engine->FpErr,
                "Error al cargar el diccionario de transcripcion de palabras\n");
        return -1;
    }
    if (engine->FicDicSust != NULL
        && (engine->DicSust = CargDicExc(engine->FicDicSust)) == NULL)
    {
        fprintf(engine->FpErr,
                "Error al cargar el diccionario de substituciones\n");
        return -1;
    }
    if (engine->FicDicGrp != NULL
        && (engine->DicGrp = CargDicExc(engine->FicDicGrp)) == NULL)
    {
        fprintf(engine->FpErr,
                "Error al cargar el diccionario de substitucion de grupos\n");
        return -1;
    }
    return 0;
}

static int SagaEngine_LoadCharacters(SagaEngine *engine)
{
    size_t i, NumLet, NumVoc, NumCons, NumFon;
    size_t bytes_written;
    char Fonema[1024];

    /*
     * Creamos las listas de letras, consonantes y fonemas.
     */
    NumLet = 0;
    for (i = 0; _Letras[i] != NULL; i++)
    {
        if (MeteLisUdf(_Letras[i], &NumLet, &engine->Letras) == LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr, "Error al crear la lista de letras\n");
            return -1;
        }
    }

    NumCons = 0;
    for (i = 0; _ConsTxt[i] != NULL; i++)
    {
        if (MeteLisUdf(_ConsTxt[i], &NumCons, &engine->ConsTxt) ==
            LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr,
                    "Error al crear la lista de consonantes\n");
            return -1;
        }
    }

    NumVoc = 0;
    for (i = 0; _Vocales[i] != NULL; i++)
    {
        if (MeteLisUdf(_Vocales[i], &NumVoc, &engine->Vocales) ==
            LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr, "Error al crear la lista de vocales\n");
            return -1;
        }
    }

    NumFon = 0;
    for (i = 0; _Fonemas[i] != NULL; i++)
    {
        if (MeteLisUdf(_Fonemas[i], &NumFon, &engine->Fonemas) ==
            LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
            return -1;
        }
    }
    for (i = 0; FonCns[i] != NULL; i++)
    {
        if (MeteLisUdf(FonCns[i], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
            return -1;
        }
    }
    for (i = 0; FonVoc[i] != NULL; i++)
    {
        if (MeteLisUdf(FonVoc[i], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
            return -1;
        }
    }
    for (i = 0; FonSem[i] != NULL; i++)
    {
        if (MeteLisUdf(FonSem[i], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
            return -1;
        }
    }

    if (engine->ClaveModif & VOCAL_NASAL)
    {
        for (i = 0; FonVoc[i] != NULL; i++)
        {
            bytes_written = snprintf(Fonema, 1024, "%s~", FonVoc[i]);
            if (bytes_written >= 1024)
            {
                fprintf(engine->FpErr,
                        "Buffer overflow al crear la lista de fonemas\n");
                return -1;
            }
            if (MeteLisUdf(Fonema, &NumFon, &engine->Fonemas) ==
                LIS_UDF_ERROR)
            {
                fprintf(engine->FpErr,
                        "Error al crear la lista de fonemas\n");
                return -1;
            }
        }
    }

    for (i = 0; engine->DicTrnFon && engine->DicTrnFon[i] != NULL; i++)
    {
        if (MeteLisUdf(engine->DicTrnFon[i][0], &NumLet, &engine->Letras) ==
            LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr, "Error al crear la lista de letras\n");
            return -1;
        }
        if (MeteLisUdf(engine->DicTrnFon[i][1], &NumFon, &engine->Fonemas) ==
            LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
            return -1;
        }
    }

    for (i = 0; engine->DicSust && engine->DicSust[i] != NULL; i++)
    {
        if (MeteLisUdf(engine->DicSust[i][1], &NumFon, &engine->Fonemas) ==
            LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
            return -1;
        }
    }

    if (engine->FicNovFon != NULL)
    {
        if (ReadLisUdf(engine->FicNovFon, &engine->LisNovFon) ==
            LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr,
                    "Error al leer la lista de nuevos fonemas %s\n",
                    engine->FicNovFon);
            return -1;
        }

        for (i = 0; engine->LisNovFon[i] != NULL; i++)
        {
            if (MeteLisUdf(engine->LisNovFon[i], &NumFon, &engine->Fonemas) ==
                LIS_UDF_ERROR)
            {
                fprintf(engine->FpErr,
                        "Error al anhadir la lista de fonemas nuevos\n");
                return -1;
            }
        }
    }

    if (engine->FicNovVoc != NULL)
    {
        if (ReadLisUdf(engine->FicNovVoc, &engine->LisNovVoc) ==
            LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr,
                    "Error al leer la lista de nuevas vocales %s\n",
                    engine->FicNovVoc);
            return -1;
        }

        for (i = 0; engine->LisNovVoc[i] != NULL; i++)
        {
            if (MeteLisUdf(engine->LisNovVoc[i], &NumLet, &engine->Letras) ==
                LIS_UDF_ERROR
                || MeteLisUdf(engine->LisNovVoc[i], &NumVoc,
                              &engine->Vocales) == LIS_UDF_ERROR)
            {
                fprintf(engine->FpErr,
                        "Error al anhadir la lista de vocales nuevas\n");
                return -1;
            }
        }
    }

    if (engine->FicNovCons != NULL)
    {
        if (ReadLisUdf(engine->FicNovCons, &engine->LisNovCons) ==
            LIS_UDF_ERROR)
        {
            fprintf(engine->FpErr,
                    "Error al leer la lista de nuevas consonantes %s\n",
                    engine->FicNovCons);
            return -1;
        }

        for (i = 0; engine->LisNovCons[i] != NULL; i++)
        {
            if (MeteLisUdf(engine->LisNovCons[i], &NumLet, &engine->Letras) ==
                LIS_UDF_ERROR
                || MeteLisUdf(engine->LisNovCons[i], &NumCons,
                              &engine->ConsTxt) == LIS_UDF_ERROR)
            {
                fprintf(engine->FpErr,
                        "Error al anhadir la lista de consonantes nuevas\n");
                return -1;
            }
        }
    }
    return 0;
}

int SagaEngine_Prepare(SagaEngine *engine)
{
    int err;
    err = SagaEngine_LoadDictionaries(engine);
    if (err < 0)
        return err;
    err = SagaEngine_LoadCharacters(engine);
    if (err < 0)
        return err;
    return err;
}

int SagaEngine_InputFromText(SagaEngine *engine, const char *text,
                             const char *encoding)
{
    size_t text_size;
    char *text_holder = NULL;
    SagaEngine_CloseInput(engine);
    if (strcmp(encoding, "ISO-8859-15") == 0)
    {
        engine->TxtIn = strdup(text);
        engine->close_in = 1;
    }
    else if (strcmp(encoding, "UTF-8") == 0)
    {
        text_size = strlen(text);
        text_holder = malloc(text_size + 1);
        if (text_holder == NULL)
        {
            fprintf(engine->FpErr,
                    "Error loading text: (memory allocation)\n");
            return -1;
        }
        text_size = utf8_to_latin9(text_holder, text, text_size);
        engine->TxtIn = realloc(text_holder, text_size + 1);
        if (engine->TxtIn == NULL)
        {
            engine->TxtIn = text_holder;
        }
        engine->close_in = 1;
    }
    else
    {
        fprintf(engine->FpErr,
                "Error encoding %s not supported. Use ISO-8859-15 or UTF-8.\n",
                encoding);
        return -1;
    }
    if (engine->TxtIn == NULL)
    {
        fprintf(engine->FpErr, "Error loading text: %s\n", text);
        return -1;
    }
    return 0;
}

static int SagaEngine_TextPreparation(SagaEngine *engine)
{
    /*
     * Si existe el diccionario de excepciones, lo aplicamos.
     */
    if (engine->DicExc != NULL
        && AplDicExc(engine->DicExc, &engine->TxtOrt, engine->Letras) < 0)
    {
        fprintf(engine->FpErr,
                "Error al aplicar el diccionario de excepciones\n");
        return -1;
    }

    if ((engine->TxtOrt = ArreglaTxt(engine->TxtOrt)) == NULL)
    {
        fprintf(engine->FpErr, "Error al arreglar el texto de entrada\n");
        return -1;
    }

    if (engine->DicExc != NULL
        && AplDicExc(engine->DicExc, &engine->TxtOrt, engine->Letras) < 0)
    {
        fprintf(engine->FpErr,
                "Error al aplicar el diccionario de excepciones\n");
        return -1;
    }

    /*
     * Determinamos si hay alguna palabra extranha.
     */
    if ((engine->PalExt =
         CogePalExt(engine->TxtOrt, engine->PalExt, engine->DicExc,
                    engine->DicTrnPal, engine->ConsTxt, engine->Vocales,
                    engine->Letras)) == NULL)
    {
        fprintf(engine->FpErr, "Error al localizar palabras extranhas\n");
        return -1;
    }
    return 0;
}

int SagaEngine_Transcribe(SagaEngine *engine)
{
    char *SilOrt = NULL, *SilAcc = NULL;
    if (SagaEngine_TextPreparation(engine) < 0)
    {
        fprintf(engine->FpErr, "Error en SagaEngine_TextPreparation\n");
        return -1;
    }
    /*
     * Silabificamos el texto ortografico.
     */
    if ((SilOrt =
         SilaTxtOrt(engine->TxtOrt, engine->DicTrnPal, engine)) == NULL)
    {
        fprintf(engine->FpErr, "Error al silabificar %s\n", engine->TxtOrt);
        return -1;
    }

    /*
     * Colocamos tildes en la vocales acentuadas.
     */
    if ((SilAcc = AcenSilOrt(SilOrt, engine->DicTrnPal, engine)) == NULL)
    {
        fprintf(engine->FpErr, "Error al acentuar %s\n", SilOrt);
        free(SilOrt);
        return -1;
    }

    /*
     * Realizamos la transcripcion fonetica.
     */
    if ((engine->TrnFon = TrnSilAcc(engine, SilAcc)) == NULL)
    {
        fprintf(engine->FpErr, "Error al transcribir %s\n", SilAcc);
        free(SilOrt);
        free(SilAcc);
        return -1;
    }

    /*
     * Si existe el diccionario de substituciones, lo aplicamos.
     */
    if (engine->DicSust != NULL
        && AplDicSust(engine->DicSust, &engine->TrnFon, engine->Fonemas) < 0)
    {
        fprintf(engine->FpErr,
                "Error al aplicar el diccionario de substituciones\n");
        free(SilOrt);
        free(SilAcc);
        return -2;
    }

    /*
     * Si existe el diccionario de substitucion de grupos, lo aplicamos.
     */
    if (engine->DicGrp != NULL
        && AplDicGrp(engine->DicGrp, &engine->TrnFon, engine->Fonemas) < 0)
    {
        fprintf(engine->FpErr,
                "Error al aplicar el diccionario de substitucion de grupos\n");
        free(SilOrt);
        free(SilAcc);
        return -2;
    }

    /*
     * Realizamos la transcripcion en fonemas.
     */
    if (engine->SalFnm > 0
        && (engine->TrnFnm =
            TrnFonFnm(engine->TrnFon, engine->ConSil,
                      engine->Fonemas)) == NULL)
    {
        fprintf(engine->FpErr, "Error al transcribir %s\n", engine->TrnFon);
        free(SilOrt);
        free(SilAcc);
        return -1;
    }

    /*
     * Realizamos la transcripcion en fonemas por palabras.
     */
    if (engine->SalFnmPal > 0
        && (engine->TrnFnmPal =
            TrnFonFnmPal(engine->TrnFon, engine->ConSil,
                         engine->Fonemas)) == NULL)
    {
        fprintf(engine->FpErr, "Error al transcribir %s\n", engine->TrnFon);
        free(SilOrt);
        free(SilAcc);
        return -1;
    }

    /*
     * Realizamos la transcripcion en semisilabas.
     */
    if (engine->SalSem > 0
        && (engine->TrnSem =
            TrnFonSem(engine->TrnFon, engine->ConSil,
                      engine->Fonemas)) == NULL)
    {
        fprintf(engine->FpErr, "Error al transcribir %s\n", engine->TrnFon);
        free(SilOrt);
        free(SilAcc);
        return -1;
    }

    /*
     * Realizamos la transcripcion en semifonemas.
     */
    if (engine->SalSefo > 0
        && (engine->TrnSefo =
            TrnFonSefo(engine->TrnFon, engine->ConSil, engine->StrIniPal,
                       engine->StrFinPal, engine->Fonemas)) == NULL)
    {
        fprintf(engine->FpErr, "Error al transcribir %s\n", engine->TrnFon);
        free(SilOrt);
        free(SilAcc);
        return -1;
    }
    free(SilOrt);
    free(SilAcc);
    return 0;
}

int SagaEngine_OpenOutputFiles(SagaEngine *engine, const char *NomOut)
{
    char PathOut[SAGA_PATH_MAX];

    if (NomOut == NULL)
    {
        engine->FpFon = NULL;
        engine->FpFnm = NULL;
        engine->FpFnmPal = NULL;
        engine->FpSem = NULL;
        engine->FpSefo = NULL;
        return 0;
    }
    else if (strcmp(NomOut, "-") == 0)
    {
        engine->FpFon = engine->SalFon ? stdout : NULL;
        engine->close_FpFon = 0;
        engine->FpFnm = engine->SalFnm ? stdout : NULL;
        engine->close_FpFnm = 0;
        engine->FpFnmPal = engine->SalFnmPal ? stdout : NULL;
        engine->close_FpFnmPal = 0;
        engine->FpSem = engine->SalSem ? stdout : NULL;
        engine->close_FpSem = 0;
        engine->FpSefo = engine->SalSefo ? stdout : NULL;
        engine->close_FpSefo = 0;
        return 0;
    }
    /* Prevent buffer overflow */
    if (strlen(NomOut) >= SAGA_PATH_MAX - 5)
    {
        return -1;
    }
    strcpy(PathOut, NomOut);
    strcat(PathOut, ".fon");
    if (engine->SalFon && (engine->FpFon = fopen(PathOut, "wt")) == NULL)
    {
        return -1;
    }
    engine->close_FpFon = 1;

    strcpy(PathOut, NomOut);
    strcat(PathOut, ".fnm");
    if (engine->SalFnm && (engine->FpFnm = fopen(PathOut, "wt")) == NULL)
    {
        return -1;
    }
    engine->close_FpFnm = 0;

    strcpy(PathOut, NomOut);
    strcat(PathOut, ".fnp");
    if (engine->SalFnmPal
        && (engine->FpFnmPal = fopen(PathOut, "wt")) == NULL)
    {
        return -1;
    }
    engine->close_FpFnmPal = 0;

    strcpy(PathOut, NomOut);
    strcat(PathOut, ".sem");
    if (engine->SalSem && (engine->FpSem = fopen(PathOut, "wt")) == NULL)
    {
        return -1;
    }
    engine->close_FpSem = 0;

    strcpy(PathOut, NomOut);
    strcat(PathOut, ".sef");
    if (engine->SalSefo && (engine->FpSefo = fopen(PathOut, "wt")) == NULL)
    {
        return -1;
    }
    engine->close_FpSefo = 0;
    return 0;
}

static int SagaEngine_WriteOutputStream(SagaEngine *engine)
{
    if (engine->SalFon)
    {
        if (safe_strcat(&(engine->StreamOutputs.fon),
                        engine->TrnFon,
                        &(engine->StreamOutputs.fon_size),
                        &(engine->StreamOutputs.fon_filled)) < 0)
        {
            return -1;
        }
    }
    if (engine->SalFnm)
    {
        if (safe_strcat(&(engine->StreamOutputs.fnm),
                        engine->TrnFnm,
                        &(engine->StreamOutputs.fnm_size),
                        &(engine->StreamOutputs.fnm_filled)) < 0)
        {
            return -1;
        }
    }
    if (engine->SalFnmPal)
    {
        if (safe_strcat
            (&(engine->StreamOutputs.fnmpal),
             engine->TrnFnmPal,
             &(engine->StreamOutputs.fnmpal_size),
             &(engine->StreamOutputs.fnmpal_filled)) < 0)
        {
            return -1;
        }
    }
    if (engine->SalSefo)
    {
        if (safe_strcat(&(engine->StreamOutputs.sefo),
                        engine->TrnSefo,
                        &(engine->StreamOutputs.sefo_size),
                        &(engine->StreamOutputs.sefo_filled)) < 0)
        {
            return -1;
        }
    }
    if (engine->SalSem)
    {
        if (safe_strcat(&(engine->StreamOutputs.sem),
                        engine->TrnSem,
                        &(engine->StreamOutputs.sem_size),
                        &(engine->StreamOutputs.sem_filled)) < 0)
        {
            return -1;
        }
    }
    return 0;
}

int SagaEngine_WriteOutputFiles(SagaEngine *engine)
{
    /*
     * Escribimos el texto de salida.
     */
    if (engine->SalFon > 0
        && fprintf(engine->FpFon, "%s", engine->TrnFon) < 0)
    {
        fprintf(engine->FpErr, "Error al escribir el texto transcrito\n");
        return -1;
    }

    if (engine->SalFnm > 0
        && fprintf(engine->FpFnm, "%s", engine->TrnFnm) < 0)
    {
        fprintf(engine->FpErr,
                "Error al escribir el texto transcrito en fonemas\n");
        return -1;
    }

    if (engine->SalFnmPal > 0
        && fprintf(engine->FpFnmPal, "%s", engine->TrnFnmPal) < 0)
    {
        fprintf(engine->FpErr,
                "Error al escribir el texto transcrito en fonemas\n");
        return -1;
    }

    if (engine->SalSem > 0
        && fprintf(engine->FpSem, "%s", engine->TrnSem) < 0)
    {
        fprintf(engine->FpErr,
                "Error al escribir el texto transcrito en semisilabas\n");
        return -1;
    }

    if (engine->SalSefo > 0
        && fprintf(engine->FpSefo, "%s", engine->TrnSefo) < 0)
    {
        fprintf(engine->FpErr,
                "Error al escribir el texto transcrito en semifonemas\n");
        return -1;
    }
    return 0;
}

int SagaEngine_InputFromFileName(SagaEngine *engine, const char *NomIn)
{
    if (NomIn == NULL)
    {
        /* No deberia pasar */
        fprintf(engine->FpErr, "Fichero de entrada NULL\n");
        return -1;
    }
    engine->FicInName = NomIn;

    if (strcmp(engine->FicInName, "-") == 0)
    {
        engine->FpIn = stdin;
        engine->close_in = 0;
    }
    else
    {
        engine->FpIn = fopen(engine->FicInName, "rt");
        if (engine->FpIn == NULL)
        {
            fprintf(engine->FpErr, "Error al abrir %s\n", engine->FicInName);
            return -1;
        }
        engine->close_in = 1;
    }
    return 0;
}

int SagaEngine_CloseInput(SagaEngine *engine)
{
    if (engine->close_in)
    {
        if (engine->FpIn != NULL)
        {
            fclose(engine->FpIn);
            engine->FpIn = NULL;
        }
        if (engine->TxtIn != NULL)
        {
            free(engine->TxtIn);
            engine->TxtIn = NULL;
            engine->TxtInOffset = 0;
        }
    }
    engine->close_in = 0;
    return 0;
}


/***********************************************************************
 * CargTxtOrt - Carga un texto ortografico.
 **********************************************************************/

int CargTxtOrtChar(const char *txtin, intptr_t * TxtInOffset, int TrnLinAis,
                                     char **txtout)
{
    char *Txt;
    size_t input_len;
    if (txtin == NULL)
    {
        fprintf(stderr, "Error al leer el texto de entrada\n");
        *txtout = NULL;
        return -2;
    }
    input_len = 0;
    while (1)
    {
        if (TrnLinAis && txtin[*TxtInOffset + input_len] == '\n')
        {
            input_len++;
            break;
        }
        if (txtin[*TxtInOffset + input_len] == '\0')
        {
            break;
        }
        input_len++;
    }
    Txt = malloc((input_len + 1) * sizeof(char));
    if (Txt == NULL)
    {
        fprintf(stderr, "Error al reservar memoria\n");
        *txtout = NULL;
        return -2;
    }
    memcpy(Txt, txtin + *TxtInOffset, input_len);
    Txt[input_len] = '\0';
    *TxtInOffset += input_len;
    *txtout = Txt;
    if (input_len == 0)
    {
        return -1;
    } else
    {
        return 0;
    }
}

int CargTxtOrt(FILE * fpin, int TrnLinAis, char *encoding, char **txtout)
{
    char *Txt;
    char *txt_converted;
    size_t txt_conv_size;
    int Long, AllocLong;
    int Final;

    AllocLong = 1000;
    if ((Txt = (char *) malloc((AllocLong + 1) * sizeof(char))) == NULL)
    {
        fprintf(stderr, "Error al ubicar memoria para Txt\n");
        *txtout = NULL;
        return -2;
    }

    Final = 0;
    Long = 0;
    while (Final == 0)
    {
        Long++;
        if (Long == AllocLong)
        {
            AllocLong += 1000;
            if ((Txt =
                 (char *) realloc((void *) Txt,
                                  (size_t) (AllocLong + 1) * sizeof(char))) ==
                NULL)
            {
                fprintf(stderr, "Error al reubicar memoria para Txt\n");
               *txtout = NULL;
                return -2;
            }
        }

        if ((Txt[Long - 1] = (char) fgetc(fpin)) == (char) EOF)
        {
            Final++;
        }

        if (TrnLinAis && (Txt[Long - 1] == '\n'))
        {
            Long++;
            Final++;
        }

        if (Long > 2 && Txt[Long - 1] == '\n'
            && IndexChr(&Txt[Long - 2], Silen) >= 0)
        {
            Long++;
            break;
        }
    }

    Txt[Long - 1] = '\0';
    if (Long == 1)
    {
        *txtout = Txt;
        return -1;
    }
    /* Convert encoding */
    if (encoding == NULL || strcmp(encoding, "UTF-8") == 0)
    {
        txt_converted = malloc(Long + 1);
        if (txt_converted == NULL)
        {
            free(Txt);
            *txtout = NULL;
            return -2;
        }
        txt_conv_size = utf8_to_latin9(txt_converted, Txt, Long);
        free(Txt);
        Txt = realloc(txt_converted, txt_conv_size);
        if (Txt == NULL)
        {
            Txt = txt_converted;
        }
    }
    *txtout = Txt;
    return 0;
}

/***********************************************************************
 * ArreglaTxt - Arregla el texto ortografico
 **********************************************************************/

char *ArreglaTxt(char *TxtOrt)
{
    char *Txt;
    size_t Chr;

    if ((Txt =
         (char *) malloc((2 * strlen(TxtOrt) + 1) * sizeof(char))) == NULL)
    {
        fprintf(stderr, "Error al ubicar memoria para Txt\n");
        return NULL;
    }

    *Txt = '\0';
    for (Chr = 0; Chr < strlen(TxtOrt); Chr++)
    {
        switch (TxtOrt[Chr])
        {
        case 'y':
            if ((Chr == 0
                 || strchr(ALL_PUNCT_AND_SPACES, TxtOrt[Chr - 1]) != NULL)
                && (Chr == strlen(TxtOrt) - 1
                    || strchr(ALL_PUNCT_AND_SPACES, TxtOrt[Chr + 1]) != NULL))
            {
                strcat(Txt, "y");
            }
            else if (strchr("aeiou" ALL_ACUTE "'", TxtOrt[Chr + 1]) == NULL)
            {
                strcat(Txt, "i");
            }
            else
            {
                strcat(Txt, "y");
            }
            break;
        case AACUTE:
            strcat(Txt, "'a");
            break;
        case EACUTE:
            strcat(Txt, "'e");
            break;
        case IACUTE:
            strcat(Txt, "'i");
            break;
        case OACUTE:
            strcat(Txt, "'o");
            break;
        case UACUTE:
            strcat(Txt, "'u");
            break;
        case UUMLAUT:
            strcat(Txt, "~u");
            break;
        case NTILDE:
            strcat(Txt, "~n");
            break;
        case CEDILLA:
            strcat(Txt, "~c");
            break;
        default:
            strncat(Txt, TxtOrt + Chr, 1);
            break;
        }
    }

    free((void *) TxtOrt);
    return Txt;
}

static const char *SagaGetDiccDir()
{
  const char *basedir = getenv("SAGA_DICCDIR");
  if (basedir == NULL)
 {
    basedir = SAGA_DICCDIR;
 }
  return basedir;
}

int SagaArgentinaParams(SagaEngine *engine)
{
    const char *basedir = SagaGetDiccDir();
    engine->FicDicExc = Saga_concat(basedir, "/Arg/ArgExc.dicc");
    engine->FicDicSust = Saga_concat(basedir, "/Arg/ArgSust.dicc");
    engine->FicDicGrp = Saga_concat(basedir, "/Arg/ArgDicGrp.dicc");
    engine->FicNovFon = Saga_concat(basedir, "/Arg/ArgNovFon.dicc");
    engine->ClaveModif = SESEO | ESE_ASP_CON;
    engine->FreeDiccNames = 1;
    if (engine->FicDicExc == NULL || engine->FicDicSust == NULL || 
           engine->FicDicGrp == NULL || engine->FicNovFon == NULL)
        {
            SagaEngine_ErrMsg(engine,  "Error allocating dictionary names\n");
            return -1;
        }
    return 0;
}


int SagaCastillaParams(SagaEngine *engine)
{
    const char *basedir = SagaGetDiccDir();
    engine->FicDicExc = Saga_concat(basedir, "/Cas/CasExc.dicc");
    engine->FicTrnFon = Saga_concat(basedir, "/Cas/CasTrnFon.dicc");
    engine->FicTrnPal = Saga_concat(basedir, "/Cas/CasTrnPal.dicc");

    engine->FicDicSust = Saga_concat(basedir, "/Cas/CasDicSust.dicc");
    engine->FicDicGrp = Saga_concat(basedir, "/Cas/CasDicGrp.dicc");
    engine->FicNovCons = Saga_concat(basedir, "/Cas/CasNovCns.dicc");
    engine->FreeDiccNames = 1;
    if (engine->FicDicExc == NULL || engine->FicTrnFon == NULL ||
          engine->FicTrnPal == NULL || engine->FicDicSust == NULL || 
          engine->FicDicGrp == NULL || engine->FicNovCons == NULL)
        {
            SagaEngine_ErrMsg(engine, "Error allocating dictionary names\n");
            return -1;
        }
    return 0;
}

int SagaChileParams(SagaEngine *engine)
{
    const char *basedir = SagaGetDiccDir();
    engine->FicNovFon = Saga_concat(basedir, "/Chl/ChlNovFon.dicc");
    engine->FicDicExc = Saga_concat(basedir, "/Chl/ChlExc.dicc");
    engine->FicDicSust = Saga_concat(basedir, "/Chl/ChlSust.dicc");
    engine->FicDicGrp = Saga_concat(basedir, "/Chl/ChlGrup.dicc");
    engine->ClaveModif = SESEO | ESE_ASP_INC;
    engine->FreeDiccNames = 1;
    if (engine->FicNovFon == NULL || engine->FicDicExc == NULL ||
          engine->FicDicSust == NULL || engine->FicDicGrp == NULL )
        {
            SagaEngine_ErrMsg(engine, "Error allocating dictionary names\n");
            return -1;
        }
    return 0;
}

int SagaColombiaParams(SagaEngine *engine)
{
    const char *basedir = SagaGetDiccDir();
    engine->FicDicExc = Saga_concat(basedir, "/Col/ColExc.dicc");
    engine->FicDicSust = Saga_concat(basedir, "/Col/ColSust.dicc");
    engine->ClaveModif = SESEO | BDG_ANDES;
    engine->FreeDiccNames = 1;
    if (engine->FicDicExc == NULL || engine->FicDicSust == NULL)
        {
            SagaEngine_ErrMsg(engine, "Error allocating dictionary names\n");
            return -1;
        }
    return 0;
}

int SagaMexicoParams(SagaEngine *engine)
{
    const char *basedir = SagaGetDiccDir();
    engine->FicDicExc = Saga_concat(basedir, "/Mex/MexExc.dicc");
    engine->FicDicSust = Saga_concat(basedir, "/Mex/MexSust.dicc");
    engine->ClaveModif = SESEO | EQUIS_KS;
    engine->FreeDiccNames = 1;
    if (engine->FicDicExc == NULL || engine->FicDicSust == NULL)
        {
            SagaEngine_ErrMsg(engine, "Error allocating dictionary names\n");
            return -1;
        }
    return 0;
}

int SagaPeruParams(SagaEngine *engine)
{
    const char *basedir = SagaGetDiccDir();
    engine->FicDicExc = Saga_concat(basedir, "/Per/PerExc.dicc");
    engine->FicDicSust = Saga_concat(basedir, "/Per/PerSust.dicc");
    engine->ClaveModif =
        SESEO | ESE_ASP_CON | NAS_VELAR | ARCHI_IMPL | ELIM_D;
    engine->FreeDiccNames = 1;
    if (engine->FicDicExc == NULL || engine->FicDicSust == NULL)
        {
            SagaEngine_ErrMsg(engine, "Error allocating dictionary names\n");
            return -1;
        }
    return 0;
}

int SagaVenezuelaParams(SagaEngine *engine)
{
    const char *basedir = SagaGetDiccDir();
    engine->FicDicExc = Saga_concat(basedir, "/Ven/VenExc.dicc");
    engine->FicDicSust = Saga_concat(basedir, "/Ven/VenSust.dicc");
    engine->ClaveModif = SESEO | ESE_ASP_INC | NAS_VELAR | EQUIS_KS;
    engine->FreeDiccNames = 1;
    if (engine->FicDicExc == NULL || engine->FicDicSust == NULL)
        {
            SagaEngine_ErrMsg(engine, "Error allocating dictionary names\n");
            return -1;
        }
    return 0;
}

int SagaEngine_SetParamsFromVariant(SagaEngine *engine, const char *variant)
{
    if (strcmp(variant, "argentina") == 0)
    {
        if (SagaArgentinaParams(engine) < 0)
        {
            return -1;
        }
    }
    else if (strcmp(variant, "castilla") == 0)
    {
        if (SagaCastillaParams(engine) < 0)
        {
            return -1;
        }
    }
    else if (strcmp(variant, "chile") == 0)
    {
        if (SagaChileParams(engine) < 0)
        {
            return -1;
        }
    }
    else if (strcmp(variant, "colombia") == 0)
    {
        if (SagaColombiaParams(engine) < 0)
        {
            return -1;
        }
    }
    else if (strcmp(variant, "mexico") == 0)
    {
        if (SagaMexicoParams(engine) < 0)
        {
            return -1;
        }
    }
    else if (strcmp(variant, "peru") == 0)
    {
        if (SagaPeruParams(engine) < 0)
        {
            return -1;
        }
    }
    else if (strcmp(variant, "venezuela") == 0)
    {
        if (SagaVenezuelaParams(engine) < 0)
        {
            return -1;
        }
    }
    else
    {
        SagaEngine_ErrMsg(engine, "Variante dialectal desconocida: '%s'\n", variant);
        return -1;
    }
    return 0;
}

SagaEngine *SagaEngine_NewFromVariant(const char *variant)
{
    SagaEngine *engine = malloc(sizeof(SagaEngine));
    if (engine == NULL)
        return NULL;
    if (SagaEngine_SetVariant(engine, variant) < 0)
    {
        free(engine);
        return NULL;
    }
    return engine;
}

int SagaEngine_SetVariant(SagaEngine *engine, const char *variant)
{
    if (engine == NULL)
        return -1;
    if (SagaEngine_Initialize(engine) < 0)
    {
        return -1;
    }
    if (SagaEngine_SetParamsFromVariant(engine, variant) < 0)
    {
        SagaEngine_Clear(engine);
        return -1;
    }
    if (SagaEngine_Prepare(engine) < 0)
    {
        SagaEngine_Clear(engine);
        return -1;
    }
    return 0;
}

int SagaEngine_ClearOutputs(SagaEngine *engine)
{
    SagaEngine_ClearFonOutput(engine);
    SagaEngine_ClearFnmOutput(engine);
    SagaEngine_ClearFnmPalOutput(engine);
    SagaEngine_ClearSefoOutput(engine);
    SagaEngine_ClearSemOutput(engine);
    return 0;
}


int SagaEngine_Outputs_Initialize(SagaEngine_Outputs * outputs)
{
    outputs->fon = NULL;
    outputs->fnm = NULL;
    outputs->fnmpal = NULL;
    outputs->sefo = NULL;
    outputs->sem = NULL;

    outputs->fon_size = 0;
    outputs->fnm_size = 0;
    outputs->fnmpal_size = 0;
    outputs->sefo_size = 0;
    outputs->sem_size = 0;

    outputs->fon_filled = 0;
    outputs->fnm_filled = 0;
    outputs->fnmpal_filled = 0;
    outputs->sefo_filled = 0;
    outputs->sem_filled = 0;
    return 0;
}

int SagaEngine_EnableFonOutput(SagaEngine *engine, int enable)
{
    if (enable == 0)
    {
        SagaEngine_ClearFonOutput(engine);
    }
    engine->SalFon = enable;
    return 0;
}

int SagaEngine_EnableFnmOutput(SagaEngine *engine, int enable)
{
    if (enable == 0)
    {
        SagaEngine_ClearFnmOutput(engine);
    }
    engine->SalFnm = enable;
    return 0;
}

int SagaEngine_EnableFnmPalOutput(SagaEngine *engine, int enable)
{
    if (enable == 0)
    {
        SagaEngine_ClearFnmPalOutput(engine);
    }

    engine->SalFnmPal = enable;
    return 0;
}

int SagaEngine_EnableSefoOutput(SagaEngine *engine, int enable)
{
    if (enable == 0)
    {
        SagaEngine_ClearSefoOutput(engine);
    }
    engine->SalSefo = enable;
    return 0;
}

int SagaEngine_EnableSemOutput(SagaEngine *engine, int enable)
{
    if (enable == 0)
    {
        SagaEngine_ClearSemOutput(engine);
    }

    engine->SalSem = enable;
    return 0;
}

char *SagaEngine_GetFonOutput(SagaEngine *engine, int copy)
{
    if (copy > 0)
        return strdup(engine->StreamOutputs.fon);
    else
        return engine->StreamOutputs.fon;
}

char *SagaEngine_GetFnmOutput(SagaEngine *engine, int copy)
{
    if (copy > 0)
        return strdup(engine->StreamOutputs.fnm);
    else
        return engine->StreamOutputs.fnm;
}

char *SagaEngine_GetFnmPalOutput(SagaEngine *engine, int copy)
{
    if (copy > 0)
        return strdup(engine->StreamOutputs.fnmpal);
    else
        return engine->StreamOutputs.fnmpal;
}

char *SagaEngine_GetSefoOutput(SagaEngine *engine, int copy)
{
    if (copy > 0)
        return strdup(engine->StreamOutputs.sefo);
    else
        return engine->StreamOutputs.sefo;
}

char *SagaEngine_GetSemOutput(SagaEngine *engine, int copy)
{
    if (copy > 0)
        return strdup(engine->StreamOutputs.sem);
    else
        return engine->StreamOutputs.sem;
}

int SagaEngine_ClearFonOutput(SagaEngine *engine)
{
    if (engine->StreamOutputs.fon != NULL)
        free(engine->StreamOutputs.fon);
    engine->StreamOutputs.fon = NULL;
    engine->StreamOutputs.fon_size = 0;
    engine->StreamOutputs.fon_filled = 0;
    return 0;
}

int SagaEngine_ClearFnmOutput(SagaEngine *engine)
{
    if (engine->StreamOutputs.fnm != NULL)
        free(engine->StreamOutputs.fnm);
    engine->StreamOutputs.fnm = NULL;
    engine->StreamOutputs.fnm_size = 0;
    engine->StreamOutputs.fnm_filled = 0;
    return 0;
}

int SagaEngine_ClearFnmPalOutput(SagaEngine *engine)
{
    if (engine->StreamOutputs.fnmpal != NULL)
        free(engine->StreamOutputs.fnmpal);
    engine->StreamOutputs.fnmpal = NULL;
    engine->StreamOutputs.fnmpal_size = 0;
    engine->StreamOutputs.fnmpal_filled = 0;
    return 0;
}

int SagaEngine_ClearSefoOutput(SagaEngine *engine)
{
    if (engine->StreamOutputs.sefo != NULL)
        free(engine->StreamOutputs.sefo);
    engine->StreamOutputs.sefo = NULL;
    engine->StreamOutputs.sefo_size = 0;
    engine->StreamOutputs.sefo_filled = 0;
    return 0;
}

int SagaEngine_ClearSemOutput(SagaEngine *engine)
{
    if (engine->StreamOutputs.sem != NULL)
        free(engine->StreamOutputs.sem);
    engine->StreamOutputs.sem = NULL;
    engine->StreamOutputs.sem_size = 0;
    engine->StreamOutputs.sem_filled = 0;
    return 0;
}


int SagaEngine_TranscribeText(SagaEngine *engine, const char *text,
                              const char *encoding)
{
    int read_status = 0;
    SagaEngine_Refresh(engine);
    SagaEngine_InputFromText(engine, text, encoding);
    while (1)
    {
        read_status = SagaEngine_ReadText(engine);
        if (read_status < -1)
        {
            SagaEngine_ErrMsg(engine,  "Error in SagaEngine_ReadText\n");
            return -1;
        }
        if (SagaEngine_Transcribe(engine) < 0)
        {
            SagaEngine_Refresh(engine);
            SagaEngine_CloseInput(engine);
            SagaEngine_ErrMsg(engine, "Error in SagaEngine_Transcribe\n");
            return -1;
        }
        if (SagaEngine_WriteOutputStream(engine) < 0)
        {
            SagaEngine_Refresh(engine);
            SagaEngine_CloseInput(engine);
            SagaEngine_ErrMsg(engine, "Error in SagaEngine_WriteOutputStream\n");
            return -1;
        }
        if (read_status == -1)
        {
            /* We have finished reading */
            break;
        }
    }
    SagaEngine_CloseInput(engine);
    return 0;
}

int SagaEngine_Opt_Seseo(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= SESEO;
    else
        engine->ClaveModif &= ~SESEO;
    return 0;
}

int SagaEngine_Opt_X_KS(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= EQUIS_KS;
    else
        engine->ClaveModif &= ~EQUIS_KS;
    return 0;
}

int SagaEngine_Opt_SC_KS(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= SC_KS;
    else
        engine->ClaveModif &= ~SC_KS;
    return 0;
}

int SagaEngine_Opt_SAspInc(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= ESE_ASP_INC;
    else
        engine->ClaveModif &= ~ESE_ASP_INC;
    return 0;
}

int SagaEngine_Opt_SAspCond(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= ESE_ASP_CON;
    else
        engine->ClaveModif &= ~ESE_ASP_CON;
    return 0;
}

int SagaEngine_Opt_BDG_Andes(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= BDG_ANDES;
    else
        engine->ClaveModif &= ~BDG_ANDES;
    return 0;
}

int SagaEngine_Opt_ElimB(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= ELIM_B;
    else
        engine->ClaveModif &= ~ELIM_B;
    return 0;
}

int SagaEngine_Opt_ElimD(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= ELIM_D;
    else
        engine->ClaveModif &= ~ELIM_D;
    return 0;
}

int SagaEngine_Opt_ElimG(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= ELIM_G;
    else
        engine->ClaveModif &= ~ELIM_G;
    return 0;
}

int SagaEngine_Opt_NVelar(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= ENE_VELAR;
    else
        engine->ClaveModif &= ~ENE_VELAR;
    return 0;
}

int SagaEngine_Opt_NasalVelar(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= NAS_VELAR;
    else
        engine->ClaveModif &= ~NAS_VELAR;
    return 0;
}

int SagaEngine_Opt_ArchImpl(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= ARCHI_IMPL;
    else
        engine->ClaveModif &= ~ARCHI_IMPL;
    return 0;
}

int SagaEngine_Opt_YVocal(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= Y_VOCAL;
    else
        engine->ClaveModif &= ~Y_VOCAL;
    return 0;
}

int SagaEngine_Opt_RImpl(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= ERRE_IMPL;
    else
        engine->ClaveModif &= ~ERRE_IMPL;
    return 0;
}

int SagaEngine_Opt_GrupoSil(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= GRUPO_SIL;
    else
        engine->ClaveModif &= ~GRUPO_SIL;
    return 0;
}

int SagaEngine_Opt_MarcaImpl(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= MARCA_IMPL;
    else
        engine->ClaveModif &= ~MARCA_IMPL;
    return 0;
}

int SagaEngine_Opt_VocalNasal(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= VOCAL_NASAL;
    else
        engine->ClaveModif &= ~VOCAL_NASAL;
    return 0;
}

int SagaEngine_Opt_VocalPTON(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= VOCAL_PTON;
    else
        engine->ClaveModif &= ~VOCAL_PTON;
    return 0;
}

int SagaEngine_Opt_OclusExpl(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= OCLUS_EXPL;
    else
        engine->ClaveModif &= ~OCLUS_EXPL;
    return 0;
}

int SagaEngine_Opt_IniFinPal(SagaEngine *engine, int enable)
{
    if (enable > 0)
        engine->ClaveModif |= INI_FIN_PAL;
    else
        engine->ClaveModif &= ~INI_FIN_PAL;
    return 0;
}

 
void SagaEngine_ErrMsg(SagaEngine *engine, const char* format, ... ) {
    if (engine == NULL || engine->FpErr == NULL)
    {
        return;
    }
    va_list args;
    va_start( args, format );
    vfprintf(engine->FpErr, format, args );
    va_end( args );
    return;
}
