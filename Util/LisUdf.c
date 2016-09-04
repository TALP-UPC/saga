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

#include	<limits.h>
#include	<values.h>
#include	<string.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	"LisUdf.h"
#include	"Util.h"
#include <stdint.h>

/***********************************************************************
 * ReadLisUdf - Lee una lista de unidades foneticas
 **********************************************************************/

int		ReadLisUdf(
	const char	*FicLisUdf,	/* Fichero ASCII con las unidades foneticas	*/
	char	***LisUdf)	/* Lista de unidades resultante				*/

{
	char	Unidad[64000];
	size_t		TamLis;
	FILE	*FpLis;

	/*
	 * Inicializamos la memoria empleada por LisUdf.
	 */
	if ((*LisUdf = malloc(sizeof(char *))) == NULL) {
		return -1;
	}

	/*
	 * Abrimos el fichero de la lista.
	 */
	if ((FpLis = fopen(FicLisUdf, "rt")) == NULL) {
		free(*LisUdf);
		return -1;
	}

	/*
	 * Bucle para todas las unidades contenidas en el fichero.
	 */
	TamLis = 0;
	while (fgets(Unidad, sizeof(Unidad), FpLis) != NULL) {
		TamLis++;
		if (TamLis == SIZE_MAX-1)
		{
			fprintf(stderr, "La lista de fonemas ha alcanzado su tamanho maximo\n");
			free((*LisUdf)[TamLis-2]);
			(*LisUdf)[TamLis-2] = NULL;
			LiberaMatStr(*LisUdf);
			return -1;
		}
		if ((*LisUdf = realloc(*LisUdf, (TamLis + 1) * sizeof(char *))) == NULL) {
			return -1;
		}

		if (Unidad[strlen(Unidad) - 1] == '\n') Unidad[strlen(Unidad) - 1] = '\0';

		(*LisUdf)[TamLis-1] = strdup(Unidad);
	}
	(*LisUdf)[TamLis] = NULL;

	(void) fclose(FpLis);

	return 	TamLis;
}

/***********************************************************************
 * SeekLisUdf - Busca el indice de una unidad en una lista de unidades
 **********************************************************************/

int		SeekLisUdf(
	char	*Unidad,
	int		TamLis,
	char	**LisUdf)
{
	int		Udf;

	if (TamLis < 0) for (TamLis = 0; LisUdf[TamLis] != (char *) 0; TamLis++) ;

	for (Udf = 0; Udf < TamLis; Udf++) {
		if (strcmp(Unidad, LisUdf[Udf]) == 0) {
			return Udf;
		}
	}

	return -1;
}

/***********************************************************************
 * MeteLisUdf - Mete una nueva unidad en una lista de unidades foneticas
 **********************************************************************/

int		MeteLisUdf(
	char	*Unidad,
	int		*TamLis,
	char	***LisUdf)
{
	int		Indice;

	if (*TamLis < 0) for (*TamLis = 0; *LisUdf && (*LisUdf)[*TamLis] != (char *) 0; (*TamLis)++) ;

	if (*TamLis == 0 || *LisUdf == (char **) 0) {
		if ((*LisUdf = (char **) malloc(2 * sizeof(char *))) == (char **) 0) {
			return -1;
		}
		Indice = 0;
		(*TamLis) = 1;
		(*LisUdf)[0] = strdup(Unidad);
	}
	else if ((Indice = SeekLisUdf(Unidad, *TamLis, *LisUdf)) < 0) {
		Indice = (*TamLis)++;
		if ((*LisUdf = (char **) realloc((void *) *LisUdf, (*TamLis + 1) * sizeof(char *))) == (char **) 0) {
			return -1;
		}
		(*LisUdf)[Indice] = strdup(Unidad);
	}
	(*LisUdf)[*TamLis] = (char *) 0;

	return Indice;
}
