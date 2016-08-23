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
#include	"Saga.h"

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
	engine->StrIniPal = strdup(".-");
	if (engine->StrIniPal == NULL) {
		return -1;
	}
	engine->StrFinPal = strdup("+.");
	if (engine->StrFinPal == NULL) {
		return -1;
	}
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

	engine->TxtSalFon = NULL;
	engine->TxtSalFnm = NULL;
	engine->TxtSalFnmPal = NULL;
	engine->TxtSalSefo = NULL;
	engine->TxtSalSem = NULL;

	return 0;
}

int SagaEngine_LoadDictionaries(SagaEngine *engine)
{
	/*
	 * Cargamos los diccionarios de excepciones y substituciones.
	 */
	if (engine->FicDicExc != NULL && (engine->DicExc = CargDicExc(engine->FicDicExc)) == NULL) {
		fprintf(stderr, "Error al cargar el diccionario de excepciones\n");
		return -1;
	}
	if (engine->FicTrnFon != NULL && (engine->DicTrnFon = CargDicExc(engine->FicTrnFon)) == NULL) {
		fprintf(stderr, "Error al cargar el diccionario de transcripcion de fonemas\n");
		return -1;
	}
	if (engine->FicTrnPal != NULL && (engine->DicTrnPal = CargDicExc(engine->FicTrnPal)) == NULL) {
		fprintf(stderr, "Error al cargar el diccionario de transcripcion de palabras\n");
		return -1;
	}
	if (engine->FicDicSust != NULL && (engine->DicSust = CargDicExc(engine->FicDicSust)) == NULL) {
		fprintf(stderr, "Error al cargar el diccionario de substituciones\n");
		return -1;
	}
	if (engine->FicDicGrp != NULL && (engine->DicGrp = CargDicExc(engine->FicDicGrp)) == NULL) {
		fprintf(stderr, "Error al cargar el diccionario de substitucion de grupos\n");
		return -1;
	}
	return 0;
}

int SagaEngine_LoadCharacters(SagaEngine *engine)
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
			fprintf(stderr, "Error al crear la lista de letras\n");
			return -1;
		}
	}

	NumCons = 0;
	for (i = 0; _ConsTxt[i] != NULL; i++) {
		if (MeteLisUdf(_ConsTxt[i], &NumCons, &engine->ConsTxt) < 0) {
			fprintf(stderr, "Error al crear la lista de consonantes\n");
			return -1;
		}
	}

	NumVoc = 0;
	for (i = 0; _Vocales[i] != NULL; i++) {
		if (MeteLisUdf(_Vocales[i], &NumVoc, &engine->Vocales) < 0) {
			fprintf(stderr, "Error al crear la lista de vocales\n");
			return -1;
		}
	}

	NumFon = 0;
	for (i = 0; _Fonemas[i] != NULL; i++) {
		if (MeteLisUdf(_Fonemas[i], &NumFon, &engine->Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}
	for (i = 0; FonCns[i] != NULL; i++) {
		if (MeteLisUdf(FonCns[i], &NumFon, &engine->Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}
	for (i = 0; FonVoc[i] != NULL; i++) {
		if (MeteLisUdf(FonVoc[i], &NumFon, &engine->Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}
	for (i = 0; FonSem[i] != NULL; i++) {
		if (MeteLisUdf(FonSem[i], &NumFon, &engine->Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}

	if (engine->ClaveModif & VOCAL_NASAL) {
		for (i = 0; FonVoc[i] != NULL; i++) {
			bytes_written = snprintf(Fonema, 1024, "%s~", FonVoc[i]);
			if (bytes_written >= 1024) {
				fprintf(stderr, "Buffer overflow al crear la lista de fonemas\n");
				return -1;
			}
			if (MeteLisUdf(Fonema, &NumFon, &engine->Fonemas) < 0) {
				fprintf(stderr, "Error al crear la lista de fonemas\n");
				return -1;
			}
		}
	}

	for (i = 0; engine->DicTrnFon && engine->DicTrnFon[i] != NULL; i++) {
		if (MeteLisUdf(engine->DicTrnFon[i][0], &NumLet, &engine->Letras) < 0) {
			fprintf(stderr, "Error al crear la lista de letras\n");
			return -1;
		}
		if (MeteLisUdf(engine->DicTrnFon[i][1], &NumFon, &engine->Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}

	for (i = 0; engine->DicSust && engine->DicSust[i] != NULL; i++) {
		if (MeteLisUdf(engine->DicSust[i][1], &NumFon, &engine->Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return -1;
		}
	}

	if (engine->FicNovFon != NULL) {
		if (ReadLisUdf(engine->FicNovFon, &engine->LisNovFon) < 0) {
			fprintf(stderr, "Error al leer la lista de nuevos fonemas %s\n", engine->FicNovFon);
			return -1;
		}

		for (i = 0; engine->LisNovFon[i] != NULL; i++) {
			if (MeteLisUdf(engine->LisNovFon[i], &NumFon, &engine->Fonemas) < 0) {
				fprintf(stderr, "Error al anhadir la lista de fonemas nuevos\n");
				return -1;
			}
		}
	}

	if (engine->FicNovVoc != NULL) {
		if (ReadLisUdf(engine->FicNovVoc, &engine->LisNovVoc) < 0) {
			fprintf(stderr, "Error al leer la lista de nuevas vocales %s\n", engine->FicNovVoc);
			return -1;
		}

		for (i = 0; engine->LisNovVoc[i] != NULL; i++) {
			if (MeteLisUdf(engine->LisNovVoc[i], &NumLet, &engine->Letras) < 0 || MeteLisUdf(engine->LisNovVoc[i], &NumVoc, &engine->Vocales) < 0) {
				fprintf(stderr, "Error al anhadir la lista de vocales nuevas\n");
				return -1;
			}
		}
	}

	if (engine->FicNovCons != NULL) {
		if (ReadLisUdf(engine->FicNovCons, &engine->LisNovCons) < 0) {
			fprintf(stderr, "Error al leer la lista de nuevas consonantes %s\n", engine->FicNovCons);
			return -1;
		}

		for (i = 0; engine->LisNovCons[i] != NULL; i++) {
			if (MeteLisUdf(engine->LisNovCons[i], &NumLet, &engine->Letras) < 0 || MeteLisUdf(engine->LisNovCons[i], &NumCons, &engine->ConsTxt) < 0) {
				fprintf(stderr, "Error al anhadir la lista de consonantes nuevas\n");
				return -1;
			}
		}
	}
	return 0;
}

int MainPorArgumentos(SagaEngine *engine, char *NomOut)
{
	char	*TxtOrt = NULL, *TrnFon = NULL, *SilOrt = NULL, *SilAcc = NULL;
	char	*TrnFnm = NULL, *TrnFnmPal = NULL, *TrnSem = NULL, *TrnSefo = NULL;
	char	**PalExt = NULL;
	char	PathOut[_POSIX_PATH_MAX];
	FILE	*FpFon, *FpFnm, *FpFnmPal, *FpSem, *FpSefo;

	FpSem = FpFon = FpFnm = FpFnmPal = FpSefo = stdout;
	if (strcmp(NomOut, "-") != 0) {
		strcpy(PathOut, NomOut);
		strcat(PathOut, ".fon");
		if (engine->SalFon && (FpFon = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;

		strcpy(PathOut, NomOut);
		strcat(PathOut, ".fnm");
		if (engine->SalFnm && (FpFnm = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;

		strcpy(PathOut, NomOut);
		strcat(PathOut, ".fnp");
		if (engine->SalFnmPal && (FpFnmPal = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;

		strcpy(PathOut, NomOut);
		strcat(PathOut, ".sem");
		if (engine->SalSem && (FpSem = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;

		strcpy(PathOut, NomOut);
		strcat(PathOut, ".sef");
		if (engine->SalSefo && (FpSefo = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;
	}

        SagaEngine_LoadDictionaries(engine);
	SagaEngine_LoadCharacters(engine);
	/*
	 * Cargamos el texto ortografico.
	 */
	while ((TxtOrt = CargTxtOrt(engine->TrnLinAis)) != NULL) {
		/*
		 * Si existe el diccionario de excepciones, lo aplicamos.
		 */
		if (engine->DicExc != NULL && AplDicExc(engine->DicExc, &TxtOrt, engine->Letras) < 0) {
			fprintf(stderr, "Error al aplicar el diccionario de excepciones\n");
			return EXIT_FAILURE;
		}

		if ((TxtOrt = ArreglaTxt(TxtOrt)) == NULL) {
			fprintf(stderr, "Error al arreglar el texto de entrada\n");
			return EXIT_FAILURE;
		}
		
		if (engine->DicExc != NULL && AplDicExc(engine->DicExc, &TxtOrt, engine->Letras) < 0) {
			fprintf(stderr, "Error al aplicar el diccionario de excepciones\n");
			return EXIT_FAILURE;
		}

		/*
		 * Determinamos si hay alguna palabra extranha.
		 */
		if ((PalExt = CogePalExt(TxtOrt, PalExt, engine->DicExc, engine->DicTrnPal, engine->ConsTxt, engine->Vocales, engine->Letras)) == NULL) {
			fprintf(stderr, "Error al localizar palabras extranhas\n");
			return EXIT_FAILURE;
		}

		/*
		 * Silabificamos el texto ortografico.
		 */
		if ((SilOrt = SilaTxtOrt(TxtOrt, engine->DicTrnPal, engine)) == NULL) {
			fprintf(stderr, "Error al silabificar %s\n", TxtOrt);
			continue;
		}

		/*
		 * Colocamos tildes en la vocales acentuadas.
		 */
		if ((SilAcc = AcenSilOrt(SilOrt, engine->DicTrnPal, engine)) == NULL) {
			fprintf(stderr, "Error al acentuar %s\n", SilOrt);
			continue;
		}

		/*
		 * Realizamos la transcripcion fonetica.
		 */
		if ((TrnFon = TrnSilAcc(SilAcc, engine->DicTrnPal, engine->DicTrnFon, engine->TrnPalAis, engine->ClaveModif, engine)) == NULL) {
			fprintf(stderr, "Error al transcribir %s\n", SilAcc);
			continue;
		}
	
		/*
		 * Si existe el diccionario de substituciones, lo aplicamos.
		 */
		if (engine->DicSust != NULL && AplDicSust(engine->DicSust, &TrnFon, engine->Fonemas) < 0) {
			fprintf(stderr, "Error al aplicar el diccionario de substituciones\n");
			return EXIT_FAILURE;
		}

		/*
		 * Si existe el diccionario de substitucion de grupos, lo aplicamos.
		 */
		if (engine->DicGrp != NULL && AplDicGrp(engine->DicGrp, &TrnFon, engine->Fonemas) < 0) {
			fprintf(stderr, "Error al aplicar el diccionario de substitucion de grupos\n");
			return EXIT_FAILURE;
		}

		/*
		 * Realizamos la transcripcion en fonemas.
		 */
		if (engine->SalFnm > 0 && (TrnFnm = TrnFonFnm(TrnFon, engine->ConSil, engine->Fonemas)) == NULL) {
			fprintf(stderr, "Error al transcribir %s\n", TrnFon);
			continue;
		}
	
		/*
		 * Realizamos la transcripcion en fonemas por palabras.
		 */
		if (engine->SalFnmPal > 0 && (TrnFnmPal = TrnFonFnmPal(TrnFon, engine->ConSil, engine->Fonemas)) == NULL) {
			fprintf(stderr, "Error al transcribir %s\n", TrnFon);
			continue;
		}
	
		/*
		 * Realizamos la transcripcion en semisilabas.
		 */
		if (engine->SalSem > 0 && (TrnSem = TrnFonSem(TrnFon, engine->ConSil, engine->Fonemas)) == NULL) {
			fprintf(stderr, "Error al transcribir %s\n", TrnFon);
			continue;
		}
	
		/*
		 * Realizamos la transcripcion en semifonemas.
		 */
		if (engine->SalSefo > 0 && (TrnSefo = TrnFonSefo(TrnFon, engine->ConSil, engine->StrIniPal, engine->StrFinPal, engine->Fonemas)) == NULL) {
			fprintf(stderr, "Error al transcribir %s\n", TrnFon);
			continue;
		}
	
		/*
		 * Escribimos el texto de salida.
		 */
		if (engine->SalFon > 0 && fprintf(FpFon, "%s", TrnFon) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito\n");
			continue;
		}

		if (engine->SalFnm > 0 && fprintf(FpFnm, "%s", TrnFnm) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito en fonemas\n");
			continue;
		}

		if (engine->SalFnmPal > 0 && fprintf(FpFnmPal, "%s", TrnFnmPal) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito en fonemas\n");
			continue;
		}

		if (engine->SalSem > 0 && fprintf(FpSem, "%s", TrnSem) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito en semisilabas\n");
			continue;
		}

		if (engine->SalSefo > 0 && fprintf(FpSefo, "%s", TrnSefo) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito en semifonemas\n");
			continue;
		}

		free((void *) TxtOrt);
		free((void *) SilOrt);
		free((void *) SilAcc);
		free((void *) TrnFon);

		if (engine->SalFnm > 0) free(TrnFnm);
		if (engine->SalFnmPal > 0) free(TrnFnmPal);
		if (engine->SalSem > 0) free(TrnSem);
		if (engine->SalSefo > 0) free(TrnSefo);
	}

	/*
	 * Escribimos las palabras extranhas encontradas.
	 */
	if (EscrPalExt(PalExt) < 0) {
		fprintf(stderr, "Error al escribir las palabras extranhas\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

int main(int ArgC, char *ArgV[])
{
	char	*NomOut;
	SagaEngine engine;
	/*
	 * Analizamos la linea de comandos.
	 */
	if (OpcSaga(ArgC, ArgV, &engine, &NomOut) < 0) {
		EmpleoSaga(ArgV);
		return EXIT_FAILURE;
	}
        return MainPorArgumentos(&engine, NomOut);
}


/***********************************************************************
 * CargTxtOrt - Carga un texto ortografico.
 **********************************************************************/

char	*CargTxtOrt(int TrnLinAis)

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

		if ((Txt[Long-1] = (char) getchar()) == (char) EOF) {
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

	return (Long > 1) ? Txt : NULL;
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

/***********************************************************************
 * OpcSaga - Analiza las opciones de la linea de comandos
 **********************************************************************/

int OpcSaga(
	int		ArgC,			/* No. argumentos linea de comandos		*/
	char	**ArgV,			/* Argumentos linea de comandos			*/
        SagaEngine *engine,
	char	**NomOut		/* Nombre de los ficheros de salida		*/
	)

{
	int		Opcion;
	size_t i;
	char	*FicErr;
	char	**Matriz;

	/*
	 * Valores por defecto.
	 */
	SagaEngine_Initialize(engine);
	/* Por defecto se hara la transcripcion fonetica si no se especifica
	   otra. Dejamos todas las Sal* a 0 para ver si hay alguna especificada
	   o no. */
	engine->SalFon = 0;
	*NomOut = NULL;
	FicErr = NULL;

	while ((Opcion = getopt(ArgC, ArgV, "abd:t:T:x:g:v:c:l:e:fFpysSM:Y:")) != -1) {
		switch (Opcion) {
		case 'a' :	engine->TrnPalAis = 1;
					engine->TrnLinAis = 1;
					break;
		case 'b' :	engine->TrnLinAis = 1;
					break;
		case 'd' :	engine->FicDicExc = optarg;
					break;
		case 't' :	engine->FicTrnFon = optarg;
					break;
		case 'T' :	engine->FicTrnPal = optarg;
					break;
		case 'x' :	engine->FicDicSust = optarg;
					break;
		case 'g' :	engine->FicDicGrp = optarg;
					break;
		case 'v' :	engine->FicNovVoc = optarg;
					break;
		case 'c' :	engine->FicNovCons = optarg;
					break;
		case 'l' :	engine->FicNovFon = optarg;
					break;
		case 'e' :	FicErr = optarg;
					break;
		case 'f' :	engine->SalFon = 1;
					break;
		case 'F' :	engine->SalFnm = 1;
					break;
		case 'p' :	engine->SalFnmPal = 1;
					break;
		case 'y' :	engine->SalSefo = 1;
					break;
		case 'Y' :	Matriz = MatStr(optarg);
					engine->StrIniPal = engine->StrFinPal = Matriz[0];
					if (Matriz[1] != NULL) engine->StrFinPal = Matriz[1];
					break;
		case 's' :	engine->SalSem = 1;
					break;
		case 'S' :	engine->ConSil = 1;
					break;
		case 'M' :	for (i = 0; i < strlen(optarg); i++) {
						switch (optarg[i]) {
						case ' '	:	
										continue;
						case 'S'	:	engine->ClaveModif |= SESEO;
										break;
						case 'X'	:	engine->ClaveModif |= EQUIS_KS;
										break;
						case 'H'	:	engine->ClaveModif |= ESE_ASP_INC;
										break;
						case 'h'	:	engine->ClaveModif |= ESE_ASP_CON;
										break;
						case 'K'	:	engine->ClaveModif |= SC_KS;
										break;
						case 'A'	:	engine->ClaveModif |= BDG_ANDES;
										break;
						case 'N'	:	engine->ClaveModif |= ENE_VELAR;
										break;
						case 'M'	:	engine->ClaveModif |= NAS_VELAR;
										break;
						case 'P'	:	engine->ClaveModif |= ARCHI_IMPL;
										break;
						case 'y'	:	engine->ClaveModif |= Y_VOCAL;
										break;
						case 'R'	:	engine->ClaveModif |= ERRE_IMPL;
										break;
						case '@'	:	engine->ClaveModif |= GRUPO_SIL;
										break;
						case ':'	:	engine->ClaveModif |= MARCA_IMPL;
										break;
						case '_'	:	engine->ClaveModif |= VOCAL_PTON;
										break;
						case '.'	:	engine->ClaveModif |= INI_FIN_PAL;
										break;
						case '~'	:	engine->ClaveModif |= VOCAL_NASAL;
										break;
						case 'C'	:	engine->ClaveModif |= OCLUS_EXPL;
										break;
						case 'E'	:	switch (optarg[++i]) {
										case 'b'	:	engine->ClaveModif |= ELIM_B;
														break;
										case 'd'	:	engine->ClaveModif |= ELIM_D;
														break;
										case 'g'	:	engine->ClaveModif |= ELIM_G;
														break;
										default		:	fprintf(stderr, "Clave desconocida \"%c\"\n", optarg[i + 1]);
														return -1;
														break;
										}
										break;
						default		:	fprintf(stderr, "Clave de modificacion desconocida \"%c\"\n", optarg[i]);
										return -1;
						}
					}
					break;
		case '?' :
					return -1;
		}
	}

	if (ArgC == optind) {
		return -1;
	}

	if (strcmp(ArgV[optind], "-") != 0) {
		if (freopen(ArgV[optind], "rt", stdin) == (FILE *) 0) {
			fprintf(stderr, "Error al abrir %s\n", ArgV[optind]);
			return -1;
		}
	}

	optind++;
	if (ArgC > optind) {
		*NomOut = ArgV[optind];
	}
	else {
		*NomOut = strdup("-");
	}

	if (FicErr != NULL) {
		if (freopen(FicErr, "wt", stderr) == (FILE *) 0) {
			fprintf(stderr, "Error al abrir %s\n", FicErr);
			return -1;
		}
	}

	/*
	 * La transcripcion por defecto es en alofonos (.fon);
	 */
	if (!engine->SalFon && !engine->SalFnm && !engine->SalFnmPal && !engine->SalSem && !engine->SalSefo) {
		engine->SalFon = 1;
	}

	return 0;
}

/***********************************************************************
 * EmpleoSaga - Indica el empleo correcto de Saga
 **********************************************************************/

void	EmpleoSaga(char	**ArgV)

{
	fprintf(stderr, "Empleo:\n");
	fprintf(stderr, "    %s [opciones] (TxtIn | -) [NomOut]\n", ArgV[0]);
	fprintf(stderr, "donde:\n");
	fprintf(stderr, "    TxtIn:    Texto ortografico de entrada\n");
	fprintf(stderr, "    NomOut:    Nombre, sin ext., ficheros de salida\n");
	fprintf(stderr, "y opciones puede ser:\n");
	fprintf(stderr, "    -f        : Transcripcion fonetica (ext. .fon)\n");
	fprintf(stderr, "    -F        : Transcripcion en fonemas (ext. .fnm)\n");
	fprintf(stderr, "    -p        : Transcripcion en fonemas por palabras (ext. .fnp)\n");
	fprintf(stderr, "    -s        : Transcripcion en semisilabas (ext. .sem)\n");
	fprintf(stderr, "    -y        : Transcripcion en semifonemas (ext. .sef)\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "    -a        : Transcribir palabras aisladas\n");
	fprintf(stderr, "    -b        : Transcribir cada línea de forma aislada\n");
	fprintf(stderr, "    -S        : Conservar los silencios en la salida\n");
	fprintf(stderr, "    -Y ExtPal[,FinPal] : Marca de inicio y/o final de palabra\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "    -d DicExc : Diccionario de excepciones ortograficas\n");
	fprintf(stderr, "    -T TrnPal : Diccionario de transcripcion de palabras\n");
	fprintf(stderr, "    -t TrnFon : Diccionario de transcripcion de grafemas\n");
	fprintf(stderr, "    -x DicSus : Diccionario de substitucion de fonemas\n");
	fprintf(stderr, "    -g DicGrp : Diccionario de substitucion de grupos foneticos\n");
	fprintf(stderr, "    -v NovVoc : Lista de grafemas vocalicos introducidos\n");
	fprintf(stderr, "    -c NovCns : Lista de grafemas consonanticos introducidos\n");
	fprintf(stderr, "    -l NovFon : Lista de fonemas introducidos\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "    -MS       : Aplicar seseo\n");
	fprintf(stderr, "    -MX       : Transcribir [x] como [ks]\n");
	fprintf(stderr, "    -MH       : Transcribir [s] implosiva como [h] siempre\n");
	fprintf(stderr, "    -Mh       : Transcribir [s] implosiva como [h] salvo final o enlace entre palabras\n");
	fprintf(stderr, "    -MK       : Transcribir [sT] como [ks]\n");
	fprintf(stderr, "    -MA       : Transcribir [b], [d] y [g] andinas\n");
	fprintf(stderr, "    -My       : Transcribir siempre vocalica la conjuncion /y/\n");
	fprintf(stderr, "    -MEb      : Eliminar [b] entre vocales y al final de palabra\n");
	fprintf(stderr, "    -MEd      : Eliminar [d] entre vocales y al final de palabra\n");
	fprintf(stderr, "    -MEg      : Eliminar [g] entre vocales y al final de palabra\n");
	fprintf(stderr, "    -MN       : Velarizar las [n] implosivas\n");
	fprintf(stderr, "    -MM       : Velarizar todas las nasales implosivas\n");
	fprintf(stderr, "    -M~       : Nasalizar vocales\n");
	fprintf(stderr, "    -MP       : Representar [pbdtkgx] implosivas internas como [G]\n");
	fprintf(stderr, "    -MR       : Transcribir [r] implosiva como [R]\n");
	fprintf(stderr, "    -M@       : Transcribir [l] y [r] tras [pbd...] como [@l] [@r]\n");
	fprintf(stderr, "    -M:       : Marcar consonantes implosivas con [:]\n");
	fprintf(stderr, "    -M_       : Marcar vocales postonicas con [_]\n");
	fprintf(stderr, "    -M.       : Marcar con . los fonemas inicial y final de palabra\n");
	fprintf(stderr, "    -MC       : Separar las oclusiones (p: pcl p   t: tcl t ...)\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "    -e FicErr : Fichero de errores\n");
	fprintf(stderr, "\nTxtIn = \"-\" implica lectura de entrada estandar (teclado). Por defecto,\n");
	fprintf(stderr, "(FonOut no indicado) se escribe en salida estandar (pantalla)\n");
	fprintf(stderr, "\nPor defecto se realiza solo la transcripcion fonetica (ext. .fon)\n");
	fprintf(stderr, "\nLa transcripcion fonetica siempre conserva los silencios. Las otras,\n");
	fprintf(stderr, "si no se indica la opcion -S, no los conservan.\n");
	fprintf(stderr, "\nLos diccionarios de excepciones ortograficas y substitucion de grupos\n");
	fprintf(stderr, "foneticos permiten el uso de * al principio y/o final de la palabra\n");
	fprintf(stderr, "\nPara encadenar mas de un diccionario, usar: FicDic1+FicDic2...\n");

	return;
}

