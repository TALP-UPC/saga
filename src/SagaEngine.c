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

#define _POSIX_C_SOURCE 200809L

#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>
#include	<ctype.h>
#include	<limits.h>
#include	<values.h>
#include	<unistd.h> /* getopt family */
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

	engine->SalFon = 1;
	engine->SalFnm = 0;
	engine->SalFnmPal = 0;
	engine->SalSem = 0;
	engine->SalSefo = 0;
	engine->ConSil = 0;
	engine->ClaveModif = 0;

  engine->FicInName = NULL;
	engine->FpIn = NULL;

	engine->TxtOrt = NULL;

	engine->PalExt = NULL;
	engine->NumPalExt = 0;

	engine->TrnFon = NULL;
	engine->TrnFnm = NULL;
	engine->TrnFnmPal = NULL;
	engine->TrnSem = NULL;
	engine->TrnSefo = NULL;

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
	/* TODO: FIXME: If text is set with inputFromText this does not do anything */
	if (engine->FpIn != NULL) {
    engine->TxtOrt = CargTxtOrt(engine->FpIn, engine->TrnLinAis);
	}
	if (engine->TxtOrt == NULL) {
		fprintf(engine->FpErr, "%s", "");
		return -1;
	}
	return 0;
}

int SagaEngine_CloseOutputFiles(SagaEngine *engine) {
	if (engine->FpFon != NULL) fclose(engine->FpFon);
	if (engine->FpFnm != NULL) fclose(engine->FpFnm);
	if (engine->FpFnmPal != NULL) fclose(engine->FpFnmPal);
	if (engine->FpSem != NULL) fclose(engine->FpSem);
	if (engine->FpSefo != NULL) fclose(engine->FpSefo);
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
	int i, NumLet, NumVoc, NumCons, NumFon;
	size_t bytes_written;
	char Fonema[1024];

	/*
	 * Creamos las listas de letras, consonantes y fonemas.
	 */
	NumLet = 0;
	for (i = 0; _Letras[i] != NULL; i++) {
		if (MeteLisUdf(_Letras[i], &NumLet, &engine->Letras) < 0) {
			fprintf(engine->FpErr, "Error al crear la lista de letras\n");
			return -1;
		}
	}

	NumCons = 0;
	for (i = 0; _ConsTxt[i] != NULL; i++) {
		if (MeteLisUdf(_ConsTxt[i], &NumCons, &engine->ConsTxt) < 0) {
			fprintf(engine->FpErr, "Error al crear la lista de consonantes\n");
			return -1;
		}
	}

	NumVoc = 0;
	for (i = 0; _Vocales[i] != NULL; i++) {
		if (MeteLisUdf(_Vocales[i], &NumVoc, &engine->Vocales) < 0) {
			fprintf(engine->FpErr, "Error al crear la lista de vocales\n");
			return -1;
		}
	}

	NumFon = 0;
	for (i = 0; _Fonemas[i] != NULL; i++) {
		if (MeteLisUdf(_Fonemas[i], &NumFon, &engine->Fonemas) < 0) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}
	for (i = 0; FonCns[i] != NULL; i++) {
		if (MeteLisUdf(FonCns[i], &NumFon, &engine->Fonemas) < 0) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}
	for (i = 0; FonVoc[i] != NULL; i++) {
		if (MeteLisUdf(FonVoc[i], &NumFon, &engine->Fonemas) < 0) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}
	for (i = 0; FonSem[i] != NULL; i++) {
		if (MeteLisUdf(FonSem[i], &NumFon, &engine->Fonemas) < 0) {
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
			if (MeteLisUdf(Fonema, &NumFon, &engine->Fonemas) < 0) {
				fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
				return -1;
			}
		}
	}

	for (i = 0; engine->DicTrnFon && engine->DicTrnFon[i] != NULL; i++) {
		if (MeteLisUdf(engine->DicTrnFon[i][0], &NumLet, &engine->Letras) < 0) {
			fprintf(engine->FpErr, "Error al crear la lista de letras\n");
			return -1;
		}
		if (MeteLisUdf(engine->DicTrnFon[i][1], &NumFon, &engine->Fonemas) < 0) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}

	for (i = 0; engine->DicSust && engine->DicSust[i] != NULL; i++) {
		if (MeteLisUdf(engine->DicSust[i][1], &NumFon, &engine->Fonemas) < 0) {
			fprintf(engine->FpErr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}

	if (engine->FicNovFon != NULL) {
		if (ReadLisUdf(engine->FicNovFon, &engine->LisNovFon) < 0) {
			fprintf(engine->FpErr, "Error al leer la lista de nuevos fonemas %s\n", engine->FicNovFon);
			return -1;
		}

		for (i = 0; engine->LisNovFon[i] != NULL; i++) {
			if (MeteLisUdf(engine->LisNovFon[i], &NumFon, &engine->Fonemas) < 0) {
				fprintf(engine->FpErr, "Error al anhadir la lista de fonemas nuevos\n");
				return -1;
			}
		}
	}

	if (engine->FicNovVoc != NULL) {
		if (ReadLisUdf(engine->FicNovVoc, &engine->LisNovVoc) < 0) {
			fprintf(engine->FpErr, "Error al leer la lista de nuevas vocales %s\n", engine->FicNovVoc);
			return -1;
		}

		for (i = 0; engine->LisNovVoc[i] != NULL; i++) {
			if (MeteLisUdf(engine->LisNovVoc[i], &NumLet, &engine->Letras) < 0 || MeteLisUdf(engine->LisNovVoc[i], &NumVoc, &engine->Vocales) < 0) {
				fprintf(engine->FpErr, "Error al anhadir la lista de vocales nuevas\n");
				return -1;
			}
		}
	}

	if (engine->FicNovCons != NULL) {
		if (ReadLisUdf(engine->FicNovCons, &engine->LisNovCons) < 0) {
			fprintf(engine->FpErr, "Error al leer la lista de nuevas consonantes %s\n", engine->FicNovCons);
			return -1;
		}

		for (i = 0; engine->LisNovCons[i] != NULL; i++) {
			if (MeteLisUdf(engine->LisNovCons[i], &NumLet, &engine->Letras) < 0 || MeteLisUdf(engine->LisNovCons[i], &NumCons, &engine->ConsTxt) < 0) {
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
	if (strcmp(encoding, "ISO-8859-15") != 0) {
		fprintf(engine->FpErr, "Error encoding %s not supported. Use ISO-8859-15.\n", encoding);
		return -1;
	}
	engine->TxtOrt = strdup(text);
	if (engine->TxtOrt == NULL) {
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
	char	PathOut[_POSIX_PATH_MAX];

  if (NomOut == NULL) {
		engine->FpFon = NULL;
		engine->FpFnm = NULL;
		engine->FpFnmPal = NULL;
		engine->FpSem = NULL;
		engine->FpSefo = NULL;
		return 0;
	} else if (strcmp(NomOut, "-") == 0) {
		engine->FpFon = engine->SalFon ? stdout : NULL ;
		engine->FpFnm = engine->SalFnm ? stdout : NULL ;
		engine->FpFnmPal = engine->SalFnmPal ? stdout : NULL ;
		engine->FpSem = engine->SalSem ? stdout : NULL ;
		engine->FpSefo = engine->SalSefo ? stdout : NULL ;
		return 0;
	}
  strcpy(PathOut, NomOut);
	strcat(PathOut, ".fon");
	if (engine->SalFon && (engine->FpFon = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}

	strcpy(PathOut, NomOut);
	strcat(PathOut, ".fnm");
	if (engine->SalFnm && (engine->FpFnm = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}

	strcpy(PathOut, NomOut);
	strcat(PathOut, ".fnp");
	if (engine->SalFnmPal && (engine->FpFnmPal = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}

	strcpy(PathOut, NomOut);
	strcat(PathOut, ".sem");
	if (engine->SalSem && (engine->FpSem = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}

	strcpy(PathOut, NomOut);
	strcat(PathOut, ".sef");
	if (engine->SalSefo && (engine->FpSefo = fopen(PathOut, "wt")) == NULL) {
		 return -1;
	}
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

int SagaEngine_CloseInputFile(SagaEngine *engine) {
	if (engine->close_in) fclose(engine->FpIn);
	return 0;
}


/***********************************************************************
 * CargTxtOrt - Carga un texto ortografico.
 **********************************************************************/

char	*CargTxtOrt(FILE *fpin, int TrnLinAis)

{
	char	*Txt;
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
			case 'y':	if ((Chr == 0 || strchr(".,:;¿?()¿¡! \t\n", TxtOrt[Chr - 1])) && (Chr == strlen(TxtOrt) - 1 || strchr(".,:;¿?()¿¡! \t\n", TxtOrt[Chr + 1])))
							strcat(Txt, "y");
						else if (strchr("aeiouáéíóú'", TxtOrt[Chr + 1]) == NULL) strcat(Txt, "i");
						else strcat(Txt, "y");
						break;
			case 'á':	strcat(Txt, "'a");
						break;
			case 'é':	strcat(Txt, "'e");
						break;
			case 'í':	strcat(Txt, "'i");
						break;
			case 'ó':	strcat(Txt, "'o");
						break;
			case 'ú':	strcat(Txt, "'u");
						break;
			case 'ü':	strcat(Txt, "~u");
						break;
			case 'ñ':	strcat(Txt, "~n");
						break;
			case 'ç':	strcat(Txt, "~c");
						break;
			default :	strncat(Txt, TxtOrt + Chr, 1);
						break;
		}
	}

	free((void *) TxtOrt);
	return Txt;
}
