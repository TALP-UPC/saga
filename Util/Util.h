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

#ifndef	UTIL_H
#define	UTIL_H

#include <time.h>
#include	<limits.h>
#include	<math.h>
#include	<values.h>
#include	<errno.h>

#define	Min(a, b)	(((a) > (b)) ? (b) : (a))
#define	Max(a, b)	(((a) > (b)) ? (a) : (b))
#define	Abs(a)		((a) * Sign(a))
#define	Sign(a)		(((a) > 0.0) ? 1 : -1)


char	**MatStr(const char *);
char	**MatStrChr(const char *, const char *);
int		NovaIden(char ****IdenUdf, int *NumIden, char *Iden);
int		EsExcUdf(char *Unidad, char **ExcUdf);
int		EsIdenUdf(char *Uni1, char *Uni2, char ***IdenUdf);
char	**CreaLisSen(char *FicGui, char **NomFic, char *DirSen, char *ExtSen, int *NumFic);
char	**LeeLisSen(int *NumFic);
void LiberaMatStr(char	**MatChr);
void LiberaLisSen(char	**NomFic, int		NumFic);

#endif						/* defined (UTIL_H)					*/
