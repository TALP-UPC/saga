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


int safe_strcat(char **dest, const char *src, size_t *dest_size, size_t *dest_strlen)
{
	size_t src_strlen = strlen(src);
	char *new_dest;
	if (dest == NULL) {
		fprintf(stderr, "Error concatenating: Nothing to concatenate\n");
		return -1;
	}
	if (src_strlen == 0) {
		return 0;
	}
	if (*dest_size == 0 || *dest_size-1-*dest_strlen < src_strlen) {
		new_dest = realloc(*dest, 2*(*dest_size+src_strlen));
		if (new_dest == NULL) {
			*dest = NULL;
			return -1;
		}
		*dest = new_dest;
		*dest_size = 2*(*dest_size+src_strlen);
		memset(*dest + *dest_strlen, 0, *dest_size - *dest_strlen);
	}
	strcpy(*dest + *dest_strlen, src);
	*dest_strlen += src_strlen;
	return 0;
}


/* UTF-8 to ISO-8859-1/ISO-8859-15 mapper.
 * Return 0..255 for valid ISO-8859-15 code points, 256 otherwise.
 * http://stackoverflow.com/a/11173493/446149
*/
static inline unsigned int to_latin9(const unsigned int code)
{
    /* Code points 0 to U+00FF are the same in both. */
    if (code < 256U)
        return code;
    switch (code) {
    case 0x0152U: return 188U; /* U+0152 = 0xBC: OE ligature */
    case 0x0153U: return 189U; /* U+0153 = 0xBD: oe ligature */
    case 0x0160U: return 166U; /* U+0160 = 0xA6: S with caron */
    case 0x0161U: return 168U; /* U+0161 = 0xA8: s with caron */
    case 0x0178U: return 190U; /* U+0178 = 0xBE: Y with diaresis */
    case 0x017DU: return 180U; /* U+017D = 0xB4: Z with caron */
    case 0x017EU: return 184U; /* U+017E = 0xB8: z with caron */
    case 0x20ACU: return 164U; /* U+20AC = 0xA4: Euro */
    default:      return 256U;
    }
}

/* Convert an UTF-8 string to ISO-8859-15.
 * All invalid sequences are ignored.
 * Note: output == input is allowed,
 * but   input < output < input + length
 * is not.
 * Output has to have room for (length+1) chars, including the trailing NUL byte.
 * http://stackoverflow.com/a/11173493/446149
*/
size_t utf8_to_latin9(char *const output, const char *const input, const size_t length)
{
    unsigned char             *out = (unsigned char *)output;
    const unsigned char       *in  = (const unsigned char *)input;
    const unsigned char *const end = (const unsigned char *)input + length;
    unsigned int               c;

    while (in < end)
        if (*in < 128)
            *(out++) = *(in++); /* Valid codepoint */
        else
        if (*in < 192)
            in++;               /* 10000000 .. 10111111 are invalid */
        else
        if (*in < 224) {        /* 110xxxxx 10xxxxxx */
            if (in + 1 >= end)
                break;
            if ((in[1] & 192U) == 128U) {
                c = to_latin9( (((unsigned int)(in[0] & 0x1FU)) << 6U)
                             |  ((unsigned int)(in[1] & 0x3FU)) );
                if (c < 256)
                    *(out++) = c;
            }
            in += 2;

        } else
        if (*in < 240) {        /* 1110xxxx 10xxxxxx 10xxxxxx */
            if (in + 2 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U) {
                c = to_latin9( (((unsigned int)(in[0] & 0x0FU)) << 12U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[2] & 0x3FU)) );
                if (c < 256)
                    *(out++) = c;
            }
            in += 3;

        } else
        if (*in < 248) {        /* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 3 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U) {
                c = to_latin9( (((unsigned int)(in[0] & 0x07U)) << 18U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 12U)
                             | (((unsigned int)(in[2] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[3] & 0x3FU)) );
                if (c < 256)
                    *(out++) = c;
            }
            in += 4;

        } else
        if (*in < 252) {        /* 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 4 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U &&
                (in[4] & 192U) == 128U) {
                c = to_latin9( (((unsigned int)(in[0] & 0x03U)) << 24U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 18U)
                             | (((unsigned int)(in[2] & 0x3FU)) << 12U)
                             | (((unsigned int)(in[3] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[4] & 0x3FU)) );
                if (c < 256)
                    *(out++) = c;
            }
            in += 5;

        } else
        if (*in < 254) {        /* 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
            if (in + 5 >= end)
                break;
            if ((in[1] & 192U) == 128U &&
                (in[2] & 192U) == 128U &&
                (in[3] & 192U) == 128U &&
                (in[4] & 192U) == 128U &&
                (in[5] & 192U) == 128U) {
                c = to_latin9( (((unsigned int)(in[0] & 0x01U)) << 30U)
                             | (((unsigned int)(in[1] & 0x3FU)) << 24U)
                             | (((unsigned int)(in[2] & 0x3FU)) << 18U)
                             | (((unsigned int)(in[3] & 0x3FU)) << 12U)
                             | (((unsigned int)(in[4] & 0x3FU)) << 6U)
                             |  ((unsigned int)(in[5] & 0x3FU)) );
                if (c < 256)
                    *(out++) = c;
            }
            in += 6;

        } else
            in++;               /* 11111110 and 11111111 are invalid */

    /* Terminate the output string. */
    *out = '\0';

    return (size_t)(out - (unsigned char *)output);
}
