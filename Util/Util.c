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

/*********************************************************************
 * MatStrLength - Devuelve la longitud de la matriz de cadenas
 *
 *********************************************************************/
size_t MatStrLength(char **mat)
{
    size_t len = 0;
    if (mat == NULL) return 0;
    while (mat[len] != NULL) {
        len++;
    }
    return len;
}

/** Concatenate a list of strings NULL terminated on a single string
 *  using `sep` as separator between the strings.
 * 
 *  @param mat A list of strings, NULL terminated
 *  @param sep A string to use as separator in the concatenation
 *  @return a string with the concatenated elements. NULL if error
 **/
char *MatrStr2Str(char **mat, char *sep) {
	size_t total_length = 0;
	size_t i;
	char *output = NULL;
	if (mat == NULL) {
		return NULL;
	}
	/* First we determine the size of the output string. This is the
	 * sum of the length of each of the strings + the length of the
	 * separators */
	i = 0;
	while (mat[i] != NULL) {
		total_length += strlen(mat[i]);
		i++;
	}
	if (sep != NULL) {
		total_length += (i-1)*strlen(sep);
	}
	/* Plus 1 because it has to be 0 ended */
	output = malloc((total_length+1)*sizeof(char));
	if (output == NULL) {
		return NULL;
	}
	output[0] = '\0';
	i = 0;
	while (mat[i] != NULL) {
		strcat(output, mat[i]);
		if (sep != NULL && mat[i+1] != NULL) {
			strcat(output, sep);
		}
		i++;
	}
	return 0;
}

/** Concatenate a list of strings NULL terminated on a single string.
 * 
 *  @param mat A list of strings, NULL terminated
 *  @return a string with the concatenated elements. NULL if error
 **/
char *MatrStr2Str0(char **mat) {
	return MatrStr2Str(mat, NULL);
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
	char	**Mat, **Mat2 = NULL, *Str;
	char *token;
	char	*fStr;
	size_t	i;

    if (_Str == NULL)
    {
        return NULL;
    }
    Str = strdup(_Str);
	if (Str == NULL) {
		return NULL;
	}
    fStr = Str;
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
			free(token);
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


void safe_strcat(char **dest, const char *src, size_t *dest_size, size_t *dest_strlen)
{
	size_t src_strlen = strlen(src);
	char *new_dest;
	if (*dest_size-1-*dest_strlen < src_strlen) {
		new_dest = realloc(*dest, 2*(*dest_size+src_strlen));
		if (new_dest == NULL) {
			*dest = NULL;
			return;
		}
		*dest = new_dest;
		*dest_size = 2*(*dest_size+src_strlen);
		memset(*dest + *dest_strlen, 0, *dest_size - *dest_strlen);
	}
	strcpy(*dest + *dest_strlen, src);
	*dest_strlen += src_strlen;
	return;
}
