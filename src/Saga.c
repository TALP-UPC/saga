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

int		AplDicGrp(	char	***DicExc,
					char	**TxtOrt);

char	**Letras, **Fonemas, **ConsTxt, **Vocales;
int		 main(int ArgC, char *ArgV[])
{
	char	*TxtOrt = (char *) 0, *TrnFon = (char *) 0, *SilOrt = (char *) 0, *SilAcc = (char *) 0;
	char	*TrnFnm = (char *) 0, *TrnFnmPal = (char *) 0, *TrnSem = (char *) 0, *TrnSefo = (char *) 0;
	char	*FicDicExc, *FicTrnFon, *FicTrnPal, *FicDicSust, *FicDicGrp, *FicNovVoc, *FicNovCons, *FicNovFon;
	char	***DicExc, ***DicTrnFon, ***DicTrnPal, ***DicSust, ***DicGrp, **LisNovVoc, **LisNovCons, **LisNovFon;
	char	**PalExt = (char **) 0;
	int		SalFon, SalFnm, SalFnmPal, SalSem, SalSefo;
	int		ConSil;
	int		TrnPalAis;
	int		TrnLinAis;
	char	*StrIniPal, *StrFinPal;
	char	*NomOut, PathOut[_POSIX_PATH_MAX];
	char	Fonema[1024];
	FILE	*FpFon, *FpFnm, *FpFnmPal, *FpSem, *FpSefo;
	long	ClaveModif = 0;
	int		i;
	int		NumLet, NumVoc, NumCons, NumFon;

	/*
	 * Analizamos la linea de comandos.
	 */
	if (OpcSaga(ArgC, ArgV, &FicDicExc, &FicTrnFon, &FicTrnPal, &FicDicSust, &FicDicGrp, &FicNovVoc, &FicNovCons,
			&FicNovFon, &TrnPalAis, &SalFon, &SalFnm, &SalFnmPal, &SalSem, &SalSefo, &ConSil, &StrIniPal, &StrFinPal, &NomOut,
			&ClaveModif, &TrnLinAis) < 0) {
		EmpleoSaga(ArgV);
		return EXIT_FAILURE;
	}

	FpSem = FpFon = FpFnm = FpFnmPal = FpSefo = stdout;
	if (strcmp(NomOut, "-") != 0) {
		strcpy(PathOut, NomOut);
		strcat(PathOut, ".fon");
		if (SalFon && (FpFon = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;

		strcpy(PathOut, NomOut);
		strcat(PathOut, ".fnm");
		if (SalFnm && (FpFnm = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;

		strcpy(PathOut, NomOut);
		strcat(PathOut, ".fnp");
		if (SalFnmPal && (FpFnmPal = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;

		strcpy(PathOut, NomOut);
		strcat(PathOut, ".sem");
		if (SalSem && (FpSem = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;

		strcpy(PathOut, NomOut);
		strcat(PathOut, ".sef");
		if (SalSefo && (FpSefo = fopen(PathOut, "wt")) == (FILE *) 0) return EXIT_FAILURE;
	}

	/*
	 * Cargamos los diccionarios de excepciones y substituciones.
	 */
	DicExc = (char ***) 0;
	if (FicDicExc != (char *) 0 && (DicExc = CargDicExc(FicDicExc)) == (char ***) 0) {
		fprintf(stderr, "Error al cargar el diccionario de excepciones\n");
		return EXIT_FAILURE;
	}
	DicTrnFon = (char ***) 0;
	if (FicTrnFon != (char *) 0 && (DicTrnFon = CargDicExc(FicTrnFon)) == (char ***) 0) {
		fprintf(stderr, "Error al cargar el diccionario de transcripcion de fonemas\n");
		return EXIT_FAILURE;
	}
	DicTrnPal = (char ***) 0;
	if (FicTrnPal != (char *) 0 && (DicTrnPal = CargDicExc(FicTrnPal)) == (char ***) 0) {
		fprintf(stderr, "Error al cargar el diccionario de transcripcion de palabras\n");
		return EXIT_FAILURE;
	}
	DicSust = (char ***) 0;
	if (FicDicSust != (char *) 0 && (DicSust = CargDicExc(FicDicSust)) == (char ***) 0) {
		fprintf(stderr, "Error al cargar el diccionario de substituciones\n");
		return EXIT_FAILURE;
	}
	DicGrp = (char ***) 0;
	if (FicDicGrp != (char *) 0 && (DicGrp = CargDicExc(FicDicGrp)) == (char ***) 0) {
		fprintf(stderr, "Error al cargar el diccionario de substitucion de grupos\n");
		return EXIT_FAILURE;
	}

	/*
	 * Creamos las listas de letras, consonantes y fonemas.
	 */
	Letras = (char **) 0;
	NumLet = 0;
	for (i = 0; _Letras[i] != (char *) 0; i++) {
		if (MeteLisUdf(_Letras[i], &NumLet, &Letras) < 0) {
			fprintf(stderr, "Error al crear la lista de letras\n");
			return EXIT_FAILURE;
		}
	}

	ConsTxt = (char **) 0;
	NumCons = 0;
	for (i = 0; _ConsTxt[i] != (char *) 0; i++) {
		if (MeteLisUdf(_ConsTxt[i], &NumCons, &ConsTxt) < 0) {
			fprintf(stderr, "Error al crear la lista de consonantes\n");
			return EXIT_FAILURE;
		}
	}

	Vocales = (char **) 0;
	NumVoc = 0;
	for (i = 0; _Vocales[i] != (char *) 0; i++) {
		if (MeteLisUdf(_Vocales[i], &NumVoc, &Vocales) < 0) {
			fprintf(stderr, "Error al crear la lista de vocales\n");
			return EXIT_FAILURE;
		}
	}

	Fonemas = (char **) 0;
	NumFon = 0;
	for (i = 0; _Fonemas[i] != (char *) 0; i++) {
		if (MeteLisUdf(_Fonemas[i], &NumFon, &Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return EXIT_FAILURE;
		}
	}
	for (i = 0; FonCns[i] != (char *) 0; i++) {
		if (MeteLisUdf(FonCns[i], &NumFon, &Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return EXIT_FAILURE;
		}
	}
	for (i = 0; FonVoc[i] != (char *) 0; i++) {
		if (MeteLisUdf(FonVoc[i], &NumFon, &Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return EXIT_FAILURE;
		}
	}
	for (i = 0; FonSem[i] != (char *) 0; i++) {
		if (MeteLisUdf(FonSem[i], &NumFon, &Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return EXIT_FAILURE;
		}
	}

	if (ClaveModif & VOCAL_NASAL) {
		for (i = 0; FonVoc[i] != (char *) 0; i++) {
			sprintf(Fonema, "%s~", FonVoc[i]);
			if (MeteLisUdf(Fonema, &NumFon, &Fonemas) < 0) {
				fprintf(stderr, "Error al crear la lista de fonemas\n");
				return EXIT_FAILURE;
			}
		}
	}

	for (i = 0; DicTrnFon && DicTrnFon[i] != (char **) 0; i++) {
		if (MeteLisUdf(DicTrnFon[i][0], &NumLet, &Letras) < 0) {
			fprintf(stderr, "Error al crear la lista de letras\n");
			return EXIT_FAILURE;
		}
		if (MeteLisUdf(DicTrnFon[i][1], &NumFon, &Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return EXIT_FAILURE;
		}
	}

	for (i = 0; DicSust && DicSust[i] != (char **) 0; i++) {
		if (MeteLisUdf(DicSust[i][1], &NumFon, &Fonemas) < 0) {
			fprintf(stderr, "Error al crear la lista de fonemas\n");
			return EXIT_FAILURE;
		}
	}

	if (FicNovFon != (char *) 0) {
		if (ReadLisUdf(FicNovFon, &LisNovFon) < 0) {
			fprintf(stderr, "Error al leer la lista de nuevos fonemas %s\n", FicNovFon);
			return EXIT_FAILURE;
		}

		for (i = 0; LisNovFon[i] != (char *) 0; i++) {
			if (MeteLisUdf(LisNovFon[i], &NumFon, &Fonemas) < 0) {
				fprintf(stderr, "Error al anhadir la lista de fonemas nuevos\n");
				return EXIT_FAILURE;
			}
		}
	}

	if (FicNovVoc != (char *) 0) {
		if (ReadLisUdf(FicNovVoc, &LisNovVoc) < 0) {
			fprintf(stderr, "Error al leer la lista de nuevas vocales %s\n", FicNovVoc);
			return EXIT_FAILURE;
		}

		for (i = 0; LisNovVoc[i] != (char *) 0; i++) {
			if (MeteLisUdf(LisNovVoc[i], &NumLet, &Letras) < 0 || MeteLisUdf(LisNovVoc[i], &NumVoc, &Vocales) < 0) {
				fprintf(stderr, "Error al anhadir la lista de vocales nuevas\n");
				return EXIT_FAILURE;
			}
		}
	}

	if (FicNovCons != (char *) 0) {
		if (ReadLisUdf(FicNovCons, &LisNovCons) < 0) {
			fprintf(stderr, "Error al leer la lista de nuevas consonantes %s\n", FicNovCons);
			return EXIT_FAILURE;
		}

		for (i = 0; LisNovCons[i] != (char *) 0; i++) {
			if (MeteLisUdf(LisNovCons[i], &NumLet, &Letras) < 0 || MeteLisUdf(LisNovCons[i], &NumCons, &ConsTxt) < 0) {
				fprintf(stderr, "Error al anhadir la lista de consonantes nuevas\n");
				return EXIT_FAILURE;
			}
		}
	}

	/*
	 * Cargamos el texto ortografico.
	 */
	while ((TxtOrt = CargTxtOrt(TrnLinAis)) != (char *) 0) {
		/*
		 * Si existe el diccionario de excepciones, lo aplicamos.
		 */
		if (DicExc != (char ***) 0 && AplDicExc(DicExc, &TxtOrt) < 0) {
			fprintf(stderr, "Error al aplicar el diccionario de excepciones\n");
			return EXIT_FAILURE;
		}

		if ((TxtOrt = ArreglaTxt(TxtOrt)) == (char *) 0) {
			fprintf(stderr, "Error al arreglar el texto de entrada\n");
			return EXIT_FAILURE;
		}
		
		if (DicExc != (char ***) 0 && AplDicExc(DicExc, &TxtOrt) < 0) {
			fprintf(stderr, "Error al aplicar el diccionario de excepciones\n");
			return EXIT_FAILURE;
		}

		/*
		 * Determinamos si hay alguna palabra extranha.
		 */
		if ((PalExt = CogePalExt(TxtOrt, PalExt, DicExc, DicTrnPal)) == (char **) 0) {
			fprintf(stderr, "Error al localizar palabras extranhas\n");
			return EXIT_FAILURE;
		}

		/*
		 * Silabificamos el texto ortografico.
		 */
		if ((SilOrt = SilaTxtOrt(TxtOrt, DicTrnPal)) == (char *) 0) {
			fprintf(stderr, "Error al silabificar %s\n", TxtOrt);
			continue;
		}

		/*
		 * Colocamos tildes en la vocales acentuadas.
		 */
		if ((SilAcc = AcenSilOrt(SilOrt, DicTrnPal)) == (char *) 0) {
			fprintf(stderr, "Error al acentuar %s\n", SilOrt);
			continue;
		}

		/*
		 * Realizamos la transcripcion fonetica.
		 */
		if ((TrnFon = TrnSilAcc(SilAcc, DicTrnPal, DicTrnFon, TrnPalAis, ClaveModif)) == (char *) 0) {
			fprintf(stderr, "Error al transcribir %s\n", SilAcc);
			continue;
		}
	
		/*
		 * Si existe el diccionario de substituciones, lo aplicamos.
		 */
		if (DicSust != (char ***) 0 && AplDicSust(DicSust, &TrnFon) < 0) {
			fprintf(stderr, "Error al aplicar el diccionario de substituciones\n");
			return EXIT_FAILURE;
		}

		/*
		 * Si existe el diccionario de substitucion de grupos, lo aplicamos.
		 */
		if (DicGrp != (char ***) 0 && AplDicGrp(DicGrp, &TrnFon) < 0) {
			fprintf(stderr, "Error al aplicar el diccionario de substitucion de grupos\n");
			return EXIT_FAILURE;
		}

		/*
		 * Realizamos la transcripcion en fonemas.
		 */
		if (SalFnm > 0 && (TrnFnm = TrnFonFnm(TrnFon, ConSil)) == (char *) 0) {
			fprintf(stderr, "Error al transcribir %s\n", TrnFon);
			continue;
		}
	
		/*
		 * Realizamos la transcripcion en fonemas por palabras.
		 */
		if (SalFnmPal > 0 && (TrnFnmPal = TrnFonFnmPal(TrnFon, ConSil)) == (char *) 0) {
			fprintf(stderr, "Error al transcribir %s\n", TrnFon);
			continue;
		}
	
		/*
		 * Realizamos la transcripcion en semisilabas.
		 */
		if (SalSem > 0 && (TrnSem = TrnFonSem(TrnFon, ConSil)) == (char *) 0) {
			fprintf(stderr, "Error al transcribir %s\n", TrnFon);
			continue;
		}
	
		/*
		 * Realizamos la transcripcion en semifonemas.
		 */
		if (SalSefo > 0 && (TrnSefo = TrnFonSefo(TrnFon, ConSil, StrIniPal, StrFinPal)) == (char *) 0) {
			fprintf(stderr, "Error al transcribir %s\n", TrnFon);
			continue;
		}
	
		/*
		 * Escribimos el texto de salida.
		 */
		if (SalFon > 0 && fprintf(FpFon, "%s", TrnFon) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito\n");
			continue;
		}

		if (SalFnm > 0 && fprintf(FpFnm, "%s", TrnFnm) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito en fonemas\n");
			continue;
		}

		if (SalFnmPal > 0 && fprintf(FpFnmPal, "%s", TrnFnmPal) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito en fonemas\n");
			continue;
		}

		if (SalSem > 0 && fprintf(FpSem, "%s", TrnSem) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito en semisilabas\n");
			continue;
		}

		if (SalSefo > 0 && fprintf(FpSefo, "%s", TrnSefo) < 0) {
			fprintf(stderr, "Error al escribir el texto transcrito en semifonemas\n");
			continue;
		}

		free((void *) TxtOrt);
		free((void *) SilOrt);
		free((void *) SilAcc);
		free((void *) TrnFon);

		if (SalFnm > 0) free((void *) TrnFnm);
		if (SalFnmPal > 0) free((void *) TrnFnmPal);
		if (SalSem > 0) free((void *) TrnSem);
		if (SalSefo > 0) free((void *) TrnSefo);
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

/***********************************************************************
 * CargTxtOrt - Carga un texto ortografico.
 **********************************************************************/

char	*CargTxtOrt(int TrnLinAis)

{
	char	*Txt;
	int		Long, AllocLong;
	int		Final;

	AllocLong = 1000;
	if ((Txt = (char *) malloc((AllocLong+1)*sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para Txt\n");
		return (char *) 0;
	}

	Final = 0;
	Long = 0;
	while (Final == 0) {
		Long++;
		if (Long == AllocLong) {
			AllocLong += 1000;
			if ((Txt = (char *) realloc((void *) Txt, (size_t) (AllocLong+1) * sizeof(char))) == (char *) 0) {
				fprintf(stderr, "Error al reubicar memoria para Txt\n");
				return (char *) 0;
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

	return (Long > 1) ? Txt : (char *) 0;
}

/***********************************************************************
 * ArreglaTxt - Arregla el texto ortografico
 **********************************************************************/

char	*ArreglaTxt(char *TxtOrt)

{
	char	*Txt;
	size_t		Chr;

	if ((Txt = (char *) malloc((2 * strlen(TxtOrt) + 1) * sizeof(char))) == (char *) 0) {
		fprintf(stderr, "Error al ubicar memoria para Txt\n");
		return (char *) 0;
	}

	*Txt = '\0';
	for (Chr = 0; Chr < strlen(TxtOrt); Chr++) {
		switch(TxtOrt[Chr]) {
			case 'y':	if ((Chr == 0 || strchr(".,:;¿?()¿¡! \t\n", TxtOrt[Chr - 1])) && (Chr == strlen(TxtOrt) - 1 || strchr(".,:;¿?()¿¡! \t\n", TxtOrt[Chr + 1])))
							strcat(Txt, "y");
						else if (strchr("aeiouáéíóú'", TxtOrt[Chr + 1]) == (char *) 0) strcat(Txt, "i");
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

int		OpcSaga(
	int		ArgC,			/* No. argumentos linea de comandos		*/
	char	**ArgV,			/* Argumentos linea de comandos			*/
	char	**FicDicExc,	/* Diccionario de excepciones			*/
	char	**FicTrnFon,
	char	**FicTrnPal,
	char	**FicDicSust,	/* Diccionario de substituciones		*/
	char	**FicDicGrp,	/* Diccionario de substituciones		*/
	char	**FicNovVoc,
	char	**FicNovCons,
	char	**FicNovFon,
	int		*TrnPalAis,		/* Flag de trans. palabras aisladas		*/
	int		*SalFon,		/* Transcripcion fonetica				*/
	int		*SalFnm,		/* Transcripcion en fonemas				*/
	int		*SalFnmPal,
	int		*SalSem,		/* Transcripcion en semisilabas			*/
	int		*SalSefo,
	int		*ConSil,		/* Conservar los silencios				*/
	char	**StrIniPal,
	char	**StrFinPal,
	char	**NomOut,		/* Nombre de los ficheros de salida		*/
	long	*ClaveModif,
	int		*TrnLinAis		/* Flag de trans. lineas aisladas		*/
	)

{
	int		Opcion;
	size_t i;
	char	*FicErr;
	char	**Matriz;

	/*
	 * Valores por defecto.
	 */
	*FicDicExc = (char *) 0;
	*FicTrnFon = (char *) 0;
	*FicTrnPal = (char *) 0;
	*FicDicSust = (char *) 0;
	*FicDicGrp = (char *) 0;
	*FicNovFon = (char *) 0;
	*FicNovVoc = (char *) 0;
	*FicNovCons = (char *) 0;
	*StrIniPal = strdup(".-");
	*StrFinPal = strdup("+.");
	*TrnPalAis = 0;
	*TrnLinAis = 0;
	*SalFon = 0;
	*SalFnm = 0;
	*SalFnmPal = 0;
	*SalSem = 0;
	*SalSefo = 0;
	*ConSil = 0;
	*NomOut = (char *) 0;
	*ClaveModif = 0;
	FicErr = (char *) 0;

	while ((Opcion = getopt(ArgC, ArgV, "abd:t:T:x:g:v:c:l:e:fFpysSM:Y:")) != -1) {
		switch (Opcion) {
		case 'a' :	*TrnPalAis = 1;
					*TrnLinAis = 1;
					break;
		case 'b' :	*TrnLinAis = 1;
					break;
		case 'd' :	*FicDicExc = optarg;
					break;
		case 't' :	*FicTrnFon = optarg;
					break;
		case 'T' :	*FicTrnPal = optarg;
					break;
		case 'x' :	*FicDicSust = optarg;
					break;
		case 'g' :	*FicDicGrp = optarg;
					break;
		case 'v' :	*FicNovVoc = optarg;
					break;
		case 'c' :	*FicNovCons = optarg;
					break;
		case 'l' :	*FicNovFon = optarg;
					break;
		case 'e' :	FicErr = optarg;
					break;
		case 'f' :	*SalFon = 1;
					break;
		case 'F' :	*SalFnm = 1;
					break;
		case 'p' :	*SalFnmPal = 1;
					break;
		case 'y' :	*SalSefo = 1;
					break;
		case 'Y' :	Matriz = MatStr(optarg);
					*StrIniPal = *StrFinPal = Matriz[0];
					if (Matriz[1] != (char *) 0) *StrFinPal = Matriz[1];
					break;
		case 's' :	*SalSem = 1;
					break;
		case 'S' :	*ConSil = 1;
					break;
		case 'M' :	for (i = 0; i < strlen(optarg); i++) {
						switch (optarg[i]) {
						case ' '	:	
										continue;
						case 'S'	:	*ClaveModif |= SESEO;
										break;
						case 'X'	:	*ClaveModif |= EQUIS_KS;
										break;
						case 'H'	:	*ClaveModif |= ESE_ASP_INC;
										break;
						case 'h'	:	*ClaveModif |= ESE_ASP_CON;
										break;
						case 'K'	:	*ClaveModif |= SC_KS;
										break;
						case 'A'	:	*ClaveModif |= BDG_ANDES;
										break;
						case 'N'	:	*ClaveModif |= ENE_VELAR;
										break;
						case 'M'	:	*ClaveModif |= NAS_VELAR;
										break;
						case 'P'	:	*ClaveModif |= ARCHI_IMPL;
										break;
						case 'y'	:	*ClaveModif |= Y_VOCAL;
										break;
						case 'R'	:	*ClaveModif |= ERRE_IMPL;
										break;
						case '@'	:	*ClaveModif |= GRUPO_SIL;
										break;
						case ':'	:	*ClaveModif |= MARCA_IMPL;
										break;
						case '_'	:	*ClaveModif |= VOCAL_PTON;
										break;
						case '.'	:	*ClaveModif |= INI_FIN_PAL;
										break;
						case '~'	:	*ClaveModif |= VOCAL_NASAL;
										break;
						case 'C'	:	*ClaveModif |= OCLUS_EXPL;
										break;
						case 'E'	:	switch (optarg[++i]) {
										case 'b'	:	*ClaveModif |= ELIM_B;
														break;
										case 'd'	:	*ClaveModif |= ELIM_D;
														break;
										case 'g'	:	*ClaveModif |= ELIM_G;
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

	if (FicErr != (char *) 0) {
		if (freopen(FicErr, "wt", stderr) == (FILE *) 0) {
			fprintf(stderr, "Error al abrir %s\n", FicErr);
			return -1;
		}
	}

	/*
	 * La transcripcion por defecto es en alofonos (.fon);
	 */
	if (!*SalFon && !*SalFnm && !*SalFnmPal && !*SalSem && !*SalSefo) {
		*SalFon = 1;
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

