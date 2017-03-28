/* Saga - Un transcriptor fonetico para el idioma espanhol
 *
 * Copyright (C) 1993-2009  Albino Nogueiras Rodriguez y Jose B. Marinho
 *       TALP - Universitat Politecnica de Catalunya, ESPANA
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

#include	<limits.h>
#include	<values.h>
#include	<string.h>
#include	<stdlib.h>
#include	<stdio.h>
#include	"LisUdf.h"
#include "PosixCompat.h"
#include	"Util.h"
#include <stdint.h>

/***********************************************************************
 * ReadLisUdf - Lee una lista de unidades foneticas
 **********************************************************************/

size_t		ReadLisUdf(
	const char	*FicLisUdf,	/* Fichero ASCII con las unidades foneticas	*/
	char	***LisUdf)	/* Lista de unidades resultante				*/

{
	char *Unidad = NULL;
	size_t TamUnidad = 0;
	char **tmplis;
	size_t TamLis; /* Numero de unidades en la lista */
	FILE *FpLis;

	/*
	 * Inicializamos la memoria empleada por LisUdf.
	 */
	if ((*LisUdf = malloc(sizeof(char *))) == NULL) {
		return LIS_UDF_ERROR;
	}
	*LisUdf[0] = NULL;

	/*
	 * Abrimos el fichero de la lista.
	 */
	if ((FpLis = fopen(FicLisUdf, "rt")) == NULL) {
		free(*LisUdf);
		return LIS_UDF_ERROR;
	}

	/*
	 * Bucle para todas las unidades contenidas en el fichero.
	 */
	TamLis = 0;
	while (getline(&Unidad, &TamUnidad, FpLis) != -1) {
		TamLis++;
		tmplis = realloc(*LisUdf, (TamLis + 1) * sizeof(char *));
		if (tmplis == NULL) {
			LiberaMatStr(*LisUdf);
			*LisUdf = NULL;
			fclose(FpLis);
			if (Unidad != NULL) free(Unidad);
			return LIS_UDF_ERROR;
		}
		*LisUdf = tmplis;

		if (Unidad[strlen(Unidad) - 1] == '\n') Unidad[strlen(Unidad) - 1] = '\0';

		(*LisUdf)[TamLis-1] = strdup(Unidad);
		(*LisUdf)[TamLis] = NULL;

		if (TamLis == SIZE_MAX-1)
		{
			/* La lista ya tiene SIZE_MAX elementos */
			fprintf(stderr, "La lista de fonemas ha alcanzado su tamanho maximo\n");
			LiberaMatStr(*LisUdf);
			fclose(FpLis);
      free(Unidad);
			return LIS_UDF_ERROR;
		}

	}

  fclose(FpLis);
  if (Unidad != NULL) free(Unidad);
	return 	TamLis;
}

/**
 SeekLisUdf - Busca el indice de una unidad en una lista de unidades
 
 \param Unidad Unidad a buscar
 \param TamLis Tamanho de la lista (Usa 0 para autocalcularlo)
 \param LisUdf Lista de unidades
 \return El indice donde la unidad se encuentra o `LIS_UDF_UNKNOWN`
        si no se encuentra.
*/
size_t		SeekLisUdf(
	char	*Unidad,
	size_t		TamLis,
	char	**LisUdf)
{
	size_t		Udf;

	if (TamLis == 0) TamLis = MatStrLength(LisUdf);

	for (Udf = 0; Udf < TamLis; Udf++) {
		if (strcmp(Unidad, LisUdf[Udf]) == 0) {
			return Udf;
		}
	}

	return LIS_UDF_UNKNOWN;
}

/***********************************************************************
 * MeteLisUdf - Mete una nueva unidad en una lista de unidades foneticas
 **********************************************************************/
size_t MeteLisUdf(
	char	*Unidad,
	size_t		*TamLis,
	char	***LisUdf)
{
	size_t Indice;
  char **tmplis;

	if (*TamLis == LIS_UDF_UNKNOWN) *TamLis = MatStrLength(*LisUdf);

  if (*LisUdf == NULL)
  {
    if ((*LisUdf = malloc(2 * sizeof(char *))) == NULL)
    {
		  return LIS_UDF_ERROR;
    }
		(*LisUdf)[0] = strdup(Unidad);
    (*LisUdf)[1] = NULL;
		(*TamLis) = 1;
		return 0;
  } else if ((Indice = SeekLisUdf(Unidad, *TamLis, *LisUdf)) == LIS_UDF_UNKNOWN)
  {
    Indice = *TamLis;
    if (*TamLis == SIZE_MAX -1)
    {
			fprintf(stderr, "Error: La lista ha alcanzado su tamanho maximo\n");
			LiberaMatStr(*LisUdf);
			*LisUdf = NULL;
			return LIS_UDF_ERROR;
		}
    (*TamLis)++;
    tmplis = realloc(*LisUdf, (*TamLis + 1) * sizeof(char *));
    if (tmplis == NULL)
    {
      LiberaMatStr(*LisUdf);
      *LisUdf = NULL;
      return LIS_UDF_ERROR;
    }
    *LisUdf = tmplis;
		(*LisUdf)[Indice] = strdup(Unidad);
  	(*LisUdf)[*TamLis] = NULL;
	}

	return Indice;
}
