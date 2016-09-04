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

static void	EmpleoSaga(char	**ArgV);
static int OpcSaga(int ArgC,	char	**ArgV,
                   SagaEngine *engine,
                   char **NomIn, char	**NomOut, char **NomErr);


int main(int ArgC, char *ArgV[])
{
	char *NomIn = NULL, *NomOut = NULL, *NomErr = NULL;
	SagaEngine engine;

	SagaEngine_Initialize(&engine);
	/*
	 * Analizamos la linea de comandos.
	 */
	if (OpcSaga(ArgC, ArgV, &engine, &NomIn, &NomOut, &NomErr) < 0) {
		EmpleoSaga(ArgV);
    SagaEngine_Clear(&engine);
    if (NomIn != NULL) free(NomIn);
    if (NomOut != NULL) free(NomOut);
    if (NomErr != NULL) free(NomErr);
		return EXIT_FAILURE;
	}
	
	if (SagaEngine_OpenErrorFile(&engine, NomErr) < 0) {
    SagaEngine_Clear(&engine);
    if (NomIn != NULL) free(NomIn);
    if (NomOut != NULL) free(NomOut);
    if (NomErr != NULL) free(NomErr);
		return EXIT_FAILURE;
	}

	if (SagaEngine_InputFromFileName(&engine, NomIn) < 0) {
    SagaEngine_Clear(&engine);
    if (NomIn != NULL) free(NomIn);
    if (NomOut != NULL) free(NomOut);
    if (NomErr != NULL) free(NomErr);
		return EXIT_FAILURE;
	}

  if (SagaEngine_OpenOutputFiles(&engine, NomOut) < 0) {
    SagaEngine_Clear(&engine);
    if (NomIn != NULL) free(NomIn);
    if (NomOut != NULL) free(NomOut);
    if (NomErr != NULL) free(NomErr);
		return EXIT_FAILURE;
	}

  if (SagaEngine_LoadData(&engine) < 0) {
    SagaEngine_Clear(&engine);
    if (NomIn != NULL) free(NomIn);
    if (NomOut != NULL) free(NomOut);
    if (NomErr != NULL) free(NomErr);
		return EXIT_FAILURE;
	}
	
	while ( ! (SagaEngine_ReadText(&engine) < 0)) {
		
    if (SagaEngine_Transcribe(&engine) < 0) {
			SagaEngine_Refresh(&engine);
      SagaEngine_Clear(&engine);
      if (NomIn != NULL) free(NomIn);
      if (NomOut != NULL) free(NomOut);
      if (NomErr != NULL) free(NomErr);
			return EXIT_FAILURE;
		}
		
		if (SagaEngine_WriteOutputFiles(&engine) < 0) {
			SagaEngine_Refresh(&engine);
			continue;
		}

    if (SagaEngine_WriteErrorWords(&engine) < 0) {
			SagaEngine_Refresh(&engine);
      SagaEngine_Clear(&engine);
      if (NomIn != NULL) free(NomIn);
      if (NomOut != NULL) free(NomOut);
      if (NomErr != NULL) free(NomErr);
			return EXIT_FAILURE;
		}		
	}
	
	SagaEngine_CloseInput(&engine);
  free(NomIn);
  if (NomOut != NULL) free(NomOut);
  if (NomErr != NULL) free(NomErr);
	SagaEngine_Clear(&engine);
  return EXIT_SUCCESS;
}


/***********************************************************************
 * OpcSaga - Analiza las opciones de la linea de comandos
 **********************************************************************/

static int OpcSaga(
	int		ArgC,			/* No. argumentos linea de comandos		*/
	char	**ArgV,			/* Argumentos linea de comandos			*/
  SagaEngine *engine,
  char **NomIn, /* Nombre del fichero de entrada */
	char	**NomOut,		/* Nombre de los ficheros de salida		*/
  char **NomErr /* Nombre del fichero de error */
	)
{
	int		Opcion;
	size_t i;
	char	**Matriz;

	/* Por defecto se hara la transcripcion fonetica si no se especifica
	   otra. Dejamos todas las Sal* a 0 para ver si hay alguna especificada
	   o no. */
	engine->SalFon = 0;
	*NomOut = NULL;
	*NomIn = NULL;
	*NomErr = NULL;

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
		case 'e' :	*NomErr = strdup(optarg);
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
					free(Matriz); /* I have stolen the inner pointers */
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

	*NomIn = strdup(ArgV[optind]);

	optind++;
	if (ArgC > optind) {
		*NomOut = strdup(ArgV[optind]);
	}
	else {
		*NomOut = strdup("-");
	}
	
	if (*NomErr == NULL) {
		*NomErr = strdup("-");
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

static void	EmpleoSaga(char	**ArgV)
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
