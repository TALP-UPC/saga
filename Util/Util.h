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

#ifndef	_UTIL_H
#	define	_UTIL_H

#	include <errno.h>
#	include <time.h>

#	if !defined(_UNISTD_H) && !defined(_MSC_VER)
#		include	<unistd.h>
#	endif

#	if !defined(_LIMITS_H)
#		include	<limits.h>
#	endif

#	if !defined(_MATH_H)
#		include	<math.h>
#	endif

#	if !defined(_VALUES_H)
#		include	<values.h>
#	endif

#	ifndef	_ERRNO_H
#		include	<errno.h>
#	endif

#	ifndef	CLOCKS_PER_SEC
#		ifdef	CLK_TCK
#			define CLOCKS_PER_SEC	CLK_TCK
#		else
#			define CLOCKS_PER_SEC	1000000
#		endif
#	endif

#define	Min(a, b)	(((a) > (b)) ? (b) : (a))
#define	Max(a, b)	(((a) > (b)) ? (a) : (b))
#define	Abs(a)		((a) * Sign(a))
#define	Sign(a)		(((a) > 0.0) ? 1 : -1)

/*****************************************************************
/ LA DEFINICON DE LAS FUNCIONES HAN DE ENTRARSE USANDO EL MACRO  /
/ EXTERN_FUNCTION DE FORMA QUE PUEDAN UTILIZARSE DESDE DISTINTOS /
/ COMPILADORES. VEASE c_varieties.h                              /
 ****************************************************************/

/* Which type of C/C++ compiler are we using? */
#if defined(__cplusplus)
#define __C_tipo 3
#elif defined(__STDC__)
#define __C_tipo 2
#else
#define __C_tipo 1
#endif /* Which type of C/C++ compiler are we using? */


char	**MatStr(char *);
char	**MatStrChr(char *, char *);
int		NovaIden(char ****IdenUdf, int *NumIden, char *Iden);
int		EsExcUdf(char *Unidad, char **ExcUdf);
int		EsIdenUdf(char *Uni1, char *Uni2, char ***IdenUdf);
char	**CreaLisSen(char *FicGui, char **NomFic, char *DirSen, char *ExtSen, int *NumFic);
char	**LeeLisSen(int *NumFic);
void LiberaMatStr(char	**MatChr);
void LiberaLisSen(char	**NomFic, int		NumFic);

#endif						/* defined (_UTIL_H)					*/
