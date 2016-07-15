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

#ifndef	_LIS_UDF_H
#	define	_LIS_UDF_H

int		ReadLisUdf(
	char	*FicLisUdf,	/* Fichero ASCII con las unidades foneticas	*/
	char	***LisUdf);	/* Lista de unidades resultante				*/
int		SeekLisUdf(
	char	*Unidad,
	int		TamLis,
	char	**LisUdf);
int		MeteLisUdf(
	char	*Unidad,
	int		*TamLis,
	char	***LisUdf);

#endif						/* defined (_LIS_UDF_H)					*/
