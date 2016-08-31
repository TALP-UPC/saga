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

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	"Util.h"


/***********************************************************************
 * LiberaMatStr - Libera una lista de strings acabados en 0 
 **********************************************************************/

void	LiberaMatStr(
	char	**matStr)
{
	size_t i;
	if (matStr == NULL) {
		return;
	}
	for (i=0; matStr[i] != NULL; i++)
		free(matStr[i]);
	free(matStr);
	return;
}


/***********************************************************************
 * MatStr - Construye una matriz de cadenas a partir de una cadena 
 * separada en "tokens" por comas y/o espacios.
 **********************************************************************/

char	**MatStr(const char	*Str)
{
	return MatStrChr(Str, " ,");
}

/***********************************************************************
 * MatStr - Construye una matriz de cadenas a partir de una cadena 
 * separada en "tokens" por comas y/o espacios.
 **********************************************************************/

char	**MatStrChr(
	const char	*_Str,
	const char	*Delim)

{
	char	**Mat, **Mat2 = NULL, *Str = strdup(_Str);
	char *token;
	char	*fStr = Str;
	size_t	i;

	if (Str == NULL) {
		return NULL;
	}

	/* Allocate space for the final NULL */
	if ((Mat = malloc(sizeof(char *))) == NULL) {
		free(fStr);
		return NULL;
	}

	for (i = 0; (token = strtok(Str, Delim)) != NULL; i++) {
		token = strdup(token);
		if (token == NULL) {
			free(Mat);
			free(fStr);
			return NULL;
		}
		/* We have a new token. Allocate space for it */
		Mat2 = realloc(Mat, (i + 2) * sizeof(char *));
		if (Mat2 == NULL) {
			free(fStr);
			free(Mat);
			return NULL;
		} else { /* realloc successful */
			Mat = Mat2;
			Mat2 = NULL;
		}
		/* Store the token */
		Mat[i] = token;
		/* All the following calls to strtok continue on the same str */
		Str = NULL;	/* Para la siguiente llamada a strtok	*/
	}

	Mat[i] = NULL;
	free(fStr);

	return Mat;
}
