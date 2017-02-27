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
#include	<values.h>
#include	"Util.h"
#include	"LisUdf.h"
#include	"SagaInternal.h"
#include "Saga.h"

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

  engine->TxtIn =NULL;
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


  engine->FpErr = stderr;
  engine->close_err = 0;
	return 0;
}

/* Clean engine to make a new transcription */
int SagaEngine_Refresh(SagaEngine *engine)
{
	if (engine->TxtOrt != NULL) {
	  free(engine->TxtOrt);
    engine->TxtOrt = NULL;
	}
	if (engine->TrnFon != NULL) {
		free(engine->TrnFon);
		engine->TrnFon = NULL;
	}
	if (engine->TrnFnm != NULL) {
		free(engine->TrnFnm);
		engine->TrnFnm = NULL;
	}
	if (engine->TrnFnmPal != NULL) {
		free(engine->TrnFnmPal);
    engine->TrnFnmPal = NULL;
	}
	if (engine->TrnSem != NULL) {
		free(engine->TrnSem);
    engine->TrnSem = NULL;
	}
	if (engine->TrnSefo != NULL) {
		free(engine->TrnSefo);
    engine->TrnSefo = NULL;
	}

	if (engine->PalExt != NULL) {
		LiberaMatStr(engine->PalExt);
		engine->PalExt = NULL;
		engine->NumPalExt = 0;
	}
	return 0;
}

int SagaEngine_OpenErrorFile(SagaEngine *engine, const char *NomErr)
{
	if (NomErr == NULL) {
		engine->close_err = 0;
		engine->FpErr = NULL;
	} else if (strcmp(NomErr, "-") == 0) {
		engine->close_err = 0;
		engine->FpErr = stderr;
	} else {
		engine->close_err = 1;
		engine->FpErr = fopen(NomErr, "wt");
		if (engine->FpErr == NULL) {
			fprintf(stderr, "Error al abrir %s\n", NomErr);
			return -1;
		}
	}
	return 0;
}

int SagaEngine_ReadText(SagaEngine *engine) {
	SagaEngine_Refresh(engine);
	if (engine->FpIn != NULL) {
	    engine->TxtOrt = CargTxtOrt(engine->FpIn, engine->TrnLinAis, engine->in_encoding);
	} else if (engine->TxtIn != NULL) {
	    engine->TxtOrt = CargTxtOrtChar(engine->TxtIn, &engine->TxtInOffset, engine->TrnLinAis);
	}
	if (engine->TxtOrt == NULL) {
		return -1;
	}
	return 0;
}

int SagaEngine_CloseOutputFiles(SagaEngine *engine) {
	if (engine->close_FpFon) fclose(engine->FpFon);
	if (engine->close_FpFnm) fclose(engine->FpFnm);
	if (engine->close_FpFnmPal) fclose(engine->FpFnmPal);
	if (engine->close_FpSem) fclose(engine->FpSem);
	if (engine->close_FpSefo) fclose(engine->FpSefo);
	return 0;
}

int SagaEngine_CloseErrorFile(SagaEngine *engine)
{
	if (engine->close_err) fclose(engine->FpErr);
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
	SagaEngine_Initialize(engine);
	return 0;
}

int SagaEngine_WriteErrorWords(SagaEngine *engine) {
		/*
	 * Escribimos las palabras extranhas encontradas.
	 */
	 if (EscrPalExt(engine->PalExt) < 0) {
  		fprintf(engine->FpErr, "Error al escribir las palabras extranhas\n");
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

	if (engine->FicDicExc != NULL && (engine->DicExc = CargDicExc(engine->FicDicExc)) == NULL) {
		fprintf(engine->FpErr, "Error al cargar el diccionario de excepciones\n");
		return -1;
	}
	if (engine->FicTrnFon != NULL && (engine->DicTrnFon = CargDicExc(engine->FicTrnFon)) == NULL) {
		fprintf(engine->FpErr, "Error al cargar el diccionario de transcripcion de fonemas\n");
		return -1;
	}
	if (engine->FicTrnPal != NULL && (engine->DicTrnPal = CargDicExc(engine->FicTrnPal)) == NULL) {
		fprintf(engine->FpErr, "Error al cargar el diccionario de transcripcion de palabras\n");
		return -1;
	}
	if (engine->FicDicSust != NULL && (engine->DicSust = CargDicExc(engine->FicDicSust)) == NULL) {
		fprintf(engine->FpErr, "Error al cargar el diccionario de substituciones\n");
		return -1;
	}
	if (engine->FicDicGrp != NULL && (engine->DicGrp = CargDicExc(engine->FicDicGrp)) == NULL) {
		fprintf(engine->FpErr, "Error al cargar el diccionario de substitucion de grupos\n");
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
	for (i = 0; _Letras[i] != NULL; i++) {
		if (MeteLisUdf(_Letras[i], &NumLet, &engine->Letras) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de letras\n");
			return -1;
		}
	}

	NumCons = 0;
	for (i = 0; _ConsTxt[i] != NULL; i++) {
		if (MeteLisUdf(_ConsTxt[i], &NumCons, &engine->ConsTxt) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de consonantes\n");
			return -1;
		}
	}

	NumVoc = 0;
	for (i = 0; _Vocales[i] != NULL; i++) {
		if (MeteLisUdf(_Vocales[i], &NumVoc, &engine->Vocales) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de vocales\n");
			return -1;
		}
	}

	NumFon = 0;
	for (i = 0; _Fonemas[i] != NULL; i++) {
		if (MeteLisUdf(_Fonemas[i], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}
	for (i = 0; FonCns[i] != NULL; i++) {
		if (MeteLisUdf(FonCns[i], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}
	for (i = 0; FonVoc[i] != NULL; i++) {
		if (MeteLisUdf(FonVoc[i], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}
	for (i = 0; FonSem[i] != NULL; i++) {
		if (MeteLisUdf(FonSem[i], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}

	if (engine->ClaveModif & VOCAL_NASAL) {
		for (i = 0; FonVoc[i] != NULL; i++) {
			bytes_written = snprintf(Fonema, 1024, "%s~", FonVoc[i]);
			if (bytes_written >= 1024) {
				fprintf(engine->FpErr, "Buffer overflow al crear la lista de fonemas\n");
				return -1;
			}
			if (MeteLisUdf(Fonema, &NumFon, &engine->Fonemas) == LIS_UDF_ERROR) {
				fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
				return -1;
			}
		}
	}

	for (i = 0; engine->DicTrnFon && engine->DicTrnFon[i] != NULL; i++) {
		if (MeteLisUdf(engine->DicTrnFon[i][0], &NumLet, &engine->Letras) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de letras\n");
			return -1;
		}
		if (MeteLisUdf(engine->DicTrnFon[i][1], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}

	for (i = 0; engine->DicSust && engine->DicSust[i] != NULL; i++) {
		if (MeteLisUdf(engine->DicSust[i][1], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}

	if (engine->FicNovFon != NULL) {
		if (ReadLisUdf(engine->FicNovFon, &engine->LisNovFon) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al leer la lista de nuevos fonemas %s\n", engine->FicNovFon);
			return -1;
		}

		for (i = 0; engine->LisNovFon[i] != NULL; i++) {
			if (MeteLisUdf(engine->LisNovFon[i], &NumFon, &engine->Fonemas) == LIS_UDF_ERROR) {
				fprintf(engine->FpErr, "Error al anhadir la lista de fonemas nuevos\n");
				return -1;
			}
		}
	}

	if (engine->FicNovVoc != NULL) {
		if (ReadLisUdf(engine->FicNovVoc, &engine->LisNovVoc) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al leer la lista de nuevas vocales %s\n", engine->FicNovVoc);
			return -1;
		}

		for (i = 0; engine->LisNovVoc[i] != NULL; i++) {
			if (MeteLisUdf(engine->LisNovVoc[i], &NumLet, &engine->Letras) == LIS_UDF_ERROR || \
			    MeteLisUdf(engine->LisNovVoc[i], &NumVoc, &engine->Vocales) == LIS_UDF_ERROR) {
				fprintf(engine->FpErr, "Error al anhadir la lista de vocales nuevas\n");
				return -1;
			}
		}
	}

	if (engine->FicNovCons != NULL) {
		if (ReadLisUdf(engine->FicNovCons, &engine->LisNovCons) == LIS_UDF_ERROR) {
			fprintf(engine->FpErr, "Error al leer la lista de nuevas consonantes %s\n", engine->FicNovCons);
			return -1;
		}

		for (i = 0; engine->LisNovCons[i] != NULL; i++) {
			if (MeteLisUdf(engine->LisNovCons[i], &NumLet, &engine->Letras) == LIS_UDF_ERROR || \
			    MeteLisUdf(engine->LisNovCons[i], &NumCons, &engine->ConsTxt) == LIS_UDF_ERROR) {
				fprintf(engine->FpErr, "Error al anhadir la lista de consonantes nuevas\n");
				return -1;
			}
		}
	}
	return 0;
}

int SagaEngine_LoadData(SagaEngine *engine) {
	int err;
	err = SagaEngine_LoadDictionaries(engine);
	if (err < 0) return err;
	err = SagaEngine_LoadCharacters(engine);
	return err;
}

int SagaEngine_InputFromText(SagaEngine *engine, const char *text, const char *encoding) {
	size_t text_size;
	char *text_holder = NULL;
    SagaEngine_CloseInput(engine);
	if (strcmp(encoding, "ISO-8859-15") == 0) {
		engine->TxtIn = strdup(text);
        engine->close_in = 1;
	} else if (strcmp(encoding, "UTF-8") == 0) {
		text_size = strlen(text);
		text_holder= malloc(text_size+1);
		if (text_holder == NULL) {
		  fprintf(engine->FpErr, "Error loading text: (memory allocation)\n");
		  return -1;
		}
		text_size = utf8_to_latin9(text_holder, text, text_size);
		engine->TxtIn = realloc(text_holder, text_size + 1);
		if (engine->TxtIn == NULL) {
            engine->TxtIn = text_holder;
		}
        engine->close_in = 1;
	} else {
		fprintf(engine->FpErr, "Error encoding %s not supported. Use ISO-8859-15 or UTF-8.\n", encoding);
		return -1;
	}
	if (engine->TxtIn == NULL) {
		fprintf(engine->FpErr, "Error loading text: %s\n", text);
		return -1;
	}
	return 0;
}

static int SagaEngine_TextPreparation(SagaEngine *engine) {
			/*
		 * Si existe el diccionario de excepciones, lo aplicamos.
		 */
		if (engine->DicExc != NULL && AplDicExc(engine->DicExc, &engine->TxtOrt, engine->Letras) < 0) {
			fprintf(engine->FpErr, "Error al aplicar el diccionario de excepciones\n");
			return -1;
		}

		if ((engine->TxtOrt = ArreglaTxt(engine->TxtOrt)) == NULL) {
			fprintf(engine->FpErr, "Error al arreglar el texto de entrada\n");
			return -1;
		}
		
		if (engine->DicExc != NULL && AplDicExc(engine->DicExc, &engine->TxtOrt, engine->Letras) < 0) {
			fprintf(engine->FpErr, "Error al aplicar el diccionario de excepciones\n");
			return -1;
		}

		/*
		 * Determinamos si hay alguna palabra extranha.
		 */
		if ((engine->PalExt = CogePalExt(engine->TxtOrt, engine->PalExt, engine->DicExc, engine->DicTrnPal, engine->ConsTxt, engine->Vocales, engine->Letras)) == NULL) {
			fprintf(engine->FpErr, "Error al localizar palabras extranhas\n");
			return -1;
		}
		return 0;
}

int SagaEngine_Transcribe(SagaEngine *engine) {
		char	*SilOrt = NULL, *SilAcc = NULL;
    SagaEngine_TextPreparation(engine);
		/*
	 * Silabificamos el texto ortografico.
	 */
	if ((SilOrt = SilaTxtOrt(engine->TxtOrt, engine->DicTrnPal, engine)) == NULL) {
		fprintf(engine->FpErr, "Error al silabificar %s\n", engine->TxtOrt);
		return -1;
	}

	/*
	 * Colocamos tildes en la vocales acentuadas.
	 */
	if ((SilAcc = AcenSilOrt(SilOrt, engine->DicTrnPal, engine)) == NULL) {
		fprintf(engine->FpErr, "Error al acentuar %s\n", SilOrt);
		free(SilOrt);
		return -1;
	}

	/*
	 * Realizamos la transcripcion fonetica.
	 */
	if ((engine->TrnFon = TrnSilAcc(SilAcc, engine->DicTrnPal, engine->DicTrnFon, engine->TrnPalAis, engine->ClaveModif, engine)) == NULL) {
		fprintf(engine->FpErr, "Error al transcribir %s\n", SilAcc);
		free(SilOrt);
		free(SilAcc);
		return -1;
	}

	/*
	 * Si existe el diccionario de substituciones, lo aplicamos.
	 */
	if (engine->DicSust != NULL && AplDicSust(engine->DicSust, &engine->TrnFon, engine->Fonemas) < 0) {
		fprintf(engine->FpErr, "Error al aplicar el diccionario de substituciones\n");
		free(SilOrt);
		free(SilAcc);
		return -2;
	}

	/*
	 * Si existe el diccionario de substitucion de grupos, lo aplicamos.
	 */
	if (engine->DicGrp != NULL && AplDicGrp(engine->DicGrp, &engine->TrnFon, engine->Fonemas) < 0) {
		fprintf(engine->FpErr, "Error al aplicar el diccionario de substitucion de grupos\n");
		free(SilOrt);
		free(SilAcc);
		return -2;
	}

	/*
	 * Realizamos la transcripcion en fonemas.
	 */
	if (engine->SalFnm > 0 && (engine->TrnFnm = TrnFonFnm(engine->TrnFon, engine->ConSil, engine->Fonemas)) == NULL) {
		fprintf(engine->FpErr, "Error al transcribir %s\n", engine->TrnFon);
		free(SilOrt);
		free(SilAcc);
		return -1;
	}

	/*
	 * Realizamos la transcripcion en fonemas por palabras.
	 */
	if (engine->SalFnmPal > 0 && (engine->TrnFnmPal = TrnFonFnmPal(engine->TrnFon, engine->ConSil, engine->Fonemas)) == NULL) {
		fprintf(engine->FpErr, "Error al transcribir %s\n", engine->TrnFon);
		free(SilOrt);
		free(SilAcc);
		return -1;
	}

	/*
	 * Realizamos la transcripcion en semisilabas.
	 */
	if (engine->SalSem > 0 && (engine->TrnSem = TrnFonSem(engine->TrnFon, engine->ConSil, engine->Fonemas)) == NULL) {
		fprintf(engine->FpErr, "Error al transcribir %s\n", engine->TrnFon);
		free(SilOrt);
		free(SilAcc);
		return -1;
	}

	/*
	 * Realizamos la transcripcion en semifonemas.
	 */
	if (engine->SalSefo > 0 && (engine->TrnSefo = TrnFonSefo(engine->TrnFon, engine->ConSil, engine->StrIniPal, engine->StrFinPal, engine->Fonemas)) == NULL) {
		fprintf(engine->FpErr, "Error al transcribir %s\n", engine->TrnFon);
		free(SilOrt);
		free(SilAcc);
		return -1;
	}
		free(SilOrt);
		free(SilAcc);
  return 0;
}

int SagaEngine_OpenOutputFiles(SagaEngine *engine, const char *NomOut) {
	char	PathOut[SAGA_PATH_MAX];

  if (NomOut == NULL) {
		engine->FpFon = NULL;
		engine->FpFnm = NULL;
		engine->FpFnmPal = NULL;
		engine->FpSem = NULL;
		engine->FpSefo = NULL;
		return 0;
	} else if (strcmp(NomOut, "-") == 0) {
		engine->FpFon = engine->SalFon ? stdout : NULL ;
		engine->close_FpFon = 0;
		engine->FpFnm = engine->SalFnm ? stdout : NULL ;
		engine->close_FpFnm = 0;
		engine->FpFnmPal = engine->SalFnmPal ? stdout : NULL ;
		engine->close_FpFnmPal = 0;
		engine->FpSem = engine->SalSem ? stdout : NULL ;
		engine->close_FpSem = 0;
		engine->FpSefo = engine->SalSefo ? stdout : NULL ;
		engine->close_FpSefo = 0;
		return 0;
	}
  /* Prevent buffer overflow */
  if (strlen(NomOut) >= SAGA_PATH_MAX -5 ) {
    return -1;
  }
  strcpy(PathOut, NomOut);
	strcat(PathOut, ".fon");
	if (engine->SalFon && (engine->FpFon = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}
	engine->close_FpFon = 1;

	strcpy(PathOut, NomOut);
	strcat(PathOut, ".fnm");
	if (engine->SalFnm && (engine->FpFnm = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}
	engine->close_FpFnm = 0;

	strcpy(PathOut, NomOut);
	strcat(PathOut, ".fnp");
	if (engine->SalFnmPal && (engine->FpFnmPal = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}
	engine->close_FpFnmPal = 0;

	strcpy(PathOut, NomOut);
	strcat(PathOut, ".sem");
	if (engine->SalSem && (engine->FpSem = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}
	engine->close_FpSem = 0;

	strcpy(PathOut, NomOut);
	strcat(PathOut, ".sef");
	if (engine->SalSefo && (engine->FpSefo = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}
	engine->close_FpSefo = 0;
	return 0;
}

int SagaEngine_WriteOutputFiles(SagaEngine *engine) {
			/*
		 * Escribimos el texto de salida.
		 */
		if (engine->SalFon > 0 && fprintf(engine->FpFon, "%s", engine->TrnFon) < 0) {
			fprintf(engine->FpErr, "Error al escribir el texto transcrito\n");
			return -1;
		}

		if (engine->SalFnm > 0 && fprintf(engine->FpFnm, "%s", engine->TrnFnm) < 0) {
			fprintf(engine->FpErr, "Error al escribir el texto transcrito en fonemas\n");
			return -1;
		}

		if (engine->SalFnmPal > 0 && fprintf(engine->FpFnmPal, "%s", engine->TrnFnmPal) < 0) {
			fprintf(engine->FpErr, "Error al escribir el texto transcrito en fonemas\n");
			return -1;
		}

		if (engine->SalSem > 0 && fprintf(engine->FpSem, "%s", engine->TrnSem) < 0) {
			fprintf(engine->FpErr, "Error al escribir el texto transcrito en semisilabas\n");
			return -1;
		}

		if (engine->SalSefo > 0 && fprintf(engine->FpSefo, "%s", engine->TrnSefo) < 0) {
			fprintf(engine->FpErr, "Error al escribir el texto transcrito en semifonemas\n");
			return -1;
		}
	return 0;
}

int SagaEngine_InputFromFileName(SagaEngine *engine, const char *NomIn) {
	if (NomIn == NULL) {
		/* No deberia pasar */
		fprintf(engine->FpErr, "Fichero de entrada NULL\n");
		return -1;
	}
	engine->FicInName = NomIn;
	
	if (strcmp(engine->FicInName, "-") == 0) {
		engine->FpIn = stdin;
		engine->close_in = 0;
	} else {
		engine->FpIn = fopen(engine->FicInName, "rt");
		if (engine->FpIn == NULL) {
			fprintf(engine->FpErr, "Error al abrir %s\n", engine->FicInName);
			return -1;
		}
		engine->close_in = 1;
	}
	return 0;
}

int SagaEngine_CloseInput(SagaEngine *engine) {
	if (engine->close_in) {
		if (engine->FpIn != NULL) {
		  fclose(engine->FpIn);
		  engine->FpIn = NULL;
	  }
		if (engine->TxtIn != NULL) {
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

char *CargTxtOrtChar(const char *txtin, intptr_t *TxtInOffset, int TrnLinAis) {
	char	*Txt;
	size_t input_len;
	if (txtin == NULL)
	{
		fprintf(stderr, "Error al leer el texto de entrada\n");
		return NULL;
	}
	input_len = 0;
        while (1) {
	    if (TrnLinAis && txtin[*TxtInOffset + input_len] == '\n') {
		input_len++;
		break;
	    }
	    if (txtin[*TxtInOffset + input_len] == '\0') {
                break;
	    }
            input_len++;
	}
	if (input_len == 0) return NULL;
        Txt = malloc((input_len+1) * sizeof(char));
	if (Txt == NULL) {
		fprintf(stderr, "Error al reservar memoria\n");
		return NULL;
	}
        memcpy(Txt, txtin + *TxtInOffset, input_len);
        Txt[input_len] = '\0';
	*TxtInOffset += input_len;
	return Txt;
}

char	*CargTxtOrt(FILE *fpin, int TrnLinAis, char *encoding)

{
	char	*Txt;
	char *txt_converted;
	size_t txt_conv_size;
	int		Long, AllocLong;
	int		Final;

	AllocLong = 1000;
	if ((Txt = (char *) malloc((AllocLong+1)*sizeof(char))) == NULL) {
		fprintf(stderr, "Error al ubicar memoria para Txt\n");
		return NULL;
	}

	Final = 0;
	Long = 0;
	while (Final == 0) {
		Long++;
		if (Long == AllocLong) {
			AllocLong += 1000;
			if ((Txt = (char *) realloc((void *) Txt, (size_t) (AllocLong+1) * sizeof(char))) == NULL) {
				fprintf(stderr, "Error al reubicar memoria para Txt\n");
				return NULL;
			}
		}

		if ((Txt[Long-1] = (char) fgetc(fpin)) == (char) EOF) {
			Final++;
		}

		if (TrnLinAis && (Txt[Long-1] == '\n')) {
			Long++;
			Final++;
		}

		if (Long > 2 && Txt[Long-1] == '\n' && IndexChr(&Txt[Long-2], Silen) >= 0) {
			Long++;
			break;
		}
	}

	Txt[Long-1] = '\0';
	if (Long == 1) {
		free(Txt);
		return NULL;
	}
	/* Convert encoding */
	if (encoding == NULL ||
	    strcmp(encoding, "UTF-8") == 0) {
		/* default assume UTF-8 */
		txt_converted = malloc(Long+1);
		if (txt_converted == NULL) {
			free(Txt);
			return NULL;
		}
		txt_conv_size = utf8_to_latin9(txt_converted, Txt, Long);
		free(Txt);
		Txt = realloc(txt_converted, txt_conv_size);
		if (Txt == NULL) {
			Txt = txt_converted;
		}
	}
	return Txt;
}

/***********************************************************************
 * ArreglaTxt - Arregla el texto ortografico
 **********************************************************************/

char	*ArreglaTxt(char *TxtOrt)

{
	char	*Txt;
	size_t		Chr;

	if ((Txt = (char *) malloc((2 * strlen(TxtOrt) + 1) * sizeof(char))) == NULL) {
		fprintf(stderr, "Error al ubicar memoria para Txt\n");
		return NULL;
	}

	*Txt = '\0';
	for (Chr = 0; Chr < strlen(TxtOrt); Chr++) {
		switch(TxtOrt[Chr]) {
			case 'y':
				if ((Chr == 0 || strchr(ALL_PUNCT_AND_SPACES, TxtOrt[Chr - 1]) != NULL) && \
					(Chr == strlen(TxtOrt) - 1 || strchr(ALL_PUNCT_AND_SPACES, TxtOrt[Chr + 1]) != NULL))
				{
					strcat(Txt, "y");
				} else if (strchr("aeiou" ALL_ACUTE "'", TxtOrt[Chr + 1]) == NULL)
				{
					strcat(Txt, "i");
				} else
				{
					strcat(Txt, "y");
				}
				break;
			case AACUTE:	strcat(Txt, "'a");
						break;
			case EACUTE:	strcat(Txt, "'e");
						break;
			case IACUTE:	strcat(Txt, "'i");
						break;
			case OACUTE:	strcat(Txt, "'o");
						break;
			case UACUTE:	strcat(Txt, "'u");
						break;
			case UUMLAUT:	strcat(Txt, "~u");
						break;
			case NTILDE:	strcat(Txt, "~n");
						break;
			case CEDILLA:	strcat(Txt, "~c");
						break;
			default :	strncat(Txt, TxtOrt + Chr, 1);
						break;
		}
	}

	free((void *) TxtOrt);
	return Txt;
}

int SagaArgentinaParams(SagaEngine *engine) {
	
  engine->FicDicExc = SAGA_DICCDIR "/Arg/ArgExc.dicc";
	engine->FicDicSust = SAGA_DICCDIR "/Arg/ArgSust.dicc";
	engine->FicDicGrp = SAGA_DICCDIR "/Arg/ArgDicGrp.dicc";
  engine->FicNovFon = SAGA_DICCDIR "/Arg/ArgNovFon.dicc";
	engine->ClaveModif = SESEO | ESE_ASP_CON;
	return 0;
}


int SagaCastillaParams(SagaEngine *engine) {
	
  engine->FicDicExc = SAGA_DICCDIR "/Cas/CasExc.dicc";
	engine->FicTrnFon = SAGA_DICCDIR "/Cas/CasTrnFon.dicc";
	engine->FicTrnPal = SAGA_DICCDIR "/Cas/CasTrnPal.dicc";
	engine->FicDicSust = SAGA_DICCDIR "/Cas/CasDicSust.dicc";
	engine->FicDicGrp = SAGA_DICCDIR "/Cas/CasDicGrp.dicc";
  engine->FicNovCons = SAGA_DICCDIR "/Cas/CasNovCns.dicc";
	return 0;
}

int SagaChileParams(SagaEngine *engine) {
	
  engine->FicNovFon = SAGA_DICCDIR "/Chl/ChlNovFon.dicc";
  engine->FicDicExc = SAGA_DICCDIR "/Chl/ChlExc.dicc";
	engine->FicDicSust = SAGA_DICCDIR "/Chl/ChlSust.dicc";
	engine->FicDicGrp = SAGA_DICCDIR "/Chl/ChlGrup.dicc";
	engine->ClaveModif = SESEO | ESE_ASP_INC;
	return 0;
}

int SagaColombiaParams(SagaEngine *engine) {
	
  engine->FicDicExc = SAGA_DICCDIR "/Col/ColExc.dicc";
	engine->FicDicSust = SAGA_DICCDIR "/Col/ColSust.dicc";

	engine->ClaveModif = SESEO | BDG_ANDES;
	return 0;
}

int SagaMexicoParams(SagaEngine *engine) {
	
  engine->FicDicExc = SAGA_DICCDIR "/Mex/MexExc.dicc";
	engine->FicDicSust = SAGA_DICCDIR "/Mex/MexSust.dicc";

	engine->ClaveModif = SESEO | EQUIS_KS;
	return 0;
}

int SagaPeruParams(SagaEngine *engine) {
	
  engine->FicDicExc = SAGA_DICCDIR "/Per/PerExc.dicc";
	engine->FicDicSust = SAGA_DICCDIR "/Per/PerSust.dicc";

	engine->ClaveModif = SESEO | ESE_ASP_CON | NAS_VELAR | ARCHI_IMPL | ELIM_D;
	return 0;
}

int SagaVenezuelaParams(SagaEngine *engine) {
	
  engine->FicDicExc = SAGA_DICCDIR "/Ven/VenExc.dicc";
	engine->FicDicSust = SAGA_DICCDIR "/Ven/VenSust.dicc";

	engine->ClaveModif = SESEO | ESE_ASP_INC | NAS_VELAR | EQUIS_KS;
	return 0;
}

int SagaEngine_SetParamsFromVariant(SagaEngine *engine, const char *variant) {
  if (strcmp(variant, "argentina") == 0) {
	 if (SagaArgentinaParams(engine) <0) {
		 return -1;
	 }
	} else if (strcmp(variant, "castilla") == 0) {
	 if (SagaCastillaParams(engine) <0) {
		 return -1;
	 }				 
 } else if (strcmp(variant, "chile") == 0) {
	 if (SagaChileParams(engine) <0) {
		 return -1;
	 }				 
 } else if (strcmp(variant, "colombia") == 0) {
	 if (SagaColombiaParams(engine) <0) {
		 return -1;
	 }				 
 } else if (strcmp(variant, "mexico") == 0) {
	 if (SagaMexicoParams(engine) <0) {
		 return -1;
	 }				 
 } else if (strcmp(variant, "peru") == 0) {
	 if (SagaPeruParams(engine) <0) {
		 return -1;
	 }				 
 } else if (strcmp(variant, "venezuela") == 0) {
	 if (SagaVenezuelaParams(engine) <0) {
		 return -1;
	 }				 
 } else {
	 fprintf(stderr, "Variante dialectal desconocida: '%s'\n", variant);
	 return -1;
 }
 return 0;
}

SagaEngine * SagaEngine_NewFromVariant(const char *variant) {
	SagaEngine *engine = calloc(1, sizeof(SagaEngine));
	if (engine == NULL) return NULL;
	if (SagaEngine_Initialize(engine) < 0) {
		free(engine);
		return NULL;
	}
	if (SagaEngine_SetParamsFromVariant(engine, variant) < 0) {
    SagaEngine_Clear(engine);
		free(engine);
		return NULL;
	}
  if (SagaEngine_LoadData(engine) < 0) {
    SagaEngine_Clear(engine);
    free(engine);
		return NULL;
	}
	return engine;
}

int SagaEngine_TranscribeText(SagaEngine *engine, const char *text, const char *encoding,
                              char **fon, char **fnm, char **fnmpal, char **sefo, char **sem) {
	size_t fon_size=0, fnm_size=0, fnmpal_size=0, sefo_size=0, sem_size=0;
	size_t fon_filled=0, fnm_filled=0, fnmpal_filled=0, sefo_filled=0, sem_filled=0;
	if (fon != NULL) {
    if (*fon != NULL) {
		  fon_filled = strlen(*fon);
		  fon_size = fon_filled + 1;
	  }
	  engine->SalFon = 1;
  }
  
  if (fnm != NULL) {
		if (*fnm != NULL) {
		  fnm_filled = strlen(*fnm);		
		  fnm_size = fnm_filled + 1;
	  }
	  engine->SalFnm = 1;
	}

	if (fnmpal != NULL) {
		if (*fnmpal != NULL) {
  		fnmpal_filled = strlen(*fnmpal);
	  	fnmpal_size = fnmpal_filled + 1;
		}
		engine->SalFnmPal = 1;
	}
	
	if (sefo != NULL) {
		if (*sefo != NULL) {
		  sefo_filled = strlen(*sefo);
		  sefo_size = sefo_filled + 1;
	  }
	  engine->SalSefo = 1;
	}
	
	if (sem != NULL) {
		if (*sem != NULL) {
		  sem_filled = strlen(*sem);
		  sem_size = sem_filled + 1;
		}
		engine->SalSem = 1;
	}

	SagaEngine_Refresh(engine);
	SagaEngine_InputFromText(engine, text, encoding);
		
	while ( ! (SagaEngine_ReadText(engine) < 0)) {
    if (SagaEngine_Transcribe(engine) < 0) {
			SagaEngine_Refresh(engine);
			return -1;
		}
		if (engine->SalFon && fon != NULL) {
			if (safe_strcat(fon, engine->TrnFon, &fon_size, &fon_filled) < 0) {
				return -1;
			}
		}
		if (engine->SalFnm && fnm != NULL) {
			if (safe_strcat(fnm, engine->TrnFnm, &fnm_size, &fnm_filled) < 0) {
				return -1;
			}
		}
		if (engine->SalFnmPal && fnmpal != NULL) {
			if (safe_strcat(fnmpal, engine->TrnFnmPal, &fnmpal_size, &fnmpal_filled) < 0) {
				return -1;
			}
		}
		if (engine->SalSefo && sefo != NULL) {
			if (safe_strcat(sefo, engine->TrnSem, &sefo_size, &sefo_filled) < 0) {
				return -1;
			}
		}
		if (engine->SalSem && sem != NULL) {
			if (safe_strcat(sem, engine->TrnSefo, &sem_size, &sem_filled) < 0) {
				return -1;
			}
		}
	}
	SagaEngine_CloseInput(engine);
  return 0;
}
