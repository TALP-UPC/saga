/* coding: utf-8 */
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

/*
 * Palabras no acentuadas.
 */
static char	*ExcAcc[] = {
	/* articulos determinados	*/
	"el", "la", "los", "las",
	/* contracciones			*/
	"del", "al",
	/* posesivos				*/
	"mi", "tu", "su", "mis", "tus", "sus",
	"nues-tro", "vues-tro", "nues-tros", "vues-tros",
	"nues-tra", "vues-tra", "nues-tras", "vues-tras",
	/* pronombres personales	*/
	"me", "te", "se", "nos", "os", "lo", "le", "les",
	/* pronombres relativos		*/
	"que", "quien", "quie-nes", "cu-yo", "cu-ya", "cu-yos", "cu-yas",
	"cuan-to", "cuan-ta", "cuan-tos", "cuan-tas",
	/* adverbios				*/
	"me-dio", "co-mo", "cuan-do", "don-de", "a-don-de", "cuan", "tan",
	/* preposiciones			*/
	"a", "an-te", "ba-jo", "ca-be", "con", "con-for-me a", "con-tra",
	"de", "des-de", "du-ran-te", "en", "en-tre", "fren-te a", "ha-cia",
	"has-ta", "jun-to a", "me-dian-te", "pa-ra", "por", "res-pec-to a",
	"sin", "so", "so-bre", "tras",
	/* conjunciones				*/
	"aun cuan-do", "aun-que", "co-mo que", "co-mo si", "con-for-me",
	"con-que", "lue-go", "mas", "mien-tras", "ni", "pe-ro", "por-que",
	"pues", "pues-to que", "que", "si-no", "si",
	"y", "e", "o", "u",
	/* particulas incluyentes	*/
	 "aun", "has-ta", "in-clu-so", "ex-cep-to", "me-nos", "sal-vo",

	 NULL
};
